#define VOIP_LINE_NUM 2
#define SUPPORT_CODEC_NUM 4
/* Add by mtk06404 */
#define VOIP_SIMULATE_TEST_NUM 4
#define VOIP_IMS_NUM           4
#define VOIP_POOR_QL_NUM       10   //add by mtk06404
#define VOIP_INFO_NUM          4

/* serena_modify */
#define PHONE_BOOK_NUM 10
/* serena_modify end*/
#define VOIPSYSPARAM "VoIPSysParam"
#define VOIPADVANCED "VoIPAdvanced"
#define VOIPBASIC "VoIPBasic"
#define VOIPCALLCTRL "VoIPCallCtrl"
#define VOIPMEDIA "VoIPMedia"
#define VOIPSPEED "VoIPSpeed"
#define VOIPCODECS "VoIPCodecs"
#define VOIPDIGITMAP "VoIPDigitMap"
#define VOIPENTRY "Entry"
/* Add by mtk06404 */
#define VOIPTEST  "VoIPTest"
#define VOIPSIMULATETEST "VoIPSimulateTest"
#define VOIPH248    "VoIPH248"
#define VOIPCOMMON    "Common"
#define VOIPIMS     "VoIPIMS"
#define VOIPDIAGNOSTIC "VoIPDiagnostic"
#define VOIPPOORQL    "InfoVoIPPoorQL"
#define VOIPINFO      "InfoVoIP"
#define LINE_TEST_REQ 	"Requested"
#define LINE_TEST_PHONE_CONN "PhoneConnectivityTest"

#define LINE_TEST_TID_LINE_V  "LineVoltageTest"     
#define LINE_TEST_TID_RES_FLT "LineResistiveTest" 
  
/* jrchen add 20100222*/
#define VOIPLOG		"VoIPLog"
#define VOIPLOGSUB 	"Entry"
#define LOGENABLE 	"LogEnable"
#define LOGSERVERIP	"LogServerIP"
#define LOGSERVERPORT	"LogServerPort"
#define LOGDISPLAY	"LogDisplay"


#define SIPCONFPATH "/etc/SIPPhone.cfg"
#define MEDIAENTRY_NUM 5
#define MAXSPEED_NUM 10
#define RETRY_TIME 3
#define MAXCODEC_NUM 10

/*string define*/
#define REGISTRARADDR 		"RegistrarAddress"
#define REGISTRARPORT 		"RegistrarPort"
#define OUTBOUNDPROXYADDR 	"OutboundProxyAddress"
#define OUTBOUNDPROXYPORT 	"OutboundProxyPort"
#define SIPTRANSPORTTYPE 	"TransportType"
#define SIPTCPENABLED 		"TcpEnabled"
#define SIPTCPPORT 		"TcpPort"
#define SIPUDPPORT 		"UdpPort"
#define PRACKENABLE 		"PRACKEnable"
#define CALLERIDENABLE 		"CallerIdEnable"
// #define CALLERIDTYPE 		"CallerIdType"
#define CALLWAITINGENABLE 	"CallWaitingEnable"
#define CALLWAITINGREPLY 	"CallWaitingReply"
#define CALLFORWARDENABLE 	"CallForwardEnable"
#define CFUNUMBER		"CFUNumber"
#define CFBNUMBER		"CFBNumber"
#define CFNRNUMBER		"CFNRNumber"
#define CALLTRANSFER		"CallTransfer"
#define BLINDTRANSFERNUM	"BlindTransferNumber"
#define ATTENDEDTRANSFERNUM	"AttendedTransferNumber"
#define CALLHOLD		"CallHold"
#define CALLRETURN		"CallReturn"
/* 20110110 PTChen add for call return */
#define CALLRETURNNUMBER	"CallReturnNumber"
/* 20110110 PTChen add for DND */
#define CALLDNDENABLE		"CallDNDEnable"
#define CALLDNDENABLENUMBER 	"CallDNDEnableNumber"
#define CALLDNDDISABLENUMBER 	"CallDNDDisableNumber"
/* 20110321 Pork add for DOD */
#define CALLDODENABLE		"CallDODEnable"
#define CALLDODNUMBER 		"CallDODNumber"




