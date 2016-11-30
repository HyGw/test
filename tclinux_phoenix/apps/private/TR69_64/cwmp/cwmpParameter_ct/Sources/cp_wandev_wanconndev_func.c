#include "Global_res.h"
#include "cp_wandev_wanconndev.h"

#include "cp_wandev.h"

#if defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_WAN_PTM)
int8 isDuplicatedIFaceID(char *IFId)
{
	char nodeName[32] = {0};
	char ifaceID[8] = {0};
	uint8 pvc_index = 0;
	uint8 entry_idx = 0;

	if ( NULL == IFId )
		return -1;

	for ( pvc_index = 0; pvc_index < PVC_NUM; pvc_index ++ )
	{
		for ( entry_idx = 0; entry_idx < MAX_SMUX_NUM; entry_idx ++ )
		{
			sprintf(nodeName, "Wan_PVC%d_Entry%d", pvc_index, entry_idx);
			if ( tcapi_get(nodeName, "IFIdx", ifaceID) < 0 )
				continue;

			if ( 0 == strcmp(ifaceID, IFId) )
				return 1;
		}
	}

	return 0;
}

uint8 getNewIFaceID()
{
	int latestID = 0;
	uint8 id_idx = 0;
	char lastIFId[8] = "1";
	char nodeName[32] = {0};
	char idbuf[8] = {0};

	strcpy(nodeName, "Wan_Common");
	tcapi_get(nodeName, "LatestIFIdx", lastIFId);
	latestID = atoi(lastIFId);
	latestID ++;

	for ( id_idx = latestID; id_idx <= 99; id_idx ++ )
	{
		sprintf(idbuf, "%d", id_idx);
		if ( 1 != isDuplicatedIFaceID(idbuf) )
			return id_idx;
	}

	for ( id_idx = 1; id_idx < latestID; id_idx ++ )
	{
		sprintf(idbuf, "%d", id_idx);
		if ( 1 != isDuplicatedIFaceID(idbuf) )
			return id_idx;
	}

	return 1;
}
#endif

/* delete pvc object */
int8 DeviceNodeDelPVCObject(uint32 num, DeviceNode *deviceNode)
{

	char wan_node[32], wan_if_node[32];
	char vpi[4], active[4];
	uint32 del_pvc_index, i, ret = 0;

	DeviceNodePtr WANDeviceNode = NULL;
	char *objectName = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.";
#if defined(TCSUPPORT_CT_PON_GD) || defined(TCSUPPORT_CT_PON_C9) || defined(TCSUPPORT_CT_PON_SN) || defined(TCSUPPORT_CT_PON_GS) || defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_PON_CY_JX)
	char servicelist[64] = {0};
	int isTr69Wan = 0;
#endif

	tcdbg_printf("Deleting PVC device!\n");
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &WANDeviceNode);
	if (WANDeviceNode == NULL) {
		return -1;
	}
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	del_pvc_index = num;
	ret = FindPVCMapping(&del_pvc_index);
#else
	del_pvc_index = num - 1;

	/* check if the pvc node is exist */
	sprintf(wan_node, "Wan_PVC%d", del_pvc_index);
	/* this pvc is not exist, just return */
	ret = tcapi_get(wan_node, "VPI", vpi);
#endif
	if (ret < 0) {
		return ret;
	}
#if defined(TCSUPPORT_CT_PON_SN)
	for (i = 0; i <  MAX_WAN_NODE; i++)
	{
		sprintf(wan_if_node, "Wan_PVC%d_Entry%d", del_pvc_index, i);
		ret = tcapi_get(wan_if_node, "ServiceList", servicelist);
		if (ret < 0) {
				continue;
			}
		if(NULL != strstr(servicelist,"TR069"))
		{
			return -1;
		}
	}
#endif

#if defined(TCSUPPORT_CT_PON_GD) || defined(TCSUPPORT_CT_PON_C9) || defined(TCSUPPORT_CT_PON_GS) || defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_PON_CY_JX)
	for(i = 0; i < MAX_WAN_NODE; i++)
	{
		snprintf(wan_if_node,sizeof(wan_if_node),"Wan_PVC%d_Entry%d",del_pvc_index,i);        // how to get Entry index?
		ret = tcapi_get(wan_if_node, "ServiceList", servicelist);
		if (ret < 0) {
			continue;
		}

//		tcdbg_printf("servicelist %s\n",servicelist);
		if(NULL != strstr(servicelist,"TR069"))
		{
			isTr69Wan = 1;
			break;
		}
	
	}

	if(0 == isTr69Wan)
	{
		tcapi_unset(wan_node);
		tcdbg_printf("[DeviceNodeDelPVCObject]Unset Wan %s\n!",wan_node);
		
		setCwmpInformTaskStart(1); 
		
		cwmpFreeDeviceTable(WANDeviceNode);
		ret = cwmpInitWANConnectionDeviceTable(WANDeviceNode);

		setCwmpInformTaskStart(0); 
		
		if(ret  < 0 ){
			tcdbg_printf("\r\ncp_wandev_wanconndev_func.c [DeviceNodeDelPVCObject]cwmpInitWANConnectionDeviceTable failed\n");
		}
	}
	else      // for tr069 connection, it must commit afer session close
#endif
       {
	//commit mechanism adding
	// no need to reinit the WAN tree here, the commit mechanism will do it later
	SetCfgNodeFlag ( "Wan", del_pvc_index+WAN_PVC_UNSET_ALIAS );
	}


#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
	for(i = 0; i < MAX_WAN_NODE; i++)
	{
		setMonitorCollectorParaList(WAN_TYPE_NOCARE,del_pvc_index,i,WAN_DEV_DEL);
	}
#endif

	ret = 0;
	return ret;
}

#if defined(TCSUPPORT_CT_FJ)
void setProvisioningCode(char *regProvisioningCode){
	char s_ProvisioningCode[512] = {0};
	char buf[512] = {0};
	int i_serviceNum = 0;
	char s_serviceNum[16] = {0};
	
	tcapi_get("deviceAccount_Entry", "serviceNum", s_serviceNum);
	i_serviceNum = atoi(s_serviceNum);

	
	tcapi_get("deviceAccount_Entry", "ProvisioningCode", s_ProvisioningCode);
	if(strlen(s_ProvisioningCode) == 0){
		tcapi_set("deviceAccount_Entry", "ProvisioningCode", regProvisioningCode);
		i_serviceNum++;
		sprintf(s_serviceNum, "%d", i_serviceNum);
		tcapi_set("deviceAccount_Entry", "serviceNum", s_serviceNum);
	}else{
		if(strstr(s_ProvisioningCode,regProvisioningCode) == NULL){	
			i_serviceNum++;
			sprintf(s_serviceNum, "%d", i_serviceNum);
			tcapi_set("deviceAccount_Entry", "serviceNum", s_serviceNum);
			
			strcat(s_ProvisioningCode,",");			
			strcat(s_ProvisioningCode,regProvisioningCode);
			tcapi_set("deviceAccount_Entry", "ProvisioningCode", s_ProvisioningCode);
		}

	}

	
	return;
}
#endif

