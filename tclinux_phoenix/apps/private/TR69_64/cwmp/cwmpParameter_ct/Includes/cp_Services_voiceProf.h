#ifndef CP_SERVICES_VOICEPROF_H_
#define CP_SERVICES_VOICEPROF_H_

#include "cp_core.h"

#if defined(TCSUPPORT_CT_CWMP_WORK_COMPATIBLE)
#if defined(TCSUPPORT_CT_1FXS)
#define LINENUM					1
#else
#define LINENUM					2
#endif
#else
#if defined(TCSUPPORT_CT_PON_CY)||defined(TCSUPPORT_CUC_VOIP_NODE)||defined(TCSUPPORT_CT_PON_C9)
#if defined(TCSUPPORT_CT_PON_CY) && !defined(TCSUPPORT_CT_PON_JX) && !defined(TCSUPPORT_CT_PON_CY_JX)
#define LINENUM					2
#else
#define LINENUM					1
#endif
#elif defined(TCSUPPORT_CT_1FXS)
#define LINENUM					1
#else
#define LINENUM					2
#endif
#endif


#define VOIPBASIC				"VoIPBasic"
#define VOIPBASIC_COMMON		"VoIPBasic_Common"
#define  VOIPH248                            "VoIPH248"
#define  VOIPH248_COMMON                     "VoIPH248_Common"


#define  MEDIA_GATEWAY_CONTROLER             "MediaGatewayControler"
#define  SB_MEDIA_GATEWAY_CONTROLER          "SBMediaGatewayControler"
#define  MEDIA_GATEWAY_CONTROLER_PORT        "MediaGatewayControlerPort"
#define  SB_MEDIA_GATEWAY_CONTROLER_PORT     "SBMediaGatewayControlerPort"

#define  MEDIA_GATEWAT_PORT                  "MediaGatewayPort"
#define  MESSAGE_ENCODING_TYPE               "MessageEncodingType"

#define  DEVICE_ID                           "DeviceID"
#define  DEVICE_ID_TYPE                      "DeviceIDType"
#define  PHYSICAL_TERMID                     "PhysicalTermID"
#define  PHYSICAL_TERMID_START               "PhysicalTermIDStart" 
#define  PHYSICAL_TERMID_CONFIG_METHOD       "PhysicalTermIDConfigMethod"
#define  PHYSICAL_TERMID_PREFIX              "PhysicalTermIDPrefix"
#define  PHYSICAL_TERMID_ADDLEN              "PhysicalTermIDAddLen"
#define  RTP_PREFIX                          "RTPPrefix"
#define  EPHEMERAL_TERMID_ADDLEN             "EphemeralTermIDAddLen"
#define  EPHEMERAL_TERMID_UNIFORM            "EphemeralTermIDUniform"
#define  EPHEMERAL_TERMID_START              "EphemeralTermIDStart"
#define  THREE_HAND_SHAKE                    "ThreeHandShake"
#define  LONG_TIMER                          "LongTimer"
#define  PENDING_TIMER_INIT                  "PendingTimerInit"
#define  RETRAN_INTERVAL_TIMER               "RetranIntervalTimer"
#define  MAX_RETRAN_COUNT                    "MaxRetranCount"
#define  RETRANS_MISSION_TIME                "RetransmissionTime"
#define  RETRANS_MISSION_CYCLE               "RetransmissionCycle"
#define  HEARTBEAT_MODE                      "HeartbeatMode"
#define  HEARTBEAT_CYCLE_H248                     "HeartBeatTimer"
#define  HEARTBEAT_COUNT_H248                     "HeartBeatRetransTimes"
#define  HEARTBEAT_CYCLE_NOH248                     "HeartbeatCycle"
#define  HEARTBEAT_COUNT_NOH248                     "HeartbeatCount"
#define  REGISTER_CYCLE                      "RegisterCycle"
#define  MANDATORY_REGISTER                  "MandatoryRegister"
#define  AUTHENTICATION_METHID               "AuthenticationMethID"

#define  DEVICE_NAME                         "DeviceName"
#define  MEGACO_SIGNAL_DSCP                  "MegacoSignalDSCP"
#define  MEGACO_RTP_DSCP                     "MegacoRTPDSCP"

