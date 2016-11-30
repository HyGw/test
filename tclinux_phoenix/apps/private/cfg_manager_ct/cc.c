/******************************************************************************/
/*
 * Copyright (C) 1994-2008 TrendChip Technologies, Corp.
 * All Rights Reserved.
 *
 * TrendChip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of TrendChip Technologies, Corp. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * TrendChip Technologies, Corp.
 */
/******************************************************************************/
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <stdlib.h>
//#include <linux/in.h>
//#include <linux/in6.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <pthread.h>
#include <netdb.h>
#include "cfg_manager.h"
#include "cc.h"
#include "utility.h"
#include "../lib/libtcapi.h"
#include "wan_monitor.h"
#if defined(TCSUPPORT_AUTO_DMZ)
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <linux/if_packet.h> 
#include <sys/socket.h>
#endif
#include "libcompileoption.h"

#if defined(TCSUPPORT_CT_PPPOEPROXY)
#include <linux/if_ether.h>
#define PPPOEPROXYOFFSET 0
#endif

#if defined(TCSUPPORT_CT_QOS)
void changeCongestionMode(void);
#endif

#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)		
void check_ctIppingDiagnosatic(void);
int recheckPingDiagnostic_flag = 0;
#endif
#if defined(TCSUPPORT_CT_BUTTONDETECT)
	int devfd = 0;	
#endif

#if defined(TCSUPPORT_CT_PPPOE_EMULATOR)
extern pthread_t PppoeEmulatorBridgeThread_id;
extern mxml_node_t **gTreePtr;
#endif
#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
extern int checkPPPoE_Emulator_timer();
#endif
#if defined(TCSUPPORT_CT_L2TP_VPN)
extern int checkVPNPPPTunnels(int ponState);
#endif

void upgrade_firmware();
void check_wifi(void);
void check_pppoeproxy(void);

extern int isbootset;
#if defined(TCSUPPORT_SIMPLE_UPNP)
void restart_upnp(mxml_node_t *top, char *if_name, int if_index);
extern int write_upnp_conf(mxml_node_t *top);
#endif

/*global data of autopvc*/
extern pvc_pool_t g_pvcPool[];
extern vc_pool_t g_vcPool[];
extern int g_vcPool_encap;
extern int g_dhcprelay;
#if defined(TCSUPPORT_CT)
extern unsigned int ntpdelayflag1;
extern unsigned int ntpdelayflag2;
unsigned int ntpnumber = 0;
char ntpneedexecute = NTPFLAGWITHEXECUTE;/*0:no need to execute ntp again; 1:need to execute ntp(boot);2:need to execute ntp(sync in peridic time)*/
#define NTPDELAYTIME 5	/*max count after wan side up and get ip*/
unsigned long ntpSynchroNb = 0;
#define MAX_NTP_SYNC_TIME 24*60*60
extern int g_wan_boot_finish;
#define NTPTRYMAXNUMBER 10
#define NTPTRYTIME 60
#define NTPTRYMINTIME 30
#endif
int autopvc_state=0;
char fork_or_not[PVC_NUM];/*wan interface state*/  /*rodney_20090420 add*/
unsigned int pvc_bit_map=0,vcpool_bit_map=0;
#ifdef IPV6
char g_wan_ipv6_active[MAX_WAN_IF_INDEX] = {0};
#endif
#if defined(TCSUPPORT_CT_DNSMASQ_MONITOR)
#define DNS_CHECK_TIME 60
extern int init_dnsmasq_mutex(void);
#endif
#if defined(TCSUPPORT_CT_DSLITE)
extern void route_switch(mxml_node_t *top, char *route_index, int state);
#if defined(TCSUPPORT_PORTBIND)
extern void set_portbinding_info_for_dslite(int pvc_index, int entry_index);
#endif
#endif
#if defined(TCSUPPORT_CT_BRIDGEARP_NOFWD_LAN)
extern int AddBridgeArpRules ();
#endif
#if defined(TCSUPPORT_CT_PON)
#if !defined(TCSUPPORT_CUC) && !defined(TCSUPPORT_CMCC)
extern int setPonState(int state);
extern int getPonState();
#endif
#endif

#if (VOIP == 1)
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
#define MAX_DSP_LINE 4
#define SIMULATE_TEST_NORMAL_TIMEOUT  (40)
int line_error_restartsip_timer_cnt[MAX_DSP_LINE] = {0};
extern int sipStatusFlag;
extern int VOIPLineNum;
static int restart_sip_when_sip_down(void);
static void restart_sip_when_line_unreg(void);
int voipStatUpgrade(void);
int voipIdleProcess(void);
#endif
#endif

#if defined(TCSUPPORT_CT_DSL_EX) && defined(TCSUPPORT_CT_WAN_PTM)
extern int g_adsl_state;
int g_dsldetect = 0, g_dslstate_chg = 0, g_dslstate_up = 0;
#endif
#if defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505)
/*
check is TR069 firmware upgrade
*/
int isTR69FWUpgrade()
{
	char upgradeVal[64] = {0};

	if( TCAPI_PROCESS_OK ==
		tcapi_get("WebCurSet_Entry", "cwmpFWUpgrade", upgradeVal)
		&& 0 == strcmp("Yes", upgradeVal) )
		return 1;
	else
		return 0;
}
#endif

#ifdef IPV6
extern int isWan6UP(int iface_index);

void set_cc_wan_state(int ifindex, char is_active)
{
	if(ifindex < 0 || ifindex >= MAX_WAN_IF_INDEX)
		return;
	g_wan_ipv6_active[ifindex] = is_active;
}

void shrink_ipv6addr(char *in_ipv6addr, char *out_ipv6addr, int out_len)
{
	struct sockaddr_in6 ipv6addr;

	inet_pton(AF_INET6, in_ipv6addr, (struct sockaddr *) &ipv6addr.sin6_addr);
	ipv6addr.sin6_family = AF_INET6;

	getnameinfo((struct sockaddr *) &ipv6addr, sizeof(struct sockaddr_in6), out_ipv6addr, out_len, NULL, 0, NI_NUMERICHOST);
	return;
}

void check_ipv6_gateway()
{
	int i, fd, ret;
	char path[64] = {0}, gw_addr[64] = {0}, slaac_addr[64] = {0}, prefix_len[5] = {0};
	char cmd[128] = {0}, tmp[64] = {0},tmp2[64] = {0};
	char dhcpv6_active[5] = {0}, ifname[16] = {0}, gw6_manual[5] = {0};
	int pvc_index = 0, entry_index = 0, need_commit = 0;

	for(i = 0; i < MAX_WAN_IF_INDEX; i++){
		if(g_wan_ipv6_active[i]){
			pvc_index = i / MAX_SMUX_NUM;
			entry_index = i % MAX_SMUX_NUM;
			memset(gw_addr, 0, sizeof(gw_addr));
			memset(slaac_addr, 0, sizeof(slaac_addr));
			memset(ifname, 0, sizeof(ifname));
			memset(dhcpv6_active, 0, sizeof(dhcpv6_active));
			memset(gw6_manual, 0, sizeof(gw6_manual));
			sprintf(tmp, "Wan_PVC%d_Entry%d", pvc_index, entry_index);
			tcapi_get(tmp, "IFName", ifname );
			tcapi_get(tmp, "DHCPv6", dhcpv6_active);
			tcapi_get(tmp, "GW6_Manual", gw6_manual);
			need_commit = 0;	

			/*check ipv6 gateway address if not manual mode*/
			if(strcmp(gw6_manual, "Yes") && 0 == isWan6UP(i)){			
				sprintf(path, "/proc/sys/net/ipv6/neigh/%s/default_route", ifname);
				fd=open(path, O_RDONLY|O_NONBLOCK);
				if(fd!=-1){
					memset(tmp, 0, sizeof(tmp));
					ret = read(fd, tmp, 39);
					close(fd);
					if(ret > 1 && strcmp(tmp, "")){
						shrink_ipv6addr(tmp, gw_addr, sizeof(gw_addr));
						
						sprintf(tmp, "WanInfo_Entry%d", i);
						if(!(0 == tcapi_get(tmp, "GateWay6", tmp2) && !strcmp(tmp2, gw_addr))){
							//tcdbg_printf("check_ipv6_gateway:%s get gateway: %s\n", ifname, gw_addr);

							sprintf(cmd, "echo \"%s\" > /var/run/%s/gateway6", gw_addr, ifname);					
							system(cmd);
							need_commit = 1;
						}
						sprintf(cmd, "echo %s > /proc/sys/net/ipv6/neigh/%s/default_route", "", ifname);
						system(cmd);					
					}									
				}
			}
			
			/*check slaac address*/
			if(!strcmp(dhcpv6_active, DEACTIVE)){				
				sprintf(path, "/proc/sys/net/ipv6/conf/%s/slaac_addr", ifname);
				fd=open(path, O_RDONLY|O_NONBLOCK);
				
				if(fd!=-1){					
					memset(tmp, 0, sizeof(tmp));
					ret = read(fd, tmp, 64);					
					close(fd);
					if(ret > 1 && strcmp(tmp, "")){			
						
						memset(tmp2, 0, sizeof(tmp2));
						sscanf(tmp, "%39s %s", tmp2, prefix_len);						
						shrink_ipv6addr(tmp2, slaac_addr, sizeof(slaac_addr));
						//tcdbg_printf("check_ipv6_gateway:%s get slaac: %s, prefixlen:%s\n", ifname, slaac_addr, prefix_len);
						/*Check slaac address or prefix len been changed or not*/
						sprintf(tmp, "WanInfo_Entry%d", i);
						memset(tmp2, 0, sizeof(tmp2));
						tcapi_get(tmp, "IP6", tmp2);
						if(strcmp(tmp2, slaac_addr))
							need_commit = 1;
						
						memset(tmp2, 0, sizeof(tmp2));
						tcapi_get(tmp, "PrefixLen6", tmp2);						
						if(strcmp(tmp2, slaac_addr))
							need_commit = 1;
						
						if(need_commit){
							sprintf(cmd, "echo \"%s\" > /var/run/%s/ip6", slaac_addr, ifname);
							system(cmd);
							sprintf(cmd, "echo \"%s\" > /var/run/%s/prefix6", prefix_len, ifname);
							system(cmd);
						}
						sprintf(cmd, "echo %s > /proc/sys/net/ipv6/conf/%s/slaac_addr", "", ifname);
						system(cmd);
						
					}
				}
			}
			
			if((strlen(gw_addr) > 1 || strlen(slaac_addr) > 1) && need_commit){
				tcdbg_printf("check_ipv6_gateway:commit %s\n", ifname);
				sprintf(cmd, "echo %s > /var/run/%s/status6", "up", ifname);
				system(cmd);
				sprintf(cmd, "WanInfo_Message_%s/6", ifname);
				tcapi_commit(cmd);
			}
		}
	}
}
#endif

//#ifdef IPV6
#if 0
static int
writeAppendFile(char *buf, char *pathname){
	FILE *fp=NULL;
	fp = fopen(pathname, "a");

	if(fp == NULL){
		return FAIL;
	}

	fputs(buf,fp);
	fclose(fp);
	return SUCCESS;
}

/*______________________________________________________________________________
**function name:checkAndWriteDNSConfFile
**
**description:
*    Check interface status
**parameters:
*    char *SrcFile -- source file to read from
*    char *DestFile -- destination file to write into (dnsmasq.conf ...)
*    char *IfBuf -- Interface selection
**global:
*    none
**return:
*    SUCCESS: operation success
*    FAIL: error occurs
**call:
*    none
**revision:
*     1.rclv_nj 20100526
**____________________________________________________________________________*/
static int checkAndWriteDNSConfFile(char *SrcFile, char *DestFile, char *IfBuf)
{
	int fd = 0, num = 0;
	char buf[256] = {0};
	char dst[256] = {0};
	int len = 0;
	char *ptr_buf = NULL;

	int ret = 0;

	char tmpIPv6Addr[48] = {0}; 
	
	fd = open(SrcFile,O_RDONLY|O_NONBLOCK);
	if(fd<0)
		return FAIL;

	num = read(fd, buf , sizeof(buf)-1 );
	close(fd);
	if(num <= 0)
		return FAIL;


	tcdbg_printf("%s\n",buf); 
		
	ptr_buf = strstr(buf,"nameserver=");
			
	while(ptr_buf != NULL)
	{
		ptr_buf += strlen("nameserver=") -1;
		*ptr_buf = ' ';
		*ptr_buf ++;

		memset( tmpIPv6Addr, 0, sizeof(tmpIPv6Addr) );
		sscanf(ptr_buf , "%s", tmpIPv6Addr);
		len = strlen(dst);
		sprintf(dst+len, "server=%s@%s\n", tmpIPv6Addr, IfBuf);
			
		ptr_buf = strstr(buf,"nameserver=");

	}

		
	ret = writeAppendFile(dst, DestFile);
		
	
	if(SUCCESS == ret)
	{
		system("killall -9 dnsmasq");
		system("/userfs/bin/dnsmasq");
	}
	
	return ret;
	
}/* end checkAndWriteDNSConfFile */

/*______________________________________________________________________________
**function name:checkAndWriteRadvdDhcp6sConf
**
**description:
*    Check interface status
**parameters:
*    char *SrcFile -- source file to read from
**global:
*    none
**return:
*    SUCCESS: operation success
*    FAIL: error occurs
**call:
*    none
**revision:
*     1.rclv_nj 20100526
**____________________________________________________________________________*/
static int checkAndWriteRadvdDhcp6sConf(char *SrcFile)
{
	int fd = 0, num = 0;
	char buf[548] = {0};
	char cmd[128] = {0};
	char prefixPlen[50] = {0};
	char * ptr = NULL;
	char enableFlag[3] = {0};
	char modeFlag[3] = {0};

	fd = open(SrcFile,O_RDONLY|O_NONBLOCK);
	if(fd<0)
		return FAIL;

	num = read(fd, buf , sizeof(buf) );
	close(fd);
	if(num <= 0)
		return FAIL;

	ptr = strstr(buf, "prefix-delegation");
	if(NULL == ptr)
	{
			
		tcdbg_printf("prefix-delegation NOT found! \n");
		return FAIL;
	}
		
	if( NULL != ptr )
	{

		sscanf(ptr+strlen("prefix-delegation") , "%s",prefixPlen);
		ptr = prefixPlen;

		while( *ptr != '/' )
			ptr++;

		*ptr = '\0';
		ptr++;


		memset(cmd, 0, 128);

		if(
			( 0 == tcapi_get("Radvd_Entry", "Enable", enableFlag) ) &&
			( 0 == tcapi_get("Radvd_Entry", "Mode", modeFlag) )
		)
		{
			if( ( '1' == enableFlag[0] ) && ( '0' ==  modeFlag[0] ) )
			{
				tcapi_set("Radvd_Entry", "PrefixIPv6", prefixPlen);
				tcapi_set("Radvd_Entry", "Prefixv6Len", ptr);
				tcapi_commit("Radvd_Entry");
				tcdbg_printf("radvd.conf set success!\n") ;
			}
								
		}
		memset(enableFlag, 0, sizeof(enableFlag) );
		memset(modeFlag, 0, sizeof(modeFlag) );

		if( 
			( 0 == tcapi_get("Dhcp6s_Entry", "Enable", enableFlag) ) &&
			( 0 == tcapi_get("Dhcp6s_Entry", "Mode", modeFlag) )
		)
		{
			if( ( '1' == enableFlag[0] ) && ( '0' == modeFlag[0]) )
			{
				tcapi_set("Dhcp6s_Entry", "PrefixIPv6", prefixPlen);
				tcapi_set("Dhcp6s_Entry", "Prefixv6Len", "64");
				tcapi_commit("Dhcp6s_Entry");
				tcdbg_printf("dhcp6s.conf set success!\n") ;
			}
		}
		return SUCCESS;
	
	}
	
	return FAIL;
		
}/* end checkAndWriteRadvdDhcp6sConf*/


/*______________________________________________________________________________
**function name:checkInterfaceStatus
**
**description:
*    Check interface status
**parameters:
*    char *if_dev -- name of the interface
**global:
*    none
**return:
*    0 : success
*    -1 : device not working
*    -2 : tmp file read error
**call:
*    none
**revision:
*     1.rclv_nj 20100526
**____________________________________________________________________________*/
static int	checkInterfaceStatus (char * if_dev)
{
	FILE * fp = NULL;
	char cmd[32]={0};
	char tmpBuf[512]={0};
	char *pValue = NULL;

	//static int flag = 0;
	unlink(TMP_CHECK_IF);
	sprintf(cmd,"ifconfig %s > %s", if_dev, TMP_CHECK_IF);
	system(cmd);

		fp=fopen(TMP_CHECK_IF, "r");
		if(fp==NULL){
			unlink(TMP_CHECK_IF);
			return -2;
		}
		else
		{
			while (fgets (tmpBuf, sizeof(tmpBuf), fp) != NULL )
			{
			
				if( (pValue=strstr(tmpBuf,"inet6 addr") ) != NULL  )  //if something found here, failure status is returned 
				{
					unlink(TMP_CHECK_IF);
					fclose(fp);
								       
					return 0;
                     	 }
			
			}
		}
	
	//if key words indicating failure of getting interface status NOT found, 
	//success value 0 is returned  

	unlink(TMP_CHECK_IF);
	fclose(fp);
		
	return -1;
}/* end checkInterfaceStatus */


#endif


#if defined(TCSUPPORT_CT)
#if 0
void ntpexecute()
{
	char nodebuf[32] = {0};
	char tz_type[4] = {0};
	char syncbuf[10] = {0};
	unsigned int synctime = MAX_NTP_SYNC_TIME;
	mxml_node_t *top = NULL;

	sprintf(nodebuf,"%s_%s",TIMEZONE,SUB_NODE_NAME);
	tcapi_get(nodebuf,"TYPE",tz_type);
	if(atoi(tz_type) != TZ_NTP_TYPE)
	{
		ntpSynchroNb = 0;
		ntpneedexecute = NTPFLAGWITHSYNC;
		ntpnumber = 0;
		return;	
	}

	tcapi_get(nodebuf,"SYNCTIME",syncbuf);
	if(0 != atoi(syncbuf))
	{
		synctime = atoi(syncbuf)*60;
	}
	
	if(ntpSynchroNb >= synctime)
	{
		ntpSynchroNb = 0;
		ntpneedexecute = NTPFLAGWITHSYNC;
	}
	else
	{
		ntpSynchroNb++;
	}
	
	if(!ntpneedexecute)
	{
		return;
	}
	
	if(0 != ntpdelayflag1 || 0 != ntpdelayflag2)
	{
		top = get_node_top();
		if(NTPFLAGWITHEXECUTE == ntpneedexecute)
		{
			ntpnumber++;
			if(ntpnumber >= NTPDELAYTIME)
			{
				//sprintf(nodebuf,"%s_%s",TIMEZONE,SUB_NODE_NAME);
				tcapi_commit_internal(top, nodebuf);
				ntpnumber = 0;
				ntpneedexecute = NTPFLAGWITHNOEXECUTE;
			}
		}
		else
		{
			//sprintf(nodebuf,"%s_%s",TIMEZONE,SUB_NODE_NAME);
			tcapi_commit_internal(top, nodebuf);
			ntpneedexecute = NTPFLAGWITHNOEXECUTE;
		}
	}

	return;
}
#endif
struct timespec req_time_pre,req_time_cur,req_time_diff;
void initNTPTimerStruct()
{
	memset(&req_time_pre,0,sizeof(req_time_pre));
	memset(&req_time_cur,0,sizeof(req_time_cur));
	memset(&req_time_diff,0,sizeof(req_time_diff));
	return;
}
void ntpexecute()
{
	char nodebuf[32] = {0};
	char tz_type[4] = {0};
	char syncbuf[10] = {0};
	unsigned int synctime = MAX_NTP_SYNC_TIME;
	mxml_node_t *top = NULL;
	static unsigned char ntpsta = 1;
	unsigned int synctrytime = NTPTRYTIME, ntptrynumber_max = NTPTRYMAXNUMBER;
	static unsigned int ntptrynumber = 0;

	sprintf(nodebuf,"%s_%s",TIMEZONE,SUB_NODE_NAME);
	tcapi_get(nodebuf,"TYPE",tz_type);
	if(atoi(tz_type) != TZ_NTP_TYPE)
	{
		ntpneedexecute = NTPFLAGWITHSYNC;
		ntpnumber = 0;
		initNTPTimerStruct();	
		ntpsta = 0;
		ntptrynumber = 0;
		return;	
	}
	
	if(req_time_pre.tv_sec == 0)
	{
		clock_gettime(CLOCK_MONOTONIC,&req_time_pre);
	}

	if(0 != ntpdelayflag1 || 0 != ntpdelayflag2)
	{
		top = get_node_top();
		if(NTPFLAGWITHEXECUTE == ntpneedexecute)
		{
			ntpnumber++;
			if(ntpnumber >= NTPDELAYTIME)
			{
				//sprintf(nodebuf,"%s_%s",TIMEZONE,SUB_NODE_NAME);
				tcapi_commit_internal(top, nodebuf);
				ntpnumber = 0;
				ntpneedexecute = NTPFLAGWITHSYNC;
			}
		}
		else if(NTPFLAGWITHSYNC == ntpneedexecute)
		{
			//sprintf(nodebuf,"%s_%s",TIMEZONE,SUB_NODE_NAME);
			tcapi_get(nodebuf,"SYNCTIME",syncbuf);
			if(0 != atoi(syncbuf))
			{
#if defined(TCSUPPORT_CT_2NTP)
				synctime = atoi(syncbuf);
#else
				synctime = atoi(syncbuf)*60;
#endif
			}
			
			if(ntpsta)
			{
				memset(syncbuf, 0, sizeof(syncbuf));
				tcapi_get(nodebuf,"SyncTryMaxNum",syncbuf);
				if(0 != atoi(syncbuf))
					ntptrynumber_max = atoi(syncbuf);

				memset(syncbuf, 0, sizeof(syncbuf));
				tcapi_get(nodebuf,"SyncResult",syncbuf);

				if((strcmp(syncbuf, "1")==0) || (ntptrynumber>=ntptrynumber_max))
				{
					ntpsta = 0;
					ntptrynumber = 0;
				}
				else
				{
					memset(syncbuf, 0, sizeof(syncbuf));
					tcapi_get(nodebuf,"SyncTryTime",syncbuf);
					if(atoi(syncbuf) >= NTPTRYMINTIME)
					{
#if defined(TCSUPPORT_CT_2NTP)
						synctrytime = atoi(syncbuf);
#else
						synctrytime = atoi(syncbuf)*60;
#endif
					}
					if(synctrytime < synctime)
						synctime = synctrytime;
				}
				
			}
			
			clock_gettime(CLOCK_MONOTONIC,&req_time_cur);	
			req_time_diff.tv_sec = req_time_cur.tv_sec - req_time_pre.tv_sec;
			req_time_diff.tv_nsec += req_time_cur.tv_nsec - req_time_pre.tv_nsec;
			while(req_time_diff.tv_nsec > 1000000000)
			{
				req_time_diff.tv_sec++;
				req_time_diff.tv_nsec -= 1000000000;
			}
			if(req_time_diff.tv_nsec < 0)
			{
				req_time_diff.tv_sec--;
				req_time_diff.tv_nsec += 1000000000;
			}
			if(req_time_diff.tv_sec >= synctime)
			{
				tcapi_commit_internal(top, nodebuf);
				initNTPTimerStruct();	
				synctime = MAX_NTP_SYNC_TIME;
				if(ntpsta)
					ntptrynumber ++;
			}
		}
	}

	return;
}
#endif
#if defined(TCSUPPORT_CT_DNSMASQ_MONITOR)
/*______________________________________________________________________________
**function name:dnsmasq_check
**
**description:
*    check dnsmasq exist or not
**parameters:
*    none
**global:
*    none
**return:
*    int 
*    >0 the pid of dnsmasq
*    =0 dnsmasq does not exist 
*    -1 error
**call:
*    none
**revision:
*     1.donglei 20110721
**____________________________________________________________________________*/
int dnsmasq_check(void){
#if defined(TCSUPPORT_CT_PON)
	int dnsmasq_pid = 0;
	int i = 0, rv = 0, pidnum = 0, pid_t[128] = {0};

	rv = find_pid_by_name( "dnsmasq", pid_t, &pidnum);
	if( 0 == rv 
		&& 1 == pidnum )
		dnsmasq_pid = pid_t[0];

	return dnsmasq_pid;
#else
	FILE *fp = NULL;
	char pid_of_dnsmasq[6]={0};
	int dnsmasq_pid = 0;
	system("pidof dnsmasq > /tmp/dnsmasq.pid");
	fp = fopen("/tmp/dnsmasq.pid", "r");
	if(fp == NULL){
		return -1;
	}
	fgets(pid_of_dnsmasq, 6, fp);
	fclose(fp);
	unlink("/tmp/dnsmasq.pid");
	dnsmasq_pid = atoi(pid_of_dnsmasq);	
	//if(dnsmasq_pid==0) tcdbg_printf("dnsmasq pid %d\n",dnsmasq_pid);
	return dnsmasq_pid;
#endif
}
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
struct stats {
        unsigned int    user;
        unsigned int    nice;
        unsigned int    system;
        unsigned int    idle;
        unsigned int    iowait;
        unsigned int    irq;
        unsigned int    softirq;
        unsigned int    total;
};
unsigned int CpuUsage;
void CalcuCpuUsage(){
	static struct stats stat, stold;
	char value[80] = {0};
	char temp[80] = {0};
	unsigned int curtotal;

	memset(value,0, sizeof(value));
	memset(temp,0, sizeof(temp));
	fileRead("/proc/stat", temp, sizeof(temp));
	if(strlen(temp) == 0)
		return;
	sscanf(temp, "%s %d %d %d %d %d %d %d", value, &stat.user, &stat.nice,&stat.system, 
			&stat.idle, &stat.iowait, &stat.irq, &stat.softirq);
	stat.total = stat.user + stat.nice + stat.system + stat.idle + stat.iowait + stat.irq + stat.softirq;

	curtotal = stat.total - stold.total;
	if(0 == curtotal)
		return;

	CpuUsage = ((stat.total - stat.idle) - (stold.total - stold.idle))*100/curtotal;
	memcpy(&stold, &stat, sizeof(stat));
}
#endif
#if defined(TCSUPPORT_XPON_LED)
/*_____________________________________________________________________________
**      function name: xpon_led
**      descriptions:
**	      change xpon led state.
**
**      parameters:
**            "0" led off
**            "1" led blink
**            "2" led on
**
**      global:
**            None
**
**      return:
**      
**	call:
**            None
**____________________________________________________________________________
*/
int xpon_led(char *led_st)
{
  FILE *xpon_led_proc;

  xpon_led_proc = fopen("/proc/tc3162/led_xpon", "w+");
    
  if (!xpon_led_proc) {
  	tcdbg_printf("\nopen led_xpon proc fail.");
    return -1;
  }

  fwrite(led_st, 1, strlen(led_st), xpon_led_proc);
  fclose(xpon_led_proc);

  return 0;
}

/*
 *  port = 0~3 & led_st = 0: OFF; 1: BLINK; 2: ON
 */
int xpon_752x_lan_led(int port, int led_st){
	char buf[20]= {0};
	FILE *xpon_lan_led_proc;	
#if defined(TCSUPPORT_CUC_C5_SFU)
	FILE *gse_link_sta_proc;

	gse_link_sta_proc = fopen("/proc/tc3162/gsw_link_st", "r"); 
	memset(buf, 0, sizeof(buf));
	fgets(buf, sizeof(buf), gse_link_sta_proc);
	fclose(gse_link_sta_proc);
#endif

	xpon_lan_led_proc = fopen("/proc/tc3162/led_752x", "w+");

	if (!xpon_lan_led_proc)
		return -1;

	sprintf(buf, "%d %d", port, led_st);
	fwrite(buf, 1, strlen(buf), xpon_lan_led_proc);
	fclose(xpon_lan_led_proc);
	
	return 0;
}

#define ETH_PORT_STS    "/proc/tc3162/eth_port_status"
#define LANPORTNUM		4
int xpon_752x_get_lan_status(int lanStatus[]){
	FILE *fp = NULL;
	char string[16] = "";
	int portNum = 0, i = 0;
	
	fp = fopen(ETH_PORT_STS, "r");
	if(fp != NULL){
		memset(string, 0, sizeof(string));
		fgets(string, sizeof(string), fp);
		portNum = sscanf(string, "%d %d %d %d", lanStatus, lanStatus+1, lanStatus+2, lanStatus+3);
		fclose(fp);
		if(portNum == LANPORTNUM) return 0;
	}
	return -1;
}
#endif


/*______________________________________________________________________________
**function name:changeCongestionMode
**
**description:
*    when qos active is yes , enable pwanCong_ThresholdMode
*    when qos active is no, disable pwanCong_ThresholdMode
**parameters:
*    none
**global:
*    none
**return:
*    none
**call:
*    none
**revision:
*    
**____________________________________________________________________________*/
#if defined(TCSUPPORT_CT_QOS)

void changeCongestionMode(void)
{
		char nodename[MAX_NODE_NAME], qos_active[8];	
		int pwanCong_TrtcmMode = 0;
		int pwanCong_DeiDropMode = 0;
		int pwanCong_ThresholdMode= 0;
		char pwanCongestModeCmd[48] = {0};
		FILE *pwanCongestModeFile = NULL;
		
		char s_pwanCong_TrtcmMode[48] = {0};
		char s_pwanCong_DeiDropMode[48] = {0};
		char s_ThresholdMode[48] = {0};
		char s_modename[48] = {0};
		int qos_active_flag = 0;
		

		system("/userfs/bin/ponmgr pwan get congestion > /tmp/congestionFile.txt");

		pwanCongestModeFile=fopen("/tmp/congestionFile.txt","r");
		if(pwanCongestModeFile == NULL){
			tcdbg_printf("/tmp/congestionFile.txt open failed!!\n");
		}

		fscanf(pwanCongestModeFile,"%s %s %s %s",s_modename,s_pwanCong_TrtcmMode,s_pwanCong_DeiDropMode,s_ThresholdMode);//////////////////////////////////////////
		
		//tcdbg_printf("s_modename:%s\n",s_modename); 
		//tcdbg_printf("v_pwanCong_TrtcmMode:%s\n",s_pwanCong_TrtcmMode);
		//tcdbg_printf("s_pwanCong_DeiDropMode:%s\n",s_pwanCong_DeiDropMode);
		//tcdbg_printf("s_pwanCong_ThresholdMode:%s\n",s_ThresholdMode);
		
		fclose(pwanCongestModeFile);	

		pwanCong_TrtcmMode=(strcmp(s_pwanCong_TrtcmMode,"enable")==0)?1:0;
		pwanCong_DeiDropMode=(strcmp(s_pwanCong_DeiDropMode,"enable")==0)?1:0;
		pwanCong_ThresholdMode=(strcmp(s_ThresholdMode,"enable")==0)?1:0;

		sprintf(nodename,"QoS_Common"); 
		tcapi_get(nodename,"Active",qos_active);
	
		if((strcmp(qos_active,"Yes")== 0)&&(pwanCong_ThresholdMode != 1))
		{
			//tcdbg_printf("\ncc_start qos active is yes!!!!!\n");
			qos_active_flag = 1;
			pwanCong_ThresholdMode = 1;
		}
		if((strcmp(qos_active,"No")== 0)&&(pwanCong_ThresholdMode != 0))
		{
			//tcdbg_printf("\ncc_start qos active is no!!!!!\n"); 
			qos_active_flag = 1;
			pwanCong_ThresholdMode = 0;
		}
		if(qos_active_flag == 1)
		{
			sprintf(pwanCongestModeCmd,"/userfs/bin/ponmgr pwan set congest_mode %d %d %d",pwanCong_TrtcmMode,pwanCong_DeiDropMode,pwanCong_ThresholdMode);
			system(pwanCongestModeCmd);
		}

}

#endif

#ifdef TCSUPPORT_CPU_MT7520
static inline void check_omci_need_restart(){
    FILE *fp=NULL;
    char fe_reset_happened=0;

    fp = fopen("/proc/xpon/fe_reset_happened", "r");
    if(fp){
        fe_reset_happened = fgetc(fp);
        fclose(fp);
    }
    
    if(fe_reset_happened =='1') {
        /* When pon link down, after driver layer has reset FE, omci should be restared */
        system("killall -SIGUSR1 omci");
        system("killall -9 omci");
        system("/userfs/bin/omci &");
        system("echo 0 > /proc/xpon/fe_reset_happened");
    }
}
#endif

