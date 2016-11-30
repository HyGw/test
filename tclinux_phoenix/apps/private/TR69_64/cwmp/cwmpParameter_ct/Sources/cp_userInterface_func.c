#include "Global_res.h"
#include "cp_userInterface.h"


static boolean checkDateTimeFormat(char *sTime)
{
	int i;
	char strTime[64];
	int timeLen = strlen(sTime);

	if ( timeLen > 64 ||timeLen==0 )
	{
		strcpy(sTime, "0001-01-01T00:00:00Z"); 
		return 0;
	}

	/*
		We do not need to check DateTimeFormat too strictly now.
		The following code could be opend if needed in the future.
	*/
	#if 0
	strcpy(strTime, sTime);

	if ((strTime[4] != '-') || (strTime[7] != '-') || (strTime[13] != ':') 
		|| (strTime[16] != ':') || (strTime[10] != 'T'))
	{
		tc_printf("\r\n  Format is not correct!  ");
		return FALSE;
	}

	for (i=0; i<timeLen; i++)
	{
		if (i==4 || i==7 ||i==13 || i==16 ||i==10 || i==19)
		{
			continue;
		}
		if (strTime[i] >'9' || strTime[i] <'0')
		{
			tc_printf("\r\n 1 strTime[i] >9 || strTime[i] <0  ");
			return FALSE;
		}
	}
	#endif

	return 1;
}

int8 getWarrantyDateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char warrantyDate[64];
	char tmpBuf[64];

	memset(warrantyDate, 0, sizeof(warrantyDate));
	memset(tmpBuf, 0, sizeof(tmpBuf));
	getCwmpAttrValue(Warranty_Date, tmpBuf, sizeof(tmpBuf));

	if(checkDateTimeFormat(tmpBuf)) {
		strcpy(warrantyDate, tmpBuf); 
	}
	else {	
		/*
			if the format of WarrantyDate is not correct, eg. when CPE is 
			reset to factory configuration, vWarrantyDate is empty, then 
			set vWarrantyDate to 0001-01-01T00:00:00Z.
		*/
		tc_printf("\r\n Date Time Format is not correct, it will be reset to 0001-01-01T00:00:00Z");
		strcpy(warrantyDate, "0001-01-01T00:00:00Z"); 	
	}
	
	return getTheValue(value, type, deviceNode, warrantyDate);
}


int8 setWarrantyDateValue(void* value, struct DeviceNode*  deviceNode)
{
	char* tempValue = (char *)value;

	if (checkDateTimeFormat(tempValue))
	{
		setCwmpAttrValue(Warranty_Date, tempValue);

		SetCfgNodeFlag( "Cwmp", -1 );

		return 0;
	}
	else
	{
		tc_printf("\r\n Date Time Format is not correct!");
		return -1;
	}
}


int8 getWarrantyDateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;

	attribute = tcUserInterfaceAttr.aWarrantyDate;	
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}


int8 setWarrantyDateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;

	attribute = &tcUserInterfaceAttr.aWarrantyDate;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);	

	rpc.attrFlags.theSettingFlag0 |= USERITF_ATTR_FLAG;

	return result;
}


