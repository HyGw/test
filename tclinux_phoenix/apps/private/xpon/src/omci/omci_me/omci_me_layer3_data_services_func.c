

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "omci_types.h"
#include "omci_me.h"
#include "omci_general_func.h"
#include "omci_me_layer3_data_services.h"

/*******************************************************************************************************************************
									C O N S T A N T S
										
********************************************************************************************************************************/

#define IP_OPTION_DHCP_MASK		(1<<0)
#define IP_OPTION_ENABLE_PING_RESPONSE_MASK (1<<1)
#define IP_OPTION_ENABLE_TRACERT_RESPONSE_MASK (1<<2)
#define IP_OPTION_ENABLE_IPSTACK_MASK (1<<3)
#define IP_HOST_MAC_PATH "/etc/mac.conf"

#define IP_HOST_PING_TEST			0x1
#define IP_HOST_TRACEROUTE_TEST	0x2
#define IP_HOST_VENDOR_TEST 		0x8
#define IP_HOST_TEST_PING_PATH		"/tmp/omci/ipHostPingTest_stat"
#define IP_HOST_TEST_TRACEROUTE_PATH	"/tmp/omci/ipHostTracerouteTest_stat"
#define MAX_BUF_LEN 		1024
#define IMCOMPLETE_STR		"0 packets received"
#define ICMP_SEQ0			"icmp_seq=0"
#define ICMP_SEQ1			"icmp_seq=1"
#define ICMP_SEQ2			"icmp_seq=2"
#define ICMP_SEQ3			"icmp_seq=3"

#define MAX_ARG_NUM	3
#define MAX_NODE_NAME 16
#define WAN	"Wan"
#define WAN_PVC "PVC"
#define IPHOST_NODE "XPON_IPHost"
#define COMMIT_WAN_ATTR "commitWan"
#define PVC_NUM 8
#define TMP_IF_PATH "/tmp/wanInfo"
enum wanIFType{
	IP_ADDR=0,
	NETMASK,
	GATEWAY,
	PRI_DNS,
	SEC_DNS,
	MAC_ADDR,
	ISP,
	VLANID,
	DOT1P,
};

#if defined(TCSUPPORT_CT)
#define WAN_IF_PATH "/var/run/"
#define WAN_INTERFACE "Entry"
#define WANNODE_FORMAT "Wan_PVC%d_Entry%d"
#define IFNAME_FORMAT "nas%d_%d"
#define MAX_SMUX_NUM 8
#else
#define WANNODE_FORMAT "Wan_PVC%d"
#define IFNAME_FORMAT "nas%d"
#define WANDEV_INFO_FORMAT "DeviceInfo_PVC%d"
#endif

/*******************************************************************************************************************************
									FUNCTIONS
										
********************************************************************************************************************************/

int checkInternalClientIp(char *st);
int checkSubnetLegal(char *mask);
int omciInternalCreatActionForIPHostConfigData(uint16 classId, uint16 instanceId);
int omciTestActionForIPHostConfigData(uint16 meClassId , omciMeInst_t *meInstant_ptr, omciPayload_t * omciPayLoad, uint8 msgSrc);
int omciIPHostCfgDataTestRstMsgHandle(pthreadIPHostArgType *pthreadArg);
int omciIPHostCfgDataTestRstMsgHandle_Extended(pthreadIPHostArgType *pthreadArg);
void startTestForIPHost(void* arg);
void fileRead(char *path, char *buf, int size);
int get_profile_str(char *keyname,char *str_return, int size, int type, char *path);

extern int unlink(const char *pathname);
extern uint32 aton(char *s);
extern int commitNodeTimeId;

/*******************************************************************************************************************************
9.4.1: IP host config data

********************************************************************************************************************************/
int omciMeInitForIPHostConfigData(omciManageEntity_t *omciManageEntity_p)
{
	omciManageEntity_t *tmpOmciManageEntity_p = NULL;
	
	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n enter omciMeInitForIPHostConfigData \r\n");

	if(omciManageEntity_p == NULL)
		return -1;
	
	tmpOmciManageEntity_p = omciManageEntity_p;
	/* init some param for this ME */
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_GET] = omciGetAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_SET] = omciSetAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_TEST] = omciTestActionForIPHostConfigData;
	
	return 0;
}

int omciInternalCreatActionForIPHostConfigData(uint16 classId, uint16 instanceId)
{
	omciManageEntity_t *tmpOmciManageEntity_p = NULL;
	omciMeInst_t *tmpomciMeInst_p = NULL;
	uint8 * attributeValuePtr = NULL;
	uint16 length = 0;
	uint8 buffer[32] = {0};
	uint16 attriMask = 0;
	uint8 value[6] = {0};

	tmpOmciManageEntity_p = omciGetMeByClassId(classId);
	if(tmpOmciManageEntity_p == NULL)
		return -1;
	tmpomciMeInst_p = omciAllocInstance(tmpOmciManageEntity_p);
	if(tmpomciMeInst_p == NULL)
		return -1;
	//tmpomciMeInst_p->deviceId = instanceId;//for receive message
 
	/*meId*/
	attributeValuePtr = tmpomciMeInst_p->attributeVlaue_ptr;
	if (attributeValuePtr != NULL)
		put16(attributeValuePtr, instanceId);
	
	attriMask = 1<<15;
	attributeValuePtr = omciGetInstAttriByMask(tmpomciMeInst_p, attriMask, &length);
	if (attributeValuePtr != NULL)
		*attributeValuePtr = 0;
	else
		goto fail;
	
	getIPHostConfigDataMACAddr(value, tmpomciMeInst_p, &tmpOmciManageEntity_p->omciAttriDescriptList[2], 0);

	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "omciInternalCreatActionForIPHostConfigData: mac = %02x%02x%02x%02x%02x%02x\n", value[0], value[1], value[2], value[3], value[4], value[5]);

	attriMask = 1<<14;
	attributeValuePtr = omciGetInstAttriByMask(tmpomciMeInst_p, attriMask, &length);
	memset(buffer, 0, sizeof(buffer));
	if (attributeValuePtr != NULL)
		memcpy(attributeValuePtr, value, 6);

	attriMask = 1<<13;
	attributeValuePtr = omciGetInstAttriByMask(tmpomciMeInst_p, attriMask, &length);
	memset(buffer, 0, sizeof(buffer));
	if (attributeValuePtr != NULL)
		memcpy(attributeValuePtr, buffer, 25);

	attriMask = 1<<12;
	attributeValuePtr = omciGetInstAttriByMask(tmpomciMeInst_p, attriMask, &length);
	memset(buffer, 0, sizeof(buffer));
	if (attributeValuePtr != NULL)
		memcpy(attributeValuePtr, buffer, 4);

	attriMask = 1<<11;
	attributeValuePtr = omciGetInstAttriByMask(tmpomciMeInst_p, attriMask, &length);
	memset(buffer, 0, sizeof(buffer));
	if (attributeValuePtr != NULL)
		memcpy(attributeValuePtr, buffer, 4);

	attriMask = 1<<10;
	attributeValuePtr = omciGetInstAttriByMask(tmpomciMeInst_p, attriMask, &length);
	if (attributeValuePtr != NULL)
		memcpy(attributeValuePtr, buffer, 4);

	attriMask = 1<<9;
	attributeValuePtr = omciGetInstAttriByMask(tmpomciMeInst_p, attriMask, &length);
	memset(buffer, 0, sizeof(buffer));
	if (attributeValuePtr != NULL)
		memcpy(attributeValuePtr, buffer, 4);

	attriMask = 1<<8;
	attributeValuePtr = omciGetInstAttriByMask(tmpomciMeInst_p, attriMask, &length);
	memset(buffer, 0, sizeof(buffer));
	if (attributeValuePtr != NULL)
		memcpy(attributeValuePtr, buffer, 4);

	attriMask = 1<<7;
	attributeValuePtr = omciGetInstAttriByMask(tmpomciMeInst_p, attriMask, &length);
	memset(buffer, 0, sizeof(buffer));
	if (attributeValuePtr != NULL)
		memcpy(attributeValuePtr, buffer, 4);

	attriMask = 1<<6;
	attributeValuePtr = omciGetInstAttriByMask(tmpomciMeInst_p, attriMask, &length);
	memset(buffer, 0, sizeof(buffer));
	if (attributeValuePtr != NULL)
		memcpy(attributeValuePtr, buffer, 4);

	attriMask = 1<<5;
	attributeValuePtr = omciGetInstAttriByMask(tmpomciMeInst_p, attriMask, &length);
	memset(buffer, 0, sizeof(buffer));
	if (attributeValuePtr != NULL)
		memcpy(attributeValuePtr, buffer, 4);

	attriMask = 1<<4;
	attributeValuePtr = omciGetInstAttriByMask(tmpomciMeInst_p, attriMask, &length);
	memset(buffer, 0, sizeof(buffer));
	if (attributeValuePtr != NULL)
		memcpy(attributeValuePtr, buffer, 4);

	attriMask = 1<<3;
	attributeValuePtr = omciGetInstAttriByMask(tmpomciMeInst_p, attriMask, &length);
	memset(buffer, 0, sizeof(buffer));
	if (attributeValuePtr != NULL)
		memcpy(attributeValuePtr, buffer, 4);

	attriMask = 1<<2;
	attributeValuePtr = omciGetInstAttriByMask(tmpomciMeInst_p, attriMask, &length);
	memset(buffer, 0, sizeof(buffer));
	if (attributeValuePtr != NULL)
		memcpy(attributeValuePtr, buffer, 25);

	attriMask = 1<<1;
	attributeValuePtr = omciGetInstAttriByMask(tmpomciMeInst_p, attriMask, &length);
	memset(buffer, 0, sizeof(buffer));
	if (attributeValuePtr != NULL)
		memcpy(attributeValuePtr, buffer, 25);

	omciAddInstanceToTail(tmpOmciManageEntity_p, tmpomciMeInst_p);

	return 0;	
	
fail:
	omciFreeInstance(tmpomciMeInst_p);
	omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "attributeValuePtr == NULL");
	return -1;	
}

int omciInitInstForIPHostConfigData(void)
{
	int ret = 0;
	uint16 instanceId = 0;
	int i = 0;
	
	if(isFiberhome_551601){
		instanceId = 0x1;   /*me instance id*/
	}
	else{
		instanceId = 0x0;   /*me instance id*/
	}

#if defined(TCSUPPORT_TR69_IP_HOST)
	for( i = instanceId; i <= instanceId+1; i++){
		ret = omciInternalCreatActionForIPHostConfigData(OMCI_CLASS_ID_IP_HOST_CONFIG_DATA, i);
	        if (ret == -1)		
			omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "omciInitInst create omciInitInstForIPHostConfigData fail\n");
	}
#else
	ret = omciInternalCreatActionForIPHostConfigData(OMCI_CLASS_ID_IP_HOST_CONFIG_DATA, instanceId);
        if (ret == -1)		
		omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "omciInitInst create omciInitInstForIPHostConfigData fail\n");
#endif

	return ret;
}

int omciTestActionForIPHostConfigData(uint16 meClassId , omciMeInst_t *meInstant_ptr, omciPayload_t * omciPayLoad, uint8 msgSrc)
{
	uint8 *contentPtr = NULL;
	pthreadIPHostArgType *pthreadArg = NULL;
	pthread_t thread_id;
	pthread_attr_t thread_attr;
	uint32 addr = 0;
	uint8 resultAndReason = 0;
	uint8 testType = 0;
	uint8 devId = 0;
	omciBaselinePayload_t *omciBaselinePayload=NULL;
#ifdef TCSUPPORT_OMCI_EXTENDED_MSG
	omciExtendedPayload_t *omciExtendedPayload=NULL;
#endif

	if(omciPayLoad == NULL)
		return OMCI_CMD_ERROR;

	devId = omciPayLoad->devId;
	if(devId == OMCI_BASELINE_MSG)
	{
		omciBaselinePayload = (omciBaselinePayload_t *)omciPayLoad;
		contentPtr = (uint8 *)(omciBaselinePayload->msgContent);
	}
#ifdef TCSUPPORT_OMCI_EXTENDED_MSG
	else	if(devId == OMCI_EXTENED_MSG)
	{
		omciExtendedPayload = (omciExtendedPayload_t *)omciPayLoad;
		contentPtr = (uint8 *)(omciExtendedPayload->msgContent);
	}	
#endif
	else
	{
		omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "devId is invalid!\n");							
		return -1;
	}	
	
	pthreadArg = osMalloc(sizeof(pthreadIPHostArgType));
	if(pthreadArg == NULL)
		return -1;
	memset(pthreadArg, 0, sizeof(pthreadIPHostArgType));
	pthreadArg->msgFormat = devId;
	testType = (*contentPtr) & 0xF;
	memcpy(&addr, contentPtr+1, 4);
	if ((testType == IP_HOST_PING_TEST) || (testType == IP_HOST_TRACEROUTE_TEST) || (testType >= IP_HOST_VENDOR_TEST))
	{
		pthreadArg->tranCorrelatId = omciPayLoad->tranCorrelatId;
		pthreadArg->testType= testType;
		pthreadArg->meClassId = meClassId;
		pthreadArg->addr = addr;
		omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "omciTestAction: pthreadArg.tranCorrelatId = %x\n", pthreadArg->tranCorrelatId);
		omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "omciTestAction: pthreadArg.testType = %x\n", pthreadArg->testType);
		omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "omciTestAction: pthreadArg.meClassId = %x \n", pthreadArg->meClassId);
		omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "omciTestAction: pthreadArg.addr = %x \n", pthreadArg->addr);
	}
	else/*reserved for future use*/
	{
		resultAndReason = OMCI_CMD_NOT_SUPPORT;
		if(pthreadArg != NULL){
			osFree(pthreadArg);
			pthreadArg = NULL;
		}
		goto pktSend;
	}

	pthread_attr_init(&thread_attr);
	pthread_attr_setstacksize(&thread_attr, MAX_STACK_SIZE);
	pthread_attr_setdetachstate(&thread_attr,PTHREAD_CREATE_DETACHED); 

	if( pthread_create(&thread_id, &thread_attr, (void *)startTestForIPHost, (void*)pthreadArg) != 0 ){
		fprintf(stderr, "pthread_create error!!\n");
		if(pthreadArg != NULL){
			osFree(pthreadArg);
			pthreadArg = NULL;
		}
		exit(0);
	}
