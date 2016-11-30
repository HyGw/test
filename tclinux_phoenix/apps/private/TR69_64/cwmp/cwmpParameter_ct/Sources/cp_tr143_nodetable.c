#include "Global_res.h"
#include "cp_tr143.h"

/*******************  InternetGatewayDevice.Capabilities. start ************************/
static NodeFunc DownloadTransports=
{
	NULL,
	NULL,
	getDiagDldTsportsValue, 
	setDiagDldTsportsValue, 
	getDiagDldTsportsAttribute, 
	setDiagDldTsportsAttribute, 
	NULL,
};

static NodeFunc UploadTransports=
{
	NULL,
	NULL,
	getDiagUpTsportsValue, 
	setDiagUpTsportsValue, 
	getDiagUpTsportsAttribute, 
	setDiagUpTsportsAttribute, 
	NULL,
};

/*
	Parent Node: InternetGatewayDevice.Capabilities.PerformanceDiagnostic
	Parent Node Table Location: cp_tr143_nodetable.c 
*/
CwmpParameterStruct PerformanceDiagnostic[] = 
{
	{"DownloadTransports", StrW, NoLimite, &DownloadTransports, NULL},
	{"UploadTransports", StrW, NoLimite, &UploadTransports, NULL},
	{NULL, -1, -1, NULL, NULL}
};


/*
	Parent Node: InternetGatewayDevice.Capabilities.
	Parent Node Table Location: cp_topnodetable.c 
*/
CwmpParameterStruct tr143_Capabilities[] = 
{
	{"PerformanceDiagnostic", ObjectR, NoLimite, NULL, PerformanceDiagnostic},
	{NULL, -1, -1, NULL, NULL}
};
/*******************  InternetGatewayDevice.Capabilities. end ************************/

/*########################################################################################*/

/*******************  InternetGatewayDevice.DownloadDiagnostics. start **********************/

static NodeFunc DiagnosticsState_Dld=
{
	NULL,
	NULL,
	getDldDiagnosticsStateValue, 
	setDldDiagnosticsStateValue, 
	getDldDiagnosticsAttribute, 
	setDldDiagnosticsAttribute, 
	NULL,
};

static NodeFunc Interface_Dld=
{
	NULL,
	NULL,
	getDldInterfaceValue, 
	setDldInterfaceValue, 
	getDldInterfaceAttribute, 
	setDldInterfaceAttribute, 
	NULL,
};

static NodeFunc DownloadURL_Dld=
{
	NULL,
	NULL,
	getDldDownloadURLValue, 
	setDldDownloadURLValue, 
	getDldDownloadURLAttribute, 
	setDldDownloadURLAttribute, 
	NULL,
};

static NodeFunc DSCP_Dld=
{
	NULL,
	NULL,
	getDldDSCPValue, 
	setDldDSCPValue, 
	getDldDSCPAttribute, 
	setDldDSCPAttribute, 
	NULL,
};

static NodeFunc EthernetPriority_Dld=
{
	NULL,
	NULL,
	getDldEthernetPriorityValue, 
	setDldEthernetPriorityValue, 
	getDldEthernetPriorityAttribute, 
	setDldEthernetPriorityAttribute, 
	NULL,
};

static NodeFunc ROMTime_Dld=
{
	NULL,
	NULL,
	getDldROMTimeValue, 
	NULL, 
	getDldROMTimeAttribute, 
	setDldROMTimeAttribute, 
	NULL,
};

static NodeFunc BOMTime_Dld=
{
	NULL,
	NULL,
	getDldBOMTimeValue, 
	NULL, 
	getDldBOMTimeAttribute, 
	setDldBOMTimeAttribute, 
	NULL,
};

static NodeFunc EOMTime_Dld=
{
	NULL,
	NULL,
	getDldEOMTimeValue, 
	NULL, 
	getDldEOMTimeAttribute, 
	setDldEOMTimeAttribute, 
	NULL,
};

static NodeFunc TestBytesReceived_Dld=
{
	NULL,
	NULL,
	getDldTestBytesReceivedValue, 
	NULL, 
	getDldTestBytesReceivedAttribute, 
	setDldTestBytesReceivedAttribute, 
	NULL,
};