/*______________________________________________________________________________
**function name:start_cc
**
**description:
*    start Central_Coordinator
**parameters:
*    none
**global:
*    none
**return:
*    none
**call:
*    none
**revision:
*     1.shnwind 20080526
**____________________________________________________________________________*/
void
start_cc(void* unused)
{
	int adslIsUp=0, adsl_status=0;
	FILE *fp = NULL;
	char string[32];
#if defined(TCSUPPORT_CT_DNSMASQ_MONITOR)
	int tCount = 0;
	int Flag_Check  = 0;
#endif

	int preLanStatus[LAN_PORT_NUM] = {0};
	int curLanStatus[LAN_PORT_NUM] = {0};
	int i, flushflag = 0;
#if defined(TCSUPPORT_XPON_LED)
	int prexponLanStatus[LANPORTNUM] = {0};
	int curxponLanStatus[LANPORTNUM] = {0};
	int j = 0;
#endif
#if defined(TCSUPPORT_CT_PON)
	char cwmpreboot[8] = {0};
	char nodeName[32] = {0};
#endif
	int istate_time = 0;
	int istate_change = 0;
#if defined(TCSUPPORT_CT_FJ)
	char isRegFactoryDef[12] = {0};
	int regFactoryTimeCnt = 0;
#endif

#if defined(TCSUPPORT_CT_PPPOEPROXY)
	
	char buf[20] = {0};
	fp = fopen("/var/run/cfg_manager.pid","w+");
	snprintf(buf, sizeof(buf), "%d\n", getpid());
	fputs(buf, fp);
	fclose(fp);
	
#endif
#if defined(TCSUPPORT_WEB_INTERCEPTION)
	int web_intercption = 0;
#endif
#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
	int isLandingPageSet= 0;
#endif
#if defined(TCSUPPORT_CT_PHONEAPP)
	int isPopupWindowSet=0;
	int PopupWindowEnableFlag=0;
	int PopupWindowTime=0;
	char attrValue[32] = {0}; 
	struct timespec curtime;
	static struct timespec prev_time;
#endif

#if defined(TCSUPPORT_CT_DNSMASQ_MONITOR)
	init_dnsmasq_mutex();
#endif
#if defined(TCSUPPORT_CUC_LANDING_PAGE)
	system("iptables -t nat -N PRE_PONSTATE");
	system("iptables -t nat -A PREROUTING -j PRE_PONSTATE");
	system("iptables -t nat -A PRE_PONSTATE -i br0 -p tcp --dport 80 -j REDIRECT");
#endif
#if defined(TCSUPPORT_CT_PHONEAPP)
	system("iptables -t nat -N PRE_PHONEAPP");
	system("iptables -t nat -A PREROUTING -j PRE_PHONEAPP");
	system("iptables -t nat -A PRE_PHONEAPP -j RETURN");
#endif

#if defined(TCSUPPORT_CT_QOS)
	static int count_changeCongestionMode=0;
#endif

#if defined(TCSUPPORT_CT_PON)
#if defined(TCSUPPORT_CT_QOS)
#if !defined(TCSUPPORT_CUC) && !defined(TCSUPPORT_CMCC)
	int oldStatus=0;
	oldStatus=getPonState();
#endif
#endif
#endif

	while(1){
#if defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505)
		if ( isTR69FWUpgrade() )
		{
			tcdbg_printf("\n cwmp fw upgrading, cc thread exit.\n");
			break;
		}
#endif
#if defined(TCSUPPORT_CT_PON)
#if defined(TCSUPPORT_CT_QOS)
#if !defined(TCSUPPORT_CUC) && !defined(TCSUPPORT_CMCC)
		if(oldStatus!=getPonState())
		{
			oldStatus=getPonState();
			if((oldStatus==1) && (count_changeCongestionMode==45))
				count_changeCongestionMode=0;
		}
#endif
#endif
#endif

#ifdef TCSUPPORT_TCAPI_ENHANCE
		tcapi_enhance_commit_node_queue_process();
#endif
		//THIS modification is for mac esd RESET, restore all settings.
		fp = fopen("/proc/tc3162/mac_esd_check", "r");
		if(fp != NULL){
			fread(string, sizeof(char), sizeof(string), fp);
			fclose(fp);
			if(atoi(string) == 1){
				system("echo 1 >/proc/tc3162/mac_esd_check");
				tcdbg_printf("reenable vlan\n");	
				system("/userfs/bin/ethphxcmd eth0 vlanpt disable");
				system("/userfs/bin/ethphxcmd eth0 vlanpt enable");
			}
		}
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
		           check_ctIppingDiagnosatic();
#endif
#if defined(TCSUPPORT_ECN_MEGACO)
#if defined(TCSUPPORT_ECN_SIP)
	if(getSipStatus() == 1) {
#endif
        mgappKeepAlive();
#if defined(TCSUPPORT_ECN_SIP)
	}
#endif
#endif

#ifdef TCSUPPORT_CPU_MT7520
		check_omci_need_restart();
#endif

#ifdef TCSUPPORT_WLAN
		check_wifi();
		checkWlanBtn();
#endif

#if !defined(TCSUPPORT_CT_FJ)
		check_reset();
#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
		CalcuCpuUsage();
#endif
#if defined(TCSUPPORT_CT_PPPOEPROXY)
		check_pppoeproxy();
#endif
#ifdef TR068_LED
		check_internet_connect();
#endif
#if defined(TCSUPPORT_CT_PPPINFORM)
		check_bridge_connection();
#endif
#if defined(TCSUPPORT_CT_AUTOREGISTER)
		check_bridge_status();
#endif
#if defined(TCSUPPORT_CT_ADSL_BIND1)
		check_bridge_ppp_connection();
#endif
	#ifdef WSC_AP_SUPPORT
		check_wpsbtn();
	#endif
#if defined(TCSUPPORT_CT_BUTTONDETECT)
		check_wlan_wps_button();
#endif
#if defined(TCSUPPORT_LED_BTN_CHECK)
#if defined(TCSUPPORT_BTN_CHECK)
				check_button_type();
#endif
#endif
		check_dhcprelay();
		/*Check adsl link status*/
#if defined(TCSUPPORT_CT_PON)
		adsl_status = is_pon_link_up();
		setPonState2(adsl_status);
#if !defined(TCSUPPORT_CUC) && !defined(TCSUPPORT_CMCC)
		setPonState(adsl_status);
		checkOLTstate();
#endif
#if defined(TCSUPPORT_CT_PHONEAPP)
		tcapi_get("System_Entry","wd_enable", attrValue);
		PopupWindowEnableFlag=atoi(attrValue);
		clock_gettime(CLOCK_MONOTONIC, &curtime);
		if(isPopupWindowSet == 1 && curtime.tv_sec-prev_time.tv_sec>PopupWindowTime){				
			tcapi_set("System_Entry","wd_enable", "0");
			tcapi_set("System_Entry","wd_Stat", "0");//timeout,popup fail
			system("iptables -t nat -F PRE_PHONEAPP");
			system("iptables -t nat -Z PRE_PHONEAPP");
			system("iptables -t nat -A PRE_PHONEAPP -j RETURN");
			PopupWindowEnableFlag = 0;
			isPopupWindowSet = 0;
		}
		if(PopupWindowEnableFlag == 1 && isPopupWindowSet != 1){
			tcapi_get("System_Entry","wd_time", attrValue);
			PopupWindowTime=atoi(attrValue);				
			tcapi_set("System_Entry","wd_Status", "0");//set ok
			system("iptables -t nat -F PRE_PHONEAPP");
			system("iptables -t nat -Z PRE_PHONEAPP");
			system("iptables -t nat -A PRE_PHONEAPP -i br0 -p tcp --dport 80 -j REDIRECT");
			clock_gettime(CLOCK_MONOTONIC, &prev_time);
			isPopupWindowSet = 1;
		}
		else if(PopupWindowEnableFlag != 1 && isPopupWindowSet == 1){
			isPopupWindowSet = 0;
		}
#endif
#if defined(TCSUPPORT_CT_JOYME)
		check_olinetime();
		check_wpsstatus();
#endif
#else
		adsl_status = is_adsl_link_up();
#if defined(TCSUPPORT_CT_DSL_EX) && defined(TCSUPPORT_CT_WAN_PTM)	
		g_dslstate_up = adsl_status;
		if ( 1 == g_dsldetect
			&& 1 == g_dslstate_up
			&& 1 == g_dslstate_chg
			&& adslIsUp == adsl_status )
		{
			g_adsl_state = adslIsUp = ADSL_STATE_DOWN;
			tcdbg_printf("ccdbg: force to do DSL down-->up OP!\n");
		}
		g_dsldetect = 0;
		g_dslstate_chg = 0;
#endif
#endif
		if(adslIsUp != adsl_status){
#if defined(TCSUPPORT_CT_DSL_EX) && defined(TCSUPPORT_CT_WAN_PTM)	
			if ( 1 == g_dslstate_up )
				g_dsldetect = 1;
#endif
			tcapi_set("WebCurSet_Entry", "StatusChanging","1");
			istate_change = 1;
			istate_time = 0;
			adsl_state_change(adsl_status);
			adslIsUp = adsl_status;		
			
#if defined(TCSUPPORT_CT_PPPOE_EMULATOR)
			if(1 == adsl_status)      // down to up
			{
				tcapi_commit(PPPOE_EMULATOR_BRIDGE_NODE_NAME);
			}
			else {
				if (PppoeEmulatorBridgeThread_id > 0) {
					mxml_node_t *top;
					top = *gTreePtr;
					pppoe_Emulator_Bridge_stop(top, PPPOE_EMULATOR_STOP_NOWAIT_FLAG);
					pthread_cancel(PppoeEmulatorBridgeThread_id);
					PppoeEmulatorBridgeThread_id = 0;
				}
			}
#endif
		}
		else
		{
			if ( 1 == istate_change )
			{
				if ( istate_time > 10 )
				{
					istate_change = 0;
					istate_time = 0;
			tcapi_set("WebCurSet_Entry", "StatusChanging","0");
				}
				else
					istate_time ++;
			}
		}
		
#ifdef TCSUPPORT_LED_SWITCH_BUTTON
		check_led_switch_button(adsl_status);
#endif

#if defined(TCSUPPORT_XPON_LED)
	if(!xpon_752x_get_lan_status(curxponLanStatus)){
		for(j = 0; j < LANPORTNUM; j++) {
			if(curxponLanStatus[j] == 1 && prexponLanStatus[j] == 0) {
				tcdbg_printf("Link State: LAN_%d up.\n", j+1);
				prexponLanStatus[j] = 1;
				xpon_752x_lan_led(j, 2);
				
			} else if(curxponLanStatus[j] == 0 && prexponLanStatus[j] == 1) {
				tcdbg_printf("Link State: LAN_%d down.\n", j+1);
				prexponLanStatus[j] = 0;
				xpon_752x_lan_led(j, 0);
			}
		}		
	}
#endif
		/* Check lan link status */
		get_lan_status(curLanStatus);
		for(i = 0; i < LAN_PORT_NUM; i++)
		{
			if((preLanStatus[i] == 1) && (curLanStatus[i] == 0))
			{
				flushflag = 1;
#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT)
				if(is_portal_on() == SUCCESS)
				{
					check_lanip_exist();
				}
#endif
			}
			preLanStatus[i] = curLanStatus[i];
		}
		
		if(flushflag == 1)
		{
			flushflag = 0;
#if !defined(TCSUPPORT_CT_JOYME)
			flush_arp();
#endif
		}
#if defined(TCSUPPORT_AUTO_DMZ)
		autoDMZCheck();
#endif

	
#ifdef IPV6
		check_ipv6_gateway();
#endif
#if !defined(TCSUPPORT_CT_PON)
		autopvc_state_machine(adslIsUp);
#endif
#if defined(TCSUPPORT_CT)
		/*note:ntp period time is according to cc's peroid time*/
		//ntpexecute();
#ifdef CWMP
#if defined(TCSUPPORT_CT_PON) && !defined(TCSUPPORT_CY_E8_SFU)
			   tcapi_get("WebCustom_Entry","ReStartCWMP", cwmpreboot);
			   if(!strcmp(cwmpreboot, "1"))
			   {			   		
					for(i = 0; i < PVC_NUM; i++){
						sprintf(nodeName,"Wan_PVC%d",i);
						tcapi_commit(nodeName);
					}
					system("/usr/bin/killall -9 tr69");	
					system("/userfs/bin/tr69 &");
					tcapi_set("WebCustom_Entry","ReStartCWMP", "0");
			   }
#endif
#endif
#endif
#if defined(TCSUPPORT_WEB_INTERCEPTION) || defined(TCSUPPORT_TRUE_LANDING_PAGE)

		if(adslIsUp==1){//adsl is up
#if defined(TCSUPPORT_WEB_INTERCEPTION)
			if(web_intercption == 1)
			{
				system("iptables -t nat -D PREROUTING -p tcp --dport 80  -j REDIRECT --to-ports 80");
				web_intercption = 0;
			}
#endif
#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
			if(1 == isLandingPageEnable()){
				if((isLandingPageSet == 0) && (checkIfInternetInterfaceUp() == SUCCESS)){
					isLandingPageSet = 1;
					//interface up, start landing page rules
					system("iptables -t nat -D PRE_PORTAL -i br0 -p tcp --dport 80 -j REDIRECT");
					system("iptables -t nat -A PRE_PORTAL -i br0 -p tcp --dport 80 -j REDIRECT");
				}
			}
			
#endif
		}
		else{//adsl is down
#if defined(TCSUPPORT_WEB_INTERCEPTION)
			if(web_intercption == 0)
			{
				system("iptables -t nat -A PREROUTING -p tcp --dport 80  -j REDIRECT --to-ports 80");
				web_intercption = 1;
			}
#endif
		}
#endif
		//upgrade_firmware();
#if defined(TCSUPPORT_CT_DNSMASQ_MONITOR)
		tCount++;
		if( tCount > DNS_CHECK_TIME ){   /* check dnsmasq every 5 seconds */
			tCount = 0;
			if( dnsmasq_check() == 0 ){
				if(Flag_Check){          /* dnsmasq does not exist for the second time */
					Flag_Check=0;
					tcdbg_printf("restart dnsmasq!\n");
				#ifdef TCSUPPORT_DNSEACHPVC
					restart_dnsmasq();
				#endif
				}else {                  /* dnsmasq does not exist for the first time */
					Flag_Check=1;
				}
			}else Flag_Check=0;          /* dnsmasq exists */
		}
#endif
#if defined(TCSUPPORT_CT)
		checkUILockState();
#endif
#if defined(TCSUPPORT_CT_BRIDGEARP_NOFWD_LAN)
		AddBridgeArpRules ();
#endif
#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
		notify_OnDemandWan_Switch();
#endif
#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
		checkPPPoE_Emulator_timer();
#endif
#if defined(TCSUPPORT_CT_L2TP_VPN)
		checkVPNPPPTunnels(adsl_status);
#endif
#if (VOIP == 1)
#if defined(TCSUPPORT_ECN_SIP)
#if defined(TCSUPPORT_ECN_MEGACO)
	if(getSipStatus() == 0) {
#endif
		voipStatUpgrade();	
		voipSimulateTest();
#if defined(TCSUPPORT_ECN_MEGACO)
	}
#endif
#endif
#if defined(TCSUPPORT_ECN_MEGACO)
#if defined(TCSUPPORT_ECN_SIP)
	if(getSipStatus() == 1) {
#endif
		sendSimulateDtmf();
		voipSimulateTest();
#if defined(TCSUPPORT_ECN_SIP)
	}
#endif
#endif
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
#if defined(MT7615E) || defined(MT7612E)
	voipIdleProcess();
#endif
#endif
#endif
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_STBMAC_REPORT)
		notify_STB_change();
#endif
#if defined(TCSUPPORT_CT_FJ)
		if ( 0 == 
			tcapi_get("WebCurSet_Entry","isRegFactoryDef", isRegFactoryDef)
			&& 0 == strcmp(isRegFactoryDef, "Yes") )
		{
			if ( regFactoryTimeCnt >= 2 )
			{
				regFactoryTimeCnt = 0;
				tcapi_set("WebCurSet_Entry","isRegFactoryDef", "No");
				system("prolinecmd restore default");
			}
			else
				regFactoryTimeCnt++;
		}
#endif
#if defined(TCSUPPORT_CT_JOYME)
		notify_OSGI_UPG_change();
#endif
		sleep(1);
	}
	pthread_exit(0);
//	return NULL;
}/*end start_cc*/

#if defined(TCSUPPORT_CT)
#if 0
#if defined(TCSUPPORT_CT_2NTP)
#if defined(TCSUPPORT_CT_NTPSERVERTYPE)
extern unsigned int ntpdelayflag1_all[4];
extern unsigned int ntpdelayflag2_all[4];
#endif
#endif
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
extern int isTimeZoneCommit;
#endif

extern int ntp_server_ret;
int executeTimzone(){
	char *pValue=NULL;
	char *delim="/: ";
	char time_info[6][8];
	char tz_type[4]={0};
	char tz_pc_clock[32]={0};
	int i=0;
	char time_attr[][8]={"Year","Month","Date","Hour","Min","Sec",""};
	time_t tm;
	struct tm tm_time;
	int now_time=0,correct_time=0;//,oldtime=0;
#if defined(TCSUPPORT_CT_2NTP)
	char server[64]={0};
	char server2[64]={0};
#if defined(TCSUPPORT_CT_5NTP)
	char server3[64]={0};
	char server4[64]={0};
	char server5[64]={0};
#endif
#if defined(TCSUPPORT_CT_NTPSERVERTYPE)
	char ntpservertype[4]={0};
	int flag1 =0;
	int flag2 =0;
#endif
	char route[32]={0};
	char cmd[256] = {0};
	struct sockaddr_in cvtaddr;
	char addrouteCMD[128]={0};
	char convertIP[64]={0};	
	char convertIP2[64]={0};
#if defined(TCSUPPORT_CT_5NTP)
	char convertIP3[64]={0}, convertIP4[64]={0}, convertIP5[64]={0};
#endif
	struct sockaddr_in addr = {0};
	unsigned long theHostAddressPtr;
	int ret=0;
	int if_index=0;
	char tmp[8]={0};
#endif
#if defined(TCSUPPORT_CT_FJ)
	char ntpStatus[4]={0};	
	char ntpName[32] = {0};
#endif
	char timeZoneNode[32] = {0};
	mxml_node_t *top = NULL;



	top = get_node_top();
	now_time=time(&tm);
	memset(&tm, 0, sizeof(tm));
	memset(time_info, 0, sizeof(time_info));
	
	sprintf(timeZoneNode,"%s_%s",TIMEZONE,SUB_NODE_NAME);
	tcapi_get(timeZoneNode, "TYPE", tz_type);
	switch(atoi(tz_type)){
		case TZ_NTP_TYPE:/*NTP Server automatically*/
#if defined(TCSUPPORT_CT_FJ)
			memset(ntpName,0,sizeof(ntpName));
			strcpy(ntpName, "System_Entry");
			tcapi_get(ntpName,"NTPStatus",ntpStatus);
			if(!strcmp(ntpStatus,"1")){
				return SUCCESS;
			}
			tcapi_set(ntpName,"NTPStatus","1");
#endif
#if defined(TCSUPPORT_CT_2NTP)
			memset(server,0,sizeof(server));
			memset(route,0,sizeof(route));
			memset(convertIP,0,sizeof(convertIP));
			memset(convertIP2,0,sizeof(convertIP2));
			if(tcapi_get(timeZoneNode, TIMEZONE_ATTR_SERVER, server) 
									== TCAPI_PROCESS_OK && 
				tcapi_get(timeZoneNode, TIMEZONE_ATTR_SERVER2, server2) 
									== TCAPI_PROCESS_OK &&
#if defined(TCSUPPORT_CT_5NTP)
				tcapi_get(timeZoneNode, TIMEZONE_ATTR_SERVER3, server3) 
									== TCAPI_PROCESS_OK &&
				tcapi_get(timeZoneNode, TIMEZONE_ATTR_SERVER4, server4) 
									== TCAPI_PROCESS_OK &&
				tcapi_get(timeZoneNode, TIMEZONE_ATTR_SERVER5, server5) 
									== TCAPI_PROCESS_OK &&
#endif
#if defined(TCSUPPORT_CT_NTPSERVERTYPE)
				tcapi_get(timeZoneNode, TIMEZONE_ATTR_NTPSERVERTYPE, ntpservertype) 
									== TCAPI_PROCESS_OK)
#else
				tcapi_get(timeZoneNode, TIMEZONE_ATTR_ADDROUTE, route) 
									== TCAPI_PROCESS_OK)
#endif
			{
				if (!strcmp(server, "0.0.0.0"))
					strcpy(server, NTP_DEF_SERVER);
				if (!strcmp(server2, "0.0.0.0"))
					strcpy(server2, NTP_DEF_SERVER);
#if defined(TCSUPPORT_CT_5NTP)
				if (!strcmp(server3, "0.0.0.0"))
					strcpy(server3, NTP_DEF_SERVER);
				if (!strcmp(server4, "0.0.0.0"))
					strcpy(server4, NTP_DEF_SERVER);
				if (!strcmp(server5, "0.0.0.0"))
					strcpy(server5, NTP_DEF_SERVER);
#endif
#if defined(TCSUPPORT_CT_NTPSERVERTYPE)
				//get first match if_index
				/***
				NTPServerType:
				0:INTERNET
				1:VOICE
				2:TR069
				3:OTHER
				***/				
				i=atoi(ntpservertype);
				if(i<0 || i>3)
					return FAIL;
				flag1=ntpdelayflag1_all[i];
				flag2=ntpdelayflag2_all[i];
				if(0 != flag1){
					for(i=0;i<32;i++){
						if(flag1&0x01)
							break;
						flag1=flag1>>1;
					}
					if_index=i;
				}
				else if(0 != flag2){
					for(i=0;i<32;i++){
						if(flag2&0x01)
							break;
						flag2=flag2>>1;
					}
					if_index=i+32;
				}
				else{
					return FAIL;
				}
				if(get_waninfo_by_index(if_index,WAN_IFNAME,route) < 0){
					return FAIL;
				}
#else
				if((if_index = get_wanindex_by_name(route)) < 0){
					return FAIL;
				}
				sprintf(tmp, "%d", if_index);
				tcapi_set(timeZoneNode, TIMEZONE_ATTR_IFINDEX,tmp);
#endif

#if defined(TCSUPPORT_CT_5NTP)
				sprintf(cmd, "/bin/echo -e \"%s %s\\n%s %s \\n%s %s\\n%s %s\\n%s %s\" > %s",
				server, route, server2, route, server3, route,  server4, route, server5, route, TMP_DNS_NTP_PATH);
#else
				sprintf(cmd, "/bin/echo -e \"%s %s\\n%s %s\" > %s", server, route, server2, route, TMP_DNS_NTP_PATH);
#endif
				system(cmd);

				if(inet_aton(server,&cvtaddr.sin_addr))
				{	
					//address is ipv4
					strcpy(convertIP,server);
				}
				else{
					//address is a domain name
					theHostAddressPtr = resolveName(server);
					memcpy(&addr.sin_addr, &theHostAddressPtr, sizeof(addr.sin_addr));
					strcpy(convertIP, inet_ntoa(/*theHostAddressPtr*/addr.sin_addr));
				}
			
				if(inet_aton(server2,&cvtaddr.sin_addr))
				{	
					//address is ipv4
					strcpy(convertIP2,server2);
				}
				else{
					//address is a domain name
					theHostAddressPtr = resolveName(server2);
					memcpy(&addr.sin_addr, &theHostAddressPtr, sizeof(addr.sin_addr));
					strcpy(convertIP2, inet_ntoa(/*theHostAddressPtr*/addr.sin_addr));
				}	

#if defined(TCSUPPORT_CT_5NTP)
				if(inet_aton(server3, &cvtaddr.sin_addr))
				{	
					//address is ipv4
					strcpy(convertIP3, server3);
				}
				else
				{
					//address is a domain name
					theHostAddressPtr = resolveName(server3);
					memcpy(&addr.sin_addr, &theHostAddressPtr, sizeof(addr.sin_addr));
					strcpy(convertIP3, inet_ntoa(/*theHostAddressPtr*/addr.sin_addr));
				}

				if(inet_aton(server4, &cvtaddr.sin_addr))
				{	
					//address is ipv4
					strcpy(convertIP4, server4);
				}
				else
				{
					//address is a domain name
					theHostAddressPtr = resolveName(server4);
					memcpy(&addr.sin_addr, &theHostAddressPtr, sizeof(addr.sin_addr));
					strcpy(convertIP4, inet_ntoa(/*theHostAddressPtr*/addr.sin_addr));
				}

				if(inet_aton(server5, &cvtaddr.sin_addr))
				{	
					//address is ipv4
					strcpy(convertIP5, server5);
				}
				else
				{
					//address is a domain name
					theHostAddressPtr = resolveName(server5);
					memcpy(&addr.sin_addr, &theHostAddressPtr, sizeof(addr.sin_addr));
					strcpy(convertIP5, inet_ntoa(/*theHostAddressPtr*/addr.sin_addr));
				}
#endif

				//execute [route add cmd]
				sprintf(addrouteCMD,ADDROUTE_CMD, convertIP,route);
				ret = system(addrouteCMD);
				if(0 == ret){
					ntp_server_ret = system(NTP_PATH);
					if(0 == ntp_server_ret){
						if(correct_sys_time(top)){
							fprintf(stderr, "Cannot retrieve TimeZone data.\n");
						}
					}
					//execute [route delete cmd]
					sprintf(addrouteCMD,DELROUTE_CMD, convertIP);
					system(addrouteCMD);
				}
				
				if(0 == ret && 0 != ntp_server_ret){
					sprintf(addrouteCMD,ADDROUTE_CMD, convertIP2,route);
					ret = system(addrouteCMD);
					if(0 == ret){
						if(0 == system(NTP_PATH2)){
							ntp_server_ret = 0;
							if(correct_sys_time(top)){
								fprintf(stderr, "2NTP-Cannot retrieve TimeZone data.\n");
							}
						}
						sprintf(addrouteCMD,DELROUTE_CMD, convertIP2);
						system(addrouteCMD);
					}
				}
#if defined(TCSUPPORT_CT_5NTP)
				if(0 == ret && 0 != ntp_server_ret)
				{
					sprintf(addrouteCMD,ADDROUTE_CMD, convertIP3,route);
					ret = system(addrouteCMD);
					if(0 == ret)
					{
						if(0 == system(NTP_PATH3))
						{
							ntp_server_ret = 0;
							if(correct_sys_time(top))
							{
								fprintf(stderr, "3NTP-Cannot retrieve TimeZone data.\n");
							}
						}
						sprintf(addrouteCMD, DELROUTE_CMD, convertIP3);
						system(addrouteCMD);
					}
				}

				if(0 == ret && 0 != ntp_server_ret)
				{
					sprintf(addrouteCMD, ADDROUTE_CMD, convertIP4,route);
					ret = system(addrouteCMD);
					if(0 == ret)
					{
						if(0 == system(NTP_PATH4))
						{
							ntp_server_ret = 0;
							if(correct_sys_time(top))
							{
								fprintf(stderr, "4NTP-Cannot retrieve TimeZone data.\n");
							}
						}
						sprintf(addrouteCMD,DELROUTE_CMD, convertIP4);
						system(addrouteCMD);
					}
				}

				if(0 == ret && 0 != ntp_server_ret)
				{
					sprintf(addrouteCMD, ADDROUTE_CMD, convertIP5,route);
					ret = system(addrouteCMD);
					if(0 == ret)
					{
						if(0 == system(NTP_PATH5))
						{
							ntp_server_ret = 0;
							if(correct_sys_time(top))
							{
								fprintf(stderr, "5NTP-Cannot retrieve TimeZone data.\n");
							}
						}
						sprintf(addrouteCMD, DELROUTE_CMD, convertIP5);
						system(addrouteCMD);
					}
				}
#endif

				unlink(TMP_DNS_NTP_PATH);
			}			
#else
			ntp_server_ret = system(NTP_PATH);
			/**********20081001 racing add************/
			if(correct_sys_time(top)){
				fprintf(stderr, "Cannot retrieve TimeZone data.\n");
			}
			/**********20081001 racing add************/
#endif
			
			break;
		case TZ_PC_TYPE:/*PC CLOCK*/
		case TZ_MANUAL_TYPE:/*Manually*/
			if(atoi(tz_type)==TZ_PC_TYPE){
				tcapi_get(timeZoneNode, "PC_CLOCK", tz_pc_clock);
				strtok(tz_pc_clock,delim);
				strcpy(time_info[0], tz_pc_clock);
				for(i=1;(pValue=strtok(NULL, delim))!= NULL; i++){
					strcpy(time_info[i], pValue);
				}
			}
			else{
				for(i=0; strlen(time_attr[i])!=0; i++){
					tcapi_get(timeZoneNode, time_attr[i], time_info[i]);
				}
			}

			time(&tm);
			memset(&tm_time, 0, sizeof(tm_time));

			tm_time.tm_year	= atoi(time_info[0]);
			tm_time.tm_mon	= atoi(time_info[1]);
			tm_time.tm_mday	= atoi(time_info[2]);
			tm_time.tm_hour	= atoi(time_info[3]);
			tm_time.tm_min	= atoi(time_info[4]);
			tm_time.tm_sec	= atoi(time_info[5]);

			/* correct for century  - minor Y2K problem here? */
			if (tm_time.tm_year >= 1900){
				tm_time.tm_year -= 1900;
			}
			/* adjust date */
			if (tm_time.tm_mon >= 1){
				tm_time.tm_mon -= 1;
			}

			tm = mktime(&tm_time);
			if (tm <0){
				fprintf(stderr,"mktime error\n");
			}
			else{
				if (stime(&tm) < 0){
					fprintf(stderr,"stime err!\n");
				}
			}
			break;
		default:
			break;
	}

	/*krammer add for bug 907*/
	memset(&tm, 0, sizeof(tm));
	correct_time=time(&tm);/*get correct time after time synchronization*/
	if(abs(correct_time - now_time) > MAX_DIFFERENT_TIME){//ppp old connect time should be changed when difference >30
		correctPPPtime((correct_time - now_time));
	}

	return SUCCESS;
}
#endif

void
start_cc_new(void* unused)
{
#ifdef TCSUPPORT_PON_IP_HOST
	unsigned char igmp_flag = 1;
	FILE *fp= NULL;
#endif

	while(1){
#if defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505)
		if ( isTR69FWUpgrade() )
		{
			tcdbg_printf("\n cwmp fw upgrading, cc thread new exit.\n");
			break;
		}
#endif

		/* Check lan link status */
		/*note:ntp period time is according to cc's peroid time*/
		ntpexecute();
#if defined(TCSUPPORT_CT_DSL_EX) && defined(TCSUPPORT_CT_WAN_PTM)
		if ( 1 != g_dslstate_chg
			&& 1 == g_dsldetect
			&& 1 == g_dslstate_up 
			&& is_adsl_link_up() != g_dslstate_up )
			g_dslstate_chg = 1;
#endif
#if 0
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
		if(isTimeZoneCommit){
			executeTimzone();
			isTimeZoneCommit = 0;
		}
#endif
#endif
#ifdef TCSUPPORT_PON_IP_HOST
		if(igmp_flag)
		{
			fp = fopen("/proc/tc3162/pon_bridge_wanIf", "r");
			if(fp)
			{
				tcapi_commit("XPON_Common_xponigmp");
				igmp_flag = 0;
				fclose(fp);
			}
		}
#endif
		sleep(1);
	}
	pthread_exit(0);
}
#endif

