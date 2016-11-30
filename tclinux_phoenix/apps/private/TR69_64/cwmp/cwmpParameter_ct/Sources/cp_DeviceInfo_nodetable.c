#include "Global_res.h"
#include "cp_DeviceInfo.h"
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
#include "ct_IPPingDiagnostics.h"
#endif

/*here we add "_DevInfo" for avoiding name conflict*/
static NodeFunc Manufacturer_DevInfo=
{
	NULL,
	NULL,
	getManufacturerValue, 
	NULL, 
	getManufacturerAttribute, 
	setManufacturerAttribute, 
	NULL,
};

static NodeFunc ManufacturerOUI_DevInfo=
{
	NULL,
	NULL,
	getManufacturerOUIValue, 
	NULL, 
	getManufacturerOUIAttribute, 
	setManufacturerOUIAttribute, 
	NULL,
};

static NodeFunc ModelName_DevInfo=
{
	NULL,
	NULL,
	getModelNameValue, 
	NULL, 
	getModelNameAttribute, 
	setModelNameAttribute, 
	NULL,
};

static NodeFunc Description_DevInfo=
{
	NULL,
	NULL,
	getDescriptionValue, 
	NULL, 
	getDescriptionAttribute, 
	setDescriptionAttribute, 
	NULL,
};

static NodeFunc ProductClass_DevInfo=
{
	NULL,
	NULL,
	getProductClassValue, 
	NULL, 
	getProductClassAttribute, 
	setProductClassAttribute, 
	NULL,
};

static NodeFunc SerialNumber_DevInfo=
{
	NULL,
	NULL,
	getSerialNumberValue, 
	NULL, 
	getSerialNumberAttribute, 
	setSerialNumberAttribute, 
	NULL,
};

static NodeFunc HardwareVersion_DevInfo=
{
	NULL,
	NULL,
	getHardwareVersionValue, 
	NULL, 
	getHardwareVersionAttribute, 
	setHardwareVersionAttribute, 
	NULL,
};

static NodeFunc SoftwareVersion_DevInfo=
{
	NULL,
	NULL,
	getSoftwareVersionValue, 
	NULL, 
	getSoftwareVersionAttribute, 
	setSoftwareVersionAttribute, 
	NULL,
};

static NodeFunc SpecVersion_DevInfo=
{
	NULL,
	NULL,
	getSpecVersionValue, 
	NULL, 
	getSpecVersionAttribute, 
	setSpecVersionAttribute, 
	NULL,
};

static NodeFunc ProvisioningCode_DevInfo=
{
	NULL,
	NULL,
	getProvisioningCodeValue, 
	setProvisioningCodeValue, 
	getProvisioningCodeAttribute, 
	setProvisioningCodeAttribute, 
	NULL,
};

static NodeFunc UpTime=
{
	NULL,
	NULL,
	getUpTimeValue, 
	NULL, 
	getUpTimeAttribute, 
	setUpTimeAttribute, 
	NULL,
};

static NodeFunc DeviceLog=
{
	NULL,
	NULL,
	getDeviceLogValue, 
	NULL, 
	getDeviceLogAttribute, 
	setDeviceLogAttribute, 
	NULL,
};

static NodeFunc Name=
{
	NULL,
	NULL,
	getVendorCfgFileNameValue, 
	NULL, 
	getVendorCfgFileNameAttribute, 
	setVendorCfgFileNameAttribute, 
	NULL,
};

static NodeFunc VendorCfgFile_Des=
{
	NULL,
	NULL,
	getVendorCfgFileDescripValue, 
	NULL, 
	getVendorCfgFileDescripAttribute, 
	setVendorCfgFileDescripAttribute, 
	NULL,
};

#if defined(TCSUPPORT_CT_RECONNECT_NODE)
#if !defined(TCSUPPORT_C1_CUC)
static NodeFunc Enable=
{
	NULL,
	NULL,
	getEnableValue, 
	setEnableValue, 
	getEnableAttribute, 
	setEnableAttribute, 
	NULL,
};

