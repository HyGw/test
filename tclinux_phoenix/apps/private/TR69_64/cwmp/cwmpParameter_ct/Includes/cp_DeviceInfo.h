#ifndef _CP_DEVICEINFO_H
#define _CP_DEVICEINFO_H

#include "cp_core.h"

#define MAX_TR69_PARAM
#if defined(TCSUPPORT_CT_NETWORKMANAGESERVICE)
#define ACCCOUNT "Account"
#define MAINTENANCEACCOUNT_LOGIN_ENTRY "Account_Entry0"
#if !defined(TCSUPPORT_C1_CUC)
#define TELNET_LOGIN_ENTRY "Account_TelnetEntry"
#define FTP_LOGIN_ENTRY "Account_FtpEntry"
#if defined(TCSUPPORT_CT_PON_SC)
#define SSH_LOGIN_ENTRY "Account_SshEntry"
#define TFTP_LOGIN_ENTRY "Account_TftpEntry"
#define SNMP_LOGIN_ENTRY "Account_SnmpEntry"
#endif
#endif
#endif

#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
/*
	Parent Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_Ping.PingConfig.{i}.
	Parent Node Table Location: cp_DeviceInfo_nodetable.c
*/
#if !defined(TCSUPPORT_C1_CUC)
extern CwmpParameterStruct PingConfig[];
#endif
#define MAX_IPPINGDIAGNOSTIC_NUM 3
#define IPPINGDIAGNOSTIC_ENTRY "CtDiagnostic_Entry%d"
#define IPPINGDIAGNOSTIC_COMMON "CtDiagnostic_Common"
#endif

#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
#define SIM_CARD_NODE_NAME "SimCard_Entry"
#endif

int8 getManufacturerValue(char** value, ParameterType* type,  DeviceNode*  deviceNode);
int8 getManufacturerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setManufacturerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getManufacturerOUIValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getManufacturerOUIAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setManufacturerOUIAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getModelNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getModelNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setModelNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getDescriptionValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDescriptionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDescriptionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getProductClassValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getProductClassAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setProductClassAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getSerialNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSerialNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSerialNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getHardwareVersionValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getHardwareVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHardwareVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSoftwareVersionValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSoftwareVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSoftwareVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getSpecVersionValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSpecVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSpecVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getProvisioningCodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setProvisioningCodeValue(void* value, struct DeviceNode*  deviceNode);
int8 getProvisioningCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setProvisioningCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUpTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUpTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUpTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDeviceLogValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDeviceLogAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceLogAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getVendorCfgFileNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getVendorCfgFileNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setVendorCfgFileNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getVendorCfgFileDescripValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getVendorCfgFileDescripAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setVendorCfgFileDescripAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);
#if defined(TCSUPPORT_CT_NETWORKMANAGESERVICE)
#if !defined(TCSUPPORT_C1_CUC)
int8 getFtpEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setFtpEnableValue(void* value, struct DeviceNode*  deviceNode);
int8	getFtpEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setFtpEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFtpUserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setFtpUserNameValue(void* value, struct DeviceNode*  deviceNode);
int8	getFtpUserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setFtpUserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFtpPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setFtpPasswordValue(void* value, struct DeviceNode*  deviceNode);
int8	getFtpPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setFtpPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFtpPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setFtpPortValue(void* value, struct DeviceNode*  deviceNode);
int8	getFtpPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setFtpPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTelnetEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setTelnetEnableValue(void* value, struct DeviceNode*  deviceNode);
int8	getTelnetEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setTelnetEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTelnetUserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setTelnetUserNameValue(void* value, struct DeviceNode*  deviceNode);
int8	getTelnetUserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setTelnetUserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTelnetPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setTelnetPasswordValue(void* value, struct DeviceNode*  deviceNode);
int8	getTelnetPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setTelnetPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTelnetPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setTelnetPortValue(void* value, struct DeviceNode*  deviceNode);
int8	getTelnetPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setTelnetPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CT_PON_SC)
int8	getSshEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setSshEnableValue(void* value, struct DeviceNode*  deviceNode);
int8	getSshEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setSshEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getSshUserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setSshUserNameValue(void* value, struct DeviceNode*  deviceNode);
int8	getSshUserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setSshUserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getSshPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setSshPasswordValue(void* value, struct DeviceNode*  deviceNode);
int8	getSshPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setSshPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getSshPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setSshPortValue(void* value, struct DeviceNode*  deviceNode);
int8	getSshPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setSshPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getTftpEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setTftpEnableValue(void* value, struct DeviceNode*  deviceNode);
int8	getTftpEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setTftpEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getTftpUserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setTftpUserNameValue(void* value, struct DeviceNode*  deviceNode);
int8	getTftpUserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setTftpUserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getTftpPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setTftpPasswordValue(void* value, struct DeviceNode*  deviceNode);
int8	getTftpPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setTftpPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getTftpPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setTftpPortValue(void* value, struct DeviceNode*  deviceNode);
int8	getTftpPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setTftpPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getSnmpEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setSnmpEnableValue(void* value, struct DeviceNode*  deviceNode);
int8	getSnmpEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setSnmpEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getSnmpUserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setSnmpUserNameValue(void* value, struct DeviceNode*  deviceNode);
int8	getSnmpUserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setSnmpUserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getSnmpPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setSnmpPasswordValue(void* value, struct DeviceNode*  deviceNode);
int8	getSnmpPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setSnmpPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getSnmpPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setSnmpPortValue(void* value, struct DeviceNode*  deviceNode);
int8	getSnmpPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setSnmpPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif
#endif
int8	getMaintenanceAccountEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setMaintenanceAccountEnableValue(void* value, struct DeviceNode*  deviceNode);
int8	getMaintenanceAccountEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setMaintenanceAccountEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if !defined(TCSUPPORT_C1_CUC)
int8	getMaintenanceAccountUsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setMaintenanceAccountUsernameValue(void* value, struct DeviceNode*  deviceNode);
int8	getMaintenanceAccountUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setMaintenanceAccountUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
int8	getMaintenanceAccountPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setMaintenanceAccountPasswordValue(void* value, struct DeviceNode*  deviceNode);
int8	getMaintenanceAccountPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setMaintenanceAccountPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CT_MIDWARE)
int8	getTr069EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setTr069EnableValue(void* value, struct DeviceNode*  deviceNode);
int8	getTr069EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setTr069EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 	getMiddlewareURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 	setMiddlewareURLValue(void* value, struct DeviceNode*  deviceNode);
int8 	getMiddlewareURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 	setMiddlewareURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif


