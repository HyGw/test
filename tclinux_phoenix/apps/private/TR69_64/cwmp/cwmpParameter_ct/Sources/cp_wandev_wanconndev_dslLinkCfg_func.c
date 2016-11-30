#include "Global_res.h"
#include "cp_wandev_wanconndev_dslLinkCfg.h"

#if !defined(TCSUPPORT_CT_PON)

uint8 PPPTEST_VPI = 1;	/* default ppptest vpi */  
uint16 PPPTEST_VCI = 39;	/* default ppptest vci */

#if defined(TCSUPPORT_CT_WAN_PTM)
extern int getModulationType(char *retType, int bufsize);
#endif

/*rpc of parameters:InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANDSLLinkConfig.*/
int8 getWANDSLLinkCfg1EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char dslCfgEnable[10] = {'\0'};
	uint32 wanNodeIndex;
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&wanNodeIndex);
#else	
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name) - 1;
#endif
	sprintf(nodeName, "Wan_PVC%d", wanNodeIndex);
	if ( 0 != tcapi_get(nodeName, "DSLLinkEnable", dslCfgEnable) )
		strcpy(dslCfgEnable, "Yes");

	if(!strcmp(dslCfgEnable, "Yes")){
		strcpy(dslCfgEnable, "1");
	}
	else{
		strcpy(dslCfgEnable, "0");
	}
	
	return getTheValue(value, type, deviceNode, dslCfgEnable);
}
int8 setWANDSLLinkCfg1EnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char enable[10] = {'\0'};
	uint8 ena;
	uint32 wanNodeIndex;
	int ret;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&wanNodeIndex);
#else	
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name) - 1;
#endif
	sprintf(nodeName, "Wan_PVC%d", wanNodeIndex);

	ena = *((uint8*)value);
	if(ena == 1){
		strcpy(enable, "Yes");
	}
	else if(ena == 0){
		strcpy(enable, "No");
	}
	else{
		tc_printf("==>setWANDSLLinkCfg1EnableValue: Enable value must be 0 or 1.\n");
		return -1;
	}

	tcapi_set(nodeName, "DSLLinkEnable", enable);
	//SetCfgNodeFlag( "Wan", wanNodeIndex );
	
	return 0;
}