pktSend:

	/*MessageType response AR = 0; AK =1*/
	responseMessageTypeHandle((uint8 *)&omciPayLoad->msgType);

	if(devId == OMCI_BASELINE_MSG)
	{
		/*clear the Content to 0*/
		memset(contentPtr, 0, OMCI_BASELINE_CONT_LEN);
	}
#ifdef TCSUPPORT_OMCI_EXTENDED_MSG
	else	
	{
		/*clear the Content to 0*/
		memset(contentPtr, 0, omciExtendedPayload->msgContentLength+OMCI_EXEENTED_SIZE_OF_MIC);	
		omciExtendedPayload->msgContentLength = 1;
	}
#endif	

	/*write reason and result*/
	writeResultAndReasonToOmciPayLoad(resultAndReason, contentPtr);
	//	omciDumpPkt(omciPayLoad);

	omciPktSend(omciPayLoad, TRUE);	

	pthread_attr_destroy(&thread_attr);
	
	return 0;
}

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
	
	return;
}

int get_profile_str(char *keyname,char *str_return, int size, int type, char *path)
{

	FILE *fp = NULL;
	char *str_key = NULL;
	char stream[128] = {0};
	int enterOffSet = 1;
	int qmarkLength = 0;
	int skipQmark = 0;
	int totalLength = 0;

	fp = fopen(path,"r");
	if(fp == NULL){
		fprintf(stderr,"Can't open %s\n",path);
		return -1;
	}

	memset(str_return, 0, size);
	fseek(fp, 0, 0);
	if(type == 1){
		qmarkLength = 2;
		skipQmark = 1;
	}
	else if(type == 0){
		qmarkLength = 0;
		skipQmark = 0;
	}
	else{
		fprintf(stderr, "The input qmark type of get_profile_str is wrong \n");
		fclose(fp);
		return -1;
	}

	while(fgets(stream, 128, fp) != NULL){
		//str_enter = strrchr(stream,'\n');
		if(strrchr(stream,'\n') == NULL){
			enterOffSet = 0;
		}
		else{
			enterOffSet = 1;
		}

		str_key = strstr(stream,keyname);
		if(str_key == NULL || str_key != stream){
			continue;
		}

		totalLength = strlen(stream) - strlen(keyname) - enterOffSet - qmarkLength;
		if(size < totalLength+1){/*total length + '\0' should not less than buffer*/
			fprintf(stderr, "Too small buffer to catch the %s frome get_profile_str\n", keyname);
			fclose(fp);
			return -1;
		}
		else if(totalLength < 0) {/*can't get a negative length string*/
			fprintf(stderr, "No profile string can get\n");
			fclose(fp);
			return -1;
		}
		else{
			strncpy(str_return, stream+strlen(keyname)+skipQmark, totalLength);
			str_return[totalLength] = '\0';
			fclose(fp);
			return strlen(str_return);
		}
		
		memset(stream, 0, 128);
	}
	fclose(fp);
	fprintf(stderr,"File %s content %s is worng\n",path,keyname);
	
	return -1;
}

int omciIPHostCfgDataTestRstMsgHandle(pthreadIPHostArgType *pthreadArg)
{ 
	omciPayload_t tstPkt;
	omciBaselinePayload_t *tstPkt_baseline=NULL;
	uint8 *tmp = NULL; 
	char desAddr[32] = {0};
	char cmdbuf[120] = {0};
	char *tmpbuf = NULL;
	char *tmpPtr = NULL;
	char tmpValue[32] = {0};
	int i = 0, j = 0, cnt = 0;
	uint32 tmpAddr = 0;
	char indexStr[8] = {0};
	struct in_addr address;
	
	if (pthreadArg == NULL)
		return -1;
	
	memset(&tstPkt, 0, sizeof(omciPayload_t));
	tstPkt_baseline = (omciBaselinePayload_t *)(&tstPkt);
	
	tstPkt_baseline->tranCorrelatId = pthreadArg->tranCorrelatId;
	tmp = (uint8*)&tstPkt_baseline->meId;
	put16(tmp, OMCI_CLASS_ID_IP_HOST_CONFIG_DATA);
	put16(tmp+2, 0);
	tstPkt_baseline->msgType = MT_OMCI_MSG_TYPE_TEST_RST;
	tstPkt_baseline->devId = OMCI_BASELINE_MSG;
 
	tmpbuf = malloc(MAX_BUF_LEN);
	if(tmpbuf == NULL)
	{
		omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "malloc fail!");
		return -1;
	}
	memset(tmpbuf, 0, sizeof(tmpbuf));

	if(pthreadArg->testType == 1)//ping
	{
		//do ping
		memset(desAddr, 0, sizeof(desAddr));
		memset(&address, 0, sizeof(address));
		address.s_addr = pthreadArg->addr;
		//printf("\r\n debug ping:pthreadArg->addr=%x", pthreadArg->addr);
		strcpy(desAddr, (char *)inet_ntoa(address));
		omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n ping:desAddr=%s", desAddr);
		
		memset(cmdbuf, 0, sizeof(cmdbuf));
		sprintf(cmdbuf,"ping -c %d %s > %s 2>/dev/null", 4, desAddr, IP_HOST_TEST_PING_PATH);
		system(cmdbuf);
		
		//read the ping result
		fileRead(IP_HOST_TEST_PING_PATH, tmpbuf, MAX_BUF_LEN);
		if(strstr(tmpbuf, IMCOMPLETE_STR) || 
			((!strstr(tmpbuf, ICMP_SEQ0)) && (!strstr(tmpbuf, ICMP_SEQ1)) && (!strstr(tmpbuf, ICMP_SEQ2)) &&(!strstr(tmpbuf, ICMP_SEQ3))))
		{
			omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n ping result:no response \n");
			/*send test result packets*/
			//test result: 000, time out, no response
			tmp = (uint8 *)(tstPkt_baseline->msgContent);
			*tmp = 0;
			unlink(IP_HOST_TEST_PING_PATH);
		}
		else
		{
			omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n ping result:001, ICMP echo response attached\n");
			//test result: 001, ICMP echo response attached
			tmp = (uint8 *)(tstPkt_baseline->msgContent);
			*tmp = 1;
			tmp++;
			*tmp = 0x8;
			tmp++;
			if((tmpPtr = strstr(tmpbuf, ICMP_SEQ0)) == NULL)
			{
				put16(tmp, 0xFFFF);
			}
			else
			{
				tmpPtr = strstr(tmpbuf, "time=");
				tmpPtr += 5;//format as: 64 bytes from X.X.X.X: icmp_seq=0 ttl=64 time=X.X ms
				i = 0;
				memset(tmpValue, 0, sizeof(tmpValue));
				while((*tmpPtr) != ' '){
					tmpValue[i++] = *tmpPtr;
					tmpPtr++;
				}
				tmpValue[i] = '\0';
				//printf("\r\n debug tmpValue = %s \r\n", tmpValue);
				//printf("\r\n debug tmpTime = %d \r\n", atoi(tmpValue));
				put16(tmp, atoi(tmpValue));
			}
			tmp += 2;
			if((tmpPtr = strstr(tmpbuf, ICMP_SEQ1)) == NULL)
			{
				put16(tmp, 0xFFFF);
			}
			else
			{
				tmpPtr = strstr(tmpbuf, "time=");
				tmpPtr += 5;//format as: 64 bytes from X.X.X.X: icmp_seq=0 ttl=64 time=X.X ms
				i = 0;
				memset(tmpValue, 0, sizeof(tmpValue));
				while((*tmpPtr) != ' '){
					tmpValue[i++] = *tmpPtr;
					tmpPtr++;
				}
				tmpValue[i] = '\0';
				//printf("\r\n debug tmpValue = %s \r\n", tmpValue);
				//printf("\r\n debug tmpTime = %d \r\n", atoi(tmpValue));
				put16(tmp, atoi(tmpValue));
			}
			tmp += 2;
			if((tmpPtr = strstr(tmpbuf, ICMP_SEQ2)) == NULL)
			{
				put16(tmp, 0xFFFF);
			}
			else
			{
				tmpPtr = strstr(tmpbuf, "time=");
				tmpPtr += 5;//format as: 64 bytes from X.X.X.X: icmp_seq=0 ttl=64 time=X.X ms
				i = 0;
				memset(tmpValue, 0, sizeof(tmpValue));
				while((*tmpPtr) != ' '){
					tmpValue[i++] = *tmpPtr;
					tmpPtr++;
				}
				tmpValue[i] = '\0';
				//printf("\r\n debug tmpValue = %s \r\n", tmpValue);
				//printf("\r\n debug tmpTime = %d \r\n", atoi(tmpValue));
				put16(tmp, atoi(tmpValue));
			}
			tmp += 2;
			if((tmpPtr = strstr(tmpbuf, ICMP_SEQ3)) == NULL)
			{
				put16(tmp, 0xFFFF);
			}
			else
			{
				tmpPtr = strstr(tmpbuf, "time=");
				tmpPtr += 5;//format as: 64 bytes from X.X.X.X: icmp_seq=0 ttl=64 time=X.X ms
				i = 0;
				memset(tmpValue, 0, sizeof(tmpValue));
				while((*tmpPtr) != ' '){
					tmpValue[i++] = *tmpPtr;
					tmpPtr++;
				}
				tmpValue[i] = '\0';
				//printf("\r\n debug tmpValue = %s \r\n", tmpValue);
				//printf("\r\n debug tmpTime = %d \r\n", atoi(tmpValue));
				put16(tmp, atoi(tmpValue));
			}
			unlink(IP_HOST_TEST_PING_PATH);
		}
	}
	else if(pthreadArg->testType == 2)//traceroute
	{
		//do traceroute
		//test result: 010, ICMP time exceeded response attached
		tmp = (uint8 *)(tstPkt_baseline->msgContent);
		*tmp = 2;
		tmp++;
		
		memset(desAddr, 0, sizeof(desAddr));
		memset(&address, 0, sizeof(address));
		address.s_addr = pthreadArg->addr;
		//printf("\r\n debug traceroute:pthreadArg->addr=%x", pthreadArg->addr);
		strcpy(desAddr, (char *)inet_ntoa(address));
		omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n traceroute:desAddr=%s", desAddr);
		
		memset(cmdbuf, 0, sizeof(cmdbuf));
		sprintf(cmdbuf,"/usr/bin/traceroute -m 7 -q 1 -n %s > %s 2>/dev/null", desAddr, IP_HOST_TEST_TRACEROUTE_PATH);
		system(cmdbuf);
		
		//read the traceroute result
		fileRead(IP_HOST_TEST_TRACEROUTE_PATH, tmpbuf, MAX_BUF_LEN);
		if((tmpPtr = strstr(tmpbuf, " 1  ")) == NULL)
		{
			*tmp = 0;//cnt = 0
		}
		else
		{
			tmp++;
			for(j=1; j<8; j++)
			{
				memset(indexStr, 0, sizeof(indexStr));
				sprintf(indexStr, " %d  ", j);
				if((tmpPtr = strstr(tmpbuf, indexStr)) == NULL)
					continue;
				tmpPtr += 4;
				i = 0;
				memset(tmpValue, 0, sizeof(tmpValue));
				if((*tmpPtr) != '*'){
					while((*tmpPtr) != ' '){
						tmpValue[i++] = *tmpPtr;
						tmpPtr++;
					}
					tmpValue[i] = '\0';
					//printf("\r\n debug traceroute:tmpValue=%s", tmpValue);
					tmpAddr = aton(tmpValue);
					//printf("\r\n debug traceroute:tmpValue=%02X", tmpAddr);
					put32(tmp, tmpAddr);
					tmp += 4;
					cnt++;
				}
			}
			tmp = (uint8 *)(tstPkt_baseline->msgContent);
			tmp++;
			*tmp = 4*cnt;
		}
		//omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n traceroute: cnt=%d", cnt);
		unlink(IP_HOST_TEST_TRACEROUTE_PATH);
	}
	else
	{
		omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "testType error!");
		if(tmpbuf != NULL){
			free(tmpbuf);
			tmpbuf = NULL;
		}
		return -1;
	}

	if(tmpbuf != NULL){
		free(tmpbuf);
		tmpbuf = NULL;
	}

	return	omciInternalNotificationPktSend(&tstPkt);
}