#define  AUTH                                "auth"
#define  AUTH_INIT_KEY                       "authInitKey"
#define  AUTH_MGID                           "authmgid"
#define  AUTH_HEADER                         "authHeader"
#define  HEADER_SECURITY_TYPE                "HeaderSecurityType"
#define  RFC2833_ENCRYPT_KEY                 "Rfc2833EncryptKey"

#define  MSG_FORMAT                          "MsgFormat"
#define  UDP_ALF_GLAG                        "UdpAlfFlag"

#define  MAX_RETRANS_TIME                    "MaxreTransTime"
#define  MIN_RETRANS_TIME                    "MinreTransTime"
#define  MG_PROVISIONAL_RESP_TIME            "MGProvisionalRespTime"
#define  MTU_VALUE                           "MTUValue"
#define  MSG_SEGMENTATION                    "MsgSegmentation"
#define  HEARTBEAT_RETRANS_TIMER             "HeartBeatRetransTimer"
#define  MWD_BASE_TIME                       "MWDBaseTime"
#define  MAX_START_DELAY                     "MaxStartDelay"
#define  MGC_SWITCH_MATHOD                   "MgcSwitchMethod"

#define  HEARTBEAT_SEND_MODE                 "HeartbeatSendMode"
#define  HEARTBEAT_RETRANS_TIMES             "HeartBeatRetransTimes"

#if defined(TCSUPPORT_CT)
/*********************IAD*********************************************/
#define CWMP_VOIPSYSPARAM_COMMON            "VoIPSysParam_Common"
#define CWMP_VOIP_WAN_IFNAME                "SC_SYS_CFG_WAN_IFNAME"
#define CWMP_VOIP_WAN_IP                    "SC_SYS_CFG_WAN_IP"
#define CWMP_VOIPBASIC_COMMON               "VoIPBasic_Common"
#define CWMP_SIP_PROXY_ADDR                 "SBRegistrarServer"
#define CWMP_SIP_REG_SERVER                 "RegistrarServer"
#define CWMP_VOIPH248_COMMON                "VoIPH248_Common"
#define CWMP_H248_MGC                       "MediaGatewayControler"
#define CWMP_H248_SB_MGC                    "SBMediaGatewayControler"

#define IAD_TEST_RESULT_PATH            "/tmp/IADTestResult"
#define IAD_SIP_DEL_CMD                 "/userfs/bin/sipclient -d %d"
#define IAD_SIP_UPDATE_CMD              "/userfs/bin/sipclient -u %d"
#define IAD_SIP_EMULTE_REG_CMD          "/userfs/bin/sipclient -e \"%d %d\""
#define IAD_SIP_KILLALL_CMD             "killall -9 sipclient"
#define IAD_SIP_START_CMD               "/userfs/bin/sipclient &"
#define IAD_TIMEOUT                     (60 * 2)
#define IAD_SIP_LINE_ID                 (0)


typedef enum {
    IAD_ERROR_NONE,
    IAD_INNER_ERROR,        /* 1- IAD inner error */
    IAD_ROUTE_FAIL,
    IAD_SERVER_UNREACHABLE,
    IAD_ACCOUNT_FAIL,
    IAD_OTHER_ERROR,        /* 5- other error*/
    IAD_ERRORMAX,
}IAD_ERROR_E;

typedef struct IADDiagnose_s{
    char state[16];             /* None/Requested/Commplete, default:None */
    unsigned int testServer;    /* 1-major server, 2-minor server */
    unsigned int result;        /* 0-success, 1-fail */
    unsigned int reason;        /* 1-IAD error, 2-route fail, 3-sever not response
                                                                      4-user/pwd error, 5-other error */
}IADDiagnose_t;

#endif

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.VoiceProfile.Line.
	Parent Node Table Location: cp_Services_voiceProf_nodetable.c
*/
extern CwmpParameterStruct LineIndex[];

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.VoiceProfile.Line.{i}.
	Parent Node Table Location: cp_Services_voiceProf_nodetable.c
*/
extern CwmpParameterStruct Line[];

#if defined(TCSUPPORT_CUC_VOIP_NODE)
/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.SIP
	Parent Node Table Location: cp_Services_voiceProf_nodetable.c
*/
extern CwmpParameterStruct ResponseMapIndex[];
extern CwmpParameterStruct ResponseMap[];

