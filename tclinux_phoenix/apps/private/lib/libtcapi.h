#ifndef _LIB_TCAPI_H
#define _LIB_TCAPI_H
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>


#include "../../private/xpon/inc/omci/omci_external_msg.h"
int
tcapi_set(char* node, char* attr, char* value);

int
tcapi_unset(char* node);

int
tcapi_get(char* node, char* attr, char *retval);

int
tcapi_staticGet(char* node, char* attr, char *retval);

int
tcapi_show(char* node, char * retval);

int
tcapi_commit(char* node);

int
tcapi_save(void);
int
tcapi_read(char* node);

int
tcapi_readAll(void);

void
tcdbg_printf(char *fmt,...);

/*TR069 message mechanism----------add by brian*/
#define PROJID 2
#define MAX_READ_SIZE 256
#define MAX_SEND_SIZE 256
#define CWMP_MQ_FLAG_PATH "/tmp/cwmp/tr069agent_mq"
#define CWMP_MAX_MSG_LEN 256

/*TR069 message mechanism---------add by brian*/
typedef struct cwmp_msg
{
	char cwmptype;
	union
	{
		char reserved[CWMP_MAX_MSG_LEN - 1];
	}text;
}cwmp_msg_t;

typedef struct tc_msg
{
	long mtype;
	cwmp_msg_t msgtext;//message content
}tc_msg_t;

int 
sendmegq(long type,cwmp_msg_t *buf,int flag);

////////////////for OMCI////////////////
//#define OMCI_DEBUG
int sendOmciCmdMsg(char msgType, omciBaselinePayload_t *buf);
int sendOmciMeCmdMsg(omciBaselinePayload_t *buf);
int sendOmciNotifyMsg( notify_msg_t *buf);

typedef struct oamAlarmMsg_s
{
	long msg_type;

	unsigned short  trapIndex;			// Alarm ID
	unsigned short  devIndex_1;			// port ID
	unsigned short  devIndex_2;			// LLID
	unsigned short  devIndex_3;			// PON IF
	unsigned char   trapOtherInfo[100];	// Alarm State & Info
}OamAlarmMsg_t, *OamAlarmMsg_Ptr;

int sendAlarm2EponOam(OamAlarmMsg_Ptr pAlarm);

#define EPON_OAM_PIDFILE "/var/run/epon_oam.pid"

// romfile node or attribute used by oam. 
#define CONF_OP_ATTR 1<<0
#define OP_NODE 1<<1
	
typedef struct OamParamNodeInfo{
	int flag;		/* flag for node or attribute */	
	char *node; 	/* node name */
	char *attr; 	/* attribute name */	
}OamParamNodeInfo_t, *OamParamNodeInfo_Ptr;


/*****************************for JOYME****************************/
/*netlink socket*/
#define NLK_LOCAL_PAYLOAD		 400 
#define NLKMSG_GRP_MULTI		(1 << 0)	/* wan ip change/wlan online */
#define NLKMSG_GRP_MOBILE		(1 << 1)	/* usb to mobile*/
#define NLKMSG_GRP_LOCAL		(1 << 2)    /*  local msg for mobile use */

#define NLKMSG_SOCK_TYPE		 22


#define MAX_JOYME_MSG_LEN 			256
#define MAX_BUNDLE_NAME_LEN 		64


#define MSG_TYPE_CPLUGIN			1
#define MSG_TYPE_BUNDLE				2
#define MSG_TYPE_LOCAL				3

#define MSG_MUlTICAST_EVENT					1 	/* Wanchange or wlan dev on	*/
#define MSG_USB_EVENT2MOBILE				8	/* usb event to mibile	*/
#define MSG_USB_EVENT						10	/* usb event form mibile to plugin C */
#define MSG_HANDLE_EVENT					7 	/* For register or lock event handle*/
#define MSG_HANDLE_MOBILE					9	/* for mobile manager local handle*/

#define EVENT_REGISTER 		1
#define EVENT_UNREGISTER 	2
#define EVENT_LOCK 			3
#define EVENT_UNLOCK 		4

#define CT_USB_DEV_NAME_MAX_LEN    			(128)
#define CT_USB_DEV_MOUNT_PATH_MAX_LEN    	(256)

typedef enum
{
    CT_USB_DEV_ACTION_INSERT,
    CT_USB_DEV_ACTION_PULL,
} CtUsbDevActionType;

typedef enum
{
    CT_USB_DEV_SERIAL = 0x1,
    CT_USB_DEV_CDC_ACM = 0x2,
    CT_USB_DEV_HID = 0x4,
    CT_USB_DEV_STORAGE = 0x8,
} CtUsbDevType;

typedef struct localmsg{
	int action;
	void *ptr;
	char pluginname[MAX_BUNDLE_NAME_LEN+1];
	int pluginPid;
}mobile_msg_t;

typedef struct handle_msg{		/* for usb regsiter lock event*/
	int action; 			/* register, unresgister, lock, unlock */
	int devtype;			/* for register */
	int devid;				/* for lock */
	int pluginpid;
	char bundlename[MAX_BUNDLE_NAME_LEN+1];
}handle_msg_t;

typedef struct usb_msg{ /* for usb in or out t*/
	int action;			/* in or out */
	int devtype;
	int devid;
	char devname[CT_USB_DEV_NAME_MAX_LEN];
	char mountpath[CT_USB_DEV_MOUNT_PATH_MAX_LEN];
}usb_msg_t;

typedef struct netlink_msg{
	struct nlmsghdr nlmhdr;
	int eventType;	
	union {
		char payload[NLK_LOCAL_PAYLOAD];    /* multicast event app donot parse */
		handle_msg_t 	hmsg;
		usb_msg_t	umsg;
		mobile_msg_t	mmsg;
	}data;
}nlk_msg_t;

typedef struct netlink_buffer{
	struct nlmsghdr nlmhdr;
	int eventType;
	char payload[];
}nlk_buf_t;

#define NLK_MSG_LEN(x)	NLMSG_SPACE(sizeof(x)+ sizeof(int))

int nlksock_create(int nlk_type, int pid, int group) ;
int nlkmsg_send(int group, int len, void * context);
int msgpush_send(int pid, char *name, char *buffer);
#endif
