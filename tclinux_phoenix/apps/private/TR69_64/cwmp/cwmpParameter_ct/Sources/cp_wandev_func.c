#include "Global_res.h"
#include "cp_wandev.h"

//uint16 ACSaddingPVCFlag = 0; //flag for ACS adding PVC
#if defined(TCSUPPORT_CT_WAN_PTM)
extern int getModulationType(char *retType, int bufsize);
#endif

/* add pvc object */
int8 DeviceNodeAddPVCObject(uint32 *num, DeviceNode *deviceNode)
{

	char wan_node[32], vpi[10];
	int ret = 0, i;
#if defined(TCSUPPORT_CT_WAN_PTM)
	char s_dsltype[32] = {0};
	int dslType = 0;
#endif
#ifdef  TCSUPPORT_CT_WANNINDEX_INCREASE
	char lastUsedPVCIndex[8] = {0};
#endif

	DeviceNodePtr WANDeviceNode = NULL;
	char *objectName = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.";

	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &WANDeviceNode);
	if (NULL == WANDeviceNode) {
		tcdbg_printf("top node not found!\n");
		return -1;
	}

#ifdef  TCSUPPORT_CT_WANNINDEX_INCREASE
	i = findUnusedPvcIndex();
	sprintf(wan_node, "Wan_PVC%d", i);
#else
	
	for (i = 0; i < TOTAL_REMOTE_DEVICE_NODE; i++) {		
		sprintf(wan_node, "Wan_PVC%d", i);
#if defined(TCSUPPORT_CT_PON)
		ret = tcapi_get(wan_node, "VLANMode", vpi);
#else
#if defined(TCSUPPORT_CT_WAN_PTM)
		ret = tcapi_get(wan_node, "PTMEnable", vpi);
#else
		ret = tcapi_get(wan_node, "VPI", vpi);
#endif
#endif

		if (ret < 0) {
			break;
		}
	}
#endif

	/* there is no free pvc for adding */
	if (i == TOTAL_REMOTE_DEVICE_NODE) {
		tc_printf("%s:There is no free pvc for adding.\n");
		ret = -1;
		return ret;
	}
#ifndef TCSUPPORT_CT_WANNODE_MAPPING//tony add	
	*num = i + 1;
#endif

#if defined(TCSUPPORT_CT_PON)
	tcapi_set(wan_node, "VLANID", "0");
	tcapi_set(wan_node, "DOT1P", "0");
	tcapi_set(wan_node, "VLANMode", "UNTAG"); //default UNTAG mode
	tcapi_set(wan_node, "EPONEnable", "Yes");
	tcapi_set(wan_node, "GPONEnable", "Yes");
#else
#if defined(TCSUPPORT_CT_WAN_PTM)
	tcapi_set(wan_node, "VLANID", "0");
	tcapi_set(wan_node, "DOT1P", "0");
	tcapi_set(wan_node, "VLANMode", "TAG");
	dslType = getModulationType(s_dsltype, sizeof(s_dsltype));
	if ( 2 == dslType )
	{
		tcapi_set(wan_node, "PTMEnable", "Yes");
		tcapi_set(wan_node, "ATMEnable", "No");
	}
	else if ( 1 == dslType )
	{
		tcapi_set(wan_node, "ATMEnable", "Yes");
		tcapi_set(wan_node, "PTMEnable", "No");
	}
	tcapi_set(wan_node, "PTMLinkEnable", "Yes");
#endif
	tcapi_set(wan_node, "DSLLinkEnable", "Yes");
	/* init vpi and vci for new pvc */
	tcapi_set(wan_node, "VPI", "");
	tcapi_set(wan_node, "VCI", "");


	tcapi_set(wan_node,"QOS","ubr");
	tcapi_set(wan_node,"PCR","0");
	tcapi_set(wan_node,"SCR","0");
	tcapi_set(wan_node,"MBS","0");
#endif
	tcapi_set(wan_node,"ENCAP","1483 Bridged IP LLC");

	tcapi_set(wan_node,"Action","Add");	
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	UpdatePVCMapping(i);
	*num = i;
	GetPVCMapping(num);
#endif	
	
	cwmpFreeDeviceTable(WANDeviceNode);
	ret = cwmpInitWANConnectionDeviceTable(WANDeviceNode);

	if(dbg_cwmp >= DBG_CWMP_L2 ){
			tc_printf("\r\ncp_wandev_func.c---[DeviceNodeAddPVCObject]---*num = ", *num);
	}