CwmpParameterStruct ReConnect_DevInfo[] =
{
	{"Enable", BooleanW, NoLimite, &Enable, NULL},
	{NULL, -1, -1, NULL, NULL}
};
#endif
#endif
/*
	Parent Node: InternetGatewayDevice.DeviceInfo.VendorConfigFile.1.
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
CwmpParameterStruct VendorConfigFile[] =
{
	{"Name", StrR, NoLimite, &Name, NULL},
	{"Description", StrR, NoLimite, &VendorCfgFile_Des, NULL},
	{NULL, -1, -1, NULL, NULL}
};


/*
	Parent Node: InternetGatewayDevice.DeviceInfo.VendorConfigFile.
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
CwmpParameterStruct VendorConfigFileIndex[] =
{
	{"1", ObjectR, NoLimite, NULL, VendorConfigFile},
	{NULL, -1, -1, NULL, NULL}
};

#if defined(TCSUPPORT_CT_NETWORKMANAGESERVICE)
#if !defined(TCSUPPORT_C1_CUC)
static NodeFunc FtpEnable_DevInfo=
{
	NULL,
	NULL,
	getFtpEnableValue, 
	setFtpEnableValue, 
	getFtpEnableAttribute, 
	setFtpEnableAttribute, 
	NULL,
};

static NodeFunc FtpUserName_DevInfo=
{
	NULL,
	NULL,
	getFtpUserNameValue, 
	setFtpUserNameValue, 
	getFtpUserNameAttribute, 
	setFtpUserNameAttribute, 
	NULL,
};

static NodeFunc FtpPassword_DevInfo=
{
	NULL,
	NULL,
	getFtpPasswordValue, 
	setFtpPasswordValue, 
	getFtpPasswordAttribute, 
	setFtpPasswordAttribute, 
	NULL,
};

static NodeFunc FtpPort_DevInfo=
{
	NULL,
	NULL,
	getFtpPortValue, 
	setFtpPortValue, 
	getFtpPortAttribute, 
	setFtpPortAttribute, 
	NULL,
};

static NodeFunc TelnetEnable_DevInfo=
{
	NULL,
	NULL,
	getTelnetEnableValue, 
	setTelnetEnableValue, 
	getTelnetEnableAttribute, 
	setTelnetEnableAttribute, 
	NULL,
};

static NodeFunc TelnetUserName_DevInfo=
{
	NULL,
	NULL,
	getTelnetUserNameValue, 
	setTelnetUserNameValue, 
	getTelnetUserNameAttribute, 
	setTelnetUserNameAttribute, 
	NULL,
};

static NodeFunc TelnetPassword_DevInfo=
{
	NULL,
	NULL,
	getTelnetPasswordValue, 
	setTelnetPasswordValue, 
	getTelnetPasswordAttribute, 
	setTelnetPasswordAttribute, 
	NULL,
};

static NodeFunc TelnetPort_DevInfo=
{
	NULL,
	NULL,
	getTelnetPortValue, 
	setTelnetPortValue, 
	getTelnetPortAttribute, 
	setTelnetPortAttribute, 
	NULL,
};
#if defined(TCSUPPORT_CT_PON_SC)
static NodeFunc SshEnable_DevInfo=
{
	NULL,
	NULL,
	getSshEnableValue, 
	setSshEnableValue, 
	getSshEnableAttribute, 
	setSshEnableAttribute, 
	NULL,
};

static NodeFunc SshUserName_DevInfo=
{
	NULL,
	NULL,
	getSshUserNameValue, 
	setSshUserNameValue, 
	getSshUserNameAttribute, 
	setSshUserNameAttribute, 
	NULL,
};

static NodeFunc SshPassword_DevInfo=
{
	NULL,
	NULL,
	getSshPasswordValue, 
	setSshPasswordValue, 
	getSshPasswordAttribute, 
	setSshPasswordAttribute, 
	NULL,
};

static NodeFunc SshPort_DevInfo=
{
	NULL,
	NULL,
	getSshPortValue, 
	setSshPortValue, 
	getSshPortAttribute, 
	setSshPortAttribute, 
	NULL,
};

static NodeFunc TftpEnable_DevInfo=
{
	NULL,
	NULL,
	getTftpEnableValue, 
	setTftpEnableValue, 
	getTftpEnableAttribute, 
	setTftpEnableAttribute, 
	NULL,
};

static NodeFunc TftpUserName_DevInfo=
{
	NULL,
	NULL,
	getTftpUserNameValue, 
	setTftpUserNameValue, 
	getTftpUserNameAttribute, 
	setTftpUserNameAttribute, 
	NULL,
};

static NodeFunc TftpPassword_DevInfo=
{
	NULL,
	NULL,
	getTftpPasswordValue, 
	setTftpPasswordValue, 
	getTftpPasswordAttribute, 
	setTftpPasswordAttribute, 
	NULL,
};

static NodeFunc TftpPort_DevInfo=
{
	NULL,
	NULL,
	getTftpPortValue, 
	setTftpPortValue, 
	getTftpPortAttribute, 
	setTftpPortAttribute, 
	NULL,
};

static NodeFunc SnmpEnable_DevInfo=
{
	NULL,
	NULL,
	getSnmpEnableValue, 
	setSnmpEnableValue, 
	getSnmpEnableAttribute, 
	setSnmpEnableAttribute, 
	NULL,
};

static NodeFunc SnmpUserName_DevInfo=
{
	NULL,
	NULL,
	getSnmpUserNameValue, 
	setSnmpUserNameValue, 
	getSnmpUserNameAttribute, 
	setSnmpUserNameAttribute, 
	NULL,
};

static NodeFunc SnmpPassword_DevInfo=
{
	NULL,
	NULL,
	getSnmpPasswordValue, 
	setSnmpPasswordValue, 
	getSnmpPasswordAttribute, 
	setSnmpPasswordAttribute, 
	NULL,
};

static NodeFunc SnmpPort_DevInfo=
{
	NULL,
	NULL,
	getSnmpPortValue, 
	setSnmpPortValue, 
	getSnmpPortAttribute, 
	setSnmpPortAttribute, 
	NULL,
};
#endif
#endif
static NodeFunc MaintenanceAccountEnable_DevInfo=
{
	NULL,
	NULL,
	getMaintenanceAccountEnableValue, 
	setMaintenanceAccountEnableValue, 
	getMaintenanceAccountEnableAttribute, 
	setMaintenanceAccountEnableAttribute, 
	NULL,
};

#if !defined(TCSUPPORT_C1_CUC)
static NodeFunc MaintenanceAccountUsername_DevInfo=
{
	NULL,
	NULL,
	getMaintenanceAccountUsernameValue, 
	setMaintenanceAccountUsernameValue, 
	getMaintenanceAccountUsernameAttribute, 
	setMaintenanceAccountUsernameAttribute, 
	NULL,
};

#endif
static NodeFunc MaintenanceAccountPassword_DevInfo=
{
	NULL,
	NULL,
	getMaintenanceAccountPasswordValue, 
	setMaintenanceAccountPasswordValue, 
	getMaintenanceAccountPasswordAttribute, 
	setMaintenanceAccountPasswordAttribute, 
	NULL,
};

/*
	Parent Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_ServiceManage.
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
#if !defined(TCSUPPORT_C1_CUC)
CwmpParameterStruct X_CTCOM_ServiceManage_DevInfo[] =
{
	{"FtpEnable", BooleanW, NoLimite, &FtpEnable_DevInfo, NULL},
	{"FtpUserName", StrW, NoLimite, &FtpUserName_DevInfo, NULL},
	{"FtpPassword", StrW, NoLimite, &FtpPassword_DevInfo, NULL},
	{"FtpPort", IntW, NoLimite, &FtpPort_DevInfo, NULL},
	{"TelnetEnable", BooleanW, NoLimite, &TelnetEnable_DevInfo, NULL},
	{"TelnetUserName", StrW, NoLimite, &TelnetUserName_DevInfo, NULL},
	{"TelnetPassword", StrW, NoLimite, &TelnetPassword_DevInfo, NULL},
	{"TelnetPort", IntW, NoLimite, &TelnetPort_DevInfo, NULL},
#if defined(TCSUPPORT_CT_PON_SC)
	{"SshEnable", BooleanW, NoLimite, &SshEnable_DevInfo, NULL},
	{"SshUserName", StrW, NoLimite, &SshUserName_DevInfo, NULL},
	{"SshPassword", StrW, NoLimite, &SshPassword_DevInfo, NULL},
	{"SshPort", IntW, NoLimite, &SshPort_DevInfo, NULL},
	{"TftpEnable", BooleanW, NoLimite, &TftpEnable_DevInfo, NULL},
	{"TftpUserName", StrW, NoLimite, &TftpUserName_DevInfo, NULL},
	{"TftpPassword", StrW, NoLimite, &TftpPassword_DevInfo, NULL},
	{"TftpPort", IntW, NoLimite, &TftpPort_DevInfo, NULL},
	{"SnmpEnable", BooleanW, NoLimite, &SnmpEnable_DevInfo, NULL},
	{"SnmpUserName", StrW, NoLimite, &SnmpUserName_DevInfo, NULL},
	{"SnmpPassword", StrW, NoLimite, &SnmpPassword_DevInfo, NULL},
	{"SnmpPort", IntW, NoLimite, &SnmpPort_DevInfo, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};

#endif
/*
	Parent Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_TeleComAccount.
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
CwmpParameterStruct X_CTCOM_TeleComAccount_DevInfo[] =
{
	{"Enable", BooleanW, NoLimite, &MaintenanceAccountEnable_DevInfo, NULL},
	//{"Username", StrW, NoLimite, &MaintenanceAccountUsername_DevInfo, NULL},
	{"Password", StrW, NoLimite, &MaintenanceAccountPassword_DevInfo, NULL},
	{NULL, -1, -1, NULL, NULL}
};
#endif

#if defined(TCSUPPORT_CT_MIDWARE)
static NodeFunc Tr069Enable_DevInfo=
{
	NULL,
	NULL,
	getTr069EnableValue, 
	setTr069EnableValue, 
	getTr069EnableAttribute, 
	setTr069EnableAttribute, 
	NULL,
};

static NodeFunc MiddlewareURL_DevInfo=
{
	NULL,
	NULL,
	getMiddlewareURLValue,
	setMiddlewareURLValue, 
	getMiddlewareURLAttribute, 
	setMiddlewareURLAttribute, 
	NULL,
};

/*  
	Parent Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_MiddlewareMgt.
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
CwmpParameterStruct X_CTCOM_MiddlewareMgt_DevInfo[] =
{
	{"Tr069Enable", UnsignIntW, NoLimite, &Tr069Enable_DevInfo, NULL},
	{"MiddlewareURL", StrW, NoLimite, &MiddlewareURL_DevInfo, NULL},
	{NULL, -1, -1, NULL, NULL}
};	
#endif

#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT)
static NodeFunc MaintenancePortalEnable_DevInfo=
{
	NULL,
	NULL,
	getMaintenancePortalEnableValue, 
	setMaintenancePortalEnableValue, 
	getMaintenancePortalEnableAttribute, 
	setMaintenancePortalEnableAttribute, 
	NULL,
};

static NodeFunc MaintenancePortalComputer_DevInfo=
{
	NULL,
	NULL,
	getMaintenancePortalComputerValue, 
	setMaintenancePortalComputerValue, 
	getMaintenancePortalComputerAttribute, 
	setMaintenancePortalComputerAttribute, 
	NULL,
};

static NodeFunc MaintenancePortalSTB_DevInfo=
{
	NULL,
	NULL,
	getMaintenancePortalSTBValue, 
	setMaintenancePortalSTBValue, 
	getMaintenancePortalSTBAttribute, 
	setMaintenancePortalSTBAttribute, 
	NULL,
};

static NodeFunc MaintenancePortalPhone_DevInfo=
{
	NULL,
	NULL,
	getMaintenancePortalPhoneValue, 
	setMaintenancePortalPhoneValue, 
	getMaintenancePortalPhoneAttribute, 
	setMaintenancePortalPhoneAttribute, 
	NULL,
};
/*
	Parent Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_PortalManagement.
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
CwmpParameterStruct X_CTCOM_PortalManagement_DevInfo[] =
{
	{"Enable", BooleanW, NoLimite, &MaintenancePortalEnable_DevInfo, NULL},
	{"PortalUrl-Computer", StrW, NoLimite, &MaintenancePortalComputer_DevInfo, NULL},
	{"PortalUrl-STB", StrW, NoLimite, &MaintenancePortalSTB_DevInfo, NULL},
	{"PortalUrl-Phone", StrW, NoLimite, &MaintenancePortalPhone_DevInfo, NULL},
	{NULL, -1, -1, NULL, NULL}
};
#endif
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
static NodeFunc CTCOM_Enable=
{
	NULL,
	NULL,
	getCTCOMEnableValue, 
	setCTCOMEnableValue, 
	getCTCOMEnableAttribute, 
	setCTCOMEnableAttribute, 
	NULL,
};

static NodeFunc CTCOM_PingNumberOfEntries=
{
	NULL,
	NULL,
	getCTCOMPingNumberOfEntriesValue, 
	NULL, 
	getCTCOMPingNumberOfEntriesAttribute, 
	setCTCOMPingNumberOfEntriesAttribute, 
	NULL,
};

static NodeFunc PingConfigIndexFuncs=
{
	NULL,
	DeviceNodeDeletePingInedxObject,
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL,
};
/*
       This Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_Ping.PingConfig.{i}
	Parent Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_Ping.PingConfig.
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
CwmpParameterStruct PingConfigIndex[]= 
{
	{NULL, ObjectW, NoLimite, &PingConfigIndexFuncs, PingConfig},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc PingConfigFuncs =
{
	DeviceNodeAddPingInedxObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitIpPingDiagnosticTable
};

/*
	Parent Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_Ping.
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
CwmpParameterStruct X_CTCOM_Ping[] =
{
	{"Enable", BooleanW, NoLimite, &CTCOM_Enable, NULL},
	{"PingNumberOfEntries", UnsignIntR, NoLimite, &CTCOM_PingNumberOfEntries, NULL},
	{"PingConfig", ObjectW, NoLimite, &PingConfigFuncs, NULL},
	{NULL, -1, -1, NULL, NULL}
};
#endif

#if defined(TCSUPPORT_CT_ALARMMONITOR)
/*For Device Alarm & Monitor*/