int8 getLocalPortMinValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLocalPortMinValue(void* value, struct DeviceNode*  deviceNode);
int8 getLocalPortMinAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLocalPortMinAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLocalPortMaxValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLocalPortMaxValue(void* value, struct DeviceNode*  deviceNode);
int8 getLocalPortMaxAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLocalPortMaxAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTelephoneEventPayloadTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTelephoneEventPayloadTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getTelephoneEventPayloadTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTelephoneEventPayloadTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRTPX_CU_WANNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getRTPX_CU_WANNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRTPX_CU_WANNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRtcpEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRtcpEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getRtcpEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRtcpEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTxRepeatIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTxRepeatIntervalValue(void* value, struct DeviceNode*  deviceNode);
int8 getTxRepeatIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTxRepeatIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getX_CU_RtcpxrFlagValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setX_CU_RtcpxrFlagValue(void* value, struct DeviceNode*  deviceNode);
int8 getX_CU_RtcpxrFlagAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setX_CU_RtcpxrFlagAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRdnEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRdnEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getRdnEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRdnEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPayloadTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPayloadTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getPayloadTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPayloadTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getX_CU_EnableFixedStartValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setX_CU_EnableFixedStartValue(void* value, struct DeviceNode*  deviceNode);
int8 getX_CU_EnableFixedStartAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setX_CU_EnableFixedStartAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif
int8 findVoicePVCEntryIdx(int* pvc_index,int* entry_index);

#if defined(TCSUPPORT_CT_PON_SC)
int8 getCallIdTimePolicyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCallIdTimePolicyValue(void* value, struct DeviceNode*  deviceNode);
#endif

int8 getSIPProxyServerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSIPProxyServerValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPProxyServerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPProxyServerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPProxyServerPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSIPProxyServerPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPProxyServerPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPProxyServerPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPProxyServerTransportValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSIPProxyServerTransportValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPProxyServerTransportAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPProxyServerTransportAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPRegistrarServerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSIPRegistrarServerValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPRegistrarServerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPRegistrarServerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPRegistrarServerPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSIPRegistrarServerPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPRegistrarServerPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPRegistrarServerPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPRegistrarServerTransportValue(char** value, ParameterType* type, DeviceNode*  deviceNode);	
int8 setSIPRegistrarServerTransportValue(void* value, struct DeviceNode*  deviceNode);	
int8 getSIPRegistrarServerTransportAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPRegistrarServerTransportAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getOutboundProxyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);		
int8 setOutboundProxyValue(void* value, struct DeviceNode*  deviceNode);
int8 getOutboundProxyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setOutboundProxyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getOutboundProxyPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setOutboundProxyPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getOutboundProxyPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setOutboundProxyPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getProfileFaxT38Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setProfileFaxT38Value(void* value, struct DeviceNode*  deviceNode);
int8 getProfileFaxT38Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setProfileFaxT38Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 addLineObject(uint32* num, DeviceNode*  deviceNode);
int	cwmpInitLineSubTree( DeviceNodePtr deviceNode );