#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT) || defined(TCSUPPORT_TRUE_LANDING_PAGE)
int8	getMaintenancePortalEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setMaintenancePortalEnableValue(void* value, struct DeviceNode*  deviceNode);
int8	getMaintenancePortalEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setMaintenancePortalEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif
#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT)
int8	getMaintenancePortalComputerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setMaintenancePortalComputerValue(void* value, struct DeviceNode*  deviceNode);
int8	getMaintenancePortalComputerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setMaintenancePortalComputerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getMaintenancePortalSTBValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setMaintenancePortalSTBValue(void* value, struct DeviceNode*  deviceNode);
int8	getMaintenancePortalSTBAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setMaintenancePortalSTBAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8	getMaintenancePortalPhoneValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setMaintenancePortalPhoneValue(void* value, struct DeviceNode*  deviceNode);
int8	getMaintenancePortalPhoneAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setMaintenancePortalPhoneAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif
#endif

#ifdef TCSUPPORT_SYSLOG
/* For syslog */
int get_syslog_deviceinfo(char *devInfoBuf);
#endif

#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
int8	getCTCOMEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	setCTCOMEnableValue(void* value, struct DeviceNode*  deviceNode);
int8	getCTCOMEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setCTCOMEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8	getCTCOMPingNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8	getCTCOMPingNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode); 
int8	setCTCOMPingNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
#if defined(TCSUPPORT_CT_RECONNECT_NODE)
int8 getEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CT_ALARMMONITOR)

#define ALARM_NODENAME 		"DeviceAlarm"
#define ALARM_COMMON 		"DeviceAlarm_Common"
#define ALARM_ENTRY 		"DeviceAlarm_Entry%d"