static NodeFunc DeviceAlarmEnable=
{
	NULL,
	NULL,
	getDeviceAlarmEnableValue, 
	setDeviceAlarmEnableValue, 
	getDeviceAlarmEnableAttribute, 
	setDeviceAlarmEnableAttribute, 
	NULL,
};

static NodeFunc DeviceAlarmNumberOfEntries=
{
	NULL,
	NULL,
	getDeviceAlarmNumberOfEntriesValue, 
	NULL, 
	getDeviceAlarmNumberOfEntriesAttribute, 
	setDeviceAlarmNumberOfEntriesAttribute, 
	NULL,
};

static NodeFunc AlarmConfigParaList=
{
	NULL,
	NULL,
	getDeviceAlarmConfigParaListValue, 
	setDeviceAlarmConfigParaListValue, 
	getDeviceAlarmConfigParaListAttribute, 
	setDeviceAlarmConfigParaListAttribute, 
	NULL,
};

static NodeFunc AlarmConfigLimitMax=
{
	NULL,
	NULL,
	getDeviceAlarmConfigLimitMaxValue, 
	setDeviceAlarmConfigLimitMaxValue, 
	getDeviceAlarmConfigLimitMaxAttribute, 
	setDeviceAlarmConfigLimitMaxAttribute, 
	NULL,
};

