#include "Global_res.h"
#include "cp_qos.h"

extern CwmpParameterStruct QueueIndex[];
extern CwmpParameterStruct ClassificationIndex[];
extern CwmpParameterStruct PolicerIndex[];


enum Procotol_ID
{
	TCP_UDP = 0,
	TCP,
	UDP,
	ICMP,
	IGMP,
	
	MAX_Protocol_ID
};/* protocol ID of QoS*/
char protocol_type[][32]=
{
	{"TCP/UDP"},
	{"TCP"},
	{"UDP"},
	{"ICMP"},
	{"IGMP"},
	{""},	
};/*protocol type of QoS*/


enum Discipline_ID
{
	Discipline_SP = 0,
	Discipline_WRR,
	
	MAX_Discipline_ID
};/* Discipline ID of QoS*/
char discipline_type[][32]=
{
	{"SP"},
	{"WRR"},
	{""},	
};/*Discipline type of QoS*/


enum Queue_ID
{
	Queue_Low = 0,
	Queue_Medium,
	Queue_High,
	Queue_Highest,
	
	MAX_Queue_ID
};/* Queue ID of QoS*/
char queue_type[][32]=
{
	{"Low"},
	{"Medium"},
	{"High"},
	{"Highest"},
	{""},	
};/*Queue type of QoS*/


int queueManagementCommit(cfgNode_ptr pCfgNode)
{
	int ruleIndex;
	int i = 0, j = 0;
	char rollbackEntry[PKT_QOS_RULE_MAX][32];
	char nodeName[32];
	char classQueue[16];
	int ret = 0;
	
	for(ruleIndex=0; ruleIndex<PKT_QOS_RULE_MAX; ruleIndex++) {
		
		if(pCfgNode->bitMap & (1<<ruleIndex)) {

			/*get the classqueue value of this qos entry*/
			memset(nodeName, 0, sizeof(nodeName));
			memset(classQueue, 0, sizeof(classQueue));
			sprintf(nodeName, QOS_ENTRY, ruleIndex);
			ret = tcapi_get(nodeName, "ActQueue", classQueue);
			if( ret < 0 || strlen(classQueue) == 0 ) {/* if classqueue is not set, clear the settings and rollback this entry */
				memset(rollbackEntry[i], 0, 32);
				sprintf(rollbackEntry[i], QOS_ENTRY, ruleIndex);
				i++;
				tc_printf("\n==>The class queue of this rule %d is not set , do not save\n", ruleIndex);
			}
			else {
				tcapi_set( "QoS_Common", "QoSOptType", "typeRule" );
				tcapi_commit( nodeName );
			}

		}
	}
	
	for(j=0; j<i; j++){
		tc_printf("\n==>For not queue value set, just rollback %s\n", rollbackEntry[j]);
		tcapi_read(rollbackEntry[j]);
	}
	
	tcapi_set("QoS_Common", "QoSOptType", "discRule");
	tcapi_commit(pCfgNode->nodeName);

	return 0;
}

#if 0
int checkMACFormat(char *mac)
{
	int i, j=0;
	char newmac[20];
	char tmpS[20];

	if(strlen(mac) == 17) {
		for(i=0; i<17; i++) {
			if(mac[i] != ':') {
				newmac[j] = mac[i]; 
				j++;
			}
		}
		newmac[j] = '\0';
	}
	else if(strlen(mac) == 12)
		strcpy(newmac, mac);
	else
		return -1;

	for(i=0; i<12; i++) {
		if( (newmac[i]>='A' && newmac[i]<='F')
			||(newmac[i]>='a' && newmac[i]<='f')
			||(newmac[i]>='0' && newmac[i]<='9') ) {

			continue;
		}
		else
			return -1;
	}
	
	return 0;
}
#endif