#define	THREEWAYCONF		"ThreeWayConf"
#define	THREEWAYCONFNUMBER	"ThreeWayConfNumber"
#define	MWIENABLE		"MWIEnable"
#define SIP_YES			"Yes"
/* 2010/12/20 Enhance by Sam */
#define REGISTRATION_EXPIRE	"RegistrationExpire"
#define RETRANSMIT_INTERVAL_T1	"retransmissionT1"
#define RETRANSMIT_INTERVAL_T2	"retransmissionT2"

/* 20101220 PTchen add  */
/* 2011/1/12 Enhance by Sam */
#define	ANONY_CALLBLOCK	        "AnonyCallBlock"
#define	ANONY_CALL		"AnonyCall"
#define	VoIP_DND		"DND"
/* 20101220 PTchen add end */


#define DIGITMAPENABLE	"DigitMapEnable"
#define DIGITMAP	"DigitMap"
#define DIALTIMEOUT	"DialTimeout"
#define DIGITTIMEOUT	"DigitTimeout"

#define MANDATORY_REGISTER                  "MandatoryRegister"
#define MANDATORY_REGISTER_FLAG             "MandatoryRegisterFlag"
#define MGC_CONTROLER            "MediaGatewayControler"
#define MGC_SB_CONTROLER         "SBMediaGatewayControler"

#define VOIPINFO_H248      "InfoVoIPH248"
#define RESETFLAG "ResetFlag"
/* mtk06404 add 20140102 */
#if defined(TCSUPPORT_CUC)
#define LINE_SIMULATE_TEST "X_CU_SimulateTest"
#else
#define LINE_SIMULATE_TEST "X_CT-COM_SimulateTest"
#endif

#define LINE_CALLER_SIMULATE "Caller"
#define LINE_CALLED_SIMULATE "Called"
#define LINE_NONE_SIMULATE "None"
#define SC_LINE_REG_STATUS  "Status"
#define LINE_SIMULATE_TYPE "simulateType"


/*for VoIPH248 node attribute mtk69055*/
#define  PHYSICAL_TERMID "PhysicalTermID"
#define H248_LINE_INFO_STATUS "lineInfoStatus"
#define H248_LINE_SIMULATE_DATA "simulateData"
#define H248_REGISTER_STATUS "UserServiceState"

#define LINE_TEST_PROC_NAME "/proc/fxs/sliclinetest"
#define SIMULATE_OFFHOOK    "/userfs/bin/sipclient -x \"%d %d\""
#define SIMULATE_DIAL    "/userfs/bin/sipclient -z \"%d %c\""
#define SIMULATE_PUSH_BUTTON    "/userfs/bin/sipclient -s \"%d %c\""
#define SIMULATE_ONHOOK    "/userfs/bin/sipclient -y %d"

/*for H248 Simulate mtk69055*/
#define SIMULATE_CMD_PREFIX "callCmd {term.SendEvent}{%s}%s"
#define OFFHOOK_SG    "{al}{of}"
#define ONHOOK_SG "{al}{on}"
#define SIMULATE_DIAL_PREFIX "callCmd {term.SendEvent}{%s}{dd}{d%c}"
#define SIMULATE_PUSH_DTMF_PREFIX "callCmd {term.Send2Api}{%s}{dd}{%c}"

#define SIMULATE_TEST_TIMEOUT   60       /* 60 second */
#define SIMULATE_CALL_HOLD_TIME  30
#define SIMULATE_CALLED_WAIT_TIME 30
#define SIMULATE_CALL_HOLD_DIGIT_TIME 20

#if defined(TCSUPPORT_ECN_SIP)
#define POOR_QL_PROC_PATH   "/proc/vdsp/debug/poorQlRecord"
#endif

enum test_selector{
	TEST_SELECTOR_NOE,
	PHONE_CONN_TEST,
	SIMULATE_TEST,
	TEST_SELECTOR_MAX,
};

#if defined(TCSUPPORT_ECN_SIP)
/*********record 10 poor quailty voice communication*************************/
typedef struct s_voip_poor_ql_attr{
    char         dateTime[32];
    unsigned int txPkgCnt;
    unsigned int rxPkgCnt;
    unsigned int meanDelay;
    unsigned int meanJitter;
    unsigned int fractionLoss;
    unsigned long localIPAddr;
    unsigned int localUdpPort;
    unsigned long remoteIPAddr;
    unsigned int remoteUdpPort;
    unsigned int mosLq;
    char         codec[16];
}voip_poor_ql_attr;

