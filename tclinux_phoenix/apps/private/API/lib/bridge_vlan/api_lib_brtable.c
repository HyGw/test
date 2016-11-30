#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <errno.h>
#include <asm/param.h>
#include <linux/version.h>
#include <ecnt_lib_dbg.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <netinet/in.h>
#include <libapi_lib_flowmgr.h>
#include <linux/if_bridge.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include "libapi_lib_brtable.h"

#include <modules/eth_global_def.h>
#include <pthread.h>

#if defined(TCSUPPORT_FWC_ENV)
#define LAN_IF	"eth"
#else
#define LAN_IF	"eth0"
#endif

#define BRTABLE_MAX_SIZE	2048

struct fdb_entry brtable_port[4][BRTABLE_MAX_SIZE] ; /*for fdb entry from kernel and switch. for 0~3*/
int port_count[4] ;
pthread_mutex_t sync_brtable_lock ;
u8 multi_thread_mode = 0 ; /*if the sync thread is called , will return merged mac table directly*/

static inline int Switch_API_Entrance(mt7530_switch_api_cmdid cmdid, u32 port, void *paramext_ptr)
{
	int skfd;
	struct ifreq ifr;
	mt7530_switch_api_params api_param;

	/* Open a basic socket.*/
	if ((skfd = socket(AF_INET, SOCK_DGRAM,0)) < 0) 
	{
		ECNT_LIB_BRTABLE_PRINT_ERROR("socket create error!\n");
		return ECNT_E_BRTABLE_SOCK_FAIL;
	}
	
	strncpy(ifr.ifr_name, LAN_IF, IFNAMSIZ);
	if (ioctl(skfd, SIOCGMIIPHY, &ifr) < 0) 
	{
		ECNT_LIB_BRTABLE_PRINT_ERROR("ioctl error!\n");
		return ECNT_E_BRTABLE_SOCK_FAIL;
	}

	ifr.ifr_data = (void *)(&api_param);
	api_param.cmdid = cmdid;
	api_param.PortQueueId = port;
	api_param.paramext_ptr = paramext_ptr;

	if (ioctl(skfd, RAETH_GSW_CTLAPI, &(ifr)) < 0)
	{
		ECNT_LIB_BRTABLE_PRINT_ERROR("ioctl error!\n");
		return ECNT_E_BRTABLE_SOCK_FAIL;
	}

	if (api_param.ret_value == -1)
	{
		usleep(10000);
	}
	
	close(skfd);
	return (api_param.ret_value);
}

static inline int get_brtable_from_switch(brtable_for_switch *outptr_brtable, u8 port)
{
    return Switch_API_Entrance(CMD_GET_BR_TABLE, port, (void *)(outptr_brtable));
}

static inline int merge_brtable_switch2kernel(brtable_for_switch *brtable , struct fdb_entry *entry ,int offset ,int max_num )
{
	int i , n ;
	int fdb_count = 0 ;
	int match_if = 0 ;

	fdb_count = offset ;/*record the size of kernel mac table*/	
	for( i = 0 ; i < brtable->count ; i++ )
	{
		if( offset == max_num)/*reach the max size,  stop the merge action*/
			break ;
		
		for( n = 0 ; n < fdb_count ; n++ )
		{
			if( (entry[n].vlan_layer == 0 && brtable->macinfo[i].vid == 0xFFFF)/*untagged*/
				||(entry[n].vlan_layer == 1 && brtable->macinfo[i].vid == entry[n].vlan_id) )/*tagged*/
			{
				if( (entry[n].mac_addr[0] == brtable->macinfo[i].mac[0])  
					&& (entry[n].mac_addr[1] == brtable->macinfo[i].mac[1])
					&& (entry[n].mac_addr[2] == brtable->macinfo[i].mac[2])
					&& (entry[n].mac_addr[3] == brtable->macinfo[i].mac[3])
					&& (entry[n].mac_addr[4] == brtable->macinfo[i].mac[4])
					&& (entry[n].mac_addr[5] == brtable->macinfo[i].mac[5]) )
				{
					match_if = 1 ;		/*match the same fdb entry*/
					break ;
				}
			}
		}

		if(match_if == 0)/*not match , add to kernel mac table*/
		{
			/* copy action*/
			memcpy(entry[offset].mac_addr, brtable->macinfo[i].mac, 6);
			entry[offset].port_no = brtable->macinfo[i].port ;
			entry[offset].ageing_timer_value.tv_sec = brtable->macinfo[i].timer ;
			if( brtable->macinfo[i].vid == 0xFFFF )
			{
				entry[offset].vlan_id = entry[offset].vlan_layer = 0;
			}
			else
			{
				entry[offset].vlan_layer = 1;
				entry[offset].vlan_id = brtable->macinfo[i].vid ;
			}
			offset++ ;
		}
		match_if = 0 ;
	}

	return offset ;	
}

