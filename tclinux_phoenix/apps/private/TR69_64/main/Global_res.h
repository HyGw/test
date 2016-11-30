#ifndef GLOBAL_RES_H_
#define GLOBAL_RES_H_

#include <pthread.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <assert.h>
#include <sys/time.h>
#include <fcntl.h> //for message queue 
//#include <sys/types.h>
//#include <signal.h>
#include <unistd.h>
#include <semaphore.h>
#include <netinet/in.h>

//#include "../../../lib/libtcapi.h"
#include "cwmp_extra.h"
#include "messageHandle.h"
#include "tctimer.h"
#include "cwmp_dbg.h"
#include "mbuf.h"

#include "cwmpXmlmembuf.h"
#include "cwmpXml.h"
#include "cwmpXmlparser.h"

#include "cwmpRpc.h"
#include "cwmpSoap.h"
#include "cwmp.h"
#include "cwmpRpc.h"
#include "cwmpParamApi.h"
#include "cwmpNode.h"
//#include "cwmpParameter.h"
#include "cwmpParameter_table.h"

#if defined(TCSUPPORT_CT_MIDWARE)
#include "parse.h"
#include "midware.h"
#endif

#ifdef TCSUPPORT_BACKUPROMFILE
#include "../../../public/mtd/tc_partition.h"
#endif
//extern void tcdbg_printf(char *fmt,...);
/******************************************
 * 
 *global var define
 *
 *******************************************/ 

/*use for semaphore*/
extern sem_t SEM_IMFORM;
#ifndef THREAD_REDUCE
extern sem_t SEM_VALCHANGE;
extern sem_t CWMP_PING;//for ip ping diagnostic
#else
extern uint8 ValueChangeFlag;
extern int message_handle();
extern void ThreadedTimerCheck(void);
#endif

extern uint8 cardNotifyFlag;

/*use for tr069 rpc*/
extern /*volatile*/ cwmp_rpc_t       rpc;
extern spSysParam_t spSysParam;


/***************************
*
*global function define
*
***************************/
//void *instantiate_task();

void getdbgflag(void);
void resetdbgflag(void);
#if defined(TCSUPPORT_CT)
void resetWebpageFlag(void);
#endif
void tc_printf(char *fmt,...);
void SetCwmpPswModFlag(uint8);
void SignalInform(void);
int httpTaskStart(void);

/*for UPnP Port mapping*/
int PortMapInit();

extern void SaveCwmpFlags(void);
extern void InitCWMPFlags(void);
/*below is for session retry---add by brian*/
//extern void cwmpSetSessionRetryFlag(int value);
//extern void cwmpResetSessionRetryWaitInterval(void);
/*end*/
extern volatile void *theAllegroServerDataPtr;
extern pthread_mutex_t value_change_lock;
extern pthread_mutex_t asyn_tcapi_lock;
extern sem_t SEM_ASYNTCAPI;
extern QueueNodePtr g_pAsynQueue;
extern int g_MonitorCollectorEnable;
extern int g_PMInformEnable;
extern uint8 inform_task_start;
/*For TR064 State*/
#define kUpnpStateOff       0
#define kUpnpStateRunning   1
#define kUpnpStateShutdown  2

/*For CWMP Debug messsage in rompager*/
#define dbg_pline_1(buf) tc_printf(buf)
#define dbg_plineb_1(buf, byte) tc_printf(buf); tc_printf("%d", byte)
#define dbg_plinew_1(buf, word) tc_printf(buf); tc_printf("%d", word)
#define dbg_plinel_1(buf, long) tc_printf(buf); tc_printf("%d", long)

#define DBG_pline_1 dbg_pline_1
#define DBG_plinel_1 dbg_plinel_1

#define pause(x) usleep((x)*1000)


/*For CWMP Download UpLoad Reboot Factory Reset RPC Method*/
#define CLMP_REBOOT (1 << 0)
#define CLMP_RESET_DEF (1 << 1)

#define NO_OPERATION  	0
#define NO_HEADER	1
#define HTML_HEADER	2

extern int FirmWareUpdate(void);
extern int getCwmpAttrValue(int attrType,char *buf,uint32 size);
extern int setCwmpAttrValue(int attrType,char *buf);