int8 getWANDSLLinkCfg1EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDslLinkAttr.aWANDSLLinkCfg1Enable[getWANLinkIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANDSLLinkCfg1EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;
	
	attribute = &tcDslLinkAttr.aWANDSLLinkCfg1Enable[getWANLinkIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DSLLINK_ATTR_FLAG;
	return result;
}

int8 getWANDSLLinkCfg1LinkStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
#if  1
	char adslStatus[20];

	memset(adslStatus, 0, sizeof(adslStatus));

	tcapi_get("Info_Adsl", "lineState", adslStatus);
	if(!strcmp(adslStatus, "up")){
		strcpy(adslStatus, "Up");
	}
	else if(!strcmp(adslStatus, "down")){
		strcpy(adslStatus, "Down");
	}
	else if(!strcmp(adslStatus, "initializing")){
		strcpy(adslStatus, "Initializing");
	}

	return getTheValue(value, type, deviceNode, adslStatus);
	
#else
	char adslStatus[20] = {"Unavailable"};

	tcapi_get("Info_Adsl", "lineState", adslStatus);
	
	if(!strcmp(adslStatus, "up")){
		strcpy(adslStatus, "Up");
	}
	else if(!strcmp(adslStatus, "down")){
		strcpy(adslStatus, "Down");
	}
	else if(!strcmp(adslStatus, "initializing")){
		strcpy(adslStatus, "Initializing");
	}

	return getTheValue(value, type, deviceNode, adslStatus);
#endif
}
int8 getWANDSLLinkCfg1LinkStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDslLinkAttr.aWANDSLLinkCfg1LinkStatus[getWANLinkIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANDSLLinkCfg1LinkStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDslLinkAttr.aWANDSLLinkCfg1LinkStatus[getWANLinkIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DSLLINK_ATTR_FLAG;
	return result;
}
int8 getWANDSLLinkCfg1LinkTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
#if   1
	char WANIPDSLLinkCfg1_1Type[32] = {"EoA"};	
	return getTheValue(value, type, deviceNode, WANIPDSLLinkCfg1_1Type);
#else
	char nodeName[34] = {'\0'};
	char encap[64] = {'\0'};
	char WANIPDSLLinkCfg1_1Type[32] = {"Unconfigured"};
	int wannodeindex;
	
	wannodeindex=atoi(deviceNode->parentNode->parentNode->name) - 1;
	sprintf(nodeName, "Wan_PVC%d", wannodeindex);
	tcapi_get(nodeName, "ENCAP", encap);

	if(strcmp(encap, "PPPoA LLC") == 0 //ppoa mode
	|| strcmp(encap, "PPPoA VC-Mux") == 0){
		strcpy(WANIPDSLLinkCfg1_1Type,"PPPoA");
	}
	else if(strcmp(encap, "1483 Routed IP LLC(IPoA)") == 0 //dynamic/static mode
		 || strcmp(encap, "1483 Routed IP VC-Mux") == 0){
		strcpy(WANIPDSLLinkCfg1_1Type,"IPoA");
	}
	else if(strcmp(encap, "1483 Bridged IP LLC") == 0	//dynamic/static mode
		 || strcmp(encap, "1483 Bridged IP VC-Mux") == 0
		 || strcmp(encap, "PPPoE LLC") == 0          	//pppoe mode
		 || strcmp(encap, "PPPoE VC-Mux") == 0
		 || strcmp(encap, "1483 Bridged Only LLC") == 0 //pure bridge
		 || strcmp(encap, "1483 Bridged Only VC-Mux") == 0){
		strcpy(WANIPDSLLinkCfg1_1Type,"EoA");
	}
	
	return getTheValue(value, type, deviceNode, WANIPDSLLinkCfg1_1Type);
#endif
}
int8 setWANDSLLinkCfg1LinkTypeValue(void* value, struct DeviceNode*  deviceNode)
{
//tcapi_set(nodeName, "ENCAP", "1483 Bridged Only LLC");
	if( 0 != strcmp((char*)value, "EoA") )
		return -1;
#if 0
	char nodeName[34] = {'\0'};
	char vtype[64] = {'\0'};
	char isp[8] = {'\0'};
	char active[10] = {'\0'};
	int wanNodeIndex;
	int ret = 0;
	DeviceNodePtr WANCntDeviceNode = NULL;
	DeviceNodePtr WANConnectionNode = NULL;
	char *objectName = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.";
	char cntName[128] = {'\0'};

	strcpy(vtype,(char*)value);		
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name) - 1;

	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &WANCntDeviceNode);
	if(WANCntDeviceNode == NULL){
		return -1;
	}
	
	sprintf(nodeName, "Wan_PVC%d", wanNodeIndex);
	
	ret = tcapi_get(nodeName, "Active", active);
	if(ret){//this pvc is not exist
		return -1;
	}
	
	SetCfgNodeFlag( "Wan", wanNodeIndex );	

	tcapi_get(nodeName, "ISP", isp);

	if(strcmp(vtype, "IPoA") == 0){
		tcapi_set(nodeName, "ISP", WAN_ENCAP_DYN);
		tcapi_set(nodeName, "ENCAP", "1483 Routed IP LLC(IPoA)");
		if(ACSaddingPVCFlag & (1<<wanNodeIndex))
			ACSaddingPVCFlag &=  ~(1<<wanNodeIndex);
	}
	else if(strcmp(vtype, "PPPoA") == 0){
		tcapi_set(nodeName, "ISP", WAN_ENCAP_PPP);
		tcapi_set(nodeName, "ENCAP", "PPPoA LLC");
		tcapi_set(nodeName, "PPPGETIP", "Dynamic");
		if(ACSaddingPVCFlag & (1<<wanNodeIndex))
			ACSaddingPVCFlag &=  ~(1<<wanNodeIndex);
	}
	else if(strcmp(vtype, "EoA") == 0){//need modify later??
		if(ACSaddingPVCFlag & (1<<wanNodeIndex)){ /* ACS is adding PVC now */
			;
		}
		else{
			if(strcmp(isp, WAN_ENCAP_BRIDGE) != 0){
				sprintf(cntName,"%s%d.%s.",objectName,wanNodeIndex+1,"WANIPConnection");
				cwmpSearchDeviceNodeByName(cntName, rpc.IGDDeviceRootPtr, &WANConnectionNode);
				if(WANConnectionNode != NULL){  //dynamic mode
					tcapi_set(nodeName, "ISP", WAN_ENCAP_DYN);
					tcapi_set(nodeName, "ENCAP", "1483 Routed IP LLC(IPoA)");
				}
				else{//pppoe mode
					tcapi_set(nodeName, "ISP", WAN_ENCAP_PPP);
					tcapi_set(nodeName, "ENCAP", "PPPoE LLC");
					tcapi_set(nodeName, "PPPGETIP", "Dynamic");
				}
			}
			else{//pure bridge
				tcapi_set(nodeName, "ENCAP", "1483 Bridged Only LLC");
			}
		}
	}

	else {
		return -1;
	}
	
	cwmpFreeDeviceTable(WANCntDeviceNode);	
	ret = cwmpInitWANConnectionDeviceTable(WANCntDeviceNode);	
	
	return ret;
#endif
	return 0;
}

int8 getWANDSLLinkCfg1LinkTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDslLinkAttr.aWANDSLLinkCfg1LinkType[getWANLinkIndex(deviceNode)];;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANDSLLinkCfg1LinkTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDslLinkAttr.aWANDSLLinkCfg1LinkType[getWANLinkIndex(deviceNode)];;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DSLLINK_ATTR_FLAG;
	return result;
}
int8 getWANDSLLinkCfg1AutoConfigValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char active[10] = {'\0'};
	char  WANDSLLinkCfg1_1Status[4] = {'\0'};
	char vpi[10] = {'\0'};
	char vci[10] = {'\0'};
	uint32 wanNodeIndex;
	int vpiValue,vciValue;
	int i;

