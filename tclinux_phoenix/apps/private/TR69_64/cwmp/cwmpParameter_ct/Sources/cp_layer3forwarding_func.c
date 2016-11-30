#include "Global_res.h"
#include "cp_layer3forwarding.h"

Route_Info_List *RouteHead = NULL;

int OperateRoute(int index)
{
	Route_Info_List *tempRoute = NULL;	
	char nodeName[32] = {0};
	char l3fTemp[16] = {0};
	char indexstr[4] = {0};
	char *p = NULL;
#if 1   //ROUTE_INTERFACE
	char *tmp = NULL;
	int ifIndex = -1;
	int pvcNode= 0, entryNode = 0;
#endif
	
	//first find if the route index in the route list
	for(tempRoute = RouteHead; tempRoute != NULL; tempRoute = tempRoute->next)
	{
		if(tempRoute->entryindex == index)
			break;
	}
	
	if(tempRoute == NULL)
	{
		tc_printf("\r\n[OperateRoute]error");
		return -1;
	}
	
	p = tempRoute->interface;
#if 1   //ROUTE_INTERFACE
	if(strstr(p, "ppp")){  /*ppp interface*/
		if(get_entry_number(p,"ppp",&ifIndex) == 0 ){
			pvcNode = ifIndex/MAX_SMUX_NUM;
			entryNode = ifIndex - pvcNode*MAX_SMUX_NUM;
			sprintf(nodeName,"Wan_PVC%d_Entry%d",pvcNode, entryNode);	
			tcapi_get(nodeName,"Gateway",l3fTemp);
			if(!_checkIp(l3fTemp)){
				l3fTemp[0] = '\0';
			}
		}		
	}
	else if(strstr(p, "nas")){   /*nas interface,ext:nas0_1*/
		pvcNode = *(p+3) - '0';
		entryNode = *(p+5) - '0';
		if((pvcNode >= 0 && pvcNode < MAX_SMUX_NUM) && (entryNode >= 0 && entryNode < MAX_SMUX_NUM)){
			sprintf(nodeName,"Wan_PVC%d_Entry%d",pvcNode, entryNode);	
			tcapi_get(nodeName,"Gateway",l3fTemp);
			if(!_checkIp(l3fTemp)){
				l3fTemp[0] = '\0';
			}
		}
	}
#else
	if(p[0] != 0)
	{
		sprintf(nodeName,"DeviceInfo_PVC%d",atoi(p+strlen(tempRoute->interface)-1));	
		tcapi_get(nodeName,"gateway",l3fTemp);
	}
#endif
	
	/*according to the gateway and the interface to judge whether need to commit new parameters*/
#if 1   //ROUTE_INTERFACE
	if(strlen(tempRoute->gateway) == 0 || strlen(tempRoute->interface) == 0 || strlen(l3fTemp) == 0 || strcmp(tempRoute->gateway,l3fTemp) == 0)
#else
	if(strlen(tempRoute->gateway) == 0 || strlen(tempRoute->interface) == 0 || strcmp(l3fTemp,"N/A") == 0 || strcmp(tempRoute->gateway,l3fTemp) == 0)
#endif
	{
		sprintf(nodeName,"%s_%s%d",ROUTE_NODE,ROUTE_SUB_NODE,index-1);
		/*first unset the old node parameters*/
		sprintf(indexstr,"%d",index-1);
		tcapi_set("WebCurSet_Entry","route_id",indexstr);
		tcapi_commit("WebCurSet_Entry");
		tcapi_unset(nodeName);
		tcapi_commit(ROUTE_NODE);
		/*then set the new node parameters*/
		tcapi_set(nodeName,"Active",tempRoute->flag);
		tcapi_set(nodeName,"DST_IP",tempRoute->destip);
		tcapi_set(nodeName,"Sub_mask",tempRoute->submask);
		if(tempRoute->gateway[0] != 0)
			tcapi_set(nodeName,"Gateway",tempRoute->gateway);
		if(tempRoute->interface[0] != 0)
#if 1   //ROUTE_INTERFACE
		tcapi_set(nodeName,"Device",tempRoute->interface);
#else
		tcapi_set(nodeName,"Interface",tempRoute->interface);
#endif
		tcapi_set(nodeName,"metric",tempRoute->metric);
		tcapi_set(nodeName,"User_def","1");//must be set!
		/*last commit the new node parameters*/
		tcapi_commit(ROUTE_NODE);
	}
	return 0;
}

Route_Info_List *AddRoute(int index)
{
	Route_Info_List *tempRoute = NULL;	
	Route_Info_List *pre = NULL;
	char nodeName[32] = {0};

	//first find if the route index in the route list
	for(tempRoute = RouteHead; tempRoute != NULL; tempRoute = tempRoute->next)
	{
		if(tempRoute->entryindex == index)
		{
			return tempRoute;
		}
	}
	
	tempRoute = NULL;
	tempRoute = (Route_Info_List *)os_malloc(sizeof(Route_Info_List));
	if(tempRoute == NULL)
	{	
		tc_printf("\r\nmalloc a new route fail!");
		return NULL;
	}

	//init,if the route is new ,get the paramters from node"Route_Entry" 
	memset(tempRoute,0,sizeof(Route_Info_List));
	sprintf(nodeName,"%s_%s%d",ROUTE_NODE,ROUTE_SUB_NODE,index-1);
	tcapi_get(nodeName,"Active",tempRoute->flag);
	tcapi_get(nodeName,"DST_IP",tempRoute->destip);
	tcapi_get(nodeName,"Sub_mask",tempRoute->submask);
	tcapi_get(nodeName,"Gateway",tempRoute->gateway);
#if 1   //ROUTE_INTERFACE
	tcapi_get(nodeName,"Device",tempRoute->interface);
#else
	tcapi_get(nodeName,"Interface",tempRoute->interface);
#endif
	tcapi_get(nodeName,"metric",tempRoute->metric);
	tempRoute->entryindex = index;
	tempRoute->next = NULL;

	//insert the new route into route list
	if(RouteHead == NULL)
	{
		RouteHead = tempRoute;
		return tempRoute;
	} 
	
	pre = RouteHead;
	while(pre->next != NULL)
	{
		pre = pre->next;
	}
	
	pre->next = tempRoute;

	return tempRoute;	
}

void FreeAllRouteMemory()
{
	Route_Info_List *temp = NULL;
	Route_Info_List *next = NULL;
	
	temp = RouteHead;
	while(temp != NULL)
	{	
		next = temp->next;
		os_free(temp);
		temp = next;
	}
	
	RouteHead = NULL;//must be set to NULL
	return ;
}

int getL3ForwardingIndex(DeviceNode*  deviceNode)
{
 	return atoi(deviceNode->parentNode->name);
}

int checkFreeRoute_new()
{
	char temp[4] = {0};
	int addnum = 0;
	int i = 0;

	tcapi_get(ROUTE_NODE,"add_num",temp);
	addnum = atoi(temp);
	if(addnum >= STATIC_ROUTE_NUM)
	{
		tc_printf("\r\n[cwmp]the max static route number is %d",STATIC_ROUTE_NUM);
		return -1;
	}

	/*we need to return the index of new routing item-----addnum is from 0~15*/
	return addnum;
}

int addrOffset = 1;
void InitMyForwardingTable(uint32 number)
{
	int userdefnum = 0,totalnum = 0;
	char temp[4] = {0};
	char nodeName[32] = {0};
	char fwtemp[16] = {0};
	char lanip[20] = {0};
	struct in_addr defaultaddr;
	struct in_addr defaultmask;
	struct in_addr tempaddr;

	sprintf(nodeName,"%s_%s%d",ROUTE_NODE,ROUTE_SUB_NODE,number);
	//defaultaddr.s_addr = inet_addr("192.168.1.12")/* + addrOffset*/;
	tcapi_get("Lan_Entry0","IP",lanip);
	if(strcmp(lanip,"") == 0)
			strcpy(lanip,"192.168.1.123");
	inet_aton(lanip,&tempaddr);
	inet_aton("255.255.255.0",&defaultmask);
	defaultaddr.s_addr = (tempaddr.s_addr & defaultmask.s_addr)+addrOffset;
	if(defaultaddr.s_addr == tempaddr.s_addr)
	{
		defaultaddr.s_addr += 1;
		addrOffset++;
	}
	sprintf(fwtemp,"%s",inet_ntoa(defaultaddr));
	tcapi_set(nodeName,"DST_IP",fwtemp);
	tcapi_set(nodeName,"Sub_mask","255.255.255.255");
	//tcapi_set(nodeName,"Gateway",fwtemp);
	tcapi_set(nodeName,"metric","1");
	tcapi_set(nodeName,"User_def","1");	//for user define route
	tcapi_set(nodeName, "Active", "No"); 
#if defined(TCSUPPORT_CMCC)
	tcapi_set(nodeName, "Active", "Yes"); /* default will be active. */
#endif

	#if 0
	memset(fwtemp,0,sizeof(fwtemp));
	sprintf(fwtemp,"%d",number);
	tcapi_set(WEB_SET_ROUTE_NODE,"route_id",fwtemp);
	
	tcapi_commit(WEB_SET_ROUTE_NODE);
	tcapi_commit(ROUTE_NODE);
	#endif
	addrOffset++;

	return;
}