static inline void __jiffies_to_tv(struct timeval *tv, unsigned long jiffies)
{
	unsigned long long tvusec;

	tvusec = (1000000ULL*jiffies)/HZ;
	tv->tv_sec = tvusec/1000000;
	tv->tv_usec = tvusec - 1000000 * tv->tv_sec;
}

static inline void __copy_fdb(struct fdb_entry *ent, 
			      const struct __fdb_entry *f)
{
	memcpy(ent->mac_addr, f->mac_addr, 6);
	ent->port_no = f->port_no;
	ent->is_local = f->is_local;
	ent->vlan_id  = f->vlan_id & 0xFFF;
	ent->vlan_layer = f->vlan_layer;
	__jiffies_to_tv(&ent->ageing_timer_value, f->ageing_timer_value);
}

int br_read_fdb(const char *bridge, struct fdb_entry *fdbs, 
		int *fdb_num, int num, int br_socket_fd, int index)
{
	int i, fd = -1, n;
	struct __fdb_entry fe[num];

	/* old kernel, use ioctl */
	unsigned long args[5] = { BRCTL_GET_FDB_VLAN_ENTRIES,
				  (unsigned long) fe,
				  num, fdb_num, index};
	struct ifreq ifr;
	int retries = 0;

	strncpy(ifr.ifr_name, bridge, IFNAMSIZ);
	ifr.ifr_data = (char *) args;

retry:
	n = ioctl(br_socket_fd, SIOCDEVPRIVATE, &ifr);

	/* table can change during ioctl processing */
	if (n < 0 && errno == EAGAIN && ++retries < 10) {
		sleep(0);
		goto retry;
	}

	for (i = 0; i < n; i++) 
		__copy_fdb(fdbs+i, fe+i);

	if (fd > 0)
		close(fd);
	
	return n;
}

