#include "Global_res.h"
#include "cp_TraceRouteDiagnostics.h"
extern traceroute_t cwmp_traceroute_p;

static NodeFunc DiagnosticsState=
{
	NULL,
	NULL,
	getTraceRouteDiagnosticsStateValue,
	setTraceRouteDiagnosticsStateValue, 
	getTraceRouteDiagnosticsStateAttribute, 
	setTraceRouteDiagnosticsStateAttribute, 
	NULL,
};

static NodeFunc Interface=
{
	NULL,
	NULL,
	getTraceRouteInterfaceValue,
	setTraceRouteInterfaceValue, 
	getTraceRouteInterfaceAttribute, 
	setTraceRouteInterfaceAttribute, 
	NULL,
};

static NodeFunc Host=
{
	NULL,
	NULL,
	getTraceRouteHostValue,
	setTraceRouteHostValue, 
	getTraceRouteHostAttribute, 
	setTraceRouteHostAttribute, 
	NULL,
};

static NodeFunc NumberOfTries=
{
	NULL,
	NULL,
	getNumberOfTriesValue,
	setNumberOfTriesValue, 
	getNumberOfTriesAttribute, 
	setNumberOfTriesAttribute, 
	NULL,
};

static NodeFunc Timeout=
{
	NULL,
	NULL,
	getTraceRouteTimeoutValue,
	setTraceRouteTimeoutValue, 
	getTraceRouteTimeoutAttribute, 
	setTraceRouteTimeoutAttribute, 
	NULL,
};

static NodeFunc DataBlockSize=
{
	NULL,
	NULL,
	getTraceRouteDataBlockSizeValue,
	setTraceRouteDataBlockSizeValue, 
	getTraceRouteDataBlockSizeAttribute, 
	setTraceRouteDataBlockSizeAttribute, 
	NULL,
};

static NodeFunc DSCP=
{
	NULL,
	NULL,
	getTraceRouteDSCPValue,
	setTraceRouteDSCPValue, 
	getTraceRouteDSCPAttribute, 
	setTraceRouteDSCPAttribute, 
	NULL,
};

static NodeFunc MaxHopCount=
{
	NULL,
	NULL,
	getMaxHopCountValue,
	setMaxHopCountValue,
	getMaxHopCountAttribute, 
	setMaxHopCountAttribute, 
	NULL,
};

static NodeFunc ResponseTime=
{
	NULL,
	NULL,
	getResponseTimeValue,
	NULL, 
	getResponseTimeAttribute, 
	setResponseTimeAttribute, 
	NULL,
};

static NodeFunc RouteHopsNumberOfEntries=
{
	NULL,
	NULL,
	getRouteHopsNumberOfEntriesValue,
	NULL, 
	getRouteHopsNumberOfEntriesAttribute, 
	setRouteHopsNumberOfEntriesAttribute, 
	NULL,
};

static NodeFunc HopHost=
{
	NULL,
	NULL,
	getHopHostValue,
	NULL, 
	getHopHostAttribute, 
	setHopHostAttribute, 
	NULL,
};

static NodeFunc HopHostAddress=
{
	NULL,
	NULL,
	getHopHostAddressValue,
	NULL, 
	getHopHostAddressAttribute, 
	setHopHostAddressAttribute, 
	NULL,
};

static NodeFunc HopErrorCode=
{
	NULL,
	NULL,
	getHopErrorCodeValue,
	NULL, 
	getHopErrorCodeAttribute, 
	setHopErrorCodeAttribute, 
	NULL,
};

static NodeFunc HopRTTimes=
{
	NULL,
	NULL,
	getHopRTTimesValue,
	NULL, 
	getHopRTTimesAttribute, 
	setHopRTTimesAttribute, 
	NULL,
};

static NodeFunc RouteHopsFuncs=
{
	NULL,
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	cwmpInitRouteHops1SubTree,
};