int	cwmpInitForwardingTableFromFlash(DeviceNodePtr deviceNode)
{
	int totalnum = 0,userdefnum = 0;
	char temp[4] = {0};
	char nodeIndex[4] = {0};
	char nodeName[32] = {0};
	char dst_ip[32] = {0};
	int i,j = 0,rtnCode = 0;
	int index = 0;
	int ret = 0;
	
	if(deviceNode == NULL){
		return -1;
	}
	
	tcapi_get(ROUTE_NODE,"User_def_num",temp);
	userdefnum = atoi(temp);
	
	//we only fetch user define route
	for(i = 0; i < STATIC_ROUTE_NUM && userdefnum >= j; i++)
	{
		memset(temp,0,sizeof(temp));
		sprintf(nodeName,"%s_%s%d",ROUTE_NODE,ROUTE_SUB_NODE,i);

		/*check if this route rule is exist*/
		ret = tcapi_get( nodeName, "DST_IP", dst_ip);
		if ( ret != 0 ) 
		{
			continue;
		}
		
		sprintf( nodeIndex, "%d", i+1);
		ForwardingIndex[0].name = nodeIndex;
	
		/*register this node*/
		if ( RegisterNodeFromTable( deviceNode, ForwardingIndex ) == NULL )
		{
			rtnCode = -1;		//registerNode fail.
			return rtnCode;
		}
		j++;			
	}

	return rtnCode;
}

int8 addForwardingObject(uint32* num, DeviceNode*  deviceNode)
{
	uint32 number;
	DeviceNodePtr forwardingNode = NULL;
	char *objectName = "InternetGatewayDevice.Layer3Forwarding.Forwarding.";
	int rtnCode = 0;
	
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &forwardingNode);
	if(forwardingNode == NULL){
		return -1;
	}
	
	if ((number = checkFreeRoute_new()) < 0) {
		tc_printf("can't find free route for use!");
		return -1;
	}
	else{
		InitMyForwardingTable(number);
		cwmpFreeDeviceTable(forwardingNode);
		rtnCode = cwmpInitForwardingTableFromFlash(forwardingNode);

		*num = number + 1;
		if(dbg_cwmp >= DBG_CWMP_L2 ){
			tc_printf("\r\ncwmpParameter.c---[addForwardingObject]---*num = ", *num);
		}
	}
	
	return rtnCode;
}

void deleteMyForwardingTable(uint32 number)
{
	char cfgnode[30] = {0};
	char webId[8] = {0},nodeId[8] = {0};
	sprintf(cfgnode,"%s_%s%d",ROUTE_NODE,ROUTE_SUB_NODE,number);
	sprintf(nodeId,"%d",number);
	tcapi_get("WebCurSet_Entry","route_id",webId);
	tcapi_set("WebCurSet_Entry","route_id",nodeId);
	tcapi_unset(cfgnode);
	tcapi_commit(ROUTE_NODE);
	tcapi_set("WebCurSet_Entry","route_id",webId);
	return;
}

int8 deleteForwardingObject(uint32 num, DeviceNode*  deviceNode){
	DeviceNodePtr forwardingNode = NULL;
	char *objectName = "InternetGatewayDevice.Layer3Forwarding.Forwarding.";
	int rtnCode = 0;
	
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &forwardingNode);
	if(forwardingNode == NULL){
		return -1;
	}
	
	deleteMyForwardingTable(num - 1);//InstanceNumber - 1

	cwmpFreeDeviceTable(forwardingNode);
	rtnCode = cwmpInitForwardingTableFromFlash(forwardingNode);

	return rtnCode;

}

int8 getDefaultConnectionServiceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char buf[10] = {0};
	char connType[24] = {0};
	int nIspNode = -1;
	uint32 pvcIndex = 0;
	uint32 ifIndex = 0;
	char nodeName[32] = {0};
	char L3DefaultConnectionService[128] = {0};

	nIspNode = getISPIndex();
	if(nIspNode == -1){	  
		goto result;
	}
	tc_printf("\nDefault Router pvc is %d\n", nIspNode);
	/*Get pvc and interface index*/
	pvcIndex = nIspNode / MAX_SMUX_NUM;
	ifIndex = nIspNode % MAX_SMUX_NUM;

	memset(buf, 0, sizeof(buf));
	sprintf(nodeName, "Wan_PVC%d_Entry%d", pvcIndex, ifIndex);
	tcapi_get(nodeName,"ISP",buf);
	switch(atoi(buf))
	{
		case 0://dynamic ip address
		case 1://static ip address
			strcpy(connType,"WANIPConnection");
			break;
		case 2://pppoe
			strcpy(connType,"WANPPPConnection");
			break;		
		default:
			goto result;		
	}
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add		
	GetWanNodeMapping(&pvcIndex, &ifIndex);
	sprintf(L3DefaultConnectionService,"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.%s.%d",pvcIndex,connType, ifIndex);
#else
	sprintf(L3DefaultConnectionService,"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.%s.%d",pvcIndex+1,connType, ifIndex+1);
#endif
result:
	return getTheValue(value, type, deviceNode, L3DefaultConnectionService);
	
}

int8 setDefaultConnectionServiceValue(void* value, struct DeviceNode*  deviceNode)
{
	char conntype[256]={'\0'};
	DeviceNode* defConNode = NULL;
	uint32 pvcnode = 0;
	int errorCode = 0;
	uint32 entry_num = 0;
	char *setvalue = NULL;
	char nodeName[32] = {0};
	char buf[16] = {0};
	
	if(strlen((char*)value) > 255)
		return -1;
	
	cwmpSearchDeviceNodeByName((char*)value, rpc.IGDDeviceRootPtr, &defConNode);
	if(defConNode == NULL){
		return -1;
	}
	
	setvalue=(char*)value;
	sscanf(setvalue, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.%[^123456789]%d", 
		&pvcnode,conntype, &entry_num);

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	FindWanNodeMapping(&pvcnode, &entry_num);
#else
	pvcnode -= 1;
	entry_num -= 1;
#endif
	
	if(pvcnode < 0 || (pvcnode > MAX_SMUX_NUM-1) || entry_num < 0 || (entry_num > MAX_SMUX_NUM-1)){
		tc_printf("Invalid pvc number  or entry number\n ");
		return -1;
	}
	else 
	{

		sprintf(nodeName, "Wan_PVC%d_Entry%d", pvcnode, entry_num);
		memset(buf, 0, sizeof(buf));
		tcapi_get(nodeName, "ServiceList", buf);
		if(!strcmp(buf, TR069)){
			tc_printf("Only tr069 can't set route\n");
			return -1;
		}
		memset(buf, 0, sizeof(buf));
		tcapi_get(nodeName, "WanMode", buf);
		if(!strncmp(buf, "Route", 5)){
			tcapi_set(nodeName, "DEFAULTROUTE", "Yes");
			return 0;
		}
		
	}

	return -1;
	
}

int8 getDefaultConnectionServiceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclayer3ForwardingAttr.aDefaultConnectionService;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDefaultConnectionServiceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	
	attribute = &tclayer3ForwardingAttr.aDefaultConnectionService;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LAYER3FORWARDING_ATTR_FLAG;
	return result;
}