int	cwmpInitQoSQueueSubTree( DeviceNodePtr deviceNode )
{
	int i = 0;
	int entries = 4;//The size of this table is fixed with exactly 4 entries: Low, Medium, High, Highest
	char index[4];
	int rtnCode = 0;
	
	if(deviceNode == NULL){
		return -1;
	}

	for(i = 0; i < entries; i++)
	{
		sprintf(index, "%d", i+1);

		QueueIndex[0].name = index;
		
		if ( RegisterNodeFromTable( deviceNode, QueueIndex) == NULL ) {
			rtnCode = -1;		//registerNode fail.
			return rtnCode;
		}
	}
	
	return rtnCode;
}

int	cwmpInitQoSClassificationSubTree( DeviceNodePtr deviceNode )
{
	int i = 0;
	int entries = PKT_QOS_RULE_MAX;//The size of this table is fixed with exactly 16 rules
	char index[4];
	int rtnCode = 0;
	char nodeName[32];
	char tmpBuf[32];
	int ret = 0;
	
	if(deviceNode == NULL){
		return -1;
	}

	for(i = 0; i < entries; i++)
	{
		memset(nodeName, 0, sizeof(nodeName));
		memset(tmpBuf, 0, sizeof(tmpBuf));
		memset(index, 0, sizeof(index));

		sprintf( nodeName, QOS_ENTRY, i );

		/*check if this qos entry is exist or not*/
		ret = tcapi_get( nodeName, "Active", tmpBuf );
		if ( ret < 0 ) { //it means this qos entry is not exist.
			continue;
		}
		
		sprintf( index, "%d", i+1 );

		ClassificationIndex[0].name = index;
		
		if ( RegisterNodeFromTable( deviceNode, ClassificationIndex) == NULL ) {
			rtnCode = -1;		//registerNode fail.
			return rtnCode;
		}
	}
	
	return rtnCode;
}

int	cwmpInitQoSPolicerSubTree( DeviceNodePtr deviceNode )
{
	int i = 0;
	int entries = 1;//The size of this table is fixed with exactly 1 entries: SP
	char index[4];
	int rtnCode = 0;
	
	if(deviceNode == NULL){
		return -1;
	}

	for(i = 0; i < entries; i++)
	{
		sprintf(index,"%d",i+1);

		PolicerIndex[0].name = index;
		
		if ( RegisterNodeFromTable( deviceNode, PolicerIndex) == NULL ) {
			rtnCode = -1;		//registerNode fail.
			return rtnCode;
		}
	}
	
	return rtnCode;
}

int8 checkFreeQosRule(void)
{
	int i = 0;
	char nodeName[32];
	char tmpBuf[32];
	int ret = 0;
	
	for(i=0; i<PKT_QOS_RULE_MAX; i++){
		memset(nodeName, 0, sizeof(nodeName));
		memset(tmpBuf, 0, sizeof(tmpBuf));
		sprintf(nodeName, QOS_ENTRY, i);

		ret = tcapi_get(nodeName, "Active", tmpBuf);
		if( ret < 0 )//it means this qos rule is not exist.
			return i;
	}
	
	return -1;
}

int8 InitQoSRuleTableByIndex(int num)
{
	char nodeName[32] = {0};
	
#if 0	
	if(num<0 || num>=PKT_QOS_RULE_MAX)
		return -1;
#endif
	memset(nodeName, 0, sizeof(nodeName));
	sprintf(nodeName, QOS_ENTRY, num);

	tcapi_set(nodeName, "Active", "No");
	//tcapi_commit(nodeName);
	
	return 0;
}

int8 deleteQoSRuleEntryByIndex(uint32 num)
{
	char nodeName[32];
	
	if(num<0 || num>=PKT_QOS_RULE_MAX)
		return -1;

	memset(nodeName, 0, sizeof(nodeName));
	sprintf(nodeName, QOS_ENTRY, num);

	tcapi_unset(nodeName);
	tcapi_commit(QOS_NODENAME);
	return 0;
}