#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
int get_all_wan_and_pvc_index(int indexbuf[], int pvcbuf[],int entrybuf[], int buf_size)
{
	int pvc_index, entry_index, if_index, if_num = 0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *curNode = NULL;
	mxml_node_t *top = get_node_top();
	if(!indexbuf)
		return 0;
	
	for(pvc_index = 0; pvc_index < PVC_NUM; pvc_index++){
		for(entry_index = 0; entry_index < MAX_SMUX_NUM; entry_index++){
			memset(nodeName, 0, sizeof(nodeName));
		  	strcpy(nodeName[0],WAN);
		  	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
			sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);

			curNode = mxmlFindElement(top, top, nodeName[1],
			NULL, NULL, MXML_DESCEND);
			if(curNode){
				curNode = mxmlFindElement(curNode, curNode, nodeName[2],
				NULL, NULL, MXML_DESCEND);
				if(curNode){
					/*calculate interface index based on pvc index and entry index*/
					*pvcbuf=pvc_index;
					pvcbuf++;
					*entrybuf=entry_index;
					entrybuf++;
					if_index = pvc_index * MAX_SMUX_NUM + entry_index;
					*indexbuf = if_index;
					indexbuf++;
					if_num++;
					buf_size--;
					if(buf_size  <= 0)
						break;
				}
			}
		}
	}
	return if_num;
}
/*____________________________________________________________________________
*check if the pvc entry's service type is  internet 
_____________________________________________________________________________*/
int checkIfInternetInterfaceUp(void)
{
	char nodeName[20] = {0};
	char str_status[16] = {0};
	char servlist[SERVICE_LIST_LEN] = {0};
	int valid_if[MAX_WAN_IF_INDEX] = {0};
	int valid_pvc[MAX_WAN_IF_INDEX]={0};
	int valid_entry[MAX_WAN_IF_INDEX] = {0};
	int valid_if_num = 0;
	int i = 0;
	
	valid_if_num = get_all_wan_and_pvc_index(valid_if, valid_pvc, valid_entry,MAX_WAN_IF_INDEX);
	for(i = 0; i< valid_if_num; i++){
		sprintf(nodeName, "%s_Entry%d", WAN_IF_NODE,valid_if[i]);
		if((tcapi_get(nodeName, STATUS, str_status) == TCAPI_PROCESS_OK) && !strcmp(str_status,"up") ){
			memset(nodeName,0,sizeof(nodeName));
			sprintf(nodeName,"%s_%s%d_%s%d",WAN,WAN_PVC,valid_pvc[i],WAN_INTERFACE,valid_entry[i]);
			if(tcapi_get(nodeName, SERVICELIST, servlist) == TCAPI_PROCESS_OK && strstr(servlist,INTERNET) != NULL){
				return SUCCESS;
			}
			else 
				continue;
		}
		else
			continue;
	}
	return FAIL;
}
#endif
#if 0
/*Remove upgrade_firmware function for new method.shnwind 2009.3.31*/
/*______________________________________________________________________________
**function name:upgrade_firmware
**
**description:
*    check the firmware name. if it is correct, upgrade it.
**parameters:
*    none
**global:
*    none
**return:
*    none
**call:
*    none
**revision:
*     1.shnwind 20080526
**____________________________________________________________________________*/
void upgrade_firmware()
{
	struct stat buf;
	FILE *fp;
	char cmd_buf[128];

	if(stat("/var/tmp/upgrade",&buf) != 0){
		return;
	}

	fp = fopen("/var/tmp/upgrade", "r");
	if( fp == NULL ){
		fprintf(stderr, "Open file error!!");
		return;
	}
	fgets(cmd_buf, 80, fp);
	fclose(fp);
	fprintf(stderr, "buf=%s\n", cmd_buf);
	if(tcapi_set("System_Entry","upgrade_fw", cmd_buf) != 0){
		fprintf(stderr, "TCAPI System Entry Fail!!");
	}
	else{
		if(tcapi_commit("System_Entry")!=0){
			fprintf(stderr, "The firmware is illegal!!");
		}
		else{
//			tcdbg_printf("\r\nWARNING:It will take some minutes writing flash.\r\n");
//			tcdbg_printf("Don't reboot modem within 5 minutes.\r\n");
//			tcdbg_printf("The modem will reboot itself after finishing writing flash.\r\n");
			fprintf(stderr, "WARNING:It will take some minutes writing flash.\r\n");
			fprintf(stderr, "Don't reboot modem within 5 minutes.\r\n");
			fprintf(stderr, "The modem will reboot itself after finishing writing flash.\r\n");
		}
	}
#if 0
	if(stat(ROOTFS_PATH,&buf) == 0){
		sprintf(cmd_buf,"%s %s rootfs\n",MTD_WRITE_WITH_REBOOT,ROOTFS_PATH);
	}else if(stat(KERNEL_PATH,&buf) == 0){
		sprintf(cmd_buf,"%s %s kernel\n",MTD_WRITE_WITH_REBOOT,KERNEL_PATH);
	}else if(stat(UPLOAD_ROMFILE_PATH,&buf) == 0){
		sprintf(cmd_buf,"%s %s romfile\n",MTD_WRITE_WITH_REBOOT,ROMFILE_PATH);
	}
	fprintf(stderr, "cmd=%s\n", cmd_buf);
	unlink("/var/tmp/upgrade");
//	system(cmd_buf);
#endif
	unlink("/var/tmp/upgrade");
	return;
}/*end upgrade_firmware*/


/*______________________________________________________________________________
**function name:restart_dhcp
**
**description:
*    this function is use for restart dhcp service
**parameters:
*    none
**global:
*    none
**return:
*    none
**call:
*    none
**revision:
*     1.shnwind 20080417
**____________________________________________________________________________*/

void
restart_dhcp(int pvc)
{
	char dhcp_file[32],dhcp_cmd[128], default_route[8];

	sprintf(dhcp_file, "/var/run/udhcpc-nas%d.pid", pvc);
	kill_process(dhcp_file);

	sprintf(dhcp_file, "/etc/isp%d.conf", pvc);
		get_profile_str("DEFAULTROUTE=", default_route, sizeof(default_route), QMARKS, dhcp_file);
	/*busybox->udhcpc put in /sbin*/
	if(!strcmp(default_route,"Yes")){
		sprintf(dhcp_cmd,"/sbin/udhcpc -i nas%d -s /usr/script/udhcpc.sh -p /var/run/udhcpc-nas%d.pid &",pvc,pvc);
		system(dhcp_cmd);
	}else if(!strcmp(default_route,"No")){
		sprintf(dhcp_cmd,"/sbin/udhcpc -i nas%d -s /usr/script/udhcpc_nodef.sh -p /var/run/udhcpc-nas%d.pid &",pvc,pvc);
		system(dhcp_cmd);
	}/*else{
	fprintf(stderr,"error when reading isp%d information",pvc);
	}*/

	return;
}/*end restart_dhcp*/

/*______________________________________________________________________________
**function name:restart_PPP
**
**description:
*    restart ppp service
**parameters:
*    none
**global:
*    none
**return:
*    none
**call:
*    none
**revision:
*     1.shnwind 20080526
**____________________________________________________________________________*/
void
restart_PPP(int PVC)
{

	char ppp_file[32],ppp_cmd[300], ppp_pid[32], execmd[300];
	FILE *ppp_conf;

	sprintf(ppp_file, "/var/run/pppd%d.pid", PVC);
	kill_process(ppp_file);

	/*execute pppd*/
	sprintf(ppp_file, "/var/tmp/ppp%d.conf", PVC);
	ppp_conf = fopen(ppp_file, "r");
	if(ppp_conf)
	{
		fgets(ppp_cmd, 300, ppp_conf);
		fclose(ppp_conf);
		/*save pppd pid*/
		sprintf(ppp_pid, "echo $! > /var/run/pppd%d.pid", PVC);
		strcat(ppp_cmd, ppp_pid);
		sprintf(execmd,"/bin/sh -c %s",ppp_cmd);
		system(ppp_cmd);

	}

	return;
}/*end restart_PPP*/
#endif

#ifndef PURE_BRIDGE
/*______________________________________________________________________________
**function name:restart_nat
**
**description:
*    reload nat rule
**parameters:
*    none
**global:
*    none
**return:
*    none
**call:
*    none
**revision:
*     1.shnwind 20080526
*	2.jlliu 20100806:modify parameters:PVC to wan_if_name
**____________________________________________________________________________*/
#if 0
void
restart_nat(int PVC)
{
	char buf[64];

	sprintf(buf, "/usr/script/nat_stop.sh %d\n/usr/script/nat_start.sh %d", PVC, PVC);
	system(buf);

  return;
}/*end restart_nat*/
#endif
void
restart_nat(char *wan_if_name)
{
	char buf[128];

	//tcdbg_printf("restart_nat:wan interface is %s \n", wan_if_name);
	sprintf(buf, "/usr/script/nat_stop.sh %s\n/usr/script/nat_start.sh %s", wan_if_name, wan_if_name);
	system(buf);

  	return;
}/*end restart_nat*/

/*______________________________________________________________________________
**function name:restart_ddns
**
**description:
*    restart ddns service
**parameters:
*    none
**global:
*    none
**return:
*    none
**call:
*    none
**revision:
*     1.shnwind 20080526
**____________________________________________________________________________*/

void
restart_ddns()
{
	int fd;

	fd = open("/etc/ddns.conf", O_RDONLY);
	if(fd != -1){
		close(fd);
		kill_process("/var/log/ez-ipupdate.pid");
		system("/usr/script/ddns.sh");
		system("/usr/script/ddns_run.sh &");
	}
	return;
}/*end restart_ddns*/

#endif
#define WIFI_RESET_CMD 	"ifconfig ra0 down;sleep 5;ifconfig ra0 up;"
#define WIFI_RESET_AGAIN 	"echo 1 2 > /proc/tc3162/ahb_status"

/*______________________________________________________________________________
**function name:downWifiInterface
**
**description:
*    down all related interfaces
**parameters:
*    none
**global:
*    none
**return:
*    none
**call:
*    downWifiInterface()
**revision:
*     Brian.shi
**____________________________________________________________________________*/
static void downWifiInterface()
{
	int i,flag = 0;
	char wifibuf[16];
	char wificmd[64];

#ifdef TCSUPPORT_WLAN_WDS
	tcapi_get("WLan_WDS","WdsEnable",wifibuf);
	flag = atoi(wifibuf);
	if(flag)
	{
		for(i = 0; i < MAX_WDS_ENTRY; i++)
		{
			sprintf(wificmd,WIFI_DOWN_CMD_WDS,i);
			system(wificmd);
		}
	}
#endif
#ifdef 	TCSUPPORT_WLAN_AC
	tcapi_get("WLan11ac_Common","BssidNum",wifibuf);
	flag = atoi(wifibuf);
	for(i = 0; i < flag; i++)
	{
		sprintf(wificmd,WIFI_DOWN_CMD_RAI,i);
		system(wificmd);
	}

#ifdef TCSUPPORT_WLAN_WDS
	tcapi_get("WLan11ac_WDS","WdsEnable",wifibuf);
	flag = atoi(wifibuf);
	if(flag)
	{
		for(i = 0; i < MAX_WDS_ENTRY; i++)
		{
			sprintf(wificmd,WIFI_DOWN_CMD_WDSI,i);
			system(wificmd);
		}
	}
#endif
#endif
	return;
}

/*______________________________________________________________________________
**function name:upWifiInterface
**
**description:
*    up all related interfaces
**parameters:
*    none
**global:
*    none
**return:
*    none
**call:
*    upWifiInterface()
**revision:
*     Brian.shi
**____________________________________________________________________________*/
static void upWifiInterface()
{
	int i,flag = 0;
	char wifibuf[16];
	char wificmd[64];

#ifdef TCSUPPORT_WLAN_WDS
	tcapi_get("WLan_WDS", "WdsEnable", wifibuf);
	flag = atoi(wifibuf);
	if(flag)
	{
		for(i = 0; i < MAX_WDS_ENTRY; i++)
		{
			sprintf(wificmd,WIFI_UP_CMD_WDS,i);
			system(wificmd);
		}
	}
#endif
#ifdef 	TCSUPPORT_WLAN_AC
	tcapi_get("WLan11ac_Common", "BssidNum", wifibuf);
	flag = atoi(wifibuf);
	for(i = 0; i < flag; i++)
	{
		sprintf(wificmd,WIFI_UP_CMD_RAI,i);
		system(wificmd);
	}
#ifdef TCSUPPORT_WLAN_WDS
	tcapi_get("WLan11ac_WDS","WdsEnable",wifibuf);
	flag = atoi(wifibuf);
	if(flag)
	{
		for(i = 0; i < MAX_WDS_ENTRY; i++)
		{
			sprintf(wificmd,WIFI_UP_CMD_WDSI,i);
			system(wificmd);
		}
	}
#endif
#endif
	return;
}

void 
check_wifi(void){
	char string[8];
	int fd=0, ret=0,reset=0, onoff=0;
	fd = open("/proc/tc3162/ahb_status",O_RDONLY|O_NONBLOCK);
	if(fd!=-1){
		ret = read(fd, string, 8);
		close(fd);
		sscanf(string, "%d %d", &onoff, &reset);
		if(reset==1 && onoff==1){
		#ifdef TCSUPPORT_SYSLOG
			do_wlan_alarm_syslog();
		#endif
			#ifdef PCIE_DEBUG
			fprintf(stderr,"%s\n", WIFI_RESET_CMD);
			system("echo cc reset >> /tmp/reset");
			#endif
			/*first:down wifi interface*/
			downWifiInterface();
			/*second:do reset*/
			system(WIFI_RESET_AGAIN);
			/*tcdbg_printf("Reinit Wifi interface \n");*/
			/*three:up wifi interface*/
			upWifiInterface();

			system(WIFI_RESET_CMD);
			/*Disable Reset*/
			system("echo 1 0 > /proc/tc3162/ahb_status");
		}
	}
}

#ifdef TCSUPPORT_LED_SWITCH_BUTTON
void check_led_switch_button(int pon_stats)
{
	char string[4] = {0};
	char strRegStatus[32] = {0}, trafficSta[32] = {0}, ledSta[10] = {0};
	int ret, fd;
	static int oldStatus = LED_SWITCH_ON;
	static int do_flag = 0;
	int btnStats = 0, led_flag = 0;
	
	fd = open(LED_SWITCH_BUTTON_PATH,O_RDONLY|O_NONBLOCK);
	if(fd != -1)
	{
		ret = read(fd, string, sizeof(string));
		close(fd);
		if(ret)
		{
			btnStats = atoi(string);	

			/* skip first time */
			if (btnStats == 1) {
				tcapi_get("Sys_Entry", "ledStatus", ledSta);
				if (!strcmp(ledSta, "off"))
					led_flag = 1;
				
				if (led_flag == 1 && do_flag == 0) {
					do_flag = 1;
					
					tcdbg_printf("\r\ncheck_led_switch_button(): skip the led status");
					return;
				}
			}
			
			if(0 != btnStats)	               // 1 led switch button is pressed; 2 led off; 3 recover
			{
				if(btnStats == 2 || (btnStats == 1 && LED_SWITCH_ON == oldStatus))
				{
					tcdbg_printf("\r\ncheck_led_switch_button(): led off, btnStatus = %d", btnStats);
					system("sys led off");
					oldStatus = LED_SWITCH_OFF;
				}
				else if(btnStats == 3 || (btnStats == 1 && LED_SWITCH_OFF == oldStatus))
				{
					system("sys led recover");
					oldStatus = LED_SWITCH_ON;

#if defined(TCSUPPORT_VOIP) && defined(TCSUPPORT_ECN_SIP)
#if defined(TCSUPPORT_ECN_MEGACO)
				if(getSipStatus() == 0) {
#endif
					tcapi_get("VoIPSysParam_Entry0", "SC_ACCT_INFO_MEDIA_STATUS", trafficSta);
					if (0 == strncmp(trafficSta, "Media-Begin", strlen("Media-Begin"))) {
						led_file_write("/proc/fxs/sipRegLed","0 2");            // for one fxs
					}else{       // VOIP registeted, turn on the voip led				
						tcapi_get("VoIPSysParam_Entry0", "SC_ACCT_INFO_REG_STATUS", strRegStatus);
						if(0 == strncmp(strRegStatus,"Registered",strlen("Registered"))) {
							led_file_write("/proc/fxs/sipRegLed","0 1");            // for one fxs
						}
					}
#if defined(TCSUPPORT_ECN_MEGACO)
				}
#endif
#endif
					
#if defined(TCSUPPORT_VOIP) && defined(TCSUPPORT_ECN_MEGACO)
#if defined(TCSUPPORT_ECN_SIP)
				if(getSipStatus() == 1) {
#endif
					tcapi_get("InfoVoIPH248_Entry0", "lineInfoStatus", trafficSta);
					if (strncmp(trafficSta, "Connect", strlen("Connect")) == 0) {
						led_file_write("/proc/fxs/sipRegLed","0 2");            // for one fxs
					}
                    else {
                        tcapi_get("VoIPH248_Entry0", "UserServiceState", strRegStatus);
					    if (strcmp(strRegStatus, "1") == 0) {
							led_file_write("/proc/fxs/sipRegLed","0 1");           	 	// for one fxs
					    }
                    }
#if defined(TCSUPPORT_ECN_SIP)
				}
#endif

#endif

#if defined(TCSUPPORT_CT_PON) && defined(TCSUPPORT_XPON_LED)
					recover_pon_led(pon_stats);
#endif
					tcdbg_printf("recover all led status\n");
				}
			}
			else          // no press, do nothing
			{
				// do nothing
			}
		}
	}
	else
	{
		fprintf(stderr,"no file %s\n",LED_SWITCH_BUTTON_PATH);
		return;
	}
	return;

}

void recover_pon_led(int pon_stat)
{
	char string[4] = {0};
	int ret, fd;
	int losStats = 0;
	
	if(pon_stat == ADSL_STATE_UP)
	{
		xpon_led("2"); //led on
	}
	else
	{
		xpon_led("0"); //led off
	}

	fd = open(LED_LOS_PATH,O_RDONLY|O_NONBLOCK);
	if(fd != -1)
	{
		ret = read(fd, string, sizeof(string));
		close(fd);

		if(ret)
		{
			losStats = atoi(string);

			if(0 == losStats)          // no signal, flick
			{
				led_file_write(LED_LOS_PATH,"2");
			}
			else                       // turn off
			{
				led_file_write(LED_LOS_PATH,"0");
			}
		}
	}
	else
	{
		fprintf(stderr,"no file %s\n",LED_LOS_PATH);
		return;
	}
	
}

int led_file_write(char *path, char *led_st)
{
	FILE *led_proc;

	if(NULL == path)
	{
		return -1;
	}
	
	led_proc = fopen(path, "w+");

	if (!led_proc) 
	{	
		return -1;
	}

	fwrite(led_st, 1, strlen(led_st), led_proc);
	fclose(led_proc);

	return 0;
}


#endif

#ifdef TCSUPPORT_WLAN
/*______________________________________________________________________________
**function name:checkWlanBtn
**
**description:
*    change Wlan status
*                       
**parameters:
*    none
**global:
*    none
**return:
*    none
**call:
*    none
**revision:
*     1.xmdai 20100926
**____________________________________________________________________________*/
void
checkWlanBtn(void)
{
	char string[4], strAPOn[4];
	int ret, iWlanState, fd;
	
	fd = open(WLAN_BUTTON_PATH,O_RDONLY|O_NONBLOCK);
	if(fd != -1){
		ret = read(fd, string, sizeof(string));
		close(fd);
		if(ret){
			iWlanState = atoi(string);	
			if(iWlanState >= 1){	

			#if 1//add by xyzhu_130904, merge from general
				/*wait until unpress the button. shnwind 20110407*/
				while(iWlanState != 0){
					fd = open(WLAN_BUTTON_PATH,O_RDONLY|O_NONBLOCK);
					if(fd != -1){
						ret = read(fd, string, sizeof(string));
						close(fd);
						if(ret){
							iWlanState = atoi(string);
						}
						//else{
						//	tcdbg_printf("read fail\n");	
						//}
						sleep(1);
					}
				}
			#endif
			
				tcapi_get(WLAN_COMMON,"APOn",strAPOn);
				if( atoi(strAPOn) == 1 )
					tcapi_set(WLAN_COMMON,"APOn","0");
				else if(atoi(strAPOn) == 0 )
					tcapi_set(WLAN_COMMON,"APOn","1");
				tcapi_commit(WLAN);

			#if 1//add by xyzhu_130904
				tcapi_save();
			#endif
			
			}
		}
	}else{
		fprintf(stderr,"no file %s\n",WLAN_BUTTON_PATH);
		return;
	}
	return;
}
#endif
/*______________________________________________________________________________
**function name:check_reset
**
**description:
*    check reset_button <10s reset
*                       >10s reset to default
**parameters:
*    none
**global:
*    none
**return:
*    none
**call:
*    none
**revision:
*     1.shnwind 20080526
**____________________________________________________________________________*/
void
check_reset(void)
{
	char string[4], myString[4], buf[32];
	int ret, fd, myfd, h_sec, size, my_sec;
	int is_reset_default = 0;
	int PHASE1_SEC = 2;
	int PHASE2_SEC = 20;

#if defined(TCSUPPORT_CT_PON_SC) || defined(TCSUPPORT_CT_PON_CY_JX)
	PHASE1_SEC = 10;
	PHASE2_SEC = 60;
#endif
#if defined(TCSUPPORT_CT_ADSL_HN)
	PHASE1_SEC = 6;
	PHASE2_SEC = 20;
#endif

	fd = open("/proc/tc3162/reset_button",O_RDONLY|O_NONBLOCK);
	if(fd != -1){
		ret = read(fd, string, 4);
		close(fd);
		if(ret){
			h_sec = atoi(string);
			while(h_sec > 0){
				myfd = open("/proc/tc3162/reset_button",O_RDONLY|O_NONBLOCK);
				if(myfd != -1){
					if(isbootset)
						continue;
					size = read(myfd, myString, 8);
					close(myfd);
					my_sec = atoi(myString);
					sprintf(buf,"%s_%s",SYSTEM,SUB_NODE_NAME);					
#if defined(TCSUPPORT_CT)
					if(my_sec == 0){
						if( 1 == is_reset_default ){
#if defined(TCSUPPORT_CT_PON_GD) || defined(TCSUPPORT_CT_PON_CQ)
							break;
						}
#else
#if defined(TCSUPPORT_CT_PON_JX) || defined(TCSUPPORT_CT_PON_CY_JX)
							break;
#else
#if defined(TCSUPPORT_CT_PON_CZ_HN) 
							system("reboot");
#else
							sprintf(string,"%d",BOOT2DEFSET);
							tcapi_set(buf,"reboot_type",string);
							tcapi_commit(buf);	
#endif
#endif
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
							is_reset_default = 0;
#endif
						}
#if defined(TCSUPPORT_CT_PON_SC) || defined(TCSUPPORT_CT_PON_JX) || defined(TCSUPPORT_CT_ADSL_HN) || defined(TCSUPPORT_CT_PON_CY_JX)
						/* factory default reset */
						else if ( 2 == is_reset_default )
						{
							system("prolinecmd restore default");
						}
#endif
#if defined(TCSUPPORT_CY)
						else if ( 2 == is_reset_default )
						{
							sprintf(string,"%d",BOOT2DEFSETTR69);
							tcapi_set(buf,"reboot_type",string);
							tcapi_commit(buf);
						}
#endif
#endif
#if defined(TCSUPPORT_CT_PON_SC)
						else if ( 3 == is_reset_default )
						{
							/*
							* close tcapi save when delay reboot.
							*/
							tcapi_set("WebCurSet_Entry", "ITMS_ST", "2");

							snprintf(string, sizeof(string) - 1,
									"%d", BOOT2DEFSETITMS);
							tcapi_set(buf, "reboot_type", string);
							tcapi_commit(buf);
							/*
							* must reset it to zero, because voip will do delay reboot
							* (reboot -d x &) , so it will commit many times
							* before reboot.
							*/
							is_reset_default = 0;
						}
#endif
						else{
							/*<1s:do nothing*/
#if defined(TCSUPPORT_CT_PON_JX) || defined(TCSUPPORT_CT_PON_CY_JX) 
							/* restore key parameter reset */
							sprintf(string,"%d",BOOT2DEFSET);
							tcapi_set(buf,"reboot_type",string);
							tcapi_commit(buf);

#else
							break;
#endif
						}
					}
					/* 1s~10s:reset to default*/
					else if(my_sec >= PHASE1_SEC && my_sec <= PHASE2_SEC){
#if defined(TCSUPPORT_CT_PON_SC)
						if ( 3 != is_reset_default )
							system("sys led on");
						is_reset_default = 3;
#else
						is_reset_default = 1;
#endif
					}
					/*>10s: do nothing*/
					else if(my_sec > PHASE2_SEC){
#if defined(TCSUPPORT_CT_LONG_RESETBTN)
#if defined(TCSUPPORT_CT_PON_CQ) && !defined(TCSUPPORT_CT_PON_CZ_CQ)
						sprintf(string,"%d",BOOT2DEFSETTR69);
#else
						sprintf(string,"%d",LONGRESET_BOOT2DEFSET);
#endif
						tcapi_set(buf, "reboot_type", string);
						tcapi_commit(buf);
#endif
#if defined(TCSUPPORT_CT_PON_SC)
						if(2 != is_reset_default){
							system("sys led off");
							is_reset_default = 2;
						}
#else
#if defined(TCSUPPORT_CY) || defined(TCSUPPORT_CT_PON_JX) || defined(TCSUPPORT_CT_ADSL_HN) || defined(TCSUPPORT_CT_PON_CY_JX)
						is_reset_default = 2;
#else
						is_reset_default = 0;
#endif
#endif
#if defined(TCSUPPORT_CUC)
						sprintf(string, "%d", CUC_LONGRESET_BOOT2DEFSET);
						tcapi_set(buf, "reboot_type", string);
						tcapi_commit(buf);
#endif

						#if 0
						/*warm reboot*/						
						sprintf(string,"%d",WARM_REBOOT);
						tcapi_set(buf,"reboot_type",string);
						tcapi_commit(buf);	
						#endif
					}
#else
					if(my_sec == 0){
						/*warm reboot*/						
						sprintf(string,"%d",WARM_REBOOT);
						tcapi_set(buf,"reboot_type",string);
						tcapi_commit(buf);						
						/*system("reboot");*/
					}else if(my_sec >= 10){
						/*reset to default */
						/*sprintf(cmd_buf,"%s %s romfile\n",MTD_WRITE_WITH_REBOOT,DEF_ROMFILE_PATH);
						system(cmd_buf);*/
						sprintf(string,"%d",BOOT2DEFSET);
						tcapi_set(buf,"reboot_type",string);
						tcapi_commit(buf);
					}
#endif
				}
			}
		}
	}else{
		fprintf(stderr,"no file /proc/tc3162/reset_button\n");
		return;
	}
	return;
}/*end check_reset*/

#if defined(TCSUPPORT_CT_BUTTONDETECT)
#define BUTTON_DETECT  9     //this value should same with messageHandle.h's "BUTTON_DETECT"
void check_wlan_wps_button(void)//to check wlan & wps button's click/double click/long click
{
		int n = 0;
		//args[0]:	1 :wlan button;  2: wps button;
		//args[1]:	1:click; 2:double click; 3: long click.
		int args[2] = {0};
#if defined(TCSUPPORT_CT_MIDWARE)	
		cwmp_msg_t message;

		memset(&message,0,sizeof(cwmp_msg_t));
#endif
		args[0] = 0;
		
		if(devfd == 0){
			devfd = open("/dev/led_button",O_RDWR);
		}
		
		if(devfd > 0){
			n = ioctl(devfd, LEDBUTTON_IOCTL_SELECT, args);
			if(n){
				tcdbg_printf("getbutton list error\n");
				return;
			}
			
			if(args[0] != 0){
#if defined(TCSUPPORT_CT_MIDWARE)	
				message.text.reserved[0] = args[0];
				message.text.reserved[1] = args[1];
				message.cwmptype = BUTTON_DETECT;
				n = sendmegq(1,&message, 0);
				if(n < 0)
					tcdbg_printf("send message error\n");
#endif
				//add your event detect here.
				
			}
		}
		
	return;

}
#endif

#ifdef WSC_AP_SUPPORT
#define WPS_BUTTON_PATH "/proc/tc3162/wps_button"

#if defined(TCSUPPORT_WPS_BTN_NEWSPEC)
#ifdef BBUTOWBU
#define WPSSTART 100
#define WPSSTOP 200
#define WLANSTOP 300
#endif
#endif

void 
check_wpsbtn(void){
	char string[4], buf[32], tmp[4];
	int fd, ret;
#if defined(TCSUPPORT_WPS_BTN_NEWSPEC)
#ifdef BBUTOWBU
		char cmd[50]={0};
#endif
#endif
	
	fd=open(WPS_BUTTON_PATH, O_RDONLY|O_NONBLOCK);
	if(fd!=-1){
		ret = read(fd, string, 4);
		close(fd);
		if(ret){
			if(atoi(string)==1){
				tcapi_get(WLAN_COMMON, "APOn", tmp );
			
				if(!strcmp(tmp,"1")){
					memset(buf, 0, sizeof(buf));
					sprintf(buf, "%s_%s", "Info",WLAN);
					tcapi_set(buf, "WPSActiveStatus", "1");
					tcapi_get(buf, "wlanWPStimerRunning", tmp );//
					sprintf(buf, "%s_%s%d", WLAN, SUB_NODE_NAME,0);
					tcapi_commit(buf);
				}
				doValPut(WPS_BUTTON_PATH, "0");
				//sprintf(cmd, "echo %d > /proc/tc3162/wps_button\n", 0);
				//system(cmd);		
			}else if(atoi(string)==2){
#if !defined(TCSUPPORT_CY)
				tcapi_get(WLAN_COMMON, "APOn", tmp );
				if(!strcmp(tmp,"1")){
					memset(buf, 0, sizeof(buf));
					sprintf(buf, "%s_%s", "Info",WLAN);
					tcapi_set(buf, "WPSOOBActive", "1");
					tcapi_get(buf, "wlanWPStimerRunning", tmp );//
					sprintf(buf, "%s_%s%d", WLAN, SUB_NODE_NAME,0);
					tcapi_commit(buf);

				}
#endif
				doValPut(WPS_BUTTON_PATH, "0");
				//sprintf(cmd, "echo %d > /proc/tc3162/wps_button\n", 0);
				//system(cmd);
			}
#if defined(TCSUPPORT_WPS_BTN_NEWSPEC)
						else if (atoi(string) == 4) {
							memset(tmp, 0, sizeof(tmp));
							tcapi_get(WLAN_COMMON, "APOn", tmp);
							if(!strcmp(tmp,"1")) {
								memset(buf, 0, sizeof(buf));
								memset(tmp, 0, sizeof(tmp));
								
								sprintf(buf, "%s_%s", "Info",WLAN);
								tcapi_get(buf, "wlanWPStimerRunning", tmp);
			
								if (!atoi(tmp)) { //start pbc
									tcapi_set(buf, "WPSActiveStatus", "1");
									memset(buf, 0, sizeof(buf));
									sprintf(buf, "%s_%s%d", WLAN, SUB_NODE_NAME,0);
									tcapi_set(buf, "WPSMode","1");
									tcapi_commit(buf);
								}
								else { //stop last pbc connection and start a new connection
									tcapi_set(buf, "wlanWPStimerRunning", "0");
									sprintf(cmd, "/userfs/bin/iwpriv ra0 set WscStop");
									system(cmd);
#ifdef BBUTOWBU				
									memset(cmd, 0, sizeof(cmd));
									sprintf(cmd, "echo %d > /proc/tc3162/wps_button\n", WPSSTOP);
									system(cmd);
#endif
									tcapi_set(buf, "WPSActiveStatus", "1");
									memset(buf, 0, sizeof(buf));
									sprintf(buf, "%s_%s%d", WLAN, SUB_NODE_NAME,0);
									tcapi_set(buf, "WPSMode","1");
									tcapi_commit(buf);
								}
							}
							doValPut(WPS_BUTTON_PATH, "0");
						}
#endif
			
		}
	}else{
		fprintf(stderr,"no file /proc/tc3162/wps_button\n");
		return;
	}
	return;
}
#endif
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
void check_ctIppingDiagnosatic(void)
{
			int num = 0;
		char buf[32] = {0};
		char delay_char[32]={0};
		static int delay=-1;
		if(!recheckPingDiagnostic_flag)
			return;
		if(delay<0)
			{
				if(!tcapi_get("CtDiagnostic_Common","Delay",delay_char))
				 	delay=atoi(delay_char);
				else
					delay=5;
			}
		if(delay --> 0)
			return;
		for(num = 0;num < CTCOM_MAX_IPPINGDIAGNOSTIC_NUM;num++)
	   	{
	   	    
	   	      if(recheckPingDiagnostic_flag&(0x1 << num))
	   	      	{
			  	sprintf(buf,IPPINGDIAGNOSTIC_ENTRY,num);
				 if(!tcapi_commit(buf))
				           recheckPingDiagnostic_flag &= ~(0x1 << num);			 	
		 	}
		}
		
}				
						
#endif
void
check_dhcprelay(void)
{
	if(g_dhcprelay)
	{
		g_dhcprelay = 0;
		kill_process("/var/run/udhcpd.pid");
		unlink(DHCPD_PATH);
		unlink(DPROXYAUTO_PATH);
		system(DHCPRELAY_PATH);
		tcdbg_printf("dhcprelay restarted\n");
	}
}

