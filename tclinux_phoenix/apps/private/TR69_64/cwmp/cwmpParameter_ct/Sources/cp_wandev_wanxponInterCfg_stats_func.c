#include "Global_res.h"
#include "cp_wandev_wanxponInterCfg_stats.h"

/*
    rpc of tr069 parameters: InternetGatewayDevice.WANDevice.1.X_CT-COM_EponInterfaceConfig.Stats.
	                                 InternetGatewayDevice.WANDevice.1.X_CT-COM_GponInterfaceConfig.Stats.
*/


int8 getWANPONInterCfg_BytesSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char bytesSent[32] = "0";

	tcapi_get("Info_PonWanStats","TxFrameLen",bytesSent);

	return getTheValue(value, type, deviceNode, bytesSent);
}
int8 getWANPONInterCfg_BytesSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgStatsAttr.aWANPonInterCfgStatsBytesSent;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_BytesSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgStatsAttr.aWANPonInterCfgStatsBytesSent;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_BytesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char bytesRecv[32] = "0";

	tcapi_get("Info_PonWanStats","RxFrameLen",bytesRecv);

	return getTheValue(value, type, deviceNode, bytesRecv);
}
int8 getWANPONInterCfg_BytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgStatsAttr.aWANPonInterCfgStatsBytesReceived;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_BytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgStatsAttr.aWANPonInterCfgStatsBytesReceived;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_PacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char pktsSend[32] = "0";

	tcapi_get("Info_PonWanStats","TxFrameCnt",pktsSend);

	return getTheValue(value, type, deviceNode, pktsSend);
}
int8 getWANPONInterCfg_PacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgStatsAttr.aWANPonInterCfgStatsPacketsSent;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_PacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgStatsAttr.aWANPonInterCfgStatsPacketsSent;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_PacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char pktsRecv[32] = "0";

	tcapi_get("Info_PonWanStats","RxFrameCnt",pktsRecv);
	
	return getTheValue(value, type, deviceNode, pktsRecv);
}
int8 getWANPONInterCfg_PacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgStatsAttr.aWANPonInterCfgStatsPacketsReceived;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_PacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgStatsAttr.aWANPonInterCfgStatsPacketsReceived;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_SUnicastPacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char suniPktsSend[32] = "0";
	char totalSend[32] = {0};
	char multiSend[32] = {0};
	char broadcastSend[32] = {0};
	int  unicastSend = 0;
	
	tcapi_get("Info_PonWanStats","TxFrameCnt",totalSend);
	tcapi_get("Info_PonWanStats","TxMulticastCnt",multiSend);
	tcapi_get("Info_PonWanStats","TxBroadcastCnt",broadcastSend);

	unicastSend = atoi(totalSend) - atoi(multiSend) - atoi(broadcastSend);
	sprintf(suniPktsSend,"%d",unicastSend);
	
	return getTheValue(value, type, deviceNode, suniPktsSend);
}
int8 getWANPONInterCfg_SUnicastPacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgStatsAttr.aWANPonInterCfgStatsSUnicastPackets;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_SUnicastPacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgStatsAttr.aWANPonInterCfgStatsSUnicastPackets;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_RUnicastPacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char runiPktsRecv[32] = "0";
	char totalRecv[32] = {0};
	char multiRecv[32] = {0};
	char broadcastRecv[32] = {0};
	int  unicastRecv = 0;
	
	tcapi_get("Info_PonWanStats","RxFrameCnt",totalRecv);
	tcapi_get("Info_PonWanStats","RxMulticastCnt",multiRecv);
	tcapi_get("Info_PonWanStats","RxBroadcastCnt",broadcastRecv);
	
	unicastRecv = atoi(totalRecv) - atoi(multiRecv) - atoi(broadcastRecv);
	sprintf(runiPktsRecv,"%d",unicastRecv);

	return getTheValue(value, type, deviceNode, runiPktsRecv);
}
int8 getWANPONInterCfg_RUnicastPacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgStatsAttr.aWANPonInterCfgStatsRUnicastPackets;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_RUnicastPacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgStatsAttr.aWANPonInterCfgStatsRUnicastPackets;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_SMulticastPacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char smulPktsSend[32] = "0";

	tcapi_get("Info_PonWanStats","TxMulticastCnt",smulPktsSend);

	return getTheValue(value, type, deviceNode, smulPktsSend);
}
int8 getWANPONInterCfg_SMulticastPacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgStatsAttr.aWANPonInterCfgStatsSMulticastPackets;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_SMulticastPacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgStatsAttr.aWANPonInterCfgStatsSMulticastPackets;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_RMulticastPacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char smulPktsRecv[32] = "0";

	tcapi_get("Info_PonWanStats","RxMulticastCnt",smulPktsRecv);

	return getTheValue(value, type, deviceNode, smulPktsRecv);
}
int8 getWANPONInterCfg_RMulticastPacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgStatsAttr.aWANPonInterCfgStatsRMulticastPackets;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_RMulticastPacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgStatsAttr.aWANPonInterCfgStatsRMulticastPackets;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_SBroadcastPacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char sbroadPktsSent[32] = "0";

	tcapi_get("Info_PonWanStats","TxBroadcastCnt",sbroadPktsSent);

	return getTheValue(value, type, deviceNode, sbroadPktsSent);
}
int8 getWANPONInterCfg_SBroadcastPacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgStatsAttr.aWANPonInterCfgStatsSBroadcastPackets;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_SBroadcastPacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgStatsAttr.aWANPonInterCfgStatsSBroadcastPackets;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_RBroadcastPacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char rbroadPktsRecv[32] = "0";

	tcapi_get("Info_PonWanStats","RxBroadcastCnt",rbroadPktsRecv);
	
	return getTheValue(value, type, deviceNode, rbroadPktsRecv);
}
int8 getWANPONInterCfg_RBroadcastPacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgStatsAttr.aWANPonInterCfgStatsRBroadcastPackets;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_RBroadcastPacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgStatsAttr.aWANPonInterCfgStatsRBroadcastPackets;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_FECErrorValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char fecError[32] = "0";

	tcapi_get("Info_PonWanStats","RxFecErrorCnt",fecError);

	return getTheValue(value, type, deviceNode, fecError);
}
int8 getWANPONInterCfg_FECErrorAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgStatsAttr.aWANPonInterCfgStatsFECError;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_FECErrorAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgStatsAttr.aWANPonInterCfgStatsFECError;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_HECErrorValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char hecError[32] = "0";

	tcapi_get("Info_PonWanStats","RxHecErrorCnt",hecError);
	
	return getTheValue(value, type, deviceNode, hecError);
}
int8 getWANPONInterCfg_HECErrorAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgStatsAttr.aWANPonInterCfgStatsHECError;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_HECErrorAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgStatsAttr.aWANPonInterCfgStatsHECError;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_DropPacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char dropPkts[32] = "0";

	tcapi_get("Info_PonWanStats","TxDropCnt",dropPkts);

	return getTheValue(value, type, deviceNode, dropPkts);
}
int8 getWANPONInterCfg_DropPacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgStatsAttr.aWANPonInterCfgStatsDropPackets;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_DropPacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgStatsAttr.aWANPonInterCfgStatsDropPackets;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_SPausePacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char sPausePkts[32] = "0";

	return getTheValue(value, type, deviceNode, sPausePkts);
}
int8 getWANPONInterCfg_SPausePacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgStatsAttr.aWANPonInterCfgStatsSPausePackets;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_SPausePacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgStatsAttr.aWANPonInterCfgStatsSPausePackets;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_RPausePacketsValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[33] = {0};
	char rPausePkts[32] = "0";

	return getTheValue(value, type, deviceNode, rPausePkts);
}
int8 getWANPONInterCfg_RPausePacketsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgStatsAttr.aWANPonInterCfgStatsRPausePackets;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_RPausePacketsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgStatsAttr.aWANPonInterCfgStatsRPausePackets;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getWANPONInterCfg_ReceiveBlocksValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char receiveBlocks[32] = "0";
	
	tcapi_get("Info_PonWanStats","RxFrameCnt",receiveBlocks);
	return getTheValue(value, type, deviceNode, receiveBlocks);
}
int8 getWANPONInterCfg_ReceiveBlocksAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgStatsAttr.aWANPonInterCfgStatsReceiveBlocks;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_ReceiveBlocksAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgStatsAttr.aWANPonInterCfgStatsReceiveBlocks;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_TransmitBlocksValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char transmitBlocks[32] = "0";
	
	tcapi_get("Info_PonWanStats","TxFrameCnt",transmitBlocks);
	return getTheValue(value, type, deviceNode, transmitBlocks);
}
int8 getWANPONInterCfg_TransmitBlocksAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgStatsAttr.aWANPonInterCfgStatsTransmitBlocks;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_TransmitBlocksAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgStatsAttr.aWANPonInterCfgStatsTransmitBlocks;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}

