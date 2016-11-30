/******************************************************************************/
/*
 * Copyright (C) 1994-2008 TrendChip Technologies, Corp.
 * All Rights Reserved.
 *
 * TrendChip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of TrendChip Technologies, Corp. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * TrendChip Technologies, Corp.
 */
/******************************************************************************/
#ifndef _CT_SGW_API_H_
#define _CT_SGW_API_H_

#include "joyme.h"

int ctSgw_init(int *sock);
int ctSgw_cleanup(void);
int ctSgw_handleEvent(CtEvnetType *eventType, void **eventInfo);
int ctSgw_wlanSetState(int ssidIndex, int enable);
int ctSgw_wlanGetSecurityCfg(CtWlanSecurityCfg *securityCfg);
int ctSgw_lanGetDevInfo(CtLanDevInfo **devInfoList, int *devNum);
int ctSgw_lanUpdateDevInfo(const CtLanDevInfo *devInfo);
int ctSgw_lanSetDevAccessPermission(const CtMacAddr *macAddr, int interetAccess, int storageAccess);
int ctSgw_lanGetDevInternetAccessBlacklist(CtMacAddr **macAddrList, int *macAddrNum);
int ctSgw_lanGetDevStorageAccessBlacklist(CtMacAddr * * macAddrList,int * macAddrNum);
int ctSgw_lanGetDevBandwidth(const CtMacAddr *macAddr, int* usBandwith, int* dsBandwith);
int ctSgw_lanSetDevMaxBandwidth(const CtMacAddr *macAddr, int usBandwith, int dsBandwith);
int ctSgw_lanGetDevMaxBandwidth(const CtMacAddr *macAddr, int* usBandwith, int* dsBandwith);
int ctSgw_lanGetPortStatus(int *portStatus);
int ctSgw_lanSetDevStatsStatus(int enable);
int ctSgw_wanGetIfStats(int *usStats, int *dsStats);
int ctSgw_wanGetPppoeAccount(char *pppoeAccount, int accountLen);
int ctSgw_wanCreateL2tpTunnel(const char *serverIpAddr,const char *username,const char *password,char *tunnelName,int nameLen);
int ctSgw_wanRemoveL2tpTunnel(const char *tunnelName);
int ctSgw_wanAttachL2tpTunnel(const char *tunnelName,const CtIpAddr *ipAddrList, int ipAddrNum);
int ctSgw_wanDetachL2tpTunnel(const char *tunnelName,const CtIpAddr *ipAddrList, int ipAddrNum);
int ctSgw_wanGetL2tpTunnelStatus(const char *tunnelName, int *tunnelStatus);
int ctSgw_sysGetCpuUsage(int *percent);
int ctSgw_sysGetMemUsage(int *percent);
int ctSgw_sysGetIpAddr(CtSysIpAddrCfg *ipAddrCfg);
int ctSgw_sysGetLoid(char *loid, int loidLen);
int ctSgw_sysGetMac(unsigned char mac[6]);
int ctSgw_sysPushWeb(int enable, int width, int height, const char *place, const char *url, int time);
int ctSgw_usbRegister(CtUsbDevType devType);
int ctSgw_usbUnregister(void);
int ctSgw_usbLock(int devId);
int ctSgw_usbUnlock(int devId);
int ctSgw_usbOpen(int devId, int *handle);
int ctSgw_usbClose(int handle);
int ctSgw_usbDataAvailable(int handle);
int ctSgw_usbRead(int handle, void *buf, size_t* count, int timeout);
int ctSgw_usbWrite(int handle, void *buf, size_t* count);
int ctSgw_usbSetSerial(int handle, const CtUsbSerialCfg *serialCfg);
int ctSgw_eventInform(const char *event);
int ctSgw_dpiCtxCreate(void ** p_dpi_ctx, unsigned char flag);
int ctSgw_dpiCtxDestory(void ** p_dpi_ctx);
int ctSgw_dpiProcAppld(unsigned char *layer2data, CtSgwTupleInfo *tupleinfo, void * dpi_ctx, unsigned int *layer7_id);
int ctSgw_appRegisterFunc(ctSgw_dpiFuncs *funcs);
void ctSgw_appUnRegisterFunc(void);
void ctSgw_upg_test(void);
int ctSgw_sysSetSSN(const char *ssn);
int ctSgw_sysGetSSN(char *ssn, int ssnLen);

#endif