static NodeFunc AlarmConfigLimitMin=
{
	NULL,
	NULL,
	getDeviceAlarmConfigLimitMinValue, 
	setDeviceAlarmConfigLimitMinValue, 
	getDeviceAlarmConfigLimitMinAttribute, 
	setDeviceAlarmConfigLimitMinAttribute, 
	NULL,
};

static NodeFunc AlarmConfigTimeList=
{
	NULL,
	NULL,
	getDeviceAlarmConfigTimeListValue, 
	setDeviceAlarmConfigTimeListValue, 
	getDeviceAlarmConfigTimeListAttribute, 
	setDeviceAlarmConfigTimeListAttribute, 
	NULL,
};

static NodeFunc AlarmConfigMode=
{
	NULL,
	NULL,
	getDeviceAlarmConfigModeValue, 
	setDeviceAlarmConfigModeValue, 
	getDeviceAlarmConfigModeAttribute, 
	setDeviceAlarmConfigModeAttribute, 
	NULL,
};


static NodeFunc DeviceMonitorEnable=
{
	NULL,
	NULL,
	getDeviceMonitorEnableValue, 
	setDeviceMonitorEnableValue, 
	getDeviceMonitorEnableAttribute, 
	setDeviceMonitorEnableAttribute, 
	NULL,
};

static NodeFunc DeviceMonitorNumberOfEntries=
{
	NULL,
	NULL,
	getDeviceMonitorNumberOfEntriesValue, 
	NULL, 
	getDeviceMonitorNumberOfEntriesAttribute, 
	setDeviceMonitorNumberOfEntriesAttribute, 
	NULL,
};

static NodeFunc MonitorConfigParaList=
{
	NULL,
	NULL,
	getDeviceMonitorConfigParaListValue, 
	setDeviceMonitorConfigParaListValue, 
	getDeviceMonitorConfigParaListAttribute, 
	setDeviceMonitorConfigParaListAttribute, 
	NULL,
};

static NodeFunc MonitorConfigTimeList=
{
	NULL,
	NULL,
	getDeviceMonitorConfigTimeListValue, 
	setDeviceMonitorConfigTimeListValue, 
	getDeviceMonitorConfigTimeListAttribute, 
	setDeviceMonitorConfigTimeListAttribute, 
	NULL,
};

static NodeFunc AlarmConfigIndexFuncs=
{
	NULL,
	deleteAlarmConfigObject,
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL,
};

static NodeFunc DeviceAlarmConfig=
{
	addAlarmConfigObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitAlarmConfigSubTree,
};


static NodeFunc MonitorConfigIndexFuncs=
{
	NULL,
	deleteMonitorConfigObject,
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL,
};

static NodeFunc DeviceMonitorConfig=
{
	addMonitorConfigObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitMonitorConfigSubTree,
};


