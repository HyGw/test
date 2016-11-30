#ifndef CP_X_CT_DEIVCE_PROCY_H_
#define CP_X_CT_DEIVCE_PROCY_H_

#include "cp_core.h"
#include "mxml.h"
#include <pthread.h>
#include <stdlib.h>

#define MAXLEN_NODE_NAME	32
#define MAXLEN_ATTR_NAME	32
#define MAXLEN_TCAPI_MSG		1024

#define SERVICE_CONFIG_PROFILE_MAXNUM	4
#define AP_MAX_WLANNUM			4
#define SOFTWARE_FILE_PROFILE_MAXNUM	4
#define TIME_WINDOW_LIST_MAXNUM			4

#define APWLAN_MAX_WEPKEY			4
#define APWLAN_MAX_PRE_SHARED_KEY	10
#define APWLAN_MAX_ASSOCIATED_DEV	1		//10


#define UPNPDM_DEVICELIST "DeviceList"
#define ADD_BJECT 		1 		
#define DEL_BJECT 		2	
#define NODE_NAME_LEN 256
#define RESPONSE_VLAUE_LEN 256

#define BMS_INVOKE 1
#define BMS_REBOOT_STATUS 2
#define BMS_DEVICE_STATUS 2
#define BMS_DOWNLOADSTATUS_OPERATIONSTATUS 2
#define BMS_DOWNLOAD_FILETYPE 2
#define BMS_DOWNLOAD_URL 3
#define BMS_DOWNLOAD_SOFTWAREVERSION 4
#define BMS_DOWNLOAD_HARDWAREVERSION 5
#define BMS_DOWNLOAD_CHANNEL 6
#define BMS_DOWNLOAD_TIMEWINDOW 7
#define BMS_DOWNLOAD_USERNAME 8
#define BMS_DOWNLOAD_PASSWORD 9 
#define BMS_DOWNLOAD_STATUS 10 
#define BMS_DOWNLOAD_OPERETIONID 11 

#define SOCK_SET 1
#define SOCK_GET 2

#define BMS_PATH "/userfs/BasicManagement_scdp.xml"

extern CwmpParameterStruct UPnPDMDeviceList_1[]; 
extern CwmpParameterStruct UPnPDMDeviceListIndex[];
extern CwmpParameterStruct X_CTCOM_ProxyDevice[];
extern CwmpParameterStruct UPnPDMDynamicNode[];
extern CwmpParameterStruct TimeWindowListIndex[];
extern CwmpParameterStruct FileProfileListIndex[];
extern CwmpParameterStruct ConfigProfileListIndex[];
extern CwmpParameterStruct APWLanIndex[];
extern CwmpParameterStruct APAssociatedDeviceIndex[];
extern CwmpParameterStruct APPreSharedKeyIndex[];
extern CwmpParameterStruct APWEPKeyeIndex[];

extern pthread_mutex_t value_change_lock;
extern uint8 inform_task_start;
extern int valueModify_Nofity;
extern void setUPnPDMDownloadFlag(boolean value);

enum bmsAction{
	BMS_REBOOT_INDEX=0,
	BMS_BASELINE_RESET_INDEX,
	BMS_DEVICE_STATUS_INDEX,
	BMS_CTCOM_DOWNLOAD_INDEX,
	BMS_CTCOM_DOWNLOAD_STATUS_INDEX
};


enum deviceInfo_node {
	DevInfo_FriendlyName = 0,
	DevInfo_DeviceType,
	DevInfo_ProvisioningCode,
	DevInfo_SoftwareVersion,
	DevInfo_SoftwareDescription,
	DevInfo_UpTime,	
	DevInfo_Mode,
	PhysicalDevice_HardwareVersion,
	PhysicalDevice_ManufacturerOUI,
	PhysicalDevice_ProductClass,
	PhysicalDevice_SerialNumber,
	Monitoring_CurrentTime,
	Monitoring_CPUUsage,
	Monitoring_MemoryUsage,
	ApplyMode_Mode,
	ApplyMode_UserMessage
};


typedef struct _BMS_Reboot
{
	boolean InvokeAction;
	char RebootStatus[16];
}BMS_Reboot,*pBMS_Reboot;

typedef struct _BMS_BaselineReset
{
	boolean InvokeAction;
}BMS_BaselineReset,*pBMS_BaselineReset;