#ifndef PURE_BRIDGE

#ifdef QOS_DROP  /*Rodney_20091115*/
/*______________________________________________________________________________
**function name: qos_drop_restart
**
**description:   restart qos packet drop machine
*                
**parameters:
*                none
**global:
*                none
**return:
*                none
**call:
*                none
**revision:
*                rodney_20091115
**____________________________________________________________________________*/
void qos_drop_restart(void){
	char nodename[MAX_NODE_NAME], qos_active[8], qos_drop[8], qos_prio[8], encap[32];
	int drop_mode_id, prio_mode_id, pvc;	

	sprintf(nodename,"QoS_Common");	
	tcapi_get(nodename,"Active",qos_active);
	if(strcmp(qos_active,"Yes") != 0)
		goto unset;
	tcapi_get(nodename,"Drop",qos_drop);
	if(strcmp(qos_drop,"WRED") == 0)
		drop_mode_id = QOS_DROP_WRED;
	else if(strcmp(qos_drop,"RED") == 0)
		drop_mode_id = QOS_DROP_RED;
	else
		drop_mode_id = QOS_DROP_TAIL;
	tcapi_get(nodename,"Prio",qos_prio);
	if(strcmp(qos_prio,"DSCP") == 0)
		prio_mode_id = QOS_PRIO_DSCP;
	else if(strcmp(qos_prio,"IPP") == 0)
		prio_mode_id = QOS_PRIO_IPP;
	else
		prio_mode_id = QOS_PRIO_QUEUE;	

	if((drop_mode_id == QOS_DROP_RED) || (drop_mode_id == QOS_DROP_WRED)){
		qos_drop_unset_rule();
		qos_drop_set(ENABLE, drop_mode_id, prio_mode_id);
		for(pvc=0;pvc<PVC_NUM;pvc++){
			sprintf(nodename,"Wan_PVC%d",pvc);		
			tcapi_get(nodename,"ENCAP",encap);			
			if((strcmp(encap, "PPPoA LLC") == 0) || (strcmp(encap, "PPPoA VC-Mux") == 0))
				qos_drop_set_rule(QOS_PPP_INTERFACE, pvc);	
			else
				qos_drop_set_rule(QOS_NAS_INTERFACE, pvc);	
		}
	}
	else
		goto unset;

	return;

	unset:
	qos_drop_set(DISABLE, QOS_DROP_TAIL, 0);
}
#endif

#if 0
/*______________________________________________________________________________
**function name:restart_igmpproxy
**
**description:
*                restart igmpproxy
**parameters:
*                input_pvc: input PVC index
*                interface: WAN or LAN
**global:
*                none
**return:
*                none
**call:
*                none
**revision:
*                rodney_20090420
**____________________________________________________________________________*/
void
restart_igmpproxy(char *interface, int input_pvc)
{
	char nodename[MAX_NODE_NAME];
	char cur_wan_active[8], cur_igmpproxy_active[8], cur_igmpproxy_quickleave[8], cur_wan_isp_temp[8];
	char buf[128]={0}, igmpproxy_pid_tmp[6];
	FILE *fp=NULL;
	struct stat fbuf;
	int pvc_index, cur_wan_isp=0, igmpproxy_pid, check_flag = 1;
    memset(nodename,0,sizeof(nodename));

    /*generate igmpproxy.conf*/
    if(strcmp(interface, "WAN") == 0){
    	/*If Wan interface is changed for a certain PVC (input_pvc), it's necessary to check if IGMPproxy */
    	/*process should be restarted or not, but we should not change the status of the other PVC in this*/
    	/*moment. So when there is PVC(not input_pvc) which enable IGMPproxy already, that's mean we      */
    	/*should not try to restart it*/
    	for(pvc_index=0 ; pvc_index<PVC_NUM ; pvc_index++){
	        sprintf(nodename,"%s%d","Wan_PVC",pvc_index);
	        tcapi_get(nodename,"IGMPproxy",cur_igmpproxy_active);
	        if((strcmp(cur_igmpproxy_active,"Yes") == 0) && (pvc_index != input_pvc)){
	        	return;
	        }
    	}
     	if(stat(IGMPPROXY_CONF_PATH,&fbuf) == 0){  /*check if igmpproxy.conf exist or not*/
    		unlink(IGMPPROXY_CONF_PATH);  /*if igmpproxy.conf exist, remove it*/
    	}
     	/*there are several condition for enable igmpproxy.conf*/
    	sprintf(nodename,"%s%d","Wan_PVC",input_pvc);
    	tcapi_get(nodename,"IGMPproxy",cur_igmpproxy_active);
    	if(strcmp(cur_igmpproxy_active,"Yes") == 0){  /*condition1*/
    		tcapi_get(nodename,"Active",cur_wan_active);
    		tcapi_get(nodename,"ISP",cur_wan_isp_temp);
    		cur_wan_isp = atoi(cur_wan_isp_temp);
    		if((strcmp(cur_wan_active,"Yes") == 0) && (cur_wan_isp != WAN_ENCAP_BRIDGE)){  /*condition2 and 3*/
    			if(check_ip_exist("LAN", input_pvc, cur_wan_isp, check_flag) == SUCCESS){  /*condition4*/
    				tcapi_get(nodename,"IGMPproxyQL",cur_igmpproxy_quickleave);
    				fp = fopen(IGMPPROXY_CONF_PATH,"w");
    				if(fp == NULL){
    					return;
    				}
    				/*pass condition 1~4, then generate igmpproxy.conf according to isp, quickleave, pvc*/
    				if(strcmp(cur_igmpproxy_quickleave,"Yes") == 0)
    					sprintf(buf,"quickleave\n");
    				else{
    					sprintf(buf,"#quickleave\n");
    				}
    				fputs(buf, fp);
    				if(cur_wan_isp == WAN_ENCAP_PPP){
    					sprintf(buf,"phyint ppp%d upstream  ratelimit 0  threshold 1\n",input_pvc);
    				}
    				else{
    					sprintf(buf,"phyint nas%d upstream  ratelimit 0  threshold 1\n",input_pvc);
    				}
    				fputs(buf, fp);
    				sprintf(buf,"phyint br0 downstream  ratelimit 0  threshold 1 \n");
    				fputs(buf, fp);
    				fclose(fp);
    			}
    		}
    	}
    }  /*end WAN*/
    else{
    	if(check_ip_exist("WAN", input_pvc, cur_wan_isp, check_flag) == FAIL){
    		return;
    	}

    }
    /*execute igmpproxy process*/
    system("pidof igmpproxy > /tmp/igmpproxy.pid");  /*get the PID of old igmpproxy process if exist*/
	fp = fopen("/tmp/igmpproxy.pid", "r");
	if(fp == NULL){
		return;
	}
	fgets(igmpproxy_pid_tmp, 6, fp);
	fclose(fp);
	system("rm -r /tmp/igmpproxy.pid");
	igmpproxy_pid = atoi(igmpproxy_pid_tmp);
    if(igmpproxy_pid != 0){  /*if pid != 0, that's mean old igmpproxy process already exist*/
    	sprintf(buf,"kill -9 %d",igmpproxy_pid);  /*remove old igmpproxy process*/
    	tcdbg_printf("IGMP proxy process down!\n");
        system(buf);
    }
	if(stat(IGMPPROXY_CONF_PATH,&fbuf) == 0){  /*if igmpproxy.conf exist, that's mean we have pass condition 1~4*/
		system(IGMPPROXY_DEA_PATH);
		tcdbg_printf("IGMP proxy process on!\n");
	}
    return;
}/*end restart_igmpproxy*/

#endif
#if 0
#ifdef IPV6

/*______________________________________________________________________________
**function name:restart_mldproxy
**
**description:
*                restart mldproxy
**parameters:
*                input_pvc: input PVC index
**global:
*                none
**return:
*                none
**call:
*                none
**revision:
*                rclv_20100815
**____________________________________________________________________________*/
void
restart_mldproxy(char *interface,int input_pvc,int cur_wan_isp)
{
	char nodename[MAX_NODE_NAME];
	char cur_wan_active[8] = {0}, cur_mldproxy_active[8] = {0};
	char ifname[8] = {0};
	char cmd[64] = {0};
         
	memset(nodename,0,sizeof(nodename));
   
    sprintf(nodename,"%s%d","Wan_PVC",input_pvc);
	if(strcmp(interface,"WAN") == 0)
	{
	
    	if( 0 == tcapi_get(nodename,"MLDproxy",cur_mldproxy_active) )
		{
        	if( strcmp(cur_mldproxy_active,"Yes") != 0 ){
        		return;
        	}
		}
		else
		{
			tcdbg_printf("get WAN MLDproxy fail!\n");
			return;
		}
			
		if( 0 == tcapi_get(nodename,"Active",cur_wan_active) )
	 	{
        	if( strcmp(cur_wan_active,"Yes") != 0 ){
        		return;
        	}
   	 	}
		else
		{
			tcdbg_printf("get WAN Active fail!\n");
			return;
		}

		if(cur_wan_isp == WAN_ENCAP_BRIDGE)
			return;
		
		if(cur_wan_isp != WAN_ENCAP_PPP)
			sprintf(ifname,"nas%d",input_pvc);
		else
			sprintf(ifname,"ppp%d",input_pvc);
	} 
	else
	{
		if(cur_wan_isp == WAN_ENCAP_BRIDGE)
			return;
		if(cur_wan_isp != WAN_ENCAP_PPP) 
			sprintf(ifname, "nas%d",input_pvc);
		else
			sprintf(ifname, "ppp%d",input_pvc);
		if( 0 != checkInterfaceStatus(ifname) )
			return; 
	}
		system("kill -9 `cat /var/run/ecmh.pid`");
		sprintf(cmd, "/userfs/bin/ecmh -i %s -p /var/run/ecmh.pid", ifname);
		system(cmd);
		tcdbg_printf("ecmh restarted!\n");
		
		return;
} /*end restart_mldproxy*/

#endif

#endif
#if defined(TCSUPPORT_SIMPLE_UPNP)
#if 0
/*______________________________________________________________________________
**function name:start_upnp
**
**description:
*    start upnp service
**parameters:
*    none
**global:
*    none
**return:
*    none
**call:
*    none
**revision:
*     1.shnwind 20080526
**____________________________________________________________________________*/
void
start_upnp(char *ppp_interface){

	int fd;
	FILE *ifconfig_tmp, *upnpd_pid;
	char pid_of_upnpd[6], ifconfig[100];
	/*get wan interface information*/
	sprintf(ifconfig, "ifconfig %s > /tmp/ifconfig.tmp", ppp_interface);
	system(ifconfig);
	ifconfig_tmp = fopen("/tmp/ifconfig.tmp", "r");
	if(ifconfig_tmp == NULL)
	{
		return;
	}
	fgets(ifconfig, 100, ifconfig_tmp);
	fgets(ifconfig, 100, ifconfig_tmp);
	fclose(ifconfig_tmp);
	/*get upnp deamon pid*/
	system("pidof upnpd > /tmp/upnpd.pid");
	upnpd_pid = fopen("/tmp/upnpd.pid", "r");
	if(upnpd_pid == NULL)
	{
		return;
	}
	fgets(pid_of_upnpd, 6, upnpd_pid);
	fclose(upnpd_pid);

	if(strstr(ifconfig, "inet addr:") && !atoi(pid_of_upnpd)){
		fd = open("/etc/igd/igd.conf", O_RDONLY);
		if(fd != -1){
			close(fd);
			sleep(3);
			system("/userfs/bin/upnpd &");
		}
	}
	return;
}
#endif

/*______________________________________________________________________________
**function name:restart_upnp
**
**description:
*    restart igd upnp service
**parameters:
*    none
**global:
*    none
**return:
*    none
**call:
*    none
**revision:
*     1.wangyz 20100916
**____________________________________________________________________________*/
void
restart_upnp(mxml_node_t *top, char *if_name, int if_index){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char default_route[16]={0};
	int fd = -1;

	memset(nodeName,0,sizeof(nodeName));
	memset(default_route,0,sizeof(default_route));
	strcpy(nodeName[0], "WanInfo");
	strcpy(nodeName[1], "Common");

	if(getAttrValue(top, nodeName, "DefRouteIndexv4", default_route) != SUCCESS){
		return;
	}

	if((strlen(default_route) == 0) || (!strncmp(default_route, "N/A", 3))) {
		return;
	}

	/*If default route interface change, restart upnp.*/
	if(if_index == atoi(default_route)) {
		if(write_upnp_conf(top) == 0) {
			fd = open(UPNPD_PATH, O_RDONLY);
			if(fd != -1){
				//tcdbg_printf("\n==>restart_upnp, write upnp conf and restart UPnP\n");	
				system("killall upnpd");
    				system("/userfs/bin/upnpd &");
			}
		}
	}
	
	return;
}
#endif
#if 0
/*______________________________________________________________________________
**function name:check_route
**
**description:
*    check the dev in route entry is correct or not..
**parameters:
*    PVC:pvc number
*    isp:encapsulation type (DYNAMIC........)
**global:
*    none
**return:
*    none
**call:
*    none
**revision:
*     1.shnwind 20080526
**____________________________________________________________________________*/
void
check_route(int PVC, int isp)
{
	char route_string[100];
	char *dev;
	FILE *route_sh,*route_sh_temp;

	route_sh = fopen(ROUTE_PATH, "r");
	if(route_sh){
		route_sh_temp = fopen(ROUTE2_PATH, "w");
		/*check all entry*/
		while(fgets(route_string, 100, route_sh)){
			/*find dev and pvc number is the same*/
			if((dev = strstr(route_string, "dev")) && PVC == atoi(&(dev[7]))){
				switch(isp){
					case WAN_ENCAP_PPP:/*nas -> ppp*/
						dev[4] = 'p';
						dev[5] = 'p';
						dev[6] = 'p';
					break;
					default:/*ppp - > nas*/
						dev[4] = 'n';
						dev[5] = 'a';
						dev[6] = 's';
					break;
				}
			}
			fputs(route_string, route_sh_temp);
		}
		fclose(route_sh_temp);
		fclose(route_sh);
		/*mv route_sh_tmp to route_sh*/
		unlink(ROUTE_PATH);
		rename(ROUTE2_PATH, ROUTE_PATH);
		unlink(ROUTE2_PATH);
	}
	chmod(ROUTE_PATH, 755);
	system(ROUTE_PATH);
	return;
}
#endif
/*______________________________________________________________________________
**function name:dproxy_restart
**
**description:
*    reload dproxy
**parameters:
*    none
**global:
*    none
**return:
*    none
**call:
*    tcapi_commit
**revision:
*     1.krammer 2008 1208
**____________________________________________________________________________*/
void
dproxy_restart()
{
	tcapi_commit("Dproxy_Entry");
	return;
}

#ifdef TCSUPPORT_DNSEACHPVC
void
dnsmasq_restart()
{
	FILE* fp = NULL;
	FILE* fp1 = NULL;
	int pvc = 0;
	char buf[MAXSIZE] = {0};
	char nodeName[30] = {0};
	char value[30] = {0};
	char sDNS[16] = {0};
	char sInterface[5] = {0};
	char sApplication[10] = {0};
	
	tcdbg_printf("\n dnsmasq_restart");
	
	/* step1: down dnsmasq process */
	system("killall -9 dnsmasq");

	/* step2: write dnsmasq.conf and dnsInfo.conf */
	/* dnsmasq.conf format is:
	** strict-order
	** no-resolv
	** 168.95.1.1@nas0
	** 168.95.1.2@ppp1

	** dnsInfo.conf format is:
	** 168.95.1.1 nas0 Internet
	** 168.95.1.2 ppp1 TR69
	
	** get wan information from DeviceInfo_PVC node
	*/

	fp = fopen("/etc/dnsmasq.conf", "w");
	if(fp == NULL){
		tcdbg_printf("\n open dnsmasq.conf failed");
		exit(1);
	}
	fp1 = fopen("/etc/dnsInfo.conf", "w");
	if(fp1 == NULL){
		tcdbg_printf("\n open dnsInfo.conf failed");
		fclose(fp);
		exit(1);
	}
	
	sprintf(buf, "strict-order\n");
	fwrite(buf, sizeof(char), strlen(buf), fp);
	sprintf(buf, "no-resolv\n");
	fwrite(buf, sizeof(char), strlen(buf), fp);
	
	for(pvc = 0; pvc < PVC_NUM; pvc++){
		sprintf(nodeName, "DeviceInfo_PVC%d", pvc);
		if(tcapi_get(nodeName, "Status", value) != TCAPI_PROCESS_OK||strcmp(value, "0") == 0){
			continue;
		}

		if(tcapi_get(nodeName, "connType", value) != TCAPI_PROCESS_OK)
			continue;
		if(strstr(value, "PPP") != NULL)
			sprintf(sInterface, "ppp%d", pvc);
		else
			sprintf(sInterface, "nas%d", pvc);
			
		//if(tcapi_get(nodeName, "App", value) != TCAPI_PROCESS_OK)
		//	continue;

		//for test: pvc0 App=Internet; pvc1 App=TR69	
		if(pvc == 1)
			strcpy(value, "TR69");
		else 
			strcpy(value, "Internet");
		
		strcpy(sApplication, value);
		memset(value, 0, sizeof(value));

		if(tcapi_get(nodeName, "DNSIP", value) == TCAPI_PROCESS_OK && strlen(value) != 0 && isValidDnsIp(value))
		{
			strcpy(sDNS, value);
			sprintf(buf, "server=%s@%s\n", sDNS, sInterface);
			
			fwrite(buf, sizeof(char), strlen(buf), fp);

			memset(buf, 0, sizeof(buf));
			sprintf(buf, "%s %s %s\n", sDNS, sInterface, sApplication);
			fwrite(buf, sizeof(char), strlen(buf), fp1);
		}
		
		memset(value, 0, sizeof(value));
		if(tcapi_get(nodeName, "SECDNSIP", value) == TCAPI_PROCESS_OK && strlen(value) != 0 && isValidDnsIp(value))
		{
			strcpy(sDNS, value);
			sprintf(buf, "server=%s@%s\n", sDNS, sInterface);
			fwrite(buf, sizeof(char), strlen(buf), fp);

			memset(buf, 0, sizeof(buf));
			sprintf(buf, "%s %s %s\n", sDNS, sInterface, sApplication);
			fwrite(buf, sizeof(char), strlen(buf), fp1);
		}
		
	}
	fclose(fp);
	fclose(fp1);
	
	/* step2: start dnsmasq process */
	system("/userfs/bin/dnsmasq");
	
	return;
}
void
dnsmasq_restart_down(int pvc)
{
	char wBuf[2048] = {0};
	char line[100] = {0};
	FILE *rfp = NULL;
	FILE *wfp = NULL;
	int flag = 0;
	char strnas[5] = {0}, strppp[5] = {0};

	sprintf(strnas, "nas%d", pvc);
	sprintf(strppp, "ppp%d", pvc);

	rfp = fopen("/etc/dnsmasq.conf", "r");
	if(rfp == NULL){
		printf("\n open file failed.");
		exit(1);
	}
	while(fgets(line, 100, rfp)){
		if(strlen(line) == 0)
			continue;
		if(strstr(line, strnas) == NULL && strstr(line, strppp) == NULL)
			strcat(wBuf, line);
		else
			flag = 1;
	}
	fclose(rfp);
	
	if(flag == 0)
		return;
	// restart dnsmasq and write related conf file
	tcdbg_printf("\n dnsmasq_restart_down");
	
	/* step1: down dnsmasq process */
	system("killall -9 dnsmasq");
	/* step2: write dnsmasq.conf file */
	wfp = fopen("/etc/dnsmasq.conf", "w");
	if(wfp == NULL){
		tcdbg_printf("open file failed");
		exit(1);
	}
	fwrite(wBuf, sizeof(char), strlen(wBuf), wfp);
	fclose(wfp);
	/* step3: start dnsmasq process */
	system("/userfs/bin/dnsmasq");
	return;
}
#endif
#endif
/*______________________________________________________________________________
**function name: getLanMac
**
**description:
*    get mac addr of lan side pc
**parameters:
*    none
**global:
*    none
**return:
*    0:SUCCESS
*    -1:FAIL
**call:
*
**revision:
*     1.krammer 2009 0313
**____________________________________________________________________________*/
int
getLanMac(void){
	FILE *fp=NULL;
	FILE *fp1=NULL;
	char tmp[128]={0};
	char garbage[32]={0};
	char mac[32]={0};
	char br0Mac[32]={0};
	char port[4]={0},tmpPort[4]={0};
	int i=0;

	if(get_profile_str(LAN_MAC_KEYWORD,br0Mac,sizeof(br0Mac),NO_QMARKS,MAC_PATH)==FAIL){
		return FAIL;
	}

//	tcdbg_printf("br0 mac: %s\n",br0Mac);
	for(i=0;i<sizeof(br0Mac);i++){
		br0Mac[i]=tolower(br0Mac[i]);
		if(i==17){
			br0Mac[i]='\0';
			break;
		}
	}
//	tcdbg_printf("br0 mac: %s\n",br0Mac);
	system("brctl showmacs br0 > /tmp/tmpMac");
	fp=fopen("/tmp/tmpMac","r");
	if(!fp){
		tcdbg_printf("can't open tmpMac\n");
		return FAIL;
	}
	fgets(tmp,sizeof(tmp),fp);//pass first one
	memset(tmp, 0, sizeof(tmp));
	while(fgets(tmp,sizeof(tmp),fp)){
		if(strstr(tmp,br0Mac)){//get a lan ip
			sscanf(tmp,"%s %s %s %s",port,garbage,garbage,garbage);
			fseek(fp, 0, SEEK_SET);
			fgets(tmp,sizeof(tmp),fp);//pass first one
			memset(tmp, 0, sizeof(tmp));
			while(fgets(tmp,sizeof(tmp),fp)){
				sscanf(tmp,"%s %s %s %s",tmpPort,garbage,garbage,garbage);
				if((!strcmp(port,tmpPort)) && (!strstr(tmp, br0Mac))){//get a lan ip
					fp1=fopen(TMP_MAC_PATH,"w");
					if(!fp1){
						tcdbg_printf("can't open /tmp/mac\n");
						fclose(fp);
						return FAIL;
					}
					//format:port mac islocal ageingTime
					sscanf(tmp,"%s %s %s %s",garbage,mac,garbage,garbage);
					fputs(mac,fp1);
					fclose(fp1);
					fclose(fp);
					return SUCCESS;
				}
				memset(tmp, 0, sizeof(tmp));
			}
		}
		memset(tmp, 0, sizeof(tmp));
	}
	fclose(fp);
	return FAIL;
}

/*______________________________________________________________________________
**function name: autopvcStopAction
**
**description:
*    do the action when stop state
**parameters:
*    adslState: the state of adsl now
*    replace: replace table for autopvc
**global:
*    pvc_bit_map
*    vcpool_bit_map
*    autopvc_state
**return:
*    none
**call:
*    start_autoPVC
*    getLanMac
*    autopvcIsActive
**revision:
*     1.krammer 2009 0313
**____________________________________________________________________________*/
void
autopvcStopAction(int adslState, int *replace){
	int i=0;
	//jump state condition
	if((adslState) && (autopvcIsActive() == SUCCESS) &&
			(getLanMac() == SUCCESS)){//jump to probe pvc state
		//reset replace table
		for(i=0;i<PVC_NUM;i++){
			replace[i]=0;
		}
		//jump action:reset all data
		pvc_bit_map=0;
		vcpool_bit_map=0;
		start_autoPVC();
		autopvc_state=AUTOPVC_STATE_PROBE_PVC;
		tcdbg_printf("autopvc start\n");
	}
}

/*______________________________________________________________________________
**function name: autopvcProbePvcAction
**
**description:
*    do the action when probe_pvc state
**parameters:
*    adslState: the state of adsl now
*    replace: replace table for autopvc
**global:
*    autopvc_state
**return:
*    local_not_finish_pvc: used to update new not_finish_pvc
**call:
*    read_and_set_bit_map
*    autopvcIsActive
*    commit_and_save_autopvc
*    close_autopvc
**revision:
*     1.krammer 2009 0313
**____________________________________________________________________________*/
int
autopvcProbePvcAction(int adslState, int *replace){
	static int wait_time=1;//must be reset when jump to stop
	int local_not_finish_pvc=0;
	int i=0;

	if((!adslState) || (autopvcIsActive() != SUCCESS)){//jump to stop
		goto STOP;
	}
	else if(wait_time < AUTOPVC_WAIT_TIME){//no jump
		wait_time++;
	}
	else{//jump to probe_vc or finish
		wait_time=1;
		if(read_and_set_bit_map()!=SUCCESS){
			tcdbg_printf("can't get autopvc result, stop autopvc!\n");
			goto STOP;
		}
		//find not_finish_pvc
		for(i=0;i<PVC_NUM; i++){
			if((g_pvcPool[i].state == AUTOPVC_START)){
				local_not_finish_pvc++;
//				tcdbg_printf("pvc %d not finish\n",i);
			}
		}
		//parser pvc bit map
		for(i=0;i<PVC_NUM; i++){
			if((pvc_bit_map & (1<<i)) && (g_pvcPool[i].state == AUTOPVC_START)){
				local_not_finish_pvc--;
				g_pvcPool[i].state=AUTOPVC_SUCCESS;
//				tcdbg_printf("pvc %d success\n",i);
			}
		}
		if(local_not_finish_pvc){//jump to probe_vc
			autopvc_state=AUTOPVC_STATE_PROBE_VC_POOL;
			tcdbg_printf("probe vc pool\n");
			system("sys autopvc probe vcpool");
		}
		else{//jump to finish
			commit_and_save_autopvc(replace);
			close_autopvc();
			autopvc_state=AUTOPVC_STATE_FINISH;
		}
	}

	return local_not_finish_pvc;

	STOP:
		autopvc_state=AUTOPVC_STOP;
		close_autopvc();
		wait_time=1;//must be reset when jump to stop
		return local_not_finish_pvc;
}

/*______________________________________________________________________________
**function name: autopvcProbeVcPoolAction
**
**description:
*    do the action when probe_vc state
**parameters:
*    adslState: the state of adsl now
*    replace: replace table for autopvc
*    not_finish_pvc: the remaining pvc in start state
**global:
*    autopvc_state
**return:
*    local_not_finish_pvc: used to update new not_finish_pvc
**call:
*    read_and_set_bit_map
*    autopvcIsActive
*    commit_and_save_autopvc
*    close_autopvc
**revision:
*     1.krammer 2009 0313
**____________________________________________________________________________*/
int
autopvcProbeVcPoolAction(int adslState, int *replace, int not_finish_pvc){
	static int wait_time=1;//must be reset when jump to stop
	static int repeat_time=0;//must be reset when jump to stop
	int local_not_finish_pvc=not_finish_pvc;
	int i=0;

	if((!adslState) || (autopvcIsActive() != SUCCESS)){//jump to stop
		goto STOP;
	}
	else if(wait_time < AUTOPVC_WAIT_TIME){//no jump
		wait_time++;
	}
	else{//no jump or jump to finish
		wait_time=1;
		if(read_and_set_bit_map()!=SUCCESS){
			tcdbg_printf("can't get autopvc result, stop autopvc!\n");
			goto STOP;
		}
		//parser vc pool bit map
		for(i=0;i<MAX_VCPOOL_NUM; i++){
			if((vcpool_bit_map & (1<<i)) && (check_vpi_vci(g_vcPool[i].vpi,g_vcPool[i].vci) == SUCCESS)
					&& (g_pvcPool[g_vcPool[i].pvc].state == AUTOPVC_START)){
				local_not_finish_pvc--;
				g_pvcPool[g_vcPool[i].pvc].state=AUTOPVC_SUCCESS_AND_REPLACE;
				replace[g_vcPool[i].pvc]=i;
//				tcdbg_printf("vc %d success, so pvc %d success\n",i,g_vcPool[i].pvc);
				if(!local_not_finish_pvc){//if there is no not_finish_pvc ,we quit.
					break;
				}
			}
		}
		if(local_not_finish_pvc && (repeat_time<MAX_REPEAT_TIME)){//no jump
			repeat_time++;
			system("sys autopvc probe vcpool");
			tcdbg_printf("probe vcpool\n");
		}
		else{//jump to finish
			repeat_time=0;
			commit_and_save_autopvc(replace);
			close_autopvc();
			autopvc_state=AUTOPVC_STATE_FINISH;
		}
	}

	return local_not_finish_pvc;

	STOP:
		close_autopvc();
		wait_time=1;//must be reset when jump to stop
		repeat_time=0;//must be reset when jump to stop
		autopvc_state=AUTOPVC_STOP;
		return local_not_finish_pvc;
}

/*______________________________________________________________________________
**function name: autopvcFinishAction
**
**description:
*    do the action when finish state
**parameters:
*    adslState: the state of adsl now
**global:
*    autopvc_state
**return:
*    none
**call:
*    none
**revision:
*     1.krammer 2009 0313
**____________________________________________________________________________*/
void
autopvcFinishAction(int adslState){
	if(!adslState){//jump stop
		autopvc_state=AUTOPVC_STATE_STOP;
	}
}

/*______________________________________________________________________________
**function name: autopvc_state_machine
**
**description:
*    the daemon of autopvc
**parameters:
*    adslState: the state of adsl now
**global:
*    autopvc_state
**return:
*    none
**call:
*    autopvcStopAction
*    autopvcProbePvcAction
*    autopvcProbeVcPoolAction
*    autopvcFinishAction
**revision:
*     1.krammer 2009 0313
**____________________________________________________________________________*/
void
autopvc_state_machine(int adslState){
	static int replace[PVC_NUM]={0};//must be reset in stop
	static int not_finish_pvc=0;//must be reset when probe_pvc

	switch(autopvc_state){
	case AUTOPVC_STATE_STOP:
		autopvcStopAction(adslState,replace);//reset replace
		break;
	case AUTOPVC_STATE_PROBE_PVC:
		not_finish_pvc=autopvcProbePvcAction(adslState,replace);//reset not_finish_pvc
		break;
	case AUTOPVC_STATE_PROBE_VC_POOL:
		not_finish_pvc=autopvcProbeVcPoolAction(adslState,replace,not_finish_pvc);
		break;
	case AUTOPVC_STATE_FINISH:
		autopvcFinishAction(adslState);
		break;
	default:
		tcdbg_printf("autopvc is in a strange state!\n");
		break;
	}
}

/*______________________________________________________________________________
**function start_autoPVC
**
**description:
*    commit autopvc
**parameters:
*    none
**global:
*    none
**return:
*    none
**call:
*    tcapi_commit
**revision:
*     1.krammer 2009 0311
**____________________________________________________________________________*/
void
start_autoPVC(){
	char nodeName[8]="AutoPVC";
	tcapi_commit(nodeName);
	return;
}
/*______________________________________________________________________________
**function close_autopvc
**
**description:
*    close autopvc
**parameters:
*    none
**global:
*    none
**return:
*    none
**call:
*    reset_all_autopvc_data
**revision:
*     1.krammer 2009 0311
**____________________________________________________________________________*/
void
close_autopvc(){
	tcdbg_printf("close autopvc\n");
	system("sys autopvc active 0");
	reset_all_autopvc_data();
}

/*______________________________________________________________________________
**function read_and_set_bit_map
**
**description:
*    read result from proc and set the bit map according to result.
**parameters:
*    pvc_bit_map: the pvc bit map
*    vcpool_bit_map: the vcpool bit map
**global:
*    none
**return:
*    0:SUCCESS
*    -1:FAIL
**call:
*    none
**revision:
*     1.krammer 2009 0311
**____________________________________________________________________________*/
int
read_and_set_bit_map(void){

	int i=0;
	FILE *fp;
	unsigned int tmp=0;
	char stream[64]={0};
	char p[8]={0};

	fp = fopen("/proc/tc3162/autopvc", "r");
	if(!fp){
		tcdbg_printf("/proc/tc3162/autopvc fail\n");
		return FAIL;
	}

	for(i=0;i<2;i++){
		if(fgets(stream, 64, fp) != NULL){
			sscanf(stream,"%s %u", p, &tmp);
			if(!strcmp(p,"pvc")){
				pvc_bit_map=tmp;
//				tcdbg_printf("pvc bit map:%d\n",pvc_bit_map);
			}
			if(!strcmp(p,"vcpool")){
				vcpool_bit_map=tmp;
//				tcdbg_printf("vc pool bit map:%u\n",vcpool_bit_map);
			}
		}
	}
	fclose(fp);
	return SUCCESS;
}