/*
	Parent Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.{i}
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
CwmpParameterStruct AlarmConfig[]= 
{
	{"ParaList", StrW, NoLimite, &AlarmConfigParaList, NULL},	
	{"Limit-Max", StrW, NoLimite, &AlarmConfigLimitMax, NULL},
	{"Limit-Min", StrW, NoLimite, &AlarmConfigLimitMin, NULL},
	{"TimeList", StrW, NoLimite, &AlarmConfigTimeList, NULL},
	{"Mode", UnsignIntW, NoLimite, &AlarmConfigMode, NULL},	
	{NULL, -1, -1, NULL, NULL}
};

/*
       This Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.{i}
	Parent Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
CwmpParameterStruct AlarmConfigIndex[]= 
{
	{NULL, ObjectW, NoLimite, &AlarmConfigIndexFuncs, AlarmConfig},
	{NULL, -1, -1, NULL, NULL}
};

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER) || defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
static NodeFunc DeviceAlarmNumber=
{
	NULL,
	NULL,
	getDeviceAlarmNumberValue,
	NULL,
	getDeviceAlarmNumberAttribute,
	setDeviceAlarmNumberAttribute,
	NULL,
};
#endif

/*
	Parent Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
CwmpParameterStruct DeviceAlarm[] =
{
	{"Enable", BooleanW, NoLimite, &DeviceAlarmEnable, NULL},
	{"AlarmNumberOfEntries", UnsignIntR, NoLimite, &DeviceAlarmNumberOfEntries, NULL},
	{"AlarmConfig", ObjectW, NoLimite, &DeviceAlarmConfig, NULL},
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER) || defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	{"AlarmNumber", StrR, NoLimite, &DeviceAlarmNumber, NULL},
#endif

	{NULL, -1, -1, NULL, NULL}
};


/*
	Parent Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.{i}
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
CwmpParameterStruct MonitorConfig[]= 
{
	{"ParaList", StrW, NoLimite, &MonitorConfigParaList, NULL},	
	{"TimeList", StrW, NoLimite, &MonitorConfigTimeList, NULL},
	{NULL, -1, -1, NULL, NULL}
};

/*
       This Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.{i}
	Parent Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
CwmpParameterStruct MonitorConfigIndex[]= 
{
	{NULL, ObjectW, NoLimite, &MonitorConfigIndexFuncs, MonitorConfig},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
CwmpParameterStruct DeviceMonitor[] =
{
	{"Enable", BooleanW, NoLimite, &DeviceMonitorEnable, NULL},
	{"MonitorNumberOfEntries", UnsignIntR, NoLimite, &DeviceMonitorNumberOfEntries, NULL},
	{"MonitorConfig", ObjectW, NoLimite, &DeviceMonitorConfig, NULL},
	{NULL, -1, -1, NULL, NULL}
};
#endif

#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
static NodeFunc MonitorCollectorEnable=
{
	NULL,
	NULL,
	getMonitorCollectorEnableValue, 
	setMonitorCollectorEnableValue, 
	getMonitorCollectorEnableAttribute, 
	setMonitorCollectorEnableAttribute, 
	NULL,
};

static NodeFunc MonitorCollectorServerUrl=
{
	NULL,
	NULL,
	getMonitorCollectorServerUrlValue, 
	setMonitorCollectorServerUrlValue, 
	getMonitorCollectorServerUrlAttribute, 
	setMonitorCollectorServerUrlAttribute, 
	NULL,
};

static NodeFunc MonitorCollectorTftpPort=
{
	NULL,
	NULL,
	getMonitorCollectorTftpPortValue, 
	setMonitorCollectorTftpPortValue, 
	getMonitorCollectorTftpPortAttribute, 
	setMonitorCollectorTftpPortAttribute, 
	NULL,
};

static NodeFunc MonitorCollectorNumberOfEntries=
{
	NULL,
	NULL,
	getMonitorCollectorNumberOfEntriesValue, 
	NULL, 
	getMonitorCollectorNumberOfEntriesAttribute, 
	setMonitorCollectorNumberOfEntriesAttribute, 
	NULL,
};

static NodeFunc MonitorCollectorParaList=
{
	NULL,
	NULL,
	getMonitorCollectorParaListValue, 
	setMonitorCollectorParaListValue, 
	getMonitorCollectorParaListAttribute, 
	setMonitorCollectorParaListAttribute, 
	NULL,
};

static NodeFunc MonitorCollectorTimeList=
{
	NULL,
	NULL,
	getMonitorCollectorTimeListValue, 
	setMonitorCollectorTimeListValue, 
	getMonitorCollectorTimeListAttribute, 
	setMonitorCollectorTimeListAttribute, 
	NULL,
};

static NodeFunc MonitorCollectorIndexFuncs=
{
	NULL,
	deleteMonitorCollectorObject,
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL,
};

static NodeFunc MonitorCollectorConfig=
{
	addMonitorCollectorObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitMonitorCollectorSubTree,
};


/*
	Parent Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_MonitorCollector.{i}
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
CwmpParameterStruct MonitorCollectorConfig1[]= 
{
	{"ParaList", StrW, NoLimite, &MonitorCollectorParaList, NULL},	
	{"TimeList", StrW, NoLimite, &MonitorCollectorTimeList, NULL},
	{NULL, -1, -1, NULL, NULL}
};

/*
       This Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_MonitorCollector.{i}
	Parent Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
CwmpParameterStruct MonitorCollectorConfigIndex[]= 
{
	{NULL, ObjectW, NoLimite, &MonitorCollectorIndexFuncs, MonitorCollectorConfig1},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_MonitorCollector.
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
CwmpParameterStruct MonitorCollector[] =
{
	{"Enable", BooleanW, NoLimite, &MonitorCollectorEnable, NULL},
	{"ServerUrl", StrW, NoLimite, &MonitorCollectorServerUrl, NULL},	
	{"TftpPort", UnsignIntW, NoLimite, &MonitorCollectorTftpPort, NULL},
	{"MonitorNumberOfEntries", UnsignIntR, NoLimite, &MonitorCollectorNumberOfEntries, NULL},
	{"MonitorConfig", ObjectW, NoLimite, &MonitorCollectorConfig, NULL},
	{NULL, -1, -1, NULL, NULL}
};
#endif


#ifdef TCSUPPORT_SYSLOG
static NodeFunc SysLogEnable=
{
	NULL,
	NULL,
	getSysLogEnableValue, 
	setSysLogEnableValue, 
	getSysLogEnableAttribute, 
	setSysLogEnableAttribute, 
	NULL,
};

static NodeFunc SysLogLevel=
{
	NULL,
	NULL,
	getSysLogLevelValue, 
	setSysLogLevelValue, 
	getSysLogLevelAttribute, 
	setSysLogLevelAttribute, 
	NULL,
};

/*
	Parent Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_Syslog.
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
CwmpParameterStruct SysLog[] =
{
	{"Enable", BooleanW, NoLimite, &SysLogEnable, NULL},
	{"Level", UnsignIntW, NoLimite, &SysLogLevel, NULL},

	{NULL, -1, -1, NULL, NULL}
};
#endif

#if defined(TCSUPPORT_CT_ALGABILITY)
static NodeFunc H323ALGEnable=
{
	NULL,
	NULL,
	getH323ALGEnableValue, 
	setH323ALGEnableValue, 
	getH323ALGEnableAttribute, 
	setH323ALGEnableAttribute, 
	NULL,
};
static NodeFunc SIPALGEnable=
{
	NULL,
	NULL,
	getSIPALGEnableValue, 
	setSIPALGEnableValue, 
	getSIPALGEnableAttribute, 
	setSIPALGEnableAttribute, 
	NULL,
};
static NodeFunc RTSPALGEnable=
{
	NULL,
	NULL,
	getRTSPALGEnableValue, 
	setRTSPALGEnableValue, 
	getRTSPALGEnableAttribute, 
	setRTSPALGEnableAttribute, 
	NULL,
};
static NodeFunc L2TPALGEnable=
{
	NULL,
	NULL,
	getL2TPALGEnableValue, 
	setL2TPALGEnableValue, 
	getL2TPALGEnableAttribute, 
	setL2TPALGEnableAttribute, 
	NULL,
};
static NodeFunc IPSECALGEnable=
{
	NULL,
	NULL,
	getIPSECALGEnableValue, 
	setIPSECALGEnableValue, 
	getIPSECALGEnableAttribute, 
	setIPSECALGEnableAttribute, 
	NULL,
};
static NodeFunc FTPALGEnable=
{
	NULL,
	NULL,
	getFTPALGEnableValue, 
	setFTPALGEnableValue, 
	getFTPALGEnableAttribute, 
	setFTPALGEnableAttribute, 
	NULL,
};

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
static NodeFunc PPTPALGEnable=
{
	NULL,
	NULL,
	getPPTPALGEnableValue,
	setPPTPALGEnableValue,
	getPPTPALGEnableAttribute,
	setPPTPALGEnableAttribute,
	NULL,
};
#endif

/*
	Parent Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_ALGAbility.
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
CwmpParameterStruct ALGAbility[] =
{
	{"H323Enable", BooleanW, NoLimite, &H323ALGEnable, NULL},
	{"SIPEnable", BooleanW, NoLimite, &SIPALGEnable, NULL},
	{"RTSPEnable", BooleanW, NoLimite, &RTSPALGEnable, NULL},
	{"L2TPEnable", BooleanW, NoLimite, &L2TPALGEnable, NULL},
	{"IPSECEnable", BooleanW, NoLimite, &IPSECALGEnable, NULL},
	{"FTPEnable", BooleanW, NoLimite, &FTPALGEnable, NULL},
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"PPTPEnable", BooleanW, NoLimite, &PPTPALGEnable, NULL},
#endif
#endif

	{NULL, -1, -1, NULL, NULL}
};
#endif

#ifdef TCSUPPORT_IPV6
static NodeFunc IPProtocolVersion_mode=
{
	NULL,
	NULL,
	getIPProtocolVersion_modeValue, 
	setIPProtocolVersion_modeValue, 
	getIPProtocolVersion_modeAttribute, 
	setIPProtocolVersion_modeAttribute, 
	NULL,
};
CwmpParameterStruct IPProtocolVersion[] =
{
	{"Mode", UnsignIntW, NoLimite, &IPProtocolVersion_mode, NULL},
	{NULL, -1, -1, NULL, NULL}
};
#endif

#endif
#if defined(TCSUPPORT_CT_USB_BACKUPRESTORE)
static NodeFunc UsbAutoRestoreEnable=
{
	NULL,
	NULL,
	getUsbAutoRestoreEnableValue, 
	setUsbAutoRestoreEnableValue, 
	getUsbAutoRestoreEnableAttribute, 
	setUsbAutoRestoreEnableAttribute, 
	NULL,
};

/*
	Parent Node: InternetGatewayDevice.DeviceInfo.UsbRestore.UsbAutoRestoreEnable
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
CwmpParameterStruct UsbRestore[] =
{
	{CWMP_USB_PARAM_NAME, BooleanW, NoLimite, &UsbAutoRestoreEnable, NULL},

	{NULL, -1, -1, NULL, NULL}
};
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
static NodeFunc UPNPEnable=
{
	NULL,
	NULL,
	getUPNPEnableValue,
	setUPNPEnableValue,
	getUPNPEnableAttribute,
	setUPNPEnableAttribute,
	NULL,
};

CwmpParameterStruct CTComUPNP[] =
{
	{"Enable", BooleanW, NoLimite, &UPNPEnable, NULL},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc InterfaceVersion_DevInfo=
{
	NULL,
	NULL,
	getInterfaceVersionValue, 
	NULL, 
	getInterfaceVersionAttribute, 
	setInterfaceVersionAttribute, 
	NULL,
};

static NodeFunc DeviceType_DevInfo=
{
	NULL,
	NULL,
	getDeviceTypeValue, 
	NULL, 
	getDeviceTypeAttribute, 
	setDeviceTypeAttribute, 
	NULL,
};

static NodeFunc UPLink_DevInfo=
{
	NULL,
	NULL,
	getUPLinkValue, 
	NULL, 
	getUPLinkAttribute, 
	setUPLinkAttribute, 
	NULL,
};

static NodeFunc Capability_DevInfo=
{
	NULL,
	NULL,
	getCapabilityValue, 
	NULL, 
	getCapabilityAttribute, 
	setCapabilityAttribute, 
	NULL,
};

static NodeFunc Card_DevInfo=
{
	NULL,
	NULL,
	getCardValue, 
	NULL, 
	getCardAttribute, 
	setCardAttribute, 
	NULL,
};

static NodeFunc CardInterVersion=
{
	NULL,
	NULL,
	getCardInterVersionValue, 
	NULL, 
	getCardInterVersionAttribute, 
	setCardInterVersionAttribute, 
	NULL,
};

static NodeFunc DMSEnable=
{
	NULL,
	NULL,
	getDMSEnableValue,
	setDMSEnableValue,
	getDMSEnableAttribute,
	setDMSEnableAttribute,
	NULL,
};

CwmpParameterStruct DLNA[] =
{
	{"DMSEnable", BooleanW, NoLimite, &DMSEnable, NULL},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc IPForwardModeEnabled=
{
	NULL,
	NULL,
	getIPForwardModeEnabledValue,
	setIPForwardModeEnabledValue,
	getIPForwardModeEnabledAttribute,
	setIPForwardModeEnabledAttribute,
	NULL,
};
#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
static NodeFunc MACAddress_DevInfo=
{
	NULL,
	NULL,
	getMacAddressValue, 
	NULL, 
	getMacAddressAttribute, 
	setMacAddressAttribute, 
	NULL,
};
#endif

#if defined(TCSUPPORT_CT_FJ)
static NodeFunc VoIPWANStatus=
{
	NULL,
	NULL,
	getVoIPWANStatusValue, 
	NULL, 
	getVoIPWANStatusAttribute,
	setVoIPWANStatusAttribute, 
	NULL,
};

CwmpParameterStruct VoIPWAN[] =
{
        {"Status", StrR, NoLimite, &VoIPWANStatus, NULL},
        {NULL, -1, -1, NULL, NULL}
};

static NodeFunc OltRegTime=
{
	NULL,
	NULL,
	getOltRegTimeValue, 
	NULL, 
	getOltRegTimeAttribute,
	setOltRegTimeAttribute, 
	NULL,
};
static NodeFunc ITMSRegTime=
{
	NULL,
	NULL,
	getITMSRegTimeValue, 
	NULL, 
	getITMSRegTimeAttribute,
	setITMSRegTimeAttribute, 
	NULL,
};
static NodeFunc ITMSServiceTime=
{
	NULL,
	NULL,
	getITMSServiceTimeValue, 
	NULL, 
	getITMSServiceTimeAttribute,
	setITMSServiceTimeAttribute, 
	NULL,
};
CwmpParameterStruct RegStatistics_DevInfo[] =
{
	{"OltRegTime", UnsignIntR, NoLimite, &OltRegTime, NULL},
	{"ITMSRegTime", UnsignIntR, NoLimite, &ITMSRegTime, NULL},
	{"ITMSServiceTime", UnsignIntR, NoLimite, &ITMSServiceTime, NULL},
	{NULL, -1, -1, NULL, NULL}
};

#endif

#if defined(TCSUPPORT_CT_ECN_GZ) || defined(TCSUPPORT_CT_SIMCARD_SEPARATION) || defined(TCSUPPORT_CT_DEV_ACCESS_TYPE) || defined(TCSUPPORT_CT_PPPINFORM)
static NodeFunc DeviceInfo_DevType=
{
	NULL,
	NULL,
	getECNDeviceTypeValue,
	NULL,
	getECNDeviceTypeAttribute,
	setECNDeviceTypeAttribute,
	NULL,
};
#endif

#if defined(TCSUPPORT_CT_ECN_GZ) || defined(TCSUPPORT_CT_SIMCARD_SEPARATION) || defined(TCSUPPORT_CT_DEV_ACCESS_TYPE)
static NodeFunc DeviceInfo_AccessType=
{
	NULL,
	NULL,
	getDevAccessTypeValue,
	NULL,
	getDevAccessTypeAttribute,
	setDevAccessTypeAttribute,
	NULL,
};

#endif

#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
static NodeFunc GPONSerialNumber=
{
        NULL,
        NULL,
        getGPONSerialNumberValue,
        NULL,
        getGPONSerialNumberAttribute,
        setGPONSerialNumberAttribute,
        NULL,
};

static NodeFunc BatteryStatus=
{
        NULL,
        NULL,
        getBatteryStatusValue,
        NULL,
        getBatteryStatusAttribute,
        setBatteryStatusAttribute,
        NULL,
};

static NodeFunc ProcessorLoad=
{
        NULL,
        NULL,
        getProcessorLoadValue,
        NULL,
        getProcessorLoadAttribute,
        setProcessorLoadAttribute,
        NULL,
};

static NodeFunc MemoryLoad=
{
        NULL,
        NULL,
        getMemoryLoadValue,
        NULL,
        getMemoryLoadAttribute,
        setMemoryLoadAttribute,
        NULL,
};

CwmpParameterStruct LoadInfo[] =
{
	{"ProcessorLoad", UnsignIntR, NoLimite, &ProcessorLoad, NULL},
	{"MemoryLoad", UnsignIntR, NoLimite, &MemoryLoad, NULL},
	{NULL, -1, -1, NULL, NULL}
};
#endif

#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
static NodeFunc SIMCardKey=
{
	NULL,
	NULL,
	getSIMCardKeyValue,
	setSIMCardKeyValue,
	getSIMCardKeyAttribute,
	setSIMCardKeyAttribute,
	NULL,
};

static NodeFunc SIMCardNo=
{
	NULL,
	NULL,
	getSIMCardNoValue,
	NULL,
	getSIMCardNoAttribute,
	setSIMCardNoAttribute,
	NULL,
};

static NodeFunc SIMCardStatus=
{
	NULL,
	NULL,
	getSIMCardStatusValue,
	NULL,
	getSIMCardStatusAttribute,
	setSIMCardStatusAttribute,
	NULL,
};

static NodeFunc SIMCardExistStatus=
{
	NULL,
	NULL,
	getSIMCardExistStatusValue,
	NULL,
	getSIMCardExistStatusAttribute,
	setSIMCardExistStatusAttribute,
	NULL,
};

CwmpParameterStruct SimCardManager[] =
{
	{"CardNo", StrR, NoLimite, &SIMCardNo, NULL},
	{"status", UnsignIntR, NoLimite, &SIMCardStatus, NULL},
	{"CardStatus", BooleanR, NoLimite, &SIMCardExistStatus, NULL},
	{NULL, -1, -1, NULL, NULL}
};
#endif

#if defined(TCSUPPORT_CT_STB_TEST)
static NodeFunc DetectionsState=
{
    NULL,
    NULL,
    getDetectionsStateValue,
    setDetectionsStateValue,
    getDetectionsStateAttribute,
    setDetectionsStateAttribute,
    NULL,
};

static NodeFunc DetectionsResult=
{
    NULL,
    NULL,
    getDetectionsResultValue,
    setDetectionsResultValue,
    getDetectionsResultAttribute,
    setDetectionsResultAttribute,
    NULL,
};

static NodeFunc detectionsPeriod=
{
    NULL,
    NULL,
    getDetectionsPeriodValue,
    setDetectionsPeriodValue,
    getDetectionsResultAttribute,
    setDetectionsResultAttribute,
    NULL,
};

static NodeFunc detectionsFreqency=
{
    NULL,
    NULL,
    getDetectionsFreqencyValue,
    setDetectionsFreqencyValue,
    getDetectionsResultAttribute,
    setDetectionsResultAttribute,
    NULL,
};



CwmpParameterStruct Framedetection[] =
{
	{"detectionsState", StrW, NoLimite, &DetectionsState, NULL},
	{"detectionsResult", StrW, NoLimite, &DetectionsResult, NULL},
	{"detectionsPeriod", UnsignIntW, NoLimite, &detectionsPeriod, NULL},
	{"detectionsFreqency", UnsignIntW, NoLimite, &detectionsFreqency, NULL},
	{NULL, -1, -1, NULL, NULL}
};
#endif

/*
	Parent Node: InternetGatewayDevice.DeviceInfo.
	Parent Node Table Location: cp_topnodetable.c
*/
CwmpParameterStruct DeviceInfo[] = 
{
	{"Manufacturer", StrR, NoLimite, &Manufacturer_DevInfo, NULL},
	{"ManufacturerOUI", StrR, NoLimite, &ManufacturerOUI_DevInfo, NULL},
	{"ModelName", StrR, NoLimite, &ModelName_DevInfo, NULL},
	{"Description", StrR, NoLimite, &Description_DevInfo, NULL},
	{"ProductClass", StrR, NoLimite, &ProductClass_DevInfo, NULL},
	{"SerialNumber", StrR, NoLimite, &SerialNumber_DevInfo, NULL},
	{"HardwareVersion", StrR, NoLimite, &HardwareVersion_DevInfo, NULL},
	{"SoftwareVersion", StrR, NoLimite, &SoftwareVersion_DevInfo, NULL},
	{"SpecVersion", StrR, NoLimite, &SpecVersion_DevInfo, NULL},
	{"ProvisioningCode", StrW, NoLimite, &ProvisioningCode_DevInfo, NULL},
	{"UpTime", UnsignIntR, NoLimite, &UpTime, NULL},
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{X_CT_COM"DeviceLog", StrR, NoLimite, &DeviceLog, NULL},
#else
	{"DeviceLog", StrR, NoLimite, &DeviceLog, NULL},
#endif
	{"VendorConfigFile", ObjectR, NoLimite, NULL, VendorConfigFileIndex},
#if defined(TCSUPPORT_CT_NETWORKMANAGESERVICE)
#if !defined(TCSUPPORT_C1_CUC)
	{X_CT_COM"ServiceManage", ObjectR, NoLimite, NULL,X_CTCOM_ServiceManage_DevInfo},
#endif
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
#if defined(TCSUPPORT_C1_CUC)
	{"X_CU_TeleComAccount", ObjectR, NoLimite, NULL,X_CTCOM_TeleComAccount_DevInfo},	
#else
	{X_CT_COM"TeleComAccount", ObjectR, NoLimite, NULL,X_CTCOM_TeleComAccount_DevInfo},
#endif
#endif
#endif
#if defined(TCSUPPORT_CT_MIDWARE)
	{X_CT_COM"MiddlewareMgt", ObjectR, NoLimite, NULL, X_CTCOM_MiddlewareMgt_DevInfo},		
#endif
#if !defined(TCSUPPORT_C1_CUC)
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT)
	{X_CT_COM"PortalManagement", ObjectR, NoLimite, NULL,X_CTCOM_PortalManagement_DevInfo},
