/*
** $Id: //BBN_Linux/Branch/Branch_for_MT7505_IC_20131122/tclinux_phoenix/apps/private/TR69_64/cwmp/cwmpParameter/Sources/cwmpParameter_table.c#1 $
*/
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
#include "parameter_comm_func.h"
#include <netdb.h>
#include "threadpool.h"
#include "exec.h"
#include "info.h"

void fileRead(char *path, char *buf, int size){
	int  fd=0, ret=0;

	memset(buf,0, size);
	fd = open(path,O_RDONLY);
	if(fd == -1){
		return;
	}

	ret = read(fd, buf, size);
	if(ret <= 0){
		close(fd);
		return;
	}

	close(fd);
}/*end fileRead*/

int fileWrite(char *path, char *buf, int size){
	FILE *fp = NULL;
	
	fp = fopen(path, "w");
	if(fp == NULL){
		mobile_printf(MOBILE_ERR,"\n open failed");
		return -1;
	}
	fwrite(buf, sizeof(char), size, fp);
	
	fclose(fp);
}

char* itoa(register int i)
{
	static char a[7]; 
	register char *b = a + sizeof(a) - 1;
	int   sign = (i < 0);

	if (sign)
		i = -i;
	*b = 0;
	do
	{
		*--b = '0' + (i % 10);
		i /= 10;
	}
	while (i);
	if (sign)
		*--b = '-';
	return b;
}

int resolve(char *ipstr, unsigned long *addr, unsigned short *netmask)
{
	unsigned long ipaddr = 0;
	int i, bits;
	unsigned long digit;
	char *index;
	short mask = -1;

	for (i = 0; i < 3; i++) {
		index = strchr(ipstr, '.');
		if (index == NULL) break;
		*index = '\0';
		digit = atoi(ipstr);
		*index = '.';
		if (digit > 255) return -1;
		ipaddr = (ipaddr << 8) | digit;
		ipstr = index + 1;
	}

	index = strchr(ipstr, '/');     /* netmask ? */
	if (index != NULL) {
		mask = atoi(&index[1]);
		if (mask < 0 || mask > 32) return -1;
		*index = '\0';
	}

	digit = atoi(ipstr);
	bits = (4 - i) << 3;
	if (digit > ((1 << bits) - 1)) return -1;
	ipaddr = (ipaddr << bits) | digit;

	digit = ipaddr >> 28;		/* check multicast and reserved IP */
	if (digit >= 14) return -1;
	if (mask < 0) {
		if (digit < 8)
			mask = 8;
		else if (digit < 12)
			mask = 16;
		else if (digit < 14)
			mask = 24;
	}
	else
		*index = '/';

	*addr = ipaddr;
	if (netmask != NULL) *netmask = mask;
	return 0;
}

/*______________________________________________________________________________
**  checkName
**    This function detect if input is a domain name or a IP addr.
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:  0 : this is a domain name
		   1 : this is a IP
**  called by:resolveName()
**  call:
**  revision:
**____________________________________________________________________________*/
uint8 checkName(char *Name)
{
	char c;

	if(Name == NULL)
		return 0;	/* Can't happen */
	
	while((c = *Name++) != '\0'){
		if((!isdigit(c)) && (c != '.') && (c != ':')){
			if(c == '/'){
				return 1;
			}
			return 0;
		}
	}
	return 1;
}

unsigned long resolveName(char *name)
{
	struct hostent *h = NULL;
	struct sockaddr_in addr = {0};
	unsigned long ipAddr = 0;

	if(name == NULL)
		return 0;
	tcdbg_printf("\r\nEnter reslove name: [%s]", name);
	/*add by brian*/
	if(checkName(name))
	{
		resolve((char *)name, &ipAddr, NULL);
		if(ipAddr != 0)
		{
			return ipAddr;
		}
	}

	h = gethostbyname(name);

	if(h != NULL)
	{
		memcpy(&addr.sin_addr, h->h_addr, sizeof(addr.sin_addr));
		tcdbg_printf("\r\nreslove name success: name [%s] IP is [%s]", name, inet_ntoa(addr.sin_addr)); 
	}
	else
	{

		tcdbg_printf("\r\nresolve Name failed!");
	}

	return (unsigned long)addr.sin_addr.s_addr;
}