#define UPLOAD_ROMFILE_PATH	"/var/tmp/up_romfile.cfg"
#define DOWNLOAD_CONFIG_FILE_NAME "ROM-0"
#define DOWNLOAD_CONFIG_FILE_PATH UPLOAD_ROMFILE_PATH

#define TCLINUX_PATH "/var/tmp/tclinux.bin"
#define DOWNLOAD_FIRMWARE_FILE_NAME "FIRMWARE"
#define DOWNLOAD_FIRMWARE_FILE_PATH TCLINUX_PATH


#define UPLOAD_CONFIG_FILE_NAME "/rom-0"
#define UPLOAD_CONFIG_FILE_PATH "/tmp/var/romfile.cfg"

#define UPLOAD_LOG_FILE_NAME "/device_log"
#if defined(TCSUPPORT_CT)
#define UPLOAD_LOG_FILE_PATH "/tmp/upload_log.txt"

#ifdef TCSUPPORT_SYSLOG
/*Add for syslog*/
#define SYSLOG_FILE_PATH	"/var/log/currLogFile"
int get_syslog_file(char *fileName);
#endif
#else
#ifdef TCSUPPORT_SYSLOG
#ifdef TCSUPPORT_SYSLOG_ENHANCE
#define DEFAULT_LOG_FILE 	 "/var/log/currLogFile"
#else
#define DEFAULT_LOG_FILE 	 "/tmp/var/log/messages"
#endif
#endif
#define UPLOAD_LOG_FILE_PATH "/tmp/log"
#endif

#define REBOOT_CMD	"/userfs/bin/mtd -r write %s romfile"
#define NOREBOOT_CMD	"/userfs/bin/mtd write %s romfile"
#define STORE_ROMFILE_FILE_NAME "/tmp/ct_store_romfile.conf"
#define ROMFILE_MAX_PENDING   65536
#define ROMFILE_MAX_LEN   65536
#define GZ_TMP_PATH "/tmp/"
#define GZ_ROMFILE_FILE_NAME "romfile.gz"
#define CT_ROMFILE_FILE_NAME "ctromfile.cfg"
#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
#define DEF_ROMFILE_PATH 	"/userfs/ctromfile.cfg"
#else
#define DEF_ROMFILE_PATH 	"/userfs/romfile.cfg"
#endif

#define CT_CWMP_UPLOAD_ROMFILE_FILE_NAME "/tmp/cwmp_upload_ctromfile.cfg"

#define FAIL -1
#if defined(TCSUPPORT_CT)
/*Wan cfg node*/
#ifndef PURE_BRIDGE
	#define PVC_NUM 8
#else
	#define PVC_NUM 4
#endif

#define PPP_MODE "2"
#define BRIDGE_MODE "3"

#else

#ifndef PURE_BRIDGE
#if defined(TCSUPPORT_WAN_ETHER)
	#define PVC_NUM 11
#elif defined(TCSUPPORT_WAN_PTM)
    #define PVC_NUM 10
#else
    #define PVC_NUM 8
#endif
#else
	#define PVC_NUM 4
#endif
#endif

#define MAX_SERVICE_NUM 	8
#define MAX_SMUX_NUM 8
#define MAX_WAN_IF_INDEX  (PVC_NUM*MAX_SMUX_NUM)

char *X_EVENT_CODE(const char *event);
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
#define X_CT_COM	"X_CU_"
#define X_CUC_COM	"X_CU_"
#define CHINA_UNICOM_ENABLE 1
#else
#if defined(TCSUPPORT_CMCC)
#define X_CT_COM	"X_CMCC_"
#else
#define X_CT_COM	"X_CT-COM_"
#endif
#endif
#define X_CU		"X_CU_"
#define X_CMCC		"X_CMCC_"

#if defined(TCSUPPORT_CT_USB_BACKUPRESTORE)
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
#define CWMP_USB_RESTORE_NODE X_CT_COM"Restore"
#define CWMP_USB_PARAM_NAME "Enable"
#else
#define CWMP_USB_RESTORE_NODE "UsbRestore"
#define CWMP_USB_PARAM_NAME "UsbAutoRestoreEnable"
#endif
#endif