#endif
#endif
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
	{X_CT_COM"Ping", ObjectR, NoLimite, NULL,X_CTCOM_Ping},
#endif
#if defined(TCSUPPORT_CT_RECONNECT_NODE)
	{X_CT_COM"ReConnect", ObjectR, NoLimite, NULL, ReConnect_DevInfo},
#endif
#if defined(TCSUPPORT_CT_ALARMMONITOR)
	{X_CT_COM"Alarm", ObjectR, NoLimite, NULL, DeviceAlarm},
	{X_CT_COM"Monitor", ObjectR, NoLimite, NULL, DeviceMonitor},
#endif
#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
	{X_CT_COM"MonitorCollector", ObjectR, NoLimite, NULL, MonitorCollector},
#endif
#ifdef TCSUPPORT_SYSLOG
	{X_CT_COM"Syslog", ObjectR, NoLimite, NULL, SysLog},
#endif
#if defined(TCSUPPORT_CT_ALGABILITY)
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{X_CT_COM"ALGAbility", ObjectR, NoLimite, NULL, ALGAbility},
#endif
#endif
#ifdef TCSUPPORT_IPV6
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{X_CT_COM"IPProtocolVersion", StrW, NoLimite, &IPProtocolVersion_mode, NULL},
#else
	{X_CT_COM"IPProtocolVersion", ObjectR, NoLimite, NULL, IPProtocolVersion},