/*______________________________________________________________________________
**function autopvcIsActive
**
**description:
*    see if autopvc is active.
**parameters:
*    none
**global:
*    none
**return:
*    0:SUCCESS
*    -1:FAIL
**call:
*    none
**revision:
*     1.krammer 2009 0311
**____________________________________________________________________________*/
int
autopvcIsActive(){
	char NodeName[MAX_NODE_NAME];
	char tmp[4]={0};
	memset(NodeName,0,sizeof(NodeName));
	strcpy(NodeName,"AutoPVC_Common");
	tcapi_get(NodeName,"Active",tmp);
	if(!strcmp(tmp,"1")){
		return SUCCESS;
	}
	else{
		return FAIL;
	}
}

/*______________________________________________________________________________
**function commit_and_save_autopvc
**
**description:
*    if there are some pvcs should be set, set and commit it. And save!
**parameters:
*    replace: replace table
**global:
*    none
**return:
*    none
**call:
*    set_success_pvc
**revision:
*     1.krammer 2009 0311
**____________________________________________________________________________*/
void
commit_and_save_autopvc(int *replace){
	int i=0;
	char nodeName[16]={0};
	int done=0;
	for(i=0;i<PVC_NUM;i++){
		if(g_pvcPool[i].state==AUTOPVC_SUCCESS_AND_REPLACE){
			set_success_pvc(i,replace[i]);
			sprintf(nodeName, "Wan_PVC%d", i);
//			tcdbg_printf("commit wan pvc %d\n",i);
			tcapi_commit(nodeName);
			done++;
		}
		if(g_pvcPool[i].state==AUTOPVC_SUCCESS){
			done++;
//			tcdbg_printf("pvc %d success\n",i);
		}
	}
	if(done){
		tcdbg_printf("tcapi save\n");
		tcapi_save();
	}
}
/*______________________________________________________________________________
**function reset_fork_or_not
**
**description:
*    set global variable variable fork_or_not
**parameters:
*    pvc: pvc index
**global:
*    fork_or_not
**return:
*    none
**call:
*    none
**revision:
*     1.rodney_20090420
**____________________________________________________________________________*/
void
reset_fork_or_not(int pvc){
    fork_or_not[pvc] = 1;
}  /*end reset_fork_or_not*/

/*______________________________________________________________________________
**function set_success_pvc
**
**description:
*    set the vpi vci from vcpool to pvc
**parameters:
*    pvcIndex: replaced pvc index
*    vcpoolIndex: the vcpool index we want to replace pvc
**global:
*    none
**return:
*    none
**call:
*    set_success_pvc
**revision:
*     1.krammer 2009 0311
**____________________________________________________________________________*/
void
set_success_pvc(int pvcIndex,int vcpoolIndex){
//	tcdbg_printf("PVC%d:",pvcIndex);
	char old_vpi[8], old_vci[8], tmp[32],WanNodeName[MAX_NODE_NAME],AutopvcNodeName[MAX_NODE_NAME];
	char encap[32];
	char *p=NULL;

	memset(WanNodeName,0,sizeof(WanNodeName));
	memset(AutopvcNodeName,0,sizeof(AutopvcNodeName));
	memset(tmp,0,sizeof(tmp));
	memset(encap,0,sizeof(encap));

	sprintf(WanNodeName,"%s%d","Wan_PVC",pvcIndex);
	tcapi_get(WanNodeName,WAN_VPI,old_vpi);//buffer old vpi vci
	tcapi_get(WanNodeName,WAN_VCI,old_vci);

	sprintf(tmp,"%d",g_vcPool[vcpoolIndex].vpi);//set wan vpi
	tcapi_set(WanNodeName, WAN_VPI, tmp);
	memset(tmp,0,sizeof(tmp));

	sprintf(tmp,"%d",g_vcPool[vcpoolIndex].vci);//set wan vci
	tcapi_set(WanNodeName, WAN_VCI, tmp);
	memset(tmp,0,sizeof(tmp));

	tcapi_get(WanNodeName, WAN_ENCAP, tmp);
	p=strstr(tmp,WAN_LLC);
	if(!p){
		p=strstr(tmp,WAN_VC);
		if(!p){
			tcdbg_printf("the encap of current PVC is wrong!!\n");
			return;
		}
	}
	strncpy(encap,tmp,(p-tmp));//set wan encap
	if(g_vcPool_encap == 0){//llc
		strcat(encap,"LLC\0");
		tcapi_set(WanNodeName, WAN_ENCAP, encap);
	}
	else{//vc
		strcat(encap,"VC-Mux\0");
		tcapi_set(WanNodeName, WAN_ENCAP, encap);
	}

	sprintf(AutopvcNodeName,"%s%d","AutoPVC_Entry",vcpoolIndex);
	tcapi_set(AutopvcNodeName, WAN_VPI, old_vpi);
	tcapi_set(AutopvcNodeName, WAN_VCI, old_vci);
}

#ifdef TR068_LED
int ismylanaddr(char* IPAddr)
{
	char IPBuf[16] = {0}, netMaskBuf[16] = {0};
	struct in_addr LANIP;
	struct in_addr LANSubnet;
	struct in_addr WANIP;	
	
	tcapi_get("Lan_Entry0", "IP", IPBuf);
	tcapi_get("Lan_Entry0", "netmask", netMaskBuf);
	
	inet_aton(IPBuf, &LANIP);
	inet_aton(netMaskBuf, &LANSubnet);
	inet_aton(IPAddr, &WANIP);

	/*check subnet*/
	if((WANIP.s_addr & LANSubnet.s_addr) != (LANIP.s_addr & LANSubnet.s_addr))
		return -1;

	return 0;
}

/*______________________________________________________________________________
**function name:check_internet_connect
**
**description:
*    check internet connection status and write proc file to notify drivers to turn on/off led 
**parameters:
*    none
**global:
*    none
**return:
*    none
**call:
*    none
**revision:
*     1.yzwang 20100305
**____________________________________________________________________________*/
void
check_internet_connect(void)
{
	int isInternetLedon = 0;/* 1:IP Connected; 0:No IP Connected*/ 
	int isInternetLedLedTrying = 0;/* 1:adsl up and trying to get WAN IP; 0:adsl not up or can't found defaule router*/
	char cmd[64]={0};
	char nodeName[32] = {0};
	char ispValue[8] = {0};

#if !defined(TCSUPPORT_CT)		
	/*find default router pvc*/
	int nIspNode = -1;
	char wanIPValue[16] = {0};
	char defaultRoute[8] = {0};
	char adslLinkStatus[32];

	for(nIspNode=0; nIspNode<8; nIspNode++){
		sprintf(nodeName,"Wan_PVC%d", nIspNode);
		tcapi_get(nodeName, "ISP", ispValue);
		tcapi_get(nodeName, "DEFAULTROUTE", defaultRoute);
		
		if(strcmp(ispValue, ISP_BRIDGE) == 0)
			continue;

		if(strcmp(defaultRoute, "Yes") == 0){
			break;
		}
	}

	if(nIspNode == 8) {// all PVCs are bridge mode or can't find isp
		isInternetLedon = 0;
		isInternetLedLedTrying = 0;
		goto write_proc;
	}
	
	sprintf(nodeName, "DeviceInfo_PVC%d", nIspNode);
	tcapi_get(nodeName, "WanIP", wanIPValue);	
	tcapi_get("Info_Adsl", "lineState", adslLinkStatus);	
	if(strlen(wanIPValue) != 0 && ismylanaddr(wanIPValue) < 0) {//check wan ip
		isInternetLedon = 1;	//get WAN IP and IP is valid
	}
	else {
		if(strcmp(adslLinkStatus, "up") != 0) {//DSL not up
			isInternetLedLedTrying = 0;
		}
		else {
			isInternetLedLedTrying = 1;//DSL up and trying to get WAN IP
		}
	}				
			
#else
	char defaultRouteV4[8]={0};
	char defaultRouteV6[8]={0};
	char serviceName[64]={0};	
	int ret=0;
	int nIspEntry_V4 = -1;
	int nIspEntry_V6 = -1;
	int i=0;
	int dsl_pon_status=0;
	int isBridgeInterIf = 0;
	int isInternetIf = 0;
	char isActive[8]={0};
	
#if defined(TCSUPPORT_CT_PON)
		dsl_pon_status = is_pon_link_up();
#else
		dsl_pon_status = is_adsl_link_up();
#endif
	if(dsl_pon_status == 0) {//DSL or PON not up
		isInternetLedon = 0;
		isInternetLedLedTrying = 0;
		goto write_proc;
	}
	else
	{					
		for(i=0; i<MAX_WAN_IF_INDEX; i++)
		{
			sprintf(nodeName, "Wan_PVC%d_Entry%d", i/PVC_NUM, i%PVC_NUM);
			
			memset(isActive, 0, sizeof(isActive));
			ret = tcapi_get(nodeName,"Active",isActive);
			if((ret != 0) || (strcmp(isActive, "Yes") != 0))
				continue;
			
			memset(serviceName, 0, sizeof(serviceName));		
			ret = tcapi_get(nodeName,"ServiceList",serviceName);
			
			if(ret != 0)
				continue;
				
			if(strstr(serviceName, "INTERNET") != NULL)
			{
				isInternetIf = 1;//exist  internet interface.
				memset(ispValue,0, sizeof(ispValue));
				ret = tcapi_get(nodeName,"ISP",ispValue);
				
				if(strcmp(ispValue, ISP_BRIDGE) == 0)
				{
					isBridgeInterIf = 1;//exist bridge internet interface.
					break;
				}
			}	
		}
		
		if(isInternetIf == 0) {
			isInternetLedon = 0;
			isInternetLedLedTrying = 0;
			goto write_proc;
		}

	
		if(isBridgeInterIf)//exist bridge internet interface.
		{
			isInternetLedon = 1;
			isInternetLedLedTrying = 0;
			goto write_proc;
		}
		
		ret = tcapi_get("WanInfo_Common","DefRouteIndexv4",defaultRouteV4);
		if(ret != 0)
		{
			isInternetLedon = 0;
			isInternetLedLedTrying = 1;
			goto write_proc;
		}
		ret = tcapi_get("WanInfo_Common","DefRouteIndexv6",defaultRouteV6);
		if(ret != 0)
		{
			isInternetLedon = 0;
			isInternetLedLedTrying = 1;
			goto write_proc;
		}

		if(strcmp(defaultRouteV4, "N/A") != 0)
			nIspEntry_V4 = atoi(defaultRouteV4);
			
		if(strcmp(defaultRouteV6, "N/A") != 0)
			nIspEntry_V6 = atoi(defaultRouteV6);
		
		if((nIspEntry_V4 == -1) && (nIspEntry_V6 == -1))
		{
			isInternetLedon = 0;
			isInternetLedLedTrying = 1;
			goto write_proc;
		}
		else
		{
			isInternetLedon = 1;
			isInternetLedLedTrying = 0;
			goto write_proc;	
		}
	}

#endif


write_proc:	

#if defined(TCSUPPORT_CT_PON)
	sprintf(cmd, "%d %d", isInternetLedon, isInternetLedLedTrying);
	doValPut("/proc/tc3162/led_internet", cmd);
#else
	sprintf(cmd,"echo %d %d > /proc/tc3162/led_internet\n", isInternetLedon, isInternetLedLedTrying);
	system(cmd);
#endif

	return;
}
#endif

#if defined(TCSUPPORT_CT_PPPINFORM)
int bridge_connection_status = 0;           //1: up 0:down
char pppusername[64] = {0};
void check_bridge_connection(void){
	FILE *fp = NULL;
	int i = 0;
	char line_buf[128] = {0}, line_string[2][128] = {0}, pppusername_temp[64] = {0};
	int bridge_status_temp = 0;
	cwmp_msg_t message;
	
	fp=fopen("/proc/tc3162/pppusername","r");
	if(fp==NULL){
		return FAIL;
	}
	if(fgets(line_buf, 128, fp) == NULL)
		return FAIL;
	if(line_buf[0] != '\n')
		strtok(line_buf, "\n");
	fclose(fp);	

	if(strstr(line_buf, "up") != NULL){
		bridge_status_temp = 1;
		if(bridge_connection_status != bridge_status_temp || NULL == strstr(line_buf, "unchanged")){               //status: down -> up		
			tcdbg_printf("\r\n bridge status: down to up!linebuf is %s!\r\n", line_buf);
			bridge_connection_status = bridge_status_temp;
			if(strstr(line_buf, "unchanged") == NULL){
				memset(&message, 0, sizeof(message));
				message.cwmptype = 17;		
				tcdbg_printf("\r\n sent message to tr069!\r\n");
				if(sendmegq(1, &message, 0) < 0){
					tcdbg_printf("\r\nsend message to cwmp error!!");
				}
			}
		}
	}
	
	if(strstr(line_buf, "down") != NULL){
		bridge_status_temp = 0;
		if(bridge_connection_status != bridge_status_temp){               //status: up -> down
			tcdbg_printf("\r\n bridge status: up to down!\r\n");
			bridge_connection_status = bridge_status_temp;
		}
	}

	return;
}
#endif

#if defined(TCSUPPORT_CT_AUTOREGISTER)	

int auto_register(){	
	FILE *fp = NULL;	
	char line_string[2][128] = {0}, pppusername_temp[64] = {0};
	char register_status[8] = {0};
	
	fp=fopen("/proc/tc3162/pppusername","r");
	if(fp==NULL){
		return FAIL;
	}
	if(fgets(line_string[0], 128, fp) == NULL)
		return FAIL;
	if(fgets(line_string[1], 128, fp) == NULL)
		return FAIL;
	if(line_string[1][0] != '\n')
		strtok(line_string[1], "\n");
	
	fclose(fp); 

	if(strstr(line_string[1], "Username: ") != NULL){
		char *pUsername = strstr(line_string[1], "Username: ") + strlen("Username: ");
		strcpy(pppusername_temp, pUsername);
	}

	tcdbg_printf("\r\n tr069 begin to auto register!\r\n");
	
	tcapi_set("WebCurSet_Entry", "isLoidRegistering", "Yes");
	//tcapi_commit("WebCurSet_Entry");
	
	tcapi_set("Cwmp_Entry", "devregInform", "1");
	//tcapi_commit("Cwmp_Entry");
	
	tcapi_set("deviceAccount_Entry", "newStatus", "0");
	tcapi_set("deviceAccount_Entry", "userName", pppusername_temp);
		tcapi_set("deviceAccount_Entry", "registerResult", "99");
	
	tcapi_commit("deviceAccount_Entry");
	tcapi_save();

	return SUCCESS;
}

#if defined(TCSUPPORT_CT_ADSL_HN)
unsigned char isUp = 0;
unsigned char isBoottime = 1;
#endif

void check_bridge_status(void){
	FILE *fp = NULL;
	int i = 0;
	char line_buf[128] = {0}, pppusername_temp[64] = {0};
 	int bridge_status_temp = 0, bridge_connection_status = 0; //1: up 0:down
	
	fp=fopen("/proc/tc3162/pppusername","r");
	if(fp==NULL){
		return FAIL;
	}
	if(fgets(line_buf, 128, fp) == NULL)
		return FAIL;
	if(line_buf[0] != '\n')
		strtok(line_buf, "\n");
	fclose(fp);	

	if(strstr(line_buf, "up") != NULL){
		bridge_status_temp = 1;
		if(bridge_connection_status != bridge_status_temp || NULL == strstr(line_buf, "unchanged")){               //status: down -> up		
			tcdbg_printf("\r\n bridge status: down to up!linebuf is %s!\r\n", line_buf);
			bridge_connection_status = bridge_status_temp;
#if defined(TCSUPPORT_CT_ADSL_HN)
			memset(pppusername_temp, 0, sizeof(pppusername_temp));
			tcapi_get( "deviceAccount_Entry", "registerResult", pppusername_temp  );
			if(strcmp( "1", pppusername_temp) != 0 && 0 == isUp) 
			{
				isUp = 1;
				auto_register();
			}
			else if(strstr(line_buf, "unchanged") == NULL){
				isUp = 1;
				memset(pppusername_temp, 0, sizeof(pppusername_temp));
				tcapi_get("LogicID_Entry", "Username", pppusername_temp);
				fp = fopen("/proc/tc3162/pppusername","r");
				if(fp == NULL){
					return FAIL;
				}
				if(fgets(line_buf, 128, fp) == NULL)
				{
					fclose(fp);
					return FAIL;
				}
				if(fgets(line_buf, 128, fp) == NULL)
				{
					fclose(fp);
					return FAIL;
				}
				if(line_buf[0] != '\n')
					strtok(line_buf, "\n");
				fclose(fp); 

				char *pUsername = NULL;
				if(strstr(line_buf, "Username: ") != NULL){
					pUsername = strstr(line_buf, "Username: ") + strlen("Username: ");
				}

				if ( 1 == isBoottime
					&& 0 == strcmp(pppusername_temp, pUsername) )
				{
					/* first time, when proc's pppusername equal logicid 
					do not execute auto_register */
					; 
				}
				else
				{
					/* do auto_register when pppoe username changed */
					auto_register();
				}
				isBoottime = 0;
#else
			if(strstr(line_buf, "unchanged") == NULL){
				auto_register();
				tcdbg_printf("\r\n tr069 auto register!\r\n");
#endif
				
			}
		}
	}
	
	if(strstr(line_buf, "down") != NULL){
		bridge_status_temp = 0;
		if(bridge_connection_status != bridge_status_temp){               //status: up -> down
			tcdbg_printf("\r\n bridge status: up to down!\r\n");
			bridge_connection_status = bridge_status_temp;
		}
#if defined(TCSUPPORT_CT_ADSL_HN)
		isUp = 0;
#endif
	}

	return;
}

#endif

/*check if the pid exist or not */
int CheckPid(int pid){

	char pidstring[20] = {0};
	
	snprintf(pidstring, sizeof(pidstring), "/proc/%d/", pid);

	if(access(pidstring, F_OK) == 0){
		return 1;
	}
	else
		return 0;

}
#if defined(TCSUPPORT_CT_PPPOEPROXY)

int sig_pppoeproxy = 0;

/*______________________________________________________________________________
**function name:ifaceExist
**
**description:
*    to check a interface exist or not
**parameters:
*    iface: interface name
**global:
*    none
**return:
*    1:exist
*    0:not exist
*    -1:error
**call:
*    none
**revision:
*     1.Jianhua 20101014
**____________________________________________________________________________*/
int
ifaceExist(char *iface)
{
	struct ifreq ifr;
	int sock;

	if(iface == NULL || strlen(iface) == 0){
		return -1;
	}

	if((sock = socket(PF_PACKET, SOCK_RAW, ETH_P_IP)) < 0)
	{
		perror("socket");
		return -1;
	}
	memset(&ifr, 0, sizeof(ifr));
	ifr.ifr_ifindex = -1;
	strncpy(ifr.ifr_name, iface, sizeof(ifr.ifr_name));
	

	if ( ioctl(sock, SIOCGIFINDEX, &ifr) < 0)
	{
		perror("ioctl(SIOCGIFINDEX)");
		close(sock);
		return 0;
	}
	close(sock);
	if(ifr.ifr_ifindex >= 0){
		return 1;
	}
	return 0;
}

/*check if the wan interface up or not*/
int isWanPVCup(int ID){
#if defined(TCSUPPORT_CT_PON)
	int xpon_status = 0;
#endif
	char adsl_stat[32] = {0};
	char ifacename[20] = {0};

	snprintf(ifacename, sizeof(ifacename), "ppp%d", ID);

#if defined(TCSUPPORT_CT_PON)
	xpon_status = is_pon_link_up();
	if(xpon_status == 0){
#else
	fileRead("/proc/tc3162/adsl_stats",adsl_stat,sizeof(adsl_stat));
	if(strstr(adsl_stat,"down")){
#endif
		return -1;
	}
	if(ifaceExist(ifacename)==1){
		return 1;
	}
	else{
		return 0;
	}

}

/*check if the wan interface down or not*/
int CheckWanDown(char *pvcname){
	char temp[50] = {0};
	char pvc[40] = {0};
	char proxyenable[4] = {0};
	char active[4] = {0};
	int i = 0, j = 0;
	int ID = 0;
	int wanpvcstat = -1;
	int flag = 0;
	
	snprintf(temp, sizeof(temp), "Wan_%s", pvcname);
	tcapi_get(temp, "Active", active);
	tcapi_get(temp, "ProxyEnable", proxyenable);
	if((!strcmp(active,"Yes"))&&(atoi(proxyenable) == 1)){
		for(i = 0; i < PVC_NUM; i++){
			for(j = 0; j < PVC_ENTRY_NUM; j++){
				snprintf(pvc, sizeof(pvc), "PVC%d_Entry%d", i, j);
				if(!strcmp(pvcname, pvc)){
				ID = i*PVC_ENTRY_NUM + j;
				flag = 1;
				break;
				}
			}
			if(flag){
				flag = 0;
				break;
			}
		}
		wanpvcstat = isWanPVCup(ID);
		if(wanpvcstat == -1){
			return 1;
		}
		else if(wanpvcstat == 0){
			return 1;
		}else if(wanpvcstat == 1){
			return 0;
		}
		else{
			return 1;
		}
	}
}
#endif

void sig_check_pppoeproxy(int signo){
#if defined(TCSUPPORT_CT_PPPOEPROXY)	
	if (signo == SIGUSR1){
		sig_pppoeproxy = 1;
	}
	else if (signo == SIGUSR2){
		sig_pppoeproxy = 0;
	}
#endif
	return;
}

#if defined(TCSUPPORT_CT_PPPOEPROXY)
void check_pppoeproxy(void){
	FILE *fp = NULL,*fptmp = NULL,*fptmp2 = NULL;
	char temp[64] = {0};
	int i = 0;
	int j = 0;
	char proxyenable[4] = {0};
	char active[4] = {0};
	char pvcname[50] = {0};
	char lan_if[10] = {0};
	int pid = 0,pid2 = 0;
	int pvccnt[8][8] = {0};
	char temp2[20] = {0};
	char pidfile[20] = {0};
	int fd = 0;
	char content[5] = {0};
	int flag = 0;

/*signal check*/
	if(sig_pppoeproxy == 1){
		return;
	}

/*check if any PVC has been set PPPOE PROXY Function*/
	for(i=0;i<PVC_NUM;i++){
		for(j=0; j<PVC_ENTRY_NUM; j++){
			snprintf(temp, sizeof(temp), "Wan_PVC%d_Entry%d",i,j);
			tcapi_get(temp,"Active",active);
			tcapi_get(temp,"ProxyEnable",proxyenable);
			if((!strcmp(active,"Yes"))&&(atoi(proxyenable) == 1)){
				flag = 1;
				break;
			}
			else{
				if((i == (PVC_NUM-1))&&(j == (PVC_ENTRY_NUM-1))){
					return;
				}
			}
		}
		if(flag){
			flag = 0;
			break;
		}
		
	}
	fp=fopen("/tmp/pppoeproxymsg","r+");
	if(fp == NULL){
		return;
	}
	fptmp=fopen("/tmp/pppoeproxymsgtemp","w");
	if(fptmp == NULL){
		return;
	}
	memset(temp, 0, sizeof(temp));
	
	/*Update file pppoeproxymsg*/	
		while(fgets(temp, sizeof(temp), fp) != NULL){
			sscanf(temp,"%s %d %s",pvcname,&pid, lan_if);
			if(CheckWanDown(pvcname) == 1){
				kill(pid,SIGKILL);
				continue;			
			}
			if(CheckPid(pid) == 1){ 
				fputs(temp,fptmp);
			}		
		}	
		fclose(fp);
		fclose(fptmp);
		rename("/tmp/pppoeproxymsgtemp", "/tmp/pppoeproxymsg");

	/*Count client ppp for each recorded PVC in pppoeproxymsg*/	
	fptmp2=fopen("/tmp/pppoeproxymsg","r");
	memset(temp, 0, sizeof(temp));
	if(fptmp2){
			while(fgets(temp,sizeof(temp),fptmp2) != NULL){
				sscanf(temp, "%s %d %s", pvcname, &pid, lan_if);
				for(i = 0; i < PVC_NUM; i ++){
					for(j = 0; j <PVC_ENTRY_NUM; j ++){
					snprintf(temp2, sizeof(temp2), "PVC%d_Entry%d", i, j);
					if(!strcmp(pvcname, temp2)){
						flag = 1;
						pvccnt[i][j] ++;
						break;
					}
						}
					if(flag){
						flag = 0;
						break;
					}
				}
			}
			for(i = 0;i < PVC_NUM; i++){
				for(j = 0; j < PVC_ENTRY_NUM; j++){
				snprintf(temp, sizeof(temp), "Wan_PVC%d_Entry%d",i,j);
				tcapi_get(temp,"Active",active);
				tcapi_get(temp,"ProxyEnable",proxyenable);
				if((!strcmp(active,"Yes"))&&(atoi(proxyenable) == 1)&&(pvccnt[i][j] == 0)){
					snprintf(pidfile, sizeof(pidfile), "/var/run/ppp%d.pid",(i*PVC_ENTRY_NUM+j+PPPOEPROXYOFFSET));
					fd=open(pidfile,O_RDONLY);
					if(fd!= -1){
						read(fd,content,5);
						pid2 = atoi(content);
						close(fd);
						if(CheckPid(pid2) == 1){
#if defined(TCSUPPORT_CT_PON_JS)
							kill(pid2,SIGTERM);
							sleep(3);
							if( 1 == CheckPid(pid2) )
								kill(pid2,SIGKILL);
#else
							kill(pid2,SIGKILL);
#endif
						}
					}
				}
			}
				}
			fclose(fptmp2);
	}
	return;
}
#endif

#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT)
int is_portal_on(void)
{
	char tmp[4] = {0};
	tcapi_get("Portal_Entry", "Enable", tmp);
	
	if (!strcmp(tmp, "1")){
		return SUCCESS;
	}
	else{
		return FAIL;
	}
}
#endif

#if defined(TCSUPPORT_AUTO_DMZ)
int
getArpTable(struct arpHead *pHead, char *iface)
{
	FILE *fp = NULL;
	struct arpEntry *pArpEntry = NULL, *pArpTmp = NULL;
	struct arpEntry *pTail = NULL;
	char line[200];
	int num;
	int isDevListNull;
	
	if ((fp = fopen(_PATH_PROCNET_ARP, "r")) == NULL) {
		perror(_PATH_PROCNET_ARP);
		return -1;
    	}
	if(pHead->first == NULL){
		isDevListNull = 1;
	}
	else{
		isDevListNull = 0;
	}
	pTail = pHead->tail;
	
    	/* Bypass header -- read until newline */
	if (fgets(line, sizeof(line), fp) != (char *) NULL) {
		/* Read the ARP cache entries. */
		for (; fgets(line, sizeof(line), fp);) {
			pArpEntry = (struct arpEntry *)malloc(sizeof(struct arpEntry));
			//memset(pArpEntry, 0, sizeof(struct arpEntry);
			if(pArpEntry == NULL){
				fclose(fp);
				return -1;
			}

			num = sscanf(line, "%s 0x%x 0x%x %100s %100s %100s\n",
			             pArpEntry->ip, &(pArpEntry->type), &(pArpEntry->flags), pArpEntry->hwa, pArpEntry->mask, pArpEntry->dev);
			if (num < 4){
				free(pArpEntry);
				break;
			}

			if (iface && strcmp(pArpEntry->dev, iface)){
				free(pArpEntry);
				continue;
			}

			/* incomplete entry, ignore it */
			if(!(pArpEntry->flags & ATF_COM)){
				free(pArpEntry);
				continue;
			}

			if(isDevListNull){
				if(pHead->first == NULL){
					pArpEntry->next = NULL;
					pHead->first = pArpEntry;
					pHead->tail = pArpEntry;
				}
				else{
					pArpEntry->next = pHead->first;
					pHead->first = pArpEntry;				
				}
				
			}
			else{
				pArpTmp = pHead->first;
				while(pArpTmp){
					if(strcmp(pArpEntry->ip, pArpTmp->ip) == 0)
						break;
					pArpTmp = pArpTmp->next;
				}

				if(pArpTmp == NULL){/* didn't find arp entry in g_devList, add it to tail */
					if(pTail->next == NULL){
						pArpEntry->next = NULL;
						pHead->tail = pArpEntry;
						pTail->next = pArpEntry;
					}
					else{
						pArpEntry->next = pTail->next;
						pTail->next = pArpEntry;
					}
				}
				else{
					free(pArpEntry);
				}

			}
			
			
			
	        }
	}
	
	fclose(fp);
	
	return 0;
}

int 
inet_open_rawsock(char *iface, int protocol)
{
	int sock;

	if((sock = socket(PF_PACKET, SOCK_RAW, htons(protocol))) < 0)
	{
		perror("socket");
		return -1;
	}

    
	if(iface != NULL)
	{
		struct ifreq ifr;
		struct sockaddr_ll sll;

		memset(&ifr, 0, sizeof(ifr));
		strncpy(ifr.ifr_name, iface, sizeof(ifr.ifr_name));    

		if ( ioctl(sock, SIOCGIFINDEX, &ifr) < 0)
		{
			perror("ioctl(SIOCGIFINDEX)");
			close(sock);
			return -1;
		}

		memset(&sll, 0, sizeof(sll));
		sll.sll_family = AF_PACKET;
		sll.sll_ifindex = ifr.ifr_ifindex;
		sll.sll_protocol = htons(protocol);

		if ( bind(sock, (struct sockaddr *) &sll, sizeof(sll)) == -1)
		{
			perror("bind()");
			close(sock);
			return -1;
		}
	}

    return sock;
}

int 
inet_get_ifaddr(char *iface, unsigned char* mac, unsigned int *ip)
{

	int sock;
	struct ifreq ifr;
	if((sock = inet_open_rawsock(iface, ETH_P_IP)) < 0){
		return -1;
	}

	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, iface, sizeof(ifr.ifr_name));

	if (mac != NULL)
	{
		if ( ioctl(sock, SIOCGIFHWADDR, &ifr) < 0 )
		{
			perror("ioctl(SIOCGIFHWADDR)");
			close(sock);
			return -1;
		}
		memcpy(mac, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
	}

	if (ip != NULL)
	{
		if ( ioctl(sock, SIOCGIFADDR, &ifr) < 0 )
		{
			perror("ioctl(SIOCGIFADDR)");
			close(sock);
			return -1;
		}
		memcpy((char *)ip, ifr.ifr_addr.sa_data+2, sizeof(*ip));
	}
	close(sock);
	return 0;
}

int 
forge_arp_request(arp_packet_t *pkt, unsigned char *smac,
                              unsigned int sip, unsigned int tip)
{
    memset(pkt->ethhdr.tmac, 0xFF, ETH_ALEN);
    memcpy(pkt->ethhdr.smac, smac, ETH_ALEN);
    pkt->ethhdr.type     = htons(ETH_P_ARP);
    pkt->arphdr.hwtype  = htons(HW_TYPE_ETHERNET);
    pkt->arphdr.prot    = htons(ETH_P_IP);
    pkt->arphdr.hwlen   = ETH_ALEN;
    pkt->arphdr.protlen = 4;   /* ipv4 */
    pkt->arphdr.opcode  = htons(ARP_OPCODE_REQUEST);
    memcpy(pkt->arphdr.smac, smac, ETH_ALEN);;
    pkt->arphdr.sip     = sip;
    memset(pkt->arphdr.tmac, 0, ETH_ALEN);
    pkt->arphdr.tip     = tip;

    return 0;
}

static int
get_arp_reply(int sock, arp_packet_t *pkt, int timeout)
{
	fd_set rdfds;
	struct timeval tv;

	memset(pkt, 0, sizeof(arp_packet_t));
	tv.tv_sec = timeout;
	tv.tv_usec = 0;//5000;

	FD_ZERO(&rdfds);
	FD_SET(sock, &rdfds);

	switch(select(sock + 1, &rdfds, NULL, NULL, &tv))
	{
	    case -1:
	        if (EINTR == errno)
	            break;
	        perror("select");
	    case 0:
	        /* timeout */
	        return -1;
	    default: 
	        if(FD_ISSET(sock, &rdfds))
	        {
	            if(recv(sock, pkt, sizeof(*pkt), 0) < 0 )
	            {
	                perror("recv");
	                return -1;
	            }
	        }
	}

    return 0;
}

/** 
 * Send a arp request and wait for response in timeout seconds, then
 * return the resolved ip address.
 * 
 * @param iface 
 * @param mac 
 * @param timeout 
 * 
 * @return 
 */
int
getArpReply(char *iface, unsigned int tip, int timeout)
{
	int sock;
	arp_packet_t pkt;
	unsigned int sip;
	char smac[ETH_ALEN];
	int ret = 0;

	if((sock = inet_open_rawsock(iface, ETH_P_ARP)) < 0){
		return -1;
	}
	if(inet_get_ifaddr(iface, smac, &sip) < 0){
		ret = -1;
		goto err;
	}
    
	forge_arp_request(&pkt, smac, sip, tip);

	if(send(sock, &pkt, sizeof(pkt), 0) < 0)
	{
		perror("send");
		ret = -1;
		goto err;
	}

	if(get_arp_reply(sock, &pkt, timeout) < 0)
	{
		perror("recv");
		ret = -1;
		goto err;
	}
 	if(pkt.arphdr.opcode == ARP_OPCODE_REPLY && pkt.arphdr.sip == tip){
		ret = 0;
 	}
	else{
		ret = -1;
	}
err:

	close(sock);
	return ret;
}

/* Delete an entry from the ARP cache. */
static 
int arp_del(struct arpHead *pHead, struct arpEntry *pArpEntry)
{
	struct arpreq req;
	struct sockaddr_in *sin;
	struct arpEntry *pArpPre = NULL;
	int sockfd = 0;

	memset((char *) &req, 0, sizeof(req));

	sin = (struct sockaddr_in *)&req.arp_pa;
	sin->sin_family = AF_INET;
	sin->sin_addr.s_addr = inet_addr(pArpEntry->ip);
    	req.arp_flags = ATF_PERM;
    	strcpy(req.arp_dev, LAN_IF);
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		return -1;
	}

	/* Call the kernel. */
	if (ioctl(sockfd, SIOCDARP, &req) < 0) {
		perror("SIOCDARP(priv)");
		return -1;
	}

	/* also delete it from arp list */
	if(pHead->first == pArpEntry){
		pHead->first = pArpEntry->next;
		if(pArpEntry == pHead->tail){
			pHead->tail = NULL;
		}
	}
	else{
	   	pArpPre = pHead->first;
		while(pArpPre->next){
			if(pArpPre->next == pArpEntry){
				break;
			}
			pArpPre = pArpPre->next;		
		}
		pArpPre->next = pArpEntry->next;	

		if(pArpEntry == pHead->tail){
			pHead->tail = pArpPre;
		}
	}
	
	free(pArpEntry);
	return 0;
}
void
refreshArpTable(struct arpHead *pHead)
{
	struct arpEntry *pArpEntry = NULL, *pArpNext = NULL;
	unsigned int ip;
	pArpEntry = pHead->first;
	int timeout = 1; /* 1s */
	while(pArpEntry){
		pArpNext = pArpEntry->next;
		inet_aton(pArpEntry->ip, (struct in_addr *)&ip);
		if(getArpReply(LAN_IF, ip, timeout) != 0){
			/* delete arp entry here */
			arp_del(pHead, pArpEntry);
		}
		else{
			break;
		}
		pArpEntry = pArpNext;
	}
}

void printArpTable(struct arpHead *pHead)
{
	struct arpEntry *pArpEntry = NULL;

	pArpEntry = pHead->first;
	while(pArpEntry){
		pArpEntry = pArpEntry->next;
		tcdbg_printf("IP:%s -- MAC:%s\n", pArpEntry->ip, pArpEntry->hwa);
	}
}

void 
freeArpTable(struct arpEntry *pHead)
{
	struct arpEntry *pCur = NULL, *pNext = NULL;

	pCur = pHead->next;
	while(pCur){
		pNext = pCur->next;
		free(pCur);
		pCur = pNext;
	}
}

static char g_autoDMZIP[16];
struct arpHead g_devList;
void
autoDMZCheck(void)
{
	static int second = 0;
	static int needInit = 1;
	int interval = 0;
	char sInterval[8] = {0};
	char oldDmzIp[16] = {0};
	char sDmz_Active[8] = {0};
	int DMZUpdata_Active = 0;

	char auto_dmz[8] = {0}; 
	int valid_if[MAX_WAN_IF_INDEX] = {0};
	int valid_if_num = 0;

	int i = 0;
	char dmzNode[12] = {0};

	if(needInit){
		g_devList.first = NULL;
		g_devList.tail = NULL;
		needInit = 0;
	}
	sprintf(dmzNode, DMZ_COMMON);
	if(tcapi_get(dmzNode, DMZ_INTERVAL, sInterval) != TCAPI_PROCESS_OK)
	{
		strcpy(sInterval,DEFAULT_DMZ_INTERVAL);
	}

	//get the update flag
	if(tcapi_get(dmzNode, UPDATE_DMZ_ACTIVE,sDmz_Active) != TCAPI_PROCESS_OK)
	{
		strcpy(sDmz_Active, "0");
	}

	DMZUpdata_Active = atoi(sDmz_Active);
	//clean the flag
	if(DMZUpdata_Active)
		tcapi_set(dmzNode, UPDATE_DMZ_ACTIVE, "0");

	interval = atoi(sInterval);
	if(interval == 0)
		interval = 30;
	
	if(((second++) % interval == 0) || DMZUpdata_Active == 1)		//when save dmz web, update lan ip immediately
	{				
		if(getArpTable(&g_devList, LAN_IF) == -1)
			return;
		refreshArpTable(&g_devList);

		if(g_devList.first == NULL)
		{
			if(strlen(g_autoDMZIP) > 0)
			{
				g_autoDMZIP[0] = '\0';

				valid_if_num = get_all_wan_index(valid_if, MAX_WAN_IF_INDEX);
				for(i=0; i<valid_if_num; i++)
				{
					sprintf(dmzNode, "Dmz_Entry%d", valid_if[i]);
					if(tcapi_get(dmzNode, AUTO_DMZ_FLAG, auto_dmz) != TCAPI_PROCESS_OK)
					continue;
					if(atoi(auto_dmz) == 0)						//no support auto dmz
					continue;
					tcapi_set(dmzNode, "DMZ_IP", "0.0.0.0");
					tcapi_commit(dmzNode);
				}
			}	
		}
	
		else 
		{
			valid_if_num = get_all_wan_index(valid_if, MAX_WAN_IF_INDEX);
			for(i=0; i<valid_if_num; i++)
			{
				sprintf(dmzNode, "Dmz_Entry%d", valid_if[i]);
			//	tcdbg_printf("Node: %s\n",dmzNode);
				
				if(tcapi_get(dmzNode, AUTO_DMZ_FLAG, auto_dmz) != TCAPI_PROCESS_OK)
					continue;
				if(atoi(auto_dmz) == 0)					//no support auto dmz,if dmz is not active ,the auto flag is 0
					continue;
				if(tcapi_get(dmzNode, "DMZ_IP", oldDmzIp) != TCAPI_PROCESS_OK)
					return;
			
				if((strcmp(g_autoDMZIP, g_devList.first->ip) != 0) ||
					(strcmp(oldDmzIp, g_devList.first->ip) != 0))
					tcapi_set(dmzNode, "DMZ_IP", g_devList.first->ip);
				tcapi_commit(dmzNode);
			}
			strncpy(g_autoDMZIP, g_devList.first->ip, sizeof(g_autoDMZIP));	
		}
	}
}
#endif
#if defined(TCSUPPORT_CT)
int checkUILockState()
{
	char node_name[32] = {0};
	char lock_state[4] = {0};
	int isLock = 0;
	struct timespec curtime;
	static struct timespec prev_time;
	static int isStoreTime = 0;

	if ( TCAPI_PROCESS_OK != tcapi_get("WebCurSet_Entry", "isLockState", lock_state) )
		return -1;

	isLock = atoi(lock_state);
	if ( isLock )
	{
		if ( !isStoreTime )
		{
			clock_gettime(CLOCK_MONOTONIC, &prev_time);
			isStoreTime = 1;
		}

		clock_gettime(CLOCK_MONOTONIC, &curtime);
		if ( (curtime.tv_sec - prev_time.tv_sec)  >= 60 )
		{
			isStoreTime = 0;
			tcapi_set("WebCurSet_Entry", "isLockState", "0");
		}
	}

	return 0;
}
#endif

#if defined(TCSUPPORT_CT_DSLITE)
extern int tc_getaddrinfo(const char *hostname, const char *service, const struct addrinfo *hints, struct addrinfo **result, int timeout);
static int dslite_state[MAX_WAN_IF_INDEX] = {0};
#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
extern int isPPPoE_Emulator_Start();
#endif

void set_dslite_internal_state(int if_index, int state)
{
	if ( if_index > MAX_WAN_IF_INDEX )
		return;

	if ( state )
		dslite_state[if_index] |= DS_ACTIVE;
	else
		dslite_state[if_index] &= ~DS_ACTIVE;
}
int get_dslite_internal_state(int if_index)
{
	if ( if_index > MAX_WAN_IF_INDEX )
		return;

	if (dslite_state[if_index] & DS_ACTIVE)
		return 1;
	else
		return 0;
}
void set_dslite_internal_bind_state(int if_index, int state)
{
	if ( if_index > MAX_WAN_IF_INDEX )
		return;

	if ( state )
		dslite_state[if_index] |= DS_BINDSTATE_UP;
	else
		dslite_state[if_index] &= ~DS_BINDSTATE_UP;
}
int get_dslite_internal_bind_state(int if_index)
{
	if ( if_index > MAX_WAN_IF_INDEX )
		return;

	if (dslite_state[if_index] & DS_BINDSTATE_UP)
		return 1;
	else
		return 0;
}

int get_dslite_stateviacfg(mxml_node_t *top, char nodeName[][MAX_NODE_NAME])
{
	char dslite_enable[8] = {0};
	char serviceList[64] = {0};
	char isp[5] = {0};

	if ( NULL == top )
		return 0;

	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "DsliteEnable", dslite_enable) )
		return 0;
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "ServiceList", serviceList) )
		return 0;
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "ISP", isp) )
		return 0;

	if( NULL != strstr(serviceList, "INTERNET")
		&& 0 != strcmp(isp, "3")  // not BRIDGE_MODE
		&& 0 == strcmp("Yes", dslite_enable))
		return 1;
	else
		return 0;
}

