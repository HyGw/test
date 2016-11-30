#include "Global_res.h"
#include "cp_deviceregistration.h"

/*******************  InternetGatewayDevice.UDPEchoConfig. end ************************/

static NodeFunc DeviceStatus =
{
	NULL,
	NULL,
	getDeviceRegistrationDeviceStatusValue, 
	setDeviceRegistrationDeviceStatusValue, 
	getDeviceRegistrationDeviceStatusAttribute, 
	setDeviceRegistrationDeviceStatusAttribute, 
	NULL,
};

static NodeFunc DeviceResult =
{
	NULL,
	NULL,
	getDeviceRegistrationDeviceResultValue, 
	setDeviceRegistrationDeviceResultValue, 
	getDeviceRegistrationDeviceResultAttribute, 
	setDeviceRegistrationDeviceResultAttribute, 
	NULL,
};

static NodeFunc RetryTimes =
{
	NULL,
	NULL,
	getDeviceRegistrationRetryTimesValue, 
	setDeviceRegistrationRetryTimesValue, 
	getDeviceRegistrationRetryTimesAttribute, 
	setDeviceRegistrationRetryTimesAttribute, 
	NULL,
};

static NodeFunc RetryLimit =
{
	NULL,
	NULL,
	getDeviceRegistrationRetryLimitValue, 
	setDeviceRegistrationRetryLimitValue, 
	getDeviceRegistrationRetryLimitAttribute, 
	setDeviceRegistrationRetryLimitAttribute, 
	NULL,
};

static NodeFunc DeviceUserName =
{
	NULL,
	NULL,
	getDeviceRegistrationDeviceUserNameValue, 
	setDeviceRegistrationDeviceUserNameValue, 
	getDeviceRegistrationDeviceUserNameAttribute, 
	setDeviceRegistrationDeviceUserNameAttribute, 
	NULL,
};

static NodeFunc DeviceUserId =
{
	NULL,
	NULL,
	getDeviceRegistrationDeviceUserIdValue, 
	setDeviceRegistrationDeviceUserIdValue, 
	getDeviceRegistrationDeviceUserIdAttribute, 
	setDeviceRegistrationDeviceUserIdAttribute, 
	NULL,
};

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
static NodeFunc DeviceServiceNum=
{
	NULL,
	NULL,
	getServiceNumValue,
	setServiceNumValue,
	getServiceNumAttribute,
	setServiceNumAttribute,
	NULL,
};

static NodeFunc DeviceServiceName=
{
	NULL,
	NULL,
	getServiceNameValue,
	setServiceNameValue,
	getServiceNameAttribute,
	setServiceNameAttribute,
	NULL,
};
#endif

#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_DEVREG_PROV_CODE)
static NodeFunc DeviceProvisioningCode=
{
	NULL,
	NULL,
	getRegProvisioningCodeValue,
	setRegProvisioningCodeValue,
	getRegProvisioningCodeAttribute,
	setRegProvisioningCodeAttribute,
	NULL,
};
#endif

#if defined(TCSUPPORT_CT_ADSL_HN)
static NodeFunc RouteStatus=
{
	NULL,
	NULL,
	getRouteStatusValue,
	setRouteStatusValue,
	getRouteStatusAttribute,
	setRouteStatusAttribute,
	NULL,
};	
#endif

CwmpParameterStruct  DeviceRegistration[] =
{
	{"Status", UnsignIntW, NoLimite, &DeviceStatus, NULL},
	{"Result", UnsignIntW, NoLimite, &DeviceResult, NULL},
	{"Times", UnsignIntW, NoLimite, &RetryTimes, NULL},
	{"Limit", UnsignIntW, NoLimite, &RetryLimit, NULL},
	{"UserName", StrW, NoLimite, &DeviceUserName, NULL},
	{"UserId", StrW, NoLimite, &DeviceUserId, NULL},
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	{"ServiceNum", IntW, NoLimite, &DeviceServiceNum, NULL},
	{"ServiceName", StrW, NoLimite, &DeviceServiceName, NULL},
#endif
#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_DEVREG_PROV_CODE)
	{"ProvisioningCode", StrW, NoLimite, &DeviceProvisioningCode, NULL},
#endif
#if defined(TCSUPPORT_CT_ADSL_HN)
	{"RouteStatus", BooleanW, NoLimite, &RouteStatus, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}	
};
/*******************  InternetGatewayDevice.X_CT-COM_UserInfo. end ************************/