/* add connection object */
int8 cwmpAddIPConnectionObject(uint32* num, DeviceNode*  deviceNode)
{
	char wan_if_node[32], active[10];
	int ret = 0, i;
	uint32 pvc_index;
	int node_index;
	char nasName[12] = {0};
	char attrName[12] = {0};
	int n;
#if defined(TCSUPPORT_CT_PON)
	char wan_node[32] = {0};
	char vlanmode[20] = {0};
	char vlanid[12] = "0";
	char pbit[12] = "0";
	char newIFId[8] = "1";
#endif
#if defined(TCSUPPORT_CT_WAN_PTM)
		char wan_node[32] = {0};
		char ptmenable[20] = {0};
		char vlanmode[20] = {0};
		char vlanid[12] = "0";
		char pbit[12] = "0";
		char newIFId[8] = "1";
#endif
#ifdef  TCSUPPORT_CT_WANNINDEX_INCREASE
	char lastUsedEntryIndex[8] = {0};
	char wan_pvc_node[32] = {0};
#endif


	DeviceNodePtr WANDeviceNode = NULL;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	uint32 pvctmp;
	pvc_index = atoi(deviceNode->parentNode->name);
	FindPVCMapping(&pvc_index);
#else
	pvc_index = atoi(deviceNode->parentNode->name) - 1;
#endif
	tcdbg_printf("Adding ip connection device!\n");


#ifdef  TCSUPPORT_CT_WANNINDEX_INCREASE
	i = findUnUsedEntryIndex(pvc_index);
	sprintf(wan_if_node, "Wan_PVC%d_Entry%d", pvc_index, i);
#else
	for (i = 0; i < TOTAL_REMOTE_ENTRY_NODE; i++) {
		sprintf(wan_if_node, "Wan_PVC%d_Entry%d", pvc_index, i);
		ret = tcapi_get(wan_if_node, "Active", active);
		if (ret < 0) {
			break;
		}
	}
#endif	

	
	if (i == TOTAL_REMOTE_ENTRY_NODE) {
		tcdbg_printf("error1 cwmpAddConnectionObject\n");
		return -1;
	}

#ifndef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	*num = i + 1;
#endif

	sprintf(nasName,"nas%d_%d",pvc_index, i);

	/* init the connection info */
	tcapi_set(wan_if_node,"ISP", "0");
	tcapi_set(wan_if_node, "Active", "No");
	tcapi_set(wan_if_node, "ServiceList","INTERNET");
	tcapi_set(wan_if_node, "BandActive","N/A");
	tcapi_set(wan_if_node, "WanMode","Route");
	tcapi_set(wan_if_node, "LinkMode","linkIP");
#if defined(TCSUPPORT_CT_PON_JS)
	tcapi_set(wan_if_node, "IPVERSION","IPv4/IPv6");
	tcapi_set(wan_if_node, "PDEnable","Yes");
	tcapi_set(wan_if_node, "PDOrigin","PrefixDelegation");
#else
	tcapi_set(wan_if_node, "IPVERSION","IPv4");
#endif
	tcapi_set(wan_if_node, "DHCPOption60","");
	tcapi_set(wan_if_node, "dot1q","No");
	tcapi_set(wan_if_node, "dot1p","No");
#if defined(TCSUPPORT_CT_PON)
	tcapi_set(wan_if_node, "MTU","1500");
	sprintf(wan_node, "Wan_PVC%d", pvc_index);
	tcapi_get(wan_node, "VLANMode", vlanmode);
	if ( 0 == strcmp("TAG", vlanmode) )
	{
		tcapi_get(wan_node, "VLANID", vlanid);
		tcapi_get(wan_node, "DOT1P", pbit);
		tcapi_set(wan_if_node, "dot1q", "Yes");
		tcapi_set(wan_if_node, "dot1p", "Yes");
		tcapi_set(wan_if_node, "VLANID", vlanid);
		tcapi_set(wan_if_node, "dot1pData", pbit);
	}
	sprintf(newIFId, "%u", getNewIFaceID());
	tcapi_set(wan_if_node, "IFIdx", newIFId);
	tcapi_set("Wan_Common", "LatestIFIdx", newIFId);
#endif
#if defined(TCSUPPORT_CT_WAN_PTM)
		tcapi_set(wan_if_node, "MTU","1500");
		sprintf(wan_node, "Wan_PVC%d", pvc_index);
		tcapi_get(wan_node, "PTMEnable", ptmenable);
		if ( 0 == strcmp("Yes", ptmenable) )
		{		
			tcapi_get(wan_node, "VLANMode", vlanmode);
			tcapi_set(wan_if_node, "Barrier", "0");
			tcapi_set(wan_if_node, "VLANMode", vlanmode);
			if ( 0 == strcmp("TAG", vlanmode) )
			{
				tcapi_get(wan_node, "VLANID", vlanid);
				tcapi_get(wan_node, "DOT1P", pbit);
				tcapi_set(wan_if_node, "dot1q", "Yes");
				tcapi_set(wan_if_node, "dot1p", "Yes");
				tcapi_set(wan_if_node, "VLANID", vlanid);
				tcapi_set(wan_if_node, "dot1pData", pbit);
			}
		}
		sprintf(newIFId, "%u", getNewIFaceID());
		tcapi_set(wan_if_node, "IFIdx", newIFId);
		tcapi_set("Wan_Common", "LatestIFIdx", newIFId);
#endif

	tcapi_set(wan_if_node, "NATENABLE","Enable");
	tcapi_set(wan_if_node, "IGMPproxy","No");
	tcapi_set(wan_if_node, "NASName",nasName);
	tcapi_set(wan_if_node, "IFName",nasName);
	tcapi_set(wan_if_node, "DHCPRealy","No");
	tcapi_set(wan_if_node, "DHCPEnable", "1");
	tcapi_set(wan_if_node, "DHCPv6", "No");
	tcapi_set(wan_if_node, "DHCPv6PD", "Yes");
		
	for(n = 1; n <= 4; n++)
	{
		sprintf(attrName, "LAN%d", n);
		tcapi_set(wan_if_node, attrName,"No");

		memset(attrName, 0, sizeof(attrName) );
		sprintf(attrName, "SSID%d", n);
		tcapi_set(wan_if_node, attrName,"No");
	}
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	UpdateEntryMapping(pvc_index, i);
	*num = i;
	pvctmp = pvc_index;
	GetWanNodeMapping(&pvctmp, num);//get Wan_PVC%d_Entry%d -> Node: Index
#endif
	
	spSysParam.tcCwmpAttr->vWANLinkTypeFlag[pvc_index] |=  (1<<i);

	/* register the parameter tree */
	cwmpFreeDeviceTable(deviceNode);
	ret = cwmpInitIPConnectionObjectSubtree(deviceNode);

	if(dbg_cwmp >= DBG_CWMP_L2 ){
			tc_printf("\r\ncp_wandev_wanconndev_func.c---[cwmpAddIPConnectionObject]---*num = ", *num);
	}
	
#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
	setMonitorCollectorParaList(IP_WAN_TYPE,atoi(deviceNode->parentNode->name) - 1,*num - 1,WAN_DEV_ADD);
#endif
#ifdef  TCSUPPORT_CT_WANNINDEX_INCREASE
	//usedWanIndex.entryIndex[pvc_index] = i;
	sprintf(wan_pvc_node,"Wan_PVC%d",pvc_index);
	sprintf(lastUsedEntryIndex,"%d",i);
	tcapi_set(wan_pvc_node,"lastUsedEntryIndex",lastUsedEntryIndex);	
#endif
	
#if defined(TCSUPPORT_CT_FJ)
	setProvisioningCode("INTERNET");
#endif

	ret = 0;
	return ret;
}

