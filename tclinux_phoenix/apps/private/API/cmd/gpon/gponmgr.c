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
#include <ctype.h>
#if !defined __GLIBC__
#include <curses.h>
#endif
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <libapi_lib_utility.h>
#include <libapi_gponmgr_internal.h>

#include "gponmgr.h"
#include "gponmgr_get.h"
#include "gponmgr_set.h"

/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
************************************************************************/

/************************************************************************
*                  M A C R O S
************************************************************************/
#define MAX_CMD_LEN         128
#define MAX_NUM_ARG			(20)
#define MAX_ARG_LEN         10
/************************************************************************
*                  D A T A	 T Y P E S
************************************************************************/
typedef struct{
    pthread_t tid[4];
    char    size;
}thr_rec;
/************************************************************************
*                  E X T E R N A L	 D A T A   D E C L A R A T I O N S
************************************************************************/
extern const cmds_t gponmgr_get_command[];
extern const cmds_t gponmgr_set_command[];

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
************************************************************************/

/*********************** gponmgr_root_command **************************/
static int do_set(int argc, char* argv[], void *p);
static int do_get(int argc, char* argv[], void *p);
static int create_gemport(int argc, char* argv[], void *p);
static int del_gemport(int argc,char * argv[],void * p);
static int unbind_tcont(int argc,char * argv[],void * p);
static int bindTcont(int argc, char* argv[], void *p);
static int recive_send_omciPacket(int argc,char * argv [ ],void * p);
static int send_omciPacket(int argc,char * argv [ ],void * p);
static int get_event(int argc,char * argv [ ],void * p);
static int stop_thread(int argc,char * argv [ ],void * p);
static int get_tcont_allocid_map(int argc,char * argv[ ],void * p);
static int set_emergence_state(int argc,char * argv[ ],void * p);
/************************************************************************
*                  P U B L I C   D A T A
************************************************************************/
/* Raw OMCI Get Response pkt, ME: ONT-Data(2)*/
static const char msg_send[] = {
    0x00,0x10,0x29,0x0a,0x00,0x02,0x00,0x00,
    0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x28,
    0x00,0x00
};

/************************************************************************
*                  P R I V A T E   D A T A
************************************************************************/
static const cmds_t gponmgr_root_command[] = {
/*	name,		func, 			flags,	argcmin,	argc_errmsg */
	{"set",		do_set,			0,		1,			"Set gpon driver status"},
	{"get",		do_get,			0,		1,			"Get gpon driver status"},
    {"omciRxTx",   recive_send_omciPacket,     0,      0,      "gponmgr omciRxTx"},
    {"createGemport", create_gemport, 0,      1,          "gponmgr createGemport  gemportid"},
    {"delGemport", del_gemport,0,      1,          "gponmgr delGemport portid"},
    {"bindTcont",   bindTcont,  0,      1,          "gponmgr bindTcont tcont allocid"},
    {"unbindTcont",   unbind_tcont,  0,      1,          "gponmgr unbindTcont tcontid"},
    {"pollEvent",   get_event,  0,      0,         "gponmgr pollEvent"},
    {"showTcontMap",    get_tcont_allocid_map,  0,    0,    "gponmgr showTcontMap"},
    {"stop",   stop_thread,     0,      1,          "gponmgr stop threadID"},
    {"setO7",   set_emergence_state,     0,      0,          "setO7"},
	/* Cmd should be added before this line!  */
	{"help",	api_lib_help,	0,		0,			"Show gponmgr usage."},
	{NULL, NULL, 0, 0, NULL}
};

static thr_rec event_tid_rec = {
    .tid = {0},
    .size = 0
};

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
************************************************************************/

/******************************************************************************
 Descriptor:	It's used to set gpon driver status.
 Input Args:	see gponmgr_root_command.
 Ret Value:	    SUCCESS:0.FAIL:-1. 
******************************************************************************/
static int do_set(int argc, char* argv[], void *p)
{
#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	return subcmd(gponmgr_set_command, argc, argv, (void *)gponmgr_set_command);
}