int8 getForwardNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char temp[4];
	tcapi_get(ROUTE_NODE,"User_def_num",temp);
	
	return getTheValue(value, type, deviceNode, temp);
}

int8 getForwardNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclayer3ForwardingAttr.aForwardNumberOfEntries;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setForwardNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	
	attribute = &tclayer3ForwardingAttr.aForwardNumberOfEntries;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LAYER3FORWARDING_ATTR_FLAG;
	return result;
}

int8 getForwarding1EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char  enableValue[10] = {0};
	char nodeName[32] = {0};
	int entry = -1;
	
	entry = getL3ForwardingIndex(deviceNode) -1;
	sprintf(nodeName,"%s_%s%d",ROUTE_NODE,ROUTE_SUB_NODE,entry);
	tcapi_get(nodeName,"Active",tempValue);
	
	if(strcmp(tempValue,"Yes") == 0)
		sprintf(enableValue,"%d",1);
	else
		sprintf(enableValue,"%d",0);

	return getTheValue(value, type, deviceNode, enableValue);
}

int8 setForwarding1EnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8* tempvalue = NULL;
	int entry = -1;
	char nodeName[32] = {0};
	Route_Info_List * temp = NULL;
	
	tempvalue = (uint8*)value;
	
	entry = getL3ForwardingIndex(deviceNode) -1;
	
	SetCfgNodeFlag( "Route", entry);
	
 	if(*tempvalue != 0 && *tempvalue != 1 && entry >= STATIC_ROUTE_NUM) 
	{
		tc_printf("\r\nthe value is not valid!");
		return -1;
 	}

#if 0
	sprintf(nodeName,"%s_%s%d",ROUTE_NODE,ROUTE_SUB_NODE,entry);
 	if(*tempvalue == 0)
	{
		tcapi_set(nodeName,"Active","No");
	}
	else
	{
		tcapi_set(nodeName,"Active","Yes");
	}
#endif
	temp = AddRoute(entry+1);
	if(temp == NULL)
	{
		tc_printf("\r\nadd route fail(internal error)!");
		return -1;
	}

	if(*tempvalue == 0)
	{
		strcpy(temp->flag,"No");
	}
	else
	{
		strcpy(temp->flag,"Yes");
	}
	
	return 0;
}

int8 getForwarding1EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclayer3ForwardingAttr.aForwarding1Enable[getL3ForwardingIndex(deviceNode)-1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setForwarding1EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	
	attribute = &tclayer3ForwardingAttr.aForwarding1Enable[getL3ForwardingIndex(deviceNode)-1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LAYER3FORWARDING_ATTR_FLAG;
	return result;
}

int8 getForwarding1StatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char  statusValue[10] = {0};
	char nodeName[32] = {0};
	int entry = -1;
	
	entry = getL3ForwardingIndex(deviceNode) -1;
	sprintf(nodeName,"%s_%s%d",ROUTE_NODE,ROUTE_SUB_NODE,entry);
	tcapi_get(nodeName,"Active",tempValue);

	if(strcmp(tempValue,"Yes") == 0)
		strcpy(statusValue,"Enabled");
	else if(strcmp(tempValue,"No") == 0)
		strcpy(statusValue,"Disabled");
	else
		sprintf(statusValue,"Error");

	return getTheValue(value, type, deviceNode, statusValue);
}

int8 getForwarding1StatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclayer3ForwardingAttr.aForwarding1Status[getL3ForwardingIndex(deviceNode) - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8   setForwarding1StatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	
	attribute = &tclayer3ForwardingAttr.aForwarding1Status[getL3ForwardingIndex(deviceNode) - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LAYER3FORWARDING_ATTR_FLAG;
	return result;
}

int8 getForwarding1TypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempDst_ip[20] = {0};
	char tempnetmask[20] = {0};
	int entry =-1;
	char  Forwarding1Type[8] = {0};	
	char nodeName[32] = {0};
	
	entry = getL3ForwardingIndex(deviceNode) -1;
	sprintf(nodeName,"%s_%s%d",ROUTE_NODE,ROUTE_SUB_NODE,entry);
	tcapi_get(nodeName,"DST_IP",tempDst_ip);
	tcapi_get(nodeName,"Sub_mask",tempnetmask);

	if (strcmp(tempDst_ip,"0.0.0.0") == 0)
		strcpy (Forwarding1Type, "Default");
	else if(strcmp(tempnetmask,"255.255.255.255") == 0)
		strcpy (Forwarding1Type, "Host");
	else
		strcpy (Forwarding1Type, "Network");

	return getTheValue(value, type, deviceNode, Forwarding1Type);
}

int8 setForwarding1TypeValue(void* value, struct DeviceNode*  deviceNode)
{
	//not allow to set the node because its value could confict with DestIPAddress and/or DestSubnetMask
	return 0;
}

int8 getForwarding1TypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclayer3ForwardingAttr.aForwarding1Type[getL3ForwardingIndex(deviceNode) - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setForwarding1TypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	
	attribute = &tclayer3ForwardingAttr.aForwarding1Type[getL3ForwardingIndex(deviceNode) - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LAYER3FORWARDING_ATTR_FLAG;
	return result;
}

int8 getForwarding1DestIPAddrValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int entry = -1;
	char  l3fDestIpAddr[32] = {'\0'};
	char nodeName[32] = {0};
	entry = getL3ForwardingIndex(deviceNode) -1;
	sprintf(nodeName,"%s_%s%d",ROUTE_NODE,ROUTE_SUB_NODE,entry);
	tcapi_get(nodeName,"DST_IP",l3fDestIpAddr);
	return getTheValue(value, type, deviceNode, l3fDestIpAddr);
}

int8 setForwarding1DestIPAddrValue(void* value, struct DeviceNode*  deviceNode)
{
	char *tempStr = (char*) value;
	int entry = -1;
	#if 0
	char nodeName[32] = {0};
	char  l3fDestIpAddr[32] = {'\0'};
	#endif
	Route_Info_List * temp = NULL;
	
	entry = getL3ForwardingIndex(deviceNode) -1;
	SetCfgNodeFlag( "Route", entry);
	
	//ip is "0.0.0.0" is not valid
	if(strcmp((char*)value, "0.0.0.0") == 0)
	{
		return -1;
	}
	
	if( InternalClientIpCheck((char*)value) != 1 && tempStr[0] != 0)
	{
		tc_printf("\r\nIllegal dest ip");
		return -1;
	}

	/*store the new parameters into the buf -------add by brian
		1.because route mechanism need to do "tcapi_unset()" to delete the old route first,and then can do "tcapi_commit()" to add new route.so when we do commit in function 'cwmpSetParameterValuesResponse()',we need to know the old value and the new value.
		2.not set the new value into "Route_Entry" node directly.The node involved "DST_IP","Gateway","Sub_mask","Device","Metric"
		3.if modify the whole route setting mechanism(whether delete or add a route,should reoperate the whole route table) ,here maybe no useful.
		*/
	temp = AddRoute(entry+1);
	if(temp == NULL)
	{
		tc_printf("\r\nadd route fail(internal error)!");
		return -1;
	}

	strcpy(temp->destip,(char*)value);

	#if 0
	sprintf(nodeName,"%s_%s%d",ROUTE_NODE,ROUTE_SUB_NODE,entry);
	strcpy(l3fDestIpAddr,(char*)value);
	tcapi_set(nodeName,"DST_IP",l3fDestIpAddr);
	#endif
	return 0;
}

int8 getForwarding1DestIPAddrAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclayer3ForwardingAttr.aForwarding1DestIPAddress[getL3ForwardingIndex(deviceNode) - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setForwarding1DestIPAddrAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	
	attribute = &tclayer3ForwardingAttr.aForwarding1DestIPAddress[getL3ForwardingIndex(deviceNode) - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LAYER3FORWARDING_ATTR_FLAG;
	return result;
}

int8 getForwarding1DestSubMaskValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int entry = -1;

	char  l3fDestSubMask[32] = {'\0'};
	char nodeName[32] = {0};
	
	entry = getL3ForwardingIndex(deviceNode) -1;
	sprintf(nodeName,"%s_%s%d",ROUTE_NODE,ROUTE_SUB_NODE,entry);
	tcapi_get(nodeName,"Sub_mask",l3fDestSubMask);
	return getTheValue(value, type, deviceNode, l3fDestSubMask);
}