#ifdef TCSUPPORT_OMCI_EXTENDED_MSG	
int omciIPHostCfgDataTestRstMsgHandle_Extended(pthreadIPHostArgType *pthreadArg)
{
	omciPayload_t tstPkt;
	omciExtendedPayload_t *tstPkt_extended=NULL;	
	uint8 *tmp = NULL; 
	char desAddr[32] = {0};
	char cmdbuf[120] = {0};
	char *tmpbuf = NULL;
	char *tmpPtr = NULL;
	char tmpValue[32] = {0};
	int i = 0, j = 0, cnt = 0;
	uint32 tmpAddr = 0;
	char indexStr[8] = {0};
	struct in_addr address;
	
	if (pthreadArg == NULL)
		return -1;
	
	memset(&tstPkt, 0, sizeof(tstPkt));
	tstPkt_extended = (omciExtendedPayload_t *)(&tstPkt);
	
	tstPkt_extended->tranCorrelatId = (uint16)pthreadArg->tranCorrelatId;
	tmp = (uint8*)(&(tstPkt_extended->meId));
	put16(tmp, OMCI_CLASS_ID_IP_HOST_CONFIG_DATA);
	put16(tmp+2, 0);
	tstPkt_extended->msgType = MT_OMCI_MSG_TYPE_TEST_RST;
	tstPkt_extended->devId = OMCI_EXTENED_MSG;
 
	tmpbuf = malloc(MAX_BUF_LEN);
	if(tmpbuf == NULL)
	{
		omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "malloc fail!");
		return -1;
	}
	memset(tmpbuf, 0, sizeof(tmpbuf));

	if(pthreadArg->testType == 1)//ping
	{
		//do ping
		memset(desAddr, 0, sizeof(desAddr));
		memset(&address, 0, sizeof(address));
		address.s_addr = pthreadArg->addr;
		strcpy(desAddr, (char *)inet_ntoa(address));
		omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n ping:desAddr=%s", desAddr);
		
		memset(cmdbuf, 0, sizeof(cmdbuf));
		sprintf(cmdbuf,"ping -c %d %s > %s 2>/dev/null", 4, desAddr, IP_HOST_TEST_PING_PATH);
		system(cmdbuf);
		
		//read the ping result
		fileRead(IP_HOST_TEST_PING_PATH, tmpbuf, MAX_BUF_LEN);
		if(strstr(tmpbuf, IMCOMPLETE_STR) || 
			((!strstr(tmpbuf, ICMP_SEQ0)) && (!strstr(tmpbuf, ICMP_SEQ1)) && (!strstr(tmpbuf, ICMP_SEQ2)) &&(!strstr(tmpbuf, ICMP_SEQ3))))
		{
			omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n ping result:no response \n");
			/*send test result packets*/
			//test result: 000, time out, no response
			tstPkt_extended->msgContentLength = 0x1; 
			unlink(IP_HOST_TEST_PING_PATH);
		}
		else
		{
			omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n ping result:001, ICMP echo response attached\n");
			//test result: 001, ICMP echo response attached
			tstPkt_extended->msgContentLength = 0x9; 
			tmp = tstPkt_extended->msgContent;
			*tmp = 1;
			tmp++; 
			if((tmpPtr = strstr(tmpbuf, ICMP_SEQ0)) == NULL)
			{
				put16(tmp, 0xFFFF);
			}
			else
			{
				tmpPtr = strstr(tmpbuf, "time=");
				tmpPtr += 5;//format as: 64 bytes from X.X.X.X: icmp_seq=0 ttl=64 time=X.X ms
				i = 0;
				memset(tmpValue, 0, sizeof(tmpValue));
				while((*tmpPtr) != ' '){
					tmpValue[i++] = *tmpPtr;
					tmpPtr++;
				}
				tmpValue[i] = '\0';
				//printf("\r\n debug tmpValue = %s \r\n", tmpValue);
				//printf("\r\n debug tmpTime = %d \r\n", atoi(tmpValue));
				put16(tmp, atoi(tmpValue));
			}
			tmp += 2;
			if((tmpPtr = strstr(tmpbuf, ICMP_SEQ1)) == NULL)
			{
				put16(tmp, 0xFFFF);
			}
			else
			{
				tmpPtr = strstr(tmpbuf, "time=");
				tmpPtr += 5;//format as: 64 bytes from X.X.X.X: icmp_seq=0 ttl=64 time=X.X ms
				i = 0;
				memset(tmpValue, 0, sizeof(tmpValue));
				while((*tmpPtr) != ' '){
					tmpValue[i++] = *tmpPtr;
					tmpPtr++;
				}
				tmpValue[i] = '\0';
				//printf("\r\n debug tmpValue = %s \r\n", tmpValue);
				//printf("\r\n debug tmpTime = %d \r\n", atoi(tmpValue));
				put16(tmp, atoi(tmpValue));
			}
			tmp += 2;
			if((tmpPtr = strstr(tmpbuf, ICMP_SEQ2)) == NULL)
			{
				put16(tmp, 0xFFFF);
			}
			else
			{
				tmpPtr = strstr(tmpbuf, "time=");
				tmpPtr += 5;//format as: 64 bytes from X.X.X.X: icmp_seq=0 ttl=64 time=X.X ms
				i = 0;
				memset(tmpValue, 0, sizeof(tmpValue));
				while((*tmpPtr) != ' '){
					tmpValue[i++] = *tmpPtr;
					tmpPtr++;
				}
				tmpValue[i] = '\0';
				//printf("\r\n debug tmpValue = %s \r\n", tmpValue);
				//printf("\r\n debug tmpTime = %d \r\n", atoi(tmpValue));
				put16(tmp, atoi(tmpValue));
			}
			tmp += 2;
			if((tmpPtr = strstr(tmpbuf, ICMP_SEQ3)) == NULL)
			{
				put16(tmp, 0xFFFF);
			}
			else
			{
				tmpPtr = strstr(tmpbuf, "time=");
				tmpPtr += 5;//format as: 64 bytes from X.X.X.X: icmp_seq=0 ttl=64 time=X.X ms
				i = 0;
				memset(tmpValue, 0, sizeof(tmpValue));
				while((*tmpPtr) != ' '){
					tmpValue[i++] = *tmpPtr;
					tmpPtr++;
				}
				tmpValue[i] = '\0';
				//printf("\r\n debug tmpValue = %s \r\n", tmpValue);
				//printf("\r\n debug tmpTime = %d \r\n", atoi(tmpValue));
				put16(tmp, atoi(tmpValue));
			}
			unlink(IP_HOST_TEST_PING_PATH);
		}
	}
	else if(pthreadArg->testType == 2)//traceroute
	{
		//do traceroute
		//test result: 010, ICMP time exceeded response attached
		tmp = tstPkt_extended->msgContent;
		*tmp = 2;
		tmp++;
		
		memset(desAddr, 0, sizeof(desAddr));
		memset(&address, 0, sizeof(address));
		address.s_addr = pthreadArg->addr;
		//printf("\r\n debug traceroute:pthreadArg->addr=%x", pthreadArg->addr);
		strcpy(desAddr, (char *)inet_ntoa(address));
		omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n traceroute:desAddr=%s", desAddr);
		
		memset(cmdbuf, 0, sizeof(cmdbuf));
		sprintf(cmdbuf,"/usr/bin/traceroute -m 7 -q 1 -n %s > %s 2>/dev/null", desAddr, IP_HOST_TEST_TRACEROUTE_PATH);
		system(cmdbuf);
		
		//read the traceroute result
		fileRead(IP_HOST_TEST_TRACEROUTE_PATH, tmpbuf, MAX_BUF_LEN);
		if((tmpPtr = strstr(tmpbuf, " 1  ")) == NULL)
		{
			//cnt = 0
			tstPkt_extended->msgContentLength = 1; 
		}
		else
		{
			for(j=1; j<8; j++)
			{
				memset(indexStr, 0, sizeof(indexStr));
				sprintf(indexStr, " %d  ", j);
				if((tmpPtr = strstr(tmpbuf, indexStr)) == NULL)
					continue;
				tmpPtr += 4;
				i = 0;
				memset(tmpValue, 0, sizeof(tmpValue));
				if((*tmpPtr) != '*'){
					while((*tmpPtr) != ' '){
						tmpValue[i++] = *tmpPtr;
						tmpPtr++;
					}
					tmpValue[i] = '\0';
					//printf("\r\n debug traceroute:tmpValue=%s", tmpValue);
					tmpAddr = aton(tmpValue);
					//printf("\r\n debug traceroute:tmpValue=%02X", tmpAddr);
					put32(tmp, tmpAddr);
					tmp += 4; 
					cnt++;
				}
			}
			tstPkt_extended->msgContentLength = 4*cnt+1;
		}  
		unlink(IP_HOST_TEST_TRACEROUTE_PATH);
	}
	else
	{
		omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "testType error!");
		if(tmpbuf != NULL){
			free(tmpbuf);
			tmpbuf = NULL;
		}
		return -1;
	}

	if(tmpbuf != NULL){
		free(tmpbuf);
		tmpbuf = NULL;
	}

	return	omciInternalNotificationPktSend(&tstPkt);
}
#endif

void startTestForIPHost(void* arg)
{
	pthreadIPHostArgType *pthreadArg = NULL;
	int ret = 0;

	if (arg == NULL)
		return;
	
	pthreadArg = (pthreadIPHostArgType*)arg;

	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "startTest:pthreadArg->tranCorrelatId = %x\n", pthreadArg->tranCorrelatId);
	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "startTest:pthreadArg->testType = %x\n", pthreadArg->testType);
	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "startTest:pthreadArg->meClassId = %x\n", pthreadArg->meClassId);
	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "startTest:pthreadArg->addr = %x\n", pthreadArg->addr);

	//do ME test
	if (pthreadArg->meClassId == OMCI_CLASS_ID_IP_HOST_CONFIG_DATA)
	{
		if(pthreadArg->msgFormat == OMCI_BASELINE_MSG)
		{
			ret = omciIPHostCfgDataTestRstMsgHandle(pthreadArg);
			if (ret != 0)
				omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "startTestForIPHost:omciIPHostCfgDataTestRstMsgHandle fail\n");		
		}
		#ifdef TCSUPPORT_OMCI_EXTENDED_MSG			
		else if(pthreadArg->msgFormat == OMCI_EXTENED_MSG)
		{
			ret = omciIPHostCfgDataTestRstMsgHandle_Extended(pthreadArg);
			if (ret != 0)
				omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "startTestForIPHost:omciIPHostCfgDataTestRstMsgHandle fail\n");			
		}
		#endif			
		else
		{
			omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "message format is invalid!\n");
		}
	}
	else 
	{
		omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "startTestForIPHost: unknown me Class Id\n");
	}

	osFree(pthreadArg);
}

int checkInternalClientIp(char *st)
{
	int i = 0, dot = 0;
	int i1 =0 ,i2 = 0 ,i3 = 0, i4 = 0;
	int len = 0;

	if(strcmp(st, "0.0.0.0") == 0)
		return 1;

	len = strlen(st);
	if(len < 7 || len > 15)
		return 0;
	
	for(i = 0; i<len; i++)
	{
		if(((*(st+i)) <= '9')&&((*(st+i)) >= '0'))
		{
			if(dot == 0){
				i1 = i1 *10 + (*(st+i)-'0');
			}
			else if(dot ==1) {
				i2 = i2 *10 + (*(st+i)-'0');
			}
			else if(dot ==2) {
				i3 = i3 *10 + (*(st+i)-'0');
			}		
			else if(dot ==3) {
				i4 = i4 *10 + (*(st+i)-'0');
			}
		}
		else if((*(st+i)) == '.')
			dot++;
		else if((*(st+i)) > '9')//error
			return 0;
		else if((*(st+i)) < '0')//error
			return 0;
	}
	
	//printf("i = %d, dot = %d, i1(%d), i2(%d), i3(%d), i4(%d)\n",i, dot, i1,i2,i3,i4);
	if (dot != 3)
		return 0;

	if(i1 == 0 || i4 == 255 || i1 == 127 )	
		return 0;

	if ((i1>223) || (i2>255) || (i3>255) || (i4>255))
		return 0;

	return 1;
}

int checkSubnetLegal(char *mask)
{	
	uint32 netmask = 0, broadcast = 0;
	int i = 0;

	if(mask == NULL)
		return 0;

	netmask = aton(mask);
	broadcast = aton("255.255.255.255");

	for(i = 0; i < 32; i++) {
		if(netmask & (1 << i))
			break;
		broadcast &= ~(1 << i);
	}

	if(broadcast != netmask) {
		return 0;
	}

	return 1;
}

