#include "Global_res.h"
#include "cp_ctqos.h"


static NodeFunc Mode=
{
	NULL,
	NULL,
	getCTQoSModeValue, 
	setCTQoSModeValue, 
	getCTQoSModeAttribute, 
	setCTQoSModeAttribute, 
	NULL,
};

static NodeFunc Enable=
{
	NULL,
	NULL,
	getCTQoSEnableValue, 
	setCTQoSEnableValue, 
	getCTQoSEnableAttribute, 
	setCTQoSEnableAttribute, 
	NULL,
};

static NodeFunc Bandwidth=
{
	NULL,
	NULL,
	getCTQoSBandwidthValue, 
	setCTQoSBandwidthValue, 
	getCTQoSBandwidthAttribute, 
	setCTQoSBandwidthAttribute, 
	NULL,
};

static NodeFunc Plan=
{
	NULL,
	NULL,
	getCTQoSPlanValue, 
	setCTQoSPlanValue, 
	getCTQoSPlanAttribute, 
	setCTQoSPlanAttribute, 
	NULL,
};

static NodeFunc EnableForceWeight=
{
	NULL,
	NULL,
	getCTQoSEnableForceWeightValue, 
	setCTQoSEnableForceWeightValue, 
	getCTQoSEnableForceWeightAttribute, 
	setCTQoSEnableForceWeightAttribute, 
	NULL,
};

static NodeFunc EnableDSCPMark=
{
	NULL,
	NULL,
	getCTQoSEnableDSCPMarkValue, 
	setCTQoSEnableDSCPMarkValue, 
	getCTQoSEnableDSCPMarkAttribute, 
	setCTQoSEnableDSCPMarkAttribute, 
	NULL,
};

static NodeFunc Enable8021P=
{
	NULL,
	NULL,
	getCTQoSEnable8021PValue, 
	setCTQoSEnable8021PValue, 
	getCTQoSEnable8021PAttribute, 
	setCTQoSEnable8021PAttribute, 
	NULL,
};


/*********************************************** app node start ****************************************/
static NodeFunc AppName=
{
	NULL,
	NULL,
	getCTQoSAppNameValue, 
	setCTQoSAppNameValue, 
	getCTQoSAppNameAttribute, 
	setCTQoSAppNameAttribute, 
	NULL,
};

static NodeFunc AppClassQueue=
{
	NULL,
	NULL,
	getCTQoSAppClassQueueValue, 
	setCTQoSAppClassQueueValue, 
	getCTQoSAppClassQueueAttribute, 
	setCTQoSAppClassQueueAttribute, 
	NULL,
};