/*********************************************************************/

#endif

int voip_sys_param_init(void);
int voip_sys_param_boot(mxml_node_t *top);
int voip_sys_param_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int voip_sys_param_write(mxml_node_t *top, mxml_node_t *parant);
int voip_sys_param_verify(mxml_node_t *node);
int voip_sys_param_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);


int voip_advanced_init(void);
int voip_advanced_boot(mxml_node_t *top);
int voip_advanced_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int voip_advanced_write(mxml_node_t *top, mxml_node_t *parant);
int voip_advanced_verify(mxml_node_t *node);
int voip_advanced_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);




int voip_basic_init(void);
int voip_basic_boot(mxml_node_t *top);
int voip_basic_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int voip_basic_write(mxml_node_t *top, mxml_node_t *parant);
int voip_basic_verify(mxml_node_t *node);
int voip_basic_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int voip_call_ctrl_init(void);      
int voip_call_ctrl_write(mxml_node_t *top, mxml_node_t *parant);
int voip_call_ctrl_verify(mxml_node_t *node);
int voip_call_ctrl_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int voip_media_init(void);
int voip_media_write(mxml_node_t *top, mxml_node_t *parant);
int voip_media_verify(mxml_node_t *node);
int voip_media_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int voip_speed_dial_init(void);
int voip_speed_dial_write(mxml_node_t *top, mxml_node_t *parant);
int voip_speed_dial_verify(mxml_node_t *node);
int voip_speed_dial_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int voip_real_write(mxml_node_t *top);
int voip_real_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

void voip_put_str(FILE *fp, char *name, char *value);
int voip_get_payload_id(char *name);
void put_basic(FILE *fp,mxml_node_t *top);
/* PT Chen add 20101028 */
void put_advanced(FILE *fp,mxml_node_t *top);
void put_call_ctrl(FILE *fp,mxml_node_t *top);
void put_media(FILE *fp,mxml_node_t *top,char *local_addr,char *local_rtp_port);
void put_phone_number(FILE *fp,mxml_node_t *top);
void put_dial_plan(FILE *fp,mxml_node_t *top);
/* Enhance by sam 2010/12/20 */
void put_SIPStackParameters(FILE *fp,mxml_node_t *top);

int voip_codec_init(void);
/* jrchen add 20100222 */
int voip_log_init(void);
int voip_log_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int voip_log_write(mxml_node_t *top, mxml_node_t *parant);
void put_log(FILE *fp,mxml_node_t *top);

int voip_digitmap_init(void);
int voip_digitmap_write(mxml_node_t *top, mxml_node_t *parant);
int voip_digitmap_verify(mxml_node_t *node);
int voip_digitmap_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
void put_digitmap(FILE *fp,mxml_node_t *top);

/* Add by mtk06404 */
int voip_test_init(void);
int voip_test_boot(mxml_node_t *top);
int voip_test_write(mxml_node_t *top, mxml_node_t *parant);
int voip_test_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int voip_simulate_test_init(void);
int voip_simulate_test_write(mxml_node_t *top, mxml_node_t *parant);
int voip_simulate_test_verify(mxml_node_t *node);
int voip_simulate_test_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int voip_h248_init(void);
int voip_h248_boot(mxml_node_t *top);
int voip_h248_write(mxml_node_t *top, mxml_node_t *parant);
int voip_h248_verify(mxml_node_t *node);
int voip_h248_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int voip_ims_init(void);
int voip_ims_write(mxml_node_t *top, mxml_node_t *parant);
int voip_ims_verify(mxml_node_t *node);
int voip_ims_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int voip_diagnostic_init(void);
int voip_diagnostic_write(mxml_node_t *top, mxml_node_t *parant);
int voip_diagnostic_verify(mxml_node_t *node);
int voip_diagnostic_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int voip_poor_ql_init(void);
int voip_poor_ql_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char* attr);

int voip_info_init(void);
int voip_info_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char* attr);
int voip_info_execute(mxml_node_t *top);



#if defined(TCSUPPORT_ECN_MEGACO)
int voip_info_248_init(void);
int voip_info_248_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char* attr);
int voip_info_248_write(mxml_node_t *top, mxml_node_t *parant);
#endif