int8 getRTPVLANIDMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRTPVLANIDMarkValue(void* value, struct DeviceNode*  deviceNode);
int8 getRTPVLANIDMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRTPVLANIDMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRTPlpMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRTPlpMarkValue(void* value, struct DeviceNode*  deviceNode);
int8 getRTPlpMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRTPlpMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRTPDSCPMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRTPDSCPMarkValue(void* value, struct DeviceNode*  deviceNode);
int8 getRTPDSCPMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRTPDSCPMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CT)
int8 getIADDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIADDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode);
int8 getIADDiagnosticsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIADDiagnosticsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTestServerValue(char * * value,ParameterType * type,DeviceNode * deviceNode);
int8 setTestServerValue(void* value, struct DeviceNode*  deviceNode);
int8 getTestServerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTestServerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRegistResultValue(char * * value,ParameterType * type,DeviceNode * deviceNode);
int8 getRegistResultAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRegistResultAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getReasonValue(char * * value,ParameterType * type,DeviceNode * deviceNode);
int8 getReasonAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setReasonAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPStandby_ProxyServerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSIPStandby_ProxyServerValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPStandby_ProxyServerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPStandby_ProxyServerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPStandby_ProxyServerPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSIPStandby_ProxyServerPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPStandby_ProxyServerPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPStandby_ProxyServerPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPStandby_ProxyServerTransportValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSIPStandby_ProxyServerTransportValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPStandby_ProxyServerTransportAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPStandby_ProxyServerTransportAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPStandby_RegistrarServerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);	
int8 setSIPStandby_RegistrarServerValue(void* value, struct DeviceNode*  deviceNode);	
int8 getSIPStandby_RegistrarServerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPStandby_RegistrarServerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPStandby_RegistrarServerPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);	
int8 setSIPStandby_RegistrarServerPortValue(void* value, struct DeviceNode*  deviceNode);	
int8 getSIPStandby_RegistrarServerPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPStandby_RegistrarServerPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPStandby_RegistrarServerTransportValue(char** value, ParameterType* type, DeviceNode*  deviceNode);	
int8 setSIPStandby_RegistrarServerTransportValue(void* value, struct DeviceNode*  deviceNode);	
int8 getSIPStandby_RegistrarServerTransportAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPStandby_RegistrarServerTransportAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPStandby_OutboundProxyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);		
int8 setSIPStandby_OutboundProxyValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPStandby_OutboundProxyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPStandby_OutboundProxyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPStandby_OutboundProxyPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);		
int8 setSIPStandby_OutboundProxyPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPStandby_OutboundProxyPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPStandby_OutboundProxyPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPlpMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode);		
int8 setSIPlpMarkValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPlpMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPlpMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPHeartbeatSwitchValue(char** value, ParameterType* type, DeviceNode*  deviceNode);		
int8 setSIPHeartbeatSwitchValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPHeartbeatSwitchAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPHeartbeatSwitchAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPHeartbearCycleValue(char** value, ParameterType* type, DeviceNode*  deviceNode);		
int8 setSIPHeartbearCycleValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPHeartbearCycleAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPHeartbearCycleAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPHeartbearCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode);		
int8 setSIPHeartbearCountValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPHeartbearCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPHeartbearCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPSessionUpdateTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);		
int8 setSIPSessionUpdateTimerValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPSessionUpdateTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPSessionUpdateTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

int8 getSIPDSCPMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode);		
int8 setSIPDSCPMarkValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPDSCPMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPDSCPMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getProfileFaxT38Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setProfileFaxT38Value(void* value, struct DeviceNode*  deviceNode);
int8 getProfileFaxT38Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setProfileFaxT38Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CUC_VOIP_NODE)
int8 getBitRateFaxT38Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setBitRateFaxT38Value(void* value, struct DeviceNode*  deviceNode);
int8 getBitRateFaxT38Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setBitRateFaxT38Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTCFMethodValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTCFMethodValue(void* value, struct DeviceNode*  deviceNode);
int8 getTCFMethodAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTCFMethodAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHighSpeedRedundancyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setHighSpeedRedundancyValue(void* value, struct DeviceNode*  deviceNode);
int8 getHighSpeedRedundancyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHighSpeedRedundancyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLowSpeedRedundancyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLowSpeedRedundancyValue(void* value, struct DeviceNode*  deviceNode);
int8 getLowSpeedRedundancyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLowSpeedRedundancyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getX_CU_PortAdd2Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setX_CU_PortAdd2Value(void* value, struct DeviceNode*  deviceNode);
int8 getX_CU_PortAdd2Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setX_CU_PortAdd2Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif


int	cwmpInitLineSubTree( DeviceNodePtr deviceNode );

int8 getSIPUserAgentDomainValue(char** value, ParameterType* type, DeviceNode*  deviceNode);		
int8 setSIPUserAgentDomainValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPUserAgentDomainAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPUserAgentDomainAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPUserAgentPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);		
int8 setSIPUserAgentPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPUserAgentPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPUserAgentPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPUserAgentTransportValue(char** value, ParameterType* type, DeviceNode*  deviceNode);		
int8 setSIPUserAgentTransportValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPUserAgentTransportAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPUserAgentTransportAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPVLANIDMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode);		
int8 setSIPVLANIDMarkValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPVLANIDMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPVLANIDMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPRegisterRetryIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);		
int8 setSIPRegisterRetryIntervalValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPRegisterRetryIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPRegisterRetryIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPRegisterExpiresValue(char** value, ParameterType* type, DeviceNode*  deviceNode);		
int8 setSIPRegisterExpiresValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPRegisterExpiresAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPRegisterExpiresAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#if defined(TCSUPPORT_VOIP_IMSSIP)//xflu_20140228

int8 getSIPImplicitRegistrationEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);		
int8 setSIPImplicitRegistrationEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPImplicitRegistrationEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPImplicitRegistrationEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CT_PON_SC)
int8 getRingVoltageValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRingVoltageValue(void* value, struct DeviceNode*  deviceNode);

int8 getCallIDMsgTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCallIDMsgTypeValue(void* value, struct DeviceNode*  deviceNode);
#endif

int8 getDigitMapEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDigitMapEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getDigitMapEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDigitMapEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDigitMapValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDigitMapValue(void* value, struct DeviceNode*  deviceNode);
int8 getDigitMapAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDigitMapAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if defined(TCSUPPORT_CT_PON_GS)
int8 getDigitMapMatchModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDigitMapMatchModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getDigitMapMatchModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDigitMapMatchModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CT)
int8 getDigitMapSpecialEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDigitMapSpecialEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getDigitMapSpecialEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDigitMapSpecialEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDigitMapSpecialValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDigitMapSpecialValue(void* value, struct DeviceNode*  deviceNode);
int8 getDigitMapSpecialAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDigitMapSpecialAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getServerTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setServerTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getServerTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setServerTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int8 getH248DMNameFuncsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setH248DMNameFuncsValue(void* value, struct DeviceNode*  deviceNode);
int8 getH248DMNameFuncsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setH248DMNameFuncsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getH248DigitMapFuncsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setH248DigitMapFuncsValue(void* value, struct DeviceNode*  deviceNode);
int8 getH248DigitMapFuncsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setH248DigitMapFuncsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getStartDigitTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setStartDigitTimerValue(void* value, struct DeviceNode*  deviceNode);
int8 getStartDigitTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setStartDigitTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getInterDigitTimerShortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setInterDigitTimerShortValue(void* value, struct DeviceNode*  deviceNode);
int8 getInterDigitTimerShortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setInterDigitTimerShortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getInterDigitTimerLongValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setInterDigitTimerLongValue(void* value, struct DeviceNode*  deviceNode);
int8 getInterDigitTimerLongAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setInterDigitTimerLongAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHangingReminderToneTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setHangingReminderToneTimerValue(void* value, struct DeviceNode*  deviceNode);
int8 getHangingReminderToneTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHangingReminderToneTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getBusyToneTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setBusyToneTimerValue(void* value, struct DeviceNode*  deviceNode);
int8 getBusyToneTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setBusyToneTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getNoAnswerTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setNoAnswerTimerValue(void* value, struct DeviceNode*  deviceNode);
int8 getNoAnswerTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setNoAnswerTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMediaGatewayControlerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMediaGatewayControlerValue(void* value, struct DeviceNode*	deviceNode);
int8 getMediaGatewayControlerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMediaGatewayControlerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMediaGatewayControlerPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMediaGatewayControlerPortValue(void* value, struct DeviceNode*	deviceNode);
int8 getMediaGatewayControlerPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMediaGatewayControlerPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getStandby_MediaGatewayControlerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setStandby_MediaGatewayControlerValue(void* value, struct DeviceNode*	deviceNode);
int8 getStandby_MediaGatewayControlerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setStandby_MediaGatewayControlerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getStandby_MediaGatewayControlerPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setStandby_MediaGatewayControlerPortValue(void* value, struct DeviceNode*	deviceNode);
int8 getStandby_MediaGatewayControlerPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setStandby_MediaGatewayControlerPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMediaGatewayPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMediaGatewayPortValue(void* value, struct DeviceNode*	deviceNode);
int8 getMediaGatewayPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMediaGatewayPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMessageEncodingTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMessageEncodingTypeValue(void* value, struct DeviceNode*	deviceNode);
int8 getMessageEncodingTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMessageEncodingTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDeviceIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDeviceIDValue(void* value, struct DeviceNode*	deviceNode);
int8 getDeviceIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDeviceIDTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDeviceIDTypeValue(void* value, struct DeviceNode*	deviceNode);
int8 getDeviceIDTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceIDTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPhysicalTermIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPhysicalTermIDValue(void* value, struct DeviceNode*	deviceNode);
int8 getPhysicalTermIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPhysicalTermIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPhysicalTermIDConfigMethodValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPhysicalTermIDConfigMethodValue(void* value, struct DeviceNode*	deviceNode);
int8 getPhysicalTermIDConfigMethodAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPhysicalTermIDConfigMethodAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPhysicalTermIDPrefixValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPhysicalTermIDPrefixValue(void* value, struct DeviceNode*	deviceNode);
int8 getPhysicalTermIDPrefixAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPhysicalTermIDPrefixAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPhysicalTermIDAddLenValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPhysicalTermIDAddLenValue(void* value, struct DeviceNode*	deviceNode);
int8 getPhysicalTermIDAddLenAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPhysicalTermIDAddLenAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRTPPrefixValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRTPPrefixValue(void* value, struct DeviceNode*	deviceNode);
int8 getRTPPrefixAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRTPPrefixAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getEphemeralTermIDAddLenValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setEphemeralTermIDAddLenValue(void* value, struct DeviceNode*	deviceNode);
int8 getEphemeralTermIDAddLenAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEphemeralTermIDAddLenAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getEphemeralTermIDUniformValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setEphemeralTermIDUniformValue(void* value, struct DeviceNode*	deviceNode);
int8 getEphemeralTermIDUniformAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEphemeralTermIDUniformAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getEphemeralTermIDStartValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setEphemeralTermIDStartValue(void* value, struct DeviceNode*	deviceNode);
int8 getEphemeralTermIDStartAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEphemeralTermIDStartAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getThreeHandShakeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setThreeHandShakeValue(void* value, struct DeviceNode*	deviceNode);
int8 getThreeHandShakeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setThreeHandShakeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLongTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLongTimerValue(void* value, struct DeviceNode*	deviceNode);
int8 getLongTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLongTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPendingTimerInitValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPendingTimerInitValue(void* value, struct DeviceNode*	deviceNode);
int8 getPendingTimerInitAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPendingTimerInitAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRetranIntervalTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRetranIntervalTimerValue(void* value, struct DeviceNode*	deviceNode);
int8 getRetranIntervalTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRetranIntervalTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMaxRetranCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMaxRetranCountValue(void* value, struct DeviceNode*	deviceNode);
int8 getMaxRetranCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMaxRetranCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRetransmissionTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRetransmissionTimeValue(void* value, struct DeviceNode*	deviceNode);
int8 getRetransmissionTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRetransmissionTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRetransmissionCycleValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRetransmissionCycleValue(void* value, struct DeviceNode*	deviceNode);
int8 getRetransmissionCycleAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRetransmissionCycleAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHeartbeatModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setHeartbeatModeValue(void* value, struct DeviceNode*	deviceNode);
int8 getHeartbeatModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHeartbeatModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHeartbeatCycleValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setHeartbeatCycleValue(void* value, struct DeviceNode*	deviceNode);
int8 getHeartbeatCycleAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHeartbeatCycleAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHeartbeatCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setHeartbeatCountValue(void* value, struct DeviceNode*	deviceNode);
int8 getHeartbeatCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHeartbeatCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRegisterCycleValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRegisterCycleValue(void* value, struct DeviceNode*	deviceNode);
int8 getRegisterCycleAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRegisterCycleAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMandatoryRegisterValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMandatoryRegisterValue(void* value, struct DeviceNode*	deviceNode);
int8 getMandatoryRegisterAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMandatoryRegisterAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAuthenticationMethIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setAuthenticationMethIDValue(void* value, struct DeviceNode*	deviceNode);
int8 getAuthenticationMethIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAuthenticationMethIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCOM_NumberPlanValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setCOM_NumberPlanValue(void* value, struct DeviceNode*	deviceNode);
int8 getCOM_NumberPlanAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCOM_NumberPlanAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getG711FAXEnableValue(char** value, ParameterType* type, DeviceNode*	deviceNode);
int8 setG711FAXEnableValue(char** value, ParameterType* type, DeviceNode*	deviceNode);
int8 getG711FAXEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setG711FAXEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*	deviceNode);

