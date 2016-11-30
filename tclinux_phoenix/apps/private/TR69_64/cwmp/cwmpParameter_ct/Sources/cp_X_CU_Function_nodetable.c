#include "Global_res.h"
#include "cp_X_CU_Function.h"

static NodeFunc AdminName=
{
	NULL,
	NULL,
	getAdminNameValue,
	NULL, 
	getAdminNameAttribute, 
	setAdminNameAttribute, 
	NULL,
};
static NodeFunc AdminPassword=
{
	NULL,
	NULL,
	getAdminPasswordValue,
	setAdminPasswordValue, 
	getAdminPasswordAttribute, 
	setAdminPasswordAttribute, 
	NULL,
};
static NodeFunc AdminWebIP=
{
	NULL,
	NULL,
	getAdminWebIPValue,
	NULL, 
	getAdminWebIPAttribute, 
	setAdminWebIPAttribute, 
	NULL,
};
static NodeFunc UserName=
{
	NULL,
	NULL,
	getUserNameValue,
	NULL, 
	getUserNameAttribute, 
	setUserNameAttribute, 
	NULL,
};
static NodeFunc UserPasswd=
{
	NULL,
	NULL,
	getUserPasswordValue,
	NULL, 
	getUserPasswordAttribute, 
	setUserPasswordAttribute, 
	NULL,
};
static NodeFunc UserWebIP=
{
	NULL,
	NULL,
	getUserWebIPValue,
	NULL, 
	getUserWebIPAttribute, 
	setUserWebIPAttribute, 
	NULL,
};
static NodeFunc LogEnable=
{
	NULL,
	NULL,
	getLogEnableValue,
	setLogEnableValue, 
	getLogEnableAttribute, 
	setLogEnableAttribute,  
	NULL,
};
static NodeFunc LogLevel=
{
	NULL,
	NULL,
	getLogLevelValue,
	setLogLevelValue, 
	getLogLevelAttribute, 
	setLogLevelAttribute,  
	NULL,
};
static NodeFunc LogUploadEnable=
{
	NULL,
	NULL,
	getLogUploadEnableValue,
	setLogUploadEnableValue, 
	getLogUploadEnableAttribute, 
	setLogUploadEnableAttribute,  
	NULL,
};
static NodeFunc LogUploadInterval=
{
	NULL,
	NULL,
	getLogUploadIntervalValue,
	setLogUploadIntervalValue, 
	getLogUploadIntervalAttribute, 
	setLogUploadIntervalAttribute,  
	NULL,
};
static NodeFunc AlarmEnable=
{
	NULL,
	NULL,
	getAlarmEnableValue,
	setAlarmEnableValue, 
	getAlarmEnableAttribute, 
	setAlarmEnableAttribute,  
	NULL,
};
static NodeFunc AlarmLevel=
{
	NULL,
	NULL,
	getAlarmLevelValue,
	setAlarmLevelValue, 
	getAlarmLevelAttribute, 
	setAlarmLevelAttribute,  
	NULL,
};
static NodeFunc UploadServer=
{
	NULL,
	NULL,
	getUploadServerValue,
	setUploadServerValue, 
	getUploadServerAttribute, 
	setUploadServerAttribute,  
	NULL,
};
static NodeFunc CuLogUsername=
{
	NULL,
	NULL,
	getCuLogUsernameValue,
	setCuLogUsernameValue, 
	getCuLogUsernameAttribute, 
	setCuLogUsernameAttribute,  
	NULL,
};
static NodeFunc CuLogPassword=
{
	NULL,
	NULL,
	getCuLogPasswordValue,
	setCuLogPasswordValue, 
	getCuLogPasswordAttribute, 
	setCuLogPasswordAttribute,  
	NULL,
};
static NodeFunc NumberOfAlarmInfo=
{
	NULL,
	NULL,
	getNumberOfAlarmInfoValue,
	setNumberOfAlarmInfoValue, 
	getNumberOfAlarmInfoAttribute, 
	setNumberOfAlarmInfoAttribute,  
	NULL,
};
static NodeFunc AlarmID=
{
	NULL,
	NULL,
	getAlarmIDValue,
	NULL, 
	getAlarmIDAttribute, 
	setAlarmIDAttribute, 
	NULL,
};
static NodeFunc AlarmCode=
{
	NULL,
	NULL,
	getAlarmCodeValue,
	NULL, 
	getAlarmCodeAttribute, 
	setAlarmCodeAttribute, 
	NULL,
};
static NodeFunc AlarmStatus=
{
	NULL,
	NULL,
	getAlarmStatusValue,
	NULL, 
	getAlarmStatusAttribute, 
	setAlarmStatusAttribute, 
	NULL,
};
static NodeFunc PerceivedSeverity=
{
	NULL,
	NULL,
	getPerceivedSeverityValue,
	NULL, 
	getPerceivedSeverityAttribute, 
	setPerceivedSeverityAttribute, 
	NULL,
};
static NodeFunc AlarmRaisedTime=
{
	NULL,
	NULL,
	getAlarmRaisedTimeValue,
	NULL, 
	getAlarmRaisedTimeAttribute, 
	setAlarmRaisedTimeAttribute, 
	NULL,
};
static NodeFunc AlarmClearedTime=
{
	NULL,
	NULL,
	getAlarmClearedTimeValue,
	NULL, 
	getAlarmClearedTimeAttribute, 
	setAlarmClearedTimeAttribute, 
	NULL,
};
static NodeFunc AlarmDetail=
{
	NULL,
	NULL,
	getAlarmDetailValue,
	NULL, 
	getAlarmDetailAttribute, 
	setAlarmDetailAttribute, 
	NULL,
};