ECNT_LIB_BRTABLE_RET br_get_brtable(int index, struct fdb_entry *entry, int *num)
{
	const char *brname = NULL;
	const char *if_name = NULL;
#define CHUNK 128
	int i, n;
	struct fdb_entry *fdb = NULL;
	int offset = 0;
	int fdb_num = 0;
    int br_socket_fd = -1;
	
	brtable_for_switch *brtable = NULL;
	brtable = (brtable_for_switch *)malloc(sizeof(brtable_for_switch));
	if ( !brtable )
		return -1;
	bzero(brtable, sizeof(brtable_for_switch));

	assert((entry != NULL) && (num != NULL));
	if((index < E_ETH_PORT_0) || (index > E_MAX_PORT-1)) {
		ECNT_LIB_BRTABLE_PRINT_ERROR("port index error!\n");
		return ECNT_E_BRTABLE_PARA_FAIL;
	}
	
	if ((br_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		ECNT_LIB_BRTABLE_PRINT_ERROR("socket create error!\n");
		return ECNT_E_BRTABLE_SOCK_FAIL;
	}

	brname  = BRIDGE_LAN;
	for(;;) {
		fdb = realloc(fdb, (offset + CHUNK) * sizeof(struct fdb_entry));
		if (!fdb) {
			fprintf(stderr, "Out of memory\n");
			ECNT_LIB_BRTABLE_PRINT_ERROR("Out of memory!\n");
			close(br_socket_fd);
			return ECNT_E_BRTABLE_FAIL;
		}
		n = br_read_fdb(brname, fdb+offset, &fdb_num, CHUNK, br_socket_fd, index);
		if (n == 0)
			break;

		if (n < 0) {
			fprintf(stderr, "read of forward table failed: %s\n",
				strerror(errno));
			ECNT_LIB_BRTABLE_PRINT_ERROR("read of forward table failed!\n");
			free(fdb);
			close(br_socket_fd);
			return ECNT_E_BRTABLE_FAIL;
		}

		offset += n;
	}

	if(*num >= offset) {
		memcpy(entry, fdb, offset * sizeof(struct fdb_entry));
		/*read the switch mac table , and merge to kernel brtable*/
		if( index < E_WIFI_PORT_0 )
		{
			get_brtable_from_switch(brtable, index);
			offset = merge_brtable_switch2kernel(brtable,entry,offset,*num);
		}
		*num = offset;
	}else {
		memcpy(entry, fdb, *num * sizeof(struct fdb_entry));
	}

	free(brtable);
	brtable = NULL;
	free(fdb);
	close(br_socket_fd);
	
	return ECNT_E_BRTABLE_SUCCESS;
}

 
 /*
	 description: get bridge foward table support vlan
 
		 input
				  index: index of port
				  entry: address of storing the bridge foward table
				  num  : the num expect avalible
 
			output
				   zero: success
			   non zero: fail
 
 */
 ECNT_LIB_BRTABLE_RET brmgr_lib_get_brtable(int index, struct fdb_entry *entry, int *num)
 {
	if( (multi_thread_mode==1)  && (index < E_WIFI_PORT_0) )/*sync mode is open ,return sync result directly*/
	{
		pthread_mutex_lock(&sync_brtable_lock);
		if( (*num) > port_count[index] )
			*num = port_count[index] ;
		memcpy(entry, brtable_port[index], (*num) * sizeof(struct fdb_entry));
		pthread_mutex_unlock(&sync_brtable_lock);
	}
	else
	{
		return br_get_brtable(index , entry , num) ;
	}
	 
	return ECNT_E_BRTABLE_SUCCESS;
 }

 void *syncBrtableTask()
 { 
 	int ret = 0 ;
	/*init sync_brtable_lock*/
	ret = pthread_mutex_init(&sync_brtable_lock,NULL);
	if(ret != 0)
	{
		ECNT_LIB_BRTABLE_PRINT_ERROR("\r\nMutex value_change_lock initialization fail!");
		return ;
	}
	multi_thread_mode = 1 ;
	while(1)
	{
		pthread_mutex_lock(&sync_brtable_lock);/*lock*/
		bzero(brtable_port[0], BRTABLE_MAX_SIZE*sizeof(struct fdb_entry));
		bzero(brtable_port[1], BRTABLE_MAX_SIZE*sizeof(struct fdb_entry));
		bzero(brtable_port[2], BRTABLE_MAX_SIZE*sizeof(struct fdb_entry));
		bzero(brtable_port[3], BRTABLE_MAX_SIZE*sizeof(struct fdb_entry));
		port_count[0] = port_count[1] = port_count[2] = port_count[3] = BRTABLE_MAX_SIZE  ;
		br_get_brtable(0 , brtable_port[0] , &port_count[0]) ;
		br_get_brtable(1 , brtable_port[1] , &port_count[1]) ;
		br_get_brtable(2 , brtable_port[2] , &port_count[2]) ;
		br_get_brtable(3 , brtable_port[3] , &port_count[3]) ;
		pthread_mutex_unlock(&sync_brtable_lock);/*unlock*/
		
		sleep(5);
	}
 }
 
 ECNT_LIB_BRTABLE_RET brmgr_lib_sync_brtable_start()
 {
	 int ret;
	 pthread_t p_syncHandle;
	 pthread_attr_t p_syncHandle_attr;
 
	 ret = pthread_attr_init(&p_syncHandle_attr);
	 if(ret != 0)
	 {
		 ECNT_LIB_BRTABLE_PRINT_ERROR("\r\nThread(inform) attribute creation fail!\n");
		 return ECNT_E_BRTABLE_FAIL;
	 }
 
	 ret = pthread_attr_setdetachstate(&p_syncHandle_attr,PTHREAD_CREATE_DETACHED);
	 if(ret != 0)
	 {
		 ECNT_LIB_BRTABLE_PRINT_ERROR("\r\nThread(inform):Set attribute fail!");
		 return ECNT_E_BRTABLE_FAIL;
	 }
 
	 ret = pthread_create(&p_syncHandle,&p_syncHandle_attr,(void *)syncBrtableTask,NULL);
	 if(ret != 0)
	 {
		 ECNT_LIB_BRTABLE_PRINT_ERROR("\r\nThread(inform):Create thread fail!");
		 return ECNT_E_BRTABLE_FAIL;
	 }
	 
	 pthread_attr_destroy(&p_syncHandle_attr);
	 return ECNT_E_BRTABLE_SUCCESS ;
 }

 int br_set(const char *bridge, const char *name,
		  unsigned long value, unsigned long oldcode, int fd)
{
	int ret;
    struct ifreq ifr;
	unsigned long args[4] = { oldcode, value, 0, 0 };
	
	strncpy(ifr.ifr_name, bridge, IFNAMSIZ);
	ifr.ifr_data = (char *) &args;
	ret = ioctl(fd, SIOCDEVPRIVATE, &ifr);
	

	return ret < 0 ? errno : 0;
}

unsigned long __tv_to_jiffies(const struct timeval *tv)
{
	unsigned long long jif;

	jif = 1000000ULL * tv->tv_sec + tv->tv_usec;

	return (HZ*jif)/1000000;
}

int br_set_ageing_time(const char *br, struct timeval *tv, int fd)
{
	return br_set(br, "ageing_time", __tv_to_jiffies(tv),
		      BRCTL_SET_AGEING_TIME, fd);
}

/*
	description: age time modify

		input
	        	age time (s)

	       output
	       		  zero: success
	       	  non zero: fail	
*/
ECNT_LIB_BRTABLE_RET brmgr_lib_set_brtable_age(double time) {
	int err;
	struct timeval tv;
	int br_socket_fd = -1;

	if ((br_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		ECNT_LIB_BRTABLE_PRINT_ERROR("socket create error!\n");
		return ECNT_E_BRTABLE_SOCK_FAIL;
	}

	tv.tv_sec = time;
	tv.tv_usec = 1000000 * (time - tv.tv_sec);
	
	err = br_set_ageing_time(BRIDGE_LAN, &tv, br_socket_fd);
	if (err) {
		fprintf(stderr, "set ageing time failed: %s\n",
			strerror(err));
		close(br_socket_fd);
		return ECNT_E_BRTABLE_FAIL;
	}

	close(br_socket_fd);
	
	return ECNT_E_BRTABLE_SUCCESS;
}


ECNT_LIB_BRTABLE_RET brmgr_lib_clean_brtable(void)
{
	int err;
	int br_socket_fd = -1;
	const char * brname = NULL;
	int n = 0;
	
	if ((br_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		ECNT_LIB_BRTABLE_PRINT_ERROR("socket create error!\n");
		return ECNT_E_BRTABLE_SOCK_FAIL;
	}

	/* old kernel, use ioctl */
	unsigned long args[4] = { BRCTL_CLEAN_FDB_VLAN_ENTRIES,
				  (unsigned long) NULL,
				  0, 0};
	struct ifreq ifr;
	
	brname  = BRIDGE_LAN;
	strncpy(ifr.ifr_name, brname, IFNAMSIZ);
	ifr.ifr_data = (char *) args;

	n = ioctl(br_socket_fd, SIOCDEVPRIVATE, &ifr);
	if (n < 0) {
		fprintf(stderr, "clean brtable failed: %s\n",
			strerror(err));
		close(br_socket_fd);
		return ECNT_E_BRTABLE_FAIL;
	}

	close(br_socket_fd);

	return ECNT_E_BRTABLE_SUCCESS;
	
}