int cwmpInitIPConnectionObjectSubtree(DeviceNodePtr deviceNode)
{
	char wanConnNodeName[32] = {0};
	char entryNodeName[32] = {0};
	DeviceNodePtr wanConnNodeNamePtr = NULL, entryNodeNamePtr = NULL;
	char wan_node[32], active[10], isp[10], br_type[20];
	char wan_if_node[32];
	char node_index[4];
	uint32 pvc_index;
	int isp_index;
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	uint32 pvctmp, entrytmp;
#endif
	int ret = 0, i, j;
	tc_printf("%s: enter.\n", __FUNCTION__);
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvc_index = atoi(deviceNode->parentNode->name);
	FindPVCMapping(&pvc_index);
#else
	pvc_index = atoi(deviceNode->parentNode->name) - 1;
#endif	
	
	for (i =0 ; i < TOTAL_REMOTE_ENTRY_NODE; i++) {
		sprintf(wan_node, "Wan_PVC%d_Entry%d", pvc_index,i);
		
		ret = tcapi_get(wan_node, "Active", active);
		if (ret < 0){
			/* this pvc  entry is not exist */
			continue;
		}
		memset(isp, 0, sizeof(isp) );
		tcapi_get(wan_node, "ISP", isp);
		isp_index = atoi(isp);
		if( isp_index == 2)
		//if( (isp_index != 0) && (isp_index != 1 ) && (isp_index != 3) )
		{
			continue;
		}

		if( 3 == isp_index)
		{
			char bridgeType[32]={0};
			tcapi_get(wan_node,"BridgeMode",bridgeType);
			
			if( 0 != strcmp(bridgeType,"IP_Bridged"))
				continue;
		}
		
		
		/* register this node */
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	entrytmp = i;
	pvctmp = pvc_index;
	GetWanNodeMapping(&pvctmp, &entrytmp); /* PVC_Entry{i} is exist, get "Index" to entrytmp */
	if(entrytmp == 0){
		tcdbg_printf("%s=> PVC%d_Entry%d have not 'Index', update this Entry mapping table.\n", __FUNCTION__, pvc_index, i);
		UpdateEntryMapping(pvc_index, i);
		pvctmp = pvc_index;
		entrytmp = i;
		GetWanNodeMapping(&pvctmp, &entrytmp); /* After update mapping table, get "Index" again */
	}
	sprintf(node_index, "%d", entrytmp);
#else
		sprintf(node_index, "%d", i + 1);
#endif
		WANIPConnectionIndex[0].name = node_index;
		if (RegisterNodeFromTable(deviceNode, WANIPConnectionIndex) == NULL) {
			tcdbg_printf("%s:error!.\n", __FUNCTION__);
			ret = -1;
			return ret;
		}
	}

	ret = 0;
	return ret;
}