#define MONITOR_NODENAME 	"DeviceMonitor"
#define MONITOR_COMMON 		"DeviceMonitor_Common"
#define MONITOR_ENTRY 		"DeviceMonitor_Entry%d"

#define	MAX_ATTRIBUTE_LEN	256

extern CwmpParameterStruct AlarmConfigIndex[];
extern CwmpParameterStruct MonitorConfigIndex[];

int cwmpInitAlarmConfigSubTree( DeviceNodePtr deviceNode );
int8 deleteAlarmConfigObject(uint32 num, DeviceNode*  deviceNode);
int8 addAlarmConfigObject(uint32* num, DeviceNode*  deviceNode);
	
int8 getDeviceAlarmEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDeviceAlarmEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getDeviceAlarmEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceAlarmEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDeviceAlarmNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDeviceAlarmNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceAlarmNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDeviceAlarmConfigParaListValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDeviceAlarmConfigParaListValue(void* value, struct DeviceNode*  deviceNode);
int8 getDeviceAlarmConfigParaListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceAlarmConfigParaListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDeviceAlarmConfigLimitMaxValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDeviceAlarmConfigLimitMaxValue(void* value, struct DeviceNode*  deviceNode);
int8 getDeviceAlarmConfigLimitMaxAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceAlarmConfigLimitMaxAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDeviceAlarmConfigLimitMinValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDeviceAlarmConfigLimitMinValue(void* value, struct DeviceNode*  deviceNode);
int8 getDeviceAlarmConfigLimitMinAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceAlarmConfigLimitMinAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDeviceAlarmConfigTimeListValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDeviceAlarmConfigTimeListValue(void* value, struct DeviceNode*  deviceNode);
int8 getDeviceAlarmConfigTimeListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceAlarmConfigTimeListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDeviceAlarmConfigModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDeviceAlarmConfigModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getDeviceAlarmConfigModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceAlarmConfigModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int cwmpInitMonitorConfigSubTree( DeviceNodePtr deviceNode );
int8 deleteMonitorConfigObject(uint32 num, DeviceNode*  deviceNode);
int8 addMonitorConfigObject(uint32* num, DeviceNode*  deviceNode);

int8 getDeviceMonitorEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDeviceMonitorEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getDeviceMonitorEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceMonitorEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDeviceMonitorNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDeviceMonitorNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceMonitorNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDeviceMonitorConfigParaListValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDeviceMonitorConfigParaListValue(void* value, struct DeviceNode*  deviceNode);
int8 getDeviceMonitorConfigParaListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceMonitorConfigParaListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDeviceMonitorConfigTimeListValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDeviceMonitorConfigTimeListValue(void* value, struct DeviceNode*  deviceNode);
int8 getDeviceMonitorConfigTimeListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceMonitorConfigTimeListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER) || defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
int8 getDeviceAlarmNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDeviceAlarmNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceAlarmNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#endif

#ifdef TCSUPPORT_SYSLOG
int8 getSysLogEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSysLogEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getSysLogEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSysLogEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSysLogLevelValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSysLogLevelValue(void* value, struct DeviceNode*  deviceNode);
int8 getSysLogLevelAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSysLogLevelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CT_ALGABILITY)
/*For ALG*/
int8 getH323ALGEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setH323ALGEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getH323ALGEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setH323ALGEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPALGEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSIPALGEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPALGEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPALGEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRTSPALGEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRTSPALGEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getRTSPALGEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRTSPALGEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getL2TPALGEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setL2TPALGEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getL2TPALGEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setL2TPALGEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPSECALGEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPSECALGEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPSECALGEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPSECALGEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFTPALGEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setFTPALGEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getFTPALGEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFTPALGEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 getPPTPALGEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPPTPALGEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getPPTPALGEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPPTPALGEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#endif
#ifdef TCSUPPORT_IPV6
int8 getIPProtocolVersion_modeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPProtocolVersion_modeValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPProtocolVersion_modeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPProtocolVersion_modeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
#endif