static NodeFunc AlarmInfoFuncs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitAlarmInfoSubTree,
};

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
static NodeFunc AdminaccountChange=
{
	NULL,
	NULL,
	getAdminaccountChangeValue,
	setAdminaccountChangeValue,
	getAdminaccountChangeAttribute, 
	setAdminaccountChangeAttribute, 
	NULL,
};

static NodeFunc PPPoEDiagState=
{
	NULL,
	NULL,
	getPPPoEDiagnosticsStateValue,
	setPPPoEDiagnosticsStateValue,
	getPPPoEDiagnosticsStateAttribute, 
	setPPPoEDiagnosticsStateAttribute, 
	NULL,
};

static NodeFunc PPPoEDiagUsername=
{
	NULL,
	NULL,
	getPPPoEDiagUsernameValue,
	setPPPoEDiagUsernameValue,
	getPPPoEDiagUsernameAttribute, 
	setPPPoEDiagUsernameAttribute, 
	NULL,
};

static NodeFunc PPPoEDiagPassword=
{
	NULL,
	NULL,
	getPPPoEDiagPasswordValue,
	setPPPoEDiagPasswordValue,
	getPPPoEDiagPasswordAttribute, 
	setPPPoEDiagPasswordAttribute, 
	NULL,
};

static NodeFunc PPPoEDiagVLANID=
{
	NULL,
	NULL,
	getPPPoEDiagVLANIDValue,
	setPPPoEDiagVLANIDValue,
	getPPPoEDiagVLANIDAttribute, 
	setPPPoEDiagVLANIDAttribute, 
	NULL,
};

static NodeFunc PPPoEDiagPort=
{
	NULL,
	NULL,
	getPPPoEDiagPortValue,
	setPPPoEDiagPortValue,
	getPPPoEDiagPortAttribute, 
	setPPPoEDiagPortAttribute, 
	NULL,
};

static NodeFunc PPPoEDiagAuthProtocol=
{
	NULL,
	NULL,
	getPPPoEDiagAuthProtocolValue,
	setPPPoEDiagAuthProtocolValue,
	getPPPoEDiagAuthProtocolAttribute, 
	setPPPoEDiagAuthProtocolAttribute, 
	NULL,
};

static NodeFunc PPPoEDiagRetryTimes=
{
	NULL,
	NULL,
	getPPPoEDiagRetryTimesValue,
	setPPPoEDiagRetryTimesValue,
	getPPPoEDiagRetryTimesAttribute, 
	setPPPoEDiagRetryTimesAttribute, 
	NULL,
};

static NodeFunc PPPoEDiagResult=
{
	NULL,
	NULL,
	getPPPoEDiagResultValue,
	NULL,
	getPPPoEDiagResultAttribute, 
	setPPPoEDiagResultAttribute, 
	NULL,
};

static NodeFunc PPPoEDiagSessionID=
{
	NULL,
	NULL,
	getPPPoEDiagSessionIDValue,
	NULL,
	getPPPoEDiagSessionIDAttribute, 
	setPPPoEDiagSessionIDAttribute, 
	NULL,
};

static NodeFunc PPPoEDiagExtIP=
{
	NULL,
	NULL,
	getPPPoEDiagExtIPValue,
	NULL,
	getPPPoEDiagExtIPAttribute, 
	setPPPoEDiagExtIPAttribute, 
	NULL,
};

