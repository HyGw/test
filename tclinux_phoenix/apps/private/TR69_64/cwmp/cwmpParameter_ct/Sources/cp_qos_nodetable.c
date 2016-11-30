#include "Global_res.h"
#include "cp_qos.h"

/*Node Functions*/
static NodeFunc QueueConfig=
{
	NULL,
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	cwmpInitQoSQueueSubTree,
};

static NodeFunc ClassificationConfig=
{
	addClassificationObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitQoSClassificationSubTree,
};

static NodeFunc PolicerConfig=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitQoSPolicerSubTree,
};


/*Queue Funcs*/
static NodeFunc QueueInterface=
{
	NULL,
	NULL,
	getQoSQueueInterfaceValue, 
	NULL, 
	getQoSQueueInterfaceAttribute, 
	setQoSQueueInterfaceAttribute, 
	NULL,
};

static NodeFunc DropAlgorithm=
{
	NULL,
	NULL,
	getQoSDropAlgorithmValue, 
	NULL, 
	getQoSDropAlgorithmAttribute, 
	setQoSDropAlgorithmAttribute, 
	NULL,
};

static NodeFunc SchedulerAlgorithm=
{
	NULL,
	NULL,
	getQoSSchedulerAlgorithmValue, 
	NULL, 
	getQoSSchedulerAlgorithmAttribute, 
	setQoSSchedulerAlgorithmAttribute, 
	NULL,
};

static NodeFunc QueueEnable=
{
	NULL,
	NULL,
	getQoSQueueEnableValue, 
	NULL, 
	getQoSQueueEnableAttribute, 
	setQoSQueueEnableAttribute, 
	NULL,
};


/*Classification Funcs*/
static NodeFunc ClassificationIndexFuncs=
{
	NULL,
	deleteQoSRuleObject,
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL,
};

static NodeFunc Protocol=
{
	NULL,
	NULL,
	getQoSProtocolValue, 
	setQoSProtocolValue, 
	getQoSProtocolAttribute, 
	setQoSProtocolAttribute, 
	NULL,
};

static NodeFunc DSCPMark=
{
	NULL,
	NULL,
	getQoSDSCPMarkValue, 
	setQoSDSCPMarkValue, 
	getQoSDSCPMarkAttribute, 
	setQoSDSCPMarkAttribute, 
	NULL,
};

static NodeFunc ForwardingPolicy=
{
	NULL,
	NULL,
	getQoSForwardingPolicyValue, 
	setQoSForwardingPolicyValue, 
	getQoSForwardingPolicyAttribute, 
	setQoSForwardingPolicyAttribute, 
	NULL,
};

static NodeFunc ClassQueue=
{
	NULL,
	NULL,
	getQoSClassQueueValue, 
	setQoSClassQueueValue, 
	getQoSClassQueueAttribute, 
	setQoSClassQueueAttribute, 
	NULL,
};

static NodeFunc ClassPolicer=
{
	NULL,
	NULL,
	getQoSClassPolicerValue, 
	setQoSClassPolicerValue, 
	getQoSClassPolicerAttribute, 
	setQoSClassPolicerAttribute, 
	NULL,
};

static NodeFunc ClassificationEnable=
{
	NULL,
	NULL,
	getQoSClassificationEnableValue, 
	setQoSClassificationEnableValue, 
	getQoSClassificationEnableAttribute, 
	setQoSClassificationEnableAttribute, 
	NULL,
};

static NodeFunc DestIP=
{
	NULL,
	NULL,
	getQosDestIPValue, 
	setQosDestIPValue, 
	getQosDestIPAttribute, 
	setQosDestIPAttribute, 
	NULL,
};

static NodeFunc DestMask=
{
	NULL,
	NULL,
	getQosDestMaskValue, 
	setQosDestMaskValue, 
	getQosDestMaskAttribute, 
	setQosDestMaskAttribute, 
	NULL,
};

static NodeFunc SourceIP=
{
	NULL,
	NULL,
	getQosSourceIPValue, 
	setQosSourceIPValue, 
	getQosSourceIPAttribute, 
	setQosSourceIPAttribute, 
	NULL,
};

static NodeFunc SourceMask=
{
	NULL,
	NULL,
	getQosSourceMaskValue, 
	setQosSourceMaskValue, 
	getQosSourceMaskAttribute, 
	setQosSourceMaskAttribute, 
	NULL,
};

static NodeFunc DestPort=
{
	NULL,
	NULL,
	getQosDestPortValue, 
	setQosDestPortValue, 
	getQosDestPortAttribute, 
	setQosDestPortAttribute, 
	NULL,
};

static NodeFunc SourcePort=
{
	NULL,
	NULL,
	getQosSourcePortValue, 
	setQosSourcePortValue, 
	getQosSourcePortAttribute, 
	setQosSourcePortAttribute, 
	NULL,
};

static NodeFunc SourceMACAddress=
{
	NULL,
	NULL,
	getQosSourceMACAddressValue, 
	setQosSourceMACAddressValue, 
	getQosSourceMACAddressAttribute, 
	setQosSourceMACAddressAttribute, 
	NULL,
};

