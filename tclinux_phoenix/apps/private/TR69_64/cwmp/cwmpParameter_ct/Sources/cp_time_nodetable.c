#include "Global_res.h"
#include "cp_time.h"

static NodeFunc NTPServer1=
{
	NULL,
	NULL,
	getTimeNTPServer1Value,
	setTimeNTPServer1Value, 
	getTimeNTPServer1Attribute, 
	setTimeNTPServer1Attribute, 
	NULL,
};


static NodeFunc CurLocalTime=
{
	NULL,
	NULL,
	getTimeCurrentLocalTimeValue, 
	NULL, 
	getTimeCurrentLocalTimeAttribute, 
	setTimeCurrentLocalTimeAttribute, 
	NULL,
};

static NodeFunc LocalTimeZone=
{
	NULL,
	NULL,
	getTimeLocalTimeZoneValue, 
	setTimeLocalTimeZoneValue, 
	getTimeLocalTimeZoneAttribute, 
	setTimeLocalTimeZoneAttribute, 
	NULL,
};

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER) || defined(TCSUPPORT_CT_PON_GD)
static NodeFunc LocalTimeZoneName=
{
	NULL,
	NULL,
	getTimeLocalTimeZoneNameValue, 
	setTimeLocalTimeZoneNameValue, 
	getTimeLocalTimeZoneNameAttribute, 
	setTimeLocalTimeZoneNameAttribute, 
	NULL,
};
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
static NodeFunc NTPServer2=
{
	NULL,
	NULL,
	getTimeNTPServer2Value,
	setTimeNTPServer2Value, 
	getTimeNTPServer2Attribute, 
	setTimeNTPServer2Attribute, 
	NULL,
};

static NodeFunc NTPServer3=
{
	NULL,
	NULL,
	getTimeNTPServer3Value,
	setTimeNTPServer3Value, 
	getTimeNTPServer3Attribute, 
	setTimeNTPServer3Attribute, 
	NULL,
};
static NodeFunc NTPServer4=
{
	NULL,
	NULL,
	getTimeNTPServer4Value,
	setTimeNTPServer4Value, 
	getTimeNTPServer4Attribute, 
	setTimeNTPServer4Attribute, 
	NULL,
};

static NodeFunc NTPServer5=
{
	NULL,
	NULL,
	getTimeNTPServer5Value,
	setTimeNTPServer5Value, 
	getTimeNTPServer5Attribute, 
	setTimeNTPServer5Attribute, 
	NULL,
};

static NodeFunc X_CTCOM_NTPInterval=
{
	NULL,
	NULL,
	getNTPIntervalValue,
	setNTPIntervalValue, 
	getNTPIntervalAttribute, 
	setNTPIntervalAttribute, 
	NULL,
};

static NodeFunc X_CTCOM_NTPServerType=
{
	NULL,
	NULL,
	getNTPServerTypeValue,
	setNTPServerTypeValue, 
	getNTPServerTypeAttribute, 
	setNTPServerTypeAttribute, 
	NULL,
};

#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER) || defined(TCSUPPORT_CT_PON_GD)
static NodeFunc NTPEnable=
{
	NULL,
	NULL,
	getNTPEnableValue, 
	setNTPEnableValue, 
	getNTPEnableAttribute, 
	setNTPEnableAttribute, 
	NULL,
};
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
static NodeFunc NTPStatus=
{
    NULL,
    NULL,
    getNTPStatusValue,
    NULL,
    getNTPStatusAttribute,
    setNTPStatusAttribute,
    NULL,
};
#endif

/*
	Parent Node: InternetGatewayDevice.Time.
	Parent Node Table Location: cp_topnodetable.c
*/
CwmpParameterStruct Time[] = 
{
	{"NTPServer1", StrW, NoLimite, &NTPServer1, NULL},	
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	{"NTPServer2", StrW, NoLimite, &NTPServer2, NULL},
	{"NTPServer3", StrW, NoLimite, &NTPServer3, NULL},
	{"NTPServer4", StrW, NoLimite, &NTPServer4, NULL},
	{"NTPServer5", StrW, NoLimite, &NTPServer5, NULL},
	{X_CT_COM"NTPInterval", UnsignIntW, NoLimite, &X_CTCOM_NTPInterval, NULL},
	{X_CT_COM"NTPServerType", UnsignIntW, NoLimite, &X_CTCOM_NTPServerType, NULL},
#endif
	{"CurrentLocalTime", DateTimeTypeR, NoLimite, &CurLocalTime, NULL},
	{"LocalTimeZone", StrW, NoLimite, &LocalTimeZone, NULL},
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER) || defined(TCSUPPORT_CT_PON_GD)
	{"Enable", BooleanW, NoLimite, &NTPEnable, NULL},
	{"LocalTimeZoneName", StrW, NoLimite, &LocalTimeZoneName, NULL},
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"Status", StrR, NoLimite, &NTPStatus, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};
