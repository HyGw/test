#ifndef _CP_IPOEEMULATOR_H
#define _CP_IPOEEMULATOR_H

#include "cp_core.h"

#define IPOEEMULATOR_DIAGNOSTICS_STATE_STR "DiagnosticsState"
#define IPOEEMULATOR_WANINTERFACE_STR "WANInterface"
#define IPOEEMULATOR_USERMAC_STR "UserMac"
#define IPOEEMULATOR_VENDORCLASS_STR "VendorClassID"
#define IPOEEMULATOR_RESULT_STR "Result"
#define IPOEEMULATOR_LOCALADDRESS_STR "LocalAddress"
#define IPOEEMULATOR_DEFGATEWAY_STR "DefaultGateway"
#define IPOEEMULATOR_PINGDSTIP_STR "PingDestIPAddress"
#define IPOEEMULATOR_PINGNUM_STR "PingNumberOfRepetitions"
#define IPOEEMULATOR_PINGTIMEOUT_STR "Timeout"
#define IPOEEMULATOR_PINGSUCC_COUNT_STR "SuccessCount"
#define IPOEEMULATOR_PINGFAIL_COUNT_STR "FailureCount"
#define IPOEEMULATOR_PINGAVR_REPTIME_STR "AverageResponseTime"
#define IPOEEMULATOR_PINGMIN_REPTIME_STR "MinimumResponseTime"
#define IPOEEMULATOR_PINGMAX_REPTIME_STR "MaximumResponseTime"
#define IPOEEMULATOR_ENTRY_STR "IpoeEmulator_Entry"


/*******************  InternetGatewayDevice.X_CT-COM_IPoEDiagnostics. start ************************/
int8 getIPoeEmulatorDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPoeEmulatorDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPoeEmulatorDiagnosticsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPoeEmulatorDiagnosticsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPoeEmulatorWANInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPoeEmulatorWANInterfaceValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPoeEmulatorWANInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPoeEmulatorWANInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPoeEmulatorUserMacValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPoeEmulatorUserMacValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPoeEmulatorUserMacAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPoeEmulatorUserMacAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPoeEmulatorVendorClassIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPoeEmulatorVendorClassIDValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPoeEmulatorVendorClassIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPoeEmulatorVendorClassIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPoeEmulatorResultValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getIPoeEmulatorResultAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPoeEmulatorResultAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPoeEmulatorLocalAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getIPoeEmulatorLocalAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPoeEmulatorLocalAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPoeEmulatorDefaultGatewayValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getIPoeEmulatorDefaultGatewayAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPoeEmulatorDefaultGatewayAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPoeEmulatorPingDestIPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPoeEmulatorPingDestIPAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPoeEmulatorPingDestIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPoeEmulatorPingDestIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPoeEmulatorPingNumberOfRepetitionsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPoeEmulatorPingNumberOfRepetitionsValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPoeEmulatorPingNumberOfRepetitionsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPoeEmulatorPingNumberOfRepetitionsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPoeEmulatorTimeoutValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPoeEmulatorTimeoutValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPoeEmulatorTimeoutAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPoeEmulatorTimeoutAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPoeEmulatorSuccessCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getIPoeEmulatorSuccessCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPoeEmulatorSuccessCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPoeEmulatorFailureCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getIPoeEmulatorFailureCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPoeEmulatorFailureCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPoeEmulatorAverageResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getIPoeEmulatorAverageResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPoeEmulatorAverageResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPoeEmulatorMinimumResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getIPoeEmulatorMinimumResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPoeEmulatorMinimumResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPoeEmulatorMaximumResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getIPoeEmulatorMaximumResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPoeEmulatorMaximumResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