int isInRouteTable(char *ip, char *dev)
{
	char routePath[] = "/tmp/tmp_ipping_diagnostic_route";
	char cmdbuf[200] = {'\0'}, buf[200] = {'\0'}, tmp[200] = {'\0'};
	char routeInfo[7][16];
	int route_number = 0;
	FILE *fp = NULL;
	enum routeInfo_en{dstIP=0,gw,mask,metric,use,iface,usr_def};

	mobile_printf(MOBILE_DEBUG,"%s, line = %d\n",__FUNCTION__, __LINE__);

	sprintf(cmdbuf, "/sbin/route -n > %s", routePath);
#if EXEC_SYSTEM
	send2Exec(0x30, cmdbuf);
#else
	system(cmdbuf);
#endif	
	memset(routeInfo,0, sizeof(routeInfo));
	fp = fopen(routePath, "r");
	if(fp != NULL)
	{
		while (fgets(buf, sizeof(buf), fp))
		{
			if(route_number > 1)
			{
				sscanf(buf, "%s %s %s %s %s %s %s %s",
					routeInfo[dstIP], routeInfo[gw], routeInfo[mask],
					tmp, routeInfo[metric], tmp,
					routeInfo[use], routeInfo[iface]);
				if(strcmp(routeInfo[dstIP], ip) == 0 && strcmp(routeInfo[iface], dev) == 0){ //is aleardy in route table
					fclose(fp);					
					return 1;	
				}
			}
			route_number++;	
		}
		fclose(fp);
	}
	unlink(routePath);
	return 0;
}
/* 2 led off; 3 recover */
void LedStatusChange(int onoff){
#if defined(TCSUPPORT_CT_JOYME)
	if(onoff == 0){
		system("echo 2 > /proc/tc3162/led_switch_button");
		LedStatus = 0;
	}
	else{
		system("echo 3 > /proc/tc3162/led_switch_button");
		LedStatus = 1;
	}
#endif
}

void setSleepAct(int type){
#if defined(TCSUPPORT_CT_JOYME)
	char attrValue[8]={0};
	char strValue[32] = {0};
    char dev_path[32], file_path[32], mountfs[32], strline[128], nodeName[64];      
	int i = 0, number = 0, res = 0;
	
	if(type == 1){/* sleep */
		system("sys led off");
		system("echo 0 0 > /proc/tc3162/operate_lanport");
		SleepStatus=0;
		LedStatus = 0;
		
		/*usb*/
		tcapi_get("UsbMount_Common", "number", strValue);
		number = atoi(strValue);
		for (i = 0; i < number; i++){
			memset(nodeName, 0, sizeof(nodeName));
			sprintf(nodeName, "UsbMount_Entry%d", i);
			memset(dev_path, 0 ,sizeof(dev_path));
			tcapi_get(nodeName, "devpath", dev_path);
			memset(strline, 0 ,sizeof(strline));
            sprintf(strline, "/bin/umount -d %s", dev_path);        
			res = system(strline);   	
		}

		tcapi_set("UsbMount_Common", "SleepStatus", "1");
		/*end the usb*/

		/*wifi*/
		tcapi_set("WLan_Common", "SleepFlag", "1");
		/*end the wifi*/
		
	}
	else if(type == 0){/* wakeup */
		system("sys led recover");
		system("echo 0 1 > /proc/tc3162/operate_lanport");
		SleepStatus=1;
		LedStatus = 1;

		/*usb*/
		tcapi_get("UsbMount_Common", "number", strValue);
		number = atoi(strValue);
		for (i = 0; i < number; i++) {
			memset(nodeName, 0, sizeof(nodeName));
			sprintf(nodeName, "UsbMount_Entry%d", i);
			memset(mountfs, 0 ,sizeof(mountfs));
			tcapi_get(nodeName, "mountfs", mountfs);
			memset(dev_path, 0 ,sizeof(dev_path));
			tcapi_get(nodeName, "devpath", dev_path);
			memset(file_path, 0 ,sizeof(file_path));
			tcapi_get(nodeName, "filepath", file_path);
			memset(strline, 0 ,sizeof(strline));
			
			if (strlen(file_path) > 0) {
				sprintf(strline, "/bin/mount -o iocharset=utf8 -t vfat %s %s", dev_path, file_path);	
				res = system(strline);	

				/* vfat mount fail, try to use ntfs */
				if (res != 0) {
					sprintf(strline, "/userfs/bin/ntfs-3g %s %s", dev_path, file_path);
					system(strline);
				}
			}
		}
		
		tcapi_set("UsbMount_Common", "SleepStatus", "0");
		/*end the usb*/

		/*wifi*/
		tcapi_set("WLan_Common", "SleepFlag", "0");
		/*end the wifi*/
		
	}
	if(tcapi_get("WLan_Common", "APOn", attrValue) || atoi(attrValue) == type){
		tcapi_set("WLan_Common", "APOn", type ? "0" : "1");
		tcapi_commit("WLan_Entry");
		tcapi_save();
	}	
#endif
}