#endif
#endif
#endif

#if defined(TCSUPPORT_CT_USB_BACKUPRESTORE)
	{CWMP_USB_RESTORE_NODE, ObjectR, NoLimite, NULL, UsbRestore},
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	{X_CT_COM"UPNP", ObjectR, NoLimite, NULL, CTComUPNP},
	{X_CT_COM"InterfaceVersion", StrR, NoLimite, &InterfaceVersion_DevInfo, NULL},
	{X_CT_COM"DeviceType", UnsignIntR, NoLimite, &DeviceType_DevInfo, NULL},
	{X_CT_COM"UPLink", UnsignIntR, NoLimite, &UPLink_DevInfo, NULL},
	{X_CT_COM"Capability", StrR, NoLimite, &Capability_DevInfo, NULL},
	{X_CT_COM"Card", BooleanR, NoLimite, &Card_DevInfo, NULL},
	{X_CT_COM"CardInterVersion", StrR, NoLimite, &CardInterVersion, NULL},
	{X_CT_COM"DLNA", ObjectR, NoLimite, NULL, DLNA},
	{X_CT_COM"IPForwardModeEnabled", BooleanW, NoLimite, &IPForwardModeEnabled, NULL},
#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	{X_CT_COM"MACAddress", StrR, NoLimite, &MACAddress_DevInfo, NULL},
#endif
#if defined(TCSUPPORT_CT_FJ)
	{X_CT_COM"VoIPWANStatus", ObjectR, NoLimite, NULL, VoIPWAN},
	{X_CT_COM"RegStatistics", ObjectR, NoLimite, NULL, RegStatistics_DevInfo},