/******************************************************************************
 Descriptor:	It's used to get gpon driver status.
 Input Args:	see gponmgr_root_command.
 Ret Value:	    SUCCESS:0.FAIL:-1. 
******************************************************************************/
static int do_get(int argc, char* argv[], void *p)
{
#if API_CMD_GPONMGR_DBG
	printf("enter %s.\n", __func__);
#endif

	return subcmd(gponmgr_get_command, argc, argv, (void *)gponmgr_get_command);
}
/******************************************************************************
 Descriptor:	It's used to dump omci packet.
 Input Args:	buff and buff length
 Ret Value:	none.
******************************************************************************/
void dump_omci(unsigned char *msg, unsigned int msg_len){
	unsigned char n, *p = msg;
	int i ;
	printf("\r\n===================omciDumpPkt:--msg_len = %d=================",msg_len);
	for(i=0; i<msg_len; i++) {
		n = i & 0x0f;
		if(n == 0x00) 		printf(" \n%.4x: ", i);
		else if(n == 0x08) 	printf(" ");
		printf("%.2x ", *p++) ; 
	}
	printf("\n") ;
}
/******************************************************************************
 Descriptor:	test Omci recive and  send api
 Input Args:	see gponmgr_root_command.
 Ret Value:	    SUCCESS:0.FAIL:-1. 
******************************************************************************/
static int recive_send_omciPacket(int argc,char * argv[],void * p)
{
    int ret = -1; 
    int socket = -1;
    unsigned char msg[100] = {0};
    unsigned int msg_len = 0;
    ret = gponmgr_lib_init_omci_socket(&socket);
    if(0 != ret){
        printf("init socket failed\n");
    }
    printf("socket:%d\n",socket);
    while(1){
        memset(&msg, 0, 100);
        ret = gponmgr_lib_omci_receive_block(msg,&msg_len);
        if(0 == ret){
            printf("\n===========recv successed================\n");
            ret = gponmgr_lib_omci_send_block(&msg_send,60);
            if(0 == ret){
                printf("\n===========send successed================\n");
            }
            else{
                printf("\n===========send failed===================\n");
                return -1;
            } 
            dump_omci(&msg,msg_len);
        }
        else{
            printf("\n===========recv failed===================\n");
            return -1;
        }
    }
    return 0;
}
/******************************************************************************
 Descriptor:	test create gemport api
 Input Args:	see gponmgr_root_command. argv[1] gemport id,argv[2] tcont,argv[3] gemport type
 Ret Value:     SUCCESS:0.FAIL:-1. 
******************************************************************************/
static int create_gemport(int argc,char * argv[],void * p)
{
    unsigned short portid = 0;
    unsigned short tcont = 0;
    int type = 0;
    int ret = -1;

    if(argc < 3){
        printf("gponmgr gemport portid tcontid type\n");
    }
    portid = atoi(argv[1]);
    type = atoi(argv[2]);
    tcont = atoi(argv[3]);
    ret = gponmgr_lib_config_gemport(portid, type, tcont);
    if(ret == 0){
        printf("create gemport success\n");
        return 0;
    }
    else{
        printf("create gemport failed\n");
        return -1;
    }
}
/******************************************************************************
 Descriptor:	test delete gemport api
 Input Args:	see gponmgr_root_command.
 Ret Value:	    SUCCESS:0.FAIL:-1. 
******************************************************************************/
static int del_gemport(int argc,char * argv[],void * p){
  
    int portid = 0;
    if(argc < 2){
        printf("delGem portid\n");
    }
    
    portid = atoi(argv[1]);
    if(gponmgr_lib_delete_gemport(portid)== 0){
        printf("del gemport success\n");
    }
    else{
        printf("del gemport unsuccess\n");
        return -1;
    }
    return 0;
}
/******************************************************************************
 Descriptor:	test bind tcont api
 Input Args:	see gponmgr_root_command.
 Ret Value:	    SUCCESS:0.FAIL:-1.
******************************************************************************/

static int bindTcont(int argc,char * argv[ ],void * p){
  int ret = -1;
  unsigned short tcont = 0;
  unsigned short allocid = 0;
  if(argc < 2){
    printf("bindTcont tcont allocid\n");
    return ret;
  }
  tcont = (unsigned short)atoi(argv[1]);
  allocid = (unsigned short)atoi(argv[2]);
  ret =  gponmgr_lib_bind_tcont_to_allocid(tcont, allocid);
  if(ret !=0 ){
        printf("bindTcont failed!\n"); 
        return ret;
  }
  printf("tcont:%u,allocid:%u\n",tcont,allocid);
  printf("bindTcont successed!\n");
  return ret;
}
/******************************************************************************
 Descriptor:	test unbind tcont api
 Input Args:	see gponmgr_root_command.
 Ret Value:	    SUCCESS:0.FAIL:-1.
******************************************************************************/
static int unbind_tcont(int argc,char * argv[],void * p){
    int ret = -1;
    unsigned short tcont = 0;
    tcont = atoi(argv[1]);
    ret = gponmgr_lib_unbind_tcont_to_allocid(tcont);
    if(0 != ret){
        printf("unbind tcont failed!\n");
        return ret;
    }
    printf("unbind tcont successed!\n");
    return ret;
}
/******************************************************************************
 Descriptor:	thread function to poll event from driver
 Input Args:	reserved
 Ret Value:	    FAIL:-1.
******************************************************************************/
void *poll_event(void* arg){
    int ret = -1;
    XPON_EVENT_t event; 
    while(1){
        memset(&event, 0, sizeof(XPON_EVENT_t));
        ret = gponmgr_lib_get_driver_event_block(&event);
        if(0 == ret){
            printf("get event id:%u, event value:%u\n",event.id,event.value);
        }
        else{
            printf("get event failed!\n");
            return -1;
        }
    }

}
/******************************************************************************
 Descriptor:	test poll event api
 Input Args:	see gponmgr_root_command.
 Ret Value:	    FAIL:-1.
******************************************************************************/
static int get_event(int argc,char * argv[],void * p)
{
    pthread_t tid;
    if(0 != pthread_create(&tid, NULL, poll_event, 0)){
        printf("create thread err\n");
        return -1;
    }
    if(event_tid_rec.size<=4){
        event_tid_rec.tid[event_tid_rec.size] = tid;
        event_tid_rec.size++;
    }
    return 0;
}

