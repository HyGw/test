#include "Global_res.h"
#include "cp_managerment.h"

static NodeFunc KickURL=
{
	NULL,
	NULL,
	getKickURLValue,
	NULL, 
	getKickURLAttribute, 
	setKickURLAttribute, 
	NULL,
};

static NodeFunc URL=
{
	NULL,
	NULL,
	getURLValue,
	setURLValue, 
	getURLAttribute, 
	setURLAttribute, 
	NULL,
};

static NodeFunc Username=
{
	NULL,
	NULL,
	getUsernameValue,
	setUsernameValue, 
	getUsernameAttribute, 
	setUsernameAttribute, 
	NULL,
};

static NodeFunc Password=
{
	NULL,
	NULL,
	getPasswordValue, 
	setPasswordValue, 
	getPasswordAttribute, 
	setPasswordAttribute, 
	NULL,
};

static NodeFunc PeriodicInformEnable=
{
	NULL,
	NULL,
	getPeriodicInformEnableValue, 
	setPeriodicInformEnableValue, 
	getPeriodicInformEnableAttribute, 
	setPeriodicInformEnableAttribute, 
	NULL,
};

static NodeFunc PeriodicInformInterval=
{
	NULL,
	NULL,
	getPeriodicInformIntervalValue, 
	setPeriodicInformIntervalValue, 
	getPeriodicInformIntervalAttribute, 
	setPeriodicInformIntervalAttribute, 
	NULL,
};

static NodeFunc PeriodicInformTime=
{
	NULL,
	NULL,
	getPeriodicInformTimeValue, 
	setPeriodicInformTimeValue, 
	getPeriodicInformTimeAttribute, 
	setPeriodicInformTimeAttribute, 
	NULL,
};

static NodeFunc ParameterKey=
{
	NULL,
	NULL,
	getParameterKeyValue, 
	NULL, 
	getParameterKeyAttribute, 
	setParameterKeyAttribute, 
	NULL,
};

static NodeFunc ConnectionRequestURL=
{
	NULL,
	NULL,
	getConnectionRequestURLValue, 
	NULL, 
	getConnectionRequestURLAttribute, 
	setConnectionRequestURLAttribute, 
	NULL,
};

static NodeFunc ConnectionRequestUsername=
{
	NULL,
	NULL,
	getConnectionRequestUsernameValue, 
	setConnectionRequestUsernameValue, 
	getConnectionRequestUsernameAttribute, 
	setConnectionRequestUsernameAttribute, 
	NULL,
};

static NodeFunc ConnectionRequestPassword=
{
	NULL,
	NULL,
	getConnectionRequestPasswordValue, 
	setConnectionRequestPasswordValue, 
	getConnectionRequestPasswordAttribute, 
	setConnectionRequestPasswordAttribute, 
	NULL,
};

static NodeFunc UpgradesManaged=
{
	NULL,
	NULL,
	getUpgradesManagedValue, 
	setUpgradesManagedValue, 
	getUpgradesManagedAttribute, 
	setUpgradesManagedAttribute, 
	NULL,
};

#ifdef TR111
static NodeFunc ManageableDeviceNumberOfEntries=
{
	NULL,
	NULL,
	getmDevNumberOfEntriesValue, 
	NULL, 
	getmDevNumberOfEntriesAttribute, 
	setmDevNumberOfEntriesAttribute, 
	NULL,
};

static NodeFunc ManageableDeviceNotificationLimit=
{
	NULL,
	NULL,
	getmDevNotificationLimitValue, 
	setmDevNotificationLimitValue, 
	getmDevNotificationLimitAttribute, 
	setmDevNotificationLimitAttribute, 
	NULL,
};

static NodeFunc ManufacturerOUI_TR111=
{
	NULL,
	NULL,
	getmDevOUIValue, 
	NULL, 
	getmDevOUIAttribute, 
	NULL, 
	NULL,
};

static NodeFunc SerialNumber=
{
	NULL,
	NULL,
	getmDevSNValue, 
	NULL, 
	getmDevSNAttribute, 
	NULL, 
	NULL,
};

static NodeFunc ProductClass_TR111=
{
	NULL,
	NULL,
	getmDevClassValue, 
	NULL, 
	getmDevClassAttribute, 
	NULL, 
	NULL,
};

/*
	Parent Node: InternetGatewayDevice.ManagementServer.ManageableDevice.{i}.
	Parent Node Table Location: cp_managermet_nodetable.c
*/
CwmpParameterStruct ManageableDevice[] = 
{
	{"ManufacturerOUI", StrR, NoLimite, &ManufacturerOUI_TR111, NULL},
	{"SerialNumber", StrR, NoLimite, &SerialNumber, NULL},
	{"ProductClass", StrR, NoLimite, &ProductClass_TR111, NULL},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.ManagementServer.ManageableDevice.
	Parent Node Table Location: cp_managermet_nodetable.c
*/
CwmpParameterStruct ManageableDeviceIndex[]= 
{
	{NULL, ObjectR, NoLimite, NULL, ManageableDevice},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc ManageableDeviceDynFunc=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitManageableDeviceSubTree
};
#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
static NodeFunc BindInterface=
{
	NULL,
	NULL,
	getBindInterfaceValue, 
	setBindInterfaceValue, 
	getBindInterfaceAttribute, 
	setBindInterfaceAttribute, 
	NULL,
};

static NodeFunc RMSFlooding=
{
	NULL,
	NULL,
	getRMSFloodingValue, 
	setRMSFloodingValue, 
	getRMSFloodingAttribute, 
	setRMSFloodingAttribute, 
	NULL,
};

#endif


/*
	Parent Node: InternetGatewayDevice.ManagementServer.
	Parent Node Table Location: cp_topnodetable.c
*/
CwmpParameterStruct ManagementServer[] = 
{
	{"URL", StrW, NoLimite, &URL, NULL},	
	{"Username", StrW, NoLimite, &Username, NULL},
	{"Password", StrW, NoLimite, &Password, NULL},
	{"PeriodicInformEnable", BooleanW, NoLimite, &PeriodicInformEnable, NULL},
	{"PeriodicInformInterval", UnsignIntW, NoLimite, &PeriodicInformInterval, NULL},
	{"PeriodicInformTime", DateTimeTypeW, NoLimite, &PeriodicInformTime, NULL},
	{"ParameterKey", StrR, NoLimite, &ParameterKey, NULL},
	{"ConnectionRequestURL", StrR, NoLimite, &ConnectionRequestURL, NULL},
	{"ConnectionRequestUsername", StrW, NoLimite, &ConnectionRequestUsername, NULL},
	{"ConnectionRequestPassword", StrW, NoLimite, &ConnectionRequestPassword, NULL},
	{"UpgradesManaged", BooleanW, NoLimite, &UpgradesManaged, NULL},
#ifdef TR111
	{"ManageableDeviceNumberOfEntries", UnsignIntR, NoLimite, &ManageableDeviceNumberOfEntries, NULL},
	{"ManageableDeviceNotificationLimit", UnsignIntW, NoLimite, &ManageableDeviceNotificationLimit, NULL},
	{"ManageableDevice", ObjectR, NoLimite, &ManageableDeviceDynFunc, NULL},
#endif
	{"KickURL", StrR, NoLimite, &KickURL, NULL},	            //xmdai_nj
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
#if 0
	{X_CU"BindInterface", StrW, NoLimite, &BindInterface, NULL},
#endif
	{X_CU"RandomInformEnable", BooleanW, NoLimite, &RMSFlooding, NULL},
#endif

	{NULL, -1, -1, NULL, NULL}
};