int stop_dslite(int pvc_index, int entry_index)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char str_ifname[16] = {0};
	char cmd[128] = {0};
	mxml_node_t *top = get_node_top();

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "Wan");
	sprintf(nodeName[1], "PVC%d", pvc_index);
	sprintf(nodeName[2], "Entry%d", entry_index);

	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "IFName", str_ifname) )
		return -1;

	sprintf(cmd, STOP_DSLITE, str_ifname);
	system(cmd);

	return 0;
}

char global_dslite_addr[120] = {0};

void notify_dslite_state()
{

	int valid_if[MAX_WAN_IF_INDEX] = {0};
	int valid_if_num = 0;
	char waninfo_node[32] = {0};
	char wan_addr[64] = {0};
	char dslite_addr[120] = {0};
	char dslite_domain[120] = {0};
	char dslite_enable[8] = {0};
	char dslite_mode[4] = {0};
	char dslite_ifname[16] = {0};
	char str_ifname[16] = {0};
	char cmd[128] = {0};
	char def_route_index_v4[10] = {0};
	char serviceList[64] = {0};
	char connactive[8] = {0};
	char ip_version[20] = {0};
	int idx = 0;
	int pvc_index = 0;
	int entry_index = 0;
	int if_index = 0;
	int i_dslite_enable = 0;
	int i_dslite_mode = -1; // 0:auto, 1:manual
	int ret1 = 0, ret2 = 0;
	mxml_node_t *top = NULL;
	char dslite_addr_buf[64] = {0};
	struct in6_addr in_dslite_v6addr = {0};
	struct sockaddr_in6 *p_dslite_v6addr = NULL;
	struct addrinfo hints = {0};
	struct addrinfo *res = NULL;

	if ( !g_wan_boot_finish )
		return;

#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
	if ( isPPPoE_Emulator_Start() )
		return;
#endif

	valid_if_num = get_all_wan_index(valid_if, MAX_WAN_IF_INDEX);
	for( idx = 0; idx < valid_if_num; idx ++)
	{
		if_index = valid_if[idx];
		if ( if_index > MAX_WAN_IF_INDEX )
			return;

		pvc_index = valid_if[idx] / MAX_SMUX_NUM;
		entry_index = valid_if[idx] % MAX_SMUX_NUM;

		memset(waninfo_node, 0, sizeof(waninfo_node));
		sprintf(waninfo_node, "Wan_PVC%d_Entry%d", pvc_index, entry_index);
		if( TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "DsliteEnable", dslite_enable) )
			continue;
		if( TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "DsliteMode", dslite_mode) )
			strcpy(dslite_mode, "0");
		if( TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "IFName", str_ifname) )
			continue;
		if( TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "ServiceList", serviceList) )
			continue;
		if( TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "Active", connactive) )
			continue;
		if( TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "IPVERSION", ip_version) )
			continue;

		if ( 0 != strcmp("Yes", connactive)
			|| 0 == strcmp("IPv4", ip_version) // dslite only for ipv6 or ipv4v6
			|| NULL == strstr(serviceList, "INTERNET"))
			continue;

		if ( 0 == strcmp("Yes", dslite_enable) )
			i_dslite_enable  = 1;
		else
			i_dslite_enable  = 0;
		i_dslite_mode = atoi(dslite_mode);

		if ( i_dslite_enable 
			&& get_dslite_internal_state(if_index) ) // ds-lite already activated.
		{
			if (get_dslite_internal_bind_state(if_index))
			{
#if defined(TCSUPPORT_PORTBIND)
				set_portbinding_info_for_dslite(pvc_index, entry_index);
#endif
				set_dslite_internal_bind_state(if_index, 0);
			}
			continue;
		}
		else if ( !i_dslite_enable 
			&& !get_dslite_internal_state(if_index) ) // ds-lite already deactivated.
			continue;

		/* get default route for ipv4 */
		memset(def_route_index_v4, 0, sizeof(def_route_index_v4));
		if ( TCAPI_PROCESS_OK != tcapi_get("WanInfo_Common", "DefRouteIndexv4", def_route_index_v4) )
			strcpy(def_route_index_v4, "N/A");

		if ( 1 == i_dslite_enable )
		{
			if ( 0 == i_dslite_mode ) // auto
			{
				memset(waninfo_node, 0, sizeof(waninfo_node));
				sprintf(waninfo_node, "WanInfo_Entry%d", if_index );
				ret1 = tcapi_get(waninfo_node, "DsliteAddr", dslite_addr);
				ret2 = tcapi_get(waninfo_node, "DsliteName", dslite_domain);
				if( ( TCAPI_PROCESS_OK != ret1 && TCAPI_PROCESS_OK != ret2 )
					|| ( '\0' == dslite_addr[0] && '\0' == dslite_domain[0] )
					|| TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "IP6", wan_addr) )
					continue;
			}
			else if ( 1 == i_dslite_mode ) // manual
			{
				memset(waninfo_node, 0, sizeof(waninfo_node));
				sprintf(waninfo_node, "Wan_PVC%d_Entry%d", pvc_index, entry_index);
				if( TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "DsliteAddr", dslite_addr) )
					continue;

				memset(waninfo_node, 0, sizeof(waninfo_node));
				sprintf(waninfo_node, "WanInfo_Entry%d", if_index );
				if( TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "IP6", wan_addr) )
					continue;
			}
			else
				continue;

			// check dslite address
			if ( 1 != inet_pton(AF_INET6, dslite_addr, &in_dslite_v6addr) )
			{
				if ( 0 == i_dslite_mode ) // auto
					strcpy(dslite_addr, dslite_domain);
				//resolve the domain
				memset( &hints, 0 , sizeof(hints));
				hints.ai_family = AF_INET6;
				hints.ai_socktype = SOCK_DGRAM; 		//DNS is UDP packet
				hints.ai_protocol = IPPROTO_UDP;

#if defined(TCSUPPORT_CT_PON_CY_JX)
				if ( 0 != tc_getaddrinfo(dslite_addr, NULL, &hints, &res, 5) )
					strcpy(dslite_addr, global_dslite_addr);
#else
/*				if ( 0 != tc_getaddrinfo(dslite_addr, NULL, &hints, &res, 5) )
					continue; */
				if(0 != getaddrinfo(dslite_addr, NULL, &hints, &res))
					continue;
#endif

#if defined(TCSUPPORT_CT_PON_CY_JX)
				if ( NULL == res || AF_INET6 != res->ai_family )
					strcpy(dslite_addr, global_dslite_addr);
#else
				if ( NULL == res || AF_INET6 != res->ai_family )
					continue;
#endif

				if (res != NULL) {
				p_dslite_v6addr = (struct sockaddr_in6 *)res->ai_addr;
				inet_ntop(AF_INET6, &p_dslite_v6addr->sin6_addr, dslite_addr_buf, INET6_ADDRSTRLEN);
				strcpy(dslite_addr, dslite_addr_buf);
			}
			}

#if defined(TCSUPPORT_CT_PON_CY_JX)
			if (!strcmp(dslite_addr, "::")) {
				strcpy(dslite_addr, "240e:cc:1000::1");
			}
			strcpy(global_dslite_addr, dslite_addr);
#endif
			
			if( strlen(dslite_addr) > 0 && strlen(wan_addr) > 0 )
			{
				sprintf(cmd, START_DSLITE, wan_addr, dslite_addr, str_ifname);
				system(cmd);
				/* update if_index to wanInfo node */
				if ( 0 == strcmp(def_route_index_v4, "N/A")
					|| atoi(def_route_index_v4) == def_route_index_v4  )
				{
					sprintf(dslite_ifname, "ds.%s", str_ifname);
					strcpy(cmd, "/sbin/route del default");
					system(cmd);
					sprintf(cmd, "/sbin/route add default dev %s", dslite_ifname);
					system(cmd);
					sprintf(def_route_index_v4, "%d", if_index);
					tcapi_set("WanInfo_Common", "DefRouteIndexv4", def_route_index_v4);
				}
				set_dslite_internal_state(if_index, 1);
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
				bzero(waninfo_node, sizeof(waninfo_node));
				sprintf(waninfo_node, "WanInfo_Entry%d", if_index );
				tcapi_set(waninfo_node, "DsliteUP", "Yes");
#endif
			}
		}
		else if ( 0 == i_dslite_enable )
		{
			sprintf(cmd, STOP_DSLITE, str_ifname);
			system(cmd);
			set_dslite_internal_state(if_index, 0);
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
			bzero(waninfo_node, sizeof(waninfo_node));
			sprintf(waninfo_node, "WanInfo_Entry%d", if_index );
			tcapi_set(waninfo_node, "DsliteUP", "No");
#endif

			if ( 0 != strcmp(def_route_index_v4, "N/A"))
			{
				/* default route for dslite interface is down, find another V4 INTERNET interface to replace it. */
				if (if_index == atoi(def_route_index_v4))
				{
					top = get_node_top();
					route_switch(top, def_route_index_v4, IF_STATE_DOWN);
					tcapi_set("WanInfo_Common", "DefRouteIndexv4", def_route_index_v4);
				}
			}
		}
	}
}
#endif

#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
extern int AddRouteforOnDemandWan(mxml_node_t *top, int pvc_index, int entry_index);
extern int isOndemandWan(mxml_node_t *top, int pvc_index, int entry_index);
static int ondemand_route_state[MAX_WAN_IF_INDEX] = {0};
void set_ondemand_route_state(int if_index, int state)
{
	if ( if_index > MAX_WAN_IF_INDEX )
		return;

	if ( state )
		ondemand_route_state[if_index] |= ONDEMAND_NEED_ADDROUTE;
	else
		ondemand_route_state[if_index] &= ~ONDEMAND_NEED_ADDROUTE;
}
int get_ondemand_route_state(int if_index)
{
	if ( if_index > MAX_WAN_IF_INDEX )
		return;

	if (ondemand_route_state[if_index] & ONDEMAND_NEED_ADDROUTE)
		return 1;
	else
		return 0;
}

void notify_OnDemandWan_Switch()

{
	int valid_if[MAX_WAN_IF_INDEX] = {0};
	int valid_ondemand_if[MAX_WAN_IF_INDEX] = {0};
	int valid_if_num = 0;
	char waninfo_node[32] = {0};
	char ispMode[12] = {0};
	char connection[32] = {0};
	char ifidxbuf[12] = {0};
	char str_ifname[16] = {0};
	char str_oldifname[16] = {0};
	char cmdbuf[128] = {0};
	char serviceList[64] = {0};
	char connactive[8] = {0};
	char ip_version[20] = {0};
	char wan_lasterror[128] = {0};
	int idx = 0;
	int pvc_index = 0;
	int entry_index = 0;
	int if_index = 0;
	int ipVersion = 0;
	char def_route_index_v4[10] = {0};
	int i_def_route_index_v4 = -1;
	char def_route_index_v6[10] = {0};
	int i_def_route_index_v6 = -1;
	char ondemand_route_index_v4[10] = {0};
	int i_ondemand_route_index_v4 = -1;
	char ondemand_route_index_v6[10] = {0};
	int i_ondemand_route_index_v6 = -1;
	char pidbuf[64] = {0};
	char pidPath[128] = {0};
	char pidallbuf[128] = {0};
	int isFoundOnDemand = 0;
	mxml_node_t *top = NULL;
	int isneedRestartDNS = 0;

	if ( !g_wan_boot_finish )
		return;

	top = get_node_top();
	for( idx = 0; idx < MAX_WAN_IF_INDEX; idx ++)
		valid_ondemand_if[idx] = -1;
	valid_if_num = get_all_wan_index(valid_if, MAX_WAN_IF_INDEX);
	for( idx = 0; idx < valid_if_num; idx ++)
	{
		if_index = valid_if[idx];
		if ( if_index > MAX_WAN_IF_INDEX )
			return;
		pvc_index = if_index / MAX_SMUX_NUM;
		entry_index = if_index % MAX_SMUX_NUM;

		if ( get_ondemand_route_state(if_index) )
		{
			memset(pidbuf, 0, sizeof(pidbuf));
			memset(pidPath, 0, sizeof(pidPath));

			// get ppp pid
			sprintf(pidPath, "sppp%d.pid", WAN_IF_PATH, if_index);
			fileRead(pidPath, pidbuf, sizeof(pidbuf) -1);

			memset(cmdbuf, 0, sizeof(cmdbuf));
			sprintf(cmdbuf, "pidof pppd > %s", PPPD_PID_TEMP_PATH);
			system(cmdbuf);
			fileRead(PPPD_PID_TEMP_PATH, pidallbuf, sizeof(pidallbuf) -1);
			unlink(PPPD_PID_TEMP_PATH);
			if ( NULL != strstr(pidallbuf, pidbuf) ) // pppd is up
			{
				if ( 0 == AddRouteforOnDemandWan(top, pvc_index, entry_index) )
					isneedRestartDNS = 1;
			}
			else
				continue;

			set_ondemand_route_state(if_index, 0 );
		}

		if ( 1 == isOndemandWan(top, pvc_index, entry_index) )
			valid_ondemand_if[idx] = if_index;
	}

	if ( 1 == isneedRestartDNS )
	{
		restart_dnsmasq();
	}
	// get default route index for V4&V6
	memset(def_route_index_v4, 0, sizeof(def_route_index_v4));
	memset(def_route_index_v6, 0, sizeof(def_route_index_v6));
	memset(waninfo_node, 0, sizeof(waninfo_node));
	strcpy(waninfo_node, "WanInfo_Common");
	if ( TCAPI_PROCESS_OK == tcapi_get(waninfo_node, WAN_COMMON_DEFROUTEIDXV4, def_route_index_v4)
		&& '\0' != def_route_index_v4[0] && 0 != strcmp(def_route_index_v4, "N/A") )
		i_def_route_index_v4 = atoi(def_route_index_v4);
	if ( TCAPI_PROCESS_OK == tcapi_get(waninfo_node, WAN_COMMON_DEFROUTEIDXV6, def_route_index_v6)
		&& '\0' != def_route_index_v6[0] && 0 != strcmp(def_route_index_v6, "N/A") )
		i_def_route_index_v6 = atoi(def_route_index_v6);
	
	if ( -1 != i_def_route_index_v4 && -1 != i_def_route_index_v6 )
		return; // no need to check when V4/V6 default route exist

	// get on-demand wan connection index for V4&V6
	memset(ondemand_route_index_v4, 0, sizeof(ondemand_route_index_v4));
	memset(ondemand_route_index_v6, 0, sizeof(ondemand_route_index_v6));
	if ( TCAPI_PROCESS_OK == tcapi_get(waninfo_node, WAN_COMMON_ONDEMANDIDXV4, ondemand_route_index_v4)
		&& '\0' != ondemand_route_index_v4[0] && 0 != strcmp(ondemand_route_index_v4, "N/A") )
		i_ondemand_route_index_v4 = atoi(ondemand_route_index_v4);
	if ( TCAPI_PROCESS_OK == tcapi_get(waninfo_node, WAN_COMMON_ONDEMANDIDXV6, ondemand_route_index_v6)
		&& '\0' != ondemand_route_index_v6[0] && 0 != strcmp(ondemand_route_index_v6, "N/A") )
		i_ondemand_route_index_v6 = atoi(ondemand_route_index_v6);

	if ( -1 != i_ondemand_route_index_v4 && -1 != i_ondemand_route_index_v6 )
		return; // no need to check when V4/V6 on-demand wan already exist

	// check self on-demand first
	if (  ( -1 == i_def_route_index_v4 && i_ondemand_route_index_v4 >= 0 )
		|| ( -1 == i_def_route_index_v6 && i_ondemand_route_index_v6 >= 0 ) )
	{
		if ( -1 == i_def_route_index_v4 && i_ondemand_route_index_v4 >= 0 )
		{
			if_index = i_ondemand_route_index_v4;
			ipVersion = 0;
		}
		else
		{
			if_index = i_ondemand_route_index_v6;
			ipVersion = 1;
		}

		pvc_index = if_index / MAX_SMUX_NUM;
		entry_index = if_index % MAX_SMUX_NUM;

		memset(waninfo_node, 0, sizeof(waninfo_node));
		sprintf(waninfo_node, "Wan_PVC%d_Entry%d", pvc_index, entry_index);

		if( TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "Active", connactive)
			|| 0 != strcmp("Yes", connactive)
			|| TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "ISP", ispMode)
			|| 0 != strcmp(ispMode, PPP_MODE)
			|| TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "CONNECTION", connection)
			|| 0 != strcmp(connection, "Connect_on_Demand")
			|| TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "ServiceList", serviceList)
			|| NULL == strstr(serviceList, "INTERNET")
			|| TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "IFName", str_oldifname)
			|| '\0' == str_oldifname[0] )
		{
			tcapi_set("WanInfo_Common", ( 0 == ipVersion ) ? WAN_COMMON_ONDEMANDIDXV4 : WAN_COMMON_ONDEMANDIDXV6, "N/A");
			return; 
		}

		if( TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "ConnectionError", wan_lasterror)
			|| ( 0 != strcmp(wan_lasterror, "ERROR_AUTHENTICATION_FAILURE")
			   &&  0 != strcmp(wan_lasterror, "ERROR_ISP_TIME_OUT")) )
			return; // no need to switch other on-demand wan connection.
	}


	if ( ( -1 != i_def_route_index_v4 && i_ondemand_route_index_v4 >= 0 )
		|| ( -1 != i_def_route_index_v6 && i_ondemand_route_index_v6 >= 0 ) )
		return; // no need to check when V4/V6 on-demand wan already exist and default route exist

	for( idx = 0; idx < MAX_WAN_IF_INDEX; idx ++)
	{
		if ( -1 != valid_ondemand_if[idx]
			&& i_ondemand_route_index_v4 != valid_ondemand_if[idx]
			&& i_ondemand_route_index_v6 != valid_ondemand_if[idx])
		{
			isFoundOnDemand = 1;
			break;
		}
	}

	if (  (( -1 == i_def_route_index_v4 && -1 == i_ondemand_route_index_v4 )
			|| ( -1 == i_def_route_index_v6 && -1 == i_ondemand_route_index_v6 ))
		&& ( 0 == isFoundOnDemand ))
		return; // no need to check when other on-demand wan isn't exist


	for( idx = 0; idx < valid_if_num; idx ++)
	{
		if_index = valid_if[idx];
		if ( if_index > MAX_WAN_IF_INDEX )
			return;

		if ( i_ondemand_route_index_v4 == if_index
			|| i_ondemand_route_index_v6 == if_index )
			continue;

		pvc_index = valid_if[idx] / MAX_SMUX_NUM;
		entry_index = valid_if[idx] % MAX_SMUX_NUM;
		sprintf(ifidxbuf, "%d", if_index);

		memset(waninfo_node, 0, sizeof(waninfo_node));
		sprintf(waninfo_node, "Wan_PVC%d_Entry%d", pvc_index, entry_index);

		if( TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "Active", connactive)
			|| 0 != strcmp("Yes", connactive) )
			continue;
		if( TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "ISP", ispMode)
			|| 0 != strcmp(ispMode, PPP_MODE) )
			continue;
		if( TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "CONNECTION", connection)
			|| 0 != strcmp(connection, "Connect_on_Demand") )
			continue;
		if( TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "IFName", str_ifname)
			|| '\0' == str_ifname[0] )
			continue;
		if( TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "ServiceList", serviceList)
			|| NULL == strstr(serviceList, "INTERNET") )
			continue;
		if( TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "IPVERSION", ip_version) )
			continue;
		if( TCAPI_PROCESS_OK != tcapi_get(waninfo_node, "ConnectionError", wan_lasterror)
			|| 0 == strcmp(wan_lasterror, "ERROR_AUTHENTICATION_FAILURE")
			|| 0 == strcmp(wan_lasterror, "ERROR_ISP_TIME_OUT"))
			continue;

		if ( 0 == strcmp(ip_version, "IPv4") )
			ipVersion = 0;
		else if ( 0 == strcmp(ip_version, "IPv6") )
			ipVersion = 1;
		else if ( 0 == strcmp(ip_version, "IPv4/IPv6") )
			ipVersion = 2;
		else
			continue;

		// switch to other V4 on-demand
		if ( ( 1 == isFoundOnDemand || i_ondemand_route_index_v4 >= 0 ) && ( 0 == ipVersion || 2 == ipVersion ) )
		{
			if ( '\0' != str_oldifname[0] )
			{
				sprintf(cmdbuf, "/sbin/route del default dev %s", str_oldifname);
				wan_cmd_excute(cmdbuf);
			}

			sprintf(cmdbuf, "/sbin/route add default dev %s", str_ifname);
			wan_cmd_excute(cmdbuf);
			tcapi_set("WanInfo_Common", WAN_COMMON_ONDEMANDIDXV4, ifidxbuf);
		}
		// switch to other V6 on-demand
		if ( ( 1 == isFoundOnDemand || i_ondemand_route_index_v6 >= 0 ) && ( 1 == ipVersion || 2 == ipVersion ) )
		{
			if ( '\0' != str_oldifname[0] )
			{
				sprintf(cmdbuf, "/sbin/route -A inet6 del default dev %s", str_oldifname);
				wan_cmd_excute(cmdbuf);
			}

			sprintf(cmdbuf, "/sbin/route -A inet6 add default dev %s", str_ifname);
			wan_cmd_excute(cmdbuf);
			tcapi_set("WanInfo_Common", WAN_COMMON_ONDEMANDIDXV6, ifidxbuf);
		}

		return;
	}
}
#endif

#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_STBMAC_REPORT)
void notify_STB_change()
{
	FILE *f = NULL;
	char tmpbuf[512] = {0};
	char statebuf[12] = {0};
	char maclist[256] = {0};
	static int stb_old_count = 0, stb_new = 0;
	int stbcount = 0;

	f = fopen(STB_LIST_PATH, "r");
	if ( f )
	{
		memset(tmpbuf, 0 , sizeof(tmpbuf));
		fgets(tmpbuf, sizeof(tmpbuf) - 1, f);
		sscanf(tmpbuf, "%[^=]=%d=%[^=]", statebuf, &stbcount, maclist);
		fclose(f);

		if ( 0 == strcmp("new", statebuf) )
		{
			stb_old_count = stbcount;
			stb_new = 1;
		}
		else
		{
			if ( stb_new && stb_old_count == stbcount )
			{
				stb_new = 0;
#ifdef CWMP
				signalCwmpInfo(14, 0, 0);
#endif
			}
			stb_old_count = stbcount;
		}
	}


}
#endif