#define MAX_ENTRY_NODE 8

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&wanNodeIndex);
#else	
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name) - 1;
#endif
	sprintf(nodeName, "Wan_PVC%d", wanNodeIndex);
	tcapi_get(nodeName, "VPI", vpi);
	vpiValue = atoi(vpi);
	tcapi_get(nodeName, "VCI", vci);
	vciValue = atoi(vci);

	for(i = 0; i <MAX_ENTRY_NODE; i++)
	{
		tcapi_get(nodeName, "Active", active);
		if(strcmp(active, "Yes"))
			break;
	}
	if(i == MAX_ENTRY_NODE){ //this pvc is not exist.
		return -1;
	}
	
	
	if( vpiValue == PPPTEST_VPI && vciValue == PPPTEST_VCI){
		strcpy(WANDSLLinkCfg1_1Status, "1");
	}
	else{
		strcpy(WANDSLLinkCfg1_1Status, "0");
	}

	return getTheValue(value, type, deviceNode, WANDSLLinkCfg1_1Status);
}
int8 getWANDSLLinkCfg1AutoConfigAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDslLinkAttr.aWANDSLLinkCfg1AutoConfig[getWANLinkIndex(deviceNode)];;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANDSLLinkCfg1AutoConfigAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDslLinkAttr.aWANDSLLinkCfg1AutoConfig[getWANLinkIndex(deviceNode)];;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DSLLINK_ATTR_FLAG;
	return result;
}
int8 getWANDSLLinkCfg1ModulationTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char WANIPDSLLinkCfg1_1Type[32] = {'\0'};
	char modulationType[32] = {'\0'};
#if defined(TCSUPPORT_CT_WAN_PTM)
	char s_dsltype[32] = {0};
	int dslType = 0;
#endif

	tcapi_get("Adsl_Entry", "MODULATIONTYPE", modulationType);
	
	if(!strcmp(modulationType, "T1.413")){
		strcpy(WANIPDSLLinkCfg1_1Type, "ADSL_ANSI_T1.413");
	}
	else if(!strcmp(modulationType, "G.DMT")){
		strcpy(WANIPDSLLinkCfg1_1Type, "ADSL_G.dmt");
	}
	else if(!strcmp(modulationType, "G.lite")){
		strcpy(WANIPDSLLinkCfg1_1Type, "ADSL_G.lite");
	}
	else if(!strcmp(modulationType, "ADSL2")){
		strcpy(WANIPDSLLinkCfg1_1Type, "ADSL_G.dmt.bis");
	}
	else{
		strcpy(WANIPDSLLinkCfg1_1Type, "ADSL_2plus");
	}
#if defined(TCSUPPORT_CT_WAN_PTM)
	dslType = getModulationType(s_dsltype, sizeof(s_dsltype));
	if ( 2 == dslType )
		strcpy(WANIPDSLLinkCfg1_1Type, s_dsltype);
#endif

	return getTheValue(value, type, deviceNode, WANIPDSLLinkCfg1_1Type);
}
int8 getWANDSLLinkCfg1ModulationTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDslLinkAttr.aWANDSLLinkCfg1ModType[getWANLinkIndex(deviceNode)];;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANDSLLinkCfg1ModulationTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDslLinkAttr.aWANDSLLinkCfg1ModType[getWANLinkIndex(deviceNode)];;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DSLLINK_ATTR_FLAG;
	return result;
}
int8 getWANDSLLinkCfg1DestinationAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char vpi[10] = {"0"};
	char vci[10] = {"0"};
	char WANIPDSLLinkCfg1_1Type[20] = {'\0'};
	uint32 wanNodeIndex;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&wanNodeIndex);
#else	
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name) - 1;
#endif
	sprintf(nodeName, "Wan_PVC%d", wanNodeIndex);

	tcapi_get(nodeName, "VPI", vpi);
	tcapi_get(nodeName, "VCI", vci);

	sprintf(WANIPDSLLinkCfg1_1Type, "PVC:%s/%s",vpi, vci);
	
	return getTheValue(value, type, deviceNode, WANIPDSLLinkCfg1_1Type);
}
int8 setWANDSLLinkCfg1DestinationAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char active[10] = {'\0'};
	char vtype[32], vpistr[8],vcistr[16];
	uint32 wanNodeIndex;
	int ret;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&wanNodeIndex);
#else	
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name) - 1;
#endif

	sprintf(nodeName, "Wan_PVC%d", wanNodeIndex);
	tcdbg_printf("nodeName:%s\n", nodeName);

	ret = tcapi_get(nodeName, "VPI", active);
	if(ret){//this pvc is not exist.
		tcdbg_printf("This PVC does not exist!\n");
		return -1;
	}
	if(strlen((char*)value) > 31){
		tcdbg_printf("This value is longer than 31!\n");
		return -1;
	}

	memset(vtype, 0, sizeof(vtype));
	memset(vpistr, 0, sizeof(vpistr));
	memset(vcistr, 0, sizeof(vcistr));
	strcpy(vtype,(char*)value);		
	ret = cwmpGetVpiVciStr(vtype, vpistr, vcistr);
	tcdbg_printf("\r\nGet vpi:%s, vci:%s fails", vpistr, vcistr);
	
	if(ret == -1){
		tcdbg_printf("\r\nGet vpi vci fails");
		return -1;
	}
	else if(ret == -2){
		tcdbg_printf("\r\nWithout vpi/vci for setting!");
		return -1;
	}

	if(CheckVPIVCI(wanNodeIndex,vpistr,vcistr) == -1){
		tcdbg_printf("vpi/vci collision.\n"); //
		return -1;
	}
	tcapi_set(nodeName, "VPI", vpistr);
	tcapi_set(nodeName, "VCI", vcistr);
	SetCfgNodeFlag( "Wan", wanNodeIndex );
	return 0;	
}