int8 getControlTypeValue(char** value, ParameterType* type, DeviceNode*	deviceNode);
int8 setControlTypeValue(void* value, struct DeviceNode* deviceNode);
int8 getControlTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setControlTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*	deviceNode);
#endif

#if defined(TCSUPPORT_CT_PON_GS)
int8 getProfileBodyValue(char** value, ParameterType* type, DeviceNode*	deviceNode);
int8 setProfileBodyValue(void* value, struct DeviceNode* deviceNode);
#endif

#if CHINA_UNICOM_ENABLE
int8 getDeviceNameValue(char **value, ParameterType *type, DeviceNode *deviceNode);
int8 setDeviceNameValue(void *value, struct DeviceNode *deviceNode);
int8 getDeviceNameAttribute(uint8 *notification, AccessEntry **acl, DeviceNode *deviceNode);
int8 setDeviceNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry *acl, struct DeviceNode *deviceNode);

int8 getMIDFormatValue(char **value, ParameterType *type, DeviceNode *deviceNode);
int8 setMIDFormatValue(void *value, struct DeviceNode *deviceNode);
int8 getMIDFormatAttribute(uint8 *notification, AccessEntry **acl, DeviceNode *deviceNode);
int8 setMIDFormatAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry *acl, struct DeviceNode *deviceNode);