#if defined(TCSUPPORT_LED_BTN_CHECK)
#if defined(TCSUPPORT_BTN_CHECK)
void 
check_button_type(void){
	char string[4] = {'\0'},cmd[100] = {'\0'},pts[15]={'\0'};
	int i, type, fd, ret;
	fd=open("/proc/tc3162/button_type", O_RDONLY|O_NONBLOCK);
	if(fd!=-1){
		ret = read(fd, string, 4);
		close(fd);
		if(ret){
			type = atoi(string);
	#ifdef TCSUPPORT_LED_SWITCH_BUTTON
			if((type > 0) && (type < 5))
	#else
			if((type > 0) && (type < 4))
	#endif
			{
				for(i=0; i<PTS_NUMBER; i++){
					memset(cmd, 0, 100);
					memset(pts, 0, 15);
					sprintf(pts, "/dev/pts/%d", i);
					fd=open(pts, O_RDONLY|O_NONBLOCK);
					if(fd != -1){
						close(fd);
						if(1 == type)
							sprintf(cmd, "echo %s > /dev/pts/%d\n", "Reset button is pressed!", i);
						else if(2 == type)
							sprintf(cmd, "echo %s > /dev/pts/%d\n", "Wifi button is pressed!", i);
#ifdef TCSUPPORT_LED_SWITCH_BUTTON
						else if(3 == type)
							sprintf(cmd, "echo %s > /dev/pts/%d\n", "WPS button is pressed!", i);
						else
							sprintf(cmd, "echo %s > /dev/pts/%d\n", "LED switch button is pressed!", i);
#else
						else	
							sprintf(cmd, "echo %s > /dev/pts/%d\n", "WPS button is pressed!", i);
#endif
						system(cmd);
					}
				}
				
			}
			if(type != 0){
			memset(cmd, 0, 100);
			sprintf(cmd, "echo %d > /proc/tc3162/button_type\n", 0);
			system(cmd);
			}
		}
	}else{
		fprintf(stderr,"no file /proc/tc3162/button_type\n");
		return;
	}
	return;
}
#endif
#endif

#if (VOIP == 1)
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
int voipIdleProcess(void)
{
	char ch1Status[32] = "Idle";
	char ch2Status[32] = "Idle";
#if defined(MT7612E)
	char bindwifiToVPE1[48] =  "echo 8 > /proc/irq/24/smp_affinity";
	char bindwifiToVPE1_5g[48] =  "echo 8 > /proc/irq/25/smp_affinity";
	char noBindwifiToVPE1[48] = "echo 7 > /proc/irq/24/smp_affinity";
	char noBindwifiToVPE1_5g[48] = "echo 7 > /proc/irq/25/smp_affinity";
	static int wifiBindStatus = 0;
#else
	char bindEthernetToVPE1[48] = "echo 8 > /proc/irq/22/smp_affinity";
	char noBindEthernetToVPE1[48] = "echo 7 > /proc/irq/22/smp_affinity";
	static int ethernetBindStatus = 0;
#endif

	if(VOIPLineNum == 1){
#if defined(TCSUPPORT_ECN_MEGACO)
		if(getSipStatus() == 1) 
			tcapi_get("InfoVoIPH248_Entry0","lineInfoStatus",ch1Status);
		else
#endif
			tcapi_get("VoIPSysParam_Entry0","SC_LINE_INFO_STATUS",ch1Status);
#if defined(MT7612E)
		if(wifiBindStatus == 0){
			if(!strcmp(ch1Status, "Idle")){
				system (bindwifiToVPE1);
				system (bindwifiToVPE1_5g);
				wifiBindStatus = 1;
				tcdbg_printf("fxs1 status is %s\n",ch1Status);
			}
		}
		else if(wifiBindStatus == 1){
			if(strcmp(ch1Status, "Idle")){
				system (noBindwifiToVPE1);
				system (noBindwifiToVPE1_5g);
				wifiBindStatus = 0;
				tcdbg_printf("fxs1 status is %s\n",ch1Status);
			}
		}
		else{
			tcdbg_printf("wifiBindStatus is wrong,value:%d\n",wifiBindStatus);
		}
#else
		if(ethernetBindStatus == 0){
			if(!strcmp(ch1Status, "Idle")){
				system (bindEthernetToVPE1);
				ethernetBindStatus = 1;
				tcdbg_printf("fxs1 status is %s\n",ch1Status);
			}
		}
		else if(ethernetBindStatus == 1){
			if(strcmp(ch1Status, "Idle")){
				system (noBindEthernetToVPE1);
				ethernetBindStatus = 0;
				tcdbg_printf("fxs1 status is %s\n",ch1Status);
			}
		}
		else{
			tcdbg_printf("ethernetBindStatus is wrong,value:%d\n",ethernetBindStatus);
		}
#endif
	}
	else if(VOIPLineNum == 2){
#if defined(TCSUPPORT_ECN_MEGACO)
		if(getSipStatus() == 1) {
			tcapi_get("InfoVoIPH248_Entry0","lineInfoStatus",ch1Status);
			tcapi_get("InfoVoIPH248_Entry1","lineInfoStatus",ch2Status);
		}
		else{
#endif
    		tcapi_get("VoIPSysParam_Entry0","SC_LINE_INFO_STATUS",ch1Status);
    		tcapi_get("VoIPSysParam_Entry1","SC_LINE_INFO_STATUS",ch2Status);
#if defined(TCSUPPORT_ECN_MEGACO)
		}
#endif
#if defined(MT7612E)
		if(wifiBindStatus == 0){
			if(!strcmp(ch1Status, "Idle") && !strcmp(ch2Status, "Idle")){
				system (bindwifiToVPE1);
				system (bindwifiToVPE1_5g);
				wifiBindStatus = 1;
				tcdbg_printf("fxs1 status is %s\n",ch1Status);
				tcdbg_printf("fxs2 status is %s\n",ch2Status);
			}
		}
		else if(wifiBindStatus == 1){
			if(strcmp(ch1Status, "Idle") || strcmp(ch2Status, "Idle")){
				system (noBindwifiToVPE1);
				system (noBindwifiToVPE1_5g);
				wifiBindStatus = 0;
				tcdbg_printf("fxs1 status is %s\n",ch1Status);
				tcdbg_printf("fxs2 status is %s\n",ch2Status);
			}
		}
		else{
			tcdbg_printf("wifiBindStatus is wrong,value:%d\n",wifiBindStatus);
		} 
#else 
		if(ethernetBindStatus == 0){
			if(!strcmp(ch1Status, "Idle") && !strcmp(ch2Status, "Idle")){
				system (bindEthernetToVPE1);
				ethernetBindStatus = 1;
				tcdbg_printf("fxs1 status is %s\n",ch1Status);
				tcdbg_printf("fxs2 status is %s\n",ch2Status);
			}
		}
		else if(ethernetBindStatus == 1){
			if(strcmp(ch1Status, "Idle") || strcmp(ch2Status, "Idle")){
				system (noBindEthernetToVPE1);
				ethernetBindStatus = 0;
				tcdbg_printf("fxs1 status is %s\n",ch1Status);
				tcdbg_printf("fxs2 status is %s\n",ch2Status);
			}
		}
		else{
			tcdbg_printf("ethernetBindStatus is wrong,value:%d\n",ethernetBindStatus);
		} 
#endif
	}
}
#endif