int8 getWANDSLLinkCfg1DestinationAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDslLinkAttr.aWANDSLLinkCfg1DestAddr[getWANLinkIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANDSLLinkCfg1DestinationAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDslLinkAttr.aWANDSLLinkCfg1DestAddr[getWANLinkIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DSLLINK_ATTR_FLAG;
	return result;
}
int8 getWANDSLLinkCfg1ATMEncapsulationValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char encap[128] = {'\0'};
	char WANIPDSLLinkCfg1_1Type[20] = {'\0'};
	uint32 wanNodeIndex;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&wanNodeIndex);
#else	
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name) - 1;
#endif
	sprintf(nodeName, "Wan_PVC%d", wanNodeIndex);

	tcapi_get(nodeName, "ENCAP", encap);
	if(strcmp(encap, "1483 Bridged IP LLC") == 0 
		|| strcmp(encap, "1483 Routed IP LLC(IPoA)") == 0 
		|| strcmp(encap, "PPPoE LLC") == 0 
		|| strcmp(encap, "PPPoA LLC") == 0
		|| strcmp(encap, "1483 Bridged Only LLC") == 0){
		strcpy(WANIPDSLLinkCfg1_1Type, "LLC");
	}
	else if(strcmp(encap, "1483 Bridged IP VC-Mux") == 0 
		|| strcmp(encap, "1483 Routed IP VC-Mux") == 0 
		|| strcmp(encap, "PPPoE VC-Mux") == 0 
		|| strcmp(encap, "PPPoA VC-Mux") == 0
		|| strcmp(encap, "1483 Bridged Only VC-Mux") == 0){
		strcpy(WANIPDSLLinkCfg1_1Type, "VCMUX");
	}
	else{
		strcpy(WANIPDSLLinkCfg1_1Type, " ");
	}
	
	return getTheValue(value, type, deviceNode, WANIPDSLLinkCfg1_1Type);
}
int8 setWANDSLLinkCfg1ATMEncapsulationValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char vtype[20] = {'\0'};
	char active[10] = {'\0'};
	char encap[128] = {'\0'};
	uint32 wanNodeIndex;
	int ret;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&wanNodeIndex);
#else	
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name) - 1;
#endif

	sprintf(nodeName, "Wan_PVC%d", wanNodeIndex);
	ret = tcapi_get(nodeName, "Active", active);
	if(ret){//this pvc is not exist.
		return  -1;
	}

	if(strlen((char*)value) > 19){
		return -1;
	}
	
	strcpy(vtype,(char*)value);		
	tcapi_get(nodeName, "ENCAP", encap);
	
	if(strcmp(vtype,"LLC") == 0){
		if(strcmp(encap, "1483 Bridged IP VC-Mux") == 0){
			tcapi_set(nodeName, "ENCAP", "1483 Bridged IP LLC");
		}
		else if(strcmp(encap, "1483 Routed IP VC-Mux") == 0){
			tcapi_set(nodeName, "ENCAP", "1483 Routed IP LLC(IPoA)");
		}
		else if(strcmp(encap, "PPPoE VC-Mux") == 0){
			tcapi_set(nodeName, "ENCAP", "PPPoE LLC");
		}
		else if(strcmp(encap, "PPPoA VC-Mux") == 0){
			tcapi_set(nodeName, "ENCAP", "PPPoA LLC");
		}
		else if(strcmp(encap, "1483 Bridged Only VC-Mux") == 0){
			tcapi_set(nodeName, "ENCAP", "1483 Bridged Only LLC");
		}
		else{ // it will not happened in general.
			return -1;
		}
	}
	else if(strcmp(vtype,"VCMUX") == 0){
		if(strcmp(encap, "1483 Bridged IP LLC") == 0){
			tcapi_set(nodeName, "ENCAP", "1483 Bridged IP VC-Mux");
		}
		else if(strcmp(encap, "1483 Routed IP LLC(IPoA)") == 0){
			tcapi_set(nodeName, "ENCAP", "1483 Routed IP VC-Mux");
		}
		else if(strcmp(encap, "PPPoE LLC") == 0){
			tcapi_set(nodeName, "ENCAP", "PPPoE VC-Mux");
		}
		else if(strcmp(encap, "PPPoA LLC") == 0){
			tcapi_set(nodeName, "ENCAP", "PPPoA VC-Mux");
		}
		else if(strcmp(encap, "1483 Bridged Only LLC") == 0){
			tcapi_set(nodeName, "ENCAP", "1483 Bridged Only VC-Mux");
		}
		else{ // it will not happened in general.
			return -1;
		}
	}
	else{
		tc_printf("muxtype must be LLC or VCMUX");
		return -1;
	}

	SetCfgNodeFlag( "Wan", wanNodeIndex );
	return 0;
}

