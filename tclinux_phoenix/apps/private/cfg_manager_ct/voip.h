#define VOIP_LINE_NUM 2
#define SUPPORT_CODEC_NUM 4
#define VOIP_SIMULATE_TEST_NUM 4
#define VOIP_IMS_NUM           4
#define VOIP_POOR_QL_NUM       4
#define VOIP_INFO_NUM          4
/* serena_modify */
#define PHONE_BOOK_NUM 10
/* serena_modify end*/
#define VOIPBASIC "VoIPBasic"
#define VOIPADVANCED "VoIPAdvanced"
#define VOIPCALLCTRL "VoIPCallCtrl"
#define VOIPMEDIA "VoIPMedia"
#define VOIPSPEED "VoIPSpeed"
#define VOIPCODECS "VoIPCodecs"
/* jrchen add 20100222*/
#define VOIPLOG		"VoIPLog"
#define VOIPLOGSUB 	"Entry"
#define LOGENABLE 	"LogEnable"
#define LOGSERVERIP	"LogServerIP"
#define LOGSERVERPORT	"LogServerPort"
#define LOGDISPLAY	"LogDisplay"
#define VOIPDIGITMAP "VoIPDigitMap"
#define VOIPENTRY "Entry"
#define VOIPTEST  "VoIPTest"
#define VOIPSIMULATETEST "VoIPSimulateTest"
#define VOIPH248    "VoIPH248"
#define VOIPIMS     "VoIPIMS"
#define VOIPDIAGNOSTIC "VoIPDiagnostic"
#define VOIPPOORQL    "InfoVoIPPoorQL"
#define VOIPINFO      "InfoVoIP"

#define SIPCONFPATH "/etc/SIPPhone.cfg"
#define MEDIAENTRY_NUM 4
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
#define CALLFORWARDTYPE		"CallForwardType"
#define CALLFORWARDINGNUMBER	"CallForwardingNumber"
#define CALLTRANSFER		"CallTransfer"
#define CALLHOLD		"CallHold"
#define CALLRETURN		"CallReturn"
#define	THREEWAYCONF		"ThreeWayConf"
#define	MWIENABLE		"MWIEnable"
#define SIP_YES			"Yes"
#define LINE_TEST_REQ 	"Requested"
#define LINE_TEST_PHONE_CONN "PhoneConnectivityTest"

#define DIGITMAPENABLE	"DigitMapEnable"
#define DIGITMAP	"DigitMap"
#define DIALTIMEOUT	"DialTimeout"
#define DIGITTIMEOUT	"DigitTimeout"

#define DIGITMAP_DML	"InterDigitTimerLong"
#define DIGITMAP_DMS	"InterDigitTimerShort"
#define DIGITMAP_MIN_DMLS		1
#define DIGITMAP_MAX_DMLS		40
#define DIGITMAP_DEFAULT_DML	20
#define DIGITMAP_DEFAULT_DMS	5
#define DIGITMAP_SET_FLAG_0		0
#define DIGITMAP_SET_FLAG_1		1

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
void put_call_ctrl(FILE *fp,mxml_node_t *top);
void put_media(FILE *fp,mxml_node_t *top,char *local_addr,char *local_rtp_port);
void put_phone_number(FILE *fp,mxml_node_t *top);

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

int voip_advanced_init(void);
int voip_advanced_write(mxml_node_t *top, mxml_node_t *parant);
int voip_advanced_verify(mxml_node_t *node);
int voip_advanced_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int voip_test_init(void);
int voip_test_write(mxml_node_t *top, mxml_node_t *parant);
int voip_test_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int voip_simulate_test_init(void);
int voip_simulate_test_write(mxml_node_t *top, mxml_node_t *parant);
int voip_simulate_test_verify(mxml_node_t *node);
int voip_simulate_test_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int voip_h248_init(void);
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
