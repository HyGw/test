/************************************************************************
 *
 *	Copyright (C) 2003 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/

#ifndef _GET_DEVICEINFO_ALL_FUNC_C_
#define _GET_DEVICEINFO_ALL_FUNC_C_

#include "parameter_comm_func.h"
#include "Get_LanInfo_func.h"

#define PARA_WVERSION	"SWVersion"
#define PARA_HDVERSION	"HDVersion"
#define PARA_PRODUCTCLASS	"ProductCLass"
#define PARA_CPUCLASS	"CPUClass"
#define PARA_DEVTYPE	"DEVType"
#define PARA_DEVTYPE1	"DEVType1"
#define PARA_CAPABILITY	"Capability"
#define	PARA_CARD	"Card"
#define PARA_UPLINK	"UPLink"
#define PARA_CARDNO	"Cardno"
#define PARA_PONREGSTATUS	"PonRegStatus"
#define PARA_VOIP1REGSTATUS	"VOIP1RegStatus"
#define PARA_VOIP2REGSTATUS	"VOIP2RegStatus"

#define PARA_WANINFO	"WANInfo"
#define PARA_TXPOWER	"TXPower"
#define PARA_RXPOWER	"RXPower"
#define PARA_WLANINFORM	"WLANInform"
#define PARA_SYSDURATION	"SYSDuration"
#define PARA_PPPOEDURATION	"PPPoEDuration"
#define PARA_PONDURATION	"PONDuration"
#define PARA_VOTTAGE	"Vottage"
#define PARA_CURRENT	"Current"
#define PARA_LINE	"line"
#define PARA_REGSTATUS	"RegStatus"
#define PARA_BUSSSTATUS	"BussinessStatus"
#define PARA_IP	"IP"
#define PARA_INTERDNS1	"InternetDNS1"
#define PARA_INTERDNS2	"InternetDNS2"
#define PARA_CPUPERCENT	"CPUPercent"
#define PARA_STOREPERCENT	"STOREPercent"
#define PARA_INFO	"Info"
#define PARA_WIFIEN	"WifiEnable"
#define PARA_VOIPNAME1	"VOIPNAME1"
#define PARA_VOIPNAME2	"VOIPNAME2"
#define PARA_CONNESTATUS	"ConnectionStatus"
#define PARA_WANSTATUS	"WANStatus"
#define PARA_DIALREASON	"DialReason"
#define PARA_CONNESTATUS1	"ConnectionStatus1"
#define PARA_WANSTATUS1	"WANStatus1"
#define PARA_DIALREASON1	"DialReason1"
#define	PARA_CARDSTATUS	"CardStatus"

typedef struct DeviceInfo_All_s{
	char SwVersion[64];
	char hwVersion[64];
	char ProductClass[64];
	char ModelName[164];
	char devtype1[12];
	char CpuUsagevalue[80];
	char MemUsagevalue[80];
	char ctempTemperature[16];
	char ctempVottage[16];
	char ctempCurrent[16];
	char AllWanStaInfo[2048];
	char ctempTXPower[16];
	char ctempRXPower[16];
	char tempPonRegStatus[48];
	char tempPonStatus[4];
	char VoIPRegInfo1[32];
	char VoIPRegInfo2[32];
	char WLanEnable[8];
	char cur_ssid_name[33];
	char ssidEnable[12];
	char wlaninfom[44];
	char passedsystime[12];
	char pppdurtime[64];
	char ponduration[12];
	char LanNetInfo[LANINFOMAXENTRY*LANINFOENTRYSIZE];
	char VoipName1[32];
	char VoipName2[32];
	char devcapval[100];
	char uplinkval[12];
	char bussinesssta[12];
	char regstainfo[20];
	char cardno[60];
	char iscardsepa[4];
	char IPv4ConStatus[8];
	char IPv4Status[32];
	char IPv4ConError[128];
	char IPv6ConStatus[8];
	char IPv6Status[32];
	char IPv6ConError[128];
	char str_tr069Ip[128];
	char str_dns[32];
	char str_secdns[32];
	char cardsta[4];
	char cpuclass[10];
} DeviceInfo_All_t;

json_t* GetDeviceInfoAll(json_t* req_root, json_t* req_para);
int GetPonStatus(char *cTemperature, char *cVottage, char *cCurrent, char *cTXPower, char *cRXPower);
void GetPonRegInfo(char *PonRegStatus, char *PonStatus);
void GetVoIPRegInfo(char *VoIPRegInfo1, char *VoIPRegInfo2);
void GetVoIPInfo(char *voipname1, char *voipname2);
void getDevCapabilityValue(char *devcapval);
void getUPLinkValue(char *uplinkval);
void getDeviceBasicInfo(char *swVer,char *hwVer,char *PClass,char *MName,char *devtype1,char *cpuclass);
void getWanStaInfo(char *AllWanStaInfo, char *tr069ip, char *intdns1, char *intdns2);
void GetSimCardInfo(char *existflag, char *num, char *cardstatus);
void GetBussinessSta(char *bussta);
int GetPPPDialStatus(char *IPv4ConStatus,char *IPv4Status,char *IPv4ConError,char *IPv6ConStatus,char *IPv6Status,char *IPv6ConError);
void GetRegStatusInfo(char * regstavalue);
int DeviceInfo(DeviceInfo_All_t *devinfo);

#endif