int8 setForwarding1DestSubMaskValue(void* value, struct DeviceNode*  deviceNode)
{
	char *tempStr = (char*) value;
	int entry = -1;
	#if 0
	char nodeName[32] = {0};
	char  l3fDestSubMask[32] = {'\0'};
	#endif
	Route_Info_List * temp = NULL;
	
	entry = getL3ForwardingIndex(deviceNode) -1;
	SetCfgNodeFlag( "Route", entry);
	
	//mask is "0.0.0.0" is not valid
	if(strcmp((char*)value, "0.0.0.0") == 0)
	{
		return -1;
	}
	
	if(SubnetLegalCheck((char*)value) != 1 && tempStr[0] != 0)
	{
		tc_printf("\r\nIllegal submask");
		return -1;
	}
	
	temp = AddRoute(entry+1);
	if(temp == NULL)
	{
		tc_printf("\r\nadd route fail(internal error)!");
		return -1;
	}

	strcpy(temp->submask,(char*)value);
	#if 0
	sprintf(nodeName,"%s_%s%d",ROUTE_NODE,ROUTE_SUB_NODE,entry);
	strcpy(l3fDestSubMask,(char*)value);
	tcapi_set(nodeName,"Sub_mask",l3fDestSubMask);
	#endif
	return 0;
}

int8 getForwarding1DestSubMaskAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclayer3ForwardingAttr.aForwarding1SubnetMask[getL3ForwardingIndex(deviceNode) - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setForwarding1DestSubMaskAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	
	attribute = &tclayer3ForwardingAttr.aForwarding1SubnetMask[getL3ForwardingIndex(deviceNode) - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LAYER3FORWARDING_ATTR_FLAG;
	return result;
}

static char ForwardingSrcIp[16][16];
static char ForwardingSrcSubnet[16][16];
int8 getForwarding1SrcIPAddrValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char SrcIPAddr[24] = {0};
	strcpy(SrcIPAddr , ForwardingSrcIp[getL3ForwardingIndex(deviceNode)-1]);

	return getTheValue(value, type, deviceNode, SrcIPAddr);
}

int8 setForwarding1SrcIPAddrValue(void* value, struct DeviceNode*  deviceNode)
{
	int entry = -1;
	char *strIp = NULL;
	
	strIp = (char*)value;
	if( InternalClientIpCheck(strIp) != 1 && strIp[0] != 0)
	{
		tc_printf("\r\nIllegal src ip");
		return -1;
	}
	
	strcpy(ForwardingSrcIp[getL3ForwardingIndex(deviceNode)-1], strIp);

	return 0;
}

int8 getForwarding1SrcIPAddrAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclayer3ForwardingAttr.aForwarding1SourceIPAddress[getL3ForwardingIndex(deviceNode)-1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setForwarding1SrcIPAddrAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	
	attribute = &tclayer3ForwardingAttr.aForwarding1SourceIPAddress[getL3ForwardingIndex(deviceNode) - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LAYER3FORWARDING_ATTR_FLAG;
	return result;
}


int8 getForwarding1SrcSubMaskValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char SrcSubMask[24] = {0};
	strcpy(SrcSubMask , ForwardingSrcIp[getL3ForwardingIndex(deviceNode)-1]);

	return getTheValue(value, type, deviceNode, SrcSubMask);
}

int8 setForwarding1SrcSubMaskValue(void* value, struct DeviceNode*  deviceNode)
{
	int entry = -1;
	char *mask = NULL;
	
	mask = (char*)value;
	if( SubnetLegalCheck(mask) != 1 && mask[0] != 0)
	{
		tc_printf("\r\nIllegal src mask");
		return -1;
	}
	
	strcpy(ForwardingSrcSubnet[getL3ForwardingIndex(deviceNode)-1], mask);

	return 0;
}

int8 getForwarding1SrcSubMaskAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclayer3ForwardingAttr.aForwarding1SourceSubnetMask[getL3ForwardingIndex(deviceNode) - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setForwarding1SrcSubMaskAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	
	attribute = &tclayer3ForwardingAttr.aForwarding1SourceSubnetMask[getL3ForwardingIndex(deviceNode) - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LAYER3FORWARDING_ATTR_FLAG;
	return result;
}

int8 getForwarding1GtwIPAddrValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int entry = -1;
	char nodeName[32] = {0};
	char l3fGtwIP[20] = {0};
	
	entry = getL3ForwardingIndex(deviceNode) -1;
	sprintf(nodeName,"%s_%s%d",ROUTE_NODE,ROUTE_SUB_NODE,entry);
	
	tcapi_get(nodeName,"Gateway",l3fGtwIP);
	return getTheValue(value, type, deviceNode, l3fGtwIP);
}

int8 setForwarding1GtwIPAddrValue(void* value, struct DeviceNode*  deviceNode)
{
	int entry = -1;
	char *tempStr = (char*) value;
	#if 0
	char nodeName[32] = {0};
	char l3fGtwIP[20] = {0};
	#endif
	Route_Info_List * temp = NULL;
	
	entry = getL3ForwardingIndex(deviceNode) -1;
	SetCfgNodeFlag( "Route", entry);
	
	if( InternalClientIpCheck((char*)value) != 1)
	{
		tc_printf("\r\nIllegal gateway ip");
		return -1;
	}
	
	temp = AddRoute(entry+1);
	if(temp == NULL)
	{
		tc_printf("\r\nadd route fail(internal error)!");
		return -1;
	}

	strcpy(temp->gateway,(char*)value);

	#if 0
	sprintf(nodeName,"%s_%s%d",ROUTE_NODE,ROUTE_SUB_NODE,entry);
	strcpy(l3fGtwIP,(char*)value);	
	tcapi_set(nodeName,"Gateway",l3fGtwIP);
	#endif
	return 0;
	
}

