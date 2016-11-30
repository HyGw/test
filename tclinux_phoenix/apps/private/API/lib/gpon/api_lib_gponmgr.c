/***************************************************************
Copyright Statement:

This software/firmware and related documentation (EcoNet Software) 
are protected under relevant copyright laws. The information contained herein 
is confidential and proprietary to EcoNet (HK) Limited (EcoNet) and/or 
its licensors. Without the prior written permission of EcoNet and/or its licensors, 
any reproduction, modification, use or disclosure of EcoNet Software, and 
information contained herein, in whole or in part, shall be strictly prohibited.

EcoNet (HK) Limited  EcoNet. ALL RIGHTS RESERVED.

BY OPENING OR USING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY 
ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
DOCUMENTATIONS (ECONET SOFTWARE) RECEIVED FROM ECONET 
AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON AN AS IS 
BASIS ONLY. ECONET EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, 
WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
OR NON-INFRINGEMENT. NOR DOES ECONET PROVIDE ANY WARRANTY 
WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTIES WHICH 
MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE ECONET SOFTWARE. 
RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTIES FOR ANY AND ALL 
WARRANTY CLAIMS RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES 
THAT IT IS RECEIVERS SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVERS SPECIFICATION OR CONFORMING TO A PARTICULAR 
STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND 
ECONET'S ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE ECONET 
SOFTWARE RELEASED HEREUNDER SHALL BE, AT ECONET'S SOLE OPTION, TO 
REVISE OR REPLACE THE ECONET SOFTWARE AT ISSUE OR REFUND ANY SOFTWARE 
LICENSE FEES OR SERVICE CHARGES PAID BY RECEIVER TO ECONET FOR SUCH 
ECONET SOFTWARE.
***************************************************************/

/************************************************************************
*                  I N C L U D E S
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_packet.h>
#include <stddef.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <pthread.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include "libapi_gponmgr_internal.h"
#include "ecnt_lib_dbg.h"


/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
************************************************************************/
#define IPCKEY                              (0X20160303)
#define EVENT_IPC_ID                         0

/************************************************************************
*                  M A C R O S
************************************************************************/
#define OMCI_INTERFACE 	    "omci"
#define PON_DEV 			"/dev/pon"
#define DEFAULT_IPC_PATH    "/tmp/gpon_api_ipc_path"

#define MAX_OMCI_PKT_LEN (1980)
#define MAX_OMCI_SOFTIMAGE_UPDATE_WINDOW (256)

#define LIB_GPON_ERROR(fmt, ...)     ECNT_LIB_DBG_PRINT(ECNT_DBG_ERROR, ECNT_DBG_TYPE_GPON, fmt, ##__VA_ARGS__)
#define LIB_GPON_WARN(fmt, ...)      ECNT_LIB_DBG_PRINT(ECNT_DBG_WARN, ECNT_DBG_TYPE_GPON, fmt, ##__VA_ARGS__)
#define LIB_GPON_NOTICE(fmt, ...)    ECNT_LIB_DBG_PRINT(ECNT_DBG_NOTICE, ECNT_DBG_TYPE_GPON, fmt,##__VA_ARGS__)
#define LIB_GPON_DEBUG(fmt, ...)     ECNT_LIB_DBG_PRINT(ECNT_DBG_DEBUG, ECNT_DBG_TYPE_GPON, fmt,##__VA_ARGS__)

#define PRINT_ARGS(fmt, ...)         ECNT_LIB_DBG_PRINT(ECNT_DBG_DEBUG, ECNT_DBG_TYPE_GPON, "[in args] "fmt,##__VA_ARGS__)

#define FUNC_TRACE_ENTER           LIB_GPON_DEBUG("===Enter===\n") 

#define RETURN_FAIL_MSG(fmt, ...)   \
    do{\
        LIB_GPON_ERROR("===Fail Exit==="fmt,##__VA_ARGS__); \
        return ECONET_E_FAIL; \
    }while(0)

#define RETURN_FAIL   \
    do{\
        LIB_GPON_DEBUG("===Fail Exit===\n"); \
        return ECONET_E_FAIL; \
    }while(0)

#define RETURN_SUCCESS \
    do{\
        LIB_GPON_DEBUG("===Success Exit===\n"); \
        return ECONET_E_SUCCESS; \
    }while(0)

#define offsetofend(type,member) (offsetof(type,member)+sizeof(((type*)0)->member))
/************************************************************************
*                  D A T A   T Y P E S
************************************************************************/
struct {
    struct{
        unsigned char msg[MAX_OMCI_PKT_LEN]; /*omci pkt body*/
        unsigned short len;                  /*omci pkt len*/
    }buf[MAX_OMCI_SOFTIMAGE_UPDATE_WINDOW];  /*buf to hold omci pkts*/

    sem_t sem;              /*inter-thread sync semaphore*/
    unsigned short r_idx;   /*read index*/
    unsigned short w_idx;   /*write index*/
} _omciRxBuf;

typedef struct{
    unsigned short map[CONFIG_GPON_MAX_TCONT];
    pthread_rwlock_t rwlock;
}tcont_allocid_t;

typedef struct{
    pthread_mutex_t event_ctrl_mutexlock;
}event_ctrl_data_t;

/*queue struct for event store*/
typedef struct {
    int capacity;
    int front;
    int rear;
    int size;
    XPON_EVENT_t event[XMCS_EVENT_GPON_MAX];
}event_queue_t;

typedef struct{
    Event_Filter_Type filter_type;
    union{
        unsigned short     black_list[XMCS_EVENT_GPON_MAX];
        unsigned short     white_list[XMCS_EVENT_GPON_MAX];
        }list;
    unsigned short list_length;
    XMCS_GponEventId_t mapper_src[XMCS_EVENT_GPON_MAX];
    unsigned short     mapper_dst[XMCS_EVENT_GPON_MAX];
      
    // todo: add more if you wish
} event_poll_policy_t;



/************************************************************************
*                  E X T E R N A L   D A T A   D E C L A R A T I O N S
************************************************************************/

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
************************************************************************/

/************************************************************************
*                  P U B L I C   D A T A
************************************************************************/

/************************************************************************
*                  P R I V A T E   D A T A
************************************************************************/
static int   _omci_socket = -1;
static pthread_t * _omciRxThread = NULL;

static event_poll_policy_t _event_poll_policy;
static event_queue_t _event_queue = {
    .capacity = XMCS_EVENT_GPON_MAX,
    .front = 1,
    .rear = 0,
    .size = 0,
    .event = {},
};
static tcont_allocid_t * _tcont_allocid_map = NULL;
static event_ctrl_data_t * _event_ctrl_data_t = NULL;

static char _ipc_path[MAX_IPC_PATH_LEN] = DEFAULT_IPC_PATH; 
/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
************************************************************************/

/******************************************************************************
 Descriptor:    It's used to ioctl to gpon device.
 Input Args:    cmd: ioctl command.
                     p: pointer to gpon driver structure, it is used to 
 Ret Value: success: ECONET_E_SUCCESS
               fail: ECONET_E_FAIL
******************************************************************************/
static int gponmgr_lib_ioctl(int cmd, void *arg)
{
    int fd = -1;

    PRINT_ARGS("cmd:%d arg:%x\n", cmd, arg);
    
    fd = open(PON_DEV, O_RDONLY);
    if (fd < 0)
    {
        RETURN_FAIL_MSG("Couldn't open %s, fd = %d.\n", PON_DEV, fd);
    }

    if (ioctl(fd, cmd, arg) < 0) 
    {
        close(fd);
        RETURN_FAIL_MSG("ioctl error!\n");
    }

    close(fd);
    RETURN_SUCCESS;

}
/******************************************************************************
 Descriptor:    It's used to initialize a block of shared memory to store tcont
                to allocid mapping. The shared memory address is stored in 
                g_tcont_to_allocid_map.
 Input Args:    NONE.
 Ret Value: success: ECONET_E_SUCCESS
               fail: ECONET_E_FAIL
******************************************************************************/
static int init_tcont_to_allocid_map_data(void)
{
    int shmid = -1;
    pthread_rwlockattr_t rwlattr;
    
    FUNC_TRACE_ENTER;
    
    if (NULL != _tcont_allocid_map){
        RETURN_SUCCESS;
    }

    shmid = shmget((key_t)IPCKEY, sizeof(tcont_allocid_t) , 0666);
    if(-1 == shmid){
        LIB_GPON_DEBUG("SHARE MEMORY NOT CREATE!\n");
        shmid = shmget((key_t)IPCKEY, sizeof(tcont_allocid_t) , 0666|IPC_CREAT);
        if(-1 == shmid){
            RETURN_FAIL_MSG("CREATE SHARE MEMORY FAILED\n");
        }
        _tcont_allocid_map = (tcont_allocid_t*)shmat(shmid, (void *)0, 0);
        if (NULL == _tcont_allocid_map){
            RETURN_FAIL_MSG("ATTACH SHARE MEMORY ERROR!\n");
        }
        
        memset(_tcont_allocid_map, 0, sizeof(tcont_allocid_t));
        
        /* Process Shared Read/Write lock */      
        if(0 !=pthread_rwlockattr_init(&rwlattr)){
            RETURN_FAIL;
        }        
        if(0 !=pthread_rwlockattr_setpshared(&rwlattr, PTHREAD_PROCESS_SHARED)){
            RETURN_FAIL;
        }    
        if(0 !=pthread_rwlock_init(&_tcont_allocid_map->rwlock, &rwlattr)){
            RETURN_FAIL;
        }  
        
    }
    else{
        _tcont_allocid_map = (tcont_allocid_t*)shmat(shmid, (void *)0, 0);
        
        if (NULL == _tcont_allocid_map){
            RETURN_FAIL_MSG("ATTACH SHARE MEMORY ERROR!\n");
        }
    }
    RETURN_SUCCESS;
}
static init_ipc_path(void)
{
    FILE *fp = NULL;

    fp = fopen(_ipc_path, "w");
    if(NULL==fp){
        RETURN_FAIL_MSG("fopen default ipc path failed\n");
    }

    fclose(fp);

    RETURN_SUCCESS;
}
/******************************************************************************
 Descriptor:    It's used to initialize a block of shared memory to restore _event_ctrl_data_t,and 
                    initialize the pthread_mutex_lock
 Input Args:    NONE.
 Ret Value: success: ECONET_E_SUCCESS
               fail: ECONET_E_FAIL
******************************************************************************/
static int init_event_ctrlflag_data(void)
{
    int shmid = -1;
    int key = -1;
    pthread_mutexattr_t attr;
    
    FUNC_TRACE_ENTER;    

    if (NULL != _event_ctrl_data_t){
        RETURN_SUCCESS;
    }

    key = ftok(_ipc_path, EVENT_IPC_ID);
    if(-1==key){
        RETURN_FAIL_MSG("ftok return failed\n");
    }
    printf("\nkey:%d\n",key);
    shmid = shmget((key_t)key, sizeof(event_ctrl_data_t) , 0666);
    if(-1 == shmid){
        LIB_GPON_DEBUG("SHARE MEMORY NOT CREATE!\n");
        shmid = shmget((key_t)IPCKEY, sizeof(event_ctrl_data_t) , 0666|IPC_CREAT);
        if(-1 == shmid){
            RETURN_FAIL_MSG("CREATE SHARE MEMORY FAILED\n");
        }
        _event_ctrl_data_t= (event_ctrl_data_t*)shmat(shmid, (void *)0, 0);
        if (NULL == _event_ctrl_data_t){
            RETURN_FAIL_MSG("ATTACH SHARE MEMORY ERROR!\n");
        }
        
        memset(_event_ctrl_data_t, 0, sizeof(event_ctrl_data_t));
        
        /* Process Shared mutex lock */      
        if(0 !=pthread_mutexattr_init(&attr)){
            RETURN_FAIL;
        }        
        if(0 !=pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED)){
            RETURN_FAIL;
        }    
        if(0 !=pthread_mutex_init(&_event_ctrl_data_t->event_ctrl_mutexlock, &attr)){
            RETURN_FAIL;
        }  
        
    }
    else{
        _event_ctrl_data_t = (event_ctrl_data_t*)shmat(shmid, (void *)0, 0);
        
        if (NULL == _event_ctrl_data_t){
            RETURN_FAIL_MSG("ATTACH SHARE MEMORY ERROR!\n");
        }
    }
    RETURN_SUCCESS;
    


}
/******************************************************************************
 Descriptor:    internal omci rx thread function
 Input Args:    arg: arg passed to thread routine
 Ret Value: success: return 0
******************************************************************************/
static void * omci_rx_task(void * arg){

    int sock_f = (int)arg;
    unsigned int pktLen ;
    unsigned char * buf = NULL;
    
    FUNC_TRACE_ENTER;
    PRINT_ARGS("arg:%d\n",arg);

    while(1)
    {
        buf = _omciRxBuf.buf[_omciRxBuf.w_idx].msg;
        pktLen = MAX_OMCI_PKT_LEN;

        pktLen = recv(sock_f, buf, pktLen, 0);
        
        LIB_GPON_DEBUG("recv pktLen:%d\n",pktLen);
          
        if(pktLen <= 0){
            LIB_GPON_ERROR("msg recv len <=0 !\n");
            continue;
        }  

        _omciRxBuf.buf[_omciRxBuf.w_idx].len = pktLen;
        _omciRxBuf.w_idx = (_omciRxBuf.w_idx + 1) & 0xFF;

        if(_omciRxBuf.w_idx == _omciRxBuf.r_idx){
            LIB_GPON_ERROR("omci mesg queue overrun! Drop newly arrived OMCI pkt!\n");
            _omciRxBuf.w_idx = (_omciRxBuf.w_idx - 1) & 0xFF;
            continue;
        }
        
        if(0 != sem_post(&_omciRxBuf.sem) ){
            LIB_GPON_ERROR("sem_post failed!\n");
            _omciRxBuf.w_idx = (_omciRxBuf.w_idx - 1) & 0xFF;
        }
    }

    return 0;
}