/* add connection object */
int8 cwmpAddPPPConnectionObject(uint32* num, DeviceNode*  deviceNode)
{
	char wan_if_node[32], active[10];
	int ret = 0, i;
	uint32 pvc_index;
	int node_index;
	
	char nasName[12] = {0};
	char ifName[12] = {0};
	char attrName[12] = {0};
	char pppUnit[4] = {0};
	int n;

	DeviceNodePtr WANDeviceNode = NULL;
#if defined(TCSUPPORT_CT_PON)
		char wan_node[32] = {0};
		char vlanmode[20] = {0};
		char vlanid[12] = "0";
		char pbit[12] = "0";
		char newIFId[8] = "1";
#endif
#if defined(TCSUPPORT_CT_WAN_PTM)
		char wan_node[32] = {0};
		char ptmenable[20] = {0};
		char vlanmode[20] = {0};
		char vlanid[12] = "0";
		char pbit[12] = "0";
		char newIFId[8] = "1";
#endif
#ifdef  TCSUPPORT_CT_WANNINDEX_INCREASE
	char lastUsedEntryIndex[8] = {0};
	char wan_pvc_node[32] = {0};
#endif


#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	uint32 pvctmp;
	pvc_index = atoi(deviceNode->parentNode->name);
	FindPVCMapping(&pvc_index);
#else
	pvc_index = atoi(deviceNode->parentNode->name) - 1;
#endif
	tc_printf("Adding ppp connection device!\n");

#ifdef  TCSUPPORT_CT_WANNINDEX_INCREASE
	i = findUnUsedEntryIndex(pvc_index);
	sprintf(wan_if_node, "Wan_PVC%d_Entry%d", pvc_index, i);
#else
	for (i = 0; i < TOTAL_REMOTE_ENTRY_NODE; i++) {
		sprintf(wan_if_node, "Wan_PVC%d_Entry%d", pvc_index, i);
		ret = tcapi_get(wan_if_node, "Active", active);
		if (ret < 0) {
			break;
		}
	}
#endif

	if (i == TOTAL_REMOTE_ENTRY_NODE) {
		tcdbg_printf("error1 cwmpAddConnectionObject\n");
		return -1;
	}
#ifndef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	*num = i + 1;
#endif

	/* init the connection info */
	sprintf(nasName,"nas%d_%d",pvc_index, i);
	sprintf(ifName, "ppp%d", pvc_index<<3 | i);
	sprintf(pppUnit, "%d", pvc_index<<3 | i);

	tcapi_set(wan_if_node,"ISP", "2");
	tcapi_set(wan_if_node, "Active", "Yes");
	tcapi_set(wan_if_node, "ServiceList","INTERNET");
	tcapi_set(wan_if_node, "BandActive","N/A");
	tcapi_set(wan_if_node, "WanMode","Route");
	tcapi_set(wan_if_node, "LinkMode","linkPPP");
	tcapi_set(wan_if_node, "PPPGETIP","Dynamic");
#if defined(TCSUPPORT_CT_PON_JS)
	tcapi_set(wan_if_node, "IPVERSION","IPv4/IPv6");
	tcapi_set(wan_if_node, "PDEnable","Yes");
	tcapi_set(wan_if_node, "PDOrigin","PrefixDelegation");
#else
	tcapi_set(wan_if_node, "IPVERSION","IPv4");
#endif
	tcapi_set(wan_if_node, "DHCPOption60","");
	tcapi_set(wan_if_node, "dot1q","No");
	tcapi_set(wan_if_node, "dot1p","No");
#if defined(TCSUPPORT_CT_PON)
	tcapi_set(wan_if_node, "MTU","1492");
	sprintf(wan_node, "Wan_PVC%d", pvc_index);
	tcapi_get(wan_node, "VLANMode", vlanmode);
	tcapi_set(wan_if_node, "VLANMode", vlanmode);
	if ( 0 == strcmp("TAG", vlanmode) )
	{
		tcapi_get(wan_node, "VLANID", vlanid);
		tcapi_get(wan_node, "DOT1P", pbit);
		tcapi_set(wan_if_node, "dot1q", "Yes");
		tcapi_set(wan_if_node, "dot1p", "Yes");
		tcapi_set(wan_if_node, "VLANID", vlanid);
		tcapi_set(wan_if_node, "dot1pData", pbit);
	}
	sprintf(newIFId, "%u", getNewIFaceID());
	tcapi_set(wan_if_node, "IFIdx", newIFId);
	tcapi_set("Wan_Common", "LatestIFIdx", newIFId);
#endif
#if defined(TCSUPPORT_CT_WAN_PTM)
	tcapi_set(wan_if_node, "MTU","1492");
	sprintf(wan_node, "Wan_PVC%d", pvc_index);
	tcapi_get(wan_node, "PTMEnable", ptmenable);
	if ( 0 == strcmp("Yes", ptmenable) )
	{		
		tcapi_get(wan_node, "VLANMode", vlanmode);
		tcapi_set(wan_if_node, "Barrier", "0");
		tcapi_set(wan_if_node, "VLANMode", vlanmode);
		if ( 0 == strcmp("TAG", vlanmode) )
		{
			tcapi_get(wan_node, "VLANID", vlanid);
			tcapi_get(wan_node, "DOT1P", pbit);
			tcapi_set(wan_if_node, "dot1q", "Yes");
			tcapi_set(wan_if_node, "dot1p", "Yes");
			tcapi_set(wan_if_node, "VLANID", vlanid);
			tcapi_set(wan_if_node, "dot1pData", pbit);
		}
	}
	sprintf(newIFId, "%u", getNewIFaceID());
	tcapi_set(wan_if_node, "IFIdx", newIFId);
	tcapi_set("Wan_Common", "LatestIFIdx", newIFId);
#endif

	tcapi_set(wan_if_node, "NATENABLE","Enable");
	tcapi_set(wan_if_node, "IGMPproxy","No");
	tcapi_set(wan_if_node, "NASName",nasName);
	tcapi_set(wan_if_node, "IFName",ifName);
	tcapi_set(wan_if_node, "PPPUNIT",pppUnit);
#if defined(TCSUPPORT_CT_PON_SC)
	tcapi_set(wan_if_node, "CONNECTION", "Connect_on_Demand");
#else
	tcapi_set(wan_if_node, "CONNECTION", "Connect_Keep_Alive");
#endif
#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
#if defined(TCSUPPORT_CT_PON_CZ_QH)
	tcapi_set(wan_if_node, "CLOSEIFIDLE", "900");
#else
#if defined(TCSUPPORT_CT_FJ)
	tcapi_set(wan_if_node, "CLOSEIFIDLE", "600"); // default idle time to 10min
#else
	tcapi_set(wan_if_node, "CLOSEIFIDLE", "1800"); // default idle time to 30min
#endif
#endif
#endif
	tcapi_set(wan_if_node, "DHCPRealy","No");
	tcapi_set(wan_if_node, "DHCPEnable", "1");
        tcapi_set(wan_if_node, "DHCPv6", "No");
	tcapi_set(wan_if_node, "DHCPv6PD", "Yes");

	for(n = 1; n <= 4; n++)
	{
		memset(attrName, 0, sizeof(attrName) );
		sprintf(attrName, "LAN%d", n);
		tcapi_set(wan_if_node, attrName,"No");

		memset(attrName, 0, sizeof(attrName) );
		sprintf(attrName, "SSID%d", n);
		tcapi_set(wan_if_node, attrName,"No");
	}
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	UpdateEntryMapping(pvc_index, i);
	*num = i;
	pvctmp = pvc_index;
	GetWanNodeMapping(&pvctmp, num); /* get Wan_PVC%d_Entry%d -> Node: Index */
#endif

	spSysParam.tcCwmpAttr->vWANLinkTypeFlag[pvc_index] |=  (1<<i);

	/* register the parameter tree */
	cwmpFreeDeviceTable(deviceNode);
	ret = cwmpInitPPPConnectionObjectSubtree(deviceNode);
	if(dbg_cwmp >= DBG_CWMP_L2 ){
			tc_printf("\r\ncp_wandev_wanconndev_func.c---[cwmpAddPPPConnectionObject]---*num = ", *num);
	}
	

#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
	setMonitorCollectorParaList(PPP_WAN_TYPE,atoi(deviceNode->parentNode->name) - 1,*num - 1,WAN_DEV_ADD);
#endif
#ifdef  TCSUPPORT_CT_WANNINDEX_INCREASE
	//usedWanIndex.entryIndex[pvc_index] = i;
	sprintf(wan_pvc_node,"Wan_PVC%d",pvc_index);
	sprintf(lastUsedEntryIndex,"%d",i);
	tcapi_set(wan_pvc_node,"lastUsedEntryIndex",lastUsedEntryIndex);	
#endif

#if defined(TCSUPPORT_CT_FJ)
	setProvisioningCode("INTERNET");
#endif

	ret = 0;
	return ret;
}


int cwmpInitPPPConnectionObjectSubtree(DeviceNodePtr deviceNode)
{
	char wanConnNodeName[32] = {0};
	char entryNodeName[32] = {0};
	DeviceNodePtr wanConnNodeNamePtr = NULL, entryNodeNamePtr = NULL;
	char wan_node[32], active[10], isp[10], br_type[20];
	char wan_if_node[32];
	char node_index[4];
	uint32 pvc_index;
	int isp_index;
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	uint32 pvctmp, entrytmp;
#endif
	int ret = 0, i, j;
	tc_printf("%s:enter.\n", __FUNCTION__);
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvc_index = atoi(deviceNode->parentNode->name);
	FindPVCMapping(&pvc_index);
#else
	pvc_index = atoi(deviceNode->parentNode->name) - 1;
#endif
	
	for (i =0 ; i < TOTAL_REMOTE_ENTRY_NODE; i++) {
		sprintf(wan_node, "Wan_PVC%d_Entry%d", pvc_index,i);
		
		ret = tcapi_get(wan_node, "Active", active);
		if (ret < 0){
			/* this pvc  entry is not exist */
			continue;
		}

		memset(isp, 0, sizeof(isp) );
		tcapi_get(wan_node, "ISP", isp);
		isp_index = atoi(isp);
		if(  (isp_index != 2 ) && (isp_index != 3) )
		{
			continue;
		}

		if( 3 == isp_index)
		{
			char bridgeType[32]={0};
			tcapi_get(wan_node,"BridgeMode",bridgeType);
			
			if( 0 != strcmp(bridgeType,"PPPoE_Bridged"))
				continue;
		}
		
		/* register this node */
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
		entrytmp = i;
		pvctmp = pvc_index;
		GetWanNodeMapping(&pvctmp, &entrytmp); /* PVC_Entry{i} is exist, get "Index" to entrytmp */
		if(entrytmp == 0){
			tcdbg_printf("%s=> PVC%d_Entry%d have not 'Index', update this Entry mapping table.\n", __FUNCTION__, pvc_index, i);
			UpdateEntryMapping(pvc_index, i);
			pvctmp = pvc_index;
			entrytmp = i;
			GetWanNodeMapping(&pvctmp, &entrytmp); /* After update mapping table, get "Index" again */
		}
		sprintf(node_index, "%d", entrytmp);
#else
		sprintf(node_index, "%d", i + 1);
#endif
		WANPPPConnectionIndex[0].name = node_index;
		if (RegisterNodeFromTable(deviceNode, WANPPPConnectionIndex) == NULL) {
			tcdbg_printf("%s:error!.\n", __FUNCTION__);
			ret = -1;
			return ret;
		}
	}

	ret = 0;
	return ret;
}