int32 addIPHostWanTR69(int newPvcIndex_TR69, int newEntryIndex_TR69)
{
	char entryName[32] = {0};
	char nasName[32] = {0};
	char wanPVCNode[32] = {0};
	int i = 0;
	if (newPvcIndex_TR69 != -1 && newEntryIndex_TR69 != -1) { 
		sprintf(wanPVCNode, "Wan_PVC%d", newPvcIndex_TR69);
		sprintf(nasName,IFNAME_FORMAT,newPvcIndex_TR69, newEntryIndex_TR69);
		sprintf(entryName, WANNODE_FORMAT, newPvcIndex_TR69, newEntryIndex_TR69);
		omciNodeInfo_t wanParam_tbl[] = {
			{wanPVCNode,"GPONEnable", "Yes"},
			{wanPVCNode,"EPONEnable", "Yes"},	
			{wanPVCNode,"ATMEnable", "No"},
			{wanPVCNode,"PTMEnable", "No"},
			{wanPVCNode,"ENCAP", "1483 Bridged IP LLC"},
			{wanPVCNode,"VLANMode", "TAG"},
			{wanPVCNode,"VLANID", "200"},
			{wanPVCNode,"DOT1P", "0"},
			{wanPVCNode,"PVCACTIVE", "Yes"},
			{entryName,"ISP", "0"},
			{entryName, "Active", "Yes"},
			{entryName, "ServiceList","TR069"},
			{entryName, "BandActive","N/A"},
			{entryName, "WanMode","Route"},
			{entryName, "LinkMode","linkIP"},
			{entryName, "IPVERSION","IPv4"},
			{entryName, "DHCPOption60",""},
			{entryName, "dot1q","Yes"},
			{entryName, "dot1p","Yes"},
			{entryName, "VLANID", "200"},
			{entryName, "dot1pData", "0"},
			{entryName, "VLANMode", "TAG"},
			{entryName, "NATENABLE","Disabled"},
			{entryName, "IGMPproxy","No"},
			{entryName, "NASName",nasName},
			{entryName, "IFName",nasName},
			{entryName, "DHCPRealy","Yes"},
			{entryName, "DHCPEnable", "1"},
			{entryName, "DHCPv6", "No"},
			{entryName, "DHCPv6PD", "No"},
			{entryName, "MTU", "1500"},
			{entryName, "MulticastVID", ""},
		};
		for(i=0; i<(sizeof(wanParam_tbl)/sizeof(wanParam_tbl[0])); i++){
			if(TCAPI_SUCCESS != tcapi_set(wanParam_tbl[i].nodeName, wanParam_tbl[i].attrName, wanParam_tbl[i].value))
				OMCI_ERROR("addIPHostWanTR69 can't add a new attribute, fail\n");		
		}
		return 0;
	}
	return -1;
}

int32 addIPHostWanVoice(int newPvcIndex_Voice, int newEntryIndex_Voice)
{
	char entryName[32] = {0};
	char nasName[32] = {0};
	char wanPVCNode[32] = {0};
	int i = 0;	
	if (newPvcIndex_Voice != -1 && newEntryIndex_Voice != -1) { 
		//pvc
		sprintf(wanPVCNode, "Wan_PVC%d", newPvcIndex_Voice);
		sprintf(nasName,IFNAME_FORMAT,newPvcIndex_Voice, newEntryIndex_Voice);
		sprintf(entryName, WANNODE_FORMAT, newPvcIndex_Voice, newEntryIndex_Voice);
		omciNodeInfo_t wanParam_tbl[] = {
			//pvc
			{wanPVCNode,"GPONEnable", "Yes"},
			{wanPVCNode,"EPONEnable", "Yes"},	
			{wanPVCNode,"ATMEnable", "No"},
			{wanPVCNode,"PTMEnable", "No"},
			{wanPVCNode,"ENCAP", "1483 Bridged IP LLC"},
			{wanPVCNode,"VLANMode", "TAG"},
			{wanPVCNode,"VLANID", "99"},
			{wanPVCNode,"DOT1P", "0"},
			{wanPVCNode,"PVCACTIVE", "Yes"},
			//entry
			{entryName, "Active", "Yes"},
			{entryName, "ServiceList","VOICE"},
			{entryName, "BandActive","N/A"},
			{entryName, "LAN1","No"},
			{entryName, "LAN2","No"},
			{entryName, "WanMode","Route"},
			{entryName, "LinkMode","linkIP"},
			{entryName, "MTU", "1500"},
			{entryName, "IPVERSION","IPv4"},
			{entryName, "ISP", "1"},
			{entryName, "VLANMode", "TAG"},
		//	{entryName, "DHCPOption60",""},
			{entryName, "dot1q","Yes"},
			{entryName, "dot1p","Yes"},
			{entryName, "VLANID", "99"},
			{entryName, "dot1pData", "0"},
			{entryName, "NATENABLE","Disabled"},
			{entryName, "IGMPproxy","No"},
			{entryName, "IFIdx","4"},
			{entryName, "DsliteEnable","No"},
			{entryName, "PDEnable","No"},
			{entryName, "DHCPv6PD", "No"},
			{entryName, "PDOrigin", "None"},
			{entryName, "DHCPEnable", "0"},
			{entryName, "BridgeInterface", "No"},
			{entryName, "NASName",nasName},
			{entryName, "IFName",nasName},		
			{IPHOST_NODE, "WanIf", nasName},
		//	{entryName, "DHCPRealy","Yes"},		
		//	{entryName, "DHCPv6", "No"},
		//	{entryName, "MulticastVID", ""},		
		};
		for(i=0; i<(sizeof(wanParam_tbl)/sizeof(wanParam_tbl[0])); i++){
			if(TCAPI_SUCCESS != tcapi_set(wanParam_tbl[i].nodeName, wanParam_tbl[i].attrName, wanParam_tbl[i].value))
				OMCI_ERROR("addIPHostWanVoice can't add a new attribute, fail\n");
		}
		return 0;
	}
	return -1;
}
int32 getIPHostWanInterface(char* ifName, char noCreate)
{
	char buffer[32] = {0};
	char nodeName[32] = {0};

	int pvcIndex = 0;
	int entryIndex = 0;
	char wanMode[8] = {0};
	char serviceList[16] = {0};

#if defined(TCSUPPORT_TR69_IP_HOST)
	int newPvcIndex_TR69 = -1, newEntryIndex_TR69 = -1;
#endif

#if defined(TCSUPPORT_PON_IP_HOST_AUTOWAN_VOICE)
	int newPvcIndex_Voice = -1, newEntryIndex_Voice = -1;
#endif

	memset(nodeName, 0, sizeof(nodeName));
	memset(buffer, 0, sizeof(buffer));

	tcapi_get(IPHOST_NODE, "WanIf", buffer);

	if(strlen(buffer) != 0){
		omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "getIPHostWanInterface: WanIf %s\n", buffer);
		strcpy(ifName, buffer);
		return 0;
	}

#if defined(TCSUPPORT_CT)
	// just find one interface which only support voip service 
	for(pvcIndex = 0; pvcIndex < PVC_NUM; pvcIndex++){
		for(entryIndex = 0; entryIndex < MAX_SMUX_NUM; entryIndex++){
			memset(nodeName, 0, sizeof(nodeName));
			sprintf(nodeName, "%s_%s%d_%s%d", WAN, WAN_PVC, pvcIndex, WAN_INTERFACE, entryIndex);			
			memset(buffer, 0, sizeof(buffer));

			if(tcapi_get(nodeName, "WanMode", wanMode) != 0 
				|| tcapi_get(nodeName, "ServiceList", serviceList) != 0) {
		#if  defined(TCSUPPORT_TR69_IP_HOST)
				if (newPvcIndex_TR69 == -1 && newEntryIndex_TR69 == -1) { 
					newPvcIndex_TR69 = pvcIndex;
					newEntryIndex_TR69 = entryIndex;
				}
		#endif
		#if  defined(TCSUPPORT_PON_IP_HOST_AUTOWAN_VOICE)
				if (newPvcIndex_Voice == -1 && newEntryIndex_Voice == -1) { 
					newPvcIndex_Voice = pvcIndex;
					newEntryIndex_Voice = entryIndex;
				}
		#endif
				continue;
			}
	#if !defined(TCSUPPORT_TR69_IP_HOST)
			if(strcmp(wanMode, "Route") != 0 || strcmp(serviceList, "VOICE") != 0)
					continue;
	#else
			if((strcmp(wanMode, "Route") != 0) || (strstr(serviceList, "TR069") == NULL))
					continue;
	#endif

			sprintf(ifName, IFNAME_FORMAT, pvcIndex, entryIndex);
			return 0;
		}
	}
    
    if (noCreate)
        return 0;

#if defined(TCSUPPORT_TR69_IP_HOST)//if not found tr69 wan, add new wan
	if(addIPHostWanTR69(newPvcIndex_TR69, newEntryIndex_TR69) == 0){	
		sprintf(ifName, IFNAME_FORMAT, newPvcIndex_TR69, newEntryIndex_TR69);
		return 0;
	}
	else{
		OMCI_ERROR("addIPHostWanTR69 can't add a new wan, fail\n");
	}
#endif

#if defined(TCSUPPORT_PON_IP_HOST_AUTOWAN_VOICE)//if not found ip_host_voice wan, add new wan
	if(addIPHostWanVoice(newPvcIndex_Voice, newEntryIndex_Voice) == 0){	
		sprintf(ifName, IFNAME_FORMAT, newPvcIndex_Voice, newEntryIndex_Voice);
		return 0;
	}
	else{
		OMCI_ERROR("addIPHostWanVoice can't add a new wan, fail\n");
	}
#endif
	
#endif
	return -1;
}

int ifNameToNodeName(char *ifName, char *nodeName){
#if defined(TCSUPPORT_CT)
	int pvcIndex = -1;
	int entryIndex = -1;
	
	sscanf(ifName, IFNAME_FORMAT, &pvcIndex, &entryIndex);
	if(pvcIndex < 0 || pvcIndex >= PVC_NUM
		|| entryIndex < 0 || entryIndex > MAX_SMUX_NUM)
		return -1;
	
	sprintf(nodeName, WANNODE_FORMAT, pvcIndex, entryIndex);
#else
	int pvcIndex = -1;

	sscanf(ifName, IFNAME_FORMAT, &pvcIndex);
	if(pvcIndex < 0 || pvcIndex >= PVC_NUM)
		return -1;

	sprintf(nodeName, WANNODE_FORMAT, pvcIndex);
#endif
	return 0;
}
	
int omciCommitIPHostConfigData(uint16 meClassId , omciMeInst_t *meInstant_ptr)
{
	char commitWan[8] = {0};
	char ifName[16] = {0};
	char nodeName[32] = {0};
	
	if (meInstant_ptr == NULL)
		return -1;
	
	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "omciCommitIPHostConfigData\n");

	tcapi_get(IPHOST_NODE, COMMIT_WAN_ATTR, commitWan);
	
	if(strcmp(commitWan, "0") != 0){
		if(getIPHostWanInterface(ifName,0) != 0){
			omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "omciCommitIPHostConfigData can't find ip host wanif, fail\n");
			return -1;
		}
		
		if(ifNameToNodeName(ifName, nodeName) != 0){
			omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "omciCommitIPHostConfigData ifNameToNodeName error, fail\n");
			return -1;
		}
 		if(delayOmciCommitNode(nodeName) !=0){
			OMCI_ERROR("addOmciCommitNodeList and start the Timer fail\n");
			return -1;  
		}	
		tcapi_set(IPHOST_NODE, COMMIT_WAN_ATTR, "0");
	}
	
	return 0;
}

char* getWanInfo(int type, char* ifName, char* buf){
	FILE *fp = NULL;
	char tmpBuf[512];
	char cmd[64];
	char macInfo[2][18];
	char *pValue = NULL;
#if defined(TCSUPPORT_CT)
	char fileName[32];
#else
	char nodeName[32] = {0};
	char attr[32] = {0};
	int pvcIndex = -1;
#endif
	
	if(ifName == NULL || buf == NULL)
		return NULL;
	
	strcpy(buf, "");

	// get mac info
	if(type == MAC_ADDR){
		memset(cmd, 0, sizeof(cmd));
		sprintf(cmd,"ifconfig %s > %s", ifName, TMP_IF_PATH);
		system(cmd);
		
		fp = fopen(TMP_IF_PATH, "r");
		if(fp == NULL)
			return buf;
		
		memset(macInfo,0, sizeof(macInfo));
		while (fgets (tmpBuf, sizeof(tmpBuf), fp) != NULL){
			if(pValue = strstr(tmpBuf,"HWaddr"))
				sscanf(pValue,"%s %s", macInfo[0],macInfo[1]); //format: HWaddr 00:AA:BB:01:23:45
		}
		
		if(strlen(macInfo[1]) > 0)
			strcpy(buf, macInfo[1]);
	
		fclose(fp);
		unlink(TMP_IF_PATH);
		return buf;
	}

#if defined(TCSUPPORT_CT)
	//get wan ip info
	memset(fileName, 0, sizeof(fileName));
	sprintf(fileName, "%s%s/", WAN_IF_PATH, ifName);

	switch(type){
		case IP_ADDR:
			strcat(fileName, "ip");
			break;
		case NETMASK:
			strcat(fileName, "netmask");
			break;
		case GATEWAY:
			strcat(fileName, "gateway");
			break;
		case PRI_DNS:
		case SEC_DNS:
			strcat(fileName, "dns");
			break;
		default:
			return buf;
	}

	fp=fopen(fileName, "r");
	if(fp == NULL)	
		return buf;


	memset(tmpBuf, 0, sizeof(tmpBuf));
	fgets(tmpBuf, 512, fp); //get line

	if(type == SEC_DNS){
		memset(tmpBuf, 0, sizeof(tmpBuf));
		fgets(tmpBuf, 512, fp); //get next line		
	}

	if(tmpBuf[0] != '\n'){
		strtok(tmpBuf, "\n");
		if(strlen(tmpBuf) >= 7 && strlen(tmpBuf) <= 15) //ip addr format xxx.xxx.xx.xxx
			strcpy(buf, tmpBuf);
	}

	fclose(fp);
	
#else

	switch(type){
		case IP_ADDR:
			strcat(attr, "WanIP");
			break;
		case NETMASK:
			strcat(attr, "WanSubMask");
			break;
		case GATEWAY:
			strcat(attr, "WanDefGW");
			break;
		case PRI_DNS:
			strcat(attr, "DNSIP");
			break;	
		case SEC_DNS:
			strcat(attr, "SECDNSIP");
			break;
		default:
			return buf;
	}

	sscanf(ifName, IFNAME_FORMAT, &pvcIndex);
	if(pvcIndex < 0 || pvcIndex >= PVC_NUM)
		return buf;

	sprintf(nodeName, WANDEV_INFO_FORMAT, pvcIndex);
	memset(tmpBuf, 0, sizeof(tmpBuf));
	tcapi_get(nodeName, attr, tmpBuf);

	if(strlen(tmpBuf) >= 7 && strlen(tmpBuf) <= 15) //ip addr format xxx.xxx.xx.xxx
		strcpy(buf, tmpBuf);
#endif

	return buf;
}