int8 getWANDSLLinkCfg1ATMEncapsulationAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDslLinkAttr.aWANDSLLinkCfg1ATMEncap[getWANLinkIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANDSLLinkCfg1ATMEncapsulationAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDslLinkAttr.aWANDSLLinkCfg1ATMEncap[getWANLinkIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DSLLINK_ATTR_FLAG;
	return result;
}
static uint8 FCSPreserved[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int8 getWANDSLLinkCfg1FCSPreservedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  WANFCSPreserved[20];
	uint32 wanNodeIndex;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&wanNodeIndex);
#else	
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name)-1;
#endif
	sprintf(WANFCSPreserved, "%d", FCSPreserved[wanNodeIndex]);
	return getTheValue(value, type, deviceNode, WANFCSPreserved);
}
int8 setWANDSLLinkCfg1FCSPreservedValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 wanNodeIndex;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&wanNodeIndex);
#else	
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name) - 1;
#endif

	FCSPreserved[wanNodeIndex] = *((uint8*)value);

	return 0;
}
int8 getWANDSLLinkCfg1FCSPreservedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDslLinkAttr.aWANDSLLinkCfg1FCSPreserved[getWANLinkIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANDSLLinkCfg1FCSPreservedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDslLinkAttr.aWANDSLLinkCfg1FCSPreserved[getWANLinkIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DSLLINK_ATTR_FLAG;
	return result;
}
int8 getWANDSLLinkCfg1VCSearchListValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char WANIPDSLLinkCfg1_1VCSearchList[256] = "";
	char tmp[512] = {'\0'};	//merge from linos,but it seems too large.
	char vpi[10] = {'\0'};
	char vci[10] = {'\0'};
	uint8 vpiVal,vciVal;
	int len=0, i=0;
	int theFirstpvcFlag = 0;

	for (i = 0; i < MAX_VCPOOL_NUM; i++) {
		sprintf(nodeName, "AutoPVC_Entry%d", i);
		tcapi_get(nodeName, "VPI", vpi);
		tcapi_get(nodeName, "VCI", vci);
		vpiVal = atoi(vpi);
		vciVal = atoi(vci);
		if ( vci == 0 ) {
			continue;
		}

		if( theFirstpvcFlag == 0 ){
			sprintf(tmp, "%d/%d", vpiVal, vciVal);
			theFirstpvcFlag = 1;
		}
		else{
			sprintf(tmp, ",%d/%d", vpiVal, vciVal);

		}
		
		len += strlen(tmp);
		if(len > 256)
			break;
		strcat(WANIPDSLLinkCfg1_1VCSearchList, tmp);
	}
	return getTheValue(value, type, deviceNode, WANIPDSLLinkCfg1_1VCSearchList);
}
int8 setWANDSLLinkCfg1VCSearchListValue(void* value, struct DeviceNode*  deviceNode)
{
	char vtype[256], vpistr[8],vcistr[16],*tempstr;
	int ret = 0, offStart=0, offset=0, len=0, i=0, vpi,vci;
	char nodeName[34] = {'\0'};
	char wanvpi[10],wanvci[10];
	uint8 vpival,vcival;

	if((len = strlen((char*)value)) > 255){
		return -1;
	}

	strcpy(vtype, (char *)value);
	tempstr = &vtype[0];

	for(i = 0; i < MAX_VCPOOL_NUM; i++){
		sprintf(nodeName, "AutoPVC_Entry%d", i);
		tcapi_unset(nodeName);
	}
	
	while(len > 0){
		memset(vpistr, 0, sizeof(vpistr));
		memset(vcistr, 0, sizeof(vcistr));
		ret = cwmpGetVpiVciStrFromList(tempstr, vpistr, vcistr, &offset);
		if(ret == -1){
			tc_printf("\r\nGet vpi vci fails");
			return -1;
		}
		else if(ret == -2){
			tc_printf("\r\nWithout vpi/vci for setting!");
			return -1;
		}
	
		vpi = atoi(vpistr);
		vci = atoi(vcistr);

		if(vpi < 0 || vpi > 255 || vci<1 || vci>65535){

			return -1;
		}

		
		offStart += (offset + 1);
		tempstr = &vtype[offStart];
		len -= (offset + 1);

		/* Check the remoteNode conflict or not */
		if (!(GetVcPoolProbeAction() & 0x01)) {
			for (i = 0; i < MAX_WAN_NODE; i++) {
				sprintf(nodeName, "Wan_PVC%d", i);
				tcapi_get(nodeName, "VPI", wanvpi);
				tcapi_get(nodeName, "VCI", wanvci);
				vpival = atoi(wanvpi);
				vcival = atoi(wanvci);

				if(vpival == vpi && vcival == vci){
					tc_printf("Error: duplicate vpi and vci in RemoteNode %d\n", i + 1);
					return -1;
				}
			}
		}

		/* Check the VC pool conflict or not */
		for (i = 0; i < MAX_VCPOOL_NUM; i++) {
			sprintf(nodeName, "AutoPVC_Entry%d", i);
			memset(wanvpi, 0, sizeof(wanvpi));
			memset(wanvci, 0, sizeof(wanvci));
			
			tcapi_get(nodeName, "VPI", wanvpi);
			tcapi_get(nodeName, "VCI", wanvci);
			
			/*if tcapi_get fail, vpival and vcival will be 0, but vpi and vci will not be 0,so it 
			  will not happen duplicate vpi and vci in vc pool.
			*/
			vpival = atoi(wanvpi);
			vcival = atoi(wanvci);

			if(vpival == vpi && vcival == vci){
				tc_printf("Error: duplicate vpi and vci in VC pool\n");
				return -1;
			}
		}

		/* Find free entry in VC pool Table */
		for (i = 0; i < MAX_VCPOOL_NUM; i++){
			sprintf(nodeName, "AutoPVC_Entry%d", i);
			memset(wanvpi, 0, sizeof(wanvpi));
			memset(wanvci, 0, sizeof(wanvci));
			
			tcapi_get(nodeName, "VPI", wanvpi);
			tcapi_get(nodeName, "VCI", wanvci);
			vpival = atoi(wanvpi);
			vcival = atoi(wanvci);
			
		/*	if vpival and vcival equals 0, it meas tcapi_get fail,
		**	and this entry is free.
		*/
			if(vpival == 0 && vcival == 0)
				break;
		}
		
		if (i == MAX_VCPOOL_NUM) {
			tc_printf("Error: no free entry in VC pool\n");
			return -1;
		}

		sprintf(nodeName, "AutoPVC_Entry%d", i);
		tcapi_set(nodeName, "PVC", "0");
		tcapi_set(nodeName, "VPI", vpistr);
		tcapi_set(nodeName, "VCI", vcistr);
	}

	SetCfgNodeFlag( "AutoPVC", 0 );
	return 0;
}
int8 getWANDSLLinkCfg1VCSearchListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDslLinkAttr.aWANDSLLinkCfg1VCSearchList;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANDSLLinkCfg1VCSearchListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDslLinkAttr.aWANDSLLinkCfg1VCSearchList;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DSLLINK_ATTR_FLAG;
	return result;
}
int8 getWANDSLLinkCfg1ATMTransmittedBlocksValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char txpkt[32] = {"0"}; 	//init txpkt to 0.
	uint32 instance;
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	instance = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&instance);
#else	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
#endif

	sprintf(nodeName, "Info_nas%d", instance);
	if ( 0 != tcapi_get(nodeName, "txpackets", txpkt) )
		strcpy(txpkt, "0");

	return getTheValue(value, type, deviceNode, txpkt);
}
int8 getWANDSLLinkCfg1ATMTransmittedBlocksAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDslLinkAttr.aWANDSLLinkCfg1ATMTransmittedBlocks[getWANLinkIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANDSLLinkCfg1ATMTransmittedBlocksAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDslLinkAttr.aWANDSLLinkCfg1ATMTransmittedBlocks[getWANLinkIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DSLLINK_ATTR_FLAG;
	return result;
}
int8 getWANDSLLinkCfg1ATMReceivedBlocksValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char rxpkt[32] = {"0"}; 	//init rxpkt to 0.
	uint32 instance;
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	instance = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&instance);
#else	
	instance = atoi(deviceNode->parentNode->parentNode->name)-1;