typedef struct _BMS_GetDeviceStatus
{
	boolean InvokeAction;
	char DeviceStaus[16];
}BMS_GetDeviceStatus,*pBMS_GetDeviceStatus;

typedef struct _BMS_CTCOM_Download
{
	boolean InvokeAction;	
	int Channel;
	char* FileType;
	char* URL;
	char* SoftwareVersion;
	char* HardwareVersion;
	char* TimeWindow;
	char* Username;
	char* Password;
	uint32 DownloadStatus;
	uint32 OperationID;
}BMS_CTCOM_Download,*pBMS_CTCOM_Download;


typedef struct _BMS_CTCOM_GetDownloadStatus
{
	boolean InvokeAction;
	char OperationStatus[256];
}BMS_CTCOM_GetDownloadStatus,*pBMS_CTCOM_GetDownloadStatus;

typedef struct _BMS_Node
{
	BMS_Reboot Reboot;
	BMS_BaselineReset BaselineReset;
	BMS_GetDeviceStatus GetDeviceStatus;
	BMS_CTCOM_Download CTCOM_Download;
	BMS_CTCOM_GetDownloadStatus CTCOM_GetDownloadStatus;	
}BMS_Node,*pBMS_Node;

typedef struct _Dev_Node
{
	int index;				//for tr69 node tree
	int upnpInstance;		//for upnp dm
	pBMS_Node pBMS_Node;
	struct _Dev_Node *next;
}Dev_Node,*pDev_Node;


typedef struct _DeviceList_Header
{
	int num;
	pDev_Node pDevNode;
	pthread_mutex_t upnpLock;
}DeviceList_Header,*pDeviceList_Header;

typedef struct _UPnPAction
{
	int actType;		//0:Add, 1:del
	int devNum;
	struct _UPnPAction *next;
}UPnPAction,*pUPnPAction;

typedef struct _UPnPAction_Header
{
	int num;
	pUPnPAction *pUPnPAction;
	pthread_mutex_t upnpActLock;
}UPnPAction_Header,*pUPnPAction_Header;


int getTR69ErrCodeByUPnPDM();

int getDevInstance(char*buf,const char* delim, int* num, int instanceList[]);

char* getOperationStatus(int index);

int setUPnPDownloadInstance(int index);

int getUPnPDownloadInstance(void);

int getDeviceListIndexByDevNode(DeviceNodePtr pDevNode);


int getUPnPDMNodeByDevNode(DeviceNodePtr pDevNode, char* upnpNodeName);


void freeBMSNode(pBMS_Node pBMSNode);


void freeDevNode(pDev_Node pNode);

pDev_Node getDevNodeByIndex(int index);

pBMS_Node getBMSNodeByIndex(int index);

int selectCurUnusedIndex();

//need call os_free paravalue  to release memory after call getValueByName.
int getValueByName(char*buf, char* name, char** paravalue,const char* delim);

int getUPnPInstanceByTr69Index(int index);


int getIndexByUPnPInstance(int devnum);

int getBMSValueByIndex(int index, int bmsNameIndex1, int bmsNameIndex2, char* value);


int setBMSValueByIndex(int index, int bmsNameIndex1, int bmsNameIndex2, void* value);

int getValueOfXMLAttr(mxml_node_t* node,char* attrName, char* attrValue);

DeviceNodePtr findChildNodeByName(DeviceNodePtr pParentNode,char* name);

int printChildNode(DeviceNodePtr pParentNode);



DeviceNodePtr RegisterDynmicNodeByXMLNode(DeviceNodePtr pDevNode, mxml_node_t *xmlNode, char* name, int haveChild);


//DeviceNodePtr RegisterDynmicObjectNodeByName(DeviceNodePtr pParentNode, char* name);

int RegisterObjectNodeByName(DeviceNodePtr pDevNode,mxml_node_t *objectNode, char* name);

int InitDynamicUPnPDMNodeByPath(DeviceNodePtr pDevNode, char* filePath);

int InitDynamicUPnPDMNode(int index,DeviceNodePtr pDevNode);

int appendDevNode(int index,int devnum);

int AddUPnPDMDeviceListObject(uint32 devnum,  DeviceNode*  deviceNode);

int delDevNode(int index);

int DelUPnPDMDeviceListObject(uint32 index, DeviceNode*  deviceNode);

int ProcessUPnPDMMessage(char* buf);