/******************************************************************************
 Descriptor:	test poll event api
 Input Args:	see gponmgr_root_command.
 Ret Value:	    FAIL:-1.SUCCESS:0.
******************************************************************************/
static int stop_thread(int argc, char * argv[], void*p){
    int index = atoi(argv[1]);
    
    pthread_t tid = event_tid_rec.tid[index];
    printf("tid: %d %s %d\n",tid,__FUNCTION__,__LINE__);
    if(0 != pthread_cancel(tid)){
        printf("cancel thread err!\n");
        return -1;
    }
    event_tid_rec.tid[index] = 0;
    event_tid_rec.size--;
    
    return 0;
}

/******************************************************************************
 Descriptor:	test get tcont allocid map api
 Input Args:	see gponmgr_root_command.
 Ret Value:	    FAIL:-1.SUCCESS:0.
******************************************************************************/
static int get_tcont_allocid_map(int argc,char * argv [ ],void * p){
    return gponmgr_lib_get_tcontMapInfo();
}

/******************************************************************************
 Descriptor:	test set emergence state api
 Input Args:	see gponmgr_root_command.
 Ret Value:	    FAIL:-1.SUCCESS:0.
******************************************************************************/

static int set_emergence_state(int argc,char * argv[ ],void * p){
    if(ECONET_E_SUCCESS!=gponmgr_lib_set_emergency_state()){
        printf("set emergence failed\n");
    }
    printf("call set emergence successed\n");
    return ECONET_E_SUCCESS;
}
/******************************************************************************
 Descriptor:	prepare the cmdline args, make it similar to 'main' function's arg
 Input Args:	see gponmgr_root_command.
 Ret Value:	    FAIL:-1.SUCCESS:0.
******************************************************************************/
int cmdline_parser(char * cmdline, size_t * argc, char ** argv)
{
    size_t cnt = 1, pos = 0;
    enum{SEARCH_ARG, HIT_ARG} state = SEARCH_ARG;
    
    for(pos = 0; cmdline[pos] != 0; ++pos)
    {
        switch(state)
        {
            case SEARCH_ARG:
                if(islower(cmdline[pos]) || isupper(cmdline[pos]) || isdigit(cmdline[pos]))
                {
                    state = HIT_ARG;
                    argv[cnt] = & cmdline[pos];
                }
                break;

            case HIT_ARG:
                if('\x20' == cmdline[pos] || '\n' == cmdline[pos])
                {
                    cmdline[pos] = 0;
                    state = SEARCH_ARG;
                    ++ cnt;
                    if(cnt > MAX_NUM_ARG)
                    {
                        return ECONET_E_SUCCESS;
                    }
                }
                break;

            default:
                return ECONET_E_FAIL;
        }
    }

    *argc = cnt;
    return ECONET_E_SUCCESS;
}
/******************************************************************************
 Descriptor:	main function to get && parser cmdline then run it.
 Input Args:	see gponmgr_root_command
 Ret Value:     Success: ECONET_E_SUCCESS
                      Fail: ECONET_E_FAIL
******************************************************************************/
int main (int argc, char* argv[])
{
    char cmdline[MAX_CMD_LEN]={0};
    char *value[MAX_NUM_ARG]={0};
    size_t count = 0;
  
    while(1){
        printf("\nGPON_API_TEST>>");         
        fgets(cmdline, MAX_CMD_LEN, stdin);

        if(ECONET_E_SUCCESS!=cmdline_parser(cmdline,&count,value)){
            printf("parser cmdline err!\n");
            return ECONET_E_FAIL;
        }
        
        subcmd(gponmgr_root_command, count, value, (void*)gponmgr_root_command);
        memset(cmdline, 0, MAX_CMD_LEN);
           
    }
    return ECONET_E_SUCCESS;
}