/* delete connection object */
int8 cwmpDeleteConnectionObject(uint32 num, DeviceNode *deviceNode)
{
	DeviceNodePtr WANDeviceNode = NULL;
	char wan_if_node[32], active[10];
	uint32 conn_index, pvc_index;
	int ret;
#if defined(TCSUPPORT_CT_PON_SN)	
	char servicelist[64] = {0};
#endif

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvc_index = atoi(deviceNode->parentNode->parentNode->name);
	conn_index = num;
	ret = FindWanNodeMapping(&pvc_index, &conn_index);
#else
	pvc_index = atoi(deviceNode->parentNode->parentNode->name) - 1;
	conn_index = num - 1;
#endif

	tcdbg_printf("Deleting connection device, deviceNode:%s!\n", deviceNode->name);
	
	/* check if this entry is exist */
	sprintf(wan_if_node, "Wan_PVC%d_Entry%d", pvc_index, conn_index);
	tcdbg_printf("%s\n", wan_if_node);
#if defined(TCSUPPORT_CT_PON_SN)	
	ret = tcapi_get(wan_if_node, "ServiceList", servicelist);
	if (ret < 0)
	{
		tcdbg_printf("Get %s servicelist fail!\n",wan_if_node);
		return -1;
	}
	if(NULL != strstr(servicelist,"TR069"))
	{
		return -1;
	}
#endif

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	if (ret < 0) {
		tcdbg_printf("node not found!\n");
		return -1;
	}
#else
	ret = tcapi_get(wan_if_node, "Active", active);
	if (ret < 0) {
		tcdbg_printf("%s : node not found!\n", wan_if_node);
		return -1;
	}
#endif

	// commit mechanism
	// no need to reinit the tree here, the commit mechanism will do it later
	spSysParam.tcCwmpAttr->vWANLinkTypeFlag[pvc_index] &=  ~(1<<conn_index);
	SetCfgNodeFlag ( "Wan", (pvc_index <<3 |conn_index) + WAN_ENTRY_UNSET_ALIAS +COMMIT_THIRD_ALIAS );



#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
	setMonitorCollectorParaList(WAN_TYPE_NOCARE,atoi(deviceNode->parentNode->parentNode->name) - 1,num - 1,WAN_DEV_DEL);
#endif

	ret = 0;	
	return ret;
}

int8 getWANPPPCntNumOfEntries1_1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char wan_if_node[32], isp[4], br_type[6];
	int i, ret, ppp_num = 0;
	uint32 pvc_index;
	char ppp_num_buf[4];

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvc_index = atoi(deviceNode->parentNode->name);
	FindPVCMapping(&pvc_index);
#else
	pvc_index = atoi(deviceNode->parentNode->name) - 1;
#endif

	for (i = 0; i < TOTAL_REMOTE_ENTRY_NODE; i++) {
		sprintf(wan_if_node, "Wan_PVC%d_Entry%d", pvc_index, i);
		tcdbg_printf("%s :%s\n",deviceNode->name, wan_if_node);
		ret = tcapi_get(wan_if_node, "ISP", isp);
		if (ret < 0) {
			continue;
		}
		/* ppp connection */
		if (!strcmp(isp, WAN_ENCAP_PPP)) {
			ppp_num += 1;
		}
		else if (!strcmp(isp, WAN_ENCAP_BRIDGE)) {
			/* ppp bridge */
			tcapi_get(wan_if_node, "BRIDGEDTYPE", br_type);
			if (!strcmp(br_type, "PPP")) {
				ppp_num += 1;
			}
		}
	}

	sprintf(ppp_num_buf,"%d", ppp_num);	
	return getTheValue(value, type, deviceNode, ppp_num_buf);
}
int8 getWANPPPCntNumOfEntries1_1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint32 instance;

	instance = atoi(deviceNode->parentNode->name);
#ifdef  TCSUPPORT_CT_WANNODE_MAPPING//tony add
	FindPVCMapping(&instance);//
	instance += 1;