static NodeFunc TotalBytesReceived_Dld=
{
	NULL,
	NULL,
	getDldTotalBytesReceivedValue, 
	NULL, 
	getDldTotalBytesReceivedAttribute, 
	setDldTotalBytesReceivedAttribute, 
	NULL,
};

static NodeFunc TCPOpenRequestTime_Dld=
{
	NULL,
	NULL,
	getDldTCPOpenRequestTimeValue, 
	NULL, 
	getDldTCPOpenRequestTimeAttribute, 
	setDldTCPOpenRequestTimeAttribute, 
	NULL,
};

static NodeFunc TCPOpenResponseTime_Dld=
{
	NULL,
	NULL,
	getDldTCPOpenResponseTimeValue, 
	NULL, 
	getDldTCPOpenResponseTimeAttribute, 
	setDldTCPOpenResponseTimeAttribute, 
	NULL,
};

#if defined(TCSUPPORT_CT_PON_SC)
static NodeFunc SampledValues_Dld=
{
	NULL,
	NULL,
	getDldSampledValuesValue, 
	NULL, 
	getDldSampledValuesAttribute, 
	setDldSampledValuesAttribute, 
	NULL,
};
#endif

/*
	Parent Node: InternetGatewayDevice.DownloadDiagnostics.
	Parent Node Table Location: cp_topnodetable.c 
*/
CwmpParameterStruct DownloadDiagnostics[] = 
{
	{"DiagnosticsState", StrW, NoActiveSet, &DiagnosticsState_Dld, NULL},
	{"Interface", StrW, NoLimite, &Interface_Dld, NULL},
	{"DownloadURL", StrW, NoLimite, &DownloadURL_Dld, NULL},
	{"DSCP", UnsignIntW, NoLimite, &DSCP_Dld, NULL},
	{"EthernetPriority", UnsignIntW, NoLimite, &EthernetPriority_Dld, NULL},
	{"ROMTime", DateTimeTypeR, NoActiveSet, &ROMTime_Dld, NULL},
	{"BOMTime", DateTimeTypeR, NoActiveSet, &BOMTime_Dld, NULL},
	{"EOMTime", DateTimeTypeR, NoActiveSet, &EOMTime_Dld, NULL},
	{"TestBytesReceived", UnsignIntR, NoActiveSet, &TestBytesReceived_Dld, NULL},
	{"TotalBytesReceived", UnsignIntR, NoActiveSet, &TotalBytesReceived_Dld, NULL},
	{"TCPOpenRequestTime", DateTimeTypeR, NoActiveSet, &TCPOpenRequestTime_Dld, NULL},
	{"TCPOpenResponseTime", DateTimeTypeR, NoActiveSet, &TCPOpenResponseTime_Dld, NULL},
#if defined(TCSUPPORT_CT_PON_SC)
	{"SampledValues", StrR, NoLimite, &SampledValues_Dld, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};


/*******************  InternetGatewayDevice.DownloadDiagnostics. end ************************/

/*########################################################################################*/

/*******************  InternetGatewayDevice.UploadDiagnostics. start **********************/


static NodeFunc DiagnosticsState_Uld=
{
	NULL,
	NULL,
	getUldDiagnosticsStateValue, 
	setUldDiagnosticsStateValue, 
	getUldDiagnosticsAttribute, 
	setUldDiagnosticsAttribute, 
	NULL,
};

static NodeFunc Interface_Uld=
{
	NULL,
	NULL,
	getUldInterfaceValue, 
	setUldInterfaceValue, 
	getUldInterfaceAttribute, 
	setUldInterfaceAttribute, 
	NULL,
};

static NodeFunc UploadURL_Uld=
{
	NULL,
	NULL,
	getUldUploadURLValue, 
	setUldUploadURLValue, 
	getUldUploadURLAttribute, 
	setUldUploadURLAttribute, 
	NULL,
};

static NodeFunc DSCP_Uld=
{
	NULL,
	NULL,
	getUldDSCPValue, 
	setUldDSCPValue, 
	getUldDSCPAttribute, 
	setUldDSCPAttribute, 
	NULL,
};

static NodeFunc EthernetPriority_Uld=
{
	NULL,
	NULL,
	getUldEthernetPriorityValue, 
	setUldEthernetPriorityValue, 
	getUldEthernetPriorityAttribute, 
	setUldEthernetPriorityAttribute, 
	NULL,
};

static NodeFunc TestFileLength_Uld=
{
	NULL,
	NULL,
	getUldTestFileLengthValue, 
	setUldTestFileLengthValue, 
	getUldTestFileLengthAttribute, 
	setUldTestFileLengthAttribute, 
	NULL,
};

static NodeFunc ROMTime_Uld=
{
	NULL,
	NULL,
	getUldROMTimeValue, 
	NULL, 
	getUldROMTimeAttribute, 
	setUldROMTimeAttribute, 
	NULL,
};

static NodeFunc BOMTime_Uld=
{
	NULL,
	NULL,
	getUldBOMTimeValue, 
	NULL, 
	getUldBOMTimeAttribute, 
	setUldBOMTimeAttribute, 
	NULL,
};

static NodeFunc EOMTime_Uld=
{
	NULL,
	NULL,
	getUldEOMTimeValue, 
	NULL, 
	getUldEOMTimeAttribute, 
	setUldEOMTimeAttribute, 
	NULL,
};

static NodeFunc TotalBytesSent_Uld=
{
	NULL,
	NULL,
	getUldTotalBytesSentValue, 
	NULL, 
	getUldTotalBytesSentAttribute, 
	setUldTotalBytesSentAttribute, 
	NULL,
};

static NodeFunc TCPOpenRequestTime_Uld=
{
	NULL,
	NULL,
	getUldTCPOpenRequestTimeValue, 
	NULL, 
	getUldTCPOpenRequestTimeAttribute, 
	setUldTCPOpenRequestTimeAttribute, 
	NULL,
};

static NodeFunc TCPOpenResponseTime_Uld=
{
	NULL,
	NULL,
	getUldTCPOpenResponseTimeValue, 
	NULL, 
	getUldTCPOpenResponseTimeAttribute, 
	setUldTCPOpenResponseTimeAttribute, 
	NULL,
};

/*
	Parent Node: InternetGatewayDevice.UploadDiagnostics.
	Parent Node Table Location: cp_topnodetable.c 
*/
CwmpParameterStruct UploadDiagnostics[] = 
{
	{"DiagnosticsState", StrW, NoActiveSet, &DiagnosticsState_Uld, NULL},
	{"Interface", StrW, NoLimite, &Interface_Uld, NULL},
	{"UploadURL", StrW, NoLimite, &UploadURL_Uld, NULL},
	{"DSCP", UnsignIntW, NoLimite, &DSCP_Uld, NULL},
	{"EthernetPriority", UnsignIntW, NoLimite, &EthernetPriority_Uld, NULL},
	{"TestFileLength", UnsignIntW, NoLimite, &TestFileLength_Uld, NULL},
	{"ROMTime", DateTimeTypeR, NoActiveSet, &ROMTime_Uld, NULL},
	{"BOMTime", DateTimeTypeR, NoActiveSet, &BOMTime_Uld, NULL},
	{"EOMTime", DateTimeTypeR, NoActiveSet, &EOMTime_Uld, NULL},
	{"TotalBytesSent", UnsignIntR, NoActiveSet, &TotalBytesSent_Uld, NULL},
	{"TCPOpenRequestTime", DateTimeTypeR, NoActiveSet, &TCPOpenRequestTime_Uld, NULL},
	{"TCPOpenResponseTime", DateTimeTypeR, NoActiveSet, &TCPOpenResponseTime_Uld, NULL},
	{NULL, -1, -1, NULL, NULL}
};
/*******************  InternetGatewayDevice.DownloadDiagnostics. end ************************/

/*########################################################################################*/

/*******************  InternetGatewayDevice.UDPEchoConfig. start **********************/
static NodeFunc Enable=
{
	NULL,
	NULL,
	getUDPEchoEnableValue, 
	setUDPEchoEnableValue, 
	getUDPEchoEnableAttribute, 
	setUDPEchoEnableAttribute, 
	NULL,
};

static NodeFunc Interface=
{
	NULL,
	NULL,
	getUDPEchoInterfaceValue, 
	setUDPEchoInterfaceValue, 
	getUDPEchoInterfaceAttribute, 
	setUDPEchoInterfaceAttribute, 
	NULL,
};

static NodeFunc SourceIPAddress=
{
	NULL,
	NULL,
	getUDPEchoSourceIPAddressValue, 
	setUDPEchoSourceIPAddressValue, 
	getUDPEchoSourceIPAddressAttribute, 
	setUDPEchoSourceIPAddressAttribute, 
	NULL,
};

static NodeFunc UDPPort=
{
	NULL,
	NULL,
	getUDPEchoUDPPortValue, 
	setUDPEchoUDPPortValue, 
	getUDPEchoUDPPortAttribute, 
	setUDPEchoUDPPortAttribute, 
	NULL,
};

static NodeFunc EchoPlusEnabled=
{
	NULL,
	NULL,
	getUDPEchoEchoPlusEnabledValue, 
	setUDPEchoEchoPlusEnabledValue, 
	getUDPEchoEchoPlusEnabledAttribute, 
	setUDPEchoEchoPlusEnabledAttribute, 
	NULL,
};

static NodeFunc EchoPlusSupported=
{
	NULL,
	NULL,
	getUDPEchoPlusSupportedValue, 
	NULL, 
	getUDPEchoPlusSupportedAttribute, 
	setUDPEchoPlusSupportedAttribute, 
	NULL,
};

static NodeFunc PacketsReceived=
{
	NULL,
	NULL,
	getUDPEchoPacketsReceivedValue, 
	NULL, 
	getUDPEchoPacketsReceivedAttribute, 
	setUDPEchoPacketsReceivedAttribute, 
	NULL,
};

static NodeFunc PacketsResponded=
{
	NULL,
	NULL,
	getUDPEchoPacketsRespondedValue, 
	NULL, 
	getUDPEchoPacketsRespondedAttribute, 
	setUDPEchoPacketsRespondedAttribute, 
	NULL,
};

static NodeFunc BytesReceived=
{
	NULL,
	NULL,
	getUDPEchoBytesReceivedValue, 
	NULL, 
	getUDPEchoBytesReceivedAttribute, 
	setUDPEchoBytesReceivedAttribute, 
	NULL,
};

static NodeFunc BytesResponded=
{
	NULL,
	NULL,
	getUDPEchoBytesRespondedValue, 
	NULL, 
	getUDPEchoBytesRespondedAttribute, 
	setUDPEchoBytesRespondedAttribute, 
	NULL,
};

static NodeFunc TimeFirstPacketReceived=
{
	NULL,
	NULL,
	getTimeFirstPacketReceivedValue, 
	NULL, 
	getTimeFirstPacketReceivedAttribute, 
	setTimeFirstPacketReceivedAttribute, 
	NULL,
};

static NodeFunc TimeLastPacketReceived=
{
	NULL,
	NULL,
	getTimeLastPacketReceivedValue, 
	NULL, 
	getTimeLastPacketReceivedAttribute, 
	setTimeLastPacketReceivedAttribute, 
	NULL,
};
/*
	Parent Node: InternetGatewayDevice.UDPEchoConfig.
	Parent Node Table Location: cp_topnodetable.c 
*/
CwmpParameterStruct UDPEchoConfig[] = 
{
	{"Enable", BooleanW, NoLimite, &Enable, NULL},
	{"Interface", StrW, NoLimite, &Interface, NULL},
	{"SourceIPAddress", StrW, NoLimite, &SourceIPAddress, NULL},
	{"UDPPort", UnsignIntW, NoLimite, &UDPPort, NULL},
	{"EchoPlusEnabled", BooleanW, NoLimite, &EchoPlusEnabled, NULL},
	{"EchoPlusSupported", BooleanR, NoLimite, &EchoPlusSupported, NULL},
	{"PacketsReceived", UnsignIntR, NoActiveSet, &PacketsReceived, NULL},
	{"PacketsResponded", UnsignIntR, NoActiveSet, &PacketsResponded, NULL},
	{"BytesReceived", UnsignIntR, NoActiveSet, &BytesReceived, NULL},
	{"BytesResponded", UnsignIntR, NoActiveSet, &BytesResponded, NULL},
	{"TimeFirstPacketReceived", DateTimeTypeR, NoActiveSet, &TimeFirstPacketReceived, NULL},
	{"TimeLastPacketReceived", DateTimeTypeR, NoActiveSet, &TimeLastPacketReceived, NULL},
	{NULL, -1, -1, NULL, NULL}
};

/*******************  InternetGatewayDevice.UDPEchoConfig. end ************************/