/******************************************************************************
 Descriptor:    init interal thread
 Input Args:    thread_func:  thread routine
                pp_thread:    new thread id
                arg:          arg passed to thread routine
 Ret Value: success: if success, return ECONET_E_SUCCESS
                     if fali, return ECONET_E_FAIL
******************************************************************************/
static int init_interal_thread(void* (*thread_func) (void*), pthread_t ** pp_thread,
                               void* arg)
{
	pthread_attr_t attr;
    pthread_t * p_thread = NULL;

    FUNC_TRACE_ENTER;

    PRINT_ARGS("thread_func:%x, pp_thread:%x, arg:%x", thread_func, pp_thread, arg);
    
    if((NULL == thread_func) || (NULL == pp_thread) ){
        RETURN_FAIL;
    }

    p_thread = (pthread_t *)malloc(sizeof(pthread_t) );
    if(NULL == p_thread){
 		RETURN_FAIL_MSG("malloc fail!\n");
    }

	if(pthread_attr_init(&attr) != 0)
	{
		RETURN_FAIL_MSG("pthread_attr_init fail!\n");
	}

	if(pthread_attr_setstacksize(&attr, 16384) != 0)
	{
		RETURN_FAIL_MSG("pthread_attr_setstacksizefail!\n");
	}
    
	if(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0)
	{
		RETURN_FAIL_MSG("pthread_attr_setdetachstate attribute fail!");
	}

	if(pthread_create(p_thread , &attr, thread_func, arg) != 0)
	{
        free(p_thread);
		RETURN_FAIL_MSG("pthread_create fail!");
	}

    *pp_thread = p_thread;
	pthread_attr_destroy(&attr);
    
	RETURN_SUCCESS;
}

/******************************************************************************
 Descriptor:    start omci rx thread, and init necessary data
 Input Args:    sock_f: socket descriptor used to recv omci pkts
 Ret Value: success: ECONET_E_SUCCESS
               fail: ECONET_E_FAIL
******************************************************************************/
static int init_omci_rx_thread(int sock_f)
{
    FUNC_TRACE_ENTER;
    PRINT_ARGS("sock_f:%d", sock_f);
    
    if(0 != sem_init(&_omciRxBuf.sem, 0, 0) ){
        RETURN_FAIL_MSG("sem_init fail!\n");
    }

    _omciRxBuf.r_idx = 0;
    _omciRxBuf.w_idx = 0;
    
    if( ECONET_E_SUCCESS != init_interal_thread(omci_rx_task, &_omciRxThread, (void*)sock_f) ){
        RETURN_FAIL_MSG("init_interal_thread fail!\n");
    }

    RETURN_SUCCESS;
}


/******************************************************************************
 Descriptor:    get allocid from tcont_to_allocid_map
 Input Args:    tcont 
 Output Args:   allocid
 Ret Value: success: ECONET_E_SUCCESS
               fail: ECONET_E_FAIL
******************************************************************************/