#endif
	attribute = tcIpConAttr.aWANPPPConnectionNumOfEntries[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPPPCntNumOfEntries1_1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	FindPVCMapping(&instance);//
	instance += 1;
#endif
	attribute = &tcIpConAttr.aWANPPPConnectionNumOfEntries[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

int8 getWANIPCntNumOfEntries1_1Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char wan_if_node[32], isp[4], br_type[6];
	int i, ret, ip_num = 0;
	char ip_num_buf[4];
	uint32 pvc_index;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	pvc_index = atoi(deviceNode->parentNode->name);
	FindPVCMapping(&pvc_index);
#else
	pvc_index = atoi(deviceNode->parentNode->name) - 1;
#endif

	for (i = 0; i < TOTAL_REMOTE_ENTRY_NODE; i++) {
		sprintf(wan_if_node, "Wan_PVC%d_Entry%d", pvc_index, i);
		tcdbg_printf("%s : %s\n",deviceNode->name, wan_if_node);
		ret = tcapi_get(wan_if_node, "ISP", isp);
		if (ret < 0) {
			continue;
		}
		/* ip connection */
		if (!strcmp(isp, WAN_ENCAP_DYN) || !strcmp(isp, WAN_ENCAP_STATIC)) {
			ip_num += 1;
		}
		else if (!strcmp(isp, WAN_ENCAP_BRIDGE)) {
			/* ip bridge */
			tcapi_get(wan_if_node, "BRIDGEDTYPE", br_type);
			if (!strcmp(br_type, "IP")) {
				ip_num += 1;
			}
		}
	}
	sprintf(ip_num_buf,"%d", ip_num);	
	return getTheValue(value, type, deviceNode, ip_num_buf);
}
int8 getWANIPCntNumOfEntries1_1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	uint32 instance;

	instance = atoi(deviceNode->parentNode->name);
#ifdef  TCSUPPORT_CT_WANNODE_MAPPING//tony add
	FindPVCMapping(&instance);//
	instance += 1;
#endif
	attribute = tcIpConAttr.aWANIPConnectionNumOfEntries[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANIPCntNumOfEntries1_1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	uint32 instance;

	instance = atoi(deviceNode->parentNode->name);
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	FindPVCMapping(&instance);//
	instance += 1;
#endif
	attribute = &tcIpConAttr.aWANIPConnectionNumOfEntries[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
	return result;
}

#if defined(TCSUPPORT_CT_PON)
int cwmpInitWANConnectionObjectSubtree(DeviceNodePtr deviceNode)
{
	int ret = 0;
	char nodeName[33] = {0};
	char linkSta[4] = "0";
	int ponsw = 0;

	/* register this node */
	if (RegisterNodeFromTable(deviceNode, WANConnectionDevice_table) == NULL) {
		tcdbg_printf("%s:error.\n", __FUNCTION__);
		ret = -1;
		return ret;
	}

#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)

	// check GPon or EPon
	strcpy(nodeName, "XPON_LinkCfg");
	tcapi_get(nodeName, "LinkSta", linkSta);
	ponsw = atoi(linkSta);
	if ( 0 == ponsw ) // down
		return ret;
	else if ( 1 == ponsw ) // gpon
	{
		if (RegisterNodeFromTable(deviceNode, WANGponLinkConfigNode) == NULL)
		{
			tcdbg_printf("%s:error.\n", __FUNCTION__);
			ret = -1;
			return ret;
		}
	}
	else if ( 2 == ponsw ) // epon
	{
		if (RegisterNodeFromTable(deviceNode, WANEponLinkConfigNode) == NULL)
		{
			tcdbg_printf("%s:error.\n", __FUNCTION__);
			ret = -1;
			return ret;
		}
	}
#endif

	return ret;
}

#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getWANVLANEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
	{
		char nodeName[33] = {0};
		char mode[20] = "UNTAG";
		uint8 instant = 0;
	
		if ( NULL == deviceNode
			|| NULL == deviceNode->parentNode)
			return -1;

	    instant = (uint8)getCU_WANLinkIndex(deviceNode);
	
		if ( instant >= MAX_WAN_NODE )
			return -1;
	
		sprintf(nodeName, "Wan_PVC%d", instant);
		tcapi_get(nodeName, "VLANMode", mode);
	
		if ( 0 == strcmp(mode, "UNTAG") )
			strcpy(mode, "0");
		else if ( 0 == strcmp(mode, "TAG") )
			strcpy(mode, "1");
		else
			strcpy(mode,"0");
	
		return getTheValue(value, type, deviceNode, mode);
	}

int8 setWANVLANEnabledValue(void* value, struct DeviceNode*  deviceNode)
	{
		char nodeName[33] = {0};
		char modeStr[20] = {0};
		char oldmodeStr[20] = {0};
		char vlanid[12] = {0};
		char pbit[12] = {0};
		uint8 instant = 0;
		char *vid_untag = "4096";
		char *vid_transp = "4097";
		char *pbit_u = "0";
		uint8 mode = *((uint8*)value);
	
		if ( NULL == deviceNode
			|| NULL == deviceNode->parentNode)
			return -1;
	
	    instant = (uint8)getCU_WANLinkIndex(deviceNode);
		if ( instant >= MAX_WAN_NODE )
			return -1;
	
		sprintf(nodeName, "Wan_PVC%d", instant);
		tcapi_get(nodeName, "VLANMode", oldmodeStr);
		if ( 0 == strcmp(oldmodeStr, "TAG") )
		{
			tcapi_get(nodeName, "VLANID", vlanid);
			tcapi_get(nodeName, "DOT1P", pbit);
		}
	
		switch ( mode )
		{
			case 0:
					strcpy(modeStr, "UNTAG");
				   if ( 1 == isExistDuplicateChannel(modeStr, vlanid, pbit, instant) )
					return -1;
					updateVLANID_PBIT(modeStr, vid_untag, pbit_u, instant);

				break;
			case 1:
				strcpy(modeStr, "TAG");
				if ( 0 == strcmp("UNTAG", oldmodeStr))
					updateVLANID_PBIT(modeStr, "", "0", instant);
				break;
			default:
				break;
		}
	
		sprintf(nodeName, "Wan_PVC%d", instant);
		tcapi_set(nodeName, "VLANMode", modeStr);
		SetCfgNodeFlag ( "Wan", instant );
		return 0;
	}

int8 getWANVLANEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
		uint8  attribute;
		int8  result;
		uint32 instance;
	
	    instance = (uint8)getCU_WANLinkIndex(deviceNode);

		attribute = ctPonLinkAttr.aWANVLANEnable[instance - 1];
		result=GetTheAttributes(attribute,notification, acl, deviceNode);
		return result;
	}

int8 setWANVLANEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
		uint8*	attribute;
		int8  result;
		int instance;
	
	    instance = (uint8)getCU_WANLinkIndex(deviceNode);

		attribute = &ctPonLinkAttr.aWANVLANEnable[instance - 1];
		result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
		rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
		return result;
}


int8 getWANVLANValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
		char nodeName[33] = {0};
		char vlanid[20] = "0";
		char mode[20] = {0};
		uint8 instant = 0;
	
		if ( NULL == deviceNode
			|| NULL == deviceNode->parentNode)
			return -1;
	
	    instant = (uint8)getCU_WANLinkIndex(deviceNode);
		if ( instant >= MAX_WAN_NODE )
			return -1;
	
		sprintf(nodeName, "Wan_PVC%d", instant);
		tcapi_get(nodeName, "VLANMode", mode);
		if ( 0 == strcmp(mode, "TAG") )
		{
			tcapi_get(nodeName, "VLANID", vlanid);
		}
	
		return getTheValue(value, type, deviceNode, vlanid);
}

int8 setWANVLANValue(void* value, struct DeviceNode*  deviceNode)
{
		char nodeName[33] = {0};
		char vidStr[20] = {0};
		char pbitStr[20] = "0";
		char mode[20] = {0};
		uint8 instant = 0;
		uint32 vlanid = *((uint32*)value);
	
		if ( NULL == deviceNode
			|| NULL == deviceNode->parentNode)
			return -1;
	
	    instant = (uint8)getCU_WANLinkIndex(deviceNode);
		if ( instant >= MAX_WAN_NODE )
			return -1;
	
		if ( vlanid > 4094 )
			return -1;
	
		sprintf(nodeName, "Wan_PVC%d", instant);
		tcapi_set(nodeName, "VLANMode", "TAG");
		
		tcapi_get(nodeName, "VLANMode", mode);
		if ( 0 != strcmp(mode, "TAG") )
			return -1;

		tcapi_get(nodeName, "DOT1P", pbitStr);
		sprintf(vidStr, "%u", vlanid);
		if ( 1 == isExistDuplicateChannel(mode, vidStr, pbitStr, instant) )
			return -1;
	
		tcapi_set(nodeName, "VLANID", vidStr);
		updateVLANID_PBIT(mode, vidStr, pbitStr, instant);
		SetCfgNodeFlag ( "Wan", instant );
		return 0;
}