int8 getForwarding1GtwIPAddrAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclayer3ForwardingAttr.aForwarding1GatewayIPAddress[getL3ForwardingIndex(deviceNode) - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setForwarding1GtwIPAddrAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	
	attribute = &tclayer3ForwardingAttr.aForwarding1GatewayIPAddress[getL3ForwardingIndex(deviceNode) - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LAYER3FORWARDING_ATTR_FLAG;
	return result;
}

int8 getForwarding1IfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int i = 0;
	int entry = -1;
	char l3fGateway[20] = {0};
	char iprouteIface[128] = {0};
	char l3fDevice[20] = {0};
	char l3fTemp[20] = {0};
	char nodeName[32] = {0};
	char buf[10] = {0};
#if 1   //ROUTE_INTERFACE
	int ifIndex = -1;
	int j = 0;
	uint32 entrynode = 0;
#endif
	int findpvc = 0,findlan = 0;
	uint32 pvcnode = 0;
	char *p = NULL;
	
	entry = getL3ForwardingIndex(deviceNode) -1;
	sprintf(nodeName,"%s_%s%d",ROUTE_NODE,ROUTE_SUB_NODE,entry);	
	tcapi_get(nodeName,"Device",l3fDevice);
	
	if(strlen(l3fDevice) != 0)
	{
			#if 0
			if(strcmp(l3fDevice,"Lan0") == 0)
			{
				findlan = 1;
			}
			else
			#endif
			{
				p = l3fDevice;
#if 1   //ROUTE_INTERFACE
				if(strstr(l3fDevice, "ppp")){  /*ppp interface*/
					if(get_entry_number(l3fDevice,"ppp",&ifIndex) == 0 ){
						pvcnode = ifIndex/MAX_SMUX_NUM;        
						entrynode = ifIndex - pvcnode*MAX_SMUX_NUM;
					}		
				}
				else if(strstr(l3fDevice, "nas")){   /*nas interface,ext:nas0_1*/	
					pvcnode = *(l3fDevice+3) - '0';
					entrynode = *(l3fDevice+5) - '0';
				}
				
#else
				pvcnode = atoi(p+strlen(l3fDevice)-1);
#endif
				findpvc = 1;
			}
	}
	else
	{
		tcapi_get(nodeName,"Gateway",l3fGateway);
		if(strlen(l3fGateway) != 0)
		{
			#if 0
			tcapi_get("Lan_Entry0","IP",l3fTemp);
			if(strcmp(l3fGateway,l3fTemp) == 0)
			{
				findlan = 1;
			}
			else
			#endif
			{
				
#if 1   //ROUTE_INTERFACE
				for(i = 0; i < MAX_SMUX_NUM; i++){  //MAX_WAN_IF_INDEX
					for(j = 0; j < MAX_PVC_NUM; j++){  
						memset(l3fTemp,0,sizeof(l3fTemp));
						sprintf(nodeName,"Wan_PVC%d_Entry%d",i,j);	
						tcapi_get(nodeName,"Gateway",l3fTemp);
						if(strcmp(l3fGateway,l3fTemp) == 0)
						{	
							findpvc = 1;
							pvcnode = i;
							entrynode = j;
							break;
						}		
					}
				}
#else
				for(i = 0; i < 8; i++)
				{
					memset(l3fTemp,0,sizeof(l3fTemp));
					sprintf(nodeName,"DeviceInfo_PVC%d",i);	
					tcapi_get(nodeName,"gateway",l3fTemp);
					if(strcmp(l3fGateway,l3fTemp) == 0)
					{	
						findpvc = 1;
						pvcnode = i;
						break;
					}		
				}
#endif
			}
		}
	}
	
	if(findpvc == 1)
	{
		memset(buf,0,sizeof(buf));
		memset(l3fTemp,0,sizeof(l3fTemp));
#if 1   //ROUTE_INTERFACE
		sprintf(l3fTemp,"Wan_PVC%d_Entry%d",pvcnode,entrynode);
		tcapi_get(l3fTemp,"ISP",buf);
#else
		sprintf(l3fTemp,"/etc/isp%d.conf",pvcnode);
		get_profile_str("ISP=",buf,sizeof(buf),1,l3fTemp);
#endif
		memset(l3fTemp,0,sizeof(l3fTemp));
		switch(atoi(buf))
		{
			case 0://dynamic ip address
			case 1://static ip address
				strcpy(l3fTemp,"WANIPConnection");
				break;
			case 2://pppoe
				strcpy(l3fTemp,"WANPPPConnection");
				break;
			default:
				tc_printf("\r\nerror type!use ip connection as defalut");
				strcpy(l3fTemp,"WANIPConnection");
				break;		
		}
#if 1   //ROUTE_INTERFACE
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add		
		GetWanNodeMapping(&pvcnode, &entrynode);
		sprintf(iprouteIface ,"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.%s.%d" , pvcnode , l3fTemp, entrynode);
#else
		sprintf(iprouteIface ,"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.%s.%d" , pvcnode+1 , l3fTemp, entrynode+1);
#endif
#else
		sprintf(iprouteIface ,"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.%s.1" , pvcnode+1 , l3fTemp);
#endif
	}
	#if 0
	else if(findlan == 1)
	{
		strcpy(iprouteIface ,"InternetGatewayDevice.LANDevice.");
	}
	#endif
	else
	{
		strcpy(iprouteIface,"none");
	}

	return getTheValue(value, type, deviceNode, iprouteIface);
	
}

int8 setForwarding1IfaceValue(void* value, struct DeviceNode*  deviceNode)
{
	int i,len,entry = -1;
	uint32 pvcnode;
	char *setvalue = NULL;
	char tempvalue[2] = {0};
	char nodeName[32] = {0};
	char tempflag[20] = {0};
	char DefaultConSer_WAN[]="InternetGatewayDevice.WANDevice.1.WANConnectionDevice.";
	//char DefaultConSer_LAN[]="InternetGatewayDevice.LANDevice.";
	char l3fDevice[10] = {0};
	Route_Info_List * temp = NULL;
#if 1   //ROUTE_INTERFACE
	char isp[4] = {0},active[4] = {0};
	uint32 entryNode = 0;
	int pppIndex;
	char *tmp = NULL;
#endif
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	uint32 pvctmp;
#endif
	len = strlen(DefaultConSer_WAN);
	setvalue = (char*)value;
	
	entry = getL3ForwardingIndex(deviceNode) -1;
	SetCfgNodeFlag( "Route", entry);
	
	if(strncmp(DefaultConSer_WAN, setvalue, len) == 0)
	{
		tempvalue[0] = setvalue[len];
		tempvalue[1] = '\0';
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
		pvcnode = atoi(tempvalue);
		pvctmp = pvcnode;
		if(FindPVCMapping(&pvcnode) != 0){
			tc_printf("Invalid pvc number!");
			return -1;
		}
#else
		if(setvalue[len] < '1'||setvalue[len] > '8')
		{
			tc_printf("Invalid pvc number!");
			return -1;
		}
		pvcnode = atoi(tempvalue) - 1;

#endif

#if 1   //ROUTE_INTERFACE
		if((tmp = strstr(setvalue, "WANPPPConnection")) != NULL){
			memset(tempvalue, 0, sizeof(tempvalue));
			tempvalue[0] = *(tmp + strlen("WANPPPConnection") + 1);
			tempvalue[1] = '\0';
			entryNode = atoi(tempvalue) - 1;
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
			entryNode = atoi(tempvalue);
			FindWanNodeMapping(&pvctmp, &entryNode);
#else
			entryNode = atoi(tempvalue) - 1;
#endif
			sprintf(nodeName,"Wan_PVC%d_Entry%d", pvcnode, entryNode);
			tcapi_get(nodeName,"Active",active);
			if(!strcmp(active,"Yes"))
			{	
				tcapi_get(nodeName,"ISP",isp);
				if(atoi(isp) != 2){//pppoe dial up
					tc_printf("\r\nthe Interface is active,but it is not PPP device!");
					return -1;
				}
			}
			else{
				tc_printf("\r\nthe Interface is not active,so not allow to set this interface!");
				return -1;
			}
			pppIndex = pvcnode * 8 + entryNode;
			sprintf(l3fDevice,"ppp%d", pppIndex);
		}
		else if((tmp = strstr(setvalue, "WANIPConnection")) != NULL){
			memset(tempvalue, 0, sizeof(tempvalue));
			tempvalue[0] = *(tmp + strlen("WANIPConnection") + 1);
			tempvalue[1] = '\0';
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
			entryNode = atoi(tempvalue);
			FindWanNodeMapping(&pvctmp, &entryNode);
#else
			entryNode = atoi(tempvalue) - 1;
#endif
			sprintf(nodeName,"Wan_PVC%d_Entry%d", pvcnode, entryNode);
			tcapi_get(nodeName,"Active",active);
			if(!strcmp(active,"Yes"))
			{	
				tcapi_get(nodeName,"ISP",isp);
				if(atoi(isp) != 0 && atoi(isp) != 1){
					tc_printf("\r\nthe Interface is active,but it is not IP device!");
					return -1;
				}
			}
			else{
				tc_printf("\r\nthe Interface is not active,so not allow to set this interface!");
				return -1;
			}
			sprintf(l3fDevice,"nas%d_%d",pvcnode,entryNode);
		}
#else
		sprintf(nodeName, "Wan_PVC%d", pvcnode);
		tcapi_get(nodeName,"Active",tempflag);
		if(strlen(tempflag) == 0 || strcmp(tempflag,"Yes") != 0)
		{
			tc_printf("\r\nthe pvc is not active,so not allow to set this interface!");
			return -1;
		}
#endif		
		temp = AddRoute(entry+1);
		if(temp == NULL)
		{
			tc_printf("\r\nadd route fail(internal error)!");
			return -1;
		}
#if 0   //ROUTE_INTERFACE
		sprintf(l3fDevice,"PVC%d",pvcnode);
#endif
		strcpy(temp->interface,l3fDevice);

		#if 0
		sprintf(nodeName,"%s_%s%d",ROUTE_NODE,ROUTE_SUB_NODE,entry);
		sprintf(l3fDevice,"PVC%d",pvcnode);
		tcapi_set(nodeName,"Device",l3fDevice);		
		#endif
	}
	else
	{
		return -1;
		#if 0
		//gateway = lan ip 
		len = strlen(DefaultConSer_LAN);
		if(strncmp(DefaultConSer_LAN, setvalue, len) != 0)
		{
			tc_printf("\r\nthe parameter is error!");
			return -1;
		}
		
		sprintf(nodeName,"%s_%s%d",ROUTE_NODE,ROUTE_SUB_NODE,entry);
		strcpy(l3fDevice,"Lan0");
		tcapi_get(l3fDevice,"IP",tempflag);
		tcapi_set(nodeName,"Device",l3fDevice);
		tcapi_set(nodeName,"Gateway",tempflag);
		#endif
	}

	return 0;
}