static NodeFunc PPPoEDiagDefGW=
{
	NULL,
	NULL,
	getPPPoEDiagDefGWValue,
	NULL,
	getPPPoEDiagDefGWAttribute, 
	setPPPoEDiagDefGWAttribute, 
	NULL,
};

static NodeFunc PPPoEDiagWanInterface=
{
	NULL,
	NULL,
	getPPPoEDiagWanInterfaceValue,
	setPPPoEDiagWanInterfaceValue,
	getPPPoEDiagWanInterfaceAttribute, 
	setPPPoEDiagWanInterfaceAttribute, 
	NULL,
};

static NodeFunc PPPoEDiagLanInterface=
{
	NULL,
	NULL,
	getPPPoEDiagLanInterfaceValue,
	setPPPoEDiagLanInterfaceValue,
	getPPPoEDiagLanInterfaceAttribute, 
	setPPPoEDiagLanInterfaceAttribute, 
	NULL,
};


static NodeFunc IGMPSnoopingEnable=
{
	NULL,
	NULL,
	getSnoopingEnableValue, 
	setSnoopingEnableValue, 
	getSnoopingEnableAttribute, 
	setSnoopingEnableAttribute, 
	NULL,
};

static NodeFunc PacketofAPPFilterEnable=
{
	NULL,
	NULL,
	getFWPacketofAPPFilterEnableValue, 
	setFWPacketofAPPFilterEnableValue, 
	getFWPacketofAPPFilterEnableAttribute, 
	setFWPacketofAPPFilterEnableAttribute, 
	NULL,
};

static NodeFunc PortScanEnable=
{
	NULL,
	NULL,
	getFWPortScanEnableValue, 
	setFWPortScanEnableValue, 
	getFWPortScanEnableAttribute, 
	setFWPortScanEnableAttribute, 
	NULL,
};

static NodeFunc InvalidPacketDenyEnable=
{
	NULL,
	NULL,
	getFWInvalidPktDenyValue, 
	setFWInvalidPktDenyValue, 
	getFWInvalidPktDenyAttribute, 
	setFWInvalidPktDenyAttribute, 
	NULL,
};

static NodeFunc DoSEnable=
{
	NULL,
	NULL,
	getFWDoSEnableValue, 
	setFWDoSEnableValue, 
	getFWDoSEnableAttribute, 
	setFWDoSEnableAttribute, 
	NULL,
};

static NodeFunc telnetEnable=
{
	NULL,
	NULL,
	getTelnetEnableValue, 
	setTelnetEnableValue, 
	getTelnetEnableAttribute, 
	setTelnetEnableAttribute, 
	NULL,
};

static NodeFunc telnetUserName=
{
	NULL,
	NULL,
	getTelnetUserNameValue, 
	setTelnetUserNameValue, 
	getTelnetUserNameAttribute, 
	setTelnetUserNameAttribute, 
	NULL,
};

static NodeFunc telnetPassword=
{
	NULL,
	NULL,
	getTelnetPasswordValue, 
	setTelnetPasswordValue, 
	getTelnetPasswordAttribute, 
	setTelnetPasswordAttribute, 
	NULL,
};

static NodeFunc telnetPort=
{
	NULL,
	NULL,
	getTelnetPortValue, 
	setTelnetPortValue, 
	getTelnetPortAttribute, 
	setTelnetPortAttribute, 
	NULL,
};

static NodeFunc ddnsEnable=
{
	NULL,
	NULL,
	getDdnsEnableValue, 
	setDdnsEnableValue, 
	getDdnsEnableAttribute, 
	setDdnsEnableAttribute, 
	NULL,
};

static NodeFunc ddnsProvider=
{
	NULL,
	NULL,
	getDdnsProviderValue, 
	setDdnsProviderValue, 
	getDdnsProviderAttribute, 
	setDdnsProviderAttribute, 
	NULL,
};

static NodeFunc ddnsUsername=
{
	NULL,
	NULL,
	getDdnsUsernameValue, 
	setDdnsUsernameValue, 
	getDdnsUsernameAttribute, 
	setDdnsUsernameAttribute, 
	NULL,
};

static NodeFunc ddnsPassword=
{
	NULL,
	NULL,
	getDdnsPasswordValue, 
	setDdnsPasswordValue, 
	getDdnsPasswordAttribute, 
	setDdnsPasswordAttribute, 
	NULL,
};

static NodeFunc ddnsServicePort=
{
	NULL,
	NULL,
	getDdnsServicePortValue, 
	setDdnsServicePortValue, 
	getDdnsServicePortAttribute, 
	setDdnsServicePortAttribute, 
	NULL,
};