int8 getWANVLANAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
			uint8  attribute;
			int8  result;
			uint32 instance;
		
	        instance = (uint8)getCU_WANLinkIndex(deviceNode);
	
			attribute = ctPonLinkAttr.aWANVLAN[instance - 1];
			result=GetTheAttributes(attribute,notification, acl, deviceNode);
			return result;
}

int8 setWANVLANAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
			uint8*	attribute;
			int8  result;
			int instance;
		
	        instance = (uint8)getCU_WANLinkIndex(deviceNode);
	
			attribute = &ctPonLinkAttr.aWANVLAN[instance - 1];
			result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
			rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
			return result;
	}


int8 getWAN8021PValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
		char nodeName[33] = {0};
		char priority[20] = "0";
		char mode[20] = {0};
		uint8 instant = 0;
	
		if ( NULL == deviceNode
			|| NULL == deviceNode->parentNode)
			return -1;
		
	    instant = (uint8)getCU_WANLinkIndex(deviceNode);
	
		if ( instant >= MAX_WAN_NODE )
			return -1;
	
		sprintf(nodeName, "Wan_PVC%d", instant);
		tcapi_get(nodeName, "VLANMode", mode);
		if ( 0 == strcmp(mode, "TAG") )
		{
			tcapi_get(nodeName, "DOT1P", priority);
		}
	
		return getTheValue(value, type, deviceNode, priority);
}

int8 setWAN8021PValue(void* value, struct DeviceNode*  deviceNode)
{
		char nodeName[33] = {0};
		char mode[20] = {0};
		char priorityStr[20] = {0};
		char vlanidStr[20] = {0};
		uint8 instant = 0;
		uint32 priValue = *((uint32*)value);
	
		if ( NULL == deviceNode
			|| NULL == deviceNode->parentNode)
			return -1;
	
	   instant = (uint8)getCU_WANLinkIndex(deviceNode);
	   
		if ( instant >= MAX_WAN_NODE )
			return -1;
	
		if ( priValue > 7)
			return -1;
	
		sprintf(nodeName, "Wan_PVC%d", instant);
#if 1
		tcapi_set(nodeName, "VLANMode", "TAG");
#endif
		tcapi_get(nodeName, "VLANMode", mode);
		if ( 0 != strcmp(mode, "TAG") )
			return -1;
	
		tcapi_get(nodeName, "VLANID", vlanidStr);
		sprintf(priorityStr, "%u", priValue);
		if ( 1 == isExistDuplicateChannel(mode, vlanidStr, priorityStr, instant) )
			return -1;
	
		tcapi_set(nodeName, "DOT1P", priorityStr);
		updateVLANID_PBIT(mode, vlanidStr, priorityStr, instant);
		SetCfgNodeFlag ( "Wan", instant );
		return 0;
	}
int8 getWAN8021PAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
				uint8  attribute;
				int8  result;
				uint32 instance;
			
			    instance = (uint8)getCU_WANLinkIndex(deviceNode);
		
				attribute = ctPonLinkAttr.aWANVLAN8021p[instance - 1];
				result=GetTheAttributes(attribute,notification, acl, deviceNode);
				return result;
}

int8 setWAN8021PAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
			uint8*	attribute;
			int8  result;
			int instance;
		
	        instance = (uint8)getCU_WANLinkIndex(deviceNode);
	
			attribute = &ctPonLinkAttr.aWANVLAN8021p[instance - 1];
			result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
			rpc.attrFlags.theSettingFlag0 |= IPCON_ATTR_FLAG;
			return result;
	}

#endif

#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
int isParaListExist(char* paraList_in, int entryIndex){
	char nodeName[32],attr[32],paraList[256],tempvalue[32] = {0};	
	int iParaListNum = 0;
	int j = 0;
	int ret = -1;
	
	memset(nodeName, 0, sizeof(nodeName));
	memset(tempvalue, 0, sizeof(tempvalue));
	sprintf(nodeName, MONITOR_COLLECTOR_ENTRY, entryIndex);
	ret = tcapi_get(nodeName, "ParaListNum", tempvalue);
	if(ret < 0 || (iParaListNum = atoi(tempvalue)) == 0) {
		tcdbg_printf("[%s]: is not exist, no need add\n",__FUNCTION__,nodeName);
		return -1;
	}

	for(j = 0; j < iParaListNum; j++){				
		memset(paraList,0,sizeof(paraList));
		memset(attr, 0, sizeof(attr));
		sprintf(attr,"ParaList%d",j);			
		tcapi_get(nodeName, attr, paraList);
		if(strcmp(paraList_in,paraList) == 0){		
			tcdbg_printf("[%s]:is already exist, no need add\n",__FUNCTION__,paraList_in);
			return 1;
		}
		
	}

	tcdbg_printf("[%s]:is not exist, need add\n",__FUNCTION__,paraList_in);
	return 0;
}

int getParaListItem(char* paraList, int ip_ppp, int pvcIndex, int entryIndex, int item){
	char tmp[256] = {0};		
	char tmpValue[8] = {0};
	
	if(pvcIndex == -1 || entryIndex == -1){		
		tcdbg_printf("[%s:%d]pvcIndex = -1,entryIndex == -1,ERROR!!!\n",__FUNCTION__,__LINE__);
		return -1;
	}

	strcat(tmp,"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.");

	memset(tmpValue, 0, sizeof(tmpValue));
	sprintf(tmpValue,"%d",pvcIndex + 1);
	strcat(tmp,tmpValue);

	switch(ip_ppp){
		case IP_WAN_TYPE:			
			strcat(tmp,".WANIPConnection.");
			break;
		case PPP_WAN_TYPE:
			strcat(tmp,".WANPPPConnection.");
			break;	
		default:			
			strcpy(paraList,"");
			tcdbg_printf("[%s:%d]ip_ppp = %d,ERROR!!!\n",__FUNCTION__,__LINE__,ip_ppp);
			return -1;
	}

	memset(tmpValue, 0, sizeof(tmpValue));
	sprintf(tmpValue,"%d.",entryIndex + 1);
	strcat(tmp,tmpValue);

	switch(item){
		case wan_ConnectionStatus:			
			strcat(tmp,"ConnectionStatus");
			break;
		case wan_LastConnectionError:
			strcat(tmp,"LastConnectionError");
			break;	
#if defined(TCSUPPORT_CT_PPPERRCODE_ENHANCE)
		case wan_IPv6ConnectionStatus:
#if defined(TCSUPPORT_CT_PON_GDV20)
			strcat(tmp,"X_CT-COM_IPv6ConnStatus");
#else
			strcat(tmp,"X_CT-COM_IPv6ConnectionStatus");
#endif
			break;
#if defined(TCSUPPORT_CT_PON_GDV20)
		case wan_LastConnectionErrorCode:
			strcat(tmp,"LastConnectionErrorCode");
			break;
#else
		case wan_ConnectionType:
			strcat(tmp,"ConnectionType");
			break;
#endif
#endif
		default:			
			strcpy(paraList,"");
			tcdbg_printf("[%s:%d]item = %d, not support!!!\n",__FUNCTION__,__LINE__,item);
			return -1;
	}

	strcpy(paraList,tmp);
	return 0;

}