int8 getForwarding1IfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclayer3ForwardingAttr.aForwarding1Interface[getL3ForwardingIndex(deviceNode) - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setForwarding1IfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	
	attribute = &tclayer3ForwardingAttr.aForwarding1Interface[getL3ForwardingIndex(deviceNode) - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LAYER3FORWARDING_ATTR_FLAG;
	return result;
}

int8 getForwarding1FwMetricValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int entry = -1;
	char nodeName[32] = {0};
	char fwMetric[4] = {0};
	entry = getL3ForwardingIndex(deviceNode) -1;
	sprintf(nodeName,"%s_%s%d",ROUTE_NODE,ROUTE_SUB_NODE,entry);
	tcapi_get(nodeName,"metric",fwMetric);

	return getTheValue(value, type, deviceNode, fwMetric);
}

int8 setForwarding1FwMetricValue(void* value, struct DeviceNode*  deviceNode)
{
	int entry = -1;
	char nodeName[32] = {0};
	char fwMetric[4] = {0};
	uint32 metricvalue = 0;
	Route_Info_List * temp = NULL;
	
	entry = getL3ForwardingIndex(deviceNode) -1;
	
	SetCfgNodeFlag( "Route", entry);
	
	metricvalue =  *((uint32*)value);
	if( metricvalue < 1 || metricvalue > 15 ){
		tc_printf("Illegal FwMetric\n");
		return -1;
	}

	temp = AddRoute(entry+1);
	if(temp == NULL)
	{
		tc_printf("\r\nadd route fail(internal error)!");
		return -1;
	}
	
	sprintf(fwMetric,"%d",metricvalue);
	strcpy(temp->metric,fwMetric);
	
	#if 0
	sprintf(nodeName,"%s_%s%d",ROUTE_NODE,ROUTE_SUB_NODE,entry);
	sprintf(fwMetric,"%d",metricvalue);
	tcapi_set(nodeName,"metric",fwMetric);
	#endif
	return 0;	
}

int8 getForwarding1FwMetricAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclayer3ForwardingAttr.aForwarding1ForwardingMetric[getL3ForwardingIndex(deviceNode) - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setForwarding1FwMetricAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	
	attribute = &tclayer3ForwardingAttr.aForwarding1ForwardingMetric[getL3ForwardingIndex(deviceNode) - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LAYER3FORWARDING_ATTR_FLAG;
	return result;
}

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getForwarding1StaticRouteValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char staticRT[4] = "1";

	return getTheValue(value, type, deviceNode, staticRT);
}
int8 getForwarding1StaticRouteAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tclayer3ForwardingAttr.aForwarding1ForwardingStaticRoute[getL3ForwardingIndex(deviceNode) - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setForwarding1StaticRouteAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	
	attribute = &tclayer3ForwardingAttr.aForwarding1ForwardingStaticRoute[getL3ForwardingIndex(deviceNode) - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LAYER3FORWARDING_ATTR_FLAG;
	return result;
}
#endif


#if defined(TCSUPPORT_CMCC)
#ifdef TCSUPPORT_IPV6
int8 getForwardingEnable6Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	char enableValue[10] = {0};
	char nodeName[32] = {0};
	int entry = 0;

	entry = getL3ForwardingIndex(deviceNode) - 1;
	sprintf(nodeName, "%s_%s%d", ROUTE6_NODE, ROUTE_SUB_NODE, entry);
	tcapi_get(nodeName, "Active", tempValue);

	if ( 0 == strcmp(tempValue, "Yes") )
		strcpy(enableValue, "1");
	else
		strcpy(enableValue, "0");

	return getTheValue(value, type, deviceNode, enableValue);
}
int8 setForwardingEnable6Value(void* value, struct DeviceNode*  deviceNode)
{
	uint8 i_enable = *((uint8*)value);
	char enableValue[10] = {0};
	char nodeName[32] = {0};
	int entry = 0;

	entry = getL3ForwardingIndex(deviceNode) -1;
	if( 1 == i_enable )
		strcpy(enableValue, "Yes");
	else if( 0 == i_enable )
		strcpy(enableValue, "No");
	else
		return -1;
	sprintf(nodeName, "%s_%s%d", ROUTE6_NODE, ROUTE_SUB_NODE, entry);
	tcapi_set(nodeName, "Active", enableValue);

	SetCfgNodeFlag(ROUTE6_NODE, entry);
	return 0;
}
int8 getForwardingEnable6Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int entry = 0;

	entry = getL3ForwardingIndex(deviceNode) - 1;
	attribute = tclayer3ForwardingAttr.aIPv6ForwardingEnable[entry];
	result = GetTheAttributes(attribute, notification, acl, deviceNode);
	return result;
}
int8 setForwardingEnable6Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int entry = 0;

	entry = getL3ForwardingIndex(deviceNode) - 1;
	attribute = &tclayer3ForwardingAttr.aIPv6ForwardingEnable[entry];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LAYER3FORWARDING_ATTR_FLAG;
	return result;
}

int8 getForwardingAliasValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char aliasValue[100] = {0};
	char nodeName[32] = {0};
	int entry = 0;

	entry = getL3ForwardingIndex(deviceNode) - 1;
	sprintf(nodeName, "%s_%s%d", ROUTE6_NODE, ROUTE_SUB_NODE, entry);
	if ( 0 != tcapi_get(nodeName, "Alias", aliasValue) )
		aliasValue[0] = 0;

	return getTheValue(value, type, deviceNode, aliasValue);
}
int8 setForwardingAliasValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	char aliasValue[100] = {0};
	int entry = 0;
	char *palias = (char*)value;

	entry = getL3ForwardingIndex(deviceNode) -1;
	if ( NULL == palias
		|| strlen(palias) >= 65 )
		return -1;
	strcpy(aliasValue, palias);

	sprintf(nodeName, "%s_%s%d", ROUTE6_NODE, ROUTE_SUB_NODE, entry);
	tcapi_set(nodeName, "Alias", aliasValue);

	SetCfgNodeFlag(ROUTE6_NODE, entry);
	return 0;
}
int8 getForwardingAliasAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int entry = 0;

	entry = getL3ForwardingIndex(deviceNode) - 1;
	attribute = tclayer3ForwardingAttr.aIPv6ForwardingAlias[entry];
	result = GetTheAttributes(attribute, notification, acl, deviceNode);
	return result;
}
int8 setForwardingAliasAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int entry = 0;

	entry = getL3ForwardingIndex(deviceNode) - 1;
	attribute = &tclayer3ForwardingAttr.aIPv6ForwardingAlias[entry];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LAYER3FORWARDING_ATTR_FLAG;
	return result;
}

int8 getForwardingDestIPPrefixValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char destIPPrefixValue[100] = {0};
	char nodeName[32] = {0}, destip[64] = {0}, prefixlen[32] = {0};
	int entry = 0;

	entry = getL3ForwardingIndex(deviceNode) - 1;
	sprintf(nodeName, "%s_%s%d", ROUTE6_NODE, ROUTE_SUB_NODE, entry);
	
	if ( 0 != tcapi_get(nodeName, "DST_IP", destip) )
		destip[0] = 0;
	if ( 0 != tcapi_get(nodeName, "Prefix_len", prefixlen) )
		prefixlen[0] = 0;
	if ( 0 == destip[0] )
		destIPPrefixValue[0] = 0;
	else
		snprintf(destIPPrefixValue, sizeof(destIPPrefixValue) - 1,
				"%s/%s", destip, prefixlen);

	return getTheValue(value, type, deviceNode, destIPPrefixValue);
}
int8 setForwardingDestIPPrefixValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	int entry = 0, i_prefixlen = 0;
	char destIP[100] = {0};
	char prefixlen[16] = {0};
	struct in6_addr in_v6addr = {0};

	char *pdest = (char*)value;
	entry = getL3ForwardingIndex(deviceNode) -1;
	if ( NULL == pdest
		|| strlen(pdest) >= sizeof(destIP) )
		return -1;
	sscanf(pdest, "%[^/]/%d", destIP, &i_prefixlen);
	if ( 1 != inet_pton(AF_INET6, destIP, &in_v6addr) )
	{
		tcdbg_printf("\nerr: invalid dest ip [%s]\n", destIP);
		return -1;
	}

	if ( !((i_prefixlen >= 1 && i_prefixlen <= 128)
		|| (0 == i_prefixlen && 0 == strcmp("::", destIP))) )
	{
		tcdbg_printf("\nerr: invalid prefix len [%d]\n", i_prefixlen);
		return -1;
	}
	sprintf(prefixlen, "%d", i_prefixlen);

	sprintf(nodeName, "%s_%s%d", ROUTE6_NODE, ROUTE_SUB_NODE, entry);
	tcapi_set(nodeName, "DST_IP", destIP);
	tcapi_set(nodeName, "Prefix_len", prefixlen);

	SetCfgNodeFlag(ROUTE6_NODE, entry);
	return 0;
}
int8 getForwardingDestIPPrefixAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int entry = 0;

	entry = getL3ForwardingIndex(deviceNode) - 1;
	attribute = tclayer3ForwardingAttr.aIPv6ForwardingDestIPPrefix[entry];
	result = GetTheAttributes(attribute, notification, acl, deviceNode);
	return result;
}
int8 setForwardingDestIPPrefixAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int entry = 0;

	entry = getL3ForwardingIndex(deviceNode) - 1;
	attribute = &tclayer3ForwardingAttr.aIPv6ForwardingDestIPPrefix[entry];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LAYER3FORWARDING_ATTR_FLAG;
	return result;
}

int8 getForwardingNextHopValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nextHopxValue[80] = {0};
	char nodeName[32] = {0};
	int entry = 0;

	entry = getL3ForwardingIndex(deviceNode) - 1;
	sprintf(nodeName, "%s_%s%d", ROUTE6_NODE, ROUTE_SUB_NODE, entry);
	
	if ( 0 != tcapi_get(nodeName, "Gateway", nextHopxValue) )
		nextHopxValue[0] = 0;

	return getTheValue(value, type, deviceNode, nextHopxValue);
}
int8 setForwardingNextHopValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[32] = {0};
	int entry = 0;
	char nextHopxValue[80] = {0};
	struct in6_addr in_v6addr = {0};

	char *pnexthop = (char*)value;
	entry = getL3ForwardingIndex(deviceNode) -1;
	if ( NULL == pnexthop
		|| strlen(pnexthop) >= sizeof(nextHopxValue) )
		return -1;
	strcpy(nextHopxValue, pnexthop);
	if ( 1 != inet_pton(AF_INET6, nextHopxValue, &in_v6addr) )
	{
		tcdbg_printf("\nerr: invalid nexthop [%s]\n", nextHopxValue);
		return -1;
	}

	sprintf(nodeName, "%s_%s%d", ROUTE6_NODE, ROUTE_SUB_NODE, entry);
	tcapi_set(nodeName, "Gateway", nextHopxValue);

	SetCfgNodeFlag(ROUTE6_NODE, entry);
	return 0;
}
int8 getForwardingNextHopAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int entry = 0;

	entry = getL3ForwardingIndex(deviceNode) - 1;
	attribute = tclayer3ForwardingAttr.aIPv6ForwardingNextHop[entry];
	result = GetTheAttributes(attribute, notification, acl, deviceNode);
	return result;
}
int8 setForwardingNextHopAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int entry = 0;

	entry = getL3ForwardingIndex(deviceNode) - 1;
	attribute = &tclayer3ForwardingAttr.aIPv6ForwardingNextHop[entry];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LAYER3FORWARDING_ATTR_FLAG;
	return result;
}

int8 getForwardingInterface6Value(char** value,
									ParameterType* type,
									DeviceNode* deviceNode)
{
	int i = 0;
	int entry = 0;
	char iprouteIface[256] = {0};
	char l3fDevice[20] = {0};
	char l3fTemp[20] = {0};
	char nodeName[32] = {0};
	char buf[10] = {0};
	int ifIndex = 0;
	int j = 0;
	uint32 entrynode = 0;
	int findpvc = 0, findlan = 0;
	uint32 pvcnode = 0;
	char *p = NULL;
	
	entry = getL3ForwardingIndex(deviceNode) - 1;
	sprintf(nodeName,"%s_%s%d",ROUTE6_NODE, ROUTE_SUB_NODE, entry);	
	tcapi_get(nodeName,"Device", l3fDevice);
	
	if ( 0 != l3fDevice[0] )
	{
		p = l3fDevice;
		if ( strstr(l3fDevice, "ppp") )
		{ /* ppp interface */
			if ( get_entry_number(l3fDevice, "ppp", &ifIndex) == 0 )
			{
				pvcnode = ifIndex / MAX_SMUX_NUM;
				entrynode = ifIndex % MAX_SMUX_NUM;
			}
		}
		else if( strstr(l3fDevice, "nas") )
		{ /* nas interface,ext:nas0_1 */
			pvcnode = *(l3fDevice+3) - '0';
			entrynode = *(l3fDevice+5) - '0';
		}

		findpvc = 1;
	}

	
	if ( 1 == findpvc )
	{
		memset(buf, 0, sizeof(buf));
		memset(l3fTemp, 0, sizeof(l3fTemp));
		sprintf(l3fTemp, "Wan_PVC%d_Entry%d", pvcnode, entrynode);
		tcapi_get(l3fTemp, "ISP", buf);
		memset(l3fTemp, 0, sizeof(l3fTemp));
		switch ( atoi(buf) )
		{
			case 0: /* dynamic ip address */
			case 1: /* static ip address */
				strcpy(l3fTemp, "WANIPConnection");
				break;
			case 2: /* pppoe */
				strcpy(l3fTemp, "WANPPPConnection");
				break;
			default:
				tc_printf("\r\nerror type!use ip connection as defalut");
				strcpy(l3fTemp, "WANIPConnection");
				break;		
		}

#ifdef TCSUPPORT_CT_WANNODE_MAPPING
		/* tony add */
		GetWanNodeMapping(&pvcnode, &entrynode);
		sprintf(iprouteIface, "InternetGatewayDevice.WANDevice.1."
								"WANConnectionDevice.%d.%s.%d"
								, pvcnode , l3fTemp, entrynode);
#else
		sprintf(iprouteIface, "InternetGatewayDevice.WANDevice.1."
								"WANConnectionDevice.%d.%s.%d"
								, pvcnode+1 , l3fTemp, entrynode+1);
#endif
	}
	else
	{
		iprouteIface[0] = 0;
	}

	return getTheValue(value, type, deviceNode, iprouteIface);
}
int8 setForwardingInterface6Value(void* value, struct DeviceNode*  deviceNode)
{
	int i = 0, len = 0, entry = 0;
	uint32 pvcnode = 0;
	char *setvalue = NULL;
	char tempvalue[2] = {0};
	char nodeName[32] = {0};
	char tempflag[20] = {0};
	char DefaultConSer_WAN[]="InternetGatewayDevice.WANDevice.1."
								"WANConnectionDevice.";
	char l3fDevice[20] = {0};
	Route_Info_List * temp = NULL;
	char isp[4] = {0}, active[4] = {0};
	uint32 entryNode = 0;
	int pppIndex= 0;
	char *tmp = NULL;
#ifdef TCSUPPORT_CT_WANNODE_MAPPING
	/* tony add */
	uint32 pvctmp = 0;
#endif
	len = strlen(DefaultConSer_WAN);
	setvalue = (char*)value;
	
	entry = getL3ForwardingIndex(deviceNode) -1;

	if ( 0 == strncmp(DefaultConSer_WAN, setvalue, len) )
	{
		tempvalue[0] = setvalue[len];
		tempvalue[1] = '\0';
#ifdef TCSUPPORT_CT_WANNODE_MAPPING
		/* tony add */
		pvcnode = atoi(tempvalue);
		pvctmp = pvcnode;
		if ( FindPVCMapping(&pvcnode) != 0 )
		{
			tc_printf("Invalid pvc number!");
			return -1;
		}
#else
		if ( setvalue[len] < '1'||setvalue[len] > '8' )
		{
			tc_printf("Invalid pvc number!");
			return -1;
		}
		pvcnode = atoi(tempvalue) - 1;

#endif
		if( (tmp = strstr(setvalue, "WANPPPConnection")) != NULL )
		{
			memset(tempvalue, 0, sizeof(tempvalue));
			tempvalue[0] = *(tmp + strlen("WANPPPConnection") + 1);
			tempvalue[1] = '\0';
			entryNode = atoi(tempvalue) - 1;
#ifdef TCSUPPORT_CT_WANNODE_MAPPING
			/* tony add */
			entryNode = atoi(tempvalue);
			FindWanNodeMapping(&pvctmp, &entryNode);
#else
			entryNode = atoi(tempvalue) - 1;
#endif
			sprintf(nodeName,"Wan_PVC%d_Entry%d", pvcnode, entryNode);
			tcapi_get(nodeName, "Active", active);
			if ( 0 == strcmp(active, "Yes") )
			{
				tcapi_get(nodeName, "ISP", isp);
				if( atoi(isp) != 2 )
				{ 
					/* pppoe dial up */
					tc_printf("\r\nthe Interface is active,"
								"but it is not PPP device!");
					return -1;
				}
			}
			else
			{
				tc_printf("\r\nthe Interface is not active,"
						"so not allow to set this interface!");
				return -1;
			}
			pppIndex = pvcnode * 8 + entryNode;
			sprintf(l3fDevice,"ppp%d", pppIndex);
		} 
		else if( (tmp = strstr(setvalue, "WANIPConnection")) != NULL )
		{
			memset(tempvalue, 0, sizeof(tempvalue));
			tempvalue[0] = *(tmp + strlen("WANIPConnection") + 1);
			tempvalue[1] = '\0';
#ifdef TCSUPPORT_CT_WANNODE_MAPPING
			/* tony add */
			entryNode = atoi(tempvalue);
			FindWanNodeMapping(&pvctmp, &entryNode);
#else
			entryNode = atoi(tempvalue) - 1;
#endif
			sprintf(nodeName, "Wan_PVC%d_Entry%d", pvcnode, entryNode);
			tcapi_get(nodeName, "Active", active);
			if ( 0 == strcmp(active, "Yes") )
			{	
				tcapi_get(nodeName, "ISP", isp);
				if ( atoi(isp) != 0 && atoi(isp) != 1 )
				{
					tc_printf("\r\nthe Interface is active,"
								"but it is not IP device!");
					return -1;
				}
			}
			else
			{
				tc_printf("\r\nthe Interface is not active,"
						"so not allow to set this interface!");
				return -1;
			}
			sprintf(l3fDevice, "nas%d_%d", pvcnode, entryNode);
		}
	}
	else
	{
		return -1;
	}

	sprintf(nodeName, "%s_%s%d", ROUTE6_NODE, ROUTE_SUB_NODE, entry);
	tcapi_set(nodeName, "Device", l3fDevice);
	SetCfgNodeFlag(ROUTE6_NODE, entry);

	return 0;
}
int8 getForwardingInterface6Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	int entry = 0;

	entry = getL3ForwardingIndex(deviceNode) - 1;
	attribute = tclayer3ForwardingAttr.aIPv6ForwardingInterface[entry];
	result = GetTheAttributes(attribute, notification, acl, deviceNode);
	return result;
}
int8 setForwardingInterface6Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	int entry = 0;

	entry = getL3ForwardingIndex(deviceNode) - 1;
	attribute = &tclayer3ForwardingAttr.aIPv6ForwardingInterface[entry];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LAYER3FORWARDING_ATTR_FLAG;
	return result;
}

