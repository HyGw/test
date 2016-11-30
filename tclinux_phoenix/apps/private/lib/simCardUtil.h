#ifndef _SIMCARDUTIL_H
#define _SIMCARDUTIL_H

#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/ioctl.h>

#include "../lib/libtcapi.h"


#define MAXLEN_TCAPI_MSG		4097
#define DIGITAL_MAP_NUM				(8)
#define DIGITAL_MAP_PART_MAXLEN		(MAXLEN_TCAPI_MSG/DIGITAL_MAP_NUM)

#define TCAPI_PROCESS_OK	0

#define     WAN_PVC_NODE				"Wan_PVC%d"
#define     WAN_PVC_ENTRY_NODE			"Wan_PVC%d_Entry%d"
#define		WLAN_COMMON_NODE			"WLan_Common"
#define		WLAN_ENTRY_NODE				"WLan_Entry%d"
#define		ACCOUNT_ENTRY0_NODE			"Account_Entry0"
#define		LAN_ENTRY_NODE				"Lan_Entry%d"
#define		DHCPD_OPTION60_NODE			"Dhcpd_Option60"
#define		DHCPD_OPTION125_NODE		"Dhcpd_Option125"
#define		DHCPD_COMMON_NODE			"Dhcpd_Common"
#define		ACCESSLIMIT_COMMON_NODE			"Accesslimit_Common"
#define		ACCESSLIMIT_STB_NODE			"Accesslimit_Entry0"
#define		ACCESSLIMIT_CAMERA_NODE			"Accesslimit_Entry1"
#define		ACCESSLIMIT_COMPUTER_NODE		"Accesslimit_Entry2"
#define		ACCESSLIMIT_PHONE_NODE			"Accesslimit_Entry3"
#define		DMZ_ENTRY_NODE				"Dmz_Entry%d"
#define     VIRSERVER_SINGLE_ENTRY_NODE		"VirServer_Entry%d"
#define     VIRSERVER_ENTRY_NODE		"VirServer_Entry%d_Entry%d"

#define		VOIPDIGITMAP_ENTRY_NODE     "VoIPDigitMap_Entry"
#define		VOIPMEDIA_COMMON_NODE       "VoIPMedia_Common"
#define     VOIPBASIC_ENTRY_NODE		"VoIPBasic_Entry%d"
#define     VOIPH248_COMMON_NODE		"VOIPH248_Common"
#define     VOIPBASIC_COMMON_NODE		"VoIPBasic_Common"
#define     VOIPCALLCTRL_ENTRY_NODE		"VoIPCallCtrl_Entry%d"
#define     VOIPIMS_ENTRY_NODE			"VoIPIMS_Entry%d"
#define     VOIPH248_ENTRY_NODE			"VoIPH248_Entry%d"
#define     VOIPCODECS_ENTRY		"VoIPCodecs_Entry%d"
#define     VOIPADVANCED_ENTRY_NODE		"VoIPAdvanced_Entry%d"
#define	    VoIPADVANCED_COMMON_NODE	"VoIPAdvanced_Common"
#define     VOIPCODECS_PVC_ENTRY		"VoIPCodecs_PVC%d_Entry%d"
#define     VOIPCODECS_PVC				"VoIPCodecs_PVC%d"
#define     VOIPMEDIA_ENTRY_NODE		"VoIPMedia_Entry%d"

#define		TR069          	 			"TR069"
#define		VOICE          	 			"VOICE"
#define		INTERNET					"INTERNET"
#define     OTHER               		"OTHER"
#define		TR069_INTERNET   			"TR069_INTERNET"
#define		TR069_VOICE          	 	"TR069_VOICE"
#define		VOICE_INTERNET          	"VOICE_INTERNET"
#define		TR069_VOICE_INTERNET        "TR069_VOICE_INTERNET"

#define 	WAN_ENCAP_DYN			"0"
#define 	WAN_ENCAP_STATIC		"1"
#define 	WAN_ENCAP_PPP			"2"
#define 	WAN_ENCAP_BRIDGE		"3"

#define		WANIF_COMMON				"WanInfo_Common"
#define		SIMCARD_NODE				"SimCard_Entry"


#define SIM_CARD  12
#define SIM_CARD_WRITE				(1)
#define SIM_CARD_NOTIFY				(2)
#define SIM_CARD_OFF_LINE				(51)		//105001
#define SIM_CARD_WRITE_FAIL			(52)		//105002
#define SIM_CARD_UPDATE_FAIL			(53)		//105003
#define SIM_CARD_AUTH_DEVICE			(54)		//105004
#define SIM_CARD_AUTH_CARD			(55)		//105005
#define SIM_CARD_RESET_FAIL			(56)		//105006	


#define UPDATE_SIMCARD_RETRY_TIME	(3)
#define MAX_DYNAMIC_LAYER			(2)
#define MAX_PVC_NUM					(8)
#define MAX_SMUX_NUM				(8)
#define MAX_MODIFY_FLAG_NUM		(16)
#define MAX_VIRSERVER_NUM		(10)
#define MAX_WAN_IF_INDEX  (MAX_PVC_NUM*MAX_SMUX_NUM)
#define FAIL -1
#define SUCCESS 0

#define DEFAULT_MAX_STA_NUM "16"
//#define SIM_FILE_DIR "/var/temp/EFFile"
#define SIM_FILE_DIR "/var/sim/%02x"
#define SIM_FILE_DIR_TMP "/var/sim/tmp"

#define MAX_SIM_FILE_NUM (0xDA+1)

typedef char* (*SimGetNodeValueFunc)(int index[]);
typedef int (*SimSetNodeValueFunc)(int index[], char* value);

typedef struct _simFileNode{
	char* simAttr;
	int	nodeDynamic;
	SimGetNodeValueFunc getValueFunc;
	SimSetNodeValueFunc setValueFunc;
	char* nodeName;
	char* nodeAttr;
}simFileNode, *pSimFileNode;


typedef struct _simFiles{
	int fileIndex;
	int isModify;
	int dynamic;	
	int reserve;
	int isKeyPara;
	pSimFileNode pSimFile;
}simFiles;

typedef struct _CfgNodeSimMapping{
	char* cfgNode;
	char* files;
}CfgNodeSimMapping,*ptrCfgNodeSimMapping;

extern char gGetValue[MAXLEN_TCAPI_MSG];
extern int gRet;
extern char* getWanIndexValue(int index[]);
extern int get_wanindex_by_simcardwanindex(char *wan_if_name);

int get_wanindex_by_ifname(char *wan_if_name);
extern char* get_Attr_Active(char *nodeName, char *attrName);
extern int set_Attr_Active(char *nodeName, char *attrName, char *value);
extern char* get_Attr_EndIP(char *nodeName, char *attrName_start, char *attrName_count);
extern int set_Attr_EndIP(char *nodeName, char *attrName_start, char *attrName_count, char *value);

#endif /* _SIMCARDUTIL_LIB_H */

