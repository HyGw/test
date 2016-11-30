#ifndef _CP_PPPOEEMULATOR_H
#define _CP_PPPOEEMULATOR_H

#include "cp_core.h"

extern CwmpParameterStruct PppoeEmulator[] ;
extern CwmpParameterStruct PppoeEmulatorBridge[] ;

#define PPPOEEMULATOR_DIAGNOSTICS_STATE_STR	"DiagnosticsState"
#define PPPOEEMULATOR_USERNAME_STR  			"Username"
#define PPPOEEMULATOR_PASSWORD_STR    			"Password"
#define PPPOEEMULATOR_WANINTERFACE_STR		"WANInterface"
#define PPPOEEMULATOR_PPPAUTHPROTO_STR		"PPPAuthenticationProtocol"
#define PPPOEEMULATOR_RETRYTIMES_STR			"RetryTimes"
#define PPPOEEMULATOR_RESULT_STR				"Result"
#define PPPOEEMULATOR_PPPSESSIONID_STR		"PPPSessionID"
#define PPPOEEMULATOR_EXTERNALIPADDRESS_STR	"ExternalIPAddress"
#define PPPOEEMULATOR_DEFAULTGATEWAY_STR		"DefaultGateway"
#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
#define PPPOEEMULATOR_ERRORCODE_NODESTR "ErrorCode"
#define PPPOEEMULATOR_ERRORCODE_STR "ConnectionError"
#define PPPOEEMULATOR_WANIPMODE_STR "WANPPPConnectionIPMode"
#define PPPOEEMULATOR_DSLITEENABLE_STR "Dslite_Enable"
#define PPPOEEMULATOR_AFTRADDRESS_STR "AftrAddress"
#define PPPOEEMULATOR_WANIPV6ADDRESS_STR "WANIPv6IPAddress"
#define PPPOEEMULATOR_WANIPV6DNSADDRESS_STR "WANIPv6DNSServers"
#define PPPOEEMULATOR_WANIPV6PREFIX_STR "WANIPv6Prefix"
#define PPPOEEMULATOR_WANDEFIPV6GATEWAY_STR "WANDefaultIPv6Gateway"
#define PPPOEEMULATOR_LANIPV6PREFIX_STR "LANIPv6Prefix"
#endif
#define PPPOEEMULATOR_ENTRY_STR				"PppoeEmulator_Entry"

#define PPPOEEMULATOR_BRIDGE_ENABLE_STR				"Enable"
#define PPPOEEMULATOR_BRIDGE_USERNAME_STR			"Username"
#define PPPOEEMULATOR_BRIDGE_PASSWORD_STR			"Password"
#define PPPOEEMULATOR_BRIDGE_TIMELIST_STR				"TimeList"
#define PPPOEEMULATOR_BRIDGE_FAILRETRYTIMELIST_STR	"FailRetryTimeList"
#define PPPOEEMULATOR_BRIDGE_ENTRY_STR				"PppoeEmulatorBridge_Entry"

/*******************  InternetGatewayDevice.X_CT-COM_PPPOE_EMULATOR. start ************************/
int8 getPppoeEmulatorDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPppoeEmulatorDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode);
int8 getPppoeEmulatorDiagnosticsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorDiagnosticsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPppoeEmulatorUsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPppoeEmulatorUsernameValue(void* value, struct DeviceNode*  deviceNode);
int8 getPppoeEmulatorUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPppoeEmulatorPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPppoeEmulatorPasswordValue(void* value, struct DeviceNode*  deviceNode);
int8 getPppoeEmulatorPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPppoeEmulatorWanInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPppoeEmulatorWanInterfaceValue(void* value, struct DeviceNode*  deviceNode);
int8 getPppoeEmulatorWanInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorWanInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPppoeEmulatorPppAuthProtoValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPppoeEmulatorPppAuthProtoValue(void* value, struct DeviceNode*  deviceNode);
int8 getPppoeEmulatorPppAuthProtoAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorPppAuthProtoAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPppoeEmulatorRetryTimesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPppoeEmulatorRetryTimesValue(void* value, struct DeviceNode*  deviceNode);
int8 getPppoeEmulatorRetryTimesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorRetryTimesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPppoeEmulatorResultValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPppoeEmulatorResultValue(void* value, struct DeviceNode*  deviceNode);
int8 getPppoeEmulatorResultAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorResultAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPppoeEmulatorPppSessionIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPppoeEmulatorPppSessionIDValue(void* value, struct DeviceNode*  deviceNode);
int8 getPppoeEmulatorPppSessionIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorPppSessionIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPppoeEmulatorExternalIPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPppoeEmulatorExternalIPValue(void* value, struct DeviceNode*  deviceNode);
int8 getPppoeEmulatorExternalIPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorExternalIPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPppoeEmulatorDefGwValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPppoeEmulatorDefGwValue(void* value, struct DeviceNode*  deviceNode);
int8 getPppoeEmulatorDefGwAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorDefGwAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
int8 getPppoeEmulatorErrCodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getPppoeEmulatorErrCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorErrCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPppoeEmulatorWanIPModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getPppoeEmulatorWanIPModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorWanIPModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPppoeEmulatorDsliteEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getPppoeEmulatorDsliteEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorDsliteEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPppoeEmulatorAftrAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getPppoeEmulatorAftrAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorAftrAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPppoeEmulatorWANIPv6AddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getPppoeEmulatorWANIPv6AddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorWANIPv6AddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPppoeEmulatorWANIPv6DNSAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getPppoeEmulatorWANIPv6DNSAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorWANIPv6DNSAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPppoeEmulatorWANIPv6PrefixValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getPppoeEmulatorWANIPv6PrefixAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorWANIPv6PrefixAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPppoeEmulatorWANDefIPv6GatewayValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getPppoeEmulatorWANDefIPv6GatewayAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorWANDefIPv6GatewayAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPppoeEmulatorLANIPv6PrefixValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getPppoeEmulatorLANIPv6PrefixAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorLANIPv6PrefixAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

int8 getPppoeEmulatorBridgeEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPppoeEmulatorBridgeEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getPppoeEmulatorBridgeEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorBridgeEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPppoeEmulatorBridgeUsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPppoeEmulatorBridgeUsernameValue(void* value, struct DeviceNode*  deviceNode);
int8 getPppoeEmulatorBridgeUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorBridgeUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPppoeEmulatorBridgePasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPppoeEmulatorBridgePasswordValue(void* value, struct DeviceNode*  deviceNode);
int8 getPppoeEmulatorBridgePasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorBridgePasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPppoeEmulatorBridgeTimeListValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPppoeEmulatorBridgeTimeListValue(void* value, struct DeviceNode*  deviceNode);
int8 getPppoeEmulatorBridgeTimeListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorBridgeTimeListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPppoeEmulatorBridgeFailRetryTimeListValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPppoeEmulatorBridgeFailRetryTimeListValue(void* value, struct DeviceNode*  deviceNode);
int8 getPppoeEmulatorBridgeFailRetryTimeListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPppoeEmulatorBridgeFailRetryTimeListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