#ifdef  TCSUPPORT_CT_WANNINDEX_INCREASE
	if(ret == 0){
		//usedWanIndex.pvcIndex = i;
		sprintf(lastUsedPVCIndex,"%d",i);
		tcapi_set("Wan_Common","lastUsedPVCIndex",lastUsedPVCIndex);	
	}
#endif	

	return ret;
}



/* init pvc node */
int cwmpInitWANConnectionDeviceTable(DeviceNodePtr deviceNode)
{
	char wanConnNodeName[32] = {0};
	char entryNodeName[32] = {0};
	DeviceNodePtr wanConnNodeNamePtr = NULL, entryNodeNamePtr = NULL;
	char wan_node[32], vpi[10], isp[10], br_type[20];
	char wan_if_node[32];
	char node_index[4];
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	uint32 pvc_index;
#endif
	int ret = 0, i, j;
//	tcdbg_printf("%s:000000000000000.\n", __FUNCTION__);
	for (i =0 ; i < TOTAL_REMOTE_DEVICE_NODE; i++) {
		sprintf(wan_node, "Wan_PVC%d", i);

#if defined(TCSUPPORT_CT_PON)
		ret = tcapi_get(wan_node, "VLANMode", vpi);
#else
#if defined(TCSUPPORT_CT_WAN_PTM)
		ret = tcapi_get(wan_node, "PTMEnable", vpi);
#else
		ret = tcapi_get(wan_node, "VPI", vpi);
#endif
#endif
		if (ret < 0) {
			/* this pvc is not exist */
			continue;
		}
		
		/* register this node */
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
		pvc_index = i;
		GetPVCMapping(&pvc_index); /* PVC{i} is exist, get "Index" to pvc_index */
		if(pvc_index == 0){
			tcdbg_printf("%s=> PVC%d have not 'Index', update this PVC mapping table.\n", __FUNCTION__, i);
			UpdatePVCMapping(i);
			pvc_index = i;
			GetPVCMapping(&pvc_index); /* After update mapping table, get "Index" again */
		}
		sprintf(node_index, "%d", pvc_index);
#else
		sprintf(node_index, "%d", i + 1);
#endif
		WANConnectionDeviceIndex[0].name = node_index;
		if (RegisterNodeFromTable(deviceNode, WANConnectionDeviceIndex) == NULL) {
			tcdbg_printf("%s:11111111111111111.\n", __FUNCTION__);
			ret = -1;
			return ret;
		}
	}

	ret = 0;
	return ret;
}

