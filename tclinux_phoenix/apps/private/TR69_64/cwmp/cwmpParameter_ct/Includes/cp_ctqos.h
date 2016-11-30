#ifndef CP_CTQOS_H_
#define CP_CTQOS_H_
#include "cp_core.h"

#define 		MAX_NODE_NAME_SIZE			34
#define			MAX_BUF_LEN					64
#define 		MAX_APP_ENTRY_NUM			4
#define			MAX_TYPE_ENTRY_NUM			10
#define 		MAX_TYPE_NUM				10

#define			MAX_QUEUE_NUM				10


#define			QOS_COMMON					"QoS_Common"
#define 		QOS_ENTRY					"QoS_Entry%d"

#define			INTERNET_TR069				"INTERNET,TR069"
#define			INTERNET_TR069_VOIP			"INTERNET,TR069,VOIP"
#define			INTERNET_TR069_IPTV			"INTERNET,TR069,IPTV"
#define 		INTERNET_TR069_VOIP_IPTV	"INTERNET,TR069,VOIP,IPTV"
#define			OTHER						"OTHER"
#define 		INTERNET	"INTERNET"
#define			TR069	"TR069"
#define 		VOIP	"VOIP"
#define			IPTV	"IPTV"

#define			MAX_TYPE_NAME_LEN			20

/*
	Parent Node: InternetGatewayDevice.X_CT-COM_UplinkQoS.Classification.{i}.Type.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct TypeIndex[];

/*
       This Node: InternetGatewayDevice.X_CT-COM_UplinkQoS.App.{i}
	Parent Node: InternetGatewayDevice.QueueManagement.App.
	Parent Node Table Location: cp_ctqos_nodetable.c
*/
extern CwmpParameterStruct AppIndex[];

/*
       This Node: InternetGatewayDevice.X_CT-COM_UplinkQoS.Classification.{i}
	Parent Node: InternetGatewayDevice.QueueManagement.Classification.
	Parent Node Table Location: cp_ctqos_nodetable.c
*/
extern CwmpParameterStruct ClassificationIndex[];

/*
       This Node: InternetGatewayDevice.X_CT-COM_UplinkQoS.Classification.{i}
	Parent Node: InternetGatewayDevice.QueueManagement.Classification.
	Parent Node Table Location: cp_ctqos_nodetable.c
*/
extern CwmpParameterStruct PriorityQueueIndex[];

/* qos parameters */
int8 getCTQoSModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTQoSModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTQoSModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTQoSModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTQoSEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTQoSEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTQoSEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTQoSEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTQoSBandwidthValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTQoSBandwidthValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTQoSBandwidthAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTQoSBandwidthAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTQoSPlanValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTQoSPlanValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTQoSPlanAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTQoSPlanAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTQoSEnableForceWeightValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTQoSEnableForceWeightValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTQoSEnableForceWeightAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTQoSEnableForceWeightAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTQoSEnableDSCPMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTQoSEnableDSCPMarkValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTQoSEnableDSCPMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTQoSEnableDSCPMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTQoSEnable8021PValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTQoSEnable8021PValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTQoSEnable8021PAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTQoSEnable8021PAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


/* app parameters */
int8 getCTQoSAppNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTQoSAppNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTQoSAppNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTQoSAppNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTQoSAppClassQueueValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTQoSAppClassQueueValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTQoSAppClassQueueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTQoSAppClassQueueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/* classqueue parameters */
int8 getCTQoSCSFClassQueueValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTQoSCSFClassQueueValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTQoSCSFClassQueueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTQoSCSFClassQueueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTQoSDSCPMarkValueValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTQoSDSCPMarkValueValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTQoSDSCPMarkValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTQoSDSCPMarkValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTQoSCSF8021PValueValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTQoSCSF8021PValueValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTQoSCSF8021PValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTQoSCSF8021PValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/* type parameters in classqueue parameters */
int8 getCTQoSCSFTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTQoSCSFTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTQoSCSFTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTQoSCSFTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTQoSMaxValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTQoSMaxValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTQoSMaxAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTQoSMaxAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTQoSMinValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTQoSMinValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTQoSMinAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTQoSMinAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTQoSProtocolListValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTQoSProtocolListValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTQoSProtocolListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTQoSProtocolListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/* priority queue parameters */
int8 getCTQoSPQEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTQoSPQEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTQoSPQEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTQoSPQEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTQoSPriorityValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTQoSPriorityValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTQoSPriorityAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTQoSPriorityAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCTQoSWeightValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCTQoSWeightValue(void* value, struct DeviceNode*  deviceNode);
int8 getCTQoSWeightAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCTQoSWeightAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/* init node tree function */
int8 DeviceNodeAddTypeObject(uint32 *num, DeviceNode*  deviceNode);
int8 DeviceNodeDelTypeObject(uint32 num, DeviceNode*  deviceNode);
int	cwmpInitTypeSubTree(DeviceNodePtr deviceNode);

/* app inin func */
int8 DeviceNodeAddAppObject(uint32 *num, DeviceNode*  deviceNode);
int8 DeviceNodeDelAppObject(uint32 num, DeviceNode*  deviceNode);
int	cwmpInitAppSubTree(DeviceNodePtr deviceNode);

int8 DeviceNodeAddClassificationObject(uint32 *num, DeviceNode*  deviceNode);
int8 DeviceNodeDelClassificationObject(uint32 num, DeviceNode*  deviceNode);
int cwmpInitClassificationSubTree(DeviceNodePtr deviceNode);

int cwmpInitPriorityQueueSubTree(DeviceNodePtr deviceNode);
int8 DeviceNodeAddPriorityQueueObject(uint32 *num, DeviceNode*  deviceNode);
int8 DeviceNodeDelPriorityQueueObject(uint32 num, DeviceNode*  deviceNode);

/* lan and wan interface function */
#define 		IF_BEGIN					0
#define			IF_END						1
int chk_and_set_wanif(char *typeSetVal, int entry_index, int type_index, int flag);
int chk_and_set_lanif(char *typeSetVal, int entry_index, int type_index, int flag);					
void get_wan_if_name(char *typeGetVal, int entry_index, int type_index, int flag);
void get_lan_if_name(char *typeGetVal, int entry_index, int type_index, int flag);

#endif