static int get_allocid(unsigned short tcont, unsigned short * allocid)
{
    FUNC_TRACE_ENTER;

    PRINT_ARGS("tcont:%u, allocid:%x\n",tcont, allocid);
    
    if(NULL == allocid){
        RETURN_FAIL_MSG("INPUT ALLOCID IS NULL\n");
    }
    
    if((NULL == _tcont_allocid_map ) && (ECONET_E_SUCCESS != init_tcont_to_allocid_map_data() ) ){
        RETURN_FAIL_MSG("INIT TCONT ALLOCID MAP ERROR!\n");
    }

    pthread_rwlock_rdlock(&_tcont_allocid_map->rwlock);
    *allocid = _tcont_allocid_map->map[tcont];
    pthread_rwlock_unlock(&_tcont_allocid_map->rwlock);

    LIB_GPON_DEBUG("output allocid:%u\n",*allocid);
    RETURN_SUCCESS;
    
}
/******************************************************************************
 Descriptor:    set allocid to tcont_to_allocid_map
 Input Args:    tcont , allocid
 Ret Value: success: ECONET_E_SUCCESS
               fail: ECONET_E_FAIL
******************************************************************************/
static int set_allocid(unsigned short tcont,unsigned short allocid){

    FUNC_TRACE_ENTER;

    PRINT_ARGS("tcont:%u, allocid:%u\n",tcont, allocid);
    if((NULL == _tcont_allocid_map ) && (ECONET_E_SUCCESS != init_tcont_to_allocid_map_data() ) ){
        RETURN_FAIL_MSG("INIT TCONT ALLOCID MAP ERROR!\n");
    }

    pthread_rwlock_wrlock(&_tcont_allocid_map->rwlock);
    _tcont_allocid_map->map[tcont]=allocid;
    pthread_rwlock_unlock(&_tcont_allocid_map->rwlock);

    return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:    It's used to detach a block of shared memory to store tcont
                to allocid mapping. The shared memory address is stored in 
                g_tcont_to_allocid_map.and delete this share memory.
 Input Args:    NONE.
 Ret Value: success: ECONET_E_SUCCESS
               fail: ECONET_E_FAIL
******************************************************************************/
static int deinit_tcont_to_allocid_map_data(void)
{
    int shmid = -1;
    
    FUNC_TRACE_ENTER; 

    shmid = shmget((key_t)IPCKEY, sizeof(tcont_allocid_t), 0666 );
    if(-1 == shmid){
        RETURN_FAIL_MSG("GET SHAREMEM ID FAIL!\n");
    }

    pthread_rwlock_destroy(&_tcont_allocid_map->rwlock);
    
    if(-1 == shmdt(_tcont_allocid_map)){
        RETURN_FAIL_MSG("DETACH SHARE MEMORY ERROR!\n");
    }

    if(-1 == shmctl(shmid, IPC_RMID, 0)){
        RETURN_FAIL_MSG("DELETE SHARE MEMORY ERROR!\n");
    }
    
    RETURN_SUCCESS;
}
/******************************************************************************
 Descriptor:    It's used to initialize gpon manager lib.
 Input Args:    NONE.
 Ret Value: success: ECONET_E_SUCCESS
               fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_init(void)
{
    FUNC_TRACE_ENTER;
    
    if(ECONET_E_SUCCESS != init_tcont_to_allocid_map_data()){
        RETURN_FAIL_MSG("init_tcont_to_allocid_map_data fail!\n");
    }
    
    if(ECONET_E_SUCCESS != init_ipc_path()){
        RETURN_FAIL_MSG("init_default_ipc_path!\n");
    }

    if(ECONET_E_SUCCESS != init_event_ctrlflag_data()){
        RETURN_FAIL_MSG("init_event_ctrlflag_data fail!\n");
    }
    RETURN_SUCCESS;
}
static int config_ipc_path(gpon_lib_config_t * data, int mask)
{
    if(0==access(data->ipc_path, R_OK)){
        strncpy(_ipc_path, data->ipc_path, MAX_IPC_PATH_LEN);        
        LIB_GPON_DEBUG("input ipc_path:%s\n",_ipc_path);
    }

    RETURN_SUCCESS;
}
int gponmgr_lib_advanced_init(gpon_lib_config_t * data, int mask, unsigned int size)
{
    FUNC_TRACE_ENTER;

    if(NULL==data){
        RETURN_FAIL_MSG("input param is null\n");
    }
    
    if((size >= offsetofend(gpon_lib_config_t,ipc_path))&&(mask&CONFIG_IPC_PATH)){
        if(ECONET_E_SUCCESS!=config_ipc_path(data,mask)){
            RETURN_FAIL_MSG("config ipc path failed\n");
        }
    }
    /*if you add a member in gpon_lib_config_t,you can reference the example below*/
    /*
       if((size >= tailoffsetof(gpon_lib_config_t,member)){
            if(ECONET_E_SUCCESS!=config_member(data)&&(mask&CONFIG_member)){
                    //return fail or only warning.
            }

       }
        */
    if(ECONET_E_SUCCESS!=gponmgr_lib_init()){
        RETURN_FAIL;
    }

    RETURN_SUCCESS;   
}
/******************************************************************************
 Descriptor:    It's used to deinitialize gpon manager lib.
 Input Args:    NONE.
 Ret Value: success: ECONET_E_SUCCESS
               fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_deinit(void)
{
    FUNC_TRACE_ENTER;

    if(ECONET_E_SUCCESS != deinit_tcont_to_allocid_map_data()){
        RETURN_FAIL_MSG("deinit_tcont_to_allocid_map_data fail!\n");
    }

    RETURN_SUCCESS;
}

/******************************************************************************
 Descriptor:    It's used to set the GPON MAC act debug level.
 Input Args:    enable: GPONMGR_ENABLE/GPONMGR_DISABLE act debug level.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dbg_level_act(XPON_Mode_t enable)
{
    struct XMCS_DebugLevel_S dbg;
    
    FUNC_TRACE_ENTER;

    dbg.mask = MSG_ACT;
    dbg.enable = enable;

    return gponmgr_lib_ioctl(IO_IOS_DBG_LEVEL, &dbg);
}

/******************************************************************************
 Descriptor:    It's used to set the GPON MAC omci debug level.
 Input Args:    GPONMGR_ENABLE/GPONMGR_DISABLE omci debug level.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dbg_level_omci(XPON_Mode_t enable)
{
    struct XMCS_DebugLevel_S dbg;
    
    FUNC_TRACE_ENTER;

    dbg.mask = MSG_OMCI;
    dbg.enable = enable;

    return gponmgr_lib_ioctl(IO_IOS_DBG_LEVEL, &dbg);
}

/******************************************************************************
 Descriptor:    It's used to set the GPON MAC oam debug level.
 Input Args:    GPONMGR_ENABLE/GPONMGR_DISABLE oam debug level.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dbg_level_oam(XPON_Mode_t enable)
{
    struct XMCS_DebugLevel_S dbg;
    
    FUNC_TRACE_ENTER;

    dbg.mask = MSG_OAM;
    dbg.enable = enable;

    return gponmgr_lib_ioctl(IO_IOS_DBG_LEVEL, &dbg);
}

/******************************************************************************
 Descriptor:    It's used to set the GPON MAC int debug level.
 Input Args:    GPONMGR_ENABLE/GPONMGR_DISABLE int debug level.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dbg_level_int(XPON_Mode_t enable)
{
    struct XMCS_DebugLevel_S dbg;
    
    FUNC_TRACE_ENTER;

    dbg.mask = MSG_INT;
    dbg.enable = enable;

    return gponmgr_lib_ioctl(IO_IOS_DBG_LEVEL, &dbg);
}

/******************************************************************************
 Descriptor:    It's used to set the GPON MAC trace debug level.
 Input Args:    GPONMGR_ENABLE/GPONMGR_DISABLE trace debug level.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dbg_level_trace(XPON_Mode_t enable)
{
    struct XMCS_DebugLevel_S dbg;
    
    FUNC_TRACE_ENTER;

    dbg.mask = MSG_TRACE;
    dbg.enable = enable;

    return gponmgr_lib_ioctl(IO_IOS_DBG_LEVEL, &dbg);
}

/******************************************************************************
 Descriptor:    It's used to set the GPON MAC warning debug level.
 Input Args:    pgponEqdOffset: GPONMGR_ENABLE/GPONMGR_DISABLE warning debug level.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dbg_level_warning(XPON_Mode_t enable)
{
    struct XMCS_DebugLevel_S dbg;
    
    FUNC_TRACE_ENTER;

    dbg.mask = MSG_WARN;
    dbg.enable = enable;

    return gponmgr_lib_ioctl(IO_IOS_DBG_LEVEL, &dbg);
}

/******************************************************************************
 Descriptor:    It's used to set the GPON MAC content debug level.
 Input Args:    GPONMGR_ENABLE/GPONMGR_DISABLE content debug level.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dbg_level_content(XPON_Mode_t enable)
{
    struct XMCS_DebugLevel_S dbg;
    
    FUNC_TRACE_ENTER;

    dbg.mask = MSG_CONTENT;
    dbg.enable = enable;

    return gponmgr_lib_ioctl(IO_IOS_DBG_LEVEL, &dbg);
}

/******************************************************************************
 Descriptor:    It's used to set the GPON MAC debug debug level.
 Input Args:    GPONMGR_ENABLE/GPONMGR_DISABLE debug debug level.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dbg_level_debug(XPON_Mode_t enable)
{
    struct XMCS_DebugLevel_S dbg;
    
    FUNC_TRACE_ENTER;

    dbg.mask = MSG_DBG;
    dbg.enable = enable;

    return gponmgr_lib_ioctl(IO_IOS_DBG_LEVEL, &dbg);
}

/******************************************************************************
 Descriptor:    It's used to set the GPON MAC err debug level.
 Input Args:    GPONMGR_ENABLE/GPONMGR_DISABLE err debug level.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dbg_level_err(XPON_Mode_t enable)
{
    struct XMCS_DebugLevel_S dbg;
    
    FUNC_TRACE_ENTER;

    dbg.mask = MSG_ERR;
    dbg.enable = enable;

    return gponmgr_lib_ioctl(IO_IOS_DBG_LEVEL, &dbg);
}

/******************************************************************************
 Descriptor:	It's used to set the GPON MAC EQD debug level.
 Input Args:	enable: GPON_ENABLE/GPON_DISABLE EQD debug level.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dbg_level_eqd(XPON_Mode_t enable)
{
	struct XMCS_DebugLevel_S dbg;
	
	FUNC_TRACE_ENTER;

	dbg.mask = MSG_EQD;
	dbg.enable = enable;

	return gponmgr_lib_ioctl(IO_IOS_DBG_LEVEL, &dbg);
}

/******************************************************************************
 Descriptor:	It's used to set the GPON MAC XMCS debug level.
 Input Args:	enable: GPON_ENABLE/GPON_DISABLE XMCS debug level.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dbg_level_xmcs(XPON_Mode_t enable)
{
	struct XMCS_DebugLevel_S dbg;
	
	FUNC_TRACE_ENTER;

	dbg.mask = MSG_XMCS;
	dbg.enable = enable;

	return gponmgr_lib_ioctl(IO_IOS_DBG_LEVEL, &dbg);
}

/******************************************************************************
 Descriptor:    It's used to set the EqD offset at O4 state.
 Input Args:    pgponEqdOffset: used to set O4 eqd offset value.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_eqd_offset_o4(unsigned char gponEqdO4Offset)
{
    FUNC_TRACE_ENTER;

                                                            /* uint cast is used to size alignment */
    return gponmgr_lib_ioctl(GPON_IOS_EQD_OFFSET_O4, (void *)((uint)gponEqdO4Offset));
}

/******************************************************************************
 Descriptor:    It's used to set the EqD offset at O5 state.
 Input Args:    pgponEqdOffset: used to set O5 eqd offset value.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_eqd_offset_o5(unsigned char gponEqdO5Offset)
{
    FUNC_TRACE_ENTER;

                                                            /* uint cast is used to size alignment */
    return gponmgr_lib_ioctl(GPON_IOS_EQD_OFFSET_O5, (void *)((uint)gponEqdO5Offset));
}

/******************************************************************************
 Descriptor:    It's used to set the EqD offset flag.
 Input Args:    pgponEqdOffset: used to set eqd offset with plus or minus.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_eqd_offset_flag(char gponEqdOffsetflag)
{
    FUNC_TRACE_ENTER;

    if((gponEqdOffsetflag != '+') && (gponEqdOffsetflag != '-')) {
        return ECONET_E_FAIL;
    }

                                                                /* uint cast is used to size alignment */
    return gponmgr_lib_ioctl(GPON_IOS_EQD_OFFSET_FLAG, (void *)((uint)gponEqdOffsetflag));
}

/******************************************************************************
 Descriptor:    It's used to set GPON MAC counter type.
 Input Args:    type: used to set counter type.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_counter_type(GPON_COUNTER_TYPE_t type)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOS_COUNTER_TYPE, (void *)type);
}

/******************************************************************************
 Descriptor:    It's used to set GPON response time.
 Input Args:    time: used to set GPON MAC response time.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_response_time(ushort time)
{
    FUNC_TRACE_ENTER;

                                                            /* uint cast is used to size alignment */
    return gponmgr_lib_ioctl(GPON_IOS_RESPONSE_TIME, (void *)((uint)time));
}

/******************************************************************************
 Descriptor:    It's used to set GPON MAC internal delay fine tune(BFB64208[15:8]).
 Input Args:    delay: used to set GPON MAC internal delay fine tune.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_internal_delay_fine_tune(unsigned char delay)
{
    FUNC_TRACE_ENTER;

                                                                        /* uint cast is used to size alignment */
    return gponmgr_lib_ioctl(GPON_IOS_INTERNAL_DELAY_FINE_TUNE, (void *)((uint)delay));
}

/******************************************************************************
 Descriptor:    It's used to set the GPON MAC whether burst mode overhead exceeds 128bytes or not.
 Input Args:    ptype: used to set GPON MAC whether burst mode overhead exceeds 128bytes or not.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_burst_mode_overhead(GPON_BURST_MODE_OVERHEAD_LEN_T type)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOS_BURST_MODE_OVERHEAD_LEN, (void *)type);
}

/******************************************************************************
 Descriptor:    It's used to clear GPON MAC counter.
 Input Args:    clear: clear GPON MAC counter.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_clear_gem_cnt(unsigned short gemportID)
{
    FUNC_TRACE_ENTER;

    PRINT_ARGS("gemportID:%hu\n", gemportID);

    uint clearType = GPON_SW_HW;
    
    if(GPON_MAX_GEM_ID < gemportID){
        RETURN_FAIL_MSG("Invalid gemport!\n");
    }

    if(GPON_MAX_GEM_ID == gemportID){ /*clear statistics for all gemports*/
        if(ECONET_E_SUCCESS != gponmgr_lib_ioctl(GPON_IOS_CLEAR_COUNTER, (void *)clearType) ){
            RETURN_FAIL;
        }
        RETURN_SUCCESS;
    }

    if(ECONET_E_SUCCESS != gponmgr_lib_ioctl(GPON_IOS_CLEAR_GEM_COUNTER, (void *)gemportID) ){
        RETURN_FAIL;
    }
    
    RETURN_SUCCESS;
}
/******************************************************************************
 Descriptor:    It's used to clear ppe tx or rx counter.
 Input Args:    mask: 0x1:tx cnt clear, 0x2:rx cnt clear
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_clear_wan_cnt(WAN_TX_RX_SELECT_t mask)
{
    FUNC_TRACE_ENTER;
 
    return gponmgr_lib_ioctl(IO_IOS_WAN_CNT_STATS, (void *)mask);
}
/******************************************************************************
 Descriptor:    It's used to clear GPON MAC serial number.
 Input Args:    psn: serial number.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_sn(char *psn)
{
    struct XMCS_GponSnPasswd_S gponSnPasswd ;
    
    FUNC_TRACE_ENTER;

    memcpy(gponSnPasswd.sn, psn, GPON_SN_LENS) ;

    return gponmgr_lib_ioctl(GPON_IOS_SN, (void *)&gponSnPasswd);
}

/******************************************************************************
 Descriptor:    It's used to clear GPON MAC password.
 Input Args:    ppasswd: password.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_passwd(char *ppasswd)
{
    struct XMCS_GponSnPasswd_S gponSnPasswd ;
    
    FUNC_TRACE_ENTER;

    memcpy(gponSnPasswd.passwd, ppasswd, GPON_PASSWD_LENS);

    return gponmgr_lib_ioctl(GPON_IOS_PASSWD, (void *)&gponSnPasswd);
}

/******************************************************************************
 Descriptor:    It's used to set when to begin using the new encryption key.
 Input Args:    spf: superframe counter.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_key_switch_time(uint spf)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOS_KEY_SWITCH_TIME, (void *)spf);
}

/******************************************************************************
 Descriptor:    It's used to set the encryption key.
 Input Args:    pkey: 16 unsigned char array encryption key. It stored 32 hexadecimal digits.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_encrypt_key(unsigned char *pkey)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOS_ENCRYPT_KEY, (void *)pkey);
}

/******************************************************************************
 Descriptor:    It's used to set Tod switch time.
 Input Args:    pgponTodCfg: ToD parameter structure.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_tod_switch_time(struct XMCS_GponTodCfg_S *pgponTodCfg)
{
    struct XMCS_GponTodCfg_S gponTodCfg;
        
    FUNC_TRACE_ENTER;

    memset(&gponTodCfg, 0, sizeof(gponTodCfg)) ;
    if(gponmgr_lib_get_new_tod(&gponTodCfg) == ECONET_E_FAIL) {
        return ECONET_E_FAIL;
    }
    pgponTodCfg->sec= gponTodCfg.sec;
    pgponTodCfg->nanosec= gponTodCfg.nanosec;

    return gponmgr_lib_ioctl(GPON_IOS_TOD_CFG, (void *)pgponTodCfg);
}

/******************************************************************************
 Descriptor:    It's used to set Tod new time.
 Input Args:    pgponTodCfg: ToD parameter structure.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_tod_new_time(struct XMCS_GponTodCfg_S *pgponTodCfg)
{
    struct XMCS_GponTodCfg_S gponTodCfg;
        
    FUNC_TRACE_ENTER;

    memset(&gponTodCfg, 0, sizeof(gponTodCfg)) ;
    if(gponmgr_lib_get_tod_switch_time(&gponTodCfg) == ECONET_E_FAIL) {
        return ECONET_E_FAIL;
    }
    pgponTodCfg->superframe = gponTodCfg.superframe;

    return gponmgr_lib_ioctl(GPON_IOS_TOD_CFG, (void *)pgponTodCfg);
}


/******************************************************************************
 Descriptor:    It's used to set ONU activation to1 timer.
 Input Args:    pActTimer: XMCS_GponActTimer_S parameter structure.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_act_timer_to1(uint to1Timer)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOS_ACT_TIMER_TO1, (void *)to1Timer);
}

/******************************************************************************
 Descriptor:    It's used to set ONU activation to2 timer.
 Input Args:    pActTimer: XMCS_GponActTimer_S parameter structure.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_act_timer_to2(uint to2Timer)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOS_ACT_TIMER_TO2, (void *)to2Timer);
}

/******************************************************************************
 Descriptor:	It's used to set idle gem.
 Input Args:	idle_gem: used to set idle gem.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_idle_gem(ushort idle_gem)
{
	FUNC_TRACE_ENTER;


	                                                    /* uint cast is used to size alignment */
	return gponmgr_lib_ioctl(GPON_IOS_IDLE_GEM, (void *)((uint)idle_gem));
}