#if defined(TCSUPPORT_CT_PON)
int cwmpInitWANDeviceTable(DeviceNodePtr deviceNode)
{
	int ret = 0;
	char nodeName[33] = {0};
	char linkSta[4] = "0";
	int ponsw = 0;

	/* register this node */
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	InitWanNodeMapping();
#endif 
	if (RegisterNodeFromTable(deviceNode, WANDevice) == NULL) {
		tcdbg_printf("%s:error.\n", __FUNCTION__);
		ret = -1;
		return ret;
	}

	// check GPon or EPon
	strcpy(nodeName, "XPON_LinkCfg");
	tcapi_get(nodeName, "LinkSta", linkSta);
	ponsw = atoi(linkSta);
	if ( 0 == ponsw ) // down
		return ret;
	else if ( 1 == ponsw ) // gpon
	{
		if (RegisterNodeFromTable(deviceNode, GponInterfaceConfig) == NULL)
		{
			tcdbg_printf("%s:error.\n", __FUNCTION__);
			ret = -1;
			return ret;
		}
	}
	else if ( 2 == ponsw ) // epon
	{
		if (RegisterNodeFromTable(deviceNode, EponInterfaceConfig) == NULL)
		{
			tcdbg_printf("%s:error.\n", __FUNCTION__);
			ret = -1;
			return ret;
		}
	}

	return ret;
}
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 getX_CTCOMWANIndexValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {0};
	uint32 pvcNodeIndex = 0,pvcNodeEntry = 0;
	int valid_if[MAX_WAN_IF_INDEX], valid_if_num = 0, i = 0;
	char *tp = NULL;

	char wanIndexInfo[500] = {0};
	char indexStr[10] = {0};
	char wanModeStr[20] = {0};
	char linkModeStr[10] = {0};
	char ispStr[10] = {0};
	char encapStr[20] = {0};
	char vidStr[20] = {0};
	char atmStr[10] = {0};
	char vpiStr[10] = {0};
	char vciStr[10] = {0};
	char servlistStr[10] = {0};
	char validIFIndexStr[100] = {0};
	char validIFIndexNumStr[3] = {0};
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	uint32 pvctmp, entrytmp;
#endif
	memset(wanIndexInfo, 0x00, sizeof(wanIndexInfo));	
	memset(validIFIndexStr, 0x00, sizeof(validIFIndexStr));

	memset(nodeName, 0, sizeof(nodeName));
	sprintf(nodeName, "WanInfo_Common");

	tcapi_get(nodeName, "ValidIFIndex", validIFIndexStr);
	tcapi_get(nodeName, "ValidIFNum", validIFIndexNumStr);

	valid_if_num = atoi(validIFIndexNumStr);

	tp = strtok(validIFIndexStr, ",");
	i = 0;
	while( tp != NULL && i < valid_if_num)
	{
		if(tp != NULL)
			valid_if[i] = atoi(tp);
		
		tp = strtok(NULL, ",");		
		i++;
	}	
	
	for(i = 0; i < valid_if_num; i++){			
		/*Init wan info entry:update GUI interface name*/		
		pvcNodeIndex = valid_if[i] / MAX_SMUX_NUM;
		pvcNodeEntry = valid_if[i] % MAX_SMUX_NUM;

		memset(indexStr, 0x00, sizeof(indexStr));
		memset(wanModeStr, 0x00, sizeof(wanModeStr));
		memset(linkModeStr, 0x00, sizeof(linkModeStr));
		memset(ispStr, 0x00, sizeof(ispStr));
		memset(encapStr, 0x00, sizeof(encapStr));
		memset(vidStr, 0x00, sizeof(vidStr));
		memset(atmStr, 0x00, sizeof(atmStr));
		memset(vpiStr, 0x00, sizeof(vpiStr));
		memset(vciStr, 0x00, sizeof(vciStr));
		memset(servlistStr, 0x00, sizeof(servlistStr));
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add		
		pvctmp = pvcNodeIndex;
		entrytmp = pvcNodeEntry;
		GetWanNodeMapping(&pvctmp, &entrytmp);
		sprintf(indexStr, "%d.%d", pvctmp, entrytmp);
#else
		sprintf(indexStr, "%d.%d", pvcNodeIndex + 1, pvcNodeEntry + 1);
#endif
		memset(nodeName, 0, sizeof(nodeName));
	
		sprintf(nodeName, "Wan_PVC%d", pvcNodeIndex);
		tcapi_get(nodeName, "ATMEnable", atmStr);
		if(strcmp(atmStr, "Yes") == 0)	//atm
		{
			tcapi_get(nodeName, "VPI", vpiStr);
			tcapi_get(nodeName, "VCI", vciStr);			
		}

		sprintf(nodeName, "Wan_PVC%d_Entry%d", pvcNodeIndex, pvcNodeEntry);
		
		tcapi_get(nodeName, "WanMode", wanModeStr);

		tcapi_get(nodeName, "LinkMode", linkModeStr);
		
		tcapi_get(nodeName, "ISP", ispStr);

		if( 0 == strcmp(wanModeStr, "Route") )
		{
			if( 0 == strcmp(linkModeStr, "linkIP") )
			{
				if( 0 == strcmp(ispStr, "0") )
					sprintf(encapStr, "DHCP_Routed");
				else
					sprintf(encapStr, "STATIC_Routed");
			}
			else
				sprintf(encapStr, "PPPoE_Routed");
		}
		else
			sprintf(encapStr, "Bridged");

		if(strcmp(atmStr, "Yes") == 0)	//atm
			sprintf(vidStr, "%s/%s", vpiStr, vciStr);
		else
			tcapi_get(nodeName, "VLANID", vidStr);
		
		tcapi_get(nodeName, "ServiceList", servlistStr);
		tp = servlistStr;
		while(*tp != '\0')
		{
			if(*tp == '_')
				*tp = ',';

			tp++;
		}
		
		if(i == 0)
			sprintf(wanIndexInfo, "(");

		strcat(wanIndexInfo, "\"");
		strcat(wanIndexInfo, indexStr);
		strcat(wanIndexInfo, ";");
		strcat(wanIndexInfo, encapStr);
		strcat(wanIndexInfo, ";");		
		strcat(wanIndexInfo, vidStr);
		strcat(wanIndexInfo, ";");	
		strcat(wanIndexInfo, servlistStr);

		if(i == valid_if_num - 1)
			strcat(wanIndexInfo, "\")");
		else
			strcat(wanIndexInfo, "\",");
	}

	return getTheValue(value, type, deviceNode, wanIndexInfo);
}

int8 getX_CTCOMWANIndexAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
		
	attribute = tcIpConAttr.aX_CTCOMWANIndex;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setX_CTCOMWANIndexAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	
	attribute = &tcIpConAttr.aX_CTCOMWANIndex;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

#endif


