#include "Global_res.h"
#include "cp_wandev_wanconndev_xponLinkCfg.h"


/*
    rpc of tr069 parameters: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.X_CT-COM_WANEponLinkConfig.
	                                 InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.X_CT-COM_WANGponLinkConfig.
*/

int8 isExistActiveTR69Tunnel(uint8 pvc_index)
{
	char nodeName[32] = {0};
	char active[8] = {0};
	char serviceList[32] = {0};
	uint8 idx = 0;

	if ( pvc_index >= PVC_NUM )
		return -1;

	for ( idx = 0; idx < MAX_SMUX_NUM; idx ++)
	{
		sprintf(nodeName, "Wan_PVC%d_Entry%d", pvc_index, idx);
		if ( tcapi_get(nodeName, "Active", active) < 0
			|| 0 != strcmp(active, "Yes") )
			continue;

		if ( tcapi_get(nodeName, "ServiceList", serviceList) < 0 )
			continue;

		if( NULL != strstr(serviceList, "TR069") )
			return 1;
	}

	return 0;
}

int8 isExistDuplicateChannel(char *newMode, char *vid, char *pbit, uint8 pvc_index)
{
	char nodeName[32] = {0};
	char vlanMode[20] = {0};
	char vlanID[12] = {0};
	char v8021p[12] = {0};
	uint8 pvcidx = 0;

#if defined(TCSUPPORT_CT_PON_GS)
	return 0;
#endif

	if ( NULL == newMode || NULL == vid || NULL == pbit )
		return -1;

	if ( pvc_index >= PVC_NUM )
		return -1;

	// untag or transparent mode
	if ( 0 == strcmp(newMode, "UNTAG")
		|| 0 == strcmp(newMode, "TRANSPARENT"))
	{
		for ( pvcidx = 0; pvcidx < PVC_NUM; pvcidx++ )
		{
			if ( pvcidx == pvc_index )
				continue;

			sprintf(nodeName, "Wan_PVC%d", pvcidx);
			if ( tcapi_get(nodeName, "VLANMode", vlanMode) < 0 )
				continue;

			if ( 0 == strcmp(newMode, vlanMode) )
				return 1;
		}
	}
	else if ( 0 == strcmp(newMode, "TAG") )
	{
		if ( '\0' == vid[0] && '\0' == pbit[0] )
			return 0; // no need to check

		for ( pvcidx = 0; pvcidx < PVC_NUM; pvcidx++ )
		{
			if ( pvcidx == pvc_index )
				continue;

			sprintf(nodeName, "Wan_PVC%d", pvcidx);
			if ( tcapi_get(nodeName, "VLANMode", vlanMode) < 0 )
				continue;

			if ( 0 == strcmp("UNTAG", vlanMode)
				|| 0 == strcmp("TRANSPARENT", vlanMode))
				continue;

			if ( tcapi_get(nodeName, "VLANID", vlanID) < 0
				|| tcapi_get(nodeName, "DOT1P", v8021p) < 0)
				continue;

			if ( 0 == strcmp(vid, vlanID) && 0 == strcmp(pbit, v8021p) )
				return 1;
		}
	}

	return 0;
}

int8 updateVLANID_PBIT(char *newMode, char *vlanid, char *pbit, uint8 pvc_index)
{
	char nodeName[32] = {0};
	char active[12] = {0};
	uint8 entry_idx = 0;
	uint8 isVidMod = 0;

	if ( pvc_index >= PVC_NUM )
		return -1;

	if ( NULL == newMode || NULL == vlanid || NULL == pbit )
		return -1;

	sprintf(nodeName, "Wan_PVC%d", pvc_index);
	tcapi_set(nodeName, "VLANID", vlanid);
	tcapi_set(nodeName, "DOT1P", pbit);

	if ( 0 == strcmp(newMode, "TAG") )
		isVidMod = 1;

	for ( entry_idx = 0; entry_idx < MAX_SMUX_NUM; entry_idx ++ )
	{
		sprintf(nodeName, "Wan_PVC%d_Entry%d", pvc_index, entry_idx);
		if ( tcapi_get(nodeName, "Active", active) < 0 )
			continue;

		tcapi_set(nodeName, "VLANMode", newMode);
		if ( 1 == isVidMod )
		{
			tcapi_set(nodeName, "dot1q", "Yes");
			tcapi_set(nodeName, "dot1p", "Yes");
			tcapi_set(nodeName, "VLANID", vlanid);
			tcapi_set(nodeName, "dot1pData", pbit);
		}
		else
		{
			tcapi_set(nodeName, "dot1q", "No");
			tcapi_set(nodeName, "dot1p", "No");
			tcapi_set(nodeName, "VLANID", "");
			tcapi_set(nodeName, "dot1pData", "");

		}
	}

	return 0;
}