char * getlandeviceupdate(char *ip) {
#if defined(TCSUPPORT_CT_JOYME)
	CtLanDevInfo *devInfoList = NULL, *devInfoList_save = NULL;
	int ret = 0, num = 0, index = 0;
	char *pDevInfo = NULL;
	char *Type[] = {"PC","phone","Pad","PC","STB","OTHER"};   //devtype
	char *connectType[] = {"0","0","1"};//ConnectType
	char *transport[] = {"0","1","2","3","4","5","6","7","8","9","10","11","12"};  //Port
    int i;
	
	mobile_printf(MOBILE_ERR,"%s: ip = %s!...\n",__FUNCTION__, ip);
	ret = joyme_lanGetDevDevInfo(&devInfoList_save, &num);
	if((ret != 0) || (num <= 0)) {
		if(devInfoList_save) free(devInfoList_save);
		return NULL;
	}
	pDevInfo = calloc(1, sizeof(CtLanDevInfo) + 40);
	if(pDevInfo == NULL){
		mobile_printf(MOBILE_ERR,"%s: malloc err!...\n",__FUNCTION__);
		if(devInfoList_save) free(devInfoList_save);
		return NULL;
	}
	devInfoList = devInfoList_save;
	for(i = 0; i < num; i++){
		if(!strcmp(devInfoList->ipAddr, ip)) {
			index += sprintf(pDevInfo+index,"%s:%s",devInfoList->devName,Type[devInfoList->devType]);			
			index += sprintf(pDevInfo+index,":%02X%02X%02X%02X%02X%02X",
					devInfoList->macAddr[0], devInfoList->macAddr[1],devInfoList->macAddr[2],devInfoList->macAddr[3],devInfoList->macAddr[4],devInfoList->macAddr[5]);			
			index += sprintf(pDevInfo+index,":%s:%s",devInfoList->ipAddr,connectType[devInfoList->connType]);
			index += sprintf(pDevInfo+index,":%s",transport[devInfoList->port]);
			index += sprintf(pDevInfo+index,":%s:%s:%d/",devInfoList->brandName, devInfoList->osName, devInfoList->onlineTime);
			pDevInfo[index-1] = '\0';
			return pDevInfo;
		}
		devInfoList++;			

	}
	if(devInfoList_save) free(devInfoList_save);
#endif
	return NULL;
}
void mobile_restore_open(){
	
}

#define DEF_ROMFILE_PATH_EPON 	"/userfs/ctromfile_epon.cfg"
#define DEF_ROMFILE_PATH 	"/userfs/ctromfile.cfg"
#define GZ_TMP_PATH "/tmp/"
#define TMP_CONFIG_FILE_PATH "/tmp/ctromfile.cfg"
#define TMP_PHONE_RESTORE_FILE_PATH "/tmp/phonerestore.cfg"
#define MOBILE_START_NODE		"<mobile>"
#define MOBILE_STOP_NODE		"</mobile>"
#define MOBILE_START_ENTRY		"<Entry "
#define IPMACFILTER_NODE "IpMacFilter"
#define IPMACFILTER_COMMON "IpMacFilter_Common"
#define IPMACFILTER_ENTRY "IpMacFilter_Entry"
#define MAC_NUM_ATTR "mac_num"

/*______________________________________________________________________________
**	mobile_entry_save
**
**	descriptions:
**		save mobile_entry values.
**	parameters:
**
**	return:
**		
**_______________________________________________________________________________
*/
static void mobile_entry_save(char *in){
	int i = 0;
	char *pName = NULL, *pValue = NULL;

	mobile_printf(MOBILE_INFO, "%s: save %s:", __FUNCTION__,  in);
	if(pName = strtok_r(in,  "=",  &pValue)){
		i = strlen(pValue);
		pValue[i-1] = '\0';
		mobile_printf(MOBILE_INFO, "%s: %s!\n", pName, pValue+1);
		tcapi_set(MOBILE_ENTRY,  pName,   pValue+1);
	}
}