#endif

	sprintf(nodeName, "Info_nas%d", instance);
	if ( 0 != tcapi_get(nodeName, "rxpackets", rxpkt) )
		strcpy(rxpkt, "0");

	return getTheValue(value, type, deviceNode, rxpkt);
}
int8 getWANDSLLinkCfg1ATMReceivedBlocksAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDslLinkAttr.aWANDSLLinkCfg1ATMReceivedBlocks[getWANLinkIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;	
}
int8 setWANDSLLinkCfg1ATMReceivedBlocksAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDslLinkAttr.aWANDSLLinkCfg1ATMReceivedBlocks[getWANLinkIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DSLLINK_ATTR_FLAG;
	return result;
}
int8 getWANDSLLinkCfg1ATMQoSValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char atmQoS[10] = {'\0'};
	uint32 wanNodeIndex;
	
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&wanNodeIndex);
#else	
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name) - 1;
#endif

	sprintf(nodeName, "Wan_PVC%d", wanNodeIndex);
	tcapi_get(nodeName, "QOS", atmQoS);
	
	if(!strcmp(atmQoS, "cbr")){
		strcpy(atmQoS, "CBR");
	}
	else if(!strcmp(atmQoS, "ubr")){
		strcpy(atmQoS, "UBR");
	}
	else if(!strcmp(atmQoS, "nrt-vbr")){
		strcpy(atmQoS, "VBR-nrt");
	}
	else if(!strcmp(atmQoS, "rt-vbr")){
		strcpy(atmQoS, "VBR-rt");
	}

	return getTheValue(value, type, deviceNode, atmQoS);
}
int8 setWANDSLLinkCfg1ATMQoSValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char active[10] = {'\0'};
	char vtype[20] = {'\0'};
	uint32 wanNodeIndex;
	int ret;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&wanNodeIndex);
#else	
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name) - 1;
#endif

	sprintf(nodeName, "Wan_PVC%d", wanNodeIndex);
	ret = tcapi_get(nodeName, "Active", active);
	if(ret){//this pvc is not exist.
		return -1;
	}
	

	if(strlen((char*)value) > 19){
		return -1;
	}
	
	strcpy(vtype,(char*)value);	
	
	if ( strcmp(vtype, "CBR") == 0 ){
		tcapi_set(nodeName, "QOS", "cbr");
	}
	else if ( strcmp(vtype, "UBR") == 0 ){
		tcapi_set(nodeName, "QOS", "ubr");
	}
	else if ( strcmp(vtype, "VBR-nrt") == 0 ){
		tcapi_set(nodeName, "QOS", "nrt-vbr");
	}
	else if ( strcmp(vtype, "VBR-rt") == 0 ){
		tcapi_set(nodeName, "QOS", "rt-vbr");
	}
	else{
		return -1;
	}
	SetCfgNodeFlag( "Wan", wanNodeIndex );	
	return 0;	
}