int parseActionResponse( char* buffer, unsigned char opcode, int upnpInstance);

int	processBMSAction();

/* init UPnP DM DeviceList node */
int cwmpInitUPnPDMDeviceListTable(DeviceNodePtr deviceNode);



int8 DeviceNodeAddUPnPDMDynamicObject(uint32* num, DeviceNode*  deviceNode);
int8 DeviceNodeDelUPnPDMDynamicObject(uint32 num, DeviceNode*  deviceNode);

int cwmpInitConfigProfileTable(DeviceNodePtr deviceNode);
int8 DeviceNodeDelConfigProfileListObject(uint32 num, DeviceNode*  deviceNode);
int8 DeviceNodeAddConfigProfileListObject(uint32* num,  DeviceNode*  deviceNode);

int cwmpInitFileProfileTable(DeviceNodePtr deviceNode);
int8 DeviceNodeDelFileProfileListObject(uint32 num, DeviceNode*  deviceNode);
int8 DeviceNodeAddFileProfileListObject(uint32* num,  DeviceNode*  deviceNode);

int cwmpInitTimeWindowListTable(DeviceNodePtr deviceNode);
int8 DeviceNodeDelTimeWindowListObject(uint32 num, DeviceNode*  deviceNode);
int8 DeviceNodeAddTimeWindowListObject(uint32* num,  DeviceNode*  deviceNode);

int cwmpInitAPWLanTable(DeviceNodePtr deviceNode);
int8 DeviceNodeDelAPWLanListObject(uint32 num, DeviceNode*  deviceNode);
int8 DeviceNodeAddAPWLanListObject(uint32* num,  DeviceNode*  deviceNode);


int cwmpInitAPAssociatedDeviceTable( DeviceNodePtr deviceNode);
int cwmpInitAPWEPKeyTable( DeviceNodePtr deviceNode );
int cwmpInitAPPreSharedKey( DeviceNodePtr deviceNode );

int8 getDynamicNodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDynamicNodeValue(void* value, struct DeviceNode*  deviceNode);
int8 getUPnPDMNodeValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUPnPDMNodeValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDeviceListNum(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDeviceListNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceListNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getBMS_InvokeActionValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setBMS_InvokeActionValue(void* value, struct DeviceNode*  deviceNode);

int8 getBMS_RebootStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);

int8 getBMS_DeviceStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);

