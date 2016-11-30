//#ifdef _MESSAGEHANDLE_H
//#define _MESSAGEHANDLE_H

#include "Global_res.h"
#include "../../../lib/libtcapi.h"
/***************************************
*
*	macro definition
*
***************************************/

#define CONN_REQ 		0 		/*message queue for connection request*/
#define CWMP_CHANGED 	1	 	/*message queue for acs url change*/
#define VALUE_CHANGED 	2  		/*message queue for value change*/
#define HOST_REINIT 	3  	 	/*message queue for host information reinit*/
#define DEVICE_REINIT 	4 		/*message queue for host information reinit*/
#define DIAG_COMPLETE	5		/*message queue for diagnosis complete, then signal inform */
#define ALARM_REINIT 	6 		/*message queue for host information reinit*/
#define WAN_CHANGE		7 		/*message queue for WAN config change*/
#define TR69_BIND_SERVER  8
#define BUTTON_DETECT  9
#define CWMP_REBOOT  10
#define UPNP_DM  11
#define SIM_CARD  12
#define NAME_CHANGE  13
#define STB_CHANGE	14
#define LOID_CHANGE  15
#define IPOE_EMULATOR_OK 16
#define BRIDGE_CONNECTION_UP	17
#define ROUTE_PPPCONNECTION_UP  18
#define BIND1_REGISTER 19
#define MONITOR_COLLECTOR_SWITCH 20
#define DNS_LIMIT_ALERT 21
#define QOE_RESTART 22


/***************************************
*
*	function declare
*
***************************************/
//int process_mq(char *message);
int process_mq(cwmp_msg_t *message);
int open_msgq();
//int read_msg(int mqid,long type,char *text,int flag);
int read_msg(int mqid,long type,cwmp_msg_t *text,int flag);
//int write_msg(int mqid,long type,char *text,int flag);
int write_msg(int mqid,long type,cwmp_msg_t *text,int flag);

#ifndef THREAD_REDUCE
void *message_handle();
#else
int message_handle();
#endif
int init_msg();

uint8 getRoutePPPFlag(void);
void setRoutePPPFlag(uint8 value);
uint8 getBridgePPPFlag(void);
void setBridgePPPFlag(uint8 value);

//#endif