int8 getWANDSLLinkCfg1ATMQoSAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDslLinkAttr.aWANDSLLinkCfg1ATMQoS[getWANLinkIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANDSLLinkCfg1ATMQoSAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDslLinkAttr.aWANDSLLinkCfg1ATMQoS[getWANLinkIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DSLLINK_ATTR_FLAG;
	return result;
}
int8 getWANDSLLinkCfg1ATMPeakCellRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char tempValue[20] = {'\0'};
	uint32 wanNodeIndex;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&wanNodeIndex);
#else	
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name) - 1;
#endif
	sprintf(nodeName, "Wan_PVC%d", wanNodeIndex);

	if ( 0 != tcapi_get(nodeName, "PCR", tempValue) )
		strcpy(tempValue, "0");
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setWANDSLLinkCfg1ATMPeakCellRateValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {'\0'};
	char active[10] = {'\0'};
	char ATMPcrStr[10] = {'\0'};
	uint32 wanNodeIndex;
	uint32 ATMPcr;
	int ret;
	
	ATMPcr = *((uint32*)value);
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&wanNodeIndex);
#else	
	wanNodeIndex= atoi(deviceNode->parentNode->parentNode->name) - 1;
#endif
	sprintf(nodeName, "Wan_PVC%d", wanNodeIndex);

	ret = tcapi_get(nodeName, "Active", active);
	if(ret){//this pvc is not exist.
		return -1;
	}

	sprintf(ATMPcrStr, "%lu", ATMPcr);
	tcapi_set(nodeName, "PCR", ATMPcrStr);

	SetCfgNodeFlag( "Wan", wanNodeIndex );
	
	return 0;
}

int8 getWANDSLLinkCfg1ATMPeakCellRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDslLinkAttr.aATMPeakCellRate[getWANLinkIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANDSLLinkCfg1ATMPeakCellRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDslLinkAttr.aATMPeakCellRate[getWANLinkIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DSLLINK_ATTR_FLAG;
	return result;	
}
int8 getWANDSLLinkCfg1ATMMaximumBurstSizeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char tempValue[20] = {'\0'};
	uint32 wanNodeIndex;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&wanNodeIndex);
#else	
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name) - 1;
#endif
	sprintf(nodeName, "Wan_PVC%d", wanNodeIndex);
	if ( 0 != tcapi_get(nodeName, "MBS", tempValue) )
		strcpy(tempValue, "0");
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setWANDSLLinkCfg1ATMMaximumBurstSizeValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char active[10] = {'\0'};
	char ATMMbsStr[10] = {'\0'};
	uint32 ATMMbs;
	uint32 wanNodeIndex;
	int ret;

	ATMMbs = *(uint32*)value;
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&wanNodeIndex);
#else	
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name)-1;
#endif

	sprintf(nodeName, "Wan_PVC%d", wanNodeIndex);
	ret = tcapi_get(nodeName, "Active", active);
	if(ret){//this pvc is not exist.
		return -1;
	}
	
	sprintf(ATMMbsStr, "%lu", ATMMbs);
	tcapi_set(nodeName, "MBS", ATMMbsStr);
	
	SetCfgNodeFlag( "Wan", wanNodeIndex );
	
	return 0;
}

int8 getWANDSLLinkCfg1ATMMaximumBurstSizeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDslLinkAttr.aATMMaximumBurstSize[getWANLinkIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANDSLLinkCfg1ATMMaximumBurstSizeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDslLinkAttr.aATMMaximumBurstSize[getWANLinkIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DSLLINK_ATTR_FLAG;
	return result;
}
int8 getWANDSLLinkCfg1ATMSustainableCellRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char tempValue[20] = {'\0'};
	uint32 wanNodeIndex;

#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&wanNodeIndex);
#else	
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name) - 1;
#endif
	sprintf(nodeName, "Wan_PVC%d", wanNodeIndex);
	if ( 0 != tcapi_get(nodeName, "SCR", tempValue) )
		strcpy(tempValue, "0");
	
	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setWANDSLLinkCfg1ATMSustainableCellRateValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[34] = {'\0'};
	char active[10] = {'\0'};
	char ATMScrStr[10] = {'\0'};
	uint32 ATMScr;
	uint32 wanNodeIndex;
	int ret;


	ATMScr = *((uint32*)value);
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name);
	FindPVCMapping(&wanNodeIndex);
#else	
	wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name) - 1;
#endif
	sprintf(nodeName, "Wan_PVC%d", wanNodeIndex);

	ret = tcapi_get(nodeName, "Active", active);
	if(ret){ // this pvc is not exist.	
		return -1;
	}
	
	sprintf(ATMScrStr, "%lu", ATMScr);
	tcapi_set(nodeName, "SCR", ATMScrStr);

	SetCfgNodeFlag( "Wan", wanNodeIndex );

	return 0;
}