#if defined(TCSUPPORT_CT_USB_BACKUPRESTORE)
int8 getUsbAutoRestoreEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUsbAutoRestoreEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getUsbAutoRestoreEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUsbAutoRestoreEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 getUPNPEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUPNPEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getUPNPEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUPNPEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getInterfaceVersionValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getInterfaceVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setInterfaceVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDeviceTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDeviceTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUPLinkValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUPLinkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUPLinkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCapabilityValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCapabilityAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCapabilityAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCardValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCardAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCardAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCardInterVersionValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCardInterVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCardInterVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDMSEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDMSEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getDMSEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDMSEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPForwardModeEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPForwardModeEnabledValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPForwardModeEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPForwardModeEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#if defined(TCSUPPORT_CT_FJ) ||  defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
int8 getMacAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getMacAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMacAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CT_FJ)
int8 getVoIPWANStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getVoIPWANStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setVoIPWANStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getOltRegTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getOltRegTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setOltRegTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getITMSRegTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getITMSRegTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setITMSRegTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getITMSServiceTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getITMSServiceTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setITMSServiceTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
#if defined(TCSUPPORT_CT_ECN_GZ) || defined(TCSUPPORT_CT_SIMCARD_SEPARATION) || defined(TCSUPPORT_CT_DEV_ACCESS_TYPE) || defined(TCSUPPORT_CT_PPPINFORM)
extern char *getECNDeviceType();
extern char *getECNAccessType();

int8 getECNDeviceTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getECNDeviceTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setECNDeviceTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CT_ECN_GZ) || defined(TCSUPPORT_CT_SIMCARD_SEPARATION) || defined(TCSUPPORT_CT_DEV_ACCESS_TYPE)
int8 getDevAccessTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDevAccessTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDevAccessTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getGPONSerialNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getGPONSerialNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setGPONSerialNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getBatteryStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getBatteryStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setBatteryStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getProcessorLoadValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getProcessorLoadAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setProcessorLoadAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMemoryLoadValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getMemoryLoadAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMemoryLoadAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif


#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
int8 getSIMCardKeyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSIMCardKeyValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIMCardKeyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIMCardKeyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIMCardNoValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSIMCardNoAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIMCardNoAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIMCardStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSIMCardStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIMCardStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIMCardExistStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSIMCardExistStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIMCardExistStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif


#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
extern CwmpParameterStruct MonitorCollectorConfigIndex[];

int8 checkFreeMonitorCollectorEntry(void);
int8 InitMonitorCollectorEntryByIndex(int num);
int8 deleteMonitorCollectorEntryByIndex(uint32 num);
int cwmpInitMonitorCollectorSubTree( DeviceNodePtr deviceNode );
int8 addMonitorCollectorObject(uint32* num, DeviceNode*  deviceNode);
int8 deleteMonitorCollectorObject(uint32 num, DeviceNode*  deviceNode);

int8 getMonitorCollectorEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMonitorCollectorEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getMonitorCollectorEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMonitorCollectorEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMonitorCollectorServerUrlValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMonitorCollectorServerUrlValue(void* value, struct DeviceNode*  deviceNode);
int8 getMonitorCollectorServerUrlAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMonitorCollectorServerUrlAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMonitorCollectorTftpPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMonitorCollectorTftpPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getMonitorCollectorTftpPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMonitorCollectorTftpPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMonitorCollectorNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getMonitorCollectorNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMonitorCollectorNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMonitorCollectorParaListValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMonitorCollectorParaListValue(void* value, struct DeviceNode*  deviceNode);
int8 getMonitorCollectorParaListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMonitorCollectorParaListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMonitorCollectorTimeListValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMonitorCollectorTimeListValue(void* value, struct DeviceNode*  deviceNode);
int8 getMonitorCollectorTimeListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMonitorCollectorTimeListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

#if defined(TCSUPPORT_CT_STB_TEST)
int8 getDetectionsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDetectionsStateValue(void* value, struct DeviceNode*  deviceNode);
int8 getDetectionsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDetectionsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDetectionsResultValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDetectionsResultValue(void* value, struct DeviceNode*  deviceNode);
int8 getDetectionsResultAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDetectionsResultAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDetectionsPeriodValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDetectionsPeriodValue(void* value, struct DeviceNode*  deviceNode);

int8 getDetectionsFreqencyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDetectionsFreqencyValue(void* value, struct DeviceNode*  deviceNode);
#endif

#endif