/*______________________________________________________________________________
**	mobile_entry_handle
**
**	descriptions:
**		handles mobile_entry values.
**	parameters:
**		in: string form romfile by line
**		out: handler buffer
**		pAdd: flag for uncomplete line
**	return:
**		Success: 	0
**		FAIL: -1
**_______________________________________________________________________________
*/
static int mobile_entry_handle(char *in, char *out, int *pAdd){
	int ret = -1, i = 0;
	char *p = NULL, *saveptr = NULL;

	saveptr = in;
	while( p= strtok_r(NULL, " ", &saveptr)){
		if(saveptr){
			if(*pAdd == 1){
				strcat(out,  p); /* add */
				p = out;
				*pAdd = 0;
			}
			mobile_entry_save(p);
			if(saveptr[0] == '\/'){
				//mobile_printf(MOBILE_INFO, "%s: stop!\n", __FUNCTION__);
				ret = 0;
				break;
			}
		}
		else{
			if(p= strtok_r(p, "\n", &saveptr)){
				//mobile_printf(MOBILE_INFO, "%s: enter %s!...\n", __FUNCTION__, p);
				mobile_entry_save(p);
			}else{				
				strcpy(out, p);
				*pAdd = 1;
				//mobile_printf(MOBILE_INFO, "%s: out %s!...\n", __FUNCTION__, out);
				break;
			}
		}		
	}
	return ret;
}

static FILE * mobile_get_defrom(void){
	FILE *rfp = NULL;
	char cmd[64]={0}, filepath[40] = {0};
	int ret=0;

	 tcapi_get("XPON_Common", "xponMode", cmd);
	if( strstr(cmd,"EPON") && !access(DEF_ROMFILE_PATH_EPON, X_OK)){
		strcpy(filepath,DEF_ROMFILE_PATH_EPON);
	}
	else{
		strcpy(filepath,DEF_ROMFILE_PATH);
	}
	
	if(TCSUPPORT_CT_ZIPROMFILE_VAL){
		memset(cmd, 0, sizeof(cmd));
		unlink(TMP_CONFIG_FILE_PATH);
		sprintf(cmd, "tar -xzvf %s -C %s",filepath,GZ_TMP_PATH);
		ret=system(cmd);
		mobile_printf(MOBILE_ERR, "%s: open tar!...\n", __FUNCTION__);
		rfp = fopen(TMP_CONFIG_FILE_PATH, "r");
	}
	else{
		mobile_printf(MOBILE_ERR, "%s: open !...\n", __FUNCTION__);
		rfp = fopen(filepath, "r");
	}
	return rfp;
	
}
/*______________________________________________________________________________
**	IpMacFilter_restore
**
**	descriptions:
**		restore IpMacFilter values from default romfile.
**	parameters:
**		
**	return:
**		
**_______________________________________________________________________________
*/
void IpMacFilter_restore(){
	char tmp[64] = {0};
	char nodeName[64] = {0};
	int iMacNum = 0;
	int i = 0, ret = 0;
	

	/* flush mac filter */
	system("iptables -F macfilter_chain");
	system("iptables -Z macfilter_chain");

	/*unset Entry*/
	tcapi_get(IPMACFILTER_NODE,MAC_NUM_ATTR,tmp);
	iMacNum = atoi(tmp);
	mobile_printf(MOBILE_INFO, "%s: iMacNum=%d\n", __FUNCTION__,iMacNum);
	for(i = 0; i < iMacNum; i++){
		memset(nodeName,0,sizeof(nodeName));
		sprintf(nodeName,"%s%d",IPMACFILTER_ENTRY,i);
		tcapi_unset(nodeName);
		mobile_printf(MOBILE_INFO, "%s: unset %s\n", __FUNCTION__,nodeName);
	}

	/*unset Common*/
	tcapi_unset(IPMACFILTER_COMMON);
}