int8 getWANDSLLinkCfg1ATMSustainableCellRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDslLinkAttr.aATMSustainableCellRates[getWANLinkIndex(deviceNode)];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANDSLLinkCfg1ATMSustainableCellRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDslLinkAttr.aATMSustainableCellRates[getWANLinkIndex(deviceNode)];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DSLLINK_ATTR_FLAG;
	return result;
}
int8 getWANDSLLinkCfg1AAL5CRCErrorsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  AAL5CRCErrors[32] = {"0"};

	if ( 0 != tcapi_get("Info_Adsl", "CRCDown", AAL5CRCErrors) )
		strcpy(AAL5CRCErrors, "0");

	return getTheValue(value, type, deviceNode, AAL5CRCErrors);
}
int8 getWANDSLLinkCfg1AAL5CRCErrorsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDslLinkAttr.aWANDSLLinkCfg1AAL5CRCErrors;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANDSLLinkCfg1AAL5CRCErrorsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDslLinkAttr.aWANDSLLinkCfg1AAL5CRCErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DSLLINK_ATTR_FLAG;
	return result;
}
int8 getWANDSLLinkCfg1ATMCRCErrorsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  ATMCRCErrors[32] = {"0"};

	if ( 0 != tcapi_get("Info_Adsl", "CRCDown", ATMCRCErrors) )
		strcpy(ATMCRCErrors, "0");

	return getTheValue(value, type, deviceNode, ATMCRCErrors);
}
int8 getWANDSLLinkCfg1ATMCRCErrorsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDslLinkAttr.aWANDSLLinkCfg1ATMCRCErrors;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANDSLLinkCfg1ATMCRCErrorsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDslLinkAttr.aWANDSLLinkCfg1ATMCRCErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DSLLINK_ATTR_FLAG;
	return result;
}

int8 getWANDSLLinkCfg1ATMHECErrorsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  ATMHECErrors[32] = {"0"};

	if ( 0 != tcapi_get("Info_Adsl", "HECDown", ATMHECErrors) )
		strcpy(ATMHECErrors, "0");

	return getTheValue(value, type, deviceNode, ATMHECErrors);
}
int8 getWANDSLLinkCfg1ATMHECErrorsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDslLinkAttr.aWANDSLLinkCfg1ATMHECErrors;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANDSLLinkCfg1ATMHECErrorsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDslLinkAttr.aWANDSLLinkCfg1ATMHECErrors;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DSLLINK_ATTR_FLAG;
	return result;
}
#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_QOS)
int8 getWANDSLLinkCfg1CT_COM_VLANValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char qos_node[32], pvc_index_buf[10], vlan_buf[10];
	uint32 wanNodeIndex;
#if defined(TCSUPPORT_CT_WAN_PTM)
	char s_dsltype[32] = {0};
	int dslType = 0;
#endif

#if defined(TCSUPPORT_CT_WAN_PTM)
	dslType = getModulationType(s_dsltype, sizeof(s_dsltype));
	if ( 2 == dslType )
	{
		return getWANVdslLinkCfg1VLANIDMarkValue(value, type, deviceNode);
	}
	else
#endif
	{
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
		wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name);
		FindPVCMapping(&wanNodeIndex);
#else	
		wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name) - 1;
#endif

		memset(vlan_buf, 0, sizeof(vlan_buf));
		sprintf(qos_node, "QoS_Entry%d", wanNodeIndex + 22);
		tcapi_get(qos_node, "vlanid", vlan_buf);
		if(vlan_buf[0] == '\0')
		{
			strcpy(vlan_buf,"10000");
		}

		return getTheValue(value, type, deviceNode, vlan_buf);
	}
}
int8 setWANDSLLinkCfg1CT_COM_VLANValue(void* value, struct DeviceNode*  deviceNode)
{
	char qos_node[32], pvc_index_buf[10], vlan_buf[10];
	uint32 wanNodeIndex;
	uint32 vlanid = 0;

#if defined(TCSUPPORT_CT_WAN_PTM)
	char s_dsltype[32] = {0};
	int dslType = 0;
#endif
	
#if defined(TCSUPPORT_CT_WAN_PTM)
	dslType = getModulationType(s_dsltype, sizeof(s_dsltype));
	if ( 2 == dslType )
	{
		return setWANVdslLinkCfg1VLANIDMarkValue(value, deviceNode);
	}
	else
#endif
	{
#ifdef TCSUPPORT_CT_WANNODE_MAPPING//tony add
		wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name);
		FindPVCMapping(&wanNodeIndex);
#else	
		wanNodeIndex = atoi(deviceNode->parentNode->parentNode->name) - 1;
#endif
		sprintf(pvc_index_buf, "%d", wanNodeIndex);

		vlanid = *((uint32*)value);

		if (vlanid < 0 || vlanid > 4094) {
			/* 10000 means disable vlan */
			if (vlanid == 10000) {
				;
			}
			else {
				return -1;
			}
		}

		sprintf(vlan_buf, "%u", vlanid);
		sprintf(qos_node, "QoS_Entry%d", wanNodeIndex + 22);

		tcapi_set(qos_node, "vlanid", vlan_buf);
		tcapi_set(qos_node, "pvcIndex", pvc_index_buf);
		
		if (vlanid == 10000) {
			tcapi_set(qos_node, "Active", "No");
		}
		else {
			tcapi_set(qos_node, "Active", "Yes");
		}
		
		tcapi_set("QoS_Common", "QoSOptType", "vlanidRule");
		tcapi_commit(qos_node);

		SetCfgNodeFlag("Wan", wanNodeIndex);
		
		return 0;
	}
}
int8 getWANDSLLinkCfg1CT_COM_VLANAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDslLinkAttr.aWANDSLLinkCfg1CT_COM_VLAN;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANDSLLinkCfg1CT_COM_VLANAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDslLinkAttr.aWANDSLLinkCfg1CT_COM_VLAN;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DSLLINK_ATTR_FLAG;
	return result;
}

#endif
#endif

#endif