int8 getIPv6ForwardNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char forwardNum[16] = {0};

	if ( 0 != tcapi_get(ROUTE6_NODE, "Route_num", forwardNum) )
		strcpy(forwardNum, "0");

	return getTheValue(value, type, deviceNode, forwardNum);
}
int8 getIPv6ForwardNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tclayer3ForwardingAttr.aIPv6ForwardNumberOfEntries;
	result = GetTheAttributes(attribute, notification, acl, deviceNode);
	return result;
}
int8 setIPv6ForwardNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tclayer3ForwardingAttr.aIPv6ForwardNumberOfEntries;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= LAYER3FORWARDING_ATTR_FLAG;
	return result;
}

int checkFreeIPv6Route_new()
{
	char temp[12] = {0};
	int addnum = 0;

	tcapi_get(ROUTE6_NODE, "add_num", temp);
	addnum = atoi(temp);
	if ( addnum >= MAX_STATIC_ROUTE6_NUM )
	{
		tc_printf("\r\n[cwmp]the max static route6 number is %d",
										MAX_STATIC_ROUTE6_NUM);
		return -1;
	}

	/* we need to return the index of new routing item-----addnum is from 0~15 */
	return addnum;
}
void InitIPv6ForwardingTable(uint32 number)
{
	char nodeName[32] = {0};

	sprintf(nodeName, "%s_%s%d", ROUTE6_NODE, ROUTE_SUB_NODE, number);
	tcapi_set(nodeName, "Active", "Yes"); /* default will be active. */
	tcapi_set(nodeName, "Alias", "");

	return;
}
int8 deleteIPv6ForwardingObject(uint32 num, DeviceNode*  deviceNode)
{
	DeviceNodePtr forwardingNode = NULL;
	char *objectName = "InternetGatewayDevice."X_CMCC
						"IPv6Layer3Forwarding.Forwarding.";
	int rtnCode = 0;
	char nodeName[32] = {0};

	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr,
								&forwardingNode);
	if ( NULL == forwardingNode )
		return -1;
	sprintf(nodeName, "%s_%s%d", ROUTE6_NODE, ROUTE_SUB_NODE, num - 1);
	tcapi_unset(nodeName);

	cwmpFreeDeviceTable(forwardingNode);
	rtnCode = cwmpInitIPv6ForwardingTableFromFlash(forwardingNode);

	return rtnCode;
}
int8 addIPv6ForwardingObject(uint32* num, DeviceNode*  deviceNode)
{
	uint32 number = 0;
	DeviceNodePtr forwardingNode = NULL;
	char *objectName = "InternetGatewayDevice."X_CMCC
						"IPv6Layer3Forwarding.Forwarding.";
	int rtnCode = 0;

	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr,
								&forwardingNode);
	if ( NULL == forwardingNode )
		return -1;

	if ( ( number = checkFreeIPv6Route_new() ) < 0 )
	{
		tc_printf("can't find free route6 for use!");
		return -1;
	}
	else
	{
		InitIPv6ForwardingTable(number);
		cwmpFreeDeviceTable(forwardingNode);
		rtnCode = cwmpInitIPv6ForwardingTableFromFlash(forwardingNode);

		*num = number + 1;
		if ( dbg_cwmp >= DBG_CWMP_L2 )
		{
			tc_printf("\r\ncwmpParameter.c---[addIPv6ForwardingObject]"
						"---*num = ", *num);
		}
	}

	return rtnCode;
}
int	cwmpInitIPv6ForwardingTableFromFlash(DeviceNodePtr deviceNode)
{
	int totalnum = 0, route_num = 0;
	char temp[4] = {0};
	char nodeIndex[4] = {0};
	char nodeName[32] = {0};
	char active[12] = {0};
	int i = 0, j = 0, rtnCode = 0;
	int index = 0;
	int ret = 0;

	if ( NULL == deviceNode )
		return -1;

	tcapi_get(ROUTE6_NODE, "Route_num", temp);
	route_num = atoi(temp);
	/* we only fetch user define route */
	for ( i = 0; i < MAX_STATIC_ROUTE6_NUM && route_num >= j; i++ )
	{
		memset(temp,0,sizeof(temp));
		sprintf(nodeName,"%s_%s%d", ROUTE6_NODE, ROUTE_SUB_NODE, i);
		/* check if this route rule is exist */
		ret = tcapi_get(nodeName, "Active", active);
		if ( ret != 0 ) 
			continue;

		sprintf( nodeIndex, "%d", i + 1 );
		IPv6ForwardingIndex[0].name = nodeIndex;
		/* register this node */
		if ( RegisterNodeFromTable( deviceNode, IPv6ForwardingIndex ) == NULL )
		{
			rtnCode = -1; /* registerNode fail. */
			return rtnCode;
		}
		j++;			
	}

	return rtnCode;
}

#endif
#endif