#if defined(TCSUPPORT_ECN_SIP)
int voipWanIPGet(char *ifName, char *wanIP){
	int            fd        = -1;
	struct ifreq   ifr;
	unsigned long  tmpIP     = 0;
	int            iRet      = 1;

	if (NULL == ifName || NULL == wanIP){
		tcdbg_printf("%s-%d, param is NULL\n", __func__, __LINE__);
		iRet = 0;
		return iRet;
	}
	
	strncpy(ifr.ifr_name, ifName, sizeof(ifr.ifr_name)-1);
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if( fd < 0 ){
		tcdbg_printf("Failed to open %s\n",ifName);
		iRet = 0;
	}else{
		if(ioctl(fd, SIOCGIFADDR, &ifr) >= 0){
			tmpIP = (((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr).s_addr;
			strcpy(wanIP,inet_ntoa(*(struct in_addr *) &tmpIP));
			iRet = 1;	
		}else{
			//tcdbg_printf ("Failed to get wanIP from ifName %s\n", ifName);
			iRet = 1;
	}
		close(fd);
	}

	return iRet;
}

/*______________________________________________________________________________
**function :restart_sip_when_sip_down
**
**description:
*    check sipclient process every second, when sipclient crash, restart it
**parameters:
*    none
**global:
*    sipStatusFlag
**return: 1-restart sipclient; 0-other
*    
*    
**call:
*    none
**revision:
*  
**____________________________________________________________________________*/

static int restart_sip_when_sip_down()
{
	FILE *fp = NULL;
	char sip_pid_tmp[100] = {0};
        char FlagFwUp_s[20] = {0};
	int  status = 0;
	int  ret = 0;
	char pidPath[] = "/tmp/sipclient.pid";		
	static int sipDownCheckCnt = 0;
	int  SIPDownCheckCycle = 5;

	/* each 5s run */
	if (SIPDownCheckCycle != sipDownCheckCnt){
		sipDownCheckCnt++;
		return ret;
	}

	sipDownCheckCnt = 0;
	fp = fopen(pidPath,"r");
	if(fp == NULL){
		/*get sipclient process pid*/
		status = system("pidof sipclient > /tmp/sipclient.pid");  
		if (-1 == status || 0 == WIFEXITED(status)){
		    /* system or run shell fail */
		    tcdbg_printf("\nrestart_sip_when_sip_down:xxxx system run fail, \
		        status:%d, WIFEXITED:%d, WEXITSTATUS:%d !!\n", status, 
		        WIFEXITED(status), WEXITSTATUS(status));
            
		    unlink(pidPath);
		    return ret;
		}
		fp = fopen(pidPath, "r");
	}
	if(fp == NULL){
		return ret;
	}
	fgets(sip_pid_tmp, sizeof(sip_pid_tmp), fp);
	fclose(fp);
	unlink(pidPath);
	tcapi_get("System_Entry", "upgrade_status", FlagFwUp_s);
	/*if sip_pid_tmp == 0 && not upgrade firmware, that's mean need restart sipclient*/
	if((atoi(sip_pid_tmp) == 0)&&(0 != strcmp(FlagFwUp_s,"true"))){  
		tcdbg_printf("\nrestart sipclient for sip crash!!\n");
		system("killall -9 sipclient");
		system("/userfs/bin/sipclient &");
		sipStatusFlag = 1;
		ret = 1;
	}
	return ret;
}

/*______________________________________________________________________________
**function :restart_sip_when_line_unreg
**
**description:
*    check line status every second, when wan up ,line enable and line unreg for longer time than rereg timer, restart sipclient
**parameters:
*    none
**global:
*    sipStatusFlag, line_error_restartsip_timer_cnt
**return:
*    
*    
**call:
*    none
**revision:
*  
**____________________________________________________________________________*/

static void restart_sip_when_line_unreg()
{
	char nodeName[128]						  = {0};
	int j = 0;
	char lineEnable[32] = {0};
	char strRegStatus[32] = {0};
	char regRetryTimestr[32] = {0};
	int regRetryTime = 0;
	char FlagFwUp_s[20] = {0};
	int line_no_unreg_cnt=0;/*count for unregistered line num*/
	int active_line_num=0;
	
	tcapi_get("System_Entry", "upgrade_status", FlagFwUp_s);
	tcapi_get("VoIPAdvanced_Common","RegisterRetryInterval",regRetryTimestr);
	regRetryTime = atoi(regRetryTimestr);
	regRetryTime = regRetryTime + 30;/*when retry register, it will take about 20s*/
	
	for(j=0 ; j<VOIPLineNum ; j++){
		memset(nodeName, 0, sizeof(nodeName));
		snprintf(nodeName, sizeof(nodeName) - 1, "VoIPBasic_Entry%d",j);
		if( TCAPI_PROCESS_OK == tcapi_get(nodeName, "Enable", lineEnable)/*make sure line is enable*/
			&& (strcmp(lineEnable,SIP_YES) == 0)){
			active_line_num++;
			memset(nodeName, 0, sizeof(nodeName));
			snprintf(nodeName, sizeof(nodeName) - 1, "VoIPSysParam_Entry%d",j);
			if( TCAPI_PROCESS_OK == tcapi_get(nodeName, "SC_ACCT_INFO_REG_STATUS", strRegStatus)/*check whether the  line status is unregistered*/
			&& (strcmp(strRegStatus,"Unregistered") == 0)){
				line_no_unreg_cnt++;/*line_no_unreg_cnt count unregister line num*/
				if(line_error_restartsip_timer_cnt[j] <= regRetryTime){
					line_error_restartsip_timer_cnt[j]++;
				}
				else{
					if(0 != strcmp(FlagFwUp_s,"true") && line_no_unreg_cnt==active_line_num){/*all line unregistered*/
					    tcdbg_printf("\nrestart sipclient for line error!!\n");
					    system("killall -9 sipclient");
					    system("/userfs/bin/sipclient &");
					    sipStatusFlag = 1;
					    memset(line_error_restartsip_timer_cnt,0,MAX_DSP_LINE*sizeof(int));
						break;
				        }
				}

			}						
		}
    }
	if(line_no_unreg_cnt != active_line_num){/*there are some line not unregistered status*/
		memset(line_error_restartsip_timer_cnt,0,MAX_DSP_LINE*sizeof(int));
	}
	return;
}

/*return 0:buf not all digit  1: buf is all digit*/
int isNumber(char *buf) {
	if(*buf == '\0') 
		return 0;
	else {
		while(*buf != '\0') {
			if(*buf<= '9' && *buf++ >= '0')
				continue;
			else
				return 0;
			}
	}
	return 1;
}

int voipStatUpgrade(void)
{
	int  valid_if[MAX_WAN_IF_INDEX]           = {0}; 
	int  valid_if_num                         = 0;
	int  i                                    = 0;
	int  j                                    = 0;
	int  pvc_index                            = 0;
	int  entry_index                          = 0;
	int  if_index                             = 0;
	char str_servicelist[32]                  = {0};
	char str_wanmode[8]                       = {0};
	char str_IP[128]                          = {0};
	char szCurrIP[128]                        = {0};
	char nodeName[128]                        = {0};
	char lanIP[32]                            = {0};
	char ifName[32]                           = {0};
	char scStatus[8]                          = {0};
	char wan_status_change_flag[8]            = {0};
	char voiceIfName[32]                      = {0};
	char lanEnable[8]                         = {0};
	char curIfIndex[8]                        = {0};
	static int noVoiceLinkFlag = 0;
	//mxml_node_t *top = get_node_top();
	if(tcapi_get("Lan_Entry0","IP",lanIP) != 0){
		 return 0;
	 }
	
	/*Lan Binding*/
	if(TCAPI_PROCESS_OK == tcapi_get("VoIPAdvanced_Common","LanBindEnable",lanEnable) && !strcmp(lanEnable,"1")) {
	   if (tcapi_get("VoIPSysParam_Common", "SC_SYS_CFG_WAN_IP", szCurrIP)==TCAPI_PROCESS_OK  && 0 != strcmp(lanIP, szCurrIP) &&
	   TCAPI_PROCESS_OK == tcapi_get("InfoVoIP_Common","SC_START_STATUS",scStatus) && 0 == strcmp(scStatus, "Yes")) {
		
		//voipWanIPGet(voiceIfName, str_IP);
		noVoiceLinkFlag = 0;
		tcdbg_printf(" lan side binding\n");
		tcapi_set("VoIPSysParam_Common","SC_SYS_CFG_WAN_IP", lanIP);
		tcapi_set("VoIPSysParam_Common","SC_SYS_CFG_WAN_IFNAME","br0");

		/*[OSBNB00043026]ADD by Eric@20150212, modify the SIP registration mechanism*/
		for(j=0 ; j<VOIPLineNum ; j++){ /*make sure every line has its independent flag*/
			memset(nodeName, 0, sizeof(nodeName));
			snprintf(nodeName, sizeof(nodeName) - 1, "InfoVoIP_Entry%d", j);

			if(strlen(lanIP)!=0){
			    /*always directly send message in Lan side*/
			    tcapi_set(nodeName,"WAN_STATUS_CHANGE_FLAG","up");
			}else{
			    tcapi_set(nodeName,"WAN_STATUS_CHANGE_FLAG","");
			}
		}
		/*[OSBNB00043026]ADD END*/
		tcapi_commit("VoIPSysParam");

	   }
	   return 0;
	}
	
	/*Wan Binding*/
	tcapi_get("VoIPAdvanced_Common","CurIFIndex",curIfIndex);
  
	valid_if_num = get_all_wan_index(valid_if, MAX_WAN_IF_INDEX);
	for(i = (valid_if_num - 1); i > -1; i--)
	{
		if(isNumber(curIfIndex)){
		/*Conditions:1, the node is not NULL ; 2, the node is all digit number value;*/
			if_index = atoi(curIfIndex);	
			pvc_index = if_index / MAX_SMUX_NUM;
			entry_index = if_index % MAX_SMUX_NUM;
		}
		else {
		/*Conditions:1, the webpage do not choose any VoIP WAN and the WAN is created by TR069; 2, the node has illegal value;*/
			pvc_index = valid_if[i] / MAX_SMUX_NUM;
			entry_index = valid_if[i] % MAX_SMUX_NUM;
		}
		memset(nodeName, 0, sizeof(nodeName));
		snprintf(nodeName, sizeof(nodeName) - 1, "Wan_PVC%d_Entry%d", pvc_index, entry_index);
			
		/*Conditions:1, the device has VoIP internet connect ; 2, the connect is route mode other than bridge;
		3, the network interface is not null;4, the sipclient has started*/
		if( TCAPI_PROCESS_OK == tcapi_get(nodeName, "Active", str_wanmode) &&
		    0 == strcmp(str_wanmode, "Yes") &&
		    TCAPI_PROCESS_OK == tcapi_get(nodeName, "WanMode", str_wanmode) &&
		    0 == strcmp(str_wanmode, "Route") &&
		    TCAPI_PROCESS_OK == tcapi_get(nodeName, "ServiceList", str_servicelist) &&
		    NULL != strstr(str_servicelist, "VOICE") &&
		    TCAPI_PROCESS_OK == tcapi_get(nodeName, "IFName", ifName) &&
		    TCAPI_PROCESS_OK == tcapi_get("InfoVoIP_Common","SC_START_STATUS",scStatus) &&
		    0 == strcmp(scStatus, "Yes")){
	
			if (restart_sip_when_sip_down()){
				return 0;   /* restart sipclient, so return directly */
			}
#if !defined(TCSUPPORT_CT_PON_SC)
			restart_sip_when_line_unreg();
#endif
				
			if ( voipWanIPGet(ifName, str_IP) && TCAPI_PROCESS_OK == tcapi_get("VoIPSysParam_Common","SC_SYS_CFG_WAN_IP",szCurrIP) &&
			   0 != strcmp(str_IP, szCurrIP)){  
		   
				for(j=0 ; j<VOIPLineNum ; j++){/*make sure every line has its independent flag*/ 
					memset(nodeName, 0, sizeof(nodeName));
					snprintf(nodeName, sizeof(nodeName) - 1, "InfoVoIP_Entry%d", j);	
				 
					/*register through lan to wan */	
					if(strlen(str_IP)!=0 && strcmp(str_IP,lanIP) && 
					   (strlen(szCurrIP)==0 || !(strcmp(szCurrIP,lanIP)) || !(strcmp(szCurrIP,"192.168.1.1")))){
						tcapi_set(nodeName,"WAN_STATUS_CHANGE_FLAG","down_up");
					/*initial power up*/	
					}else if(strlen(str_IP)!=0 && !strcmp(szCurrIP,"0.0.0.0")){
						tcapi_set(nodeName,"WAN_STATUS_CHANGE_FLAG","down_up");
					}else if(strlen(str_IP)!=0 && strlen(szCurrIP)!=0 && strcmp(szCurrIP,"0.0.0.0")){
						tcapi_set(nodeName,"WAN_STATUS_CHANGE_FLAG","up");
					}else if(strlen(str_IP)==0 && strcmp(szCurrIP,"0.0.0.0")){
						tcapi_set(nodeName,"WAN_STATUS_CHANGE_FLAG","up_down");
					}else {
						tcapi_set(nodeName,"WAN_STATUS_CHANGE_FLAG","");
					}
			     }
					tcdbg_printf("===str_IP:%s=szCurrIP:%s====\n", str_IP, szCurrIP);
					noVoiceLinkFlag = 0;
					if(strlen(str_IP)==0){
				            tcapi_set(nodeName,"Status","Error");
				            tcapi_set(nodeName,"RegFailReason","6");//error 6: voice wan has no ip	 
			                }
					tcapi_set("VoIPSysParam_Common","SC_SYS_CFG_WAN_IP",str_IP);
					tcapi_set("VoIPSysParam_Common","SC_SYS_CFG_WAN_IFNAME",ifName);
					tcapi_commit("VoIPSysParam");			
			}
			break;
		}		   
	}
	
	/*no VoIP internet*/
	if(i==-1){
	/*means there is no VoIP internet*/
	    if(noVoiceLinkFlag == 0){
			noVoiceLinkFlag = 1;
			tcdbg_printf(" no voice link\n");
			tcapi_set("VoIPSysParam_Common","SC_SYS_CFG_WAN_IP","0.0.0.0");
			tcapi_set("VoIPSysParam_Common","SC_SYS_CFG_WAN_IFNAME", " ");
			for(j=0 ; j<VOIPLineNum ; j++){ /*make sure every line has its independent flag*/
				memset(nodeName, 0, sizeof(nodeName));
				snprintf(nodeName, sizeof(nodeName) - 1, "InfoVoIP_Entry%d", j);

				/*directly send deregister message*/
				tcapi_set(nodeName,"WAN_STATUS_CHANGE_FLAG","up_down");
				tcapi_set(nodeName,"Status","Error");
				tcapi_set(nodeName,"RegFailReason","6");//error 6: no voice wan		
                
			}
			tcapi_commit("VoIPSysParam");
		}
	}

	return 0;
}
#endif


#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
extern voip_simulate_state_t gVoipSimulateState[VOIP_LINE_NUM];
char simulate_timer_flag[40] = {0};
int callConfirmDititNum = -1;
static int voip_simulate_timeout_check(int line_id)
{
	tcdbg_printf ("[Simulate Test] %s [%d] state [%d] line_state [%d] timeCnt [%d]\n", 
				__func__, line_id, 
				gVoipSimulateState[line_id].state,
				gVoipSimulateState[line_id].line_state,
				gVoipSimulateState[line_id].timeCnt);
	/*tcdbg_printf("[Simulate Test] %s   simulate_timer_flag:[%s]\n",__func__,simulate_timer_flag);*/
	if (SIMULATE_WAIT_CONN_STATE == gVoipSimulateState[line_id].state){
		if (gVoipSimulateState[line_id].timeCnt > gVoipSimulateState[line_id].calledWaitTime){
			gVoipSimulateState[line_id].state = SIMULATE_TIMEOUT_STATE;				
		}
		else{
			gVoipSimulateState[line_id].timeCnt++;
		}
		return SUCCESS;
	}
	
	if (SIMULATE_RECVING_DTMF_STATE == gVoipSimulateState[line_id].state)
	{
		if((!strcmp(simulate_timer_flag,"call_comfirm_digit_timer"))
			&&(gVoipSimulateState[line_id].timeCnt >= SIMULATE_CALL_HOLD_DIGIT_TIME))	
		{	
		gVoipSimulateState[line_id].state = SIMULATE_TIMEOUT_STATE;		
	}
		else if((!strcmp(simulate_timer_flag,"call_hold_timer"))
			&&(gVoipSimulateState[line_id].timeCnt > gVoipSimulateState[line_id].callHoldTime))
		{
		gVoipSimulateState[line_id].state = SIMULATE_TIMEOUT_STATE;	
	}
		else
		{
		gVoipSimulateState[line_id].timeCnt++;
	}

	return SUCCESS;
	}
	return SUCCESS;
}

static int voip_simulate_set_status(int line_id, char *tmpStr)
{
	if (SIMULATE_WAIT_CONN_STATE == gVoipSimulateState[line_id].state){
		if (!strcmp(tmpStr, SIMULATE_LINE_RINGBACK)){
			simulate_status_set(line_id, SIMULATE_STATUS_RINGING_BACK);
		}
		else if (!strcmp(tmpStr, SIMULATE_LINE_CONNECT)){
			simulate_status_set(line_id, SIMULATE_STATUS_CONNECTED);
		}

	}

	return SUCCESS;
}

static int voip_simulate_check_wait_conn(int line_id, char *status)
{
	if((SIMULATE_CALLER == gVoipSimulateState[line_id].testType)
			&& (!strcmp(status, SIMULATE_LINE_RINGBACK)
			|| !strcmp(status, SIMULATE_LINE_DIALING))){
		return SUCCESS;
	}
	else if ((SIMULATE_CALLED == gVoipSimulateState[line_id].testType)
			&&(!strcmp(status, SIMULATE_LINE_RINGING) 
			|| !strcmp(status, SIMULATE_LINE_IDLE))){
		return SUCCESS;
	}

	return FAIL;	
}
static int voip_simulate_wait_conn(int line_id)
{
	char *p = NULL;
	char tmpStr[MAX_VALUE_SIZE] = {0};
	char nodeName[128] = {0};
	int retval = 0;

	tcdbg_printf ("[Simulate Test] %s [%d]\n", __func__, line_id);
#if defined(TCSUPPORT_ECN_SIP)
#if defined(TCSUPPORT_ECN_MEGACO)
	if(getSipStatus() == 0) {
#endif
		snprintf(nodeName, sizeof(nodeName)-1, "VoIPSysParam_Entry%d", line_id);
		retval = tcapi_get(nodeName, "SC_LINE_INFO_STATUS", tmpStr);
#if defined(TCSUPPORT_ECN_MEGACO)
	}
#endif
#endif

#if defined(TCSUPPORT_ECN_MEGACO)
#if defined(TCSUPPORT_ECN_SIP)
	if(getSipStatus() == 1) {
#endif
		snprintf(nodeName, sizeof(nodeName)-1, "%s_%s%d",VOIPINFO_H248,VOIPENTRY, line_id);
		retval = tcapi_get(nodeName, H248_LINE_INFO_STATUS, tmpStr);
#if defined(TCSUPPORT_ECN_SIP)
	}
#endif
#endif

	if (TCAPI_PROCESS_OK == retval)
	{
		voip_simulate_set_status(line_id, tmpStr);
		if (!strcmp(tmpStr, SIMULATE_LINE_CONNECT))	{
			gVoipSimulateState[line_id].timeCnt = 0;
			gVoipSimulateState[line_id].state = SIMULATE_DTMF_TONE_STATE;
		}
		else if (SUCCESS == voip_simulate_check_wait_conn(line_id, tmpStr)){
			voip_simulate_timeout_check(line_id);
		}
		else
		{
			gVoipSimulateState[line_id].state = SIMULATE_DISCONNECT_STATE;
		}
	}
	else{
		voip_simulate_timeout_check(line_id);
	}
		
	return SUCCESS;
}

static int voip_simulate_send_dtmf(int line_id)
{
	int i = 0, retval = 0;
	char testCmd[MAX_CMD_SIZE] = {0};
	char *p = NULL;
	char tmpStr[MAX_VALUE_SIZE] = {0};
	char nodeName[MAX_NODE_NAME] = {0};

	tcdbg_printf ("[Simulate Test] %s [%d]\n", __func__, line_id);
#if defined(TCSUPPORT_ECN_MEGACO)
	char termName[MAX_VALUE_SIZE] = {0};	
#if defined(TCSUPPORT_ECN_SIP)
	if(getSipStatus() == 1) {
#endif
		sprintf(nodeName,"%s_Entry%d",VOIPH248,line_id);
		if(TCAPI_PROCESS_OK != tcapi_get(nodeName, PHYSICAL_TERMID, termName))
			return FAIL;
		memset(nodeName, 0, sizeof(nodeName));
#if defined(TCSUPPORT_ECN_SIP)
	}
#endif
#endif

#if defined(TCSUPPORT_ECN_SIP)
#if defined(TCSUPPORT_ECN_MEGACO)
	if(getSipStatus() == 0) {
#endif
		snprintf(nodeName, sizeof(nodeName)-1, "VoIPSysParam_Entry%d", line_id);
		retval = tcapi_get(nodeName, "SC_LINE_INFO_STATUS", tmpStr);
#if defined(TCSUPPORT_ECN_MEGACO)
	}
#endif
#endif

#if defined(TCSUPPORT_ECN_MEGACO)
#if defined(TCSUPPORT_ECN_SIP)
	if(getSipStatus() == 1) {
#endif
		snprintf(nodeName, sizeof(nodeName)-1, "%s_%s%d",VOIPINFO_H248,VOIPENTRY, line_id);
		retval = tcapi_get(nodeName, H248_LINE_INFO_STATUS, tmpStr);
#if defined(TCSUPPORT_ECN_SIP)
	}
#endif
#endif

	if ((TCAPI_PROCESS_OK == retval) && !strcmp(tmpStr, "Connect"))
	{		
		i = gVoipSimulateState[line_id].timeCnt;
		if (i == strlen(gVoipSimulateState[line_id].confirmNum)){
			gVoipSimulateState[line_id].timeCnt = 0;
			gVoipSimulateState[line_id].state = SIMULATE_RECVING_DTMF_STATE;
			simulate_status_set(line_id, SIMULATE_STATUS_RECEIVING);

			/* [OSBNB00039480] Fix the issue that EVENT_TYPE_DTMF_DL is received on ch 0
			** when execute Send2Api on ch 0, clear simulateData before receiving dtmf */
			sprintf(nodeName,"%s_Entry%d",VOIPINFO_H248,line_id);
			tcapi_set(nodeName, H248_LINE_SIMULATE_DATA, "");
			
			return SUCCESS;
		}
			
		p = &gVoipSimulateState[line_id].confirmNum[0];
		
#if defined(TCSUPPORT_ECN_SIP)
#if defined(TCSUPPORT_ECN_MEGACO)
		if(getSipStatus() == 0) {
#endif
			snprintf(testCmd, sizeof(testCmd)-1, SIMULATE_PUSH_BUTTON, line_id, *(p+i));
			system(testCmd);
#if defined(TCSUPPORT_ECN_MEGACO)
		}
#endif
#endif
		
#if defined(TCSUPPORT_ECN_MEGACO)
#if defined(TCSUPPORT_ECN_SIP)
		if(getSipStatus() == 1) {
#endif
			snprintf(testCmd, sizeof(testCmd)-1, SIMULATE_PUSH_DTMF_PREFIX, termName, *(p+i));
			voipCmdSend(testCmd);
#if defined(TCSUPPORT_ECN_SIP)
		}
#endif
#endif

		gVoipSimulateState[line_id].timeCnt++;
	}
	else{
		gVoipSimulateState[line_id].state = SIMULATE_DISCONNECT_STATE;
	}	
		
	return SUCCESS;
}
static int voip_simulate_reving_dtmf(int line_id)
{
	char tmpStr[MAX_VALUE_SIZE] = {0};
	char nodeName[MAX_NODE_NAME] = {0};
	int retval = 0;
	int callConfirmDititNumTemp = 0;

	tcdbg_printf ("[Simulate Test] %s [%d]\n", __func__, line_id);
#if defined(TCSUPPORT_ECN_SIP)
#if defined(TCSUPPORT_ECN_MEGACO)
	if(getSipStatus() == 0) {
#endif
		snprintf(nodeName, sizeof(nodeName)-1, "VoIPSysParam_Entry%d", line_id);
		retval = tcapi_get(nodeName, "SC_LINE_INFO_STATUS", tmpStr);
#if defined(TCSUPPORT_ECN_MEGACO)
	}
#endif
#endif
	
#if defined(TCSUPPORT_ECN_MEGACO)
#if defined(TCSUPPORT_ECN_SIP)
	if(getSipStatus() == 1) {
#endif
		snprintf(nodeName, sizeof(nodeName)-1, "%s_%s%d",VOIPINFO_H248,VOIPENTRY, line_id);
		retval = tcapi_get(nodeName, H248_LINE_INFO_STATUS, tmpStr);
#if defined(TCSUPPORT_ECN_SIP)
	}
#endif
#endif
	snprintf(nodeName, sizeof(nodeName)-1, "VoIPSysParam_Entry%d", line_id);
	tcapi_get(nodeName, "SC_LINE_SIMULATE_DATA", gVoipSimulateState[line_id].callConfirmDigit);

	if ((TCAPI_PROCESS_OK == retval) && !strcmp(tmpStr, "Connect"))
	{
		if(strlen(gVoipSimulateState[line_id].callConfirmDigit)>0)//call confirm digit not empty
		{
			callConfirmDititNumTemp = callConfirmDititNum + 1;
			if(callConfirmDititNumTemp <= strlen(gVoipSimulateState[line_id].callConfirmDigit))//receive one more digit num
			{
				if(strlen(gVoipSimulateState[line_id].callConfirmDigit)==
					strlen(gVoipSimulateState[line_id].confirmNum))//receive the same number of digits as set
				{
					gVoipSimulateState[line_id].state = SIMULATE_DISCONNECT_STATE;
					callConfirmDititNum = -1;
					return SUCCESS;
				}
				else
				{
					gVoipSimulateState[line_id].timeCnt = 0;//start 20s timer, call confirm every digit timer
					memset(simulate_timer_flag,0,sizeof(simulate_timer_flag));
					strcpy(simulate_timer_flag,"call_comfirm_digit_timer");
					callConfirmDititNum = strlen(gVoipSimulateState[line_id].callConfirmDigit);
		return voip_simulate_timeout_check(line_id);		
	}	
			}
			else{
				return voip_simulate_timeout_check(line_id);
			}
		}
		else
		{
			if((callConfirmDititNum == -1)
				&&(strlen(gVoipSimulateState[line_id].callConfirmDigit) == 0))
			{
				gVoipSimulateState[line_id].timeCnt = 0;//start 60s timer, call hold timer
				memset(simulate_timer_flag,0,sizeof(simulate_timer_flag));
				strcpy(simulate_timer_flag,"call_hold_timer");
				callConfirmDititNum = 0;
		return voip_simulate_timeout_check(line_id);		
	}	
			else if((callConfirmDititNum == 0)
				&&(strlen(gVoipSimulateState[line_id].callConfirmDigit) == 0))
			{
				return voip_simulate_timeout_check(line_id);//continue 60s timer, call hold timer
			}

		}
	}

	gVoipSimulateState[line_id].state = SIMULATE_DISCONNECT_STATE;
	return SUCCESS;
}

static int voip_simulate_onhook(int line_id)
{
	char testCmd[MAX_CMD_SIZE] = {0};
#if defined(TCSUPPORT_ECN_MEGACO)
	char termName[MAX_VALUE_SIZE] = {0};
	char nodeName[MAX_NODE_NAME] = {0};
	int i=0;
#endif
	tcdbg_printf ("[Simulate Test] %s [%d]\n", __func__, line_id);
#if defined(TCSUPPORT_ECN_SIP)
#if defined(TCSUPPORT_ECN_MEGACO)
	if(getSipStatus() == 0) {
#endif
		snprintf(testCmd, sizeof(testCmd)-1, SIMULATE_ONHOOK, line_id);
		system(testCmd);
#if defined(TCSUPPORT_ECN_MEGACO)
	}
#endif
#endif

#if defined(TCSUPPORT_ECN_MEGACO)
#if defined(TCSUPPORT_ECN_SIP)
	if(getSipStatus() == 1) {
#endif
		sprintf(nodeName,"%s_Entry%d",VOIPH248,line_id);
		tcapi_get(nodeName, PHYSICAL_TERMID, termName);
		
		snprintf(testCmd, sizeof(testCmd)-1, SIMULATE_CMD_PREFIX,termName, ONHOOK_SG);
		voipCmdSend(testCmd);
		for(i=0;i<2;i++){
			sprintf(nodeName,"%s_Entry%d",VOIPINFO_H248,i);
			termName[0]='\0';
			tcapi_set(nodeName, H248_LINE_SIMULATE_DATA, termName);
		}
#if defined(TCSUPPORT_ECN_SIP)
	}
#endif
#endif

	memset(testCmd,0,sizeof(testCmd));
	sprintf(testCmd,"echo \"2 %d 0\"> /proc/fxs/dtmf",line_id);
	system(testCmd);

	return SUCCESS;
}

static int voip_simulate_result_deal(int line_id)
{
	char nodeName[MAX_NODE_NAME] = {0};
	char tmpStr[MAX_VALUE_SIZE] = {0};
	int retval=0;
	tcdbg_printf ("[Simulate Test] %s [%d] testType [%d] line_state [%d]\n", 
		__func__, line_id, gVoipSimulateState[line_id].testType, gVoipSimulateState[line_id].line_state);

	memset(simulate_timer_flag,0,sizeof(simulate_timer_flag));	
	callConfirmDititNum = -1;
	
	/* [OSBNB00039480] ADD by peter.jiang@20140827 to set failure reason after simulation test */
	if(SIMULATE_CALLER == gVoipSimulateState[line_id].testType) {
		switch (gVoipSimulateState[line_id].line_state) {
			case SIMULATE_STATUS_IDLE:
				break;
			case SIMULATE_STATUS_OFFHOOK:
			case SIMULATE_STATUS_CONNECTED:
				simulate_result_set(line_id, SIMULATE_CALLER_OFFHOOK_RELEASE, "0");
				goto SIMULATE_TEST_END;

				break;
			case SIMULATE_STATUS_DIAL_TONE:
				simulate_result_set(line_id, SIMULATE_CALLER_AFTER_DIAL_REALEASE, "0");
				goto SIMULATE_TEST_END;

				break;
			case SIMULATE_STATUS_RINGING_BACK:
				simulate_result_set(line_id, SIMULATE_CALLER_NO_ANSWER, "0");
				goto SIMULATE_TEST_END;

				break;
			case SIMULATE_STATUS_RECEIVING:
			case SIMULATE_STATUS_RECEIVE_END:
				/* check received dtmf */
				tcdbg_printf ("[Simulate Test] %s [%d] Caller Matching received dtmf...\n",
							__func__, line_id);

				break;
			default:
				/* SHOULD NEVER be here */
				break;

		}
	}
	else if (SIMULATE_CALLED == gVoipSimulateState[line_id].testType) {
		switch (gVoipSimulateState[line_id].line_state) {
			case SIMULATE_STATUS_IDLE:
				/* H248 Called simulate test */
				snprintf(nodeName, sizeof(nodeName)-1, "InfoVoIPH248_Entry%d", line_id);
				tcapi_get(nodeName, "lineInfoStatus", tmpStr);
				tcdbg_printf ("[Simulate Test] %s [%d] current line state: %s\n", __func__, line_id, tmpStr);
				if (!strcmp (tmpStr, "Idle"))
					simulate_result_set(line_id, SIMULATE_CALLED_NO_INCOMING_CALL, "0");
				else if (!strcmp (tmpStr, "Ringing"))
					simulate_result_set(line_id, SIMULATE_CALLED_NO_ANSWER, "0");
				else 
					simulate_result_set(line_id, SIMULATE_CALLED_NO_INCOMING_CALL, "0");
				goto SIMULATE_TEST_END;
				break;
				
			case SIMULATE_STATUS_DIAL_TONE:
				simulate_result_set(line_id, SIMULATE_CALLED_NO_INCOMING_CALL, "0");
				goto SIMULATE_TEST_END;
				break;
				
			case SIMULATE_STATUS_OFFHOOK:
			case SIMULATE_STATUS_CONNECTED:
				simulate_result_set(line_id, SIMULATE_CALLED_OFFHOOK_RELEASE, "0");
				goto SIMULATE_TEST_END;
				break;

			case SIMULATE_STATUS_RECEIVING:
			case SIMULATE_STATUS_RECEIVE_END:
				/* check received dtmf */
				tcdbg_printf ("[Simulate Test] %s [%d] Called Matching received dtmf...\n",
							__func__, line_id);
				break;

			default:
				/* SHOULD NEVER be here */
				break;

		}
	}	
	/* ADD end */
#if defined(TCSUPPORT_ECN_SIP)
#if defined(TCSUPPORT_ECN_MEGACO)
	if(getSipStatus() == 0) {
#endif
		snprintf(nodeName, sizeof(nodeName)-1, "VoIPSysParam_Entry%d", line_id);
		retval = tcapi_get(nodeName, "SC_LINE_SIMULATE_DATA", tmpStr);
#if defined(TCSUPPORT_ECN_MEGACO)
	}
#endif
#endif

#if defined(TCSUPPORT_ECN_MEGACO)
#if defined(TCSUPPORT_ECN_SIP)
	if(getSipStatus() == 1) {
#endif
		snprintf(nodeName, sizeof(nodeName)-1, "%s_%s%d",VOIPINFO_H248,VOIPENTRY, line_id);
		retval = tcapi_get(nodeName, H248_LINE_SIMULATE_DATA, tmpStr);
#if defined(TCSUPPORT_ECN_SIP)
	}
#endif
#endif

	if (TCAPI_PROCESS_OK == retval)
	{
		tcdbg_printf ("[Simulate Test] %s [%d] received [%s] \n", 
			__func__, line_id, tmpStr);

		if ((!strcmp(gVoipSimulateState[line_id].confirmEnable, "Yes")
			&& !strcmp(tmpStr, gVoipSimulateState[line_id].confirmNum))
			|| (0 != strcmp(gVoipSimulateState[line_id].confirmEnable, "Yes")
			&& 0 != strlen(gVoipSimulateState[line_id].confirmNum))){
			simulate_result_set(line_id, SIMULATE_NO_ERROR, "1");
		}
/* [OSBNB00052025] ADD by Eric.guo@20151109 to modify the simulation test procedure for ZTE GD,once receiving a DTMF,the conclusion will be sucess*/
#if defined(TCSUPPORT_CT_PON_GDV20)
		else if ((!strcmp(gVoipSimulateState[line_id].confirmEnable, "Yes")
			&& 0 != strlen(tmpStr))){
			simulate_result_set(line_id, SIMULATE_NO_ERROR, "0");
		}
		else{
			simulate_result_set(line_id, SIMULATE_CALLER_NO_ANSWER, "0");
		}
#else
		else{
			simulate_result_set(line_id, SIMULATE_CALLER_KEY_ERROR, "0");
		}
#endif
	}
	else{
		simulate_result_set(line_id, SIMULATE_CALLER_KEY_ERROR, "0");
	}

SIMULATE_TEST_END:
	simulate_status_set(line_id, SIMULATE_STATUS_TEST_END);
	simulate_testState_set(line_id, "Complete");
	voip_simulate_onhook(line_id);
	
	memset(&gVoipSimulateState[line_id], 0, sizeof(voip_simulate_state_t));

	return SUCCESS;

}

static int voip_simulate_block_deal(int line_id)
{
	tcdbg_printf ("[Simulate Test] %s [%d]\n", __func__, line_id);

	if (SIMULATE_CALLED == gVoipSimulateState[line_id].testType){
		simulate_result_set(line_id, SIMULATE_CALLED_NO_ANSWER, "0");
	}
	else{
		simulate_result_set(line_id, SIMULATE_CALLER_NO_DIAL_TONE, "0");
	}
	simulate_status_set(line_id, SIMULATE_STATUS_TEST_END);
	simulate_testState_set(line_id, "Complete");
	voip_simulate_onhook(line_id);	
	
	memset(&gVoipSimulateState[line_id], 0, sizeof(voip_simulate_state_t));

	return SUCCESS;
}

static int voip_simulate_sip_no_ok_deal(int line_id)
{
	tcdbg_printf ("[Simulate Test] %s [%d]\n", __func__, line_id);

	if (SIMULATE_CALLED == gVoipSimulateState[line_id].testType){
		simulate_result_set(line_id, SIMULATE_CALLED_NO_INCOMING_CALL, "0");
	}else{
		simulate_result_set(line_id, SIMULATE_CALLER_NO_DIAL_TONE, "0");
	}
	simulate_status_set(line_id, SIMULATE_STATUS_TEST_END);
	simulate_testState_set(line_id, "Complete");
	
	memset(&gVoipSimulateState[line_id], 0, sizeof(voip_simulate_state_t));

	return SUCCESS;
}

int voipSimulateTest()
{
	int  i = 0;

	for (i=0; i < VOIP_LINE_NUM; i++){
		switch (gVoipSimulateState[i].state){
			case SIMULATE_INIT_STATE:
				break;
			case SIMULATE_WAIT_CONN_STATE:
				voip_simulate_wait_conn(i);
				break;
			case SIMULATE_DTMF_TONE_STATE:
				voip_simulate_send_dtmf(i);
				break;
			case SIMULATE_RECVING_DTMF_STATE:
				voip_simulate_reving_dtmf(i);
				break;
			case SIMULATE_TIMEOUT_STATE:
			case SIMULATE_DISCONNECT_STATE:
				voip_simulate_result_deal(i);
				break;
			case SIMULATE_BLOCK_STATE:
				voip_simulate_block_deal(i);
				break;
			case SIMULATE_SIP_NO_OK_STATE:
				voip_simulate_sip_no_ok_deal(i);
				break;
			default:
				return FAIL;
		}

	}	

	return SUCCESS;
}
#if defined(TCSUPPORT_ECN_MEGACO)
void sendSimulateDtmf()
{
	char tmpStr[MAX_VALUE_SIZE]={0};
	char tmpStr1[MAX_VALUE_SIZE]={0};
	char testCmd[MAX_CMD_SIZE]={0};
	char termName[MAX_VALUE_SIZE]={0};
	char nodeName[MAX_VALUE_SIZE]={0};
	char nodeName1[MAX_VALUE_SIZE]={0};
	int  i = 0;
	char *p=NULL;
	char lineStats[MAX_VALUE_SIZE]={0};

	for (i=0; i < VOIP_LINE_NUM; i++){
		switch(gVoipSimulateState[i].testType){
			case SIMULATE_CALLER:
				strncpy(lineStats,SIMULATE_LINE_DIALING,MAX_VALUE_SIZE-1);
				break;
			case SIMULATE_CALLED:
				strncpy(lineStats,SIMULATE_LINE_CONNECT,MAX_VALUE_SIZE-1);
				break;
			default:
				break;
		}
		memset(nodeName, 0, sizeof(nodeName));
		sprintf(nodeName,"%s_Entry%d",VOIPINFO_H248,i);
		if( (tcapi_get(nodeName, H248_LINE_INFO_STATUS, tmpStr) == TCAPI_PROCESS_OK)
			&& (strcmp(tmpStr,lineStats)==0)
			&&(tcapi_get(nodeName,SIMULATE_START,tmpStr1)==TCAPI_PROCESS_OK) 
			&& (strcmp(tmpStr1,SIMULATE_CFG_DONE)==0) ){
			tcdbg_printf ("[Simulate Test]%s [%d] Dialing or Sending DTMF\n", __func__, i);
		
			if(gVoipSimulateState[i].testType==SIMULATE_CALLER){
				memset(nodeName, 0, sizeof(nodeName));
				sprintf(nodeName,"%s_Entry%d",VOIPSIMULATETEST,i);
				sprintf(nodeName1,"%s_Entry%d",VOIPH248,i);
			
				if((tcapi_get(nodeName, SIMULATE_CALLED_NUM, tmpStr) != TCAPI_PROCESS_OK) ||
					(tcapi_get(nodeName1, PHYSICAL_TERMID, termName) != TCAPI_PROCESS_OK)) {
					tcdbg_printf ("[Simulate Test] %s [%d] get physical TermID failed.\n", __func__, i);
					return;
				}
				p=tmpStr;
				while (p != NULL && *p != '\0'){
					snprintf(testCmd, sizeof(testCmd)-1, SIMULATE_DIAL_PREFIX, termName,*p);
					voipCmdSend(testCmd);
					p++;
				}
			}
			gVoipSimulateState[i].state = SIMULATE_WAIT_CONN_STATE;
			simulate_status_set(i, SIMULATE_STATUS_DIAL_TONE);
			memset(nodeName, 0, sizeof(nodeName));
			sprintf(nodeName,"%s_Entry%d",VOIPINFO_H248,i);
			tcapi_set(nodeName,SIMULATE_START,SIMULATE_SEND_DTMF);
		}

		/* [OSBNB00039480] Add timeout checking if No incoming call or No answer in case of Called simulation test */
		if (SIMULATE_CALLED && gVoipSimulateState[i].testType
			&& (tcapi_get(nodeName,SIMULATE_START,tmpStr1)==TCAPI_PROCESS_OK)
			&& (strcmp(tmpStr1,SIMULATE_CFG_DONE)==0)){
			voip_simulate_timeout_check (i);
		}
		
	}

}

void mgappKeepAlive(void)
{
	char nodeName[64]={0};
	char reset[8]={0};
	char FlagFwUp_s[20] = {0};
	tcapi_get("System_Entry", "upgrade_status", FlagFwUp_s);
	sprintf(nodeName,"%s_Common",VOIPINFO_H248);
	if(tcapi_get(nodeName,RESETFLAG,reset)!=TCAPI_PROCESS_OK)
		return;
	if((strcmp(reset,"1")==0)&&(0 != strcmp(FlagFwUp_s,"true"))){
		system("killall -9 mgapp");
		system("/userfs/bin/mgapp -id 0 &");
		tcapi_set(nodeName,RESETFLAG,"0");
	}

	return;

}
#endif
#endif
#endif

#if defined(TCSUPPORT_CT_ADSL_BIND1)
void check_bridge_ppp_connection(void)
{
	FILE *fp = NULL;
	char line_buf[128] = {0}, status[32] = {0}, ppp_username[256] = {0};
	char br_name[32] = {0}, bind_username[256] = {0};
	char typeAutoBind[12] = {0}, autoBindRandTime[16] = {0};
	char reg_status[12] = {0}, reg_result[12] = {0};
	char reg_newstatus[12] = {0}, reg_newresult[12] = {0};
	char acs_result[12] = {0};
	char max_randtime[16] = {0};
	char webCurSetNode[32] = "WebCurSet_Entry";
	char deviceAccountNode[32] = "deviceAccount_Entry";
	char cwmpNode[32] = "Cwmp_Entry";
	cwmp_msg_t message;
	int i_reg_status = 0, i_reg_result = 0, i_reg_newstatus = 0;
	int i_reg_newresult = 0;
	static int i_maxrand_time = 0, i_detect_st = 0;
	static int time_count_bind1_reg = 0, retry_bind1_reg = 0;
#define BIND1_REGISTER 19
#define ATTR_XBIND_TYPE "XBINDType"
#define ATTR_XBIND_RESULT "XBINDResult"
#define ATTR_XBIND_USERNAME "AutoBINDUserName"
#define FILE_PPP_ST "/proc/tc3162/pppusername"


	/* check whether ITMS+ connected. */
	tcapi_get(cwmpNode, "Inform_Status", acs_result);
	if ( 8 != atoi(acs_result) )
		return; /* do nothing if ITMS+ not connnected. */

	tcapi_get(deviceAccountNode, "registerStatus", reg_status);
	tcapi_get(deviceAccountNode, "registerResult", reg_result);
	tcapi_get(deviceAccountNode, "newStatus", reg_newstatus);
	tcapi_get(deviceAccountNode, "newResult", reg_newresult);
	tcapi_get(deviceAccountNode, ATTR_XBIND_USERNAME, bind_username);
	i_reg_status = atoi(reg_status);
	i_reg_result = atoi(reg_result);
	i_reg_newstatus = atoi(reg_newstatus);
	i_reg_newresult = atoi(reg_newresult);

	/* check whether auto register ready */
	if( TCAPI_PROCESS_OK ==
		tcapi_get(webCurSetNode, "AutoBIND_1Type_Status", typeAutoBind) )
	{
		/* check delay time and send message to ITMS+ */
		if ( 0 == strcmp(typeAutoBind, "PRE") )
		{
			/* reach the delay time, start */
			if ( time_count_bind1_reg ++ >= i_maxrand_time )
			{
				time_count_bind1_reg = 0;
				tcapi_set(webCurSetNode, "AutoBIND_1Type_Status", "START");
				memset(&message, 0, sizeof(message));
				message.cwmptype = BIND1_REGISTER;
				tcdbg_printf("\nsent message[%d] to tr069!\n", BIND1_REGISTER);
				if(sendmegq(1, &message, 0) < 0)
					tcdbg_printf("\nsend message to cwmp error!\n");
				retry_bind1_reg ++;
				tcapi_set(deviceAccountNode, "newStatus", "0");
				tcapi_set(deviceAccountNode, "newResult", "0");
				tcapi_set(deviceAccountNode, ATTR_XBIND_TYPE, "BIND1");
				tcapi_set(deviceAccountNode, ATTR_XBIND_RESULT, "BINDING");
			}

			return;
		}
		/* check whether register to ITMS+ successfully. */
		else if ( 0 == strcmp(typeAutoBind, "START") )
		{
			if ( (1 == i_reg_newstatus)
				&& ((0 == i_reg_status && 1 == i_reg_result
					&& 1 == i_reg_newresult)
				|| (5 == i_reg_status)) )
			{
				tcapi_set(webCurSetNode, "AutoBIND_1Type_Status", "None");
				/* save the register status to <deviceAccount_Entry> */
				tcapi_set(deviceAccountNode, ATTR_XBIND_RESULT, "SUCC");
				time_count_bind1_reg = 0;
			}
			else
			{
				/* more than 3 minutes(180) or register failed. */
				if ( (time_count_bind1_reg ++ >= 200 && 0 == i_reg_newstatus) 
					|| ( 0 != i_reg_status && 1 == i_reg_newstatus ))
				{
					time_count_bind1_reg = 0;
					if ( retry_bind1_reg >= 3 ) /* timeout */
					{
						retry_bind1_reg = 0;
						tcapi_set(webCurSetNode,
							"AutoBIND_1Type_Status", "None");

						/* save the register status to <deviceAccount_Entry> */
						if ( 0 == i_reg_newstatus )
							tcapi_set(deviceAccountNode,
								ATTR_XBIND_RESULT, "TIMEOUT");
						else
							tcapi_set(deviceAccountNode,
								ATTR_XBIND_RESULT, "REGFAIL");

						return;
					}

					memset(&message, 0, sizeof(message));
					message.cwmptype = BIND1_REGISTER;
					tcdbg_printf("\nre-sent message=[%d]-[%d] to tr069!\n",
						retry_bind1_reg, BIND1_REGISTER);
					if(sendmegq(1, &message, 0) < 0)
						tcdbg_printf("\nsend message to cwmp error!\n");
					retry_bind1_reg ++;
					tcapi_set(deviceAccountNode, "newStatus", "0");
					tcapi_set(deviceAccountNode, "newResult", "0");
					tcapi_set(deviceAccountNode, ATTR_XBIND_TYPE, "BIND1");
 					tcapi_set(deviceAccountNode, ATTR_XBIND_RESULT, "BINDING");
				}

			}

			return;
		}
	}

	/* start ppp detect */
	if ( 0 == i_detect_st )
		doValPut(FILE_PPP_ST, "1");

	fp = fopen(FILE_PPP_ST, "r");
	if ( !fp )
		return;

	fgets(line_buf, sizeof(line_buf), fp);
	fclose(fp);

	time_count_bind1_reg = 0;
	/* get ppp status and username */
	sscanf(line_buf, "%d %s %s %s", &i_detect_st, 
					status, br_name, ppp_username);
	if ( 0 == strcmp(status, "up") )
	{
#if 0
		/* do nothing if already registered */
		if ( 0 == i_reg_status && 1 == i_reg_result
			&& 0 == strcmp(bind_username, ppp_username) )
			return;
#endif
		/* stop ppp detect */
		doValPut(FILE_PPP_ST, "0");
		i_detect_st = 0;
		tcapi_set(webCurSetNode, ATTR_XBIND_USERNAME, ppp_username);
		tcapi_set(webCurSetNode, "AutoBINDBrName", br_name);
		retry_bind1_reg = 0;
		/* generate rand time delay */
		if ( (TCAPI_PROCESS_OK !=
			tcapi_get(deviceAccountNode, "BIND1MaxRandTime", max_randtime))
			|| 0 == max_randtime[0] )
			strcpy(max_randtime, "1");
		srand((int)time(NULL));
		i_maxrand_time = rand() % (atoi(max_randtime));
		tcapi_set(webCurSetNode, "AutoBIND_1Type_Status", "PRE");
		tcdbg_printf("\nrand delay time is =[%d]!\n", i_maxrand_time);
	}

	return;

}
#endif


#if defined(TCSUPPORT_CT_JOYME)
void check_olinetime(void){
	char *path = "proc/br_fdb_host/stb_list";
	FILE *fp = NULL;
	FILE *fp1 = NULL;
	int i=0,devnum = 0,k = 0,devnum1 = 0;
	char tempbuf[64] = {0};
	char devname[64] = {0};
	char osname[64] = {0};
	char brandname[64] = {0};
	char buf[512] = {0},buf1[512] = {0};
	char mac[20] = {0};
	unsigned long olinetime = 0;
	int flag = 0;
	char *pBuf[MAX_LDEV_NUM];
	char *pBuf1[MAX_LDEV_NUM];
	int isintable = 0;
	long len = 0,linelen = 0;
	static int count = 0;

	if (count++ < 5)
		return;
	count = 0;

	for(i=0; i<MAX_LDEV_NUM; i++){
		pBuf[i] = NULL;
		pBuf1[i] = NULL;
		pBuf[i] = (char *)malloc(20);
		pBuf1[i] = (char *)malloc(20);
		if(NULL == pBuf[i] || NULL == pBuf1[i])
			goto FreeBUF;
		memset(pBuf[i], 0, 20);
		memset(pBuf1[i], 0, 20);
	}
	
	fp = fopen(path,"r");
	fp1 = fopen(ATTACH_LANINFO_PATH,"r+");
	if(fp == NULL)
		goto FreeBUF;

	if(fp1 == NULL){
		fp1 = fopen(ATTACH_LANINFO_PATH,"w");
		if(fp1 == NULL)
			goto FreeBUF;
		while(fgets(buf,sizeof(buf),fp))
		{
			sscanf(buf, "%*[^=]=%s",mac);
			olinetime ++;
			flag =1;
			memset(buf,0,sizeof(buf));
			strcpy(devname,"anonymous");
			strcpy(osname,"anonymous");
			strcpy(brandname,"anonymous");
			fprintf(fp1,"%20s %10lu %5d %64s %64s %64s\n",mac,olinetime,flag,
				devname,osname,brandname);
			olinetime = 0;
			memset(buf,0,sizeof(buf));
			devnum++;
			if(devnum >=64)
				break;
		}
	}
	else{
		i = 0;
		while(fgets(buf,sizeof(buf),fp))
		{
			sscanf(buf, "%*[^=]=%s",mac);
			sprintf(pBuf[devnum],"%s",mac);
			isintable = 0;
			len = 0;
			while(fgets(buf1,sizeof(buf1),fp1))
			{
				linelen = strlen(buf1);
				len = len + linelen;
				sscanf(buf1, "%s %lu %d %s %s %s",tempbuf,&olinetime,&flag,
					devname,osname,brandname);
				if(!strcmp(tempbuf,mac)){
					olinetime+=5;
					flag =1;
					len -= strlen(buf1);
					fseek(fp1,len,SEEK_SET);
					fprintf(fp1,"%20s %10lu %5d %64s %64s %64s\n",tempbuf,olinetime,flag,
						devname,osname,brandname);
					isintable = 1;
					break;
				}
				i++;
				memset(buf1,0,sizeof(buf1));
			}
			if(isintable == 0){
				if(i >= 64)
					break;
				i++;
				olinetime =1;
				flag = 1;
				strcpy(devname,"anonymous");
				strcpy(osname,"anonymous");
				strcpy(brandname,"anonymous");
				fprintf(fp1,"%20s %10lu %5d %64s %64s %64s\n",mac,olinetime,flag,
					devname,osname,brandname);
			}
			devnum++;
			memset(buf,0,sizeof(buf));
			fseek(fp1,0,SEEK_SET);
			i = 0;
		}

		while(fgets(buf1,sizeof(buf1),fp1))
		{
			memset(tempbuf,0,sizeof(tempbuf));
			sscanf(buf1, "%s %*lu %*d %*s %*s %*s",tempbuf);
			sprintf(pBuf1[devnum1],"%s",tempbuf);
			devnum1++;
			if(devnum1 >= 64)
				break;
		}
		for(i=0;i<devnum1;i++){
			isintable = 0;
			for(k=0;k<devnum;k++){
				if(!strcmp(pBuf1[i],pBuf[k])){
					isintable = 1;
					break;
				}
			}
			if(isintable == 0){
				fseek(fp1,0,SEEK_SET);
				len = 0;
				while(fgets(buf1,sizeof(buf1),fp1))
				{
					linelen = strlen(buf1);
					len += linelen;
					sscanf(buf1, "%s %lu %d %s %s %s",tempbuf,&olinetime,&flag,
						devname,osname,brandname);
					if(!strcmp(tempbuf,pBuf1[i])){
						olinetime =0;
						flag =0;
						len -= linelen;
						fseek(fp1,len,SEEK_SET);
						fprintf(fp1,"%20s %10lu %5d %64s %64s %64s\n",tempbuf,olinetime,flag,
							devname,osname,brandname);
						break;
					}
					memset(buf1,0,sizeof(buf1));
				}
			}
		}
	}
	
FreeBUF:
	for(i=0; i<MAX_LDEV_NUM; i++){
		if(NULL != pBuf[i]){
			free(pBuf[i]);
			pBuf[i] = NULL;
		}
		if(NULL != pBuf1[i]){
			free(pBuf1[i]);
			pBuf1[i] = NULL;
		}
	}
	if(NULL != fp) fclose(fp);
	if(NULL != fp1) fclose(fp1);
	return;
}

#define WPS_ADD_IP_PATH "usr/osgi/config/ip.conf"
void check_wpsstatus(void)
{
	char nodename[64] = {0};
	char temp[64] = {0};
	int i = 0,number = 0,k=0;
	char *ipbuf[MAX_LEN_OF_MAC_TABLE];
	static char ipbuf1[MAX_LEN_OF_MAC_TABLE][20];
	static int idle_number = 0,isconfigured = 0,time = 0;
	int isintable = 0;
	FILE *fp = NULL;

	for(i=0;i<MAX_LEN_OF_MAC_TABLE;i++)
	{
		ipbuf[i] = (char *)malloc(20);
		if(ipbuf[i] == NULL)
			goto FreeBUF;
		memset(ipbuf[i],0,sizeof(ipbuf[i]));
	}
	strcpy(nodename,"Info_WLan");
	tcapi_get(nodename,"wlanWPSStatus",temp);
	if(!strcmp(temp,"Configured")){
		if(isconfigured == 1)
			goto FreeBUF;

		fp = fopen(WPS_ADD_IP_PATH,"w");
		if(fp == NULL)
			goto FreeBUF;
		memset(nodename,0,sizeof(nodename));
		strcpy(nodename,"wifiMacTab_Common");
		tcapi_get(nodename,"NUM0",temp);
		number = atoi(temp);
		for(i = 0;i<number;i++)
		{
			memset(nodename,0,sizeof(nodename));
			sprintf(nodename,"wifiMacTab_Entry%d",i);
			tcapi_get(nodename,"IP0",temp);
			sprintf(ipbuf[i],"%s",temp);
			for(k = 0;k<idle_number;k++){
				if(!strcmp(ipbuf[i],ipbuf1[k])){
					isintable = 1;
					break;
				}
			}
			if(isintable == 0){
				fprintf(fp,"%s\n",ipbuf[i]);
			}
			isintable = 0;
			
		}
		isconfigured = 1;
	}
	else if(!strcmp(temp,"In progress")){
		if(time++ < 2)
			goto FreeBUF;
		time = 0;
		isconfigured = 0;
		memset(nodename,0,sizeof(nodename));
		strcpy(nodename,"wifiMacTab_Common");
		tcapi_get(nodename,"NUM0",temp);
		idle_number = atoi(temp);

		for(i = 0;i<idle_number;i++)
		{
			memset(ipbuf1[i],0,sizeof(ipbuf1[i]));
			memset(nodename,0,sizeof(nodename));
			sprintf(nodename,"wifiMacTab_Entry%d",i);
			tcapi_get(nodename,"IP0",temp);
			sprintf(ipbuf1[i],"%s",temp);
		}
		for(i = idle_number;i<MAX_LEN_OF_MAC_TABLE;i++)
			memset(ipbuf1[i],0,sizeof(ipbuf1[i]));
	}

FreeBUF:
	for(i=0; i<MAX_LEN_OF_MAC_TABLE; i++){
		if(NULL != ipbuf[i]){
			free(ipbuf[i]);
			ipbuf[i] = NULL;
		}
	}
	if(NULL != fp)
		fclose(fp);
	return;
}
#endif

void call_notify_dslite_state()
{
	while(1)
	{
#if defined(TCSUPPORT_CT_DSLITE)
		notify_dslite_state();
#endif
		sleep(3);
	}
	pthread_exit(0);
}