int8 getBMS_FileType(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setBMS_FileType(void* value, struct DeviceNode*  deviceNode);

int8 getBMS_URL(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setBMS_URL(void* value, struct DeviceNode*  deviceNode);

int8 getBMS_SoftwareVersion(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setBMS_SoftwareVersion(void* value, struct DeviceNode*  deviceNode);

int8 getBMS_HardwareVersion(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setBMS_HardwareVersion(void* value, struct DeviceNode*  deviceNode);

int8 getBMS_Channel(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setBMS_Channel(void* value, struct DeviceNode*  deviceN0ode);

int8 getBMS_TimeWindow(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setBMS_TimeWindow(void* value, struct DeviceNode*  deviceNode);

int8 getBMS_Username(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setBMS_Username(void* value, struct DeviceNode*  deviceNode);

int8 getBMS_Password(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setBMS_Password(void* value, struct DeviceNode*  deviceNode);

int8 getBMS_DownloadStatus(char** value, ParameterType* type, DeviceNode*  deviceNode);

int8 getBMS_OperationID(char** value, ParameterType* type, DeviceNode*  deviceNode);

int8 getBMS_OperationStatus(char** value, ParameterType* type, DeviceNode*  deviceNode);

int8 getRMSConfigOver(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRMSConfigOver(void* value, struct DeviceNode*  deviceNode);
int8 getRMSConfigOverAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRMSConfigOverAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFileProfileNum(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getFileProfileNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFileProfileNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFileProfileRule(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setFileProfileRule(void* value, struct DeviceNode*  deviceNode);
int8 getFileProfileRuleAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFileProfileRuleAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFileProfileFileType(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setFileProfileFileType(void* value, struct DeviceNode*  deviceNode);
int8 getFileProfileFileTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFileProfileFileTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFileProfileURL(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setFileProfileURL(void* value, struct DeviceNode*  deviceNode);
int8 getFileProfileURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFileProfileURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFileProfileSWVersion(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setFileProfileSWVersion(void* value, struct DeviceNode*  deviceNode);
int8 getFileProfileSWVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFileProfileSWVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFileProfileHWVersion(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setFileProfileHWVersion(void* value, struct DeviceNode*  deviceNode);
int8 getFileProfileHWVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFileProfileHWVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFileProfileChannel(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setFileProfileChannel(void* value, struct DeviceNode*  deviceNode);
int8 getFileProfileChannelAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFileProfileChannelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFileProfileUsername(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setFileProfileUsername(void* value, struct DeviceNode*  deviceNode);
int8 getFileProfileUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFileProfileUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFileProfilePassword(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setFileProfilePassword(void* value, struct DeviceNode*  deviceNode);
int8 getFileProfilePasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFileProfilePasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int8 getTimeWindowListNum(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTimeWindowListNum(void* value, struct DeviceNode*  deviceNode);
int8 getTimeWindowListNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTimeWindowListNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWindowStart(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWindowStart(void* value, struct DeviceNode*  deviceNode);
int8 getWindowStartAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWindowStartAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWindowEnd(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWindowEnd(void* value, struct DeviceNode*  deviceNode);
int8 getWindowEndAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWindowEndAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWindowMode(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWindowMode(void* value, struct DeviceNode*  deviceNode);
int8 getWindowModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWindowModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUserMessage(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUserMessage(void* value, struct DeviceNode*  deviceNode);
int8 getUserMessageAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUserMessageAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMaxRetries(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMaxRetries(void* value, struct DeviceNode*  deviceNode);
int8 getMaxRetriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMaxRetriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getConfigProfileNum(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getConfigProfileNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setConfigProfileNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSerProfileRMSConfigOver(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSerProfileRMSConfigOver(void* value, struct DeviceNode*  deviceNode);
int8 getSerProfileRMSConfigOverAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSerProfileRMSConfigOverAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getConfigurationMode(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setConfigurationMode(void* value, struct DeviceNode*  deviceNode);
int8 getConfigurationModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setConfigurationModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getConfigProfileRule(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setConfigProfileRule(void* value, struct DeviceNode*  deviceNode);
int8 getConfigProfileRuleAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setConfigProfileRuleAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getConfigurationPath(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setConfigurationPath(void* value, struct DeviceNode*  deviceNode);
int8 getConfigurationPathAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setConfigurationPathAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getConfigProvisionCode(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setConfigProvisionCode(void* value, struct DeviceNode*  deviceNode);
int8 getConfigProvisionCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setConfigProvisionCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAP_InternetNum(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAP_InternetNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAP_InternetNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAP_IPTVNum(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAP_IPTVNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAP_IPTVNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAP_VoIPNum(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAP_VoIPNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAP_VoIPNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSTB_InternetNum(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSTB_InternetNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSTB_InternetNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSTB_IPTVNum(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSTB_IPTVNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSTB_IPTVNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSTB_VoIPNum(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSTB_VoIPNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSTB_VoIPNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getScreen4_IPTVNum(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getScreen4_IPTVNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setScreen4_IPTVNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getScreen4_InternetNum(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getScreen4_InternetNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setScreen4_InternetNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getScreen4_VoIPNum(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getScreen4_VoIPNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setScreen4_VoIPNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHomeCenter_InternetNum(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getHomeCenter_InternetNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHomeCenter_InternetNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHomeCenter_NASNum(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getHomeCenter_NASNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHomeCenter_NASNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHomeCenter_IPTVNum(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getHomeCenter_IPTVNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHomeCenter_IPTVNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHomeCenter_VoIPNum(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getHomeCenter_VoIPNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHomeCenter_VoIPNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPlc_Num(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getPlc_NumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPlc_NumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAPWLanEnable(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setAPWLanEnable(void* value, struct DeviceNode*  deviceNode);
int8 getAPWLanEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPWLanEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if 0
int8 getAPWLanStatus(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAPWLanStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPWLanStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAPWLanBSSID(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAPWLanBSSIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPWLanBSSIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

int8 getAPWLanMaxBitRate(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setAPWLanMaxBitRate(void* value, struct DeviceNode*  deviceNode);
int8 getAPWLanMaxBitRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPWLanMaxBitRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAPWLanChannel(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setAPWLanChannel(void* value, struct DeviceNode*  deviceNode);
int8 getAPWLanChannelAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPWLanChannelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAPWLanSSID(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setAPWLanSSID(void* value, struct DeviceNode*  deviceNode);
int8 getAPWLanSSIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPWLanSSIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAPWLanBeaconType(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setAPWLanBeaconType(void* value, struct DeviceNode*  deviceNode);
int8 getAPWLanBeaconTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPWLanBeaconTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMACAddressControlEnabled(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMACAddressControlEnabled(void* value, struct DeviceNode*  deviceNode);
int8 getMACAddressControlEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMACAddressControlEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int8 getAPWLanSSIDHide(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setAPWLanSSIDHide(void* value, struct DeviceNode*  deviceNode);
int8 getAPWLanSSIDHideAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPWLanSSIDHideAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int8 getAPWLanPowerlevel(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setAPWLanPowerlevel(void* value, struct DeviceNode*  deviceNode);
int8 getAPWLanPowerlevelAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPWLanPowerlevelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAPWLanPowerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setAPWLanPowerValue(void* value, struct DeviceNode*  deviceNode);
int8 getAPWLanPowerValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPWLanPowerValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAPModuleEnable(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setAPModuleEnable(void* value, struct DeviceNode*  deviceNode);
int8 getAPModuleEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPModuleEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWPSKeyWord(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWPSKeyWord(void* value, struct DeviceNode*  deviceNode);
int8 getWPSKeyWordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWPSKeyWordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getChannelsInUse(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getChannelsInUseAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setChannelsInUseAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAPWLanStandard(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setAPWLanStandard(void* value, struct DeviceNode*  deviceNode);
int8 getAPWLanStandardAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPWLanStandardAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWEPEncryptionLevel(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWEPEncryptionLevel(void* value, struct DeviceNode*  deviceNode);
int8 getWEPEncryptionLevelAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWEPEncryptionLevelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWEPKeyIndex(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWEPKeyIndex(void* value, struct DeviceNode*  deviceNode);
int8 getWEPKeyIndexAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWEPKeyIndexAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getBasicAuthenticationMode(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setBasicAuthenticationMode(void* value, struct DeviceNode*  deviceNode);
int8 getBasicAuthenticationModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setBasicAuthenticationModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWPAEncryptionModes(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWPAEncryptionModes(void* value, struct DeviceNode*  deviceNode);
int8 getWPAEncryptionModesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWPAEncryptionModesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#if 0
int8 getAPTotalBytesSent(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAPTotalBytesSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPTotalBytesSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAPTotalBytesReceived(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAPTotalBytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPTotalBytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAPTotalPacketsSent(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAPTotalPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPTotalPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAPTotalPacketsReceived(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAPTotalPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPTotalPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAPTotalAssociations(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAPTotalAssociationsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPTotalAssociationsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

int8 getCTComRFBand(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTComRFBand(void* value, struct DeviceNode*  deviceNode);
int8 getCTComRFBandAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTComRFBandAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTComChannelWidth(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTComChannelWidth(void* value, struct DeviceNode*  deviceNode);
int8 getCTComChannelWidthAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTComChannelWidthAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTComGuardInterval(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTComGuardInterval(void* value, struct DeviceNode*  deviceNode);
int8 getCTComGuardIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTComGuardIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTComRetryTimeout(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTComRetryTimeout(void* value, struct DeviceNode*  deviceNode);
int8 getCTComRetryTimeoutAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTComRetryTimeoutAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTComVLAN(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTComVLAN(void* value, struct DeviceNode*  deviceNode);
int8 getCTComVLANAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTComVLANAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAPWEPKey(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setAPWEPKey(void* value, struct DeviceNode*  deviceNode);
int8 getAPWEPKeyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPWEPKeyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#if 0
int8 getAPAssociatedDeviceMACAddress(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAPAssociatedDeviceMACAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPAssociatedDeviceMACAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAPAssociatedDeviceIPAddress(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAPAssociatedDeviceIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPAssociatedDeviceIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAPAssociatedDeviceAuthenticationState(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAPAssociatedDeviceAuthenticationStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPAssociatedDeviceAuthenticationStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

int8 getAPPreSharedKey(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setAPPreSharedKey(void* value, struct DeviceNode*  deviceNode);
int8 getAPPreSharedKeyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPPreSharedKeyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAPPreKeyPassphrase(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setAPPreKeyPassphrase(void* value, struct DeviceNode*  deviceNode);
int8 getAPPreKeyPassphraseAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAPPreKeyPassphraseAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