int8 getWANPonLinkCfg1EPonEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char enabled[4] = "Yes";
	char state[4] = "1";
	uint8 instant = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode )
		return -1;

	instant = (uint8)getWANLinkIndex(deviceNode);
	if ( instant >= MAX_WAN_NODE )
		return -1;

	sprintf(nodeName, "Wan_PVC%d", instant);
	tcapi_get(nodeName, "EPONEnable", enabled);
	if ( 0 == strcmp("No", enabled) )
		strcpy(state, "0");

	return getTheValue(value, type, deviceNode, state);
}
int8 setWANPonLinkCfg1EPonEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char enableStr[20] = {0};
	uint8 instant = 0;
	uint8 enable = *((uint8*)value);

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode )
		return -1;

	instant = (uint8)getWANLinkIndex(deviceNode);
	if ( instant >= MAX_WAN_NODE )
		return -1;

	// check tr69 tunnel
	if ( 1 == isExistActiveTR69Tunnel(instant) )
		return -1;

	switch ( enable )
	{
		case 0:
			strcpy(enableStr, "No");
			break;
		case 1:
			strcpy(enableStr, "Yes");
			break;
		default:
			return -1;
	}

	sprintf(nodeName, "Wan_PVC%d", instant);
	tcapi_set(nodeName, "EPONEnable", enableStr);
	SetCfgNodeFlag ( "Wan", instant );

	return 0;
}
int8 getWANPonLinkCfg1EPonEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;
	uint8 instant = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode )
		return -1;

	instant = (uint8)getWANLinkIndex(deviceNode);
	if ( instant >= MAX_WAN_NODE )
		return -1;

	attribute = ctPonLinkAttr.aWANPonLinkCfg1EPonEnable[instant];
	result = GetTheAttributes(attribute, notification, acl, deviceNode);
	return result;
}
int8 setWANPonLinkCfg1EPonEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8  result = 0;
	uint8 instant = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode )
		return -1;

	instant = (uint8)getWANLinkIndex(deviceNode);
	if ( instant >= MAX_WAN_NODE )
		return -1;

	attribute = &ctPonLinkAttr.aWANPonLinkCfg1EPonEnable[instant];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONLINK_ATTR_FLAG;
	return result;
}

int8 getWANPonLinkCfg1GPonEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char enabled[4] = "Yes";
	char state[4] = "1";
	uint8 instant = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode )
		return -1;

	instant = (uint8)getWANLinkIndex(deviceNode);
	if ( instant >= MAX_WAN_NODE )
		return -1;

	sprintf(nodeName, "Wan_PVC%d", instant);
	tcapi_get(nodeName, "GPONEnable", enabled);
	if ( 0 == strcmp("No", enabled) )
		strcpy(state, "0");

	return getTheValue(value, type, deviceNode, state);
}
int8 setWANPonLinkCfg1GPonEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char enableStr[20] = {0};
	uint8 instant = 0;
	uint8 enable = *((uint8*)value);

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode )
		return -1;

	instant = (uint8)getWANLinkIndex(deviceNode);
	if ( instant >= MAX_WAN_NODE )
		return -1;

	// check tr69 tunnel
	if ( 1 == isExistActiveTR69Tunnel(instant) )
		return -1;

	switch ( enable )
	{
		case 0:
			strcpy(enableStr, "No");
			break;
		case 1:
			strcpy(enableStr, "Yes");
			break;
		default:
			return -1;
	}

	sprintf(nodeName, "Wan_PVC%d", instant);
	tcapi_set(nodeName, "GPONEnable", enableStr);
	SetCfgNodeFlag ( "Wan", instant );

	return 0;
}
int8 getWANPonLinkCfg1GPonEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;
	uint8 instant = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode )
		return -1;

	instant = (uint8)getWANLinkIndex(deviceNode);
	if ( instant >= MAX_WAN_NODE )
		return -1;

	attribute = ctPonLinkAttr.aWANPonLinkCfg1GPonEnable[instant];
	result = GetTheAttributes(attribute, notification, acl, deviceNode);
	return result;
}
int8 setWANPonLinkCfg1GPonEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8  result = 0;
	uint8 instant = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode )
		return -1;

	instant = (uint8)getWANLinkIndex(deviceNode);
	if ( instant >= MAX_WAN_NODE )
		return -1;

	attribute = &ctPonLinkAttr.aWANPonLinkCfg1GPonEnable[instant];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONLINK_ATTR_FLAG;
	return result;
}