void addWanInParaList(int ip_ppp, int pvcIndex, int entryIndex){
	char tmp[256] = {0};	
	char paraList[256] = {0};
	char nodeName[32] = {0};		
	char attr[32] = {0};	
	int iParaListNum = 0;
	int iParaListLen = 0;
	int i = 0;
	int j = 0;
	int ret = 0;
	int addWanItem = wan_ConnectionStatus;
	int len = 0;
	int len_add = 0;
	int addNum = 0;

	for(i = 0; i < MONITOR_COLLECTOR_MAXNUM; i++){		
		memset(tmp,0,sizeof(tmp));
		memset(nodeName,0,sizeof(nodeName));
		sprintf(nodeName,MONITOR_COLLECTOR_ENTRY,i);		
		ret = tcapi_get(nodeName, "ParaListNum", tmp);
		if(ret != 0){
			continue;
		}

		iParaListNum = atoi(tmp);
		for(j = 0; j < iParaListNum; j++){	
			memset(attr, 0, sizeof(attr));
			sprintf(attr,"ParaList%d",j);
			tcapi_get(nodeName, attr, tmp);
			if(strlen(tmp) == 0){			//add in the empty item
				memset(paraList,0,sizeof(paraList));
				getParaListItem(paraList,ip_ppp, pvcIndex, entryIndex,addWanItem);	
				if(isParaListExist(paraList,i) == 0 && strlen(paraList) != 0){
					tcapi_set(nodeName, attr, paraList);
					len = strlen(paraList);
					len_add += len;
					addNum++;
				}
				addWanItem++;
			}		

			if(addWanItem >= max_paraList_wan_item){	
				//the wan instance(ConnectionStatus,LastConnectionError) is already added in paraList.
				break;
			}					
		}

		if(j >= iParaListNum){
			//the wan instance(ConnectionStatus,LastConnectionError) is not add into paraList yet.
			for(; addWanItem < max_paraList_wan_item; ){				
				memset(paraList,0,sizeof(paraList));
				getParaListItem(paraList,ip_ppp, pvcIndex, entryIndex,addWanItem);		
				if(isParaListExist(paraList,i) != 0){					
					addWanItem++;
					continue;
				}
				if(strlen(paraList) != 0){
					memset(attr, 0, sizeof(attr));
					sprintf(attr,"ParaList%d",j);
					tcapi_set(nodeName, attr, paraList);
					len = strlen(paraList);
					len_add += len;
					addNum++;
					j++;
					addWanItem++;
				}
			}
		}

		//set ParaListLen
		memset(tmp,0,sizeof(tmp));
		tcapi_get(nodeName, "ParaListLen", tmp);
		iParaListLen = atoi(tmp);
		iParaListLen += len_add + addNum ;
		sprintf(tmp,"%d",iParaListLen);
		tcapi_set(nodeName, "ParaListLen", tmp);
		
		if(j > iParaListNum){		
			//add new ParaList item, need set ParaListNum
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"%d",j);
			tcapi_set(nodeName, "ParaListNum", tmp);
		}

		//only add in the first ParaList
		return;
	}

	return;
}


int isParaListItemNeedDel(char* paraList, int pvc_in, int entry_in){
	char tempvalue[8] = {0};
	char* ptr = NULL;
	int pvcIndex = 0;
	int entryIndex = 0;
	
	if((ptr = strstr(paraList,"WANConnectionDevice")) != NULL){
		memset(tempvalue, 0, sizeof(tempvalue));
		tempvalue[0] = *(ptr + strlen("WANConnectionDevice") + 1);
		tempvalue[1] = '\0';
		pvcIndex = atoi(tempvalue);

		//check pvc index
		if(pvc_in != pvcIndex - 1){
			return 0;
		}
		
			
		memset(tempvalue, 0, sizeof(tempvalue));
		if((ptr = strstr(paraList,"WANPPPConnection"))!= NULL){
			tempvalue[0] = *(ptr + strlen("WANPPPConnection") + 1);		
			tempvalue[1] = '\0';
			entryIndex = atoi(tempvalue);	
		}else if((ptr = strstr(paraList,"WANIPConnection"))!= NULL){
			tempvalue[0] = *(ptr + strlen("WANIPConnection") + 1);	
			tempvalue[1] = '\0';
			entryIndex = atoi(tempvalue);	
		}else{
			entryIndex = -1;
		}

		//check entry index
		if(entry_in != entryIndex - 1){
			return 0;
		}

		return 1;
	}

	return 0;
}



void delWanInParaList(int pvcIndex, int entryIndex){	
	char tmp[256] = {0};
	char nodeName[32] = {0};		
	char attr[32] = {0};	
	int iParaListNum = 0;
	int iParaListLen = 0;
	int i = 0;
	int j = 0;
	int ret = 0;
	int len = 0;
	int len_del = 0;
	int delNum = 0;

	for(i = 0; i < MONITOR_COLLECTOR_MAXNUM; i++){	
		delNum = 0;
		memset(tmp,0,sizeof(tmp));
		memset(nodeName,0,sizeof(nodeName));
		sprintf(nodeName,MONITOR_COLLECTOR_ENTRY,i);		
		ret = tcapi_get(nodeName, "ParaListNum", tmp);
		if(ret != 0){
			continue;
		}

		iParaListNum = atoi(tmp);
		for(j = 0; j < iParaListNum; j++){				
			memset(tmp,0,sizeof(tmp));
			memset(attr, 0, sizeof(attr));
			sprintf(attr,"ParaList%d",j);
			tcapi_get(nodeName, attr, tmp);
			len = strlen(tmp);
			if(len != 0 && isParaListItemNeedDel(tmp,pvcIndex,entryIndex) == 1){
				tcapi_set(nodeName, attr, "");
				len_del += len;
				delNum++;
			}			
		}


		if(delNum != 0){
			memset(tmp,0,sizeof(tmp));
			tcapi_get(nodeName, "ParaListLen", tmp);
			iParaListLen = atoi(tmp);
			iParaListLen = iParaListLen - len_del - delNum;
			sprintf(tmp,"%d",iParaListLen);
			tcapi_set(nodeName, "ParaListLen", tmp);
		}
	}

	return;
}

void setMonitorCollectorParaList(int ip_ppp,int pvcIndex, int entryIndex, int type){
	char nodeName[32] = {0};	
	char srvList[64] = {0};	

	switch(type){
		case WAN_DEV_ADD:						
			//check serviceList, only INTERNET	
			sprintf(nodeName, "Wan_PVC%d_Entry%d", pvcIndex,entryIndex);
			tcapi_get(nodeName,"ServiceList",srvList);
			if(strstr(srvList,INTERNET) == NULL){
				//not INTERNET wan
				return;
			}
			addWanInParaList(ip_ppp,pvcIndex,entryIndex);
			break;
		case WAN_DEV_DEL:
			delWanInParaList(pvcIndex,entryIndex);
			break;	
		default:
			break;

	}

	return;
}
#endif