static NodeFunc DestMACAddress=
{
	NULL,
	NULL,
	getQosDestMACAddressValue, 
	setQosDestMACAddressValue, 
	getQosDestMACAddressAttribute, 
	setQosDestMACAddressAttribute, 
	NULL,
};


/*Policer Funcs*/
static NodeFunc PolicerEnable=
{
	NULL,
	NULL,
	getQoSPolicerEnableValue, 
	setQoSPolicerEnableValue, 
	getQoSPolicerEnableAttribute, 
	setQoSPolicerEnableAttribute, 
	NULL,
};


/*-------------------Queue----------------------------*/
/*
	Parent Node: InternetGatewayDevice.QueueManagement.Queue.{i}
	Parent Node Table Location: cp_qos_nodetable.c
*/
CwmpParameterStruct Queue[]= 
{
	{"QueueInterface", StrR, NoLimite, &QueueInterface, NULL},	
	{"DropAlgorithm", StrR, NoLimite, &DropAlgorithm, NULL},
	{"SchedulerAlgorithm", StrR, NoLimite, &SchedulerAlgorithm, NULL},
	{"QueueEnable", BooleanR, NoLimite, &QueueEnable, NULL},	
	{NULL, -1, -1, NULL, NULL}
};

/*
       This Node: InternetGatewayDevice.QueueManagement.Queue.{i}
	Parent Node: InternetGatewayDevice.QueueManagement.Queue.
	Parent Node Table Location: cp_qos_nodetable.c
*/
CwmpParameterStruct QueueIndex[]= 
{
	{NULL, ObjectR, NoLimite, NULL, Queue},
	{NULL, -1, -1, NULL, NULL}
};


/*-------------------Classification----------------------------*/
/*
	Parent Node: InternetGatewayDevice.QueueManagement.Classification.{i}
	Parent Node Table Location: cp_qos_nodetable.c
*/
CwmpParameterStruct Classification[]= 
{
	{"Protocol", IntW, NoLimite, &Protocol, NULL},	
	{"DSCPMark", IntW, NoLimite, &DSCPMark, NULL},
	{"ForwardingPolicy", UnsignIntW, NoLimite, &ForwardingPolicy, NULL},
	{"ClassQueue", IntW, NoLimite, &ClassQueue, NULL},
	{"ClassPolicer", IntW, NoLimite, &ClassPolicer, NULL},	
	{"ClassificationEnable", BooleanW, NoLimite, &ClassificationEnable, NULL},
	{"DestIP", StrW, NoLimite, &DestIP, NULL},
	{"DestMask", StrW, NoLimite, &DestMask, NULL},
	{"SourceIP", StrW, NoLimite, &SourceIP, NULL},
	{"SourceMask", StrW, NoLimite, &SourceMask, NULL},
	{"DestPort", IntW, NoLimite, &DestPort, NULL},
	{"SourcePort", IntW, NoLimite, &SourcePort, NULL},
	{"SourceMACAddress", StrW, NoLimite, &SourceMACAddress, NULL},
	{"DestMACAddress", StrW, NoLimite, &DestMACAddress, NULL},
	{NULL, -1, -1, NULL, NULL}
};

/*
       This Node: InternetGatewayDevice.QueueManagement.Classification.{i}
	Parent Node: InternetGatewayDevice.QueueManagement.Classification.
	Parent Node Table Location: cp_qos_nodetable.c
*/
CwmpParameterStruct ClassificationIndex[]= 
{
	{NULL, ObjectW, NoLimite, &ClassificationIndexFuncs, Classification},
	{NULL, -1, -1, NULL, NULL}
};


/*-------------------Policer----------------------------*/
/*
	Parent Node: InternetGatewayDevice.QueueManagement.Policer.{i}
	Parent Node Table Location: cp_qos_nodetable.c
*/
CwmpParameterStruct Policer[]= 
{
	{"PolicerEnable", BooleanW, NoLimite, &PolicerEnable, NULL},	
	{NULL, -1, -1, NULL, NULL}
};

/*
       This Node: InternetGatewayDevice.QueueManagement.Policer.{i}
	Parent Node: InternetGatewayDevice.QueueManagement.Classification.
	Parent Node Table Location: cp_qos_nodetable.c
*/
CwmpParameterStruct PolicerIndex[]= 
{
	{NULL, ObjectR, NoLimite, NULL, Policer},
	{NULL, -1, -1, NULL, NULL}
};


/*
	Parent Node: InternetGatewayDevice.QueueManagement.
	Parent Node Table Location: cp_topnodetable.c
*/
CwmpParameterStruct QueueManagement[] = 
{
	{"Queue", ObjectR, NoLimite, &QueueConfig, NULL},	
	{"Classification", ObjectW, NoLimite, &ClassificationConfig, NULL},
	{"Policer", ObjectR, NoLimite, &PolicerConfig, NULL},
	
	{NULL, -1, -1, NULL, NULL}
};