int8 getDSCPMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDSCPMarkValue(void* value, struct DeviceNode*  deviceNode);
int8 getDSCPMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDSCPMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSoftswitchVendorValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSoftswitchVendorValue(void* value, struct DeviceNode*  deviceNode);
int8 getSoftswitchVendorAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSoftswitchVendorAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSoftswitchVersionValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSoftswitchVersionValue(void* value, struct DeviceNode*  deviceNode);
int8 getSoftswitchVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSoftswitchVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int8 getauthValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setauthValue(void* value, struct DeviceNode*  deviceNode);
int8 getauthAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setauthAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getauthInitKeyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setauthInitKeyValue(void* value, struct DeviceNode*  deviceNode);
int8 getauthInitKeyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setauthInitKeyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getauthmgidValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setauthmgidValue(void* value, struct DeviceNode*  deviceNode);
int8 getauthmgidAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setauthmgidAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getauthHeaderValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setauthHeaderValue(void* value, struct DeviceNode*  deviceNode);
int8 getauthHeaderAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setauthHeaderAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHeaderSecurityTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setHeaderSecurityTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getHeaderSecurityTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHeaderSecurityTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getRfc2833EncryptKeyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setRfc2833EncryptKeyValue(void* value, struct DeviceNode*  deviceNode);
int8 setRfc2833EncryptKeyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getRfc2833EncryptKeyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);

int8 getMsgFormatValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMsgFormatValue(void* value, struct DeviceNode*  deviceNode);
int8 getMsgFormatAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMsgFormatAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUdpAlfFlagValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUdpAlfFlagValue(void* value, struct DeviceNode*  deviceNode);
int8 getUdpAlfFlagAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUdpAlfFlagAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getResponseAckCtrlValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setResponseAckCtrlValue(void* value, struct DeviceNode*  deviceNode);
int8 getResponseAckCtrlAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setResponseAckCtrlAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMaxreTransTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMaxreTransTimeValue(void* value, struct DeviceNode*  deviceNode);
int8 getMaxreTransTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMaxreTransTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMinreTransTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMinreTransTimeValue(void* value, struct DeviceNode*  deviceNode);
int8 getMinreTransTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMinreTransTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMGProvisionalRespTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMGProvisionalRespTimeValue(void* value, struct DeviceNode*  deviceNode);
int8 getMGProvisionalRespTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMGProvisionalRespTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMTUValueValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMTUValueValue(void* value, struct DeviceNode*  deviceNode);
int8 getMTUValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMTUValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMsgSegmentationValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMsgSegmentationValue(void* value, struct DeviceNode*  deviceNode);
int8 getMsgSegmentationAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMsgSegmentationAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHeartBeatRetransTimerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setHeartBeatRetransTimerValue(void* value, struct DeviceNode*  deviceNode);
int8 getHeartBeatRetransTimerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHeartBeatRetransTimerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMWDBaseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMWDBaseTimeValue(void* value, struct DeviceNode*  deviceNode);
int8 getMWDBaseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMWDBaseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMWDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMWDValue(void* value, struct DeviceNode*  deviceNode);
int8 getMWDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMWDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMgcSwitchMethodValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMgcSwitchMethodValue(void* value, struct DeviceNode*  deviceNode);
int8 getMgcSwitchMethodAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMgcSwitchMethodAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