int8 getWANPonLinkCfg1ModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char mode[20] = "UNTAG";
	uint8 instant = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode )
		return -1;

	instant = (uint8)getWANLinkIndex(deviceNode);
	if ( instant >= MAX_WAN_NODE )
		return -1;

	sprintf(nodeName, "Wan_PVC%d", instant);
	tcapi_get(nodeName, "VLANMode", mode);

	if ( 0 == strcmp(mode, "UNTAG") )
		strcpy(mode, "0");
	else if ( 0 == strcmp(mode, "TRANSPARENT") )
		strcpy(mode, "1");
	else if ( 0 == strcmp(mode, "TAG") )
		strcpy(mode, "2");
	else
		strcpy(mode, "0");

	return getTheValue(value, type, deviceNode, mode);
}
int8 setWANPonLinkCfg1ModeValue(void* value, struct DeviceNode*  deviceNode)
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
	uint32 mode = *((uint32*)value);

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode )
		return -1;

	instant = (uint8)getWANLinkIndex(deviceNode);
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
		case 1:
			if ( 0 == mode )
				strcpy(modeStr, "UNTAG");
			else if ( 1 == mode )
				strcpy(modeStr, "TRANSPARENT");

			if ( 1 == isExistDuplicateChannel(modeStr, vlanid, pbit, instant) )
				return -1;

			if ( 0 == mode )
				updateVLANID_PBIT(modeStr, vid_untag, pbit_u, instant);
			else if ( 1 == mode )
				updateVLANID_PBIT(modeStr, vid_transp, pbit_u, instant);
			break;
		case 2:
			strcpy(modeStr, "TAG");
			if ( 0 == strcmp("UNTAG", oldmodeStr)
				|| 0 == strcmp("TRANSPARENT", oldmodeStr) )
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
int8 getWANPonLinkCfg1ModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;
	uint8 instant = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode )
		return -1;

	instant = (uint8)getWANLinkIndex(deviceNode);
	if ( instant >= MAX_WAN_NODE )
		return -1;

	attribute = ctPonLinkAttr.aWANPonLinkCfg1Mode[instant];
	result = GetTheAttributes(attribute, notification, acl, deviceNode);
	return result;
}
int8 setWANPonLinkCfg1ModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	uint8 instant = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode )
		return -1;

	instant = (uint8)getWANLinkIndex(deviceNode);
	if ( instant >= MAX_WAN_NODE )
		return -1;

	attribute = &ctPonLinkAttr.aWANPonLinkCfg1Mode[instant];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONLINK_ATTR_FLAG;
	return result;
}

int8 getWANPonLinkCfg1VLANIDMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char vlanid[20] = "0";
	char mode[20] = {0};
	uint8 instant = 0;
	int i_vlanid = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode )
		return -1;

	instant = (uint8)getWANLinkIndex(deviceNode);
	if ( instant >= MAX_WAN_NODE )
		return -1;

	sprintf(nodeName, "Wan_PVC%d", instant);
	tcapi_get(nodeName, "VLANMode", mode);
#if !defined(TCSUPPORT_CT_PON_C9_HUN) && !defined(TCSUPPORT_CT_PON_SK)
	if ( 0 == strcmp(mode, "TAG") )
#endif
	{
		tcapi_get(nodeName, "VLANID", vlanid);
		i_vlanid = atoi(vlanid);
		if ( '\0' == vlanid[0]
			|| i_vlanid < 0
			|| i_vlanid > 4094 )
			strcpy(vlanid, "0");
	}

	return getTheValue(value, type, deviceNode, vlanid);
}
int8 setWANPonLinkCfg1VLANIDMarkValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char vidStr[20] = {0};
	char pbitStr[20] = "0";
	char mode[20] = {0};
	uint8 instant = 0;
	uint32 vlanid = *((uint32*)value);

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode )
		return -1;

	instant = (uint8)getWANLinkIndex(deviceNode);
	if ( instant >= MAX_WAN_NODE )
		return -1;

	if ( vlanid > 4094 )
		return -1;

	sprintf(nodeName, "Wan_PVC%d", instant);

#if defined(TCSUPPORT_CT_PON) && !defined(TCSUPPORT_CT_PON_C9_HUN) && !defined(TCSUPPORT_CT_PON_SK)
	tcapi_set(nodeName, "VLANMode", "TAG");
#endif
	tcapi_get(nodeName, "VLANMode", mode);
#if defined(TCSUPPORT_CT_PON_C9_HUN)
	if ((0 == strcmp(mode, "UNTAG")) && (0 == vlanid)){
		return 0;
	}
	else
