#ifndef _MIDWARE_H_
#define _MIDWARE_H_

#include <stdlib.h>
#include <string.h>
#include "parse.h"

#define E8_PATH  "/var/ct/tmp/interface2sock"
//#define E8_PATH "/tmp/midware-server"   /* for test */
//#define E8_PATH  "/tmp/interface2sock"

typedef struct UserIP_s{
	int status;
	int index;
	char *ipaddr;
	struct UserIP_s *next;
}UserIP_t, *UserIP_p;

#define MODNAME "firmware"
#define MIDVALUECHANGEFLAG 1<<0
#define MIDPERIODFLAG 1<<1
#define MIDSCHEDULEFLAG 1<<2
#define MIDBOOTFLAG 1<<3
#define MIDIPPINGFLAG 1<<4
#define MIDFILEFLAG 1<<5
#define MIDBOOTTRAPFLAG 1<<6
#define MIDREBOOTFLAG 1<<7


#define WLANBUTTON	"WLAN"
#define WPSBUTTON	"WPS"
#define SHORT_CLICK	"SHORT"
#define LONG_CLICK	"LONG"
#define DOUBLE_CLICK	"DOUBLE"
extern uint8 button_detect_flag[2];


extern sem_t SEM_MW_IMFORM;
extern pthread_mutex_t midware_task_lock;
extern int iTr069Enable;
extern uint32 ItmsAddr, MidwareAddr;
extern int tr69WanIndex;
extern uint8 MWValueChangeFlag;
extern uint16 MWWanChangeStatus;

extern uint8 valueChangeFlag;
extern uint8 inform_task_start;  //change by dl
extern uint8 operationFlag;
extern midware_packet *resp;
extern midware_packet *in;

extern uint8 getTr069RestartItself(void);
extern uint8 getMWExitFlag(void);
extern void setMWExitFlag(uint8 flag);
extern uint8 getMWActiveNotifyFlag(void);
extern void setMWActiveNotifyFlag(uint8 value);
extern uint8 getITMSActiveNotifyFlag(void);
extern void setITMSActiveNotifyFlag(uint8 value);
//extern uint16 getMWWanChangeIndex(void);
extern uint16 getMWWanChangeIndex(uint16 index);
extern void setMWWanChangeIndex(uint16 index);


extern void setRegisterFlag(uint8 value);
extern uint8 getRegisterFlag(void);
extern void setOperationFlag(uint8 value);
extern uint8 getOperationFlag(void);
extern void midware_resovle(void);

void midwareInit(void);
int  midwareTaskStart(void);
void *midwareTask(void);
midware_packet * buildRegisterPkt(void);
int cmdRecv(int sockfd, buf_t *bufT);
int checkFlag(void);
void cmdRspSend(int sockfd, midware_packet*resp);
void sockSend(int sockfd, buf_t *bufT);
void midwareInformTask(int flag, int sockfd);
void buildInformPkt(buf_t *buf);
void informSend(int sockfd, buf_t *buf);
void midwareInformResponseResponse(void);
#endif