/*Add for VendorConfigFile TR069 Nodes*/
#define VENDORCFGFILE_COMMON	"VendorCfgFile_Common"
#define VENDORCFGFILE_ENTRY	"VendorCfgFile_Entry%d"
#define CFG_FILE_ENTRY		0

#define LAN_ARP_INFO_PATH	"/tmp/lan_host_arp.info"
#define MAX_BUF_SIZE		256

/*MIB info for TR069 & TR064 memory allocate*/
extern void showMIB(void);
extern uint32 tctimer_free_mib;
extern uint32 tctimer_malloc_mib;
extern uint32 rompager_free_mib;
extern uint32 rompager_malloc_mib;
extern uint32 os_free_mib;
extern uint32 os_malloc_mib;

extern unsigned long HostAddress_mib;
typedef int             STCP_SOCKET;
#define kStcpServerCount            (5)
//typedef unsigned short  StcpPort;

extern STCP_SOCKET 	 gTcpListenerSockets[kStcpServerCount];
extern unsigned short		 gTcpListenerPorts[kStcpServerCount];
extern int 			 gTcpListenerCount;
extern int 			 gCwmpTcpListener;

extern int ReBindToDevice();

extern void BindTR69ToServerSocket();
#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
extern unsigned long compute_crc32buf(char *buf, size_t len);
extern int compute_checksum(char *inputfile,char *outputfile);
#endif

#define PMINFORM_NODENAME "PMInform_Entry"
#define PMINFORM_ATTR_ENABLE "Enable"
#define PMINFORM_VAL_YES "Yes"
#if defined(TCSUPPORT_CT_PMINFORM)
#define PMINFORM_VAL_NO "No"
#define PMINFORM_VAL_1 "1"
#define PMINFORM_VAL_0 "0"
#define PMINFORM_VAL_DEFLCINTVAL 300 /* --> 5*60 */
#define PMINFORM_ATTR_NTPST "NTPSync"
#define PMINFORM_ATTR_CWMPIP "CwmpWanIP"
#define PMINFORM_ATTR_USERID "UserID"
#define PMINFORM_ATTR_LSURL "LogServerUrl"
#define PMINFORM_ATTR_LUINTVAL "LogUploadInterval"
#define PMINFORM_ATTR_LCINTVAL "LogCountInterval"
#define PMINFORM_ATTR_CPURENABLE "CPURateEnable"
#define PMINFORM_ATTR_MEMRENABLE "MemRateEnable"
#define PMINFORM_ATTR_SERENABLE "SEREnable"
#define PMINFORM_ATTR_ERRCODEENABLE "ErrorCodeEnable"
#define PMINFORM_ATTR_PLRENABLE "PLREnable"
#define PMINFORM_ATTR_PKTLOSTENABLE "PacketLostEnable"
#define PMINFORM_ATTR_TEMPENABLE "TEMPEnable"
#define PMINFORM_ATTR_UPDATAENABLE "UpDataEnable"
#define PMINFORM_ATTR_DWDATAENABLE "DownDataEnable"
#define PMINFORM_ATTR_LANDEVMACENABLE "LANDeviceMACEnable"
#define PMINFORM_ATTR_LANDEVMAC "LANDeviceMAC"
#define PMINFORM_ATTR_WLANDEVMACENABLE "WLANDeviceMACEnable"
#define PMINFORM_ATTR_WLANDEVMAC "WLANDeviceMAC"
#define PMINFORM_DOMAIN_CONF_PATH "/tmp/PMInformDomain.conf"
#define PMINFORM_TFTP_TMP "/tmp"
#define PMINFORM_TFTP_CMD "cd "PMINFORM_TFTP_TMP" && /usr/bin/tftp -p -t 1 -i %s -r %s %s"
#endif
#if defined(TCSUPPORT_CT_ADSL_BIND1)
#define ATTR_XBIND_RESULT "XBINDResult"
#define ATTR_XBIND_USERNAME "AutoBINDUserName"
#endif
#if defined(TCSUPPORT_CMCC)
#ifdef TCSUPPORT_IPV6
#define ROUTE6_NODE "Route6"
#endif
#endif
extern void cwmpPerformQoETask(void);
#endif