int8 addClassificationObject(uint32* num, DeviceNode*  deviceNode)
{
	int number = -1;
	DeviceNodePtr ruleNode = NULL;
	char *objectName = "InternetGatewayDevice.QueueManagement.Classification.";
	int rtnCode = 0;
	
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &ruleNode);
	if(ruleNode == NULL){
		return -1;
	}
	
	if ((number = checkFreeQosRule()) < 0) {
		tc_printf("\n==>Can't find free QOS rule for use!\n");
		return -1;
	}
	else{
		tc_printf("\n==> Find free QOS rule %d to add!\n", number+1);
		InitQoSRuleTableByIndex(number); //enable this rule
		cwmpFreeDeviceTable(ruleNode);
		rtnCode = cwmpInitQoSClassificationSubTree(ruleNode);

		*num = number + 1;
		if(dbg_cwmp >= DBG_CWMP_L2 ){
			tc_printf("\r\ncwmpParameter.c---[addClassificationObject]---*num = ", *num);
		}
	}	
	
	return rtnCode;
}

int8 deleteQoSRuleObject(uint32 num, DeviceNode*  deviceNode)
{
	DeviceNodePtr ruleNode = NULL;
	char *objectName = "InternetGatewayDevice.QueueManagement.Classification.";
	int rtnCode = 0;
	
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &ruleNode);
	if(ruleNode == NULL){
		return -1;
	}
	tc_printf("\n==>Delete QoS Rule %d--\n", num);
	deleteQoSRuleEntryByIndex(num - 1);//InstanceNumber - 1, disable this rule and reset its members

	cwmpFreeDeviceTable(ruleNode);
	rtnCode = cwmpInitQoSClassificationSubTree(ruleNode);

	return rtnCode;
}