char tr69wan[32];

int setWanInfo(int type, char* buf){

	char ifName[32] = {0};
	char nodeName[32] = {0};
	char attr[16] = {0};
	char oldValue[32] = {0};
	
	if(ifName == NULL || buf == NULL)
		return -1;

	if(getIPHostWanInterface(ifName,0) != 0){
		omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "setIPHostConfigDataIPOptions can't find ip host wanif, fail\n");
		return -1;
	}

	if(ifNameToNodeName(ifName, nodeName) != 0){
		omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "setIPHostConfigDataIPAddr ifName to nodeName error, fail\n");
		return -1;
	}
	
#if  defined(TCSUPPORT_TR69_IP_HOST)
	strcpy(tr69wan, nodeName);
#endif
	
	switch(type){
		case ISP:
			strcpy(attr, "ISP");
			break;
		case IP_ADDR:
			strcpy(attr, "IPADDR");
			break;
		case NETMASK:
			strcpy(attr, "NETMASK");
			break;

		case GATEWAY:
			strcpy(attr, "GATEWAY");
			break;

#if defined(TCSUPPORT_CT)
		case PRI_DNS:
			strcpy(attr, "DNS");
			break;
		case SEC_DNS:
			strcpy(attr, "SecDNS");
			break;
#else
		case PRI_DNS:
			strcpy(attr, "PRIMARYDNS");
			break;
		case SEC_DNS:
			strcpy(attr, "SECONDARYDNS");
			break;
#endif
		case VLANID:
			strcpy(attr, "VLANID");
			break;
		case DOT1P:
			strcpy(attr, "dot1pData");
			break;
		default:
			return -1;
	}

#if defined(TCSUPPORT_PON_IP_HOST_AUTOWAN_VOICE)
	if(strcmp(attr, "ISP") == 0 && strcmp(buf, "0") == 0){/*DHCP mode*/
		tcapi_set(nodeName, "DHCPEnable", "1");
		tcapi_set(nodeName, attr, buf);
		tcapi_set(IPHOST_NODE, COMMIT_WAN_ATTR, "1");
	}
	else{
		tcapi_get(nodeName, attr, oldValue);
		if(strcmp(oldValue, buf) != 0){ /* value change*/
			tcapi_set(nodeName, attr, buf);
			if(strcmp(attr, "ISP") != 0){
				memset(oldValue, 0, sizeof(oldValue));
				tcapi_get(nodeName, "ISP", oldValue);
				if(strcmp(oldValue, "1") == 0){	/*static ip mode*/
					tcapi_set(IPHOST_NODE, COMMIT_WAN_ATTR, "1");
				}
			}		
		}
	}
#else
	tcapi_get(nodeName, attr, oldValue);
	if(strcmp(oldValue, buf) != 0){ /* value change*/
		tcapi_set(nodeName, attr, buf);
		if(strcmp(attr, "ISP") != 0){
			tcapi_get(nodeName, "ISP", oldValue);
			if(strcmp(oldValue, "1") == 0){	//static ip mode
				tcapi_set(IPHOST_NODE, COMMIT_WAN_ATTR, "1");
			}
		}	
		else{
			tcapi_set(IPHOST_NODE, COMMIT_WAN_ATTR, "1");
		}
	}
#endif

	return 0;
}

int32 setIPHostConfigDataIPOptions(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	uint8 *attributeValuePtr = NULL;
	omciMeInst_t *tmpomciMeInst_ptr = NULL;
	
	uint8 ipOptions = 0;
	
	char isp[8] = {0};
	uint16 length = 0;
	char cmd[256];
	char ifName[16] = {0};
	
	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;
	
	ipOptions = * (uint8 *)value;
	
	if(ipOptions & IP_OPTION_DHCP_MASK){ /*DHCP*/
		strcpy(isp, "0");
	}
	else {/*Static IP*/
		strcpy(isp, "1");
	}
	setWanInfo(ISP, isp);

	if(getIPHostWanInterface(ifName,0) != 0){
		omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "setIPHostConfigDataIPOptions can't find ip host wanif, fail\n");
		return -1;
	}

	if(ipOptions & IP_OPTION_ENABLE_PING_RESPONSE_MASK){/*ping response*/
		sprintf(cmd, "/usr/bin/iptables -D OUTPUT -p icmp --icmp-type 0 -o %s -j DROP", ifName);
		system(cmd);
	}
	else{//Static IP
		sprintf(cmd, "/usr/bin/iptables -D OUTPUT -p icmp --icmp-type 0 -o %s -j DROP", ifName);
		system(cmd);
		
		sprintf(cmd, "/usr/bin/iptables -A OUTPUT -p icmp --icmp-type 0 -o %s -j DROP", ifName);
		system(cmd);
	}

	if(ipOptions & IP_OPTION_ENABLE_TRACERT_RESPONSE_MASK){ /*trace route response*/
		sprintf(cmd, "/usr/bin/iptables -D OUTPUT -p icmp --icmp-type 11 -o %s -j DROP", ifName);
		system(cmd);
		
		sprintf(cmd, "/usr/bin/iptables -D INPUT -p udp --dport 32768: -i %s -j DROP", ifName);
		system(cmd);
	}
	else{/*Static IP*/
		sprintf(cmd, "/usr/bin/iptables -D OUTPUT -p icmp --icmp-type 11 -o %s -j DROP", ifName);
		system(cmd);
		
		sprintf(cmd, "/usr/bin/iptables -A OUTPUT -p icmp --icmp-type 11 -o %s -j DROP", ifName);
		system(cmd);

		sprintf(cmd, "/usr/bin/iptables -D INPUT -p udp --dport 32768: -i %s -j DROP", ifName);
		system(cmd);
	
		sprintf(cmd, "/usr/bin/iptables -A INPUT -p udp --dport 32768: -i %s -j DROP", ifName);
		system(cmd);
	}

	/*set attr*/
	attributeValuePtr = omciGetInstAttriByName(meInstantPtr, omciAttribute->attriName, &length);
	if(attributeValuePtr == NULL)
		return -1;
	memcpy(attributeValuePtr, value, length);

	return 0;
}


int32 getIPHostConfigDataMACAddr(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	uint8 *attributeValuePtr = NULL;
	omciMeInst_t *tmpomciMeInst_ptr = NULL;
	int mac[8] = {0};
	char tmpMAC[32] = {0};
	int i = 0;
	uint16 length = 0;

#ifdef TCSUPPORT_PON_IP_HOST
	char ifName[16] = {0};
#endif

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;
	
#ifdef TCSUPPORT_PON_IP_HOST
	if(getIPHostWanInterface(ifName,1) != 0){
		omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "setIPHostConfigDataIPOptions can't find ip host wanif, fail\n");
		return -1;
	}
	
	if(getWanInfo(MAC_ADDR, ifName, tmpMAC) == NULL)

#else
	if(get_profile_str("LAN_MAC=", tmpMAC, sizeof(tmpMAC), 0, IP_HOST_MAC_PATH) == -1)
#endif
		return -1;
	else
	{						
		sscanf(tmpMAC, "%02x:%02x:%02x:%02x:%02x:%02x", &mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
		memset(tmpMAC, 0, sizeof(tmpMAC));
		for(i=0; i<6; i++)
		{
			tmpMAC[i] = (unsigned char)mac[i];
		}
	}

	tmpomciMeInst_ptr = (omciMeInst_ptr)meInstantPtr;
	attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , omciAttribute->attriName, &length);
	if(attributeValuePtr == NULL)
		return -1;
	memcpy(attributeValuePtr, tmpMAC, length);

	return getTheValue(value, (char *)attributeValuePtr, 6, omciAttribute);
}

int32 getIPHostConfigDataIPAddr(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	uint8 *attributeValuePtr = NULL;
	omciMeInst_t *tmpomciMeInst_ptr = NULL;
	//int ret = 0;
	//char ipAddr[32] = {0};
	//struct in_addr address;
	uint16 length = 0;

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpomciMeInst_ptr = (omciMeInst_ptr)meInstantPtr;
/*
	ret = tcapi_get("Lan_Entry0", "IP", ipAddr);
	if(ret == -1)
		return ret;
	
	memset(&address, 0, sizeof(address));
	inet_aton(ipAddr, &address);
*/	
	attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , omciAttribute->attriName, &length);
	if(attributeValuePtr == NULL)
		return -1;
//	put32(attributeValuePtr, address.s_addr);
	
	return getTheValue(value, (char *)attributeValuePtr, 4, omciAttribute);
}

int32 setIPHostConfigDataIPAddr(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	uint8 *attributeValuePtr = NULL;
	omciMeInst_t *tmpomciMeInst_ptr = NULL;
	char ipAddr[32] = {0};
	struct in_addr address;
	uint32 tmpAddr = 0;
	uint16 length = 0;

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpAddr = get32((uint8 *)value);
	memset(&address, 0, sizeof(address));
	address.s_addr = tmpAddr;
	strcpy(ipAddr, (char *)inet_ntoa(address));
	if(checkInternalClientIp(ipAddr) == 0)
		return -1;

#ifdef TCSUPPORT_PON_IP_HOST
	setWanInfo(IP_ADDR, ipAddr);
#else
	//tcapi_set("Lan_Entry0", "IP", ipAddr);
	//tcapi_commit("Lan_Entry0");
	//tcapi_save();
#endif
	
	tmpomciMeInst_ptr = (omciMeInst_ptr)meInstantPtr;
	attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , omciAttribute->attriName, &length);
	if(attributeValuePtr == NULL)
		return -1;
	memcpy(attributeValuePtr, value, length);
	
	return 0;
}

int32 getIPHostConfigDataMask(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	uint8 *attributeValuePtr = NULL;
	omciMeInst_t *tmpomciMeInst_ptr = NULL;
	//int ret = 0;
	//char mask[32] = {0};
	//struct in_addr address;
	uint16 length = 0;

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpomciMeInst_ptr = (omciMeInst_ptr)meInstantPtr;
/*
	ret = tcapi_get("Lan_Entry0", "netmask", mask);
	if(ret == -1)
		return ret;
	
	memset(&address, 0, sizeof(address));
	inet_aton(mask, &address);
*/	
	attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , omciAttribute->attriName, &length);
	if(attributeValuePtr == NULL)
		return -1;
//	put32(attributeValuePtr, address.s_addr);
	
	return getTheValue(value, (char *)attributeValuePtr, 4, omciAttribute);
}

int32 setIPHostConfigDataMask(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	uint8 *attributeValuePtr = NULL;
	omciMeInst_t *tmpomciMeInst_ptr = NULL;
	char mask[32] = {0};
	struct in_addr address;
	uint32 tmpAddr = 0;
	uint16 length = 0;

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpAddr = get32((uint8 *)value);
	memset(&address, 0, sizeof(address));
	address.s_addr = tmpAddr;
	strcpy(mask, (char *)inet_ntoa(address));
	if(checkSubnetLegal(mask) == 0)
		return -1;

#ifdef TCSUPPORT_PON_IP_HOST
	setWanInfo(NETMASK, mask);
#else
	//tcapi_set("Lan_Entry0", "netmask", mask);
	//tcapi_commit("Lan_Entry0");
	//tcapi_save();
#endif
	
	tmpomciMeInst_ptr = (omciMeInst_ptr)meInstantPtr;
	attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , omciAttribute->attriName, &length);
	if(attributeValuePtr == NULL)
		return -1;
	memcpy(attributeValuePtr, value, length);
	
	return 0;
}

int32 setIPHostConfigDataGateway(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	uint8 *attributeValuePtr = NULL;
	omciMeInst_t *tmpomciMeInst_ptr = NULL;
	char addr[32] = {0};
	struct in_addr address;
	uint32 tmpAddr = 0;
	uint16 length = 0;

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpAddr = get32((uint8 *)value);
	memset(&address, 0, sizeof(address));
	address.s_addr = tmpAddr;
	strcpy(addr, (char *)inet_ntoa(address));
	if(checkInternalClientIp(addr) == 0)
		return -1;
	
#ifdef TCSUPPORT_PON_IP_HOST
	setWanInfo(GATEWAY, addr);
#endif

	tmpomciMeInst_ptr = (omciMeInst_ptr)meInstantPtr;
	attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , omciAttribute->attriName, &length);
	if(attributeValuePtr == NULL)
		return -1;
	memcpy(attributeValuePtr, value, length);
	
	return 0;
}

