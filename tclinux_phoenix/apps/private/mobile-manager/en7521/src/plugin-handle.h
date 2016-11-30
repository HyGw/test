#ifndef PLUGIN_HANDE_H_
#define PLUGIN_HANDE_H_
#include <pthread.h>

#include "event.h"
#include "urandom.h"
#include "jsonpkt.h"
#include "list.h"
#include "mobile_manager.h"
#include "parameter_table.h"
//#include "msg-netlink.h"
#include "libtcapi.h"

#define PLUGIN_VER_LEN			10

#define PLUGIN_FILE_GETSIZE		256
#define BUNDLE_CMD_EXIST		(1 << 0)

#define PLUGIN_OSGI 	0		//bundle type
#define PLUGIN_CPK 	5		//cpk type
#define PLUGIN_FILE 	6		//File only

#define PLUGIN_OK				(1)
#define PLUGIN_GEN_ERR			(-1)
#define PLUGIN_INSTALL_ERR		(-17)
#define PLUGIN_NOT_EXIST 		(-12)
#define PLUGIN_NAME_ERR			(-8)
#define PLUGIN_TYPE_ERR			(-7)
#define PLUGIN_VERSION_ERR		(-6)
#define PLUGIN_BUSY				(-5)
#define PLUGIN_FLASH_LIMIT		(-4)


#define PLUGIN_C_PATH		"/usr/osgi/plugin-c"  /* path for plugin c*/
#define PLUGIN_C_INFO		"/usr/osgi/plugin-c/%s/Info.plugin"  /* info for plugin c*/
#define PLUGIN_C_PID			"/usr/osgi/plugin-c/%s/pid"  /* pid for plugin c*/
#define PLUGIN_C_BIN			"/usr/osgi/plugin-c/%s/MyPlugin"  /* path for plugin c*/

#define PLUGIN_B_PATH		"/usr/osgi/plugin-b" /* path for bundle c*/
#define PLUGIN_B_INFO		"/usr/osgi/plugin-b/%s/Info.plugin"  /* info for plugin bundle*/
#define PLUGIN_B_BACKUPINFO	"/usr/osgi/plugin-b/%s/Info"  /* info backup for plugin b*/
#define PLUGIN_VAR_PATH		"/var/osgi/%s" /* path for bundle c*/

#define PLUGIN_FACTORY_FILE		"/usr/osgi/factory/%s.%s"
#define PLUGIN_DOWNLOAD_FILE		"/tmp/%s.%s"
#define PLUGIN_DOWNLOAD_RESULT	"/tmp/%s.%s.result"

#define PLUGIN_TMP_FILE		"/tmp/plugin" /* path for bundle c*/

#define MOBLIE_PLUGIN_LOGFILE "/var/osgi/log"

#define PUSH_SOCK_PATH		"/tmp/eventInform"
#define MAX_PUSH_CLIENT		5

#define PARAM_PLUNGIN			"Plugin"
#define PARAM_PLUNGIN_NAME		"Plugin_Name"
#define PARAM_PLUNGIN_VERSION		"Version"
#define PARAM_PLUNGIN_URL		"Download_url"
#define PARAM_PLUNGIN_SIZE		"Plugin_size"
#define PARAM_PLUNGIN_OS		"OS"
#define PARAM_PLUNGIN_RUN		"Run"
#define PARAM_PARAMETER		"Parameter"

typedef enum plugctl_cmd {
	PLUGCMD_NULL=0,		//No CMD Excute now
	PLUGCMD_FACTORY, 		//FactoryPlugin	
	PLUGCMD_INSTALL,		//Install
	PLUGCMD_CANCEL,		//Install_cancel	
	PLUGCMD_QUERY,		//Install_query
	PLUGCMD_LIST,			//ListPlugin notice lisr is disorder
	PLUGCMD_POST,			//ListPlugin notice lisr is disorder
	PLUGCMD_RUN,			//Run
	PLUGCMD_SET,			//SetPlug-inParameterValues
	PLUGCMD_STOP,			//Stop
	PLUGCMD_UNSTALL,		//UnInstall
	PLUGCMD_END
}plugctl_cmd_e; 


typedef struct plugin_info {
	char name[MAX_BUNDLE_NAME_LEN+1];	
	char version[PLUGIN_VER_LEN+1];
	int size;
	int ostype;
	int pid;
	int cmd;
	int install;
	int run;
	int registerevent;	
	int lockevent;	
	void * ctx;
	struct list_head _head;
	//struct list_head _eventhead;
} plugin_info_t;

typedef int (plugctlfunc)(json_t* req_root, plugin_info_t *p, plugctl_cmd_e cmd);

typedef struct plugctl_cmd_s
{
	char *cmdname;
	int ctl;
	int cmdindex;
	plugctlfunc *plugctlcmdfunc;
}plugctl_cmd_t;

#define MAX_EVENT_TYRPE	4
#define USB_ACT_LOCKED 		(1 << 15)		/* USB EVENT locked */
#define USB_ACT_REGSITER 	(1 << 14) 	/* USB EVENT has register plugin c or b*/
#define USB_ACT_PULL 		(1 << 1)		 /* USB EVENT PULL*/
#define USB_ACT_INSERT 		(1 << 0) 		/* USB EVENT INSERT*/

#define HAS_USB_ACT(x, y) ( (x) & (y))
#define SET_USB_ACT(x, y) ( (x) |=  (y))
#define CLR_USB_ACT(x, y) ( (x) &=~(y))

#define EVENT_REGISTER_TIMEROUT 8
/*
{
  "Event":"USB_DEV_ACTION",
  "ActionType":"%s", 	//"USB_DEV_INSERT"/"USB_DEV_PULL"
  "DevType":"%s",  	//"USB_SERIAL"/"USB_CDC_ACM"/"USB_HID"/"USB_STORAGE"
  "DevId":%d,
  "DevName":"%s", 	//128
  "MountPath":"%s"	//256
}
*/
typedef struct eventinfo{
	unsigned int action; 	/*  for action insert or pull out and lock result */
	int devtype;
	int devid;
	char devname[CT_USB_DEV_NAME_MAX_LEN];
	char mountpath[CT_USB_DEV_MOUNT_PATH_MAX_LEN];
	int32_t timeout;  		/*  next action time  */
	char *info;
	plugin_info_t *pOwner;
	struct list_head _head;
	//struct list_head _pluginhead;
} eventinfo_t;

typedef struct plugin_cluster {
	event_ctrl_t *pev_ctrl;
	event_epoll_t nlksock;	/* netlink sock */
	event_epoll_t pushsock;	/* netlink sock */
	event_timer_t timer; 	/* time for handler usb in or out event */
}plugin_cluster_t;

int plugin_list_load(void);
int start_plugctl_work(void);

#endif /*PLUGIN_CTRL_H_*/