/*Queue*/
int8 getQoSQueueInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[4]="WAN";

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getQoSQueueInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aQoSQueueInterface[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setQoSQueueInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aQoSQueueInterface[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;

	return result;
}

int8 getQoSDropAlgorithmValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[4]="DT";

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getQoSDropAlgorithmAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aQoSDropAlgorithm[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setQoSDropAlgorithmAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aQoSDropAlgorithm[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;

	return result;
}

int8 getQoSSchedulerAlgorithmValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[4]="SP";

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getQoSSchedulerAlgorithmAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aQoSSchedulerAlgorithm[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setQoSSchedulerAlgorithmAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aQoSSchedulerAlgorithm[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;

	return result;
}

int8 getQoSQueueEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[4]="1";

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getQoSQueueEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aQoSQueueEnable[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setQoSQueueEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aQoSQueueEnable[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;

	return result;
}


/*Classification*/
int8 getQoSProtocolValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8]="";
	char nodeName[32];
	char tmpBuf[32];
	int instance;
	int protocolID, i;

	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	
	sprintf(nodeName, QOS_ENTRY, instance-1);
	tcapi_get(nodeName, "ProtocolID", tmpBuf);
	if(strlen(tmpBuf) == 0)
		protocolID = -1;//-1 indicates this criterion is not used for classification
	else {

	        for(i=0; i<MAX_Protocol_ID; i++){
		        if(strcmp(tmpBuf, protocol_type[i]) ==0){
			        protocolID = i;
			        break;
		        }
	        }
                if(i == MAX_Protocol_ID)
		        return -1;//-1 indicates this criterion is not used for classification
	}
	sprintf(tempValue, "%d", protocolID);

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setQoSProtocolValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance;
	int* protocol_num;
	char nodeName[32];
	char tmpBuf[32];

	protocol_num = (int*)(value);
	
	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	sprintf(nodeName, QOS_ENTRY, instance-1);
	
	if(*protocol_num == -1){
		tmpBuf[0] = '\0';//set null value
		tcapi_set(nodeName, "ProtocolID", tmpBuf);
	}
	else if(*protocol_num == TCP
		||*protocol_num == UDP
		||*protocol_num == ICMP
		||*protocol_num == IGMP
		||*protocol_num == TCP_UDP){
		strcpy(tmpBuf, protocol_type[*protocol_num]);
		tcapi_set(nodeName, "ProtocolID", tmpBuf);
	}
	else {
		tc_printf("\n==>Invalid QoS protocol value--\n");
		return-1;
	}

	SetCfgNodeFlag( QOS_NODENAME, instance-1 );
	
	return 1;
}

int8 getQoSProtocolAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aQoSProtocol[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setQoSProtocolAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aQoSProtocol[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 getQoSDSCPMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8];
	int instance;
	char nodeName[32];
	char tmpBuf[32];
	
	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	memset(tempValue, 0, sizeof(tempValue));

	sprintf(nodeName, QOS_ENTRY, instance-1);
	tcapi_get(nodeName, "DSCPRangeBegin", tmpBuf);//in tr098 spec, dscp is a single number; in our code, dscp is a range, here return dscp start
	if(strlen(tmpBuf) != 0)
		strcpy(tempValue, tmpBuf);
	else
		sprintf(tempValue, "%d", -1);//in tr068 spec, -1 indicates no channge from the incoming packet

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setQoSDSCPMarkValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance;
	int* dscp_num;
	char nodeName[32];
	char tmpBuf[32];
	
	dscp_num = (int*)(value);
	if(*dscp_num<-1 || *dscp_num>63)
		return -1;
	
	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	sprintf(nodeName, QOS_ENTRY, instance-1);

	if(*dscp_num == -1)
		tmpBuf[0] = '\0';//set null value
	else
		sprintf(tmpBuf, "%d", *dscp_num);
	
	tcapi_set(nodeName, "DSCPRangeBegin", tmpBuf);
	tcapi_set(nodeName, "DSCPRangeEnd", tmpBuf);

	SetCfgNodeFlag( QOS_NODENAME, instance-1 );

	return 1;
} 

int8 getQoSDSCPMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aQoSDSCPMark[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 setQoSDSCPMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aQoSDSCPMark[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	return result;
}

int8 getQoSForwardingPolicyValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char nodeName[32];
	char tmpBuf[32];
	char tempValue[8];
	int i;

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	memset(tempValue, 0, sizeof(tempValue));
	
	strcpy(nodeName, QOS_COMMON);
	tcapi_get(nodeName, "Discipline", tmpBuf);

	for(i=0; i<MAX_Discipline_ID; i++){
		if(strcmp(tmpBuf, discipline_type[i]) == 0){
			sprintf(tempValue, "%d", i+1);
			break;
		}
	}
	if(i == MAX_Discipline_ID)
	{
		sprintf(tempValue, "%d", -1);
		//return -1;
	}

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setQoSForwardingPolicyValue(void* value, struct DeviceNode*  deviceNode)
{
	unsigned int *temp;
	char tmpBuf[32];
	char nodeName[32];
	unsigned int policyIndex = 0;
	
	temp = (unsigned int*)value;

	if(*temp < 1  || *temp > 2 )
		return -1;

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));

	strcpy(nodeName, QOS_COMMON);
	policyIndex = *temp - 1;
	if(policyIndex == Discipline_SP
	|| policyIndex == Discipline_WRR){//change policy for all rules
		tcapi_set(nodeName, "DisciplineFlag", "Yes");
		tcapi_set(nodeName, "Discipline", discipline_type[policyIndex]);
	}
	else {
		tc_printf("\n==>Invalid QoS forwarding policey value--\n");
		return -1;
	}
	SetCfgNodeFlag( QOS_NODENAME, -1 );

	return 0;
}

int8 getQoSForwardingPolicyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aQoSForwardingPolicy[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setQoSForwardingPolicyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aQoSForwardingPolicy[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 getQoSClassQueueValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8]="";
	int instance, i;
	int queue_num;
	char nodeName[32];
	char tmpBuf[32];

	instance = atoi(deviceNode->parentNode->name);
	
	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	memset(tempValue, 0, sizeof(tempValue));

	sprintf(nodeName, QOS_ENTRY, instance-1);
	tcapi_get(nodeName, "ActQueue", tmpBuf);

	for(i=0; i<MAX_Queue_ID; i++){
		if(strcmp(tmpBuf, queue_type[i]) == 0){
			queue_num = i+1;
			break;
		}		
	}
	if(i == MAX_Queue_ID)
	{
		//return -1;
		sprintf(tempValue, "%d", -1);
	}
	else
	{
		sprintf(tempValue, "%d", queue_num);
	}

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setQoSClassQueueValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance;
	unsigned int* queue_index;
	char nodeName[32];
	char tmpBuf[32];

	queue_index = (unsigned int*)(value);
	if( *queue_index<1 || *queue_index>4 )
		return -1;
	
	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	sprintf(nodeName, QOS_ENTRY, instance-1);
	tcapi_set(nodeName, "ActQueue", queue_type[*queue_index-1]);

	SetCfgNodeFlag( QOS_NODENAME, instance-1 );

	return 1;
}

int8 getQoSClassQueueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aQoSClassQueue[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setQoSClassQueueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aQoSClassQueue[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 getQoSClassPolicerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8]="2";
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setQoSClassPolicerValue(void* value, struct DeviceNode*  deviceNode)
{
	int * temp;

	temp = (int*)value;
	if(*temp != 2) //at present, there are two policers:SP and WRR
		return -1;

	SetCfgNodeFlag( QOS_NODENAME, -1 );

	return 0;
}

int8 getQoSClassPolicerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aQoSClassPolicer[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setQoSClassPolicerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aQoSClassPolicer[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 getQoSClassificationEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8]="";
	int instance;
	int queue_num;
	char nodeName[32];
	char tmpBuf[32];

	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	memset(tempValue, 0, sizeof(tempValue));

	sprintf(nodeName, QOS_ENTRY, instance-1);
	tcapi_get(nodeName, "Active", tmpBuf);

	if(strcmp(tmpBuf, "Yes") == 0){
		strcpy(tempValue, "1"); 
	}
	else{
		strcpy(tempValue, "0"); 
	}

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setQoSClassificationEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 QoSEnable;
	int instance;
	char nodeName[32];
	char tmpBuf[32];

	memset(tmpBuf, 0, sizeof(tmpBuf));
	memset(nodeName, 0, sizeof(nodeName));
	
	QoSEnable = *(uint8*)value;
	if(QoSEnable == 1)
		strcpy(tmpBuf, "Yes");
	else if(QoSEnable == 0)
		strcpy(tmpBuf, "No");
	else
		return -1;

	instance = atoi(deviceNode->parentNode->name);	

	sprintf(nodeName, QOS_ENTRY, instance-1);
	tcapi_set(nodeName, "Active", tmpBuf);

	SetCfgNodeFlag( QOS_NODENAME, instance-1 );
	
	return 1;
}

int8 getQoSClassificationEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aQoSClassificationEnable[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setQoSClassificationEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aQoSClassificationEnable[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 getQosDestIPValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[16];
	char * addr=NULL;
	int instance;
	char nodeName[32];
	char tmpBuf[32];	

	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	memset(tempValue, 0, sizeof(tempValue));

	sprintf(nodeName, QOS_ENTRY, instance-1);
	tcapi_get(nodeName, "DesIP", tmpBuf);

	if(strlen(tmpBuf) <= 15)
		strcpy(tempValue, tmpBuf);

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setQosDestIPValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance=0;
	char * ipStr=NULL;
	int isIPLegal=0;
	char nodeName[32];
	char tmpBuf[32];	

	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));

	sprintf(nodeName, QOS_ENTRY, instance-1);

	ipStr = (char*)value;
	if( ipStr == NULL ) {
		return -1;
	}
	if( strlen(ipStr) == 0) {
		tmpBuf[0] = '\0';
	}
	else {
		isIPLegal = _checkIp(ipStr);
		if( isIPLegal<0 || strlen(ipStr)>15 ) {
			return -1;
		}
		strcpy(tmpBuf, ipStr);
	}

	tcapi_set(nodeName, "DesIP", tmpBuf);

	SetCfgNodeFlag( QOS_NODENAME, instance-1 );
	
	return 1;
}

int8 getQosDestIPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aQosDestIP[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setQosDestIPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aQosDestIP[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 getQosDestMaskValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[17]={0};
	int instance;
	char nodeName[32];
	char tmpBuf[32];	

	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	memset(tempValue, 0, sizeof(tempValue));

	sprintf(nodeName, QOS_ENTRY, instance-1);
	tcapi_get(nodeName, "DesMask", tmpBuf);

	if(strlen(tmpBuf) <= 15) 
		sprintf(tempValue, "%s", tmpBuf );

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setQosDestMaskValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance=0;
	char * maskStr=NULL;
	char * ipStr=NULL;
	int isIPLegal=0;
	ip4a netmask, broadcast;
	int i=0;
	char nodeName[32];
	char tmpBuf[32];	
	char netMaskBuf[32];
	
	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	memset(netMaskBuf, 0, sizeof(netMaskBuf));

	sprintf(nodeName, QOS_ENTRY, instance-1);
	
	maskStr = (char*)value;
	if(maskStr==NULL )
		return -1;
	
	if(strlen(maskStr)==0) {
		netMaskBuf[0] = '\0';	
	}
	else {
		netmask = aton(maskStr);
		broadcast = aton("255.255.255.255");

		for (i = 0; i < 32; i++) {
			if (netmask & (1 << i))	
				break;
			
			broadcast &= ~(1 << i);
		}

		if (broadcast != netmask || strlen(maskStr)>15 ) 
			return -1;	
		else {
			strcpy(netMaskBuf, maskStr);
		}
	}
	
	tcapi_set(nodeName, "DesMask", netMaskBuf);
	
	SetCfgNodeFlag( QOS_NODENAME, instance-1 );
	
	return 1;
}

int8 getQosDestMaskAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aQosDestMask[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setQosDestMaskAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aQosDestMask[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 getQosSourceIPValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[16]={0};
	int instance;
	char * addr=NULL;
	char nodeName[32];
	char tmpBuf[32];
	
	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	memset(tempValue, 0, sizeof(tempValue));

	sprintf(nodeName, QOS_ENTRY, instance-1);
	tcapi_get(nodeName, "SrcIP", tmpBuf);

	if(strlen(tmpBuf) <= 15)
		strcpy(tempValue, tmpBuf);
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setQosSourceIPValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance=0;
	char * ipStr=NULL;
	int isIPLegal=0;
	char * mask=NULL;
	int i=0;
	char nodeName[32];
	char tmpBuf[32];	

	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));

	sprintf(nodeName, QOS_ENTRY, instance-1);

	ipStr = (char*)value;
	if( ipStr==NULL ) 
		return -1;
	
	if( strlen(ipStr)==0) {
		tmpBuf[0]= '\0';
	}
	else {
		isIPLegal=_checkIp(ipStr);
		if( isIPLegal<0 || strlen(ipStr)>15 ) 			
			return -1;

		strcpy(tmpBuf, ipStr);
		
	}
	tcapi_set(nodeName, "SrcIP", tmpBuf);

	SetCfgNodeFlag( QOS_NODENAME, instance-1 );
	
	return 1;
}
int8 getQosSourceIPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aQosSourceIP[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setQosSourceIPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aQosSourceIP[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 getQosSourceMaskValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[17]={0};
	int instance;
	char nodeName[32];
	char tmpBuf[32];	

	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	memset(tempValue, 0, sizeof(tempValue));

	sprintf(nodeName, QOS_ENTRY, instance-1);
	tcapi_get(nodeName, "SrcMask", tmpBuf);

	if(strlen(tmpBuf) <= 15) 
		sprintf(tempValue, "%s", tmpBuf );

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setQosSourceMaskValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance=0;
	char *maskStr=NULL;
	char *ipStr=NULL;
	int isIPLegal=0;
	int i=0;
	ip4a netmask, broadcast;
	char nodeName[32];
	char tmpBuf[32];	
	char netMaskBuf[32];
	
	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	memset(netMaskBuf, 0, sizeof(netMaskBuf));

	sprintf(nodeName, QOS_ENTRY, instance-1);	
	
	maskStr = (char*)value;
	if(maskStr==NULL )
		return -1;
	
	if(strlen(maskStr)==0) {
		netMaskBuf[0] = '\0';	
	}
	else
	{
		netmask = aton(maskStr);
		broadcast = aton("255.255.255.255");

		for (i = 0; i < 32; i++) {
			if (netmask & (1 << i))
				break;
			broadcast &= ~(1 << i);
		}

		if (broadcast != netmask || strlen(maskStr)>15 ) 
			return -1;
		else {
			strcpy(netMaskBuf, maskStr);
		}
	}

	tcapi_set(nodeName, "SrcMask", netMaskBuf);
	
	SetCfgNodeFlag( QOS_NODENAME, instance-1 );

	return 1;
}

int8 getQosSourceMaskAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aQosSourceMask[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setQosSourceMaskAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aQosSourceMask[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 getQosDestPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[13]={0};
	int instance;
	char nodeName[32];
	char tmpBuf[32];	

	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	memset(tempValue, 0, sizeof(tempValue));

	sprintf(nodeName, QOS_ENTRY, instance-1);
	tcapi_get(nodeName, "DesPortRangeBegin", tmpBuf);//in tr098 spec, dest port is a single number; in our code, dest port is a range, here return the begin dest port 

	if(strlen(tmpBuf) != 0){
		strcpy(tempValue, tmpBuf);
	}
	else {
		tempValue[0]='-';
		tempValue[1]='1';//a value of -1 indicates that this criterion is not used for classification 
	}

	return getTheValue(value, type, deviceNode, tempValue);
	
}
int8 setQosDestPortValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance=0;
	char nodeName[32];
	char tmpBuf[32];	
	int port=0;

	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));

	sprintf(nodeName, QOS_ENTRY, instance-1);	

	if( value==NULL  )
		return -1;
	
	port=*((int *)value);

	if ( port<-1 || port > 65535 ) 
		return -1;
	else {
		if( port == -1 ) {//A value of -1 indicates that this criterion is not used for classification 
			tmpBuf[0] = '\0';
		}
		else {
			sprintf(tmpBuf, "%d", port);
		}
	}
	
	tcapi_set(nodeName, "DesPortRangeBegin", tmpBuf);
	tcapi_set(nodeName, "DesPortRangeEnd", tmpBuf);

	SetCfgNodeFlag( QOS_NODENAME, instance-1 );
	
	return 1;
}