#endif
#if defined(TCSUPPORT_CT_ECN_GZ) || defined(TCSUPPORT_CT_SIMCARD_SEPARATION)  || defined(TCSUPPORT_CT_DEV_ACCESS_TYPE) || defined(TCSUPPORT_CT_PPPINFORM)
	{"DeviceType", StrR, NoLimite, &DeviceInfo_DevType, NULL},
#endif
#if defined(TCSUPPORT_CT_ECN_GZ) || defined(TCSUPPORT_CT_SIMCARD_SEPARATION)  || defined(TCSUPPORT_CT_DEV_ACCESS_TYPE)
	{"AccessType", StrR, NoLimite, &DeviceInfo_AccessType, NULL},
#endif
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{X_CU"SerialNumber", StrR, NoLimite, &GPONSerialNumber, NULL},
	{X_CU"BatteryStatus", StrR, NoLimite, &BatteryStatus, NULL},
	{X_CU"LoadInfo", ObjectR, NoLimite, NULL, LoadInfo},
#endif
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	{X_CT_COM"CardKey", StrW, NoLimite, &SIMCardKey, NULL},
	{X_CT_COM"Cardmanage", ObjectR, NoLimite, NULL, SimCardManager},
#endif
#if defined(TCSUPPORT_CT_STB_TEST)
	{X_CT_COM"Framedetection", ObjectR, NoLimite, NULL, Framedetection},
#endif
	{NULL, -1, -1, NULL, NULL}
};