int32 setIPHostConfigDataPrimaryDNS(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	uint8 *attributeValuePtr = NULL;
	omciMeInst_t *tmpomciMeInst_ptr = NULL;
	char addr[32] = {0};
	struct in_addr address;
	uint32 tmpAddr = 0;
	uint16 length = 0;

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpAddr = get32((uint8 *)value);
	memset(&address, 0, sizeof(address));
	address.s_addr = tmpAddr;
	strcpy(addr, (char *)inet_ntoa(address));
	if(checkInternalClientIp(addr) == 0)
		return -1;
	
#ifdef TCSUPPORT_PON_IP_HOST
	setWanInfo(PRI_DNS, addr);
#endif

	tmpomciMeInst_ptr = (omciMeInst_ptr)meInstantPtr;
	attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , omciAttribute->attriName, &length);
	if(attributeValuePtr == NULL)
		return -1;
	memcpy(attributeValuePtr, value, length);
	
	return 0;
}

int32 setIPHostConfigDataSecondaryDNS(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	uint8 *attributeValuePtr = NULL;
	omciMeInst_t *tmpomciMeInst_ptr = NULL;
	char addr[32] = {0};
	struct in_addr address;
	uint32 tmpAddr = 0;
	uint16 length = 0;

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpAddr = get32((uint8 *)value);
	memset(&address, 0, sizeof(address));
	address.s_addr = tmpAddr;
	strcpy(addr, (char *)inet_ntoa(address));
	if(checkInternalClientIp(addr) == 0)
		return -1;
	
#ifdef TCSUPPORT_PON_IP_HOST
	setWanInfo(SEC_DNS, addr);
#endif

	tmpomciMeInst_ptr = (omciMeInst_ptr)meInstantPtr;
	attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , omciAttribute->attriName, &length);
	if(attributeValuePtr == NULL)
		return -1;
	memcpy(attributeValuePtr, value, length);
	
	return 0;
}

int32 getIPHostConfigDataCurAddr(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	uint8 *attributeValuePtr = NULL;
	omciMeInst_t *tmpomciMeInst_ptr = NULL;
	omciManageEntity_ptr currentME = NULL;
	omciMeInst_ptr meInstantCurPtr = NULL;
	omciAttriDescript_ptr currentAttribute = NULL;
	uint16 instanceId = 0;
	uint16 length = 0;

#ifdef TCSUPPORT_PON_IP_HOST
	char ifName[16] = {0};
	char ipAddr[32] = {0};
	struct in_addr address;
#endif

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpomciMeInst_ptr = (omciMeInst_ptr)meInstantPtr;
	instanceId = get16(tmpomciMeInst_ptr->attributeVlaue_ptr);
	currentME = omciGetMeByClassId(OMCI_CLASS_ID_IP_HOST_CONFIG_DATA);
	if(currentME == NULL)
		return -1;
	meInstantCurPtr = omciGetInstanceByMeId(currentME , instanceId);
	if(meInstantCurPtr == NULL)
		return -1;
	currentAttribute = &currentME->omciAttriDescriptList[1];
	attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , currentAttribute->attriName, &length);
	if(attributeValuePtr == NULL)
		return -1;

	if(((*attributeValuePtr)&IP_OPTION_DHCP_MASK) == 0)//DHCP disable, curAddr == addr
	{
		currentAttribute = &currentME->omciAttriDescriptList[4];
		attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , currentAttribute->attriName, &length);
		if(attributeValuePtr == NULL)
			return -1;
		return getTheValue(value, (char *)attributeValuePtr, 4, omciAttribute);
	}
	else//DHCP enable, curAddr is assigned by DHCP, if not,  undefined
	{
#ifdef TCSUPPORT_PON_IP_HOST
		if(getIPHostWanInterface(ifName,1) != 0){
			omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "setIPHostConfigDataSecondaryDNS can't find ip host wanif, fail\n");
			return -1;
		}

		getWanInfo(IP_ADDR, ifName, ipAddr);

		memset(&address, 0, sizeof(address));
		inet_aton(ipAddr, &address);

		put32(value, address.s_addr);		
		return 0;
		
#else
		attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , omciAttribute->attriName, &length);
		if(attributeValuePtr == NULL)
			return -1;
		return getTheValue(value, (char *)attributeValuePtr, 4, omciAttribute);
#endif

	}
	
	return getTheValue(value, (char *)attributeValuePtr, 4, omciAttribute);
}

int32 getIPHostConfigDataCurMask(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	uint8 *attributeValuePtr = NULL;
	omciMeInst_t *tmpomciMeInst_ptr = NULL;
	omciManageEntity_ptr currentME = NULL;
	omciMeInst_ptr meInstantCurPtr = NULL;
	omciAttriDescript_ptr currentAttribute = NULL;
	uint16 instanceId = 0;
	uint16 length = 0;

#ifdef TCSUPPORT_PON_IP_HOST
	char ifName[16] = {0};
	char ipAddr[32] = {0};
	struct in_addr address;
	uint32 temp;
#endif

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpomciMeInst_ptr = (omciMeInst_ptr)meInstantPtr;
	instanceId = get16(tmpomciMeInst_ptr->attributeVlaue_ptr);
	currentME = omciGetMeByClassId(OMCI_CLASS_ID_IP_HOST_CONFIG_DATA);
	if(currentME == NULL)
		return -1;
	meInstantCurPtr = omciGetInstanceByMeId(currentME , instanceId);
	if(meInstantCurPtr == NULL)
		return -1;
	currentAttribute = &currentME->omciAttriDescriptList[1];
	attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , currentAttribute->attriName, &length);
	if(attributeValuePtr == NULL)
		return -1;

	if(((*attributeValuePtr)&IP_OPTION_DHCP_MASK) == 0)//DHCP disable, curAddr == addr
	{
		currentAttribute = &currentME->omciAttriDescriptList[5];
		attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , currentAttribute->attriName, &length);
		if(attributeValuePtr == NULL)
			return -1;	
		return getTheValue(value, (char *)attributeValuePtr, 4, omciAttribute);
	}
	else//DHCP enable, curAddr is assigned by DHCP, if not,  undefined
	{
#ifdef TCSUPPORT_PON_IP_HOST
		if(getIPHostWanInterface(ifName,1) != 0){
			omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "setIPHostConfigDataSecondaryDNS can't find ip host wanif, fail\n");
			return -1;
		}

		getWanInfo(NETMASK, ifName, ipAddr);
		
#if 0
		if(strlen(ipAddr) <= 0){
			omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "getIPHostConfigDataIPAddr can't find ip host wanif IP, fail\n");
			return -1;
		}
#endif		
		memset(&address, 0, sizeof(address));
		inet_aton(ipAddr, &address);
		
		put32(value, address.s_addr);
		
		return 0;
#else
		attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , omciAttribute->attriName, &length);
		if(attributeValuePtr == NULL)
			return -1;
		return getTheValue(value, (char *)attributeValuePtr, 4, omciAttribute);
#endif
	}
	
	return getTheValue(value, (char *)attributeValuePtr, 4, omciAttribute);
}

int32 getIPHostConfigDataCurGateway(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	uint8 *attributeValuePtr = NULL;
	omciMeInst_t *tmpomciMeInst_ptr = NULL;
	omciManageEntity_ptr currentME = NULL;
	omciMeInst_ptr meInstantCurPtr = NULL;
	omciAttriDescript_ptr currentAttribute = NULL;
	uint16 instanceId = 0;
	uint16 length = 0;
#ifdef TCSUPPORT_PON_IP_HOST
	char ifName[16] = {0};
	char ipAddr[32] = {0};
	struct in_addr address;
#endif 

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpomciMeInst_ptr = (omciMeInst_ptr)meInstantPtr;
	instanceId = get16(tmpomciMeInst_ptr->attributeVlaue_ptr);
	currentME = omciGetMeByClassId(OMCI_CLASS_ID_IP_HOST_CONFIG_DATA);
	if(currentME == NULL)
		return -1;
	meInstantCurPtr = omciGetInstanceByMeId(currentME , instanceId);
	if(meInstantCurPtr == NULL)
		return -1;
	currentAttribute = &currentME->omciAttriDescriptList[1];
	attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , currentAttribute->attriName, &length);
	if(attributeValuePtr == NULL)
		return -1;

	if(((*attributeValuePtr)&IP_OPTION_DHCP_MASK) == 0)//DHCP disable, curAddr == addr
	{
		currentAttribute = &currentME->omciAttriDescriptList[6];
		attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , currentAttribute->attriName, &length);
		if(attributeValuePtr == NULL)
			return -1;
		
		return getTheValue(value, (char *)attributeValuePtr, 4, omciAttribute);
	}
	else//DHCP enable, curAddr is assigned by DHCP, if not,  undefined
	{
#ifdef TCSUPPORT_PON_IP_HOST
		if(getIPHostWanInterface(ifName,1) != 0){
			omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "setIPHostConfigDataSecondaryDNS can't find ip host wanif, fail\n");
			return -1;
		}
		
		getWanInfo(GATEWAY, ifName, ipAddr);
		
#if 0
		if(strlen(ipAddr) <= 0){
			omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "getIPHostConfigDataIPAddr can't find ip host wanif IP, fail\n");
			return -1;
		}
#endif	

		memset(&address, 0, sizeof(address));
		inet_aton(ipAddr, &address);
			
		put32(value, address.s_addr);
		return 0;
#else
		attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , omciAttribute->attriName, &length);
		if(attributeValuePtr == NULL)
			return -1;
#endif
		
		return getTheValue(value, (char *)attributeValuePtr, 4, omciAttribute);
	}
	
	return getTheValue(value, (char *)attributeValuePtr, 4, omciAttribute);
}

int32 getIPHostConfigDataCurPrimaryDNS(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	uint8 *attributeValuePtr = NULL;
	omciMeInst_t *tmpomciMeInst_ptr = NULL;
	omciManageEntity_ptr currentME = NULL;
	omciMeInst_ptr meInstantCurPtr = NULL;
	omciAttriDescript_ptr currentAttribute = NULL;
	uint16 instanceId = 0;
	uint16 length = 0;
#ifdef TCSUPPORT_PON_IP_HOST
	char ifName[16] = {0};
	char ipAddr[32] = {0};
	struct in_addr address;
#endif

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpomciMeInst_ptr = (omciMeInst_ptr)meInstantPtr;
	instanceId = get16(tmpomciMeInst_ptr->attributeVlaue_ptr);
	currentME = omciGetMeByClassId(OMCI_CLASS_ID_IP_HOST_CONFIG_DATA);
	if(currentME == NULL)
		return -1;
	meInstantCurPtr = omciGetInstanceByMeId(currentME , instanceId);
	if(meInstantCurPtr == NULL)
		return -1;
	currentAttribute = &currentME->omciAttriDescriptList[1];
	attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , currentAttribute->attriName, &length);
	if(attributeValuePtr == NULL)
		return -1;

	if(((*attributeValuePtr)&IP_OPTION_DHCP_MASK) == 0)//DHCP disable, curAddr == addr
	{
		currentAttribute = &currentME->omciAttriDescriptList[7];
		attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , currentAttribute->attriName, &length);
		if(attributeValuePtr == NULL)
			return -1;
		
		return getTheValue(value, (char *)attributeValuePtr, 4, omciAttribute);
	}
	else//DHCP enable, curAddr is assigned by DHCP, if not,  undefined
	{
#ifdef TCSUPPORT_PON_IP_HOST
		if(getIPHostWanInterface(ifName,1) != 0){
			omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "setIPHostConfigDataSecondaryDNS can't find ip host wanif, fail\n");
			return -1;
		}
		
		getWanInfo(PRI_DNS, ifName, ipAddr);
		
#if 0
		if(strlen(ipAddr) <= 0){
			omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "getIPHostConfigDataIPAddr can't find ip host wanif IP, fail\n");
			return -1;
		}
#endif		
		memset(&address, 0, sizeof(address));
		inet_aton(ipAddr, &address);
			
		put32(value, address.s_addr);
		return 0;
#else
		attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , omciAttribute->attriName, &length);
		if(attributeValuePtr == NULL)
			return -1;
#endif

		return getTheValue(value, (char *)attributeValuePtr, 4, omciAttribute);
	}
	
	return getTheValue(value, (char *)attributeValuePtr, 4, omciAttribute);
}