#if defined(TCSUPPORT_CT_JOYME)
void Bandwidth_restore(){
	char nodeName[64] = {0};
	int i = 0;
	
	mobile_printf(MOBILE_INFO, "%s: entryNum=%d\n", __FUNCTION__,10);
	for(i = 0; i < 10; i++){
		memset(nodeName,0,sizeof(nodeName));
		sprintf(nodeName,"MaxBandWidth_Entry%d", i);
		tcapi_unset(nodeName);
		mobile_printf(MOBILE_INFO, "%s: unset %s\n", __FUNCTION__,nodeName);
	}
	tcapi_commit("MaxBandWidth");
}
#endif
/*______________________________________________________________________________
**	MannualDNS_restore
**
**	descriptions:
**		restore MannualDNS values from wan info.
**	parameters:
**		
**	return:
**		
**_______________________________________________________________________________
*/
void MannualDNS_restore(){
	char wanNode[40]={0},ifname[8] = {0};
	char def_route_index_v4[10] = {0};
	int if_index = -1, ret = -1;

	ret = tcapi_unset("WanInfo_MannualDNS");
	if(ret){
		mobile_printf(MOBILE_INFO, "%s: no MannualDNS set ret = %d!...\n", __FUNCTION__, ret);
	}
	
	/* get default route for ipv4 */
	memset(def_route_index_v4, 0, sizeof(def_route_index_v4));
	if(tcapi_get("WanInfo_Common", "DefRouteIndexv4", def_route_index_v4)){
		mobile_printf(MOBILE_ERR, "%s: get DefRouteIndexv4 error!...\n", __FUNCTION__);
		return;
	}
	if(strlen(def_route_index_v4) > 0 && strcmp(def_route_index_v4, "N/A")){
		if_index = atoi(def_route_index_v4);
		snprintf(wanNode, sizeof(wanNode), "Wan_PVC%d_Entry%d", if_index/8, if_index%8);
		if(tcapi_get(wanNode, "IFName", ifname)){
			mobile_printf(MOBILE_ERR, "%s: get IFName error!...\n", __FUNCTION__);
			return;
		}
	}
	if(strlen(def_route_index_v4) > 0 && strcmp(def_route_index_v4, "N/A")){
		snprintf(wanNode, sizeof(wanNode), "WanInfo_Message_%s/4", ifname);
		mobile_printf(MOBILE_INFO, "%s: commit %s!...\n", __FUNCTION__, wanNode);
		tcapi_commit(wanNode);
	}
}
extern char gDistriServerAddr[100];
extern char gDistriServerPort[10];
/*______________________________________________________________________________
**	mobile_restore
**
**	descriptions:
**		restore mobile_entry values from default romfile.
**	parameters:
**		
**	return:
**		
**_______________________________________________________________________________
*/
void mobile_restore(void){
	FILE *rfp = NULL;
	char bufin[256] = "",bufout[256] = "";
	int ret =0, start = 0, needadd = 0;

	mobile_printf(MOBILE_INFO, "%s: satrt@%llu!...\n", __FUNCTION__,event_time());
	//rfp = fopen(TMP_CONFIG_FILE_PATH, "r");
	rfp = mobile_get_defrom();
	if(rfp == NULL){
		mobile_printf(MOBILE_ERR, "%s: open"TMP_CONFIG_FILE_PATH"error!...\n", __FUNCTION__);
		return ;
	}
	tcapi_unset(MOBILE_ENTRY);
	tcapi_unset("mobile_DnsRedirect");
	while(ret = fgets(bufin, sizeof(bufin), rfp)){
		/* find mobile */
		if(0 == start){
			if(strstr(bufin, MOBILE_START_NODE)){
				//mobile_printf(MOBILE_INFO, "%s: find start node %s!...\n", __FUNCTION__, bufin);
				start = 1;
			}
			continue;
		}
		/* find mobile_Entry */
		else if(1 == start){
			if(strstr(bufin, MOBILE_START_ENTRY)){
				//mobile_printf(MOBILE_INFO, "%s: find start entry %s!...\n", __FUNCTION__, bufin);
				if(!mobile_entry_handle(bufin+7, bufout, &needadd)) break;
				start = 2;
			}
			continue;
		}
		
		if(strstr(bufin, MOBILE_STOP_NODE)){
			mobile_printf(MOBILE_INFO, "%s: @%llu read stop %s!...\n", __FUNCTION__, event_time(), bufin);
			break;
		}
		/* mobile_Entry first get later */
		if(!mobile_entry_handle(bufin, bufout, &needadd)) break;		
	}
	
	fclose(rfp);
	/* donot restore if default romfile not set */
	memset(bufin,   0, sizeof(bufin));
	memset(bufout, 0, sizeof(bufout));
	ret =   tcapi_get(MOBILE_ENTRY,  DISTRIADDR,  bufin);
	ret +=  tcapi_get(MOBILE_ENTRY,  DISTRIPORT,  bufout);
	//mobile_printf(MOBILE_ERR, "%s: donot restore "DISTRIADDR" and "DISTRIPORT":%s:%s!...\n", __FUNCTION__, bufin, bufout);
 	if(ret != 0  || strlen(bufin) == 0  || strlen(bufout) == 0){
		mobile_printf(MOBILE_ERR, "%s: donot restore "DISTRIADDR" and "DISTRIPORT"!...\n", __FUNCTION__);
		tcapi_set(MOBILE_ENTRY,  DISTRIADDR,  gDistriServerAddr);
		tcapi_set(MOBILE_ENTRY,  DISTRIPORT,  gDistriServerPort);
	}

	if(MobileDbg) tcapi_set(MOBILE_ENTRY,  MOBILEBG,  "2");
#if defined(TCSUPPORT_CT_JOYME)
	tcapi_set(MOBILE_ENTRY, "SSN", gSSN);
	Bandwidth_restore();
#endif
	IpMacFilter_restore();
	Sendclientpipe(); /* update wlan schedule */
	MannualDNS_restore();
	mobile_restore_open();
	
	tcapi_save();
}