#endif
#if defined(TCSUPPORT_CUC_VOIP_NODE)
int8 getEnableVoipValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setEnableVoipValue(void* value, struct DeviceNode*  deviceNode);
int8 getEnableVoipAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEnableVoipAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getResetValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setResetValue(void* value, struct DeviceNode*  deviceNode);
int8 getResetAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setResetAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getNumberOfLinesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getNumberOfLinesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setNumberOfLinesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSignalingProtocolValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSignalingProtocolValue(void* value, struct DeviceNode*  deviceNode);
int8 getSignalingProtocolAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSignalingProtocolAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMaxSessionsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMaxSessionsValue(void* value, struct DeviceNode*  deviceNode);
int8 getMaxSessionsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMaxSessionsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDTMFMethodValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDTMFMethodValue(void* value, struct DeviceNode*  deviceNode);
int8 getDTMFMethodAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDTMFMethodAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDTMFMethodG711Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDTMFMethodG711Value(void* value, struct DeviceNode*  deviceNode);
int8 getDTMFMethodG711Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDTMFMethodG711Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getX_CU_DigitMapMatchModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setX_CU_DigitMapMatchModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getX_CU_DigitMapMatchModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setX_CU_DigitMapMatchModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getX_CU_WANNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getX_CU_WANNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setX_CU_WANNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getX_CU_InterfaceStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getX_CU_InterfaceStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setX_CU_InterfaceStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getServerType_CUValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setServerType_CUValue(void* value, struct DeviceNode*  deviceNode);
int8 getServerType_CUAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setServerType_CUAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getStartDigitTimer_CUValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setStartDigitTimer_CUValue(void* value, struct DeviceNode*  deviceNode);
int8 getStartDigitTimer_CUAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setStartDigitTimer_CUAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getInterDigitTimerShort_CUValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setInterDigitTimerShort_CUValue(void* value, struct DeviceNode*  deviceNode);
int8 getInterDigitTimerShort_CUAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setInterDigitTimerShort_CUAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getInterDigitTimerLong_CUValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setInterDigitTimerLong_CUValue(void* value, struct DeviceNode*  deviceNode);
int8 getInterDigitTimerLong_CUAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setInterDigitTimerLong_CUAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHangingReminderToneTimer_CUValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setHangingReminderToneTimer_CUValue(void* value, struct DeviceNode*  deviceNode);
int8 getHangingReminderToneTimer_CUAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHangingReminderToneTimer_CUAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getBusyToneTimer_CUValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setBusyToneTimer_CUValue(void* value, struct DeviceNode*  deviceNode);
int8 getBusyToneTimer_CUAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setBusyToneTimer_CUAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getNoAnswerTimer_CUValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setNoAnswerTimer_CUValue(void* value, struct DeviceNode*  deviceNode);
int8 getNoAnswerTimer_CUAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setNoAnswerTimer_CUAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#if defined(TCSUPPORT_CUC_VOIP_NODE)
int8 getTimerT1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTimerT1Value(void* value, struct DeviceNode*  deviceNode);
int8 getTimerT1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTimerT1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTimerT2Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTimerT2Value(void* value, struct DeviceNode*  deviceNode);
int8 getTimerT2Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTimerT2Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTimerT4Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTimerT4Value(void* value, struct DeviceNode*  deviceNode);
int8 getTimerT4Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTimerT4Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUseCodecPriorityInSDPResponseValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUseCodecPriorityInSDPResponseValue(void* value, struct DeviceNode*  deviceNode);
int8 getUseCodecPriorityInSDPResponseAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUseCodecPriorityInSDPResponseAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPResponseNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSIPResponseNumberValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPResponseNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPResponseNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getToneValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setToneValue(void* value, struct DeviceNode*  deviceNode);
int8 getToneAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setToneAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getX_CU_DurationValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setX_CU_DurationValue(void* value, struct DeviceNode*  deviceNode);
int8 getX_CU_DurationAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setX_CU_DurationAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int cwmpInitResponseMapFuncsSubTree( DeviceNodePtr deviceNode );
#endif

#endif