int32 getIPHostConfigDataCurSecondaryDNS(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	uint8 *attributeValuePtr = NULL;
	omciMeInst_t *tmpomciMeInst_ptr = NULL;
	omciManageEntity_ptr currentME = NULL;
	omciMeInst_ptr meInstantCurPtr = NULL;
	omciAttriDescript_ptr currentAttribute = NULL;
	uint16 instanceId = 0;
	uint16 length = 0;
#ifdef TCSUPPORT_PON_IP_HOST
	char ifName[16] = {0};
	char ipAddr[32] = {0};
	struct in_addr address;
#endif

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpomciMeInst_ptr = (omciMeInst_ptr)meInstantPtr;
	instanceId = get16(tmpomciMeInst_ptr->attributeVlaue_ptr);
	currentME = omciGetMeByClassId(OMCI_CLASS_ID_IP_HOST_CONFIG_DATA);
	if(currentME == NULL)
		return -1;
	meInstantCurPtr = omciGetInstanceByMeId(currentME , instanceId);
	if(meInstantCurPtr == NULL)
		return -1;
	currentAttribute = &currentME->omciAttriDescriptList[1];
	attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , currentAttribute->attriName, &length);
	if(attributeValuePtr == NULL)
		return -1;

	if(((*attributeValuePtr)&IP_OPTION_DHCP_MASK) == 0)//DHCP disable, curAddr == addr
	{
		currentAttribute = &currentME->omciAttriDescriptList[8];
		attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , currentAttribute->attriName, &length);
		if(attributeValuePtr == NULL)
			return -1;
		
		return getTheValue(value, (char *)attributeValuePtr, 4, omciAttribute);
	}
	else//DHCP enable, curAddr is assigned by DHCP, if not,  undefined
	{
	
#ifdef TCSUPPORT_PON_IP_HOST
		if(getIPHostWanInterface(ifName,1) != 0){
			omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "setIPHostConfigDataSecondaryDNS can't find ip host wanif, fail\n");
			return -1;
		}
			
		getWanInfo(SEC_DNS, ifName, ipAddr);
#if 0
		if(strlen(ipAddr) <= 0){
			omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "getIPHostConfigDataIPAddr can't find ip host wanif IP, fail\n");
			return -1;
		}
#endif		
		memset(&address, 0, sizeof(address));
		inet_aton(ipAddr, &address);
				
		put32(value, address.s_addr);
		return 0;
#else
		attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_ptr , omciAttribute->attriName, &length);
		if(meInstantCurPtr == NULL)
			return -1;
#endif
	
		return getTheValue(value, (char *)attributeValuePtr, 4, omciAttribute);
	}
	
	return getTheValue(value, (char *)attributeValuePtr, 4, omciAttribute);
}


/*******************************************************************************************************************************
9.4.2: IP host performance monitoring history data

********************************************************************************************************************************/
int omciMeInitForIPHostPM(omciManageEntity_t *omciManageEntity_p)
{
	omciManageEntity_t *tmpOmciManageEntity_p = NULL;
	
	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n enter omciMeInitForIPHostPM \r\n");

	if(omciManageEntity_p == NULL)
		return -1;
	
	tmpOmciManageEntity_p = omciManageEntity_p;
	/* init some param for this ME */
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_CREAT] = omciCreateAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_DELETE] = omciDeleteAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_GET] = omciGetAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_SET] = omciSetAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_GET_CURR_DATA] = omciGetCurrentDataAction;
	
	return 0;
}

int32 setIPHostPMThresholdData(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	omciMeInst_t *tmpomciMeInst_p = NULL;
	uint8 *attributeValuePtr = NULL;
	uint16 length = 0;
#if 0
	uint16 thresholdMeId = 0;
	int ret = 0;
	uint32 thresholdData = 0;
	int i  = 0;
	char tempBuffer[MAX_BUFFER_SIZE] = {0};
#endif

	if((value == NULL) || (meInstantPtr == NULL))
		return -1;

	tmpomciMeInst_p = (omciMeInst_ptr)meInstantPtr;
	attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_p , omciAttribute->attriName, &length);
	if(attributeValuePtr == NULL)
		return -1;
	memcpy(attributeValuePtr, value, length);

#if 0
	//get Threshold data instance
	thresholdMeId = get16((uint8 *)value);

	for(i=1; i<7; i++)
	{
		ret = getThresholdDataValue(OMCI_CLASS_ID_THRESHOLD_DATA1,thresholdMeId, i, &thresholdData);
		if (ret != 0)
			return -1;
		memset(tempBuffer, 0, sizeof(tempBuffer));
		sprintf(tempBuffer, "%x", thresholdData);
		ret = pmmgrTcapiSet( tmpomciMeInst_p->classId, tmpomciMeInst_p->deviceId, PMMGR_THRESHOLD, i, tempBuffer);
		if (ret != 0)
			return -1;
	}
#endif

	return 0;
}

int32 getIPHostPMICMPErrors(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	char tempBuffer[MAX_BUFFER_SIZE] = {0};
	omciMeInst_t *tmpomciMeInst_p = NULL;
	int ret = 0;
	uint32 pmData = 0;

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpomciMeInst_p = (omciMeInst_ptr)meInstantPtr;
	ret = pmmgrTcapiGet( tmpomciMeInst_p->classId, tmpomciMeInst_p->deviceId, flag, 1, tempBuffer);
	if (ret != 0)
		return -1;
	
	sscanf(tempBuffer, "%x", &pmData);
	put32((uint8 *)value, pmData);

	return 0;
}

int32 getIPHostPMDNSErrors(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	char tempBuffer[MAX_BUFFER_SIZE] = {0};
	omciMeInst_t *tmpomciMeInst_p = NULL;
	int ret = 0;
	uint32 pmData = 0;

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpomciMeInst_p = (omciMeInst_ptr)meInstantPtr;
	ret = pmmgrTcapiGet( tmpomciMeInst_p->classId, tmpomciMeInst_p->deviceId, flag, 2, tempBuffer);
	if (ret != 0)
		return -1;
	
	sscanf(tempBuffer, "%x", &pmData);
	put32((uint8 *)value, pmData);

	return 0;
}

int32 getIPHostPMIPHostPMDHCPTimeout(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	char tempBuffer[MAX_BUFFER_SIZE] = {0};
	omciMeInst_t *tmpomciMeInst_p = NULL;
	int ret = 0;
	uint32 tmpPmData = 0;
	uint16 pmData = 0;

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpomciMeInst_p = (omciMeInst_ptr)meInstantPtr;
	ret = pmmgrTcapiGet( tmpomciMeInst_p->classId, tmpomciMeInst_p->deviceId, flag, 3, tempBuffer);
	if (ret != 0)
		return -1;
	
	sscanf(tempBuffer, "%x", &tmpPmData);
	if(tmpPmData <= 65535)
		pmData = tmpPmData;
	else
		return -1;
	put16((uint8 *)value, pmData);

	return 0;
}

int32 getIPHostPMIPHostPMIPConflict(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	char tempBuffer[MAX_BUFFER_SIZE] = {0};
	omciMeInst_t *tmpomciMeInst_p = NULL;
	int ret = 0;
	uint32 tmpPmData = 0;
	uint16 pmData = 0;

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpomciMeInst_p = (omciMeInst_ptr)meInstantPtr;
	ret = pmmgrTcapiGet( tmpomciMeInst_p->classId, tmpomciMeInst_p->deviceId, flag, 4, tempBuffer);
	if (ret != 0)
		return -1;
	
	sscanf(tempBuffer, "%x", &tmpPmData);
	if(tmpPmData <= 65535)
		pmData = tmpPmData;
	else
		return -1;
	put16((uint8 *)value, pmData);

	return 0;
}

int32 getIPHostPMOutOfMemory(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	char tempBuffer[MAX_BUFFER_SIZE] = {0};
	omciMeInst_t *tmpomciMeInst_p = NULL;
	int ret = 0;
	uint32 tmpPmData = 0;
	uint16 pmData = 0;

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpomciMeInst_p = (omciMeInst_ptr)meInstantPtr;
	ret = pmmgrTcapiGet( tmpomciMeInst_p->classId, tmpomciMeInst_p->deviceId, flag, 5, tempBuffer);
	if (ret != 0)
		return -1;
	
	sscanf(tempBuffer, "%x", &tmpPmData);
	if(tmpPmData <= 65535)
		pmData = tmpPmData;
	else
		return -1;
	put16((uint8 *)value, pmData);

	return 0;
}

int32 getIPHostPMInternalError(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	char tempBuffer[MAX_BUFFER_SIZE] = {0};
	omciMeInst_t *tmpomciMeInst_p = NULL;
	int ret = 0;
	uint32 tmpPmData = 0;
	uint16 pmData = 0;

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpomciMeInst_p = (omciMeInst_ptr)meInstantPtr;
	ret = pmmgrTcapiGet( tmpomciMeInst_p->classId, tmpomciMeInst_p->deviceId, flag, 6, tempBuffer);
	if (ret != 0)
		return -1;
	
	sscanf(tempBuffer, "%x", &tmpPmData);
	if(tmpPmData <= 65535)
		pmData = tmpPmData;
	else
		return -1;
	put16((uint8 *)value, pmData);

	return 0;
}

/*******************************************************************************************************************************
9.4.3: TCP/UDP config data

********************************************************************************************************************************/
int omciMeInitForTcpUdpConfigData(omciManageEntity_t *omciManageEntity_p)
{
	omciManageEntity_t *tmpOmciManageEntity_p = NULL;
	
	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n enter omciMeInitForTcpUdpConfigData \r\n");

	if(omciManageEntity_p == NULL)
		return -1;
	
	tmpOmciManageEntity_p = omciManageEntity_p;
	/* init some param for this ME */
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_CREAT] = omciCreateAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_DELETE] = omciDeleteAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_GET] = omciGetAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_SET] = omciSetAction;
	
	return 0;
}

#ifdef TCSUPPORT_HUAWEI_OLT_VENDOR_SPECIFIC_ME_FOR_INA
#define MECONTENTLENGTH 28
#define REVERSEDMETABLELENGTH 20

static uint8 reserved_353_ME_content[MECONTENTLENGTH]={0x31,0x2e,0x33,0x2e,0x30,0x2e,0x30};
static uint8 reserved_65414_ME_content[MECONTENTLENGTH]={0x01,0x03,0x01,0x1e,0x06,0x48,0x57,0x2d,0x42,0x4f,0x42,0x2d,0x30,0x30,0x30,0x32,0x20,0x20,0x20,0x20,0x20,0x3a,0x0e,0x00,0x0a};

typedef struct omciReservedMe_s
{
	uint16 reservedMeClassId;
	uint16 reservedMeMask;
	uint8* content_ptr;
}omciReservedMe_t,*omciReservedMe_ptr;

omciReservedMe_t reversedMeTable[REVERSEDMETABLELENGTH]=
{
{OMCI_CLASS_ID_RESERVED_353_ME, 		0x0040, 		&reserved_353_ME_content},
{OMCI_CLASS_ID_RESERVED_65414_ME,		0x00ff, 		&reserved_65414_ME_content},
};

void contentReserveMe(uint16 meClassId, uint16 attributeMask, uint8 *responseContentPtr)
{
	int i;
	for (i=0; i<REVERSEDMETABLELENGTH; i++)
	{
		if (meClassId==reversedMeTable[i].reservedMeClassId && attributeMask==reversedMeTable[i].reservedMeMask)
		{
			memcpy(responseContentPtr, reversedMeTable[i].content_ptr,MECONTENTLENGTH*8);
		}
	}
}
#endif

int omciRsvMEGetAction(uint16 meClassId , omciMeInst_t *meInstant_ptr, omciPayload_t * omciPayLoad, uint8 msgSrc)
{
	uint8 *contentPtr = NULL;
	uint16 attributeMask = 0;
	uint16 responseAttributeMask = 0;
	uint16 excuteAttributeMask = 0;
	uint16 optionalAttributeMask = 0;
	int i = 0;
	int testj = 0;
	uint8 *responseContentPtr = contentPtr + 3; /*the first attribute value start Pointer*/
	uint8 result = 0; 
	int return_value = 0;

	omciManageEntity_ptr currentME = NULL;
	omciAttriDescript_t * currentAttribute =NULL;
	

	int msgContentLength = 0;
	int attributeLength = 0;

	int tableAttriubteNum = 0;
	uint8 attributeNum = 0;
	uint8 devId = 0;
	omciBaselinePayload_t *omciBaselinePayload=NULL;

	char data[OMCI_BASELINE_CONT_LEN] = {0};
	devId = omciPayLoad->devId;
	if(devId == OMCI_BASELINE_MSG)
	{

		omciBaselinePayload = (omciBaselinePayload_t *)omciPayLoad;
		contentPtr = (uint8 *)(omciBaselinePayload->msgContent);
		responseContentPtr = contentPtr +3;
	}
	else
	{
		omcidbgPrintf(OMCI_DEBUG_LEVEL_ERROR, "omciGetAction:---devId is invalid!\n");	
		return -1;
	}

	attributeMask	= parseAttributeMask(contentPtr);
	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE,"omciGetAction:attributeMask = %x\n", attributeMask);

	responseAttributeMask = attributeMask;
	
#ifdef TCSUPPORT_HUAWEI_OLT_VENDOR_SPECIFIC_ME_FOR_INA	
	contentReserveMe(meClassId,attributeMask,responseContentPtr);
#endif

#ifdef TCSUPPORT_HUAWEI_OLT_VENDOR_SPECIFIC_ME
	if (0x40 == attributeMask && OMCI_CLASS_ID_RESERVED_350_ME == meClassId)
	{
		memset(omciPayLoad->msgContent,0,OMCI_BASELINE_CONT_LEN);
		omciPayLoad->msgContent[1] = 2; 
	}
#endif
pktSend:

/*MessageType response AR = 0; AK =1*/
	responseMessageTypeHandle((uint8 *)(&omciPayLoad->msgType));
/*write reason and result*/
	responseContentPtr = contentPtr;
	writeResultAndReasonToOmciPayLoad(result, responseContentPtr);
/*write attribute mask*/
	responseContentPtr = contentPtr + 1;
	reparseAttributeMask(responseAttributeMask, responseContentPtr);
	