int8 getQosDestPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aQosDestPort[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setQosDestPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aQosDestPort[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 getQosSourcePortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[13]={0};
	int instance;
	char nodeName[32];
	char tmpBuf[32];	

	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	memset(tempValue, 0, sizeof(tempValue));

	sprintf(nodeName, QOS_ENTRY, instance-1);
	tcapi_get(nodeName, "SrcPortRangeBegin", tmpBuf);//in tr098 spec, src port is a single number; in our code, src port is a range, here return the begin src port 

	if(strlen(tmpBuf) != 0){
		strcpy(tempValue, tmpBuf);
	}
	else {
		tempValue[0]='-';
		tempValue[1]='1';//a value of -1 indicates that this criterion is not used for classification 
	}

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setQosSourcePortValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance=0;
	char nodeName[32];
	char tmpBuf[32];	
	int port=0;

	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));

	sprintf(nodeName, QOS_ENTRY, instance-1);	

	if( value==NULL  )
		return -1;
	
	port=*((int *)value);

	if ( port<-1 || port > 65535 ) 
		return -1;
	else {
		if( port == -1 ) {//A value of -1 indicates that this criterion is not used for classification 
			tmpBuf[0] = '\0';
		}
		else {
			sprintf(tmpBuf, "%d", port);
		}
	}

	tcapi_set(nodeName, "SrcPortRangeBegin", tmpBuf);
	tcapi_set(nodeName, "SrcPortRangeEnd", tmpBuf);

	SetCfgNodeFlag( QOS_NODENAME, instance-1 );

	return 1;
}