/*
	Parent Node: InternetGatewayDevice.X_CT-COM_UplinkQoS.
	Parent Node Table Location: cp_topnodetable.c 
*/
CwmpParameterStruct App[] = 
{
	{"AppName", 					StrW, 				NoLimite, 		&AppName, 					NULL},
	{"ClassQueue", 					UnsignIntW, 		NoLimite, 		&AppClassQueue, 			NULL},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc AppIndexFuncs=
{
	NULL,
	DeviceNodeDelAppObject,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	NULL
};

/*
       This Node: InternetGatewayDevice.X_CT-COM_UplinkQoS.App.{i}
	Parent Node: InternetGatewayDevice.QueueManagement.App.
	Parent Node Table Location: cp_ctqos_nodetable.c
*/
CwmpParameterStruct AppIndex[]= 
{
	{NULL, ObjectW, NoLimite, &AppIndexFuncs, App},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc AppFunc=
{
	DeviceNodeAddAppObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitAppSubTree,
};

/*********************************************** app node end ****************************************/



/*********************************************** classqueue node start ****************************************/
static NodeFunc CSFClassQueue=
{
	NULL,
	NULL,
	getCTQoSCSFClassQueueValue, 
	setCTQoSCSFClassQueueValue, 
	getCTQoSCSFClassQueueAttribute, 
	setCTQoSCSFClassQueueAttribute, 
	NULL,
};

static NodeFunc DSCPMarkValue=
{
	NULL,
	NULL,
	getCTQoSDSCPMarkValueValue, 
	setCTQoSDSCPMarkValueValue, 
	getCTQoSDSCPMarkValueAttribute, 
	setCTQoSDSCPMarkValueAttribute, 
	NULL,
};

static NodeFunc CSF8021PValue=
{
	NULL,
	NULL,
	getCTQoSCSF8021PValueValue, 
	setCTQoSCSF8021PValueValue, 
	getCTQoSCSF8021PValueAttribute, 
	setCTQoSCSF8021PValueAttribute, 
	NULL,
};


/*
	Parent Node: InternetGatewayDevice.X_CT-COM_UplinkQoS.Classification
	Parent Node Table Location: cp_topnodetable.c 
*/
static NodeFunc CSFType=
{
	NULL,
	NULL,
	getCTQoSCSFTypeValue, 
	setCTQoSCSFTypeValue, 
	getCTQoSCSFTypeAttribute, 
	setCTQoSCSFTypeAttribute, 
	NULL,
};

static NodeFunc Max=
{
	NULL,
	NULL,
	getCTQoSMaxValue, 
	setCTQoSMaxValue, 
	getCTQoSMaxAttribute, 
	setCTQoSMaxAttribute, 
	NULL,
};

static NodeFunc Min=
{
	NULL,
	NULL,
	getCTQoSMinValue, 
	setCTQoSMinValue, 
	getCTQoSMinAttribute, 
	setCTQoSMinAttribute, 
	NULL,
};

static NodeFunc ProtocolList=
{
	NULL,
	NULL,
	getCTQoSProtocolListValue, 
	setCTQoSProtocolListValue, 
	getCTQoSProtocolListAttribute, 
	setCTQoSProtocolListAttribute, 
	NULL,
};

CwmpParameterStruct Type[] = 
{
	{"Type", 					StrW, 		NoLimite, 		&CSFType, 				NULL},
	{"Max", 					StrW, 		NoLimite, 		&Max, 					NULL},
	{"Min", 					StrW, 		NoLimite, 		&Min, 					NULL},
	{"ProtocolList", 			StrW, 		NoLimite, 		&ProtocolList, 			NULL},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc TypeIndexFuncs=
{
	NULL,
	DeviceNodeDelTypeObject,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	NULL
};
/*
	Parent Node: InternetGatewayDevice.X_CT-COM_UplinkQoS.Classification.{i}.Type.
	Parent Node Table Location: cp_topnodetable.c 
*/
CwmpParameterStruct TypeIndex[]= 
{
	{NULL, ObjectW, NoLimite, &TypeIndexFuncs, Type},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc TypeFunc=
{
	DeviceNodeAddTypeObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitTypeSubTree,
};

/*
	Parent Node: InternetGatewayDevice.X_CT-COM_UplinkQoS.Classification
	Parent Node Table Location: cp_topnodetable.c 
*/
CwmpParameterStruct Classification[] = 
{
	{"ClassQueue", 					UnsignIntW, 		NoLimite, 		&CSFClassQueue, 			NULL},
	{"DSCPMarkValue", 				UnsignIntW, 		NoLimite, 		&DSCPMarkValue, 			NULL},
	{"802-1_P_Value", 				UnsignIntW, 		NoLimite, 		&CSF8021PValue, 			NULL},
	{"type", 						ObjectW, 			NoLimite, 		&TypeFunc,					NULL},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc ClassificationIndexFuncs=
{
	NULL,
	DeviceNodeDelClassificationObject,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	NULL
};
/*
       This Node: InternetGatewayDevice.X_CT-COM_UplinkQoS.Classification.{i}
	Parent Node: InternetGatewayDevice.QueueManagement.Classification.
	Parent Node Table Location: cp_ctqos_nodetable.c
*/
CwmpParameterStruct ClassificationIndex[]= 
{
	{NULL, ObjectW, NoLimite, &ClassificationIndexFuncs, Classification},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc ClassificationFunc=
{
	DeviceNodeAddClassificationObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitClassificationSubTree,
};
/*********************************************** classqueue node end ****************************************/


/*********************************************** PriorityQueue node start ****************************************/

static NodeFunc PQEnable=
{
	NULL,
	NULL,
	getCTQoSPQEnableValue, 
	setCTQoSPQEnableValue, 
	getCTQoSPQEnableAttribute, 
	setCTQoSPQEnableAttribute, 
	NULL,
};

static NodeFunc Priority=
{
	NULL,
	NULL,
	getCTQoSPriorityValue, 
	setCTQoSPriorityValue, 
	getCTQoSPriorityAttribute, 
	setCTQoSPriorityAttribute, 
	NULL,
};

static NodeFunc Weight=
{
	NULL,
	NULL,
	getCTQoSWeightValue, 
	setCTQoSWeightValue, 
	getCTQoSWeightAttribute, 
	setCTQoSWeightAttribute, 
	NULL,
};

/*
	Parent Node: InternetGatewayDevice.X_CT-COM_UplinkQoS.PriorityQueue.{i}
	Parent Node Table Location: cp_topnodetable.c 
*/
CwmpParameterStruct PriorityQueue[] = 
{
	{"Enable", 						BooleanW, 			NoLimite, 		&PQEnable, 				NULL},
	{"Priority", 					UnsignIntW, 		NoLimite, 		&Priority, 				NULL},
	{"Weight", 						UnsignIntW, 		NoLimite, 		&Weight, 				NULL},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc PriorityQueueIndexFuncs=
{
	NULL,
	DeviceNodeDelPriorityQueueObject,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	NULL
};

/*
       This Node: InternetGatewayDevice.X_CT-COM_UplinkQoS.Classification.{i}
	Parent Node: InternetGatewayDevice.QueueManagement.Classification.
	Parent Node Table Location: cp_ctqos_nodetable.c
*/
CwmpParameterStruct PriorityQueueIndex[]= 
{
	{NULL, ObjectW, NoLimite, &PriorityQueueIndexFuncs, PriorityQueue},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc PriorityQueueFunc=
{
	DeviceNodeAddPriorityQueueObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitPriorityQueueSubTree,
};
/*********************************************** PriorityQueue node end ******************************************/

/*
	Parent Node: InternetGatewayDevice.X_CT-COM_UplinkQoS.
	Parent Node Table Location: cp_topnodetable.c 
*/
CwmpParameterStruct X_CTCOM_UplinkQoS[] = 
{
	{"Mode", 					StrW, 				NoLimite, 		&Mode, 					NULL},
	{"Enable", 					BooleanW, 			NoLimite, 		&Enable,				NULL},
	{"Bandwidth", 				UnsignIntW, 		NoLimite, 		&Bandwidth,				NULL},
	{"Plan", 					StrW, 				NoLimite, 		&Plan,					NULL},
	{"EnableForceWeight", 		BooleanW, 			NoLimite, 		&EnableForceWeight,		NULL},
	{"EnableDSCPMark", 			BooleanW, 			NoLimite, 		&EnableDSCPMark,		NULL},
	{"Enable802-1_P", 			UnsignIntW, 		NoLimite, 		&Enable8021P,			NULL},
	{"App", 					ObjectW, 			NoLimite, 		&AppFunc,				NULL},
	{"Classification", 			ObjectW, 			NoLimite, 		&ClassificationFunc,	NULL},
	{"PriorityQueue", 			ObjectW, 			NoLimite, 		&PriorityQueueFunc,	NULL},
	{NULL, -1, -1, NULL, NULL}
};