/******************************************************************************
 Descriptor:    It's used to set dba block size.
 Input Args:    blockSize: used to set dba block size.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dba_block_size(ushort blockSize)
{
    FUNC_TRACE_ENTER;

                                                              /* uint cast is used to size alignment */
    return gponmgr_lib_ioctl(GPON_IOS_DBA_BLOCK_SIZE, (void *)((uint)blockSize));
}

/******************************************************************************
 Descriptor:    It's used to set the GPON Dying Gasp mode.
 Input Args:    mode: used to set dying gasp mode.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dying_gasp_mode(GPON_SW_HW_SELECT_T mode)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOS_DYING_GASP_MODE, (void *)mode);
}

/******************************************************************************
 Descriptor:    It's used to set the GPON Dying Gasp number.
 Input Args:    num: used to set dying gasp number.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dying_gasp_num(uint num)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOS_DYING_GASP_NUM, (void *)num);
}

/******************************************************************************
 Descriptor:    It's used to set software upstream traffic enable or disable.
 Input Args:    pup_traffic: used to set upstream traffic.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_up_traffic(GPON_DEV_UP_TRAFFIC_T *pup_traffic)
{
    FUNC_TRACE_ENTER;

    if((pup_traffic->omci != XPON_DISABLE) && (pup_traffic->omci != XPON_ENABLE)) {
        return ECONET_E_FAIL;
    }

    if((pup_traffic->data != XPON_DISABLE) && (pup_traffic->data != XPON_ENABLE)) {
        return ECONET_E_FAIL;
    }

    return gponmgr_lib_ioctl(GPON_IOS_UP_TRAFFIC, (void *)pup_traffic);
}

/******************************************************************************
 Descriptor:    It's used to set sniffer mode.
 Input Args:    sniffer: used to set sniffer mode.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_sniffer_gtc(GPON_DEV_SNIFFER_MODE_T *sniffer)
{
    int ret = ECONET_E_FAIL;
    
    FUNC_TRACE_ENTER;

#ifdef TCSUPPORT_CPU_EN7521
    sniffer->packet_padding =   XPON_ENABLE;
    /* Set tx */
    sniffer->tx_da =            0x0001;
    sniffer->tx_sa =            0x0002;
    sniffer->tx_ethertype =     0x88b6;
    sniffer->tx_vid =           0x0001;
    sniffer->tx_tpid=           sniffer->lan_port;
    /* Set rx */
    sniffer->rx_da =            0x0011;
    sniffer->rx_sa =            0x0012;
    sniffer->rx_ethertype =     0x88b6;
    sniffer->rx_vid =           0x0001;
    sniffer->rx_tpid=           sniffer->lan_port;
    ret = gponmgr_lib_ioctl(GPON_IOS_SNIFFER_GTC, (void *)sniffer);
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to set enable/disable filter extented burst length PLOAM at O5 state.
 Input Args:    enable: used to enable/disable filter.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_ext_bst_len_ploamd_filter(XPON_Mode_t enable)
{
    int ret = ECONET_E_FAIL;
    
    FUNC_TRACE_ENTER;

#ifdef TCSUPPORT_CPU_EN7521
    ret = gponmgr_lib_ioctl(GPON_IOS_EXT_BST_LEN_PLOAMD_FILTER_IN_O5, (void *)enable);
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to set enable/disable filter Upstream Overhead PLOAM at O5 state.
 Input Args:    enable: used to enable/disable filter.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_up_overhead_ploamd_filter(XPON_Mode_t enable)
{
    int ret = ECONET_E_FAIL;
    
    FUNC_TRACE_ENTER;

#ifdef TCSUPPORT_CPU_EN7521
    ret = gponmgr_lib_ioctl(GPON_IOS_UP_OVERHEAD_PLOAMD_FILTER_IN_O5, (void *)enable);
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to set enable/disable tx data length aligned multiple of 4.
 Input Args:    enable: used to enable/disable aligned multiple of 4.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_tx_4bytes_align(XPON_Mode_t enable)
{
    int ret = ECONET_E_FAIL;
    
    FUNC_TRACE_ENTER;

#ifdef TCSUPPORT_CPU_EN7521
    ret = gponmgr_lib_ioctl(GPON_IOS_TX_4BYTES_ALIGN, (void *)enable);
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to enable/disable DBA backdoor.
 Input Args:    enable: used to enable/disable DBA backdoor.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dba_backdoor(XPON_Mode_t enable)
{
    int ret = ECONET_E_FAIL;
    
    FUNC_TRACE_ENTER;

#ifdef TCSUPPORT_CPU_EN7521
    ret = gponmgr_lib_ioctl(GPON_IOS_DBA_BACKDOOR, (void *)enable);
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to set DBA backdoor total size.
 Input Args:    total_size: used to set DBA backdoor total size.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dba_backdoor_total(uint total_size)
{
    int ret = ECONET_E_FAIL;
    
    FUNC_TRACE_ENTER;

#ifdef TCSUPPORT_CPU_EN7521
    ret = gponmgr_lib_ioctl(GPON_IOS_DBA_BACKDOOR_TOTAL, (void *)total_size);
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to set DBA backdoor green size.
 Input Args:    total_size: used to set DBA backdoor green size.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dba_backdoor_green(uint green_size)
{
    int ret = ECONET_E_FAIL;

    FUNC_TRACE_ENTER;

#ifdef TCSUPPORT_CPU_EN7521
    ret = gponmgr_lib_ioctl(GPON_IOS_DBA_BACKDOOR_GREEN, (void *)green_size);
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to set DBA backdoor yellow size.
 Input Args:    total_size: used to set DBA backdoor yellow size.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dba_backdoor_yellow(uint yellow_size)
{
    int ret = ECONET_E_FAIL;

    FUNC_TRACE_ENTER;

#ifdef TCSUPPORT_CPU_EN7521
    ret = gponmgr_lib_ioctl(GPON_IOS_DBA_BACKDOOR_YELLOW, (void *)yellow_size);
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to enable/disable DBA slight modify.
 Input Args:    enable: used to enable/disable DBA slight modify.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dba_slight_modify(XPON_Mode_t enable)
{
    int ret = ECONET_E_FAIL;

    FUNC_TRACE_ENTER;

#ifdef TCSUPPORT_CPU_EN7521
    ret = gponmgr_lib_ioctl(GPON_IOS_DBA_SLIGHT_MODIFY, (void *)enable);
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to set DBA backdoor total size.
 Input Args:    total_size: used to set DBA slight modify total size.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dba_slight_modify_total(ushort total_size)
{
    int ret = ECONET_E_FAIL;

    FUNC_TRACE_ENTER;

#ifdef TCSUPPORT_CPU_EN7521
                                                                       /* uint cast is used to size alignment */
    ret = gponmgr_lib_ioctl(GPON_IOS_DBA_SLIGHT_MODIFY_TOTAL, (void *)((uint)total_size));
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to set DBA backdoor green size.
 Input Args:    total_size: used to set DBA slight modify green size.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dba_slight_modify_green(ushort green_size)
{
    int ret = ECONET_E_FAIL;

    FUNC_TRACE_ENTER;

#ifdef TCSUPPORT_CPU_EN7521
                                                                       /* uint cast is used to size alignment */
    ret = gponmgr_lib_ioctl(GPON_IOS_DBA_SLIGHT_MODIFY_GREEN, (void *)((uint)green_size));
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to set DBA backdoor yellow size.
 Input Args:    total_size: used to set DBA slight modify yellow size.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_dba_slight_modify_yellow(ushort yellow_size)
{
    int ret = ECONET_E_FAIL;

    FUNC_TRACE_ENTER;

#ifdef TCSUPPORT_CPU_EN7521
                                                                        /* uint cast is used to size alignment */
    ret = gponmgr_lib_ioctl(GPON_IOS_DBA_SLIGHT_MODIFY_YELLOW, (void *)((uint)yellow_size));
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to set o3 and o4 ploam control.
 Input Args:    total_size: used to set o3 and o4 ploam control.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_o3_o4_ploam_ctrl(GPON_SW_HW_SELECT_T sel)
{
    int ret = ECONET_E_FAIL;

    FUNC_TRACE_ENTER;

#ifdef TCSUPPORT_CPU_EN7521
    ret = gponmgr_lib_ioctl(GPON_IOS_O3_O4_PLOAM_CTRL, (void *)sel);
#endif

    return ret;
}

/******************************************************************************
 Descriptor:	It's used to set the high width of a 1pps pluse.
 Input Args:	width: used to set the high width of a 1pps pluse.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_1pps_h_w(uint width)
{
    FUNC_TRACE_ENTER;

	return gponmgr_lib_ioctl(GPON_IOS_1PPS_HIGH_WIDTH, (void *)width);
}

/******************************************************************************
 Descriptor:	It's used to set the send ploamu wait mode.
 Input Args:	mode: used to set the send ploamu wait mode.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_send_ploamu_wait_mode(GPON_DEV_SEND_PLOAMU_WAIT_MODE_T mode)
{
    FUNC_TRACE_ENTER;

	return gponmgr_lib_ioctl(GPON_IOS_SEND_PLOAMU_WAIT_MODE, (void *)mode);
}

/******************************************************************************
 Descriptor:    It's used to set AES key switch by sw.
 Input Args:    none: 
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_test_aes_key_switch_by_sw(void)
{
    int ret = ECONET_E_FAIL;

    FUNC_TRACE_ENTER;

#ifdef TCSUPPORT_CPU_EN7521
    ret = gponmgr_lib_ioctl(GPON_IOS_AES_KEY_SWITCH_BY_SW, NULL);
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to software reset GPON MAC exclude reg module.
 Input Args:    none: 
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_test_sw_reset(void)
{
    int ret = ECONET_E_FAIL;

    FUNC_TRACE_ENTER;

#ifdef TCSUPPORT_CPU_EN7521
    ret = gponmgr_lib_ioctl(GPON_IOS_SW_RESET, NULL);
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to software resync.
 Input Args:    none: 
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_test_sw_resync(void)
{
    int ret = ECONET_E_FAIL;

    FUNC_TRACE_ENTER;

#ifdef TCSUPPORT_CPU_EN7521
    ret = gponmgr_lib_ioctl(GPON_IOS_SW_RESYNC, NULL);
#endif

    return ret;
}


/******************************************************************************
 Descriptor:    It's used to send ploam sleep request.
 Input Args:    mode: used to set sleep_request PLOAMu mode.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_test_send_ploam_sleep_request(GPON_PLOAMu_SLEEP_MODE_t mode)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOS_TEST_SEND_PLOAM_SLEEP_REQUEST, (void *)mode);
}

/******************************************************************************
 Descriptor:    It's used to send ploam rei.
 Input Args:    counter: REI PLOAMu error counter.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_test_send_ploam_rei(uint counter)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOS_TEST_SEND_PLOAM_REI, (void *)counter);
}

/******************************************************************************
 Descriptor:    It's used to send ploam Dying Gasp.
 Input Args:    none.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_test_send_ploam_dying_gasp(void)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOS_TEST_SEND_PLOAM_DYING_GASP, NULL);
}

/******************************************************************************
 Descriptor:    It's used to Tod function. 
                 The ToD interrupt will occur after newTime seconds.
 Input Args:    pmode: used to store dying gasp mode.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_test_tod(uint newTime)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOS_TEST_TOD, (void *)newTime);
}

/******************************************************************************
 Descriptor:	It's used to test GPON hotplug. 
 Input Args:	enable: used to store enable/disable.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_test_hotplug(XPON_Mode_t enable)
{
	FUNC_TRACE_ENTER;

	return gponmgr_lib_ioctl(GPON_IOS_TEST_HOTPLUG, (void *)enable);
}


/******************************************************************************
 Descriptor:    It's used to get the GPON MAC debug level.
 Input Args:    pgponDbgLevel: used to store GPON MAC debug level.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_dbg_level(ushort *pgponDbgLevel)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(IO_IOG_DBG_LEVEL, pgponDbgLevel);
}

/******************************************************************************
 Descriptor:    It's used to get gpon detail information.
 Input Args:    pgponOnuInfo: used to store GPON MAC info.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_info(struct XMCS_GponOnuInfo_S *pgponOnuInfo)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOG_ONU_INFO, pgponOnuInfo);
}

/******************************************************************************
 Descriptor:	It's used to get GPON state.
 Input Args:	None.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_GPON_state(unsigned char *state)
{
	struct XMCS_GponOnuInfo_S pgponOnuInfo;
	
	FUNC_TRACE_ENTER;

	memset(&pgponOnuInfo, 0, sizeof(struct XMCS_GponOnuInfo_S));
	if(gponmgr_lib_get_info(&pgponOnuInfo) == ECONET_E_FAIL) {
		return ECONET_E_FAIL;
	}

	*state = pgponOnuInfo.state;

	return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to check GPON state is O1 or not.
 Input Args:	None.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_isO1(void)
{
	unsigned char state;
	
	FUNC_TRACE_ENTER;

	if(gponmgr_lib_GPON_state(&state) == ECONET_E_FAIL) {
		return ECONET_E_FAIL;
	}

	if(state == GPON_STATE_O1) {
		return ECONET_E_SUCCESS;
	} else {
		return ECONET_E_FAIL;
	}
}

/******************************************************************************
 Descriptor:	It's used to check GPON state is O5 or not.
 Input Args:	None.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_isO5(void)
{
	unsigned char state;
	
	FUNC_TRACE_ENTER;

	if(gponmgr_lib_GPON_state(&state) == ECONET_E_FAIL) {
		return ECONET_E_FAIL;
	}

	if(state == GPON_STATE_O5) {
		return ECONET_E_SUCCESS;
	} else {
		return ECONET_E_FAIL;
	}
}


/******************************************************************************
 Descriptor:    It's used to get the GPON GEM port information.
 Input Args:    pgponGemInfo: used to store GPON GEM port information.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_gemport(struct XMCS_GemPortInfo_S *pgponGemInfo)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(IF_IOG_GEMPORT_INFO, pgponGemInfo);
}

/******************************************************************************
 Descriptor:    It's used to get the GPON GEM counter information.
 Input Args:    pgponGemCounter: used to store GPON GEM counter information.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_gemcounter(struct XMCS_GponGemCounter_S *pgponGemCounter)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOG_GEM_COUNTER, pgponGemCounter);
}

/******************************************************************************
 Descriptor:    It's used to get the ToD switch time.
 Input Args:    pgponTodCfg: used to store ToD.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_tod_switch_time(struct XMCS_GponTodCfg_S *pgponTodCfg)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOG_TOD_SWITCH_TIME, pgponTodCfg);
}

/******************************************************************************
 Descriptor:    It's used to get the current ToD.
 Input Args:    pgponTodCfg: used to store ToD.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_current_tod(struct XMCS_GponTodCfg_S *pgponTodCfg)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOG_CURRENT_TOD, pgponTodCfg);
}

/******************************************************************************
 Descriptor:    It's used to get the new ToD.
 Input Args:    pgponTodCfg: used to store ToD.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_new_tod(struct XMCS_GponTodCfg_S *pgponTodCfg)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOG_NEW_TOD, pgponTodCfg);
}

/******************************************************************************
 Descriptor:    It's used to get the GPON TCONT information.
 Input Args:    pgponTcontInfo: used to store tcont information.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_tcont(struct XMCS_TcontInfo_S *pgponTcontInfo)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(IF_IOG_TCONT_INFO, pgponTcontInfo);
}

/******************************************************************************
 Descriptor:    It's used to get the EqD detail information.
 Input Args:    pgponEqdOffset: used to store EqD detail information.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_eqd_offset(struct XMCS_EqdOffset_S *pgponEqdOffset)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOG_EQD_OFFSET, pgponEqdOffset);
}

/******************************************************************************
 Descriptor:    It's used to get GPON super frame counter.
 Input Args:    pspf: used to store super frame counter.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_super_frame_counter(uint *pspf)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOG_SUPER_FRAME_COUNTER, pspf);
}

/******************************************************************************
 Descriptor:    It's used to get GPON MAC counter type.
 Input Args:    ptype: used to store GPON MAC counter type.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_counter_type(GPON_COUNTER_TYPE_t *ptype)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOG_COUNTER_TYPE, ptype);
}

/******************************************************************************
 Descriptor:    It's used to get the GPON response time.
 Input Args:    time: used to store GPON MAC response time.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_response_time(ushort *time)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOG_RESPONSE_TIME, time);
}

/******************************************************************************
 Descriptor:    It's used to get GPON MAC internal delay fine tune(BFB64208[15:8]).
 Input Args:    delay: used to store GPON MAC internal delay fine tune.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_internal_delay_fine_tune(unsigned char *delay)
{
    FUNC_TRACE_ENTER;

                                                                        /* uint cast is used to size alignment */
    return gponmgr_lib_ioctl(GPON_IOG_INTERNAL_DELAY_FINE_TUNE, (void *)((uint)delay));
}

/******************************************************************************
 Descriptor:    It's used to get the GPON MAC burst mode overhead supports more than 
                 128bytes status.
 Input Args:    ptype: used to store GPON_BURST_MODE_OVERHEAD_LEN_T.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_burst_mode_overhead(GPON_BURST_MODE_OVERHEAD_LEN_T *ptype)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOG_BURST_MODE_OVERHEAD_LEN, ptype);
}

/******************************************************************************
 Descriptor:    It's used to get the encryption key info.
 Input Args:    keyInfo: used to store GPON_DEV_ENCRYPT_KEY_INFO_T.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_key_info(GPON_DEV_ENCRYPT_KEY_INFO_T *keyInfo)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOG_ENCRYPT_KEY_INFO, keyInfo);
}

/******************************************************************************
 Descriptor:    It's used to get dba block size.
 Input Args:    pblockSize: used to store dba block size.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_dba_block_size(ushort *pblockSize)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOG_DBA_BLOCK_SIZE, pblockSize);
}

/******************************************************************************
 Descriptor:    It's used to get the GPON Dying Gasp mode.
 Input Args:    pmode: used to store dying gasp mode.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_dying_gasp_mode(GPON_SW_HW_SELECT_T *pmode)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOG_DYING_GASP_MODE, pmode);
}

/******************************************************************************
 Descriptor:    It's used to get the GPON Dying Gasp number.
 Input Args:    pmode: used to store dying gasp mode.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_dying_gasp_num(uint *pnum)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOG_DYING_GASP_NUM, pnum);
}

/******************************************************************************
 Descriptor:    It's used to get software upstream traffic enable or disable.
 Input Args:    pup_traffic: used to store upstream traffic status.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_up_traffic(GPON_DEV_UP_TRAFFIC_T *pup_traffic)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOG_UP_TRAFFIC, pup_traffic);
}

/******************************************************************************
 Descriptor:    It's used to get sniffer mode.
 Input Args:    sniffer: used to store sniffer mode.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_sniffer_gtc(GPON_DEV_SNIFFER_MODE_T *sniffer)
{
    int ret = ECONET_E_FAIL;
    
    FUNC_TRACE_ENTER;

#ifdef TCSUPPORT_CPU_EN7521
    ret = gponmgr_lib_ioctl(GPON_IOG_SNIFFER_GTC, (void *)sniffer);
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to show GPON control and status registers.
 Input Args:    none.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_csr(void)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOG_CSR, NULL);
}

/******************************************************************************
 Descriptor:    It's used to show GPON hardware gemport table information.
 Input Args:    none.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_gem_info(void)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOG_GEM_TABLE_INFO, NULL);
}

/******************************************************************************
 Descriptor:    It's used to show GPON hardware tcont table information.
 Input Args:    none.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_tcont_info(void)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(GPON_IOG_TCONT_TABLE_INFO, NULL);
}

/******************************************************************************
 Descriptor:	It's used to get idle gem threshold.
 Input Args:	idle_gem: used to store idle gem.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_idle_gem(ushort *idle_gem)
{
	FUNC_TRACE_ENTER;

	return gponmgr_lib_ioctl(GPON_IOG_IDLE_GEM, (void *)((uint)idle_gem));
}

/******************************************************************************
 Descriptor:    It's used to get status of filter extented burst length PLOAM at O5 state..
 Input Args:    enable: used to store enable status.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_ext_bst_len_ploamd_filter(XPON_Mode_t *enable)
{
    int ret = ECONET_E_FAIL;

    FUNC_TRACE_ENTER;
    
#ifdef TCSUPPORT_CPU_EN7521
    return gponmgr_lib_ioctl(GPON_IOG_EXT_BST_LEN_PLOAMD_FILTER_IN_O5, (void *)enable);
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to get status of filter Upstream Overhead PLOAM at O5 state.
 Input Args:    enable: used to store enable status.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_up_overhead_ploamd_filter(XPON_Mode_t *enable)
{
    int ret = ECONET_E_FAIL;

    FUNC_TRACE_ENTER;
    
#ifdef TCSUPPORT_CPU_EN7521
    return gponmgr_lib_ioctl(GPON_IOG_UP_OVERHEAD_PLOAMD_FILTER_IN_O5, (void *)enable);
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to get status of tx data length aligned multiple of 4.
 Input Args:    enable: used to store enable status.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_tx_4bytes_align(XPON_Mode_t *enable)
{
    int ret = ECONET_E_FAIL;

    FUNC_TRACE_ENTER;
    
#ifdef TCSUPPORT_CPU_EN7521
    return gponmgr_lib_ioctl(GPON_IOG_TX_4BYTES_ALIGN, (void *)enable);
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to get DBA backdoor configure.
 Input Args:    dba_backdoor: used to store DBA backdoor configure.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_dba_backdoor(GPON_DEV_DBA_BACKDOOR_T *dba_backdoor)
{
    int ret = ECONET_E_FAIL;

    FUNC_TRACE_ENTER;
    
#ifdef TCSUPPORT_CPU_EN7521
    return gponmgr_lib_ioctl(GPON_IOG_DBA_BACKDOOR, (void *)dba_backdoor);
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to get DBA slight modify configure.
 Input Args:    dba_backdoor: used to store DBA slight modify configure.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_dba_slight_modify(GPON_DEV_SLIGHT_MODIFY_T *dba_slight_modify)
{
    int ret = ECONET_E_FAIL;

    FUNC_TRACE_ENTER;
    
#ifdef TCSUPPORT_CPU_EN7521
    return gponmgr_lib_ioctl(GPON_IOG_DBA_SLIGHT_MODIFY, (void *)dba_slight_modify);
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to get o3 and o4 ploam control status.
 Input Args:    sel: used to store o3 and o4 ploam control status.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_o3_o4_ploam_ctrl(GPON_SW_HW_SELECT_T *sel)
{
    int ret = ECONET_E_FAIL;

    FUNC_TRACE_ENTER;
    
#ifdef TCSUPPORT_CPU_EN7521
    return gponmgr_lib_ioctl(GPON_IOG_O3_O4_PLOAM_CTRL, (void *)sel);
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to get tx sync status.
 Input Args:    sel: used to store tx sync status.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_tx_sync(unsigned char *tx_sync)
{
    int ret = ECONET_E_FAIL;

    FUNC_TRACE_ENTER;
    
#ifdef TCSUPPORT_CPU_EN7521
    return gponmgr_lib_ioctl(GPON_IOG_TX_SYNC, (void *)tx_sync);
#endif

    return ret;
}

/******************************************************************************
 Descriptor:    It's used to get interrupt mask.
 Input Args:    mask: used to store tx interrupt mask.
 Ret Value: success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_int_mask(uint *mask)
{
    FUNC_TRACE_ENTER;
    
    return gponmgr_lib_ioctl(GPON_IOG_INT_MASK, (void *)mask);
}

static inline unsigned int get_omci_max_burst()
{
    return MAX_OMCI_PKT_LEN * MAX_OMCI_SOFTIMAGE_UPDATE_WINDOW;
}

/******************************************************************************
 Descriptor:	It's used to get the high width of a 1pps pluse.
 Input Args:	width: used to store the high width of a 1pps pluse.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_1pps_h_w(uint *width)
{
    FUNC_TRACE_ENTER;

	return gponmgr_lib_ioctl(GPON_IOG_1PPS_HIGH_WIDTH, (void *)width);
}

/******************************************************************************
 Descriptor:	It's used to get the send ploamu wait mode.
 Input Args:	mode: used to store the send ploamu wait mode.
 Ret Value:	success: ECONET_E_SUCCESS
                       fail: ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_send_ploamu_wait_mode(GPON_DEV_SEND_PLOAMU_WAIT_MODE_T *mode)
{
    FUNC_TRACE_ENTER;

	return gponmgr_lib_ioctl(GPON_IOG_SEND_PLOAMU_WAIT_MODE, (void *)mode);
}

/******************************************************************************
 Descriptor:    init socket used to receive/send omci pkt
 Input Args:    pointer to socket fd,for return inited socket fd.
                if fail, *socket_f == -1
 Ret Value: success: if success, return ECONET_E_SUCCESS
                     if fali, return ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_init_omci_socket(int * socket_f)
{   
    int omci_sock_f = -1;
    int loop_time = 0;
    struct sockaddr_ll omci_socket_addr={0};
    struct ifreq ifstruct= {0}; 
    unsigned int rcvbuf = get_omci_max_burst();

    FUNC_TRACE_ENTER;
    PRINT_ARGS("socket_f:%x\n", socket_f);

    if(NULL == socket_f){
        RETURN_FAIL;
    }

    omci_sock_f = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(-1 == omci_sock_f) 
    { 
        RETURN_FAIL_MSG("CREATE SOCKET ERROR\n");
    }

    if(0!=setsockopt(omci_sock_f, SOL_SOCKET, SO_RCVBUF, &rcvbuf, sizeof(rcvbuf) )){
        LIB_GPON_WARN("SET RECV BUFFER FAILED\n");
    }
    
    omci_socket_addr.sll_family   = AF_PACKET; 
    omci_socket_addr.sll_protocol = htons(ETH_P_ALL);
    strcpy(ifstruct.ifr_name, OMCI_INTERFACE);
    
    while(loop_time<10)
    {
        if (0!= ioctl(omci_sock_f, SIOCGIFINDEX, &ifstruct))
        {
            loop_time++;
            sleep(2);
        }
        else
        {
            break;
        }
    }
    if(10 == loop_time){
        LIB_GPON_ERROR("ioctl get ifstruct error\n");
        goto fail;
    }
    omci_socket_addr.sll_ifindex= ifstruct.ifr_ifindex;
    
    if(bind(omci_sock_f, (struct sockadadr* )&omci_socket_addr, sizeof(omci_socket_addr)) == -1)
    { 
        LIB_GPON_ERROR("BIND ADDR ERROR\n");
        goto fail;
    }

    *socket_f = omci_sock_f;
    LIB_GPON_DEBUG("omci socket descriptor:%d\n", omci_sock_f);
    RETURN_SUCCESS;

fail:
    close(omci_sock_f);
    RETURN_FAIL; 
}


/******************************************************************************
 Descriptor:    init socket used to receive/send omci pkt
 Input Args:    socket:  omci socket
                msg:     omci msg
                msg_len: omci msg len
 Ret Value: success: if success, return ECONET_E_SUCCESS
                     if fali, return ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_omci_receive_msg_block(int sock_f, unsigned char * msg,
                                unsigned int *msg_len)
{
    int pktLen = 0;
    FUNC_TRACE_ENTER;
    
    PRINT_ARGS("sock_f:%d msg:%x, msg_len:%x\n",sock_f, msg, msg_len);

    if((sock_f < 0) || (NULL == msg) || (NULL== msg_len) ){
        RETURN_FAIL;
    }

    if((NULL == _omciRxThread ) && (ECONET_E_SUCCESS != init_omci_rx_thread(sock_f) ) ){
        RETURN_FAIL_MSG("init_omci_rx_thread failed!\n");
    }

    if(0 != sem_wait(&_omciRxBuf.sem) ){
        RETURN_FAIL_MSG("sem_wait failed!\n");
    }

    pktLen = _omciRxBuf.buf[_omciRxBuf.r_idx].len;
    if(pktLen > *msg_len){
        LIB_GPON_WARN("msg length bigger than rev buff, msg will be truncated\n");
        pktLen = *msg_len;
    }else{
        *msg_len = pktLen;
    }
    
    memcpy(msg, _omciRxBuf.buf[_omciRxBuf.r_idx].msg, pktLen);

    _omciRxBuf.r_idx = (_omciRxBuf.r_idx + 1) & 0xFF;
    RETURN_SUCCESS;
}

/******************************************************************************
 Descriptor:    init socket used to receive/send omci pkt
 Input Args:    socket:  omci socket
                msg:     omci msg
                msg_len: omci msg len
 Ret Value: success: if success, return ECONET_E_SUCCESS
                     if fali, return ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_omci_receive_block(unsigned char * msg, unsigned int *msg_len)
{
    FUNC_TRACE_ENTER;

    PRINT_ARGS("msg:%x, msg_len:%x\n",msg, msg_len);
    
    if(-1 == _omci_socket){
        if(ECONET_E_SUCCESS!=gponmgr_lib_init_omci_socket(&_omci_socket)){
            RETURN_FAIL_MSG("gponmgr_lib_init_omci_socket failed\n");
        }
    }

    if(ECONET_E_SUCCESS != gponmgr_lib_omci_receive_msg_block(_omci_socket, msg, msg_len) )
    {
        RETURN_FAIL_MSG("gponmgr_lib_omci_receive_msg_block failed\n");
    }

    RETURN_SUCCESS;
}

/******************************************************************************
 Descriptor:    init socket used to receive/send omci pkt
 Input Args:    socket:  omci socket
                msg:     omci msg
                msg_len: omci msg len
 Ret Value: success: if success, return ECONET_E_SUCCESS
                     if fali, return ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_omci_send_msg_block(int sock_f, unsigned char * msg,
                                unsigned int msg_len)
{
    int ret = -1;
    FUNC_TRACE_ENTER;

    PRINT_ARGS("sock_f:%d msg:%x, msg_len:%u\n",sock_f, msg, msg_len);

    if(sock_f < 0 || NULL == msg){
        RETURN_FAIL;
    }
    
    ret = send(sock_f, msg, msg_len, 0);

    LIB_GPON_DEBUG("sent len:%d\n", ret);
    if(ret == msg_len){
        RETURN_SUCCESS;
    }else if(ret>0){
        RETURN_FAIL_MSG("not all bytes is sent, input msg len:%d, sent len:%d\n", msg_len, ret);
    }else{
        RETURN_FAIL_MSG("omci send packet error\n");
    }
}

/******************************************************************************
 Descriptor:    init socket used to receive/send omci pkt
 Input Args:    socket:  omci socket
                msg:     omci msg
                msg_len: omci msg len
 Ret Value: success: if success, return ECONET_E_SUCCESS
                     if fali, return ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_omci_send_block(unsigned char * msg, unsigned int msg_len)
{
    FUNC_TRACE_ENTER;

    PRINT_ARGS("msg:%x, msg_len:%u", msg, msg_len);

    if(-1==_omci_socket){
        RETURN_FAIL_MSG("gponmgr_lib_init_omci_socket failed\n");
    }
    
    if(ECONET_E_SUCCESS != gponmgr_lib_omci_send_msg_block(_omci_socket, msg, msg_len) )
    {
        RETURN_FAIL_MSG("gponmgr_lib_omci_send_msg_block failed\n");
    }

    RETURN_SUCCESS;
}

/******************************************************************************
 Descriptor:    printf g_tcont_to_allocid_map[] to show tcont mapping to allocid
 Input Args:    void
 Ret Value: success: if success, return ECONET_E_SUCCESS
                     
******************************************************************************/
int gponmgr_lib_get_tcontMapInfo(void){
    int i = 0;
    unsigned short allocid = 0;
    FUNC_TRACE_ENTER;
    
    LIB_GPON_DEBUG("==================================================\n");
    for(i = 0; i < CONFIG_GPON_MAX_TCONT; i++){
        if(ECONET_E_SUCCESS!=get_allocid((unsigned short)i,&allocid)){
            LIB_GPON_ERROR("GET ALLOCID FAILED\n");
            return ECONET_E_FAIL;
        }
        LIB_GPON_DEBUG("tcont:%u,allocid:%u\n", i, allocid);

    }
    LIB_GPON_DEBUG("==================================================\n");

    return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:    bind tcont to allocid
 Input Args:    tcont  : omci layer tcont
                allocid: alloc id
 Ret Value: success: if success, return 0
                     if fali, return non-zero error code
******************************************************************************/
int gponmgr_lib_bind_tcont_to_allocid(unsigned short tcont, unsigned short allocid)
{
    FUNC_TRACE_ENTER;
    
    LIB_GPON_DEBUG("input para allocid:%u, tcont:%u\n",allocid, tcont);


    if(tcont >= CONFIG_GPON_MAX_TCONT){
        LIB_GPON_ERROR("wrong tcont id\n");
        return ECONET_E_FAIL;
    }
    if(allocid >= GPON_MAX_ALLOC_ID){
        LIB_GPON_ERROR("wrong allocid\n");
        return ECONET_E_FAIL;
     }
    
    if(ECONET_E_SUCCESS!=set_allocid(tcont,allocid)){
        return ECONET_E_FAIL;
    }
     
    return ECONET_E_SUCCESS;
}


/******************************************************************************
 Descriptor:    unbind tcont to allocid
 Input Args:    tcont: omci layer tcont
 Ret Value: success: if success, return 0
                     if fali, return non-zero error code
******************************************************************************/
int gponmgr_lib_unbind_tcont_to_allocid(unsigned short tcont)
{
    FUNC_TRACE_ENTER;
    LIB_GPON_DEBUG("input para, tcont:%hu\n",tcont);

    if(tcont >= CONFIG_GPON_MAX_TCONT){
        LIB_GPON_ERROR("wrong tcont id\n");
        return ECONET_E_FAIL;
    }
    
     if(ECONET_E_SUCCESS!=set_allocid(tcont,GPON_MAX_ALLOC_ID)){
        return ECONET_E_FAIL;
     }

    return ECONET_E_SUCCESS;
}


/******************************************************************************
 Descriptor:    delete tcont
 Input Args:    tcont: omci layer tcont
 Ret Value: success: if success, return 0
                     if fali, return non-zero error code
 Notice: omci layer probably do not need to delete MAC layer alloc id when
         unbinding omci tcont, so this API should only be used under very special
         occasions
******************************************************************************/
int gponmgr_lib_delete_tcont(unsigned short tcont)
{
    unsigned short allocid = 0;

    FUNC_TRACE_ENTER;

    PRINT_ARGS("tcont:%hu\n",tcont);
    
    if(tcont >= CONFIG_GPON_MAX_TCONT){
        LIB_GPON_ERROR("wrong tcont to del\n");
        return ECONET_E_FAIL;
    }

    if(ECONET_E_SUCCESS!=get_allocid(tcont,&allocid)){
        LIB_GPON_ERROR("GET ALLOCID FAILED\n"); 
        return ECONET_E_FAIL;
    }
    if(ECONET_E_SUCCESS!=set_allocid(tcont,GPON_MAX_ALLOC_ID)){
        LIB_GPON_ERROR("SET ALLOCID FAILED\n");
        return ECONET_E_FAIL;
    }

    LIB_GPON_DEBUG("alloc id to be deleted:%hu\n",allocid);
    
    if (ECONET_E_SUCCESS != gponmgr_lib_ioctl(IF_IOS_TCONT_INFO_REMOVE,(void*)allocid) ){
        LIB_GPON_WARN("del tcont fail, but maybe normal.\n");
    }
    return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:    create gemport
 Input Args:    portid: gem port id
                type  : gem port type(multicast or unicast)
                tcont : omci layer tcont
 Ret Value: success: if success, return 0
                     if fali, return non-zero error code
******************************************************************************/
int gponmgr_lib_config_gemport(unsigned short portid, GPON_GemType_t type, unsigned short tcont)
{
    struct XMCS_GemPortCreate_S gponGemCreate={0};
    struct XMCS_GponOnuInfo_S OnuInfo={0};

    FUNC_TRACE_ENTER;

    PRINT_ARGS("portid:%hu, type:%d, tcont:%hu\n", portid, type, tcont);

    if(INVALID_TCONT != tcont && tcont >= CONFIG_GPON_MAX_TCONT ){
        LIB_GPON_ERROR("wrong tcont to del\n");
        return ECONET_E_FAIL;
    }

    if(portid >= GPON_MAX_GEM_ID ){
        LIB_GPON_ERROR("wrong gem port to del\n");
        return ECONET_E_FAIL;
    }

    gponGemCreate.gemPortId = portid;

    switch(type)
    {
        case GPON_MULTICAST_GEM :
            gponGemCreate.gemType  = GPON_MULTICAST_GEM;
            break;

        case   GPON_UNICAST_GEM :
        default                 :
            gponGemCreate.gemType = GPON_UNICAST_GEM;
            if(INVALID_TCONT == tcont){
               if(ECONET_E_SUCCESS!=gponmgr_lib_get_info(&OnuInfo)){
                    return ECONET_E_FAIL;
                }
                gponGemCreate.allocId = OnuInfo.onuId;
            }else{             
                if(ECONET_E_SUCCESS!=get_allocid(tcont,&gponGemCreate.allocId)){
                    LIB_GPON_ERROR("GET ALLOCID FAILED\n"); 
                    return ECONET_E_FAIL;
                }
            }
            break;
    }
    
    LIB_GPON_DEBUG("gemport id=%u, tcont=%u, alloc id=%u \n",gponGemCreate.gemPortId, tcont, gponGemCreate.allocId);
    return gponmgr_lib_ioctl(IF_IOS_GEMPORT_CREATE, &gponGemCreate);    
}

/******************************************************************************
 Descriptor:    delete gemport
 Input Args:    portid: gem port id
 Ret Value: success: if success, return 0
                     if fali, return non-zero error code
******************************************************************************/
int gponmgr_lib_delete_gemport(unsigned short portid)
{
    FUNC_TRACE_ENTER;
    LIB_GPON_DEBUG("input para gemport id:%u\n",portid);
    if(portid >= GPON_MAX_GEM_ID ){
        LIB_GPON_ERROR("wrong gem port to del\n");
        return ECONET_E_FAIL;
    }
    if (ECONET_E_SUCCESS != gponmgr_lib_ioctl(IF_IOS_GEMPORT_REMOVE,(void*)portid) ){
        LIB_GPON_WARN("del gemport fail, but maybe normal.\n");
    }
    return ECONET_E_SUCCESS;
}


/******************************************************************************
 Descriptor:    get gemport statistic info
 Input Args:    portid: gem port id, statistic for store return value
 Ret Value: success: if success, return 0
                     if fali, return non-zero error code
******************************************************************************/
int gponmgr_lib_get_gemport_statistic(GEM_STATISTIC_t* statistic)
{
    FUNC_TRACE_ENTER;
    if(NULL==statistic){
        LIB_GPON_DEBUG("INPUT GEM PARAMS IS NULL\n");
        return ECONET_E_FAIL;
    }
    
    if(ECONET_E_SUCCESS!=gponmgr_lib_get_gemcounter(statistic)){
        LIB_GPON_ERROR("GET GEMCOUNTER ERR!\n");
        return ECONET_E_FAIL;
    }

    LIB_GPON_DEBUG("\n gemPortId:%u \n rxGemFrameL:%u ||rxGemPayloadL:%u  \
                    \n txGemFrameL:%u || txGemPayloadL:%u",               \
                        statistic->gemPortId,statistic->rxGemFrameL,     \
                        statistic->rxGemPayloadL,statistic->txGemFrameL, \
                        statistic->txGemPayloadL);
       
    return ECONET_E_SUCCESS;


}

/****************************************************************************** 
Descriptor:	get wan statistic info Input Args:	portid: statistic for storereturn value 
Ret Value:	success: if success, return 0 if fali, return non-zero error code
******************************************************************************/
int gponmgr_lib_get_wan_statistic(WAN_STATISTIC_t * pWanCntStats)
{ 
    FUNC_TRACE_ENTER;	
    if(NULL == pWanCntStats){
        LIB_GPON_DEBUG("INPUT WAN PARAMS IS NULL\n");
        return ECONET_E_FAIL;   
    } 
    gponmgr_lib_ioctl(IO_IOG_WAN_CNT_STATS, pWanCntStats); 
    LIB_GPON_DEBUG("\n wan statistics: \n txFrameCnt %u txFrameCnt:%u\n",\
                   pWanCntStats->rxFrameCnt, pWanCntStats->txFrameCnt);
    return ECONET_E_SUCCESS;
}

/******************************************************************************
 Descriptor:    select wanted event to be polled by gponmgr_lib_get_driver_event
                function
 Input Args:    filter_type: black/white list
                filter_list: filter list
                list_len   : filter list length
                
 Ret Value: success: if success, return 0
                     if fali, return non-zero error code
******************************************************************************/

int gponmgr_lib_set_driver_event_filter(Event_Filter_Type filter_type,
                                        unsigned short * filter_list,
                                        unsigned short  list_len)
{
    FUNC_TRACE_ENTER;
    if(filter_type == WHITE_LIST_FILTER){
        _event_poll_policy.filter_type = WHITE_LIST_FILTER;
        memcpy(_event_poll_policy.list.white_list, filter_list, list_len*sizeof(unsigned short));
        _event_poll_policy.list_length = list_len;
        return ECONET_E_SUCCESS;
    }
    else if(filter_type == BLACK_LIST_FILTER){
        _event_poll_policy.filter_type = BLACK_LIST_FILTER;
        memcpy(_event_poll_policy.list.black_list, filter_list, list_len*sizeof(unsigned short));
        _event_poll_policy.list_length = list_len;
        return ECONET_E_SUCCESS;
    }
    return ECONET_E_FAIL;
}

/******************************************************************************
 Descriptor:    configure polled event value mapped to another
 Input Args:    src_event: event value to be mapped
                dst_event: event value to be mapped to
                
 Ret Value: success: if success, return 0
                     if fali, return non-zero error code
******************************************************************************/
int gponmgr_lib_add_driver_event_mapper(XMCS_GponEventId_t src_event, 
                                        unsigned int dst_event)
{
    int i;
    FUNC_TRACE_ENTER;
    for(i = 0; i<XMCS_EVENT_GPON_MAX; i++){
        if(_event_poll_policy.mapper_src[i]==0){
            _event_poll_policy.mapper_src[i] = src_event;
            _event_poll_policy.mapper_dst[i] = dst_event;
            return ECONET_E_SUCCESS;
        }
    }
    return ECONET_E_FAIL;
}
/******************************************************************************
 Descriptor:    check event queue is Empty or not
 Input Args:    poninter to queue,define in xpon_driver_private_type.h
 Ret Value: success: if success, return 0
                     if fali, return non-zero error code
******************************************************************************/
static inline int IsEmpty(event_queue_t *Q){
    return Q->size == 0;
}
/******************************************************************************
 Descriptor:    check eventqueue is Full or not
 Input Args:    poninter to queue,define in xpon_driver_private_type.h
 Ret Value: success: if success, return 0
                     if fali, return non-zero error code
******************************************************************************/
static inline int IsFull(event_queue_t *Q){
    return Q->size == Q->capacity;
}
/******************************************************************************
 Descriptor:    make the front or rear to next buffer index
 Input Args:    poninter to queue,define in xpon_driver_private_type.h
 Ret Value: success: if success, return 0
                     if fali, return non-zero error code
******************************************************************************/
static inline int Succ(int index, event_queue_t *Q){
    if(++index == Q->capacity)
        index = 0;
    return index;
}
/******************************************************************************
 Descriptor:    Enqueue operation
 Input Args:    event struct def in xpon_driver_public_type.h, 
                poninter to queue,define in xpon_driver_private_type.h
 Ret Value: success: if success, return 0
                     if fali, return non-zero error code
******************************************************************************/
static int EnQueue(XPON_EVENT_t *event, event_queue_t *Q){
    if(IsFull(Q)){
        return -1;
    }
    else{
        Q->size++;
        Q->rear = Succ(Q->rear, Q);
        Q->event[Q->rear].type = event->type;
        Q->event[Q->rear].id = event->id;
        Q->event[Q->rear].value = event->value;
        return 0;
    }
    
}
/******************************************************************************
 Descriptor:    Dequeue operation
 Input Args:    event struct def in xpon_driver_public_type.h, 
                poninter to queue,define in xpon_driver_private_type.h
 Ret Value: success: if success, return 0
                     if fali, return non-zero error code
******************************************************************************/
static int DeQueue(XPON_EVENT_t *event, event_queue_t *Q){
    unsigned int ret = -1;
    if(IsEmpty(Q)){
         return ret;
    }
    else{
        Q->size--;
        event->type = Q->event[Q->front].type;
        event->id = Q->event[Q->front].id;
        event->value = Q->event[Q->front].value;
        Q->front = Succ(Q->front, Q);
        ret = 0;
    }
    return ret;
}
/******************************************************************************
Descriptor:    do event filter operation
Input Args:    event pointer for filter
Ret Value:      if pass return 0
                if drop return 1
                if no filter list return -1
*******************************************************************************/
int do_event_filter(XPON_EVENT_t *event)
{
    int j = 0;
    unsigned short list_len = _event_poll_policy.list_length;
    switch(_event_poll_policy.filter_type){
        /*filter black list*/
        case BLACK_LIST_FILTER: 
            while(j < list_len){
               if(_event_poll_policy.list.black_list[j++] == event->id){
                    break;
               }
            }
            if(j < list_len ){
                return 1;
            }
            return 0;
        /*filter white list*/
        case WHITE_LIST_FILTER: 
            while(j < list_len){
               if(_event_poll_policy.list.white_list[j++] == event->id){
                    break;
               }
            }
            if(j < list_len){
                return 0;
            }
            return 1;
        default:
            break;
    }
    return -1;
}
/******************************************************************************
 Descriptor:    poll driver event, it will block until some event is polled
 Input Args:    event pointer for get an driver event
 Ret Value: success: if success, return ECONET_E_SUCCESS
                     if fali, return ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_driver_event_block(XPON_EVENT_t *event)
{
    int i = 0;
    int filter_result= 0;

    struct XMCS_PonEventStatus_S ponEventStatus ;
    
    FUNC_TRACE_ENTER;
    PRINT_ARGS("event:%x\n", event);

    if(NULL == event){
        RETURN_FAIL;
    }

    while(!IsEmpty(&_event_queue) ){
        if(ECONET_E_SUCCESS != DeQueue(event,&_event_queue)){
            LIB_GPON_ERROR("DeQueue1 Error\n");
        }
        LIB_GPON_DEBUG("output1 event id:%u, value:%u\n", event->id, event->value);
        return ECONET_E_SUCCESS;
    }
        
    while(1){
        memset(&ponEventStatus, 0, sizeof(struct XMCS_PonEventStatus_S));
        if(ECONET_E_SUCCESS != gponmgr_lib_ioctl(FDET_IOG_WAITING_EVENT, &ponEventStatus)){
            continue;
        }

        for(i=0; i<ponEventStatus.items; i++){
            LIB_GPON_DEBUG("event items:%u\n",ponEventStatus.items);
            if(ECONET_E_SUCCESS != EnQueue(&ponEventStatus.event[i], &_event_queue)){
                LIB_GPON_ERROR("EnQueue Error\n");
            }
            LIB_GPON_DEBUG("EnQueue event id:%u, value:%u \n", ponEventStatus.event[i].id, ponEventStatus.event[i].value);
        }
        break;
    }
    if(ECONET_E_SUCCESS != DeQueue(event,&_event_queue)){
        LIB_GPON_ERROR("DeQueue2 Error\n");
    }
    LIB_GPON_DEBUG("output2 event id:%u, value:%u\n", event->id, event->value);
    RETURN_SUCCESS;
}

/******************************************************************************
 Descriptor:    make GPON activate FSM to change to O7
 Input Args:    NONE
 Ret Value: success: if success, return ECONET_E_SUCCESS
                     if fali, return ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_emergency_state()
{
    FUNC_TRACE_ENTER;
    return gponmgr_lib_ioctl(GPON_IOS_EMERGENCY_STATE, (void*)XPON_ENABLE);
}

/******************************************************************************
 Descriptor:    delete all data service in driver, leave only ONU ID and OMCC,
                stay on O5
 Input Args:    NONE
 Ret Value: success: if success, return 0
                     if fali, return non-zero error code
******************************************************************************/
int gponmgr_lib_reset_driver_data_service()
{
    FUNC_TRACE_ENTER;
    return gponmgr_lib_ioctl(GPON_IOS_RESET_SERVICE, 0);
}

/******************************************************************************
 Descriptor:    set xpon link mode 
 Input Args:    enum  XMCSIF_WanDetectionMode_t (gpon/epon/auto mode) 
 Ret Value:     success:return ECONET_E_SUCCESS
                    fali: return ECONET_E_FAIL
******************************************************************************/
static int gponmgr_lib_set_gpon_link_detection(XMCSIF_WanDetectionMode_t mode)
{
    FUNC_TRACE_ENTER;

    return gponmgr_lib_ioctl(IO_IOS_WAN_DETECTION_MODE,(void*)mode);
}

/******************************************************************************
 Descriptor:    interface for enable or disable gpon.
 Input Args:    enum XPON_Mode_t (XPON_DISABLE/XPON_ENABLE/XPON_POWER_DOWN)
 Ret Value:     success:return ECONET_E_SUCCESS
                    fali: return ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_pon_link_state(XPON_Mode_t state)
{
    FUNC_TRACE_ENTER;
    LIB_GPON_DEBUG("input param is %d\n",state);

    if(ECONET_E_SUCCESS != gponmgr_lib_set_gpon_link_detection(XMCS_IF_WAN_DETECT_MODE_GPON) ){
        RETURN_FAIL_MSG("SET GPON MODE FAILED\n");
    }


    if(ECONET_E_SUCCESS != gponmgr_lib_ioctl(IO_IOS_WAN_LINK_START,(void*)state) ){
        RETURN_FAIL_MSG("wan start failed!\n");
    }

    RETURN_SUCCESS;
}

/******************************************************************************
 Descriptor:    get phy trans params
 Input Args:    pointer to XMCS_PhyTransParams_S
                    this struct contain information about bosa
 Ret Value:     success:return ECONET_E_SUCCESS
                    fali: return ECONET_E_FAIL
******************************************************************************/

int gponmgr_lib_get_phy_trans_params(struct XMCS_PhyTransParams_S *pPhyTransParams)
{
    struct XMCS_PhyTransParams_S local_PhyTransParams;
    FUNC_TRACE_ENTER;

    if(NULL==pPhyTransParams){
        RETURN_FAIL_MSG("INPUT PARAM IS NULL\n");
    }
        
    if(ECONET_E_FAIL==gponmgr_lib_ioctl(PHY_IOG_TRANSCEIVER_PARAMS, (void *)&local_PhyTransParams)){
        RETURN_FAIL_MSG("get transceiver params ioctl error\n");
    }
    memcpy(pPhyTransParams, &local_PhyTransParams, sizeof(struct XMCS_PhyTransParams_S));
    RETURN_SUCCESS;
}

/******************************************************************************
 Descriptor:    set init report O1 flag  value,this flag userd to control welther to report O1 event
                    int the initial process
 Input Args:    flag value(0/1) 
 Ret Value:     success: return ECONET_E_SUCCESS
                     fali: return ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_set_InitRepO1_Flag(int value)
{
    Event_ctrlFlag_t event_flag;
    memset(&event_flag, 0, sizeof(Event_ctrlFlag_t));
    FUNC_TRACE_ENTER;

    PRINT_ARGS("input value is %d\n",value);

   
    pthread_mutex_lock(&_event_ctrl_data_t->event_ctrl_mutexlock);
    if(ECONET_E_SUCCESS!=gponmgr_lib_ioctl(FDET_IOG_EVENT_CTRL, (void*)&event_flag)){
        pthread_mutex_unlock(&_event_ctrl_data_t->event_ctrl_mutexlock);
        RETURN_FAIL_MSG("get event ctrl flag failed\n");      
    }
    
    event_flag.report_init_O1 = value;
    
    if(ECONET_E_SUCCESS != gponmgr_lib_ioctl(FDET_IOS_EVENT_CTRL, (void*)&event_flag)){
        pthread_mutex_unlock(&_event_ctrl_data_t->event_ctrl_mutexlock);
        RETURN_FAIL_MSG("set event ctrl flag failed\n");
    }
    pthread_mutex_unlock(&_event_ctrl_data_t->event_ctrl_mutexlock);

    RETURN_SUCCESS;
}

/******************************************************************************
 Descriptor:    get phy fec status
 Input Args:    pointer to XMCS_PhyTxRxFecStatus_S
                    this struct contain information about fec status
 Ret Value:     success:return ECONET_E_SUCCESS
                    fali: return ECONET_E_FAIL
******************************************************************************/
int gponmgr_lib_get_phy_fec_status(struct XMCS_PhyTxRxFecStatus_S * fecStatus)
{
	int ret = ECONET_E_SUCCESS;	
	
	if(NULL==fecStatus){
        RETURN_FAIL_MSG("INPUT PARAM IS NULL\n");
    }
	ret = gponmgr_lib_ioctl(PHY_IOG_TX_RX_FEC_STATUS, (unsigned long)fecStatus);
	if(ECONET_E_SUCCESS != ret)
	{
		RETURN_FAIL_MSG("get fec status ioctl error\n");
		RETURN_FAIL;
	}

	RETURN_SUCCESS;
}