/*write optional attribute mask*/	
	responseContentPtr = contentPtr + 28;
	reparseAttributeMask(optionalAttributeMask, responseContentPtr);		/*or result reason = 0011*/
/*write excute attribute mask*/
	responseContentPtr = contentPtr + 30;
	reparseAttributeMask(excuteAttributeMask, responseContentPtr);

	//memcpy(omciPayLoad->msgContent,"0200000000000000",strlen("0200000000000000"));

	omciPktSend(omciPayLoad, TRUE); 
	return 0;		
}

#ifdef TCSUPPORT_VNPTT
int omciMeInitFor250ME(omciManageEntity_t *omciManageEntity_p)
{
	omciManageEntity_t *tmpOmciManageEntity_p = NULL;
	
	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n enter omciMeInitFor250ME \r\n");

	if(omciManageEntity_p == NULL)
		return -1;
	
	tmpOmciManageEntity_p = omciManageEntity_p;
	/* init some param for this ME */
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_CREAT] = omciCreateAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_DELETE] = omciDeleteAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_GET] = omciRsvMEGetAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_SET] = omciSetAction;
	
	return 0;
}

int omciMeInitFor347ME(omciManageEntity_t *omciManageEntity_p)
{
	omciManageEntity_t *tmpOmciManageEntity_p = NULL;
	
	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n enter omciMeInitFor250ME \r\n");

	if(omciManageEntity_p == NULL)
		return -1;
	
	tmpOmciManageEntity_p = omciManageEntity_p;
	/* init some param for this ME */
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_CREAT] = omciCreateAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_DELETE] = omciDeleteAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_GET] = omciRsvMEGetAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_SET] = omciSetAction;
	
	return 0;
}
#endif
/*******************************************************************************************************************************
temp add for reserve ME

********************************************************************************************************************************/

#ifdef TCSUPPORT_HUAWEI_OLT_VENDOR_SPECIFIC_ME

int omciMeInitFor350ME(omciManageEntity_t *omciManageEntity_p)
{
	omciManageEntity_t *tmpOmciManageEntity_p = NULL;
	
	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n enter omciMeInitFor350ME \r\n");

	if(omciManageEntity_p == NULL)
		return -1;
	
	tmpOmciManageEntity_p = omciManageEntity_p;
	/* init some param for this ME */
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_CREAT] = omciCreateAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_DELETE] = omciDeleteAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_GET] = omciRsvMEGetAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_SET] = omciSetAction;
	
	return 0;
}

int omciMeInitFor352ME(omciManageEntity_t *omciManageEntity_p)
{
	omciManageEntity_t *tmpOmciManageEntity_p = NULL;
	
	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n enter omciMeInitFor352ME \r\n");

	if(omciManageEntity_p == NULL)
		return -1;
	
	tmpOmciManageEntity_p = omciManageEntity_p;
	/* init some param for this ME */
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_CREAT] = omciCreateAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_DELETE] = omciDeleteAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_GET] = omciRsvMEGetAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_SET] = omciSetAction;
	
	return 0;
}

int omciMeInitFor353ME(omciManageEntity_t *omciManageEntity_p)
{
	omciManageEntity_t *tmpOmciManageEntity_p = NULL;
	
	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n enter omciMeInitFor353ME \r\n");

	if(omciManageEntity_p == NULL)
		return -1;
	
	tmpOmciManageEntity_p = omciManageEntity_p;
	/* init some param for this ME */
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_CREAT] = omciCreateAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_DELETE] = omciDeleteAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_GET] = omciRsvMEGetAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_SET] = omciSetAction;
	
	return 0;
}


int omciMeInitFor367ME(omciManageEntity_t *omciManageEntity_p)
{
	omciManageEntity_t *tmpOmciManageEntity_p = NULL;
	
	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n enter omciMeInitFor367ME \r\n");

	if(omciManageEntity_p == NULL)
		return -1;
	
	tmpOmciManageEntity_p = omciManageEntity_p;
	/* init some param for this ME */
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_CREAT] = omciCreateAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_DELETE] = omciDeleteAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_GET] = omciRsvMEGetAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_SET] = omciSetAction;
	
	return 0;
}

int omciMeInitFor373ME(omciManageEntity_t *omciManageEntity_p)
{
	omciManageEntity_t *tmpOmciManageEntity_p = NULL;
	
	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n enter omciMeInitFor373ME \r\n");

	if(omciManageEntity_p == NULL)
		return -1;
	
	tmpOmciManageEntity_p = omciManageEntity_p;
	/* init some param for this ME */
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_CREAT] = omciCreateAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_DELETE] = omciDeleteAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_GET] = omciRsvMEGetAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_SET] = omciSetAction;
	
	return 0;
}

#ifdef TCSUPPORT_HUAWEI_OLT_VENDOR_SPECIFIC_ME_FOR_INA
int omciMeInitFor370ME(omciManageEntity_t *omciManageEntity_p)
{
	omciManageEntity_t *tmpOmciManageEntity_p = NULL;
	
	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n enter omciMeInitFor370ME \r\n");

	if(omciManageEntity_p == NULL)
		return -1;
	
	tmpOmciManageEntity_p = omciManageEntity_p;
	/* init some param for this ME */
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_CREAT] = omciCreateAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_DELETE] = omciDeleteAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_GET] = omciRsvMEGetAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_SET] = omciSetAction;
	
	return 0;
}

int omciMeInitFor65408ME(omciManageEntity_t *omciManageEntity_p)
{
	omciManageEntity_t *tmpOmciManageEntity_p = NULL;
	
	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n enter omciMeInitFor65408ME \r\n");

	if(omciManageEntity_p == NULL)
		return -1;
	
	tmpOmciManageEntity_p = omciManageEntity_p;
	/* init some param for this ME */
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_CREAT] = omciCreateAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_DELETE] = omciDeleteAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_GET] = omciRsvMEGetAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_SET] = omciSetAction;
	
	return 0;
}

int omciMeInitFor65414ME(omciManageEntity_t *omciManageEntity_p)
{
	omciManageEntity_t *tmpOmciManageEntity_p = NULL;
	
	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n enter omciMeInitFor65414ME \r\n");

	if(omciManageEntity_p == NULL)
		return -1;
	
	tmpOmciManageEntity_p = omciManageEntity_p;
	/* init some param for this ME */
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_CREAT] = omciCreateAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_DELETE] = omciDeleteAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_GET] = omciRsvMEGetAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_SET] = omciSetAction;
	
	return 0;
}

int omciMeInitFor65425ME(omciManageEntity_t *omciManageEntity_p)
{
	omciManageEntity_t *tmpOmciManageEntity_p = NULL;
	
	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n enter omciMeInitFor65425ME \r\n");

	if(omciManageEntity_p == NULL)
		return -1;
	
	tmpOmciManageEntity_p = omciManageEntity_p;
	/* init some param for this ME */
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_CREAT] = omciCreateAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_DELETE] = omciDeleteAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_GET] = omciRsvMEGetAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_SET] = omciSetAction;
	
	return 0;
}
#endif
#endif

int omciMeInitForTcpUdpPM(omciManageEntity_t *omciManageEntity_p)
{
	omciManageEntity_t *tmpOmciManageEntity_p = NULL;
	
	omcidbgPrintf(OMCI_DEBUG_LEVEL_TRACE, "\r\n enter omciMeInitForTcpUdpPM \r\n");

	if(omciManageEntity_p == NULL)
		return -1;
	
	tmpOmciManageEntity_p = omciManageEntity_p;
	/* init some param for this ME */
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_CREAT] = omciCreateAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_DELETE] = omciDeleteAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_GET] = omciGetAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_SET] = omciSetAction;
	tmpOmciManageEntity_p->omciActionFunc[MT_OMCI_MSG_TYPE_GET_CURR_DATA] = omciGetCurrentDataAction;
	
	return 0;
}

/*******************************************************************************************************************************
9.4.4: TCP/UDP performance monitoring history data

********************************************************************************************************************************/

int32 setTcpUdpPMThresholdData(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	omciMeInst_t *tmpomciMeInst_p = NULL;
	uint8 *attributeValuePtr = NULL;
	uint16 length = 0;
#if 0
	uint16 thresholdMeId = 0;
	int ret = 0;
	uint32 thresholdData = 0;
	int i = 0;
	char tempBuffer[MAX_BUFFER_SIZE] = {0};
#endif

	if((value == NULL) || (meInstantPtr == NULL))
		return -1;

	tmpomciMeInst_p = (omciMeInst_ptr)meInstantPtr;
	attributeValuePtr = omciGetInstAttriByName(tmpomciMeInst_p , omciAttribute->attriName, &length);
	if(attributeValuePtr == NULL)
		return -1;
	memcpy(attributeValuePtr, value, length);
#if 0
	//get Threshold data instance
	thresholdMeId = get16((uint8 *)value);

	for(i=1; i<6; i++)
	{
		ret = getThresholdDataValue(OMCI_CLASS_ID_THRESHOLD_DATA1,thresholdMeId, i, &thresholdData);
		if (ret != 0)
			return -1;
		memset(tempBuffer, 0, sizeof(tempBuffer));
		sprintf(tempBuffer, "%x", thresholdData);
		ret = pmmgrTcapiSet( tmpomciMeInst_p->classId, tmpomciMeInst_p->deviceId, PMMGR_THRESHOLD, i, tempBuffer);
		if (ret != 0)
			return -1;
	}
#endif

	return 0;
}

int32 getTcpUdpPMSocketFailed(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	char tempBuffer[MAX_BUFFER_SIZE] = {0};
	omciMeInst_t *tmpomciMeInst_p = NULL;
	int ret = 0;
	uint32 tmpPmData = 0;
	uint16 pmData = 0;

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpomciMeInst_p = (omciMeInst_ptr)meInstantPtr;
	ret = pmmgrTcapiGet( tmpomciMeInst_p->classId, tmpomciMeInst_p->deviceId, flag, 1, tempBuffer);
	if (ret != 0)
		return -1;
	
	sscanf(tempBuffer, "%x", &tmpPmData);
	if(tmpPmData <= 65535)
		pmData = tmpPmData;
	else
		return -1;
	put16((uint8 *)value, pmData);

	return 0;
}

int32 getTcpUdpPMListenFailed(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	char tempBuffer[MAX_BUFFER_SIZE] = {0};
	omciMeInst_t *tmpomciMeInst_p = NULL;
	int ret = 0;
	uint32 tmpPmData = 0;
	uint16 pmData = 0;

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpomciMeInst_p = (omciMeInst_ptr)meInstantPtr;
	ret = pmmgrTcapiGet( tmpomciMeInst_p->classId, tmpomciMeInst_p->deviceId, flag, 2, tempBuffer);
	if (ret != 0)
		return -1;
	
	sscanf(tempBuffer, "%x", &tmpPmData);
	if(tmpPmData <= 65535)
		pmData = tmpPmData;
	else
		return -1;
	put16((uint8 *)value, pmData);

	return 0;
}

int32 getTcpUdpPMBindFailed(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	char tempBuffer[MAX_BUFFER_SIZE] = {0};
	omciMeInst_t *tmpomciMeInst_p = NULL;
	int ret = 0;
	uint32 tmpPmData = 0;
	uint16 pmData = 0;

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpomciMeInst_p = (omciMeInst_ptr)meInstantPtr;
	ret = pmmgrTcapiGet( tmpomciMeInst_p->classId, tmpomciMeInst_p->deviceId, flag, 3, tempBuffer);
	if (ret != 0)
		return -1;
	
	sscanf(tempBuffer, "%x", &tmpPmData);
	if(tmpPmData <= 65535)
		pmData = tmpPmData;
	else
		return -1;
	put16((uint8 *)value, pmData);

	return 0;
}

int32 getTcpUdpPMAcceptFailed(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	char tempBuffer[MAX_BUFFER_SIZE] = {0};
	omciMeInst_t *tmpomciMeInst_p = NULL;
	int ret = 0;
	uint32 tmpPmData = 0;
	uint16 pmData = 0;

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpomciMeInst_p = (omciMeInst_ptr)meInstantPtr;
	ret = pmmgrTcapiGet( tmpomciMeInst_p->classId, tmpomciMeInst_p->deviceId, flag, 4, tempBuffer);
	if (ret != 0)
		return -1;
	
	sscanf(tempBuffer, "%x", &tmpPmData);
	if(tmpPmData <= 65535)
		pmData = tmpPmData;
	else
		return -1;
	put16((uint8 *)value, pmData);

	return 0;
}

int32 getTcpUdpPMSelectFailed(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag)
{
	char tempBuffer[MAX_BUFFER_SIZE] = {0};
	omciMeInst_t *tmpomciMeInst_p = NULL;
	int ret = 0;
	uint32 tmpPmData = 0;
	uint16 pmData = 0;

	if((value == NULL) || (meInstantPtr == NULL) || (omciAttribute == NULL))
		return -1;

	tmpomciMeInst_p = (omciMeInst_ptr)meInstantPtr;
	ret = pmmgrTcapiGet( tmpomciMeInst_p->classId, tmpomciMeInst_p->deviceId, flag, 5, tempBuffer);
	if (ret != 0)
		return -1;
	
	sscanf(tempBuffer, "%x", &tmpPmData);
	if(tmpPmData <= 65535)
		pmData = tmpPmData;
	else
		return -1;
	put16((uint8 *)value, pmData);

	return 0;
}