static NodeFunc ddnsDomainName=
{
	NULL,
	NULL,
	getDdnsDomainNameValue, 
	setDdnsDomainNameValue, 
	getDdnsDomainNameAttribute, 
	setDdnsDomainNameAttribute, 
	NULL,
};

static NodeFunc ddnsHostName=
{
	NULL,
	NULL,
	getDdnsHostNameValue, 
	setDdnsHostNameValue, 
	getDdnsHostNameAttribute, 
	setDdnsHostNameAttribute, 
	NULL,
};

static NodeFunc ddnsWANPath=
{
	NULL,
	NULL,
	getDdnsWANPathValue, 
	setDdnsWANPathValue, 
	getDdnsWANPathAttribute, 
	setDdnsWANPathAttribute, 
	NULL,
};

CwmpParameterStruct PPPOE_EMULATOR[] =
{
	{"DiagnosticsState", StrW, NoLimite, &PPPoEDiagState, NULL},
	{"Username", StrW, NoLimite, &PPPoEDiagUsername, NULL},
	{"Password", StrW, NoLimite, &PPPoEDiagPassword, NULL},
#if 0
	{"VLANID", UnsignIntW, NoLimite, &PPPoEDiagVLANID, NULL},
	{"Port", UnsignIntW, NoLimite, &PPPoEDiagPort, NULL},
#endif
	{"PPPAuthenticationProtocol", StrW, NoLimite, &PPPoEDiagAuthProtocol, NULL},
	{"RetryTimes", UnsignIntW, NoLimite, &PPPoEDiagRetryTimes, NULL},
	{"Result", StrR, NoLimite, &PPPoEDiagResult, NULL},
	{"PPPSessionID", UnsignIntR, NoLimite, &PPPoEDiagSessionID, NULL},
	{"ExternalIPAddress", StrR, NoLimite, &PPPoEDiagExtIP, NULL},
	{"DefaultGateway", StrR, NoLimite, &PPPoEDiagDefGW, NULL},
	{"WANInterface",StrW,NoLimite,&PPPoEDiagWanInterface,NULL},
	{"LANInterface",StrW,NoLimite,&PPPoEDiagLanInterface,NULL},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct IGMP[] =
{
	{"IGMPSnoopingEnable", BooleanW, NoLimite, &IGMPSnoopingEnable, NULL},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct Firewall[] =
{
	{"PacketofAPPFilterEnable", BooleanW, NoLimite, &PacketofAPPFilterEnable, NULL},
	{"PortScanEnable", BooleanW, NoLimite, &PortScanEnable, NULL},
	{"InvalidPacketDenyEnable", BooleanW, NoLimite, &InvalidPacketDenyEnable, NULL},
	{"DoSEnable", BooleanW, NoLimite, &DoSEnable, NULL},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct Telnet[] =
{
	{"Enable", BooleanW, NoLimite, &telnetEnable, NULL},
	{"UserName", StrW, NoLimite, &telnetUserName, NULL},
	{"Password", StrW, NoLimite, &telnetPassword, NULL},
	{"Port", IntW, NoLimite, &telnetPort, NULL},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct ddns[] =
{
	{"Enabled", BooleanW, NoLimite, &ddnsEnable, NULL},
	{"Provider", StrW, NoLimite, &ddnsProvider, NULL},
	{"Username", StrW, NoLimite, &ddnsUsername, NULL},	
	{"Password", StrW, NoLimite, &ddnsPassword, NULL},
	{"ServicePort", StrW, NoLimite, &ddnsServicePort, NULL},
	{"DomainName", StrW, NoLimite, &ddnsDomainName, NULL},
	{"HostName", StrW, NoLimite, &ddnsHostName, NULL},
	{"WANPath", StrW, NoLimite, &ddnsWANPath, NULL},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc dmzEnable=
{
	NULL,
	NULL,
	getdmzEnableValue, 
	setdmzEnableValue, 
	getdmzEnableAttribute, 
	setdmzEnableAttribute, 
	NULL,
};

static NodeFunc dmzHostIP=
{
	NULL,
	NULL,
	getdmzHostIPValue, 
	setdmzHostIPValue, 
	getdmzHostIPAttribute, 
	setdmzHostIPAttribute, 
	NULL,
};


static NodeFunc FactoryResetMode=
{
	NULL,
	NULL,
	getFactoryResetModeValue,
	setFactoryResetModeValue, 
	getFactoryResetModeAttribute, 
	setFactoryResetModeAttribute,  
	NULL,
};

CwmpParameterStruct dmz[] =
{
	{"DMZEnable", BooleanW, NoLimite, &dmzEnable, NULL},
	{"DMZHostIPAddress", StrW, NoLimite, &dmzHostIP, NULL},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct X_CU_FactoryResetMode[] =
{
	{"mode", UnsignIntW, NoLimite, &FactoryResetMode, NULL},
	{NULL, -1, -1, NULL, NULL}
};


#endif

CwmpParameterStruct X_CU_Web[] =
{
	{"AdminName", StrR, NoLimite, &AdminName, NULL},
	{"AdminPassword", StrW, NoLimite, &AdminPassword, NULL},
	{"AdminWebIP", StrR, NoLimite, &AdminWebIP, NULL},
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"AdminaccountChange", IntW, NoLimite, &AdminaccountChange, NULL},
	{"UserName", StrR, NoLimite, &UserName, NULL},
	{"UserPassword", StrR, NoLimite, &UserPasswd, NULL},
	{"UserWebIP", StrR, NoLimite, &UserWebIP, NULL},
#else
	{"UserName", StrR, NoLimite, &UserName, NULL},
	{"UserPasswd", StrR, NoLimite, &UserPasswd, NULL},
	{"UserWebIP", StrR, NoLimite, &UserWebIP, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};
CwmpParameterStruct AlarmInfo[] =
{
	{"AlarmID", UnsignIntR, NoLimite, &AlarmID, NULL},
	{"AlarmCode", UnsignIntR, NoLimite, &AlarmCode, NULL},
	{"AlarmStatus", UnsignIntR, NoLimite, &AlarmStatus, NULL},
	{"PerceivedSeverity", UnsignIntR, NoLimite, &PerceivedSeverity, NULL},
	{"AlarmRaisedTime", DateTimeTypeR, NoLimite, &AlarmRaisedTime, NULL},
	{"AlarmClearedTime", DateTimeTypeR, NoLimite, &AlarmClearedTime, NULL},
	{"AlarmDetail", StrR, NoLimite, &AlarmDetail, NULL},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct AlarmInfoIndex[]= 
{
	{NULL, ObjectR, NoLimite, NULL, AlarmInfo},
	{NULL, -1, -1, NULL, NULL}
};

CwmpParameterStruct X_CU_Log[] =
{
	{"LogEnable", BooleanW, NoLimite, &LogEnable, NULL},
	{"LogLevel", UnsignIntW, NoLimite, &LogLevel, NULL},
	{"LogUploadEnable", BooleanW, NoLimite, &LogUploadEnable, NULL},
	{"LogUploadInterval", UnsignIntW, NoLimite, &LogUploadInterval, NULL},
	{"AlarmEnable", BooleanW, NoLimite, &AlarmEnable, NULL},
	{"AlarmLevel", UnsignIntW, NoLimite, &AlarmLevel, NULL},
	{"UploadServer", StrW, NoLimite, &UploadServer, NULL},
	{"Username", StrW, NoLimite, &CuLogUsername, NULL},
	{"Password", StrW, NoLimite, &CuLogPassword, NULL},
	{"NumberOfAlarmInfo", UnsignIntW, NoLimite, &NumberOfAlarmInfo, NULL},
	{"AlarmInfo", ObjectR, NoLimite, &AlarmInfoFuncs, NULL},
	{NULL, -1, -1, NULL, NULL}
};
/*
	Parent Node: InternetGatewayDevice.UserInterface.
	Parent Node Table Location: cp_topnodetable.c
*/
CwmpParameterStruct X_CU_Function[] = 
{
	{"Web", ObjectR, NoLimite, NULL, X_CU_Web},	
	{"Log", ObjectR, NoLimite, NULL, X_CU_Log},	
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"IGMP", ObjectR, NoLimite, NULL, IGMP},
	{"ALG", ObjectR, NoLimite, NULL, ALGAbility},
	{"Firewall", ObjectR, NoLimite, NULL, Firewall},
	{"Telnet", ObjectR, NoLimite, NULL, Telnet},
	{"DDNS", ObjectR, NoLimite, NULL, ddns},
	{"DMZ", ObjectR, NoLimite, NULL, dmz},
	{"UplinkQoS", ObjectR, NoLimite, NULL, X_CTCOM_UplinkQoS},
	{"PPPOE_EMULATOR", ObjectR, NoLimite, NULL, PPPOE_EMULATOR},
	{"FactoryResetMode", ObjectR, NoLimite, NULL, X_CU_FactoryResetMode},
#endif
	{NULL, -1, -1, NULL, NULL}
};