#elif defined(TCSUPPORT_CT_PON_SK)
	if (0 != strcmp(mode, "TAG")){
		return 0;
	}
	else
#endif
	{
#if defined(TCSUPPORT_CT_PON_C9_HUN) || defined(TCSUPPORT_CT_PON_SK)
	tcapi_set(nodeName, "VLANMode", "TAG");
	tcapi_get(nodeName, "VLANMode", mode);
#endif
	if ( 0 != strcmp(mode, "TAG") )
		return -1;

	tcapi_get(nodeName, "DOT1P", pbitStr);
	sprintf(vidStr, "%u", vlanid);
#if !defined(TCSUPPORT_CT_PON_CN_JS)
	if ( 1 == isExistDuplicateChannel(mode, vidStr, pbitStr, instant) )
		return -1;
#endif
	tcapi_set(nodeName, "VLANID", vidStr);
	updateVLANID_PBIT(mode, vidStr, pbitStr, instant);
	SetCfgNodeFlag ( "Wan", instant );
	}

	return 0;
}
int8 getWANPonLinkCfg1VLANIDMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;
	uint8 instant = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode )
		return -1;

	instant = (uint8)getWANLinkIndex(deviceNode);
	if ( instant >= MAX_WAN_NODE )
		return -1;

	attribute = ctPonLinkAttr.aWANPonLinkCfg1VLANIDMark[instant];
	
#if defined(TCSUPPORT_CT_PON_CZ_GD)
	if (attribute & 0x01)
		attribute &= 0xfe;
	else
		attribute |= 0x01;
#endif

	result = GetTheAttributes(attribute, notification, acl, deviceNode);
	return result;
}
int8 setWANPonLinkCfg1VLANIDMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8  result = 0;
	uint8 instant = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode )
		return -1;

	instant = (uint8)getWANLinkIndex(deviceNode);
	if ( instant >= MAX_WAN_NODE )
		return -1;

	attribute = &ctPonLinkAttr.aWANPonLinkCfg1VLANIDMark[instant];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

#if defined(TCSUPPORT_CT_PON_CZ_GD)
	if (*attribute & 0x01)
		*attribute &= 0xfe;
	else
		*attribute |= 0x01;
#endif
	
	rpc.attrFlags.theSettingFlag1 |= CT_PONLINK_ATTR_FLAG;
	return result;
}

int8 getWANPonLinkCfg18021PMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char priority[20] = "0";
	char mode[20] = {0};
	uint8 instant = 0;
	int i_pbit = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode )
		return -1;

	instant = (uint8)getWANLinkIndex(deviceNode);
	if ( instant >= MAX_WAN_NODE )
		return -1;

	sprintf(nodeName, "Wan_PVC%d", instant);
	tcapi_get(nodeName, "VLANMode", mode);
	if ( 0 == strcmp(mode, "TAG") )
	{
		tcapi_get(nodeName, "DOT1P", priority);
		i_pbit = atoi(priority);
		if ( '\0' == priority[0]
			|| i_pbit < 0
			|| i_pbit > 7 )
			strcpy(priority, "0");
	}

	return getTheValue(value, type, deviceNode, priority);
}
int8 setWANPonLinkCfg18021PMarkValue(void* value, struct DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char mode[20] = {0};
	char priorityStr[20] = {0};
	char vlanidStr[20] = {0};
	uint8 instant = 0;
	uint32 priValue = *((uint32*)value);

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode )
		return -1;

	instant = (uint8)getWANLinkIndex(deviceNode);
	if ( instant >= MAX_WAN_NODE )
		return -1;

	if ( priValue > 7)
		return -1;

	sprintf(nodeName, "Wan_PVC%d", instant);
	tcapi_set(nodeName, "VLANMode", "TAG");
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
int8 getWANPonLinkCfg18021PMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;
	uint8 instant = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode )
		return -1;

	instant = (uint8)getWANLinkIndex(deviceNode);
	if ( instant >= MAX_WAN_NODE )
		return -1;

	attribute = ctPonLinkAttr.aWANPonLinkCfg18021PMark[instant];
	result = GetTheAttributes(attribute, notification, acl, deviceNode);
	return result;
}
int8 setWANPonLinkCfg18021PMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8  result = 0;
	uint8 instant = 0;

	if ( NULL == deviceNode
		|| NULL == deviceNode->parentNode
		|| NULL == deviceNode->parentNode->parentNode )
		return -1;

	instant = (uint8)getWANLinkIndex(deviceNode);
	if ( instant >= MAX_WAN_NODE )
		return -1;

	attribute = &ctPonLinkAttr.aWANPonLinkCfg18021PMark[instant];
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONLINK_ATTR_FLAG;
	return result;
}