int8 getWANPONInterCfg_FCSErrorValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char FCSError[32] = "0";
	int fcserror = 0;
	char buf[128] = {0};
	char temp[]="/proc/tc3162/eth_reg";	
	char tempvalue[2][32];
	char *pValue=NULL;
	FILE * fp=NULL;
	
	memset(tempvalue,0, sizeof(tempvalue));	
	fp=fopen(temp, "r");
	if(fp!=NULL)
	{
		while (fgets (buf, sizeof(buf), fp) != NULL)
		{
			if((pValue=strstr(buf,"FCS_CNT")))
			{
				if((pValue=strstr(buf,"=")))
				{
					sscanf(pValue+1,"%x", &fcserror);
				}
				break;
            }
		}		
		fclose(fp);
	}
	
	if(fcserror != 0)
	{
		sprintf(FCSError, "%d", fcserror);
	}
	
	return getTheValue(value, type, deviceNode, FCSError);
}
int8 getWANPONInterCfg_FCSErrorAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = ctPonInterCfgStatsAttr.aWANPonInterCfgStatsFCSError;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setWANPONInterCfg_FCSErrorAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &ctPonInterCfgStatsAttr.aWANPonInterCfgStatsFCSError;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_PONINTERCFGSTATS_ATTR_FLAG;
	return result;
}
#endif