/*
	Parent Node: InternetGatewayDevice.TraceRouteDiagnostics.
	Parent Node Table Location: cp_topnodetable.c
*/
CwmpParameterStruct TraceRouteDiagnostics[] = 
{
	{"DiagnosticsState", StrW, NoLimite, &DiagnosticsState, NULL},	
	{"Interface", StrW, NoLimite, &Interface, NULL},
	{"Host", StrW, NoLimite, &Host, NULL},
	{"NumberOfTries", UnsignIntW, NoLimite, &NumberOfTries, NULL},
	{"Timeout", UnsignIntW, NoLimite, &Timeout, NULL},
	{"DataBlockSize", UnsignIntW, NoLimite, &DataBlockSize, NULL},	
	{"DSCP", UnsignIntW, NoLimite, &DSCP, NULL},
	{"MaxHopCount", UnsignIntW, NoLimite, &MaxHopCount, NULL},
	{"ResponseTime", UnsignIntR, NoLimite, &ResponseTime, NULL},
#if defined(TCSUPPORT_CMCC)	
	{"HopsNumberOfEntries", UnsignIntR, NoLimite, &RouteHopsNumberOfEntries, NULL},
#else
	{"RouteHopsNumberOfEntries", UnsignIntR, NoLimite, &RouteHopsNumberOfEntries, NULL},
#endif
	{"RouteHops", ObjectR, NoLimite, &RouteHopsFuncs, NULL},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.TraceRouteDiagnostics.RouteHops.{i}
	Parent Node Table Location: cp_TraceRouteDiagnostics_nodetable.c
*/
CwmpParameterStruct RouteHops[] = 
{
	{"HopHost", StrR, NoLimite, &HopHost, NULL},	
	{"HopHostAddress", StrR, NoLimite, &HopHostAddress, NULL},
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"HopErrorCode", UnsignIntR, NoLimite, &HopErrorCode, NULL},
#else
	{"HopErrorCode", StrR, NoLimite, &HopErrorCode, NULL},
#endif
	{"HopRTTimes", StrR, NoLimite, &HopRTTimes, NULL},
	{NULL, -1, -1, NULL, NULL}
};


/*
      This Node: InternetGatewayDevice.TraceRouteDiagnostics.RouteHops.{i}
	Parent Node: InternetGatewayDevice.TraceRouteDiagnostics.RouteHops.
	Parent Node Table Location: cp_landevice_nodetable.c
*/

CwmpParameterStruct RouteHopsIndex[]= 
{
	{NULL,ObjectR, NoLimite,NULL,RouteHops},
	{NULL, -1, -1, NULL, NULL}
};

int	cwmpInitRouteHops1SubTree( DeviceNodePtr deviceNode )
{
	int rtnCode = 0;
//	char buf[10];
	
	if ( deviceNode == NULL ) {
		return -1;
	}

#if 0
	//cost a lot of throughpt and need not to do. shnwind 20110330.
	/*check if wifi module is exist*/
	
	tcapi_get( "Info_WLan","isExist",buf );
	
	if ( strcmp( buf,"On" ) != 0 ) {
		
		tc_printf( "Wifi module is not exist. so we can not build TR069 Paramters tree for it.\n" );
		
		if ( deviceNode->firstChild != NULL ) {
			cwmpNodeFree( deviceNode->firstChild );
			deviceNode->firstChild = NULL;
		}
		
		return rtnCode;
	}

#endif
	/*init arrays to zero*/
//	memset( buf, 0, sizeof(buf) );

	/*get current ssid num from cfg node "WLan_Common"*/
	
//	tcapi_get( "WLan_Common", "BssidNum", buf );
	
//	BssidNum = atoi(buf);

	if(cwmp_traceroute_p.RouteHopsNumberOfEntries == 0)
		return rtnCode;

	rtnCode = RegisterDynNodeFromIndex( deviceNode, RouteHopsIndex, cwmp_traceroute_p.RouteHopsNumberOfEntries );

#if 0//def WSC_AP_SUPPORT
	rtnCode = cwmpInitWLANWPS( deviceNode );
#endif

	return rtnCode;
}

int RebuildRouteHopsObject(void){
	int rtnCode = 0;
	DeviceNodePtr TracertNode = NULL;
	char *objectName = "InternetGatewayDevice.TraceRouteDiagnostics.RouteHops.";

	if( rpc.IGDDeviceRootPtr == NULL){
		return -1;
	}
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &TracertNode);
	if(TracertNode == NULL){
		return -1;
	}
	popNodeToName(rpc.IGDDeviceRootPtr);
	cwmpFreeDeviceTable(TracertNode);
	rtnCode = cwmpInitRouteHops1SubTree(TracertNode);
	pushNameToStack();


	return rtnCode;
}