int8 getQosSourcePortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aQoSSourcePort[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setQosSourcePortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aQoSSourcePort[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}


int8 getQosSourceMACAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[13]={0};
	int instance;
	char nodeName[32];
	char tmpBuf[32];
	
	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	memset(tempValue, 0, sizeof(tempValue));
	
	sprintf(nodeName, QOS_ENTRY, instance-1);
	tcapi_get(nodeName, "SrcMac", tmpBuf);

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setQosSourceMACAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance = 0;
	char nodeName[32];
	char tmpBuf[32];	
	int i = 0;
	char * macStr = NULL;

	instance = atoi(deviceNode->parentNode->name);	
	
	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	
	sprintf(nodeName, QOS_ENTRY, instance-1);
	
	macStr=(char *)value;

	if(macStr==NULL )
		return -1;
	
	if(  strlen(macStr) == 0 )
		tmpBuf[0] = '\0';
	else {
		if(strlen(macStr) != 17)
			return -1;
		
		if(checkMACFormat(macStr) < 0)
			return -1;

		strcpy(tmpBuf, macStr);
	}
	tcapi_set(nodeName, "SrcMac",  tmpBuf);

	SetCfgNodeFlag( QOS_NODENAME, instance-1 );
	
	return 1;
}

int8 getQosSourceMACAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aQoSSourceMACAddress[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setQosSourceMACAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aQoSSourceMACAddress[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}

int8 getQosDestMACAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[13]={0};
	int instance;
	char nodeName[32];
	char tmpBuf[32];	

	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	memset(tempValue, 0, sizeof(tempValue));
	
	sprintf(nodeName, QOS_ENTRY, instance-1);
	tcapi_get(nodeName, "DesMac", tmpBuf);

	return getTheValue(value, type, deviceNode, tempValue);
}
int8 setQosDestMACAddressValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance=0;
	char nodeName[32];
	char tmpBuf[32];	
	int i = 0;
	char * macStr = NULL;

	instance = atoi(deviceNode->parentNode->name);	
	
	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	
	sprintf(nodeName, QOS_ENTRY, instance-1);
	
	macStr=(char *)value;
	if(macStr==NULL )
		return -1;
	
	if(  strlen(macStr) == 0 )
		tmpBuf[0] = '\0';
	else {
		if(strlen(macStr) != 17)
			return -1;
		
		if(checkMACFormat(macStr) < 0)
			return -1;

		strcpy(tmpBuf, macStr);
	}

	tcapi_set(nodeName, "DesMac",  tmpBuf);
	
	SetCfgNodeFlag( QOS_NODENAME, instance-1 );		
	return 1;
}

int8 getQosDestMACAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aQoSDestMACAddress[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setQosDestMACAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aQoSDestMACAddress[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}


/*Policer*/
int8 getQoSPolicerEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8];	
	char nodeName[32];
	char tmpBuf[32];	
	
	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	memset(tempValue, 0, sizeof(tempValue));
	
	sprintf(nodeName, QOS_COMMON);	
	tcapi_get(nodeName, "Active", tmpBuf);
	
	if(strcmp(tmpBuf, "Yes") == 0){
		strcpy(tempValue, "1"); 
	}
	else{
		strcpy(tempValue, "0"); 
	}

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setQoSPolicerEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 *enable = NULL;
	char nodeName[32];
	char tmpBuf[32];	
	
	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	
	sprintf(nodeName, QOS_COMMON);	
	
	enable = (uint8*)value;

	if( *enable == 1 )
		strcpy(tmpBuf, "Yes");
	else if( *enable == 0 )
		strcpy(tmpBuf, "No");
	else
		return -1;

	tcapi_set(nodeName, "Active", tmpBuf);
	
	SetCfgNodeFlag( QOS_NODENAME, -1 );
	
	return 1;
}

int8 getQoSPolicerEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcQoSAttr.aQoSPolicerEnable[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setQoSPolicerEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcQoSAttr.aQoSPolicerEnable[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= QOS_ATTR_FLAG;
	return result;
}
