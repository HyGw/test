#include "Global_res.h"
#include "cp_DeviceInfo.h"

#if defined(TCSUPPORT_CT)
extern int needReboot;
#endif
extern int g_AlarmEnable;
extern int g_MonitorEnable;

int8 getManufacturerValue(char** value, ParameterType* type,  DeviceNode*  deviceNode)
{
	char manufacture[64];
	
	memset( manufacture, 0, sizeof(manufacture) );
	
	strcpy(manufacture, getCwmpManufacture());
	return getTheValue(value, type, deviceNode, manufacture);
}
int8 getManufacturerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aManufacturer;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setManufacturerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aManufacturer;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getManufacturerOUIValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ManuOUI[32];

	memset( ManuOUI, 0, sizeof(ManuOUI) );
	
	strcpy(ManuOUI, getCwmpOUI());
	return getTheValue(value, type, deviceNode, ManuOUI);
}
int8 getManufacturerOUIAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aManufacturerOUI;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setManufacturerOUIAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aManufacturerOUI;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getModelNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ModelName[64];

	memset( ModelName, 0, sizeof(ModelName) );
	
	strcpy(ModelName,getCwmpModelName());

	return getTheValue(value, type, deviceNode, ModelName);
}
int8 getModelNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aModelName;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setModelNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aModelName;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getDescriptionValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char Description[256];

	memset( Description, 0, sizeof(Description) );
	
	strcpy(Description,getDescription());
	
	return getTheValue(value, type, deviceNode, Description);
}
int8 getDescriptionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aDescription;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDescriptionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aDescription;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getProductClassValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char productClass[64];

	memset( productClass, 0, sizeof(productClass) );
	
	strcpy(productClass,getCwmpProductClass());
	
	return getTheValue(value, type, deviceNode, productClass);
}
int8 getProductClassAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aProductClass;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setProductClassAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aProductClass;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getSerialNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char SerialNumber[129];
	
	memset( SerialNumber, 0, sizeof(SerialNumber) );
	
	strcpy(SerialNumber, getCwmpSerialNum());
	
	return getTheValue(value, type, deviceNode, SerialNumber);
}
int8 getSerialNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aSerialNumber;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSerialNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aSerialNumber;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getHardwareVersionValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char hwVersion[64];

	memset( hwVersion, 0, sizeof(hwVersion) );

	/*
	  here we use only one flag to juge if we want to get 
	  Customer's Hardware version or Trendchip's Hardware version.
	*/
	
	//if ( getFirmwareVersionFlag() ) {
	if ( getUserCustomizeFlag() ) {
		/*Customer's Hardware version*/
		#if 0
		strcpy(hwVersion, getCustomerHardwareVer());
		#endif
		tcapi_get( "DeviceInfo_devParaStatic", "CustomerHWVersion", hwVersion );
		
	}
	else {
		
		/*Trendchip's Hardware version*/
		strcpy(hwVersion, getHardwareVer());	
	}

	return getTheValue(value, type, deviceNode, hwVersion);
}
int8 getHardwareVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aHardwareVersion;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setHardwareVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aHardwareVersion;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getSoftwareVersionValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char SoftwareVersion[64];

	memset( SoftwareVersion, 0, sizeof(SoftwareVersion) );
#if 0
	if ( getFirmwareVersionFlag() ) {
		/*Customer's Firmware version*/
	#ifdef NEW_CUSTOMER_VERSION
		strcpy(SoftwareVersion, (char*)RetRASStrPtrV1());
	#else
		strcpy(SoftwareVersion, (char*)RetRASSoftwarever());
	#endif
	}
	else {
		/*Trendchip's Firmware version*/
		strcpy(SoftwareVersion, (char*)RetRASStrPtr2());
	}
#endif
	//if ( getFirmwareVersionFlag() ) {
	if ( getUserCustomizeFlag() ) {	
		/*Customer's Firmware version*/
		#if 0
		tcapi_get( "Info_devDefInf", "CustomerSoftwareVersion", SoftwareVersion );
		#endif
		tcapi_get( "DeviceInfo_devParaStatic", "CustomerSWVersion", SoftwareVersion );
	}
	else {
		
		/*Trendchip's Firmware version*/
		strcpy( SoftwareVersion, (char*)RetRASStrPtr2() );
	}

	return getTheValue(value, type, deviceNode, SoftwareVersion);
}
int8 getSoftwareVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aSoftwareVersion;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSoftwareVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aSoftwareVersion;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getSpecVersionValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	/*merge from linos, but it may be change in linux*/
	
	char SpecVersion[16];

#if defined(TCSUPPORT_CT_PON_FHT)
	strcpy(SpecVersion, "E00R1A");	
#else
	strcpy(SpecVersion, "1.0");	
#endif

	return getTheValue(value, type, deviceNode, SpecVersion);
}
int8 getSpecVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aSpecVersion;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setSpecVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aSpecVersion;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getProvisioningCodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ProvisioningCode[64];

	strcpy(ProvisioningCode, getProvisionCode());

	return getTheValue(value, type, deviceNode, ProvisioningCode);
}
int8 setProvisioningCodeValue(void* value, struct DeviceNode*  deviceNode)
{
	char* ProvisioningCode;

	SetCfgNodeFlag( "Cwmp", -1 );
	
	if(strlen((char*)value) > 63)
		return -1;
	
	ProvisioningCode = (char*)value;
	
	setProvisionCode(ProvisioningCode);

	return 0;
}
int8 getProvisioningCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aProvisioningcode;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setProvisioningCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aProvisioningcode;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getUpTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char  retValue[32];

	memset( retValue, 0, sizeof(retValue) );
	
	sprintf(retValue,"%d",secclock());
	return getTheValue(value, type, deviceNode, retValue);
}
int8 getUpTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aUpTime;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);

	if(*notification == Active)   //reject Active notification. xzwang_060906
		*notification = None;
	
	return result;
}
int8 setUpTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aUpTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getDeviceLogValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	FILE *fp = NULL;
	char *buf = NULL;
	int rtnCode = 0;
	char logsysfile[128];

	unsigned long logSize = 0;
	
#ifndef TCSUPPORT_SYSLOG
	memset( logsysfile, 0, sizeof(logsysfile) );
	
	/*write log to file*/
	sprintf( logsysfile, "dmesg > %s", UPLOAD_LOG_FILE_PATH );
	system( logsysfile );
#else
	/* generate syslog file */
	get_syslog_file(UPLOAD_LOG_FILE_PATH);
#endif
	
	/*open log file*/
	fp = fopen( UPLOAD_LOG_FILE_PATH, "rb" );
	if( fp == NULL ) {
		tc_printf( "==> getDeviceLogValue(): open log fil fail.\n" );
		unlink( UPLOAD_LOG_FILE_PATH );
		
		return getTheValue(value, type, deviceNode, "");
	}

	/*caluate the size of this log file*/
	fseek( fp, 0L, SEEK_END );
    	logSize = ftell( fp );
    	rewind(fp);

	/*allocate buf to store log*/
	buf = os_malloc( logSize + 1 );
	if ( buf == NULL ) {
		tc_printf( "==> getDeviceLogValue(): allocate buf fail.\n" );
		unlink( UPLOAD_LOG_FILE_PATH );
		fclose(fp);
		
		return getTheValue(value, type, deviceNode, "");
	}
	memset( buf, 0, logSize + 1 );

	/*read log from file*/
	fread( buf, logSize, 1, fp );	
	fclose( fp );
	unlink( UPLOAD_LOG_FILE_PATH );

	rtnCode = getTheValue( value , type, deviceNode, buf );
	
	free(buf);

	return rtnCode;
}
int8 getDeviceLogAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aDeviceLog;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDeviceLogAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aDeviceLog;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getVendorCfgFileNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char VendorCfgFileName[] = "romfile.cfg";

	return getTheValue(value, type, deviceNode, VendorCfgFileName);
}
int8 getVendorCfgFileNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aVendorCfgFileName;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setVendorCfgFileNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aVendorCfgFileName;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getVendorCfgFileDescripValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	/*merge from linos*/
	char VendorCfgFileDescrip[] = "The customer setting of CPE";
	
	return getTheValue(value, type, deviceNode, VendorCfgFileDescrip);
}
int8 getVendorCfgFileDescripAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aVendorCfgFileDescrip;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setVendorCfgFileDescripAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aVendorCfgFileDescrip;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}


#if defined(TCSUPPORT_CT_NETWORKMANAGESERVICE)
#if !defined(TCSUPPORT_C1_CUC)
int8 getFtpEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ftpEnable[10]= {0};

	tcapi_get(FTP_LOGIN_ENTRY,"Active",ftpEnable);
	if(strcmp(ftpEnable, "Yes") == 0)
	{
		strcpy(ftpEnable, "1");
	}
	else
	{
		strcpy(ftpEnable, "0");
	}
	
	return getTheValue(value, type, deviceNode, ftpEnable);
}

int8	setFtpEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 flag;
	
	flag = *((uint8*)value);

	SetCfgNodeFlag (FTP_LOGIN_ENTRY, -1);
	
	if(flag == 1){
		tcapi_set(FTP_LOGIN_ENTRY,"Active","Yes");
	} 
	else if (flag == 0){
		tcapi_set(FTP_LOGIN_ENTRY,"Active","No");
	}
	else{
		tc_printf("==>setFtpEnableValue:value should be 0 or 1.\n");
		return -1;
	}	
	
	return 0;
}

int8	getFtpEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aFtpEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setFtpEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aFtpEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getFtpUserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ftpUserName[256]= {0};

	tcapi_get(FTP_LOGIN_ENTRY,"ftp_username",ftpUserName);
	
	return getTheValue(value, type, deviceNode, ftpUserName);
}

int8	setFtpUserNameValue(void* value, struct DeviceNode*  deviceNode)
{
	int len = 0;
	SetCfgNodeFlag (FTP_LOGIN_ENTRY, -1);
	
	len = strlen((char*)value);
	tc_printf("\r\nvalue is %s",(char*)value);
	if(len > 255)
	{
		tc_printf("\r\n==>setFtpPortValue:username is too long,the max len is 255!");
		return -1;
	}
	
	if(0 == len)
	{
		//tcapi_set(FTP_LOGIN_ENTRY,"ftp_username","admin");
	}
	else
	{
		tcapi_set(FTP_LOGIN_ENTRY,"ftp_username", (char*)value);
	}
	
	return 0;
}

int8	getFtpUserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aFtpUserName;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setFtpUserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aFtpUserName;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getFtpPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ftpPassword[256]= {0};

	tcapi_get(FTP_LOGIN_ENTRY,"ftp_passwd",ftpPassword);
#if defined(TCSUPPORT_CT_FJ)
	strcpy(ftpPassword,"");
#endif
	
	return getTheValue(value, type, deviceNode, ftpPassword);
}

int8	setFtpPasswordValue(void* value, struct DeviceNode*  deviceNode)
{
	int len = 0;
	SetCfgNodeFlag (FTP_LOGIN_ENTRY, -1);
	
	len = strlen((char*)value);
	tc_printf("\r\nvalue is %s",(char*)value);
	if(len > 255)
	{
		tc_printf("\r\n==>setFtpPortValue:password is too long,the max len is 255!");
		return -1;
	}
	
	if(0 == len)
	{
		//tcapi_set(FTP_LOGIN_ENTRY,"ftp_passwd","1234");
	}
	else
	{
		tcapi_set(FTP_LOGIN_ENTRY,"ftp_passwd", (char*)value);
	}
	
	return 0;
}

int8	getFtpPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aFtpPassword;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setFtpPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aFtpPassword;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getFtpPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char ftpport[15]= {0};

	tcapi_get(FTP_LOGIN_ENTRY,"ftp_port",ftpport);
	
	return getTheValue(value, type, deviceNode, ftpport);
}

int8	setFtpPortValue(void* value, struct DeviceNode*  deviceNode)
{
	int ftpport = 0;
	char tmpbuf[15] = {0};
	
	SetCfgNodeFlag (FTP_LOGIN_ENTRY, -1);
	
	if( value == NULL)
		return -1;
	
	ftpport=*((int *)value);

	if ( ftpport < -1 || ftpport > 65535 ) 
	{
		tc_printf("\r\n==>setFtpPortValue:port is not valid!");
		return -1;
	
	}
	else
	{
		if( ftpport == -1 ) //if set -1 mean use ftp default port:21
		{
			ftpport = 21;
		}
		sprintf(tmpbuf, "%d", ftpport);
	}
	
	tcapi_set(FTP_LOGIN_ENTRY,"ftp_port", tmpbuf);
	tc_printf("\r\nvalue is %s",tmpbuf);
	return 0;
	
}

int8	getFtpPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aFtpPort;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setFtpPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aFtpPort;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getTelnetEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char telnetEnable[10]= {0};

	tcapi_get(TELNET_LOGIN_ENTRY,"Active",telnetEnable);
	if(strcmp(telnetEnable, "Yes") == 0)
	{
		strcpy(telnetEnable, "1");
	}
	else
	{
		strcpy(telnetEnable, "0");
	}
	
	return getTheValue(value, type, deviceNode, telnetEnable);
}

int8	setTelnetEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 flag;
	
	flag = *((uint8*)value);

	SetCfgNodeFlag (TELNET_LOGIN_ENTRY, -1);
	
	if(flag == 1){
		tcapi_set(TELNET_LOGIN_ENTRY,"Active","Yes");
	} 
	else if (flag == 0){
		tcapi_set(TELNET_LOGIN_ENTRY,"Active","No");
	}
	else{
		tc_printf("==>setTelnetEnableValue:value should be 0 or 1.\n");
		return -1;
	}	
	
	return 0;
}

int8	getTelnetEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aTelnetEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setTelnetEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aTelnetEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getTelnetUserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char telnetUserName[256]= {0};

	tcapi_get(TELNET_LOGIN_ENTRY,"telnet_username",telnetUserName);
	
	return getTheValue(value, type, deviceNode, telnetUserName);
}

int8	setTelnetUserNameValue(void* value, struct DeviceNode*  deviceNode)
{
	int len = 0;
	SetCfgNodeFlag (TELNET_LOGIN_ENTRY, -1);
	
	len = strlen((char*)value);
	tc_printf("\r\nvalue is %s",(char*)value);
	if(len > 255)
	{
		tc_printf("\r\n==>setTelnetUserNameValue:username is too long,the max len is 255!");
		return -1;
	}
	
	if(0 == len)
	{
		//tcapi_set(TELNET_LOGIN_ENTRY,"telnet_username","admin");
	}
	else
	{
		tcapi_set(TELNET_LOGIN_ENTRY,"telnet_username", (char*)value);
	}
	
	return 0;
}

int8	getTelnetUserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aTelnetUserName;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setTelnetUserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aTelnetUserName;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getTelnetPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char telnetPassword[256]= {0};

	tcapi_get(TELNET_LOGIN_ENTRY,"telnet_passwd",telnetPassword);
#if defined(TCSUPPORT_CT_FJ)
	strcpy(telnetPassword,"");
#endif
	
	return getTheValue(value, type, deviceNode, telnetPassword);
}

int8	setTelnetPasswordValue(void* value, struct DeviceNode*  deviceNode)
{
	int len = 0;
	SetCfgNodeFlag (TELNET_LOGIN_ENTRY, -1);
	
	len = strlen((char*)value);
	tc_printf("\r\nvalue is %s",(char*)value);
	if(len > 255)
	{
		tc_printf("\r\n==>setTelnetPasswordValue:password is too long,the max len is 255!");
		return -1;
	}
	
	if(0 == len)
	{
		//tcapi_set(TELNET_LOGIN_ENTRY,"telnet_passwd","1234");
	}
	else
	{
		tcapi_set(TELNET_LOGIN_ENTRY,"telnet_passwd", (char*)value);
	}
	
	return 0;
}

int8	getTelnetPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aTelnetPassword;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setTelnetPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aTelnetPassword;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getTelnetPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char telnetport[15]= {0};

	tcapi_get(TELNET_LOGIN_ENTRY,"telnet_port",telnetport);
	
	return getTheValue(value, type, deviceNode, telnetport);
}

int8	setTelnetPortValue(void* value, struct DeviceNode*  deviceNode)
{
	int telnetport = 0;
	char tmpbuf[15] = {0};
	
	SetCfgNodeFlag (TELNET_LOGIN_ENTRY, -1);
	
	if( value == NULL)
		return -1;
	
	telnetport=*((int *)value);

	if ( telnetport < -1 || telnetport > 65535 ) 
	{
		tc_printf("\r\n==>setTelnetPortValue:port is not valid!");
		return -1;
	
	}
	else
	{
		if( telnetport == -1 ) //if set -1 mean use telnet default port:23
		{
			telnetport = 23;
		}
		sprintf(tmpbuf, "%d", telnetport);
	}
	
	tcapi_set(TELNET_LOGIN_ENTRY,"telnet_port", tmpbuf);
	return 0;
}

int8	getTelnetPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aTelnetPort;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setTelnetPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aTelnetPort;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}
#if defined(TCSUPPORT_CT_PON_SC)
int8	getSshEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char sshEnable[10]= {0};

	tcapi_get(SSH_LOGIN_ENTRY,"Active",sshEnable);
	if(strcmp(sshEnable, "Yes") == 0)
	{
		strcpy(sshEnable, "1");
	}
	else
	{
		strcpy(sshEnable, "0");
	}
	
	return getTheValue(value, type, deviceNode, sshEnable);
}

int8	setSshEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 flag;
	
	flag = *((uint8*)value);

	SetCfgNodeFlag (SSH_LOGIN_ENTRY, -1);
	
	if(flag == 1){
		tcapi_set(SSH_LOGIN_ENTRY,"Active","Yes");
	} 
	else if (flag == 0){
		tcapi_set(SSH_LOGIN_ENTRY,"Active","No");
	}
	else{
		tc_printf("==>setSshEnableValue:value should be 0 or 1.\n");
		return -1;
	}	
	
	return 0;
}

int8	getSshEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aSshEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setSshEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aSshEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8	getSshUserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char sshUserName[256]= {0};

	tcapi_get(SSH_LOGIN_ENTRY,"ssh_username",sshUserName);
	
	return getTheValue(value, type, deviceNode, sshUserName);
}

int8	setSshUserNameValue(void* value, struct DeviceNode*  deviceNode)
{
	int len = 0;
	SetCfgNodeFlag (SSH_LOGIN_ENTRY, -1);
	
	len = strlen((char*)value);
	tc_printf("\r\nvalue is %s",(char*)value);
	if(len > 255)
	{
		tc_printf("\r\n==>setSshUserNameValue:username is too long,the max len is 255!");
		return -1;
	}
	
	if(0 == len)
	{
		//tcapi_set(SSH_LOGIN_ENTRY,"ssh_username","admin");
	}
	else
	{
		tcapi_set(SSH_LOGIN_ENTRY,"ssh_username", (char*)value);
	}
	
	return 0;
}

int8	getSshUserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aSshUserName;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setSshUserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aSshUserName;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8	getSshPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char sshPassword[256]= {0};

	tcapi_get(SSH_LOGIN_ENTRY,"ssh_passwd",sshPassword);
	
	return getTheValue(value, type, deviceNode, sshPassword);
}

int8	setSshPasswordValue(void* value, struct DeviceNode*  deviceNode)
{
	int len = 0;
	SetCfgNodeFlag (SSH_LOGIN_ENTRY, -1);
	
	len = strlen((char*)value);
	tc_printf("\r\nvalue is %s",(char*)value);
	if(len > 255)
	{
		tc_printf("\r\n==>setSshPasswordValue:password is too long,the max len is 255!");
		return -1;
	}
	
	if(0 == len)
	{
		//tcapi_set(SSH_LOGIN_ENTRY,"ssh_passwd","1234");
	}
	else
	{
		tcapi_set(SSH_LOGIN_ENTRY,"ssh_passwd", (char*)value);
	}
	
	return 0;
}

int8	getSshPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aSshPassword;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setSshPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aSshPassword;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8	getSshPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char sshport[15]= {0};

	tcapi_get(SSH_LOGIN_ENTRY,"ssh_port",sshport);
	
	return getTheValue(value, type, deviceNode, sshport);
}

int8	setSshPortValue(void* value, struct DeviceNode*  deviceNode)
{
	int sshport = 0;
	char tmpbuf[15] = {0};
	
	SetCfgNodeFlag (SSH_LOGIN_ENTRY, -1);
	
	if( value == NULL)
		return -1;
	
	sshport=*((int *)value);

	if ( sshport < -1 || sshport > 65535 ) 
	{
		tc_printf("\r\n==>setTelnetPortValue:port is not valid!");
		return -1;
	
	}
	else
	{
		if( sshport == -1 )
		{
			sshport = 22;
		}
		sprintf(tmpbuf, "%d", sshport);
	}
	
	tcapi_set(SSH_LOGIN_ENTRY,"ssh_port", tmpbuf);
	return 0;
}

int8	getSshPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aSshPort;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setSshPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aSshPort;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8	getTftpEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tftpEnable[10]= {0};

	tcapi_get(TFTP_LOGIN_ENTRY,"Active",tftpEnable);
	if(strcmp(tftpEnable, "Yes") == 0)
	{
		strcpy(tftpEnable, "1");
	}
	else
	{
		strcpy(tftpEnable, "0");
	}
	
	return getTheValue(value, type, deviceNode, tftpEnable);
}

int8	setTftpEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 flag;
	
	flag = *((uint8*)value);

	SetCfgNodeFlag (TFTP_LOGIN_ENTRY, -1);
	
	if(flag == 1){
		tcapi_set(TFTP_LOGIN_ENTRY,"Active","Yes");
	} 
	else if (flag == 0){
		tcapi_set(TFTP_LOGIN_ENTRY,"Active","No");
	}
	else{
		tc_printf("==>setTftpEnableValue:value should be 0 or 1.\n");
		return -1;
	}	
	
	return 0;
}

int8	getTftpEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aTftpEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setTftpEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aTftpEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8	getTftpUserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tftpUserName[256]= {0};

	tcapi_get(TFTP_LOGIN_ENTRY,"tftp_username",tftpUserName);
	
	return getTheValue(value, type, deviceNode, tftpUserName);
}

int8	setTftpUserNameValue(void* value, struct DeviceNode*  deviceNode)
{
	int len = 0;
	SetCfgNodeFlag (TFTP_LOGIN_ENTRY, -1);
	
	len = strlen((char*)value);
	tc_printf("\r\nvalue is %s",(char*)value);
	if(len > 255)
	{
		tc_printf("\r\n==>setTftpUserNameValue:username is too long,the max len is 255!");
		return -1;
	}
	
	if(0 == len)
	{
		//tcapi_set(TFTP_LOGIN_ENTRY,"tftp_username","admin");
	}
	else
	{
		tcapi_set(TFTP_LOGIN_ENTRY,"tftp_username", (char*)value);
	}
	
	return 0;
}

int8	getTftpUserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aTftpUserName;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setTftpUserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aTftpUserName;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8	getTftpPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tftpPassword[256]= {0};

	tcapi_get(TFTP_LOGIN_ENTRY,"tftp_passwd",tftpPassword);

	return getTheValue(value, type, deviceNode, tftpPassword);
}

int8	setTftpPasswordValue(void* value, struct DeviceNode*  deviceNode)
{
	int len = 0;
	SetCfgNodeFlag (TFTP_LOGIN_ENTRY, -1);
	
	len = strlen((char*)value);
	tc_printf("\r\nvalue is %s",(char*)value);
	if(len > 255)
	{
		tc_printf("\r\n==>setTftpPasswordValue:password is too long,the max len is 255!");
		return -1;
	}
	
	if(0 == len)
	{
		//tcapi_set(TFTP_LOGIN_ENTRY,"tftp_passwd","1234");
	}
	else
	{
		tcapi_set(TFTP_LOGIN_ENTRY,"tftp_passwd", (char*)value);
	}
	
	return 0;
}

int8	getTftpPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aTftpPassword;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setTftpPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aTftpPassword;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8	getTftpPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tftpport[15]= {0};

	tcapi_get(TFTP_LOGIN_ENTRY,"tftp_port",tftpport);
	
	return getTheValue(value, type, deviceNode, tftpport);
}

int8	setTftpPortValue(void* value, struct DeviceNode*  deviceNode)
{
	int tftpport = 0;
	char tmpbuf[15] = {0};
	
	SetCfgNodeFlag (TFTP_LOGIN_ENTRY, -1);
	
	if( value == NULL)
		return -1;
	
	tftpport=*((int *)value);

	if ( tftpport < -1 || tftpport > 65535 ) 
	{
		tc_printf("\r\n==>setTftpPortValue:port is not valid!");
		return -1;
	
	}
	else
	{
		if( tftpport == -1 ) 
		{
			tftpport = 69;
		}
		sprintf(tmpbuf, "%d", tftpport);
	}
	
	tcapi_set(TFTP_LOGIN_ENTRY,"tftp_port", tmpbuf);
	return 0;
}

int8	getTftpPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aTftpPort;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setTftpPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aTftpPort;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8	getSnmpEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char snmpEnable[10]= {0};

	tcapi_get(SNMP_LOGIN_ENTRY,"Active",snmpEnable);
	if(strcmp(snmpEnable, "Yes") == 0)
	{
		strcpy(snmpEnable, "1");
	}
	else
	{
		strcpy(snmpEnable, "0");
	}
	
	return getTheValue(value, type, deviceNode, snmpEnable);
}

int8	setSnmpEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 flag;
	
	flag = *((uint8*)value);

	SetCfgNodeFlag (SNMP_LOGIN_ENTRY, -1);
	
	if(flag == 1){
		tcapi_set(SNMP_LOGIN_ENTRY,"Active","Yes");
	} 
	else if (flag == 0){
		tcapi_set(SNMP_LOGIN_ENTRY,"Active","No");
	}
	else{
		tc_printf("==>setSnmpEnableValue:value should be 0 or 1.\n");
		return -1;
	}	
	
	return 0;
}

int8	getSnmpEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aSnmpEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setSnmpEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aSnmpEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8	getSnmpUserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char snmpUserName[256]= {0};

	tcapi_get(SNMP_LOGIN_ENTRY,"snmp_username",snmpUserName);
	
	return getTheValue(value, type, deviceNode, snmpUserName);
}

int8	setSnmpUserNameValue(void* value, struct DeviceNode*  deviceNode)
{
	int len = 0;
	SetCfgNodeFlag (SNMP_LOGIN_ENTRY, -1);
	
	len = strlen((char*)value);
	tc_printf("\r\nvalue is %s",(char*)value);
	if(len > 255)
	{
		tc_printf("\r\n==>setSnmpUserNameValue:username is too long,the max len is 255!");
		return -1;
	}
	
	if(0 == len)
	{
		//tcapi_set(SNMP_LOGIN_ENTRY,"snmp_username","admin");
	}
	else
	{
		tcapi_set(SNMP_LOGIN_ENTRY,"snmp_username", (char*)value);
	}
	
	return 0;
}

int8	getSnmpUserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aSnmpUserName;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setSnmpUserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aSnmpUserName;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8	getSnmpPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char snmpPassword[256]= {0};

	tcapi_get(SNMP_LOGIN_ENTRY,"snmp_passwd",snmpPassword);
	
	return getTheValue(value, type, deviceNode, snmpPassword);
}

int8	setSnmpPasswordValue(void* value, struct DeviceNode*  deviceNode)
{
	int len = 0;
	SetCfgNodeFlag (SNMP_LOGIN_ENTRY, -1);
	
	len = strlen((char*)value);
	tc_printf("\r\nvalue is %s",(char*)value);
	if(len > 255)
	{
		tc_printf("\r\n==>setSnmpPasswordValue:password is too long,the max len is 255!");
		return -1;
	}
	
	if(0 == len)
	{
		//tcapi_set(SNMP_LOGIN_ENTRY,"snmp_passwd","1234");
	}
	else
	{
		tcapi_set(SNMP_LOGIN_ENTRY,"snmp_passwd", (char*)value);
	}
	
	return 0;
}

int8	getSnmpPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aSnmpPassword;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setSnmpPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aSnmpPassword;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8	getSnmpPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char snmpport[15]= {0};

	tcapi_get(SNMP_LOGIN_ENTRY,"snmp_port",snmpport);
	
	return getTheValue(value, type, deviceNode, snmpport);
}

int8	setSnmpPortValue(void* value, struct DeviceNode*  deviceNode)
{
	int snmpport = 0;
	char tmpbuf[15] = {0};
	
	SetCfgNodeFlag (SNMP_LOGIN_ENTRY, -1);
	
	if( value == NULL)
		return -1;
	
	snmpport=*((int *)value);

	if ( snmpport < -1 || snmpport > 65535 ) 
	{
		tc_printf("\r\n==>setSnmpPortValue:port is not valid!");
		return -1;
	
	}
	else
	{
		if( snmpport == -1 ) 
		{
			snmpport = 161;
		}
		sprintf(tmpbuf, "%d", snmpport);
	}
	
	tcapi_set(SNMP_LOGIN_ENTRY,"snmp_port", tmpbuf);
	return 0;
}

int8	getSnmpPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aSnmpPort;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setSnmpPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aSnmpPort;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}
#endif
#endif
int8	getMaintenanceAccountEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char MaintenanceAccountEnable[10]= {0};

	tcapi_get(MAINTENANCEACCOUNT_LOGIN_ENTRY,"Active",MaintenanceAccountEnable);
	if(strcmp(MaintenanceAccountEnable, "Yes") == 0)
	{
		strcpy(MaintenanceAccountEnable, "1");
	}
	else
	{
		strcpy(MaintenanceAccountEnable, "0");
	}
	return getTheValue(value, type, deviceNode, MaintenanceAccountEnable);
}

int8	setMaintenanceAccountEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 flag;
	char oldState[12] = {0};
	
	flag = *((uint8*)value);

	if ( 0 != tcapi_get(MAINTENANCEACCOUNT_LOGIN_ENTRY, "Active", oldState) )
		strcpy(oldState, "No");
	SetCfgNodeFlag (ACCCOUNT, 0);
	
	if(flag == 1){
		tcapi_set(MAINTENANCEACCOUNT_LOGIN_ENTRY,"Active","Yes");
	} 
	else if (flag == 0){
		if ( 0 == strcmp(oldState, "Yes") )
			tcapi_set(MAINTENANCEACCOUNT_LOGIN_ENTRY, "Logged", "0");
		tcapi_set(MAINTENANCEACCOUNT_LOGIN_ENTRY,"Active","No");
	}
	else{
		tc_printf("==>setTelnetEnableValue:value should be 0 or 1.\n");
		return -1;
	}	
	
	return 0;
}

int8	getMaintenanceAccountEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aMaintenanceAccountEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setMaintenanceAccountEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aMaintenanceAccountEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

#if !defined(TCSUPPORT_C1_CUC)
int8	getMaintenanceAccountUsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char MaintenanceAccountUsername[256]= {0};

	tcapi_get(MAINTENANCEACCOUNT_LOGIN_ENTRY,"username",MaintenanceAccountUsername);
	
	return getTheValue(value, type, deviceNode, MaintenanceAccountUsername);
}

int8	setMaintenanceAccountUsernameValue(void* value, struct DeviceNode*  deviceNode)
{
	int len = 0;
	SetCfgNodeFlag (ACCCOUNT, 0);
	
	len = strlen((char*)value);
	tc_printf("\r\nvalue is %s",(char*)value);
	if(len > 255)
	{
		tc_printf("\r\n==>setMaintenanceAccountUsernameValue:username is too long,the max len is 255!");
		return -1;
	}
	
	if(0 == len)
	{
		//tcapi_set(MAINTENANCEACCOUNT_LOGIN_ENTRY,"username","telecomadmin");
	}
	else
	{
		tcapi_set(MAINTENANCEACCOUNT_LOGIN_ENTRY,"username", (char*)value);
	}
	
	return 0;
}

int8	getMaintenanceAccountUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aMaintenanceAccountUsername;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setMaintenanceAccountUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aMaintenanceAccountUsername;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

#endif
int8	getMaintenanceAccountPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char MaintenanceAccountPassword[256]= {0};

	tcapi_get(MAINTENANCEACCOUNT_LOGIN_ENTRY,"web_passwd",MaintenanceAccountPassword);
#if defined(TCSUPPORT_CT_FJ)
	strcpy(MaintenanceAccountPassword,"");
#endif
	
	return getTheValue(value, type, deviceNode, MaintenanceAccountPassword);
}

int8	setMaintenanceAccountPasswordValue(void* value, struct DeviceNode*  deviceNode)
{
	int len = 0;
	char oldPSW[256] = {0};

	SetCfgNodeFlag (ACCCOUNT, 0);
	
	len = strlen((char*)value);
	tc_printf("\r\nvalue is %s",(char*)value);
	if(len > 255)
	{
		tc_printf("\r\n==>setMaintenanceAccountPasswordValue:password is too long,the max len is 255!");
		return -1;
	}
	if ( 0 != tcapi_get(MAINTENANCEACCOUNT_LOGIN_ENTRY, "web_passwd", oldPSW) )
		strcpy(oldPSW, "nE7jA%5m");
	if ( 0 != strcmp(oldPSW, (char*)value) )
		tcapi_set(MAINTENANCEACCOUNT_LOGIN_ENTRY, "Logged", "0");
	if(0 == len)
	{
		tcapi_set(MAINTENANCEACCOUNT_LOGIN_ENTRY,"web_passwd","nE7jA%5m");
	}
	else
	{
		tcapi_set(MAINTENANCEACCOUNT_LOGIN_ENTRY,"web_passwd", (char*)value);
	}
	
	return 0;
}

int8	getMaintenanceAccountPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aMaintenanceAccountPassword;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setMaintenanceAccountPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aMaintenanceAccountPassword;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}
#endif

#if defined(TCSUPPORT_CT_MIDWARE)
int tr069EnableCommitFunc(cfgNode_ptr pCfgNode)
{
	tc_printf("tr069EnableCommitFunc!\n");
	tcapi_commit("Cwmp_Entry");
	return 0;
}

int8	getTr069EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempTr069Enable[10]= {0};

	getCwmpAttrValue(Tr069Enable,tempTr069Enable,sizeof(tempTr069Enable));
	
	return getTheValue(value, type, deviceNode, tempTr069Enable);
}

/*
	check  tr069 enable's value can be set or not.
	tr069enable value    |     1       |   0        |    2
	------------------------------------------
		1			  |	  N	    |    Y       |    Y	
		0			  |	  Y	    |    N       |    N	
		2			  |	  Y	    |    N       |    N		
*/
int checkTr069Status(int value)
{
	static int lastValue = 0xff;
	int tempLast = 0;
	char tempTr069Enable[10]= {0};
	
	if(lastValue == 0xff){
		getCwmpAttrValue(Tr069Enable,tempTr069Enable,sizeof(tempTr069Enable));
		lastValue = atoi(tempTr069Enable);
	}
	if((value!=0) &&(value!=1) && (value!=2))
		return -1;
	
	if(lastValue == value
		||(lastValue==0 && value==2)
		||(lastValue==2 && value==0))
		return -1;
	tempLast = lastValue;
	lastValue = value;
	return tempLast;
}

int8	setTr069EnableValue(void* value, struct DeviceNode*  deviceNode)
{
	int flag = 0;
	int lastValue = -1;
	
	SetCfgNodeFlag( "Cwmp_Tr069Enable", -1 );
	flag = *((int*)value);
	
	if((lastValue= checkTr069Status(flag)) == -1)
		return -1;
	
	if(flag == 0){
		setCwmpAttrValue(Tr069Enable,"0");
		if(lastValue == 1)
			return 1;
		else 
			return 0;
	} 
	else if (flag == 1){
		setCwmpAttrValue(Tr069Enable,"1");
		if(lastValue == 0){
			return 1;
		}
		else{
			return 0;
		}
	}
	else if(flag == 2){
		setCwmpAttrValue(Tr069Enable,"2");
		return 0;
	}
	else{
		tc_printf("==>setTr069EnableValue:value should be 0  1 or 2.\n");
		return -1;
	}	
	return 0;
}

int8	getTr069EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;

	attribute = tcDeviceInfoAttr.aTr069Enable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setTr069EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;

	attribute = &tcDeviceInfoAttr.aTr069Enable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getMiddlewareURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char URL[256] = "0.0.0.0";
	char port[10] = "0";
	getCwmpAttrValue(MWMgtUrl,URL,sizeof(URL));
	getCwmpAttrValue(MWMgtPort,port,sizeof(port));
	strcat(URL, ":");
	strcat(URL, port);
	return getTheValue(value, type, deviceNode, URL);
}
int8 setMiddlewareURLValue(void* value, struct DeviceNode*  deviceNode)
{
	char *url = NULL;
	char *port = NULL;
	int portValue = 0;
	int i = 0;
	int len = strlen((char*)value);
	SetCfgNodeFlag( "Cwmp", -1 );

	if(len > 256){
		tc_printf("==>setMiddlewareURLValue(): The Length should not be more than 256.\n");
		return -1;
	}
	url = (char *)value;

	while(len > 0){
		port = &url[len];
		if(url[len-1] == ':'){
			url[len-1] = '\0';
			break;
		}
		len--;
	}

	if(!port || !url){
		tc_printf("==>setMiddlewareURLValue():error!\n");
		return -1;
	}
	
	for(i=0; i<strlen(port); i++){
		if(isdigit(port[i]))
			continue;
		tc_printf("error:Middleware port should be digit\n");
		return -1;
	}
	portValue = strtol(port, NULL, 10);
	if(portValue < -1 || portValue > 65535){
		tc_printf("\r\n==>Middleware port is not valid!");
		return -1;
	}
	setCwmpAttrValue(MWMgtUrl,url);
	setCwmpAttrValue(MWMgtPort,port);
	
	return 0;
}
int8 getMiddlewareURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{

	uint8  attribute = 0;
	int8  result = 0;

	attribute = tcDeviceInfoAttr.aMiddlewareURL;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setMiddlewareURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;

	attribute = &tcDeviceInfoAttr.aMiddlewareURL;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}
#endif

#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT) || defined(TCSUPPORT_TRUE_LANDING_PAGE)
int8	getMaintenancePortalEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char MaintenancePortalEnable[10]= {0};

	tcapi_get("Portal_Entry","Enable",MaintenancePortalEnable);
	
	return getTheValue(value, type, deviceNode, MaintenancePortalEnable);
}

int8	setMaintenancePortalEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 flag;
	
	flag = *((uint8*)value);
	
	if(flag == 1){
		tcapi_set("Portal_Entry","Enable","1");
	} 
	else if (flag == 0){
		tcapi_set("Portal_Entry","Enable","0");
	}
	else{
		tc_printf("==>setPortalEnableValue:value should be 0 or 1.\n");
		return -1;
	}	
	SetCfgNodeFlag ("Portal", -1);
	return 0;
}

int8	getMaintenancePortalEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aMaintenancePortalEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setMaintenancePortalEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aMaintenancePortalEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}
#endif
#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT) 
int8	getMaintenancePortalComputerValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char MaintenancePortalComputer[256]= {0};

	tcapi_get("Portal_Entry","PortalUrl-Computer",MaintenancePortalComputer);
	
	return getTheValue(value, type, deviceNode, MaintenancePortalComputer);
}

int8	setMaintenancePortalComputerValue(void* value, struct DeviceNode*  deviceNode)
{
	int len = 0;
	
	len = strlen((char*)value);
	tc_printf("\r\nvalue is %s",(char*)value);
	if(len > 255)
	{
		tc_printf("\r\n==>setMaintenancePortalComputerValue:url is too long,the max len is 255!");
		return -1;
	}
	
	if(0 != len)
	{
		tcapi_set("Portal_Entry","PortalUrl-Computer", (char*)value);
	}
	SetCfgNodeFlag ("Portal", -1);
	return 0;
}

int8	getMaintenancePortalComputerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aMaintenancePortalComputer;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setMaintenancePortalComputerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aMaintenancePortalComputer;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8	getMaintenancePortalSTBValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char MaintenancePortalSTB[256]= {0};

	tcapi_get("Portal_Entry","PortalUrl-STB",MaintenancePortalSTB);
	
	return getTheValue(value, type, deviceNode, MaintenancePortalSTB);
}

int8	setMaintenancePortalSTBValue(void* value, struct DeviceNode*  deviceNode)
{
	int len = 0;
	
	len = strlen((char*)value);
	tc_printf("\r\nvalue is %s",(char*)value);
	if(len > 255)
	{
		tc_printf("\r\n==>setMaintenancePortalSTBValue:url is too long,the max len is 255!");
		return -1;
	}
	
	if(0 != len)
	{
		tcapi_set("Portal_Entry","PortalUrl-STB", (char*)value);
	}
	SetCfgNodeFlag ("Portal", -1);
	return 0;
}

int8	getMaintenancePortalSTBAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aMaintenancePortalSTB;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setMaintenancePortalSTBAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aMaintenancePortalSTB;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8	getMaintenancePortalPhoneValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char MaintenancePortalPhone[256]= {0};

	tcapi_get("Portal_Entry","PortalUrl-Phone",MaintenancePortalPhone);
	
	return getTheValue(value, type, deviceNode, MaintenancePortalPhone);
}

int8	setMaintenancePortalPhoneValue(void* value, struct DeviceNode*  deviceNode)
{
	int len = 0;
	
	len = strlen((char*)value);
	tc_printf("\r\nvalue is %s",(char*)value);
	if(len > 255)
	{
		tc_printf("\r\n==>setMaintenancePortalPhoneValue:url is too long,the max len is 255!");
		return -1;
	}
	
	if(0 != len)
	{
		tcapi_set("Portal_Entry","PortalUrl-Phone", (char*)value);
	}
	SetCfgNodeFlag ("Portal", -1);
	return 0;
}

int8	getMaintenancePortalPhoneAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aMaintenancePortalPhone;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setMaintenancePortalPhoneAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aMaintenancePortalPhone;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}
#endif

#if defined(TCSUPPORT_CT_ALARMMONITOR)
int8 checkFreeAlarmEntry(void)
{
	int i = 0;
	int entries = 0;
	char nodeName[32];
	char tmpBuf[MAX_ATTRIBUTE_LEN];
	int ret = 0;

	tcapi_get(ALARM_COMMON, "MaxInstanceNum", tmpBuf);
	if(strlen(tmpBuf) != 0)
		entries =  atoi(tmpBuf);

	for(i=0; i<entries; i++){
		memset(nodeName, 0, sizeof(nodeName));
		memset(tmpBuf, 0, sizeof(tmpBuf));
		sprintf(nodeName, ALARM_ENTRY, i);

		ret = tcapi_get(nodeName, "ParaList", tmpBuf);
		if( ret < 0 )//it means this entry is not exist.
			return i;
	}
	
	return -1;
}

int8 InitAlarmEntryByIndex(int num)
{
	char nodeName[32] = {0};

	sprintf(nodeName, ALARM_ENTRY, num);

	/*Init device alarm entry*/
	tcapi_set(nodeName, "ParaList", "");
	tcapi_set(nodeName, "Limit-Max", "0");
	tcapi_set(nodeName, "Limit-Min", "0");
	tcapi_set(nodeName, "TimeList", "0");
	tcapi_set(nodeName, "Mode", "1");
		
	//tcapi_commit(nodeName);
	
	return 0;
}

int8 deleteAlarmEntryByIndex(uint32 num)
{
	char nodeName[32];
	
	memset(nodeName, 0, sizeof(nodeName));
	sprintf(nodeName, ALARM_ENTRY, num);

	tcapi_unset(nodeName);
	
	tcapi_commit(ALARM_NODENAME);
	return 0;
}

int cwmpInitAlarmConfigSubTree( DeviceNodePtr deviceNode )
{
	int i = 0;
	int entries = 0;
	char index[4] = {0};
	int rtnCode = 0;
	char nodeName[32] = {0};
	char tmpBuf[MAX_ATTRIBUTE_LEN] = {0};
	int ret = 0;
	
	if(deviceNode == NULL){
		return -1;
	}

	tcapi_get(ALARM_COMMON, "MaxInstanceNum", tmpBuf);
	if(strlen(tmpBuf) != 0)
		entries =  atoi(tmpBuf);
	
	for(i = 0; i < entries; i++)
	{
		memset(nodeName, 0, sizeof(nodeName));
		memset(tmpBuf, 0, sizeof(tmpBuf));
		memset(index, 0, sizeof(index));

		sprintf( nodeName, ALARM_ENTRY, i );

		/*check if this entry is exist or not*/
		ret = tcapi_get( nodeName, "ParaList", tmpBuf );
		if ( ret < 0 ) { //it means this entry is not exist.
			continue;
		}
		
		sprintf( index, "%d", i+1 );

		AlarmConfigIndex[0].name = index;
		
		if ( RegisterNodeFromTable( deviceNode, AlarmConfigIndex) == NULL ) {
			rtnCode = -1;		//registerNode fail.
			return rtnCode;
		}
	}
	
	return rtnCode;
}

int8 addAlarmConfigObject(uint32* num, DeviceNode*  deviceNode)
{
	int number = -1;
	DeviceNodePtr ruleNode = NULL;
	char *objectName = "InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.AlarmConfig.";
	int rtnCode = 0;
	
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &ruleNode);
	if(ruleNode == NULL){
		return -1;
	}
	
	if ((number = checkFreeAlarmEntry()) < 0) {
		tc_printf("\n==>Can't find free Alarm entry for use!\n");
		return -1;
	}
	else{
		tc_printf("\n==> Find free Alarm entry %d to add!\n", number+1);
		InitAlarmEntryByIndex(number); //enable this entry
		cwmpFreeDeviceTable(ruleNode);
		rtnCode = cwmpInitAlarmConfigSubTree(ruleNode);

		*num = number + 1;
		if(dbg_cwmp >= DBG_CWMP_L2 ){
			tc_printf("\r\ncwmpParameter.c---[addDeviceAlarmObject]---*num = ", *num);
		}
	}	
	
	return rtnCode;
}


int8 deleteAlarmConfigObject(uint32 num, DeviceNode*  deviceNode)
{
	DeviceNodePtr ruleNode = NULL;
	char *objectName = "InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.AlarmConfig.";
	int rtnCode = 0;
	
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &ruleNode);
	if(ruleNode == NULL){
		return -1;
	}
	tc_printf("\n==>Delete Alarm Config object %d--\n", num);
	deleteAlarmEntryByIndex(num - 1);//InstanceNumber - 1, disable this rule and reset its members

	cwmpFreeDeviceTable(ruleNode);
	rtnCode = cwmpInitAlarmConfigSubTree(ruleNode);

	return rtnCode;
}

int8 getDeviceAlarmEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	tcapi_get(ALARM_COMMON, "Enable", tmpBuf);
	if(strcmp(tmpBuf, "Yes") == 0){
		strcpy(tempValue, "1"); 
	}
	else{
		strcpy(tempValue, "0"); 
	}

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setDeviceAlarmEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 *enable = NULL;
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	enable = (uint8*)value;
	if( *enable == 1 )
    {   
		strcpy(tmpBuf, "Yes");
        g_AlarmEnable = 0;
    }
	else if( *enable == 0 )
    {   
		strcpy(tmpBuf, "No");
        g_AlarmEnable = -1;
    }
	else
		return -1;

	tcapi_set(ALARM_COMMON, "Enable", tmpBuf);	
    
	SetCfgNodeFlag( ALARM_NODENAME, -1 );

	return 0;
}

int8 getDeviceAlarmEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcdeviceAlarmAttr.aEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDeviceAlarmEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcdeviceAlarmAttr.aEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= DEVICEALARM_ATTR_FLAG;
	return result;
}

int8 getDeviceAlarmNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	tcapi_get(ALARM_COMMON, "InstanceNum", tmpBuf);
	if(strlen(tmpBuf) ==  0)
		strcpy(tempValue, "0");
	else
		strcpy(tempValue, tmpBuf);

	return getTheValue(value, type, deviceNode, tempValue);
}


int8 getDeviceAlarmNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcdeviceAlarmAttr.aMaxInstanceNum;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDeviceAlarmNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcdeviceAlarmAttr.aMaxInstanceNum;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= DEVICEALARM_ATTR_FLAG;
	return result;
}

int8 getDeviceAlarmConfigParaListValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance = 0;
	char tempValue[MAX_ATTRIBUTE_LEN]={0};
	char nodeName[32] = {0};
	
	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tempValue, 0, sizeof(tempValue));

	sprintf(nodeName, ALARM_ENTRY, instance-1);
	tcapi_get(nodeName, "ParaList", tempValue);

	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setDeviceAlarmConfigParaListValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance = 0;
	char *paraList = NULL;
	char nodeName[32];
	char tmpBuf[MAX_ATTRIBUTE_LEN];	

	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));

	sprintf(nodeName, ALARM_ENTRY, instance-1);

	paraList = (char*)value;
	if( paraList == NULL ) 
		return -1;
	
	if(strlen(paraList) >= MAX_ATTRIBUTE_LEN)
		return -1;

		strcpy(tmpBuf, paraList);
	
	tcapi_set(nodeName, "ParaList", tmpBuf);

	SetCfgNodeFlag( ALARM_NODENAME, -1 );
	
	return 1;
}
int8 getDeviceAlarmConfigParaListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcdeviceAlarmAttr.aParaList[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDeviceAlarmConfigParaListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcdeviceAlarmAttr.aParaList[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= DEVICEALARM_ATTR_FLAG;
	return result;
}

int8 getDeviceAlarmConfigLimitMaxValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance = 0;
	char tempValue[16]={0};
	char nodeName[32] = {0};
	
	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tempValue, 0, sizeof(tempValue));

	sprintf(nodeName, ALARM_ENTRY, instance-1);
	tcapi_get(nodeName, "Limit-Max", tempValue);

	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setDeviceAlarmConfigLimitMaxValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance = 0;
	char *limitMax = NULL;
	char nodeName[32];
	char tmpBuf[16];	

	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));

	sprintf(nodeName, ALARM_ENTRY, instance-1);

	limitMax = (char*)value;
	if( limitMax == NULL ) 
		return -1;
	
	if(strlen(limitMax)==0) {
		tmpBuf[0]= '\0';
	}
	else {
		strcpy(tmpBuf, limitMax);
	}
	tcapi_set(nodeName, "Limit-Max", tmpBuf);

	SetCfgNodeFlag( ALARM_NODENAME, -1 );
	
	return 1;
}
int8 getDeviceAlarmConfigLimitMaxAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcdeviceAlarmAttr.aLimitMax[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDeviceAlarmConfigLimitMaxAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcdeviceAlarmAttr.aLimitMax[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= DEVICEALARM_ATTR_FLAG;
	return result;
}

int8 getDeviceAlarmConfigLimitMinValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance = 0;
	char tempValue[16]={0};
	char nodeName[32] = {0};
	
	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tempValue, 0, sizeof(tempValue));

	sprintf(nodeName, ALARM_ENTRY, instance-1);
	tcapi_get(nodeName, "Limit-Min", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setDeviceAlarmConfigLimitMinValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance = 0;
	char *limitMin = NULL;
	char nodeName[32];
	char tmpBuf[16];	

	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));

	sprintf(nodeName, ALARM_ENTRY, instance-1);

	limitMin = (char*)value;
	if( limitMin == NULL ) 
		return -1;
	
	if(strlen(limitMin)==0) {
		tmpBuf[0]= '\0';
	}
	else {
		strcpy(tmpBuf, limitMin);
	}
	tcapi_set(nodeName, "Limit-Min", tmpBuf);

	SetCfgNodeFlag( ALARM_NODENAME, -1 );
	
	return 1;
}
int8 getDeviceAlarmConfigLimitMinAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcdeviceAlarmAttr.aLimitMin[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDeviceAlarmConfigLimitMinAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcdeviceAlarmAttr.aLimitMin[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= DEVICEALARM_ATTR_FLAG;
	return result;
}

int8 getDeviceAlarmConfigTimeListValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance = 0;
	char tempValue[16]={0};
	char nodeName[32] = {0};
	
	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tempValue, 0, sizeof(tempValue));

	sprintf(nodeName, ALARM_ENTRY, instance-1);
	tcapi_get(nodeName, "TimeList", tempValue);

	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setDeviceAlarmConfigTimeListValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance = 0;
	char *timeList = NULL;
	char nodeName[32];
	char tmpBuf[16];	

	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));

	sprintf(nodeName, ALARM_ENTRY, instance-1);

	timeList = (char*)value;
	if( timeList == NULL ) 
		return -1;
	
	if(strlen(timeList)==0) {
		tmpBuf[0]= '\0';
	}
	else {
		strcpy(tmpBuf, timeList);
	}
	tcapi_set(nodeName, "TimeList", tmpBuf);

	SetCfgNodeFlag( ALARM_NODENAME, -1 );
	
	return 1;
}
int8 getDeviceAlarmConfigTimeListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcdeviceAlarmAttr.aTimeList[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDeviceAlarmConfigTimeListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcdeviceAlarmAttr.aTimeList[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= DEVICEALARM_ATTR_FLAG;
	return result;
}

int8 getDeviceAlarmConfigModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance = 0;
	char tempValue[16]={0};
	char nodeName[32] = {0};
	
	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tempValue, 0, sizeof(tempValue));

	sprintf(nodeName, ALARM_ENTRY, instance-1);
	tcapi_get(nodeName, "Mode", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setDeviceAlarmConfigModeValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance = 0;
	uint32 modeValue = 0;
	char nodeName[32];
	char tmpBuf[16];

	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));

	sprintf(nodeName, ALARM_ENTRY, instance-1);

	modeValue = *(uint32*)value;

	if(modeValue < 1 || modeValue > 3)
		return -1;
	
	sprintf(tmpBuf, "%d", modeValue);
	
	tcapi_set(nodeName, "Mode", tmpBuf);

	SetCfgNodeFlag( ALARM_NODENAME, -1 );
	
	return 1;
}
int8 getDeviceAlarmConfigModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcdeviceAlarmAttr.aMode[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDeviceAlarmConfigModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcdeviceAlarmAttr.aMode[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= DEVICEALARM_ATTR_FLAG;
	return result;
}

int8 checkFreeMonitorEntry(void)
{
	int i = 0;
	int entries = 0;
	char nodeName[32];
	char tmpBuf[MAX_ATTRIBUTE_LEN];
	int ret = 0;

	tcapi_get(MONITOR_COMMON, "MaxInstanceNum", tmpBuf);
	if(strlen(tmpBuf) != 0)
		entries =  atoi(tmpBuf);

	for(i=0; i<entries; i++){
		memset(nodeName, 0, sizeof(nodeName));
		memset(tmpBuf, 0, sizeof(tmpBuf));
		sprintf(nodeName, MONITOR_ENTRY, i);

		ret = tcapi_get(nodeName, "ParaList", tmpBuf);
		if( ret < 0 )
			return i;
	}
	
	return -1;
}

int8 InitMonitorEntryByIndex(int num)
{
	char nodeName[32] = {0};

	sprintf(nodeName, MONITOR_ENTRY, num);

	/*Init device alarm entry*/
	tcapi_set(nodeName, "ParaList", "");
	tcapi_set(nodeName, "TimeList", "0");
		
	//tcapi_commit(nodeName);
	
	return 0;
}

int8 deleteMonitorEntryByIndex(uint32 num)
{
	char nodeName[32];
	
	memset(nodeName, 0, sizeof(nodeName));
	sprintf(nodeName, MONITOR_ENTRY, num);

	tcapi_unset(nodeName);
	
	tcapi_commit(MONITOR_NODENAME);
	return 0;
}

int cwmpInitMonitorConfigSubTree( DeviceNodePtr deviceNode )
{
	int i = 0;
	int entries = 0;
	char index[4] = {0};
	int rtnCode = 0;
	char nodeName[32] = {0};
	char tmpBuf[MAX_ATTRIBUTE_LEN] = {0};
	int ret = 0;
	
	if(deviceNode == NULL){
		return -1;
	}

	tcapi_get(MONITOR_COMMON, "MaxInstanceNum", tmpBuf);
	if(strlen(tmpBuf) != 0)
		entries =  atoi(tmpBuf);
	
	for(i = 0; i < entries; i++)
	{
		memset(nodeName, 0, sizeof(nodeName));
		memset(tmpBuf, 0, sizeof(tmpBuf));
		memset(index, 0, sizeof(index));

		sprintf( nodeName, MONITOR_ENTRY, i );

		/*check if this entry is exist or not*/
		ret = tcapi_get( nodeName, "ParaList", tmpBuf );
		if ( ret < 0 ) { //it means this entry is not exist.
			continue;
		}
		
		sprintf( index, "%d", i+1 );

		MonitorConfigIndex[0].name = index;
		
		if ( RegisterNodeFromTable( deviceNode, MonitorConfigIndex) == NULL ) {
			rtnCode = -1;		//registerNode fail.
			return rtnCode;
		}
	}
	
	return rtnCode;
}

int8 addMonitorConfigObject(uint32* num, DeviceNode*  deviceNode)
{
	int number = -1;
	DeviceNodePtr ruleNode = NULL;
#if defined(TCSUPPORT_CMCC)	
	char *objectName = "InternetGatewayDevice.DeviceInfo.X_CMCC_Monitor.MonitorConfig.";
#else
	char *objectName = "InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.MonitorConfig.";
#endif

	int rtnCode = 0;
	
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &ruleNode);
	if(ruleNode == NULL){
		return -1;
	}
	
	if ((number = checkFreeMonitorEntry()) < 0) {
		tc_printf("\n==>Can't find free Monitor entry for use!\n");
		return -1;
	}
	else{
		tc_printf("\n==> Find free Monitor entry %d to add!\n", number+1);
		InitMonitorEntryByIndex(number); //enable this entry
		cwmpFreeDeviceTable(ruleNode);
		rtnCode = cwmpInitMonitorConfigSubTree(ruleNode);

		*num = number + 1;
		if(dbg_cwmp >= DBG_CWMP_L2 ){
			tc_printf("\r\ncwmpParameter.c---[addDeviceMonitorObject]---*num = ", *num);
		}
	}	
	
	return rtnCode;
}


int8 deleteMonitorConfigObject(uint32 num, DeviceNode*  deviceNode)
{
	DeviceNodePtr ruleNode = NULL;
	char *objectName = "InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.MonitorConfig.";
	int rtnCode = 0;
	
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &ruleNode);
	if(ruleNode == NULL){
		return -1;
	}
	tc_printf("\n==>Delete Monitor Config object %d--\n", num);
	deleteMonitorEntryByIndex(num - 1);//InstanceNumber - 1, disable this rule and reset its members

	cwmpFreeDeviceTable(ruleNode);
	rtnCode = cwmpInitMonitorConfigSubTree(ruleNode);

	return rtnCode;
}

int8 getDeviceMonitorEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	tcapi_get(MONITOR_COMMON, "Enable", tmpBuf);
	if(strcmp(tmpBuf, "Yes") == 0){
		strcpy(tempValue, "1"); 
	}
	else{
		strcpy(tempValue, "0"); 
	}

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setDeviceMonitorEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 *enable = NULL;
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	enable = (uint8*)value;
	if( *enable == 1 )
    {   
		strcpy(tmpBuf, "Yes");
        g_MonitorEnable = 0;
    }
	else if( *enable == 0 )
    {   
		strcpy(tmpBuf, "No");
        g_MonitorEnable = -1;
    }
	else
		return -1;

	tcapi_set(MONITOR_COMMON, "Enable", tmpBuf);	

	SetCfgNodeFlag( MONITOR_NODENAME, -1 );

	return 0;
}

int8 getDeviceMonitorEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcdeviceMonitorAttr.aEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDeviceMonitorEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcdeviceMonitorAttr.aEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= DEVICEMONITOR_ATTR_FLAG;
	return result;
}

int8 getDeviceMonitorNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	tcapi_get(MONITOR_COMMON, "InstanceNum", tmpBuf);
	if(strlen(tmpBuf) ==  0)
		strcpy(tempValue, "0");
	else
		strcpy(tempValue, tmpBuf);
	
	return getTheValue(value, type, deviceNode, tempValue);
}


int8 getDeviceMonitorNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcdeviceMonitorAttr.aMaxInstanceNum;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDeviceMonitorNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcdeviceMonitorAttr.aMaxInstanceNum;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= DEVICEMONITOR_ATTR_FLAG;
	return result;
}

int8 getDeviceMonitorConfigParaListValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance = 0;
	char tempValue[MAX_ATTRIBUTE_LEN]={0};
	char nodeName[32] = {0};
	
	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tempValue, 0, sizeof(tempValue));

	sprintf(nodeName, MONITOR_ENTRY, instance-1);
	tcapi_get(nodeName, "ParaList", tempValue);

	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setDeviceMonitorConfigParaListValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance = 0;
	char *paraList = NULL;
	char nodeName[32];
	char tmpBuf[MAX_ATTRIBUTE_LEN];	

	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));

	sprintf(nodeName, MONITOR_ENTRY, instance-1);

	paraList = (char*)value;
	if( paraList == NULL ) 
		return -1;
	
	if(strlen(paraList) >= MAX_ATTRIBUTE_LEN)
		return -1;

		strcpy(tmpBuf, paraList);
	
	tcapi_set(nodeName, "ParaList", tmpBuf);

	SetCfgNodeFlag( MONITOR_NODENAME, -1 );
	
	return 1;
}
int8 getDeviceMonitorConfigParaListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcdeviceMonitorAttr.aParaList[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDeviceMonitorConfigParaListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcdeviceMonitorAttr.aParaList[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= DEVICEMONITOR_ATTR_FLAG;
	return result;
}

int8 getDeviceMonitorConfigTimeListValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance = 0;
	char tempValue[16]={0};
	char nodeName[32] = {0};
	
	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tempValue, 0, sizeof(tempValue));

	sprintf(nodeName, MONITOR_ENTRY, instance-1);
	tcapi_get(nodeName, "TimeList", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setDeviceMonitorConfigTimeListValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance = 0;
	char *timeList = NULL;
	char nodeName[32];
	char tmpBuf[16];	

	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));

	sprintf(nodeName, MONITOR_ENTRY, instance-1);

	timeList = (char*)value;
	if( timeList == NULL ) 
		return -1;
	
	if(strlen(timeList)==0) {
		tmpBuf[0]= '\0';
	}
	else {
		strcpy(tmpBuf, timeList);
	}
	tcapi_set(nodeName, "TimeList", tmpBuf);

	SetCfgNodeFlag( MONITOR_NODENAME, -1 );
	
	return 1;
}
int8 getDeviceMonitorConfigTimeListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcdeviceMonitorAttr.aTimeList[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDeviceMonitorConfigTimeListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcdeviceMonitorAttr.aTimeList[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= DEVICEMONITOR_ATTR_FLAG;
	return result;
}

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER) || defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
extern int AlarmNumberInfo[11];
int8 getDeviceAlarmNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char alarmNumber[256] = {0};
	char alarmNumber1[10] = {0};
	int i;
	for(i = 0; i < 10; i++) {
		if(AlarmNumberInfo[i] == 0)
			continue;
		sprintf(alarmNumber1, "10%d00%d", AlarmNumberInfo[i]/10, AlarmNumberInfo[i]%10);
		strcat(alarmNumber, alarmNumber1);
		if(i != AlarmNumberInfo[10]-1)
			strcat(alarmNumber, ",");
	}
	
	return getTheValue(value, type, deviceNode, alarmNumber);
}

#else
int8 getDeviceAlarmNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char alarmNumber[8] = "";

	return getTheValue(value, type, deviceNode, alarmNumber);
}
#endif
int8 getDeviceAlarmNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcdeviceAlarmAttr.aAlarmNumber;
	result = GetTheAttributes(attribute, notification, acl, deviceNode);
	return result;
}
int8 setDeviceAlarmNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8  result = 0;

	attribute = &tcdeviceAlarmAttr.aAlarmNumber;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= DEVICEALARM_ATTR_FLAG;
	return result;
}
#endif

#endif

#endif
#ifdef TCSUPPORT_SYSLOG
#if !defined(TCSUPPORT_C1_CUC)
int8 getSysLogEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	tcapi_get("SysLog_Entry", "LogEnable", tmpBuf);
	if(strcmp(tmpBuf, "Yes") == 0){
		strcpy(tempValue, "1"); 
	}
	else{
		strcpy(tempValue, "0"); 
	}

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setSysLogEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 *enable = NULL;
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	enable = (uint8*)value;
	if( *enable == 1 )
		strcpy(tmpBuf, "Yes");
	else if( *enable == 0 )
		strcpy(tmpBuf, "No");
	else
		return -1;

	tcapi_set("SysLog_Entry", "LogEnable", tmpBuf);	

	SetCfgNodeFlag( "SysLog", -1 );

	return 0;
}

int8 getSysLogEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcsysLogAttr.aEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setSysLogEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcsysLogAttr.aEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= SYSLOG_ATTR_FLAG;
	return result;
}

int8 getSysLogLevelValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};

	tcapi_get("SysLog_Entry", "WriteLevel", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setSysLogLevelValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 level = 0;
	char tmpBuf[8] = {0};

	level = *(uint32*)value;
	if(level < 0 || level > 7)
		return -1;
	
	sprintf(tmpBuf, "%d", level);
	tcapi_set("SysLog_Entry", "WriteLevel", tmpBuf);	

	SetCfgNodeFlag( "SysLog", -1 );

	return 0;
}

int8 getSysLogLevelAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcsysLogAttr.aLevel;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setSysLogLevelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcsysLogAttr.aLevel;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= SYSLOG_ATTR_FLAG;
	return result;
}

#endif

int get_syslog_deviceinfo(char *devInfoBuf){
	char buf[256] = {0};
	char tmp[64] = {0};

	memset(buf, 0, sizeof(buf));
	sprintf(buf,"Manufacturer:%s;\r\n",getCwmpManufacture());
	strcat(devInfoBuf, buf);

	memset(buf, 0, sizeof(buf));
	sprintf(buf,"ProductClass:%s;\r\n",getCwmpProductClass());
	strcat(devInfoBuf, buf);

	memset(buf, 0, sizeof(buf));
	sprintf(buf,"SerialNumber:%s;\r\n",getCwmpSerialNum());
	strcat(devInfoBuf, buf);

	memset(tmp, 0, sizeof(tmp));
	memset(buf, 0, sizeof(buf));
	tcapi_get( "Lan_Entry0", "IP", tmp );
	sprintf(buf,"IP:%s;\r\n", tmp);
	strcat(devInfoBuf, buf);

	//if(getFirmwareVersionFlag()) {
	if ( getUserCustomizeFlag() ) {
	#if 0
		/*Customer's Hardware version*/
		memset(buf, 0, sizeof(buf));
		sprintf(buf,"HWVer:%s;\r\n",getCustomerHardwareVer());
		strcat(devInfoBuf, buf);

		/*Customer's Firmware version*/
		memset(tmp, 0, sizeof(tmp));
		memset(buf, 0, sizeof(buf));
		tcapi_get( "Info_devDefInf", "CustomerSoftwareVersion", tmp );
		sprintf(buf,"SWVer:%s;\r\n", tmp);
		strcat(devInfoBuf, buf);
	#endif
		/*Customer's Hardware version*/
		memset(tmp, 0, sizeof(tmp));
		memset(buf, 0, sizeof(buf));
		tcapi_get( "DeviceInfo_devParaStatic", "CustomerHWVersion", tmp);
		sprintf(buf,"HWVer:%s;\r\n", tmp);
		strcat(devInfoBuf, buf);

		/*Customer's Firmware version*/
		memset(tmp, 0, sizeof(tmp));
		memset(buf, 0, sizeof(buf));
		tcapi_get( "DeviceInfo_devParaStatic", "CustomerSWVersion", tmp );
		sprintf(buf,"SWVer:%s;\r\n", tmp);
		strcat(devInfoBuf, buf);
	}
	else {
		/*Trendchip's Hardware version*/
		memset(buf, 0, sizeof(buf));
		sprintf(buf,"HWVer:%s;\r\n",getHardwareVer());
		strcat(devInfoBuf, buf);

		/*Trendchip's Firmware version*/
		memset(buf, 0, sizeof(buf));
		sprintf(buf,"SWVer:%s;\r\n", (char*)RetRASStrPtr2());
		strcat(devInfoBuf, buf);
	
	}

	strcat(devInfoBuf, "\r\n");

	return 0;
}

/* ***********************************************************
     1. get device info and syslog to generate syslog file
     2. The format of syslog file must follow the Telecom requirement:
         Manufacture:XXXX;
         ProductClass:XXXX;
         SerialNumber:XXXX;
         IP:X.X.X.X;
         HWVer:001.001.001;
         SWVer:001.001.001;

         YYYY-MM-DD HH:MM:SS [Severities] Message
  *************************************************************/
int get_syslog_file(char *fileName)
{
	char deviceInfoBuf[512];
	FILE *fp = NULL;
	FILE *log_fp = NULL;
	int len = 0;
	char buf[1024+1] = {0};

#if !defined(TCSUPPORT_CT_PON)
	/* get device info */
	memset(deviceInfoBuf, 0, sizeof(deviceInfoBuf));
	get_syslog_deviceinfo(deviceInfoBuf);
#endif

	fp = fopen(fileName, "w");
	if(fp == NULL)
		return -1;

#if !defined(TCSUPPORT_CT_PON)
	/* write device info first and then do fseek to change file pointer */
	fwrite(deviceInfoBuf, strlen(deviceInfoBuf), 1, fp);
	fseek(fp, strlen(deviceInfoBuf), SEEK_SET);
#endif

	/* read log msg from "/var/log/currLogFile" and append to syslog file */
	log_fp = fopen(SYSLOG_FILE_PATH, "r");
	if(log_fp == NULL) {
		fclose(fp);
		unlink(fileName);
		return -1;
	}
	while(fgets(buf, 1024 + 1, log_fp)){
		fputs(buf, fp);
	}	

	fclose(fp);
	fclose(log_fp);

	return 0;
}
#endif
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
int8 pingCommitFunc(cfgNode_ptr pCfgNode)
{	
	char nodeName[32] = {0};
	char tmpBuf[32] = {0};
	int i,ret = -1;

	sprintf(nodeName,IPPINGDIAGNOSTIC_COMMON);
	ret = tcapi_get(nodeName, "Active",tmpBuf);
	if ( ret < 0 ) 
	{
		tc_printf("\r\npingCommitFunc error!");
		return -1;	
	}
	//if Active is "No",call commit function once
	if(strcmp(tmpBuf,"No"))
	{
		tcapi_commit(nodeName);
	}
	else if(strcmp(tmpBuf,"Yes"))
	{
		//if Active is "Yes",for circle
		for(i = 0; i < MAX_IPPINGDIAGNOSTIC_NUM; i++)
		{
			sprintf(nodeName,IPPINGDIAGNOSTIC_ENTRY,i);
			ret = tcapi_get(nodeName, "DiagnosticsState",tmpBuf);
			if ( ret < 0 ) { //it means this entry is not exist.
				continue;
			}
			tcapi_commit(nodeName);
		}
	}
	else
	{
		tc_printf("\r\npingCommitFunc error,switch value is not support!");
		return -1;	
	}
	
	return 0;
}
#if !defined(TCSUPPORT_C1_CUC)
int8	getCTCOMEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmpBuf[32] = {0};
	tcapi_get(IPPINGDIAGNOSTIC_COMMON, "Active",tmpBuf);
	if(strcmp(tmpBuf, "Yes") == 0)
	{
		strcpy(tmpBuf, "1");
	}
	else
	{
		strcpy(tmpBuf, "0");
	}
	return getTheValue(value, type, deviceNode, tmpBuf);
}

int8	setCTCOMEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 flag;
	
	flag = *((uint8*)value);

	SetCfgNodeFlag (IPPINGDIAGNOSTIC_COMMON, -1);
	
	if(flag == 1){
		tcapi_set(IPPINGDIAGNOSTIC_COMMON,"Active","Yes");
	} 
	else if (flag == 0){
		tcapi_set(IPPINGDIAGNOSTIC_COMMON,"Active","No");
	}
	else{
		tc_printf("==>setCTCOMEnableValue:value should be 0 or 1.\n");
		return -1;
	}	
	
	return 0;
}

int8	getCTCOMEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aCTCOMEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setCTCOMEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aCTCOMEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}
	
int8	getCTCOMPingNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tmpBuf[32] = {0};
	sprintf(tmpBuf,"%d",getpingNumberEntries());
	return getTheValue(value, type, deviceNode, tmpBuf);
}

int8	getCTCOMPingNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aCTCOMPingNumberOfEntries;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8	setCTCOMPingNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aCTCOMPingNumberOfEntries;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}
#endif
#endif

#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_RECONNECT_NODE)
int8 getEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	/*merge from linos*/
	char ReConnectEnable[10] = {'\0'};
	
	tcapi_get("Wan_Common","ReConnect",ReConnectEnable);
	
	return getTheValue(value, type, deviceNode, ReConnectEnable);
}
int8 setEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 ReConnectEnable;
	
	ReConnectEnable = *((uint8*)value);
	if(ReConnectEnable == 0){
		tcapi_set("Wan_Common","ReConnect","0");
	}
	else if(ReConnectEnable == 1){
		tcapi_set("Wan_Common","ReConnect","1");
	}
	else{
		return -1;
	}
	
	SetCfgNodeFlag ("Wan", -1);	
	return 1;
}
int8 getEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;

	attribute = tcDeviceInfoAttr.aReConnect;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode)
{
	uint8*  attribute;
	int8  result;

	attribute = &tcDeviceInfoAttr.aReConnect;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);

	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

#endif

#if defined(TCSUPPORT_CT_ALGABILITY)
/*For ALG*/
int8 getH323ALGEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	tcapi_get("ALGSwitch_Entry", "H323SW", tmpBuf);
	if(strcmp(tmpBuf, "on") == 0){
		strcpy(tempValue, "1"); 
	}
	else{
		strcpy(tempValue, "0"); 
	}

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setH323ALGEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 *enable = NULL;
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	enable = (uint8*)value;
	if( *enable == 1 )
		strcpy(tmpBuf, "on");
	else if( *enable == 0 )
		strcpy(tmpBuf, "off");
	else
		return -1;

	tcapi_set("ALGSwitch_Entry", "H323SW", tmpBuf);	

	SetCfgNodeFlag( "ALGSwitch", -1 );

	return 0;
}

int8 getH323ALGEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcalgSwitchAttr.aH323Enable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setH323ALGEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcalgSwitchAttr.aH323Enable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= ALGSWITCH_ATTR_FLAG;
	return result;
}


int8 getSIPALGEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	tcapi_get("ALGSwitch_Entry", "SIPSW", tmpBuf);
	if(strcmp(tmpBuf, "on") == 0){
		strcpy(tempValue, "1"); 
	}
	else{
		strcpy(tempValue, "0"); 
	}

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setSIPALGEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 *enable = NULL;
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	enable = (uint8*)value;
	if( *enable == 1 )
		strcpy(tmpBuf, "on");
	else if( *enable == 0 )
		strcpy(tmpBuf, "off");
	else
		return -1;

	tcapi_set("ALGSwitch_Entry", "SIPSW", tmpBuf);	

	SetCfgNodeFlag( "ALGSwitch", -1 );

	return 0;
}

int8 getSIPALGEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcalgSwitchAttr.aSIPEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setSIPALGEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcalgSwitchAttr.aSIPEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= ALGSWITCH_ATTR_FLAG;
	return result;
}

int8 getRTSPALGEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	tcapi_get("ALGSwitch_Entry", "RTSPSW", tmpBuf);
	if(strcmp(tmpBuf, "on") == 0){
		strcpy(tempValue, "1"); 
	}
	else{
		strcpy(tempValue, "0"); 
	}

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setRTSPALGEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 *enable = NULL;
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	enable = (uint8*)value;
	if( *enable == 1 )
		strcpy(tmpBuf, "on");
	else if( *enable == 0 )
		strcpy(tmpBuf, "off");
	else
		return -1;

	tcapi_set("ALGSwitch_Entry", "RTSPSW", tmpBuf);	

	SetCfgNodeFlag( "ALGSwitch", -1 );

	return 0;
}

int8 getRTSPALGEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcalgSwitchAttr.aRTSPEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setRTSPALGEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcalgSwitchAttr.aRTSPEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= ALGSWITCH_ATTR_FLAG;
	return result;
}

int8 getL2TPALGEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	tcapi_get("ALGSwitch_Entry", "L2TPSW", tmpBuf);
	if(strcmp(tmpBuf, "on") == 0){
		strcpy(tempValue, "1"); 
	}
	else{
		strcpy(tempValue, "0"); 
	}

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setL2TPALGEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 *enable = NULL;
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	enable = (uint8*)value;
	if( *enable == 1 )
		strcpy(tmpBuf, "on");
	else if( *enable == 0 )
		strcpy(tmpBuf, "off");
	else
		return -1;

	tcapi_set("ALGSwitch_Entry", "L2TPSW", tmpBuf);	

	SetCfgNodeFlag( "ALGSwitch", -1 );

	return 0;
}

int8 getL2TPALGEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcalgSwitchAttr.aL2TPEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setL2TPALGEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcalgSwitchAttr.aL2TPEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= ALGSWITCH_ATTR_FLAG;
	return result;
}

int8 getIPSECALGEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	tcapi_get("ALGSwitch_Entry", "IPSECSW", tmpBuf);
	if(strcmp(tmpBuf, "on") == 0){
		strcpy(tempValue, "1"); 
	}
	else{
		strcpy(tempValue, "0"); 
	}

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setIPSECALGEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 *enable = NULL;
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	enable = (uint8*)value;
	if( *enable == 1 )
		strcpy(tmpBuf, "on");
	else if( *enable == 0 )
		strcpy(tmpBuf, "off");
	else
		return -1;

	tcapi_set("ALGSwitch_Entry", "IPSECSW", tmpBuf);	

	SetCfgNodeFlag( "ALGSwitch", -1 );

	return 0;
}

int8 getIPSECALGEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcalgSwitchAttr.aIPSECEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setIPSECALGEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcalgSwitchAttr.aIPSECEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= ALGSWITCH_ATTR_FLAG;
	return result;
}

int8 getFTPALGEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	tcapi_get("ALGSwitch_Entry", "FTPSW", tmpBuf);
	if(strcmp(tmpBuf, "on") == 0){
		strcpy(tempValue, "1"); 
	}
	else{
		strcpy(tempValue, "0"); 
	}

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setFTPALGEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 *enable = NULL;
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	enable = (uint8*)value;
	if( *enable == 1 )
		strcpy(tmpBuf, "on");
	else if( *enable == 0 )
		strcpy(tmpBuf, "off");
	else
		return -1;

	tcapi_set("ALGSwitch_Entry", "FTPSW", tmpBuf);	

	SetCfgNodeFlag( "ALGSwitch", -1 );

	return 0;
}

int8 getFTPALGEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcalgSwitchAttr.aFTPEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setFTPALGEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcalgSwitchAttr.aFTPEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= ALGSWITCH_ATTR_FLAG;
	return result;
}

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 getPPTPALGEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char pptpSW[8] = {0};

	tcapi_get("ALGSwitch_Entry", "PPTPSW", pptpSW);
	if( 0 == strcmp(pptpSW, "on") )
		strcpy(pptpSW, "1");
	else
		strcpy(pptpSW, "0");

	return getTheValue(value, type, deviceNode, pptpSW);
}
int8 setPPTPALGEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char pptpSW[8] = {0};
	uint8 enable = *((uint8*)value);

	switch ( enable )
	{
		case 0:
			strcpy(pptpSW, "off");
			break;
		case 1:
			strcpy(pptpSW, "on");
			break;
		default:
			return -1;
	}
	tcapi_set("ALGSwitch_Entry", "PPTPSW", pptpSW);	
	SetCfgNodeFlag( "ALGSwitch", -1 );
	return 0;
}
int8 getPPTPALGEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = tcalgSwitchAttr.aPPTPEnable;
	result = GetTheAttributes(attribute, notification, acl, deviceNode);
	return result;
}
int8 setPPTPALGEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8  result = 0;

	attribute = &tcalgSwitchAttr.aPPTPEnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= ALGSWITCH_ATTR_FLAG;
	return result;
}
#endif

#endif

#ifdef TCSUPPORT_IPV6
int8 getIPProtocolVersion_modeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};

	tcapi_get("Sys_Entry", "IPProtocolVersion", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setIPProtocolVersion_modeValue(void* value, struct DeviceNode*  deviceNode)
{
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	char *iPProtocolVersion = NULL;
#else
	int *iPProtocolVersion = NULL;
#endif
	char tempValue[8] = {0};
	
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	iPProtocolVersion = (char*)value;
	sprintf(tempValue,"%s",iPProtocolVersion);
#else
	iPProtocolVersion = (int*)value;
	sprintf(tempValue,"%d",*iPProtocolVersion);
#endif
	tcapi_set("Sys_Entry", "IPProtocolVersion", tempValue);	

	SetCfgNodeFlag( "Sys", -1 );
#if defined(TCSUPPORT_CT)
#if !defined(TCSUPPORT_CT_PON_CY) && !defined(TCSUPPORT_CT_PON_YN)
	needReboot = 1;
#endif
#endif

	return 0;
}

int8 getIPProtocolVersion_modeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcSysAttr.mode;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setIPProtocolVersion_modeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcSysAttr.mode;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= SYS_ATTR_FLAG;
	return result;
}
#endif
#endif

#if defined(TCSUPPORT_CT_USB_BACKUPRESTORE)
int8 getUsbAutoRestoreEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	tcapi_get("usbRestore_Entry", "autorestore", tmpBuf);
	if(strcmp(tmpBuf, "1") == 0){
		strcpy(tempValue, "1"); 
	}
	else{
		strcpy(tempValue, "0"); 
	}

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setUsbAutoRestoreEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 *enable = NULL;
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	enable = (uint8*)value;
	if( *enable == 1 )
		strcpy(tmpBuf, "1");
	else if( *enable == 0 )
		strcpy(tmpBuf, "0");
	else
		return -1;

	tcapi_set("usbRestore_Entry", "autorestore", tmpBuf);	

	SetCfgNodeFlag( "usbRestore", -1 );

	return 0;
}

int8 getUsbAutoRestoreEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcusbRestoreAttr.aAutoRestore;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setUsbAutoRestoreEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcusbRestoreAttr.aAutoRestore;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= USBRESTORE_ATTR_FLAG;
	return result;
}
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
int8 getUPNPEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char enable[4] = "0";
	char active[8] = {0};
	char autoconfig[4] = {0};

	tcapi_get("Upnpd_Entry", "Active", active);
	tcapi_get("Upnpd_Entry", "autoconf", autoconfig);

	if ( 0 == strcmp(active, "Yes") && 0 == strcmp(autoconfig, "1") )
		strcpy(enable, "1");

	return getTheValue(value, type, deviceNode, enable);
}
int8 setUPNPEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char active[8] = {0};
	char autoconfig[4] = {0};
	uint8 enable = *((uint8*)value);

	switch ( enable )
	{
		case 0:
			strcpy(active, "No");
			strcpy(autoconfig, "0");
			break;
		case 1:
			strcpy(active, "Yes");
			strcpy(autoconfig, "1");
			break;
		default:
			return -1;
	}

	tcapi_set("Upnpd_Entry", "Active", active);
	tcapi_set("Upnpd_Entry", "autoconf", autoconfig);
	SetCfgNodeFlag("Upnpd_Entry", -1);

	return 0;
}
int8 getUPNPEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8  result = 0;

	attribute = ctUPNPAttr.aEnable;
	result = GetTheAttributes(attribute, notification, acl, deviceNode);
	return result;
}
int8 setUPNPEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8  result = 0;

	attribute = &ctUPNPAttr.aEnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= CT_UPNP_ATTR_FLAG;
	return result;
}

int8 getInterfaceVersionValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[20] = {0};
#if defined(TCSUPPORT_CT_PON)
	sprintf(tempValue, "2271-2013.A.1");
#else
	sprintf(tempValue, "2271-2014.A.1");
#endif

	return getTheValue(value, type, deviceNode, tempValue);

}

int8 getInterfaceVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
		
	attribute = tcDeviceInfoAttr.aInterfaceVersion;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setInterfaceVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
		
	attribute = &tcDeviceInfoAttr.aInterfaceVersion;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getDeviceTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};

#if defined(TCSUPPORT_CT_STANDARD_E8B)
	sprintf(tempValue, "%d", 1);
	return getTheValue(value, type, deviceNode, tempValue);
#endif
	
#if defined(TCSUPPORT_CT_POPLUARIZE_E8B)
	sprintf(tempValue, "%d", 2);
	return getTheValue(value, type, deviceNode, tempValue);
#endif
	
#if defined(TCSUPPORT_CT_STANDARD_E8C)
	#if defined(TCSUPPORT_WLAN)
	sprintf(tempValue, "%d", 3);
	#else
	sprintf(tempValue, "%d", 4);
	#endif
#else
	sprintf(tempValue, "%d", 0);	
#endif

	return getTheValue(value, type, deviceNode, tempValue);
	
}

int8 getDeviceTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	
	attribute = tcDeviceInfoAttr.aDeviceType;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDeviceTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
	
	attribute = &tcDeviceInfoAttr.aDeviceType;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getUPLinkValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};

#if defined(TCSUPPORT_CPU_RT65168) || defined(TCSUPPORT_CPU_MT7510)
	sprintf(tempValue, "%d", 3);	//VDSL
#else
#if defined(TCSUPPORT_CPU_MT7520)
	tcapi_get("XPON_LinkCfg", "LinkSta", tempValue);
	if(strcmp(tempValue, "1") == 0)
		sprintf(tempValue, "%d", 5);	//GPON
	else
		sprintf(tempValue, "%d", 4);	//EPON
#else
	sprintf(tempValue, "%d", 1);	//ADSL	
#endif
#endif

	return getTheValue(value, type, deviceNode, tempValue);


}

int8 getUPLinkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	
	attribute = tcDeviceInfoAttr.aUPLink;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setUPLinkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
		
	attribute = &tcDeviceInfoAttr.aUPLink;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getCapabilityValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[100] = {0};
	char lanportValue[20] = {0};

	#if defined(TCSUPPORT_VOIP)
#if defined(TCSUPPORT_CT_1FXS)
		sprintf(tempValue, "%d;", 1);
#else
		sprintf(tempValue, "%d;", 2);
#endif
	#else
		sprintf(tempValue, "%d;", 0);
	#endif
	/*
		Config Lanports information in devParaStatic.conf for customer special demand.
		Add by Shuang.Li.
	*/
	if(0 == tcapi_get("DeviceInfo_devParaStatic", "Lanports", lanportValue))
	{
		strcat(tempValue,lanportValue);
		strcat(tempValue,";");
	}
	else
	{
	#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_PON_CN_100M)
		strcat(tempValue, "2FE;");
	#else
#if defined(TCSUPPORT_CT_2PORTS)
#if defined(TCSUPPORT_CT_1PORT)
		strcat(tempValue, "1GE;");
#else
#if defined(TCSUPPORT_CT_PON_CJ) || defined(TCSUPPORT_CT_PON_CY) || defined(TCSUPPORT_CT_PON_C7)
#if !defined(TCSUPPORT_CT_PON_C9)
		strcat(tempValue, "1FE+1GE;");
#else
		strcat(tempValue, "2FE;");
#endif
#else
		strcat(tempValue, "2FE;");
#endif
#endif
#else
#if defined(TCSUPPORT_CT_PON_CY) || defined(TCSUPPORT_CPU_EN7521)
		strcat(tempValue, "3FE+1GE;");
#else
		strcat(tempValue, "4FE;");
#endif
#endif
	#endif	
	}

	#if defined(TCSUPPORT_USBHOST)
		strcat(tempValue, "1;");
	#else
		strcat(tempValue, "0;");
	#endif
#ifdef TCSUPPORT_WLAN
	strcat(tempValue, "3;");
#else
	strcat(tempValue, "0;");
#endif

#ifdef TCSUPPORT_WLAN_AC
		strcat(tempValue, "5");
#else
		strcat(tempValue, "0");
#endif

	return getTheValue(value, type, deviceNode, tempValue);

}

int8 getCapabilityAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	
	attribute = tcDeviceInfoAttr.aCapability;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setCapabilityAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
		
	attribute = &tcDeviceInfoAttr.aCapability;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getCardValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	char cardVal[2] = "1";
#else
	char cardVal[2] = "0";
#endif
	return getTheValue(value, type, deviceNode, cardVal);
}
int8 getCardAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	
	attribute = tcDeviceInfoAttr.aCard;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCardAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;

	attribute = &tcDeviceInfoAttr.aCard;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getCardInterVersionValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	char cardVerVal[20] = {0};
	tcapi_get(SIM_CARD_NODE_NAME, "Ver", cardVerVal);
#else
	char cardVerVal[8] = "1.0";
#endif

	return getTheValue(value, type, deviceNode, cardVerVal);
}
int8 getCardInterVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcDeviceInfoAttr.aCardInterVersion;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setCardInterVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcDeviceInfoAttr.aCardInterVersion;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getDMSEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_enable[8] = {0};

	if ( tcapi_get("DMS_Entry", "Enable", s_enable) < 0 )
		strcpy(s_enable, "No");

	strcpy(s_enable, (0 == strcmp(s_enable, "Yes")) ? "1" : "0");
	return getTheValue(value, type, deviceNode, s_enable);
}
int8 setDMSEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	char s_enable[8] = {0};
	uint8 i_enable = *((uint8*)value);

	switch ( i_enable )
	{
		case 0:
			strcpy(s_enable, "No");
			break;
		case 1:
			strcpy(s_enable, "Yes");
			break;
		default:
			return -1;
	}

	tcapi_set("DMS_Entry", "Enable", s_enable);
	SetCfgNodeFlag("DMS", -1);
	return 0;
}
int8 getDMSEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcDeviceInfoAttr.aDMSEnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDMSEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcDeviceInfoAttr.aDMSEnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int checkAllRouteMode()
{
	char s_num[16] = {0};
	char attrVal[1025] = {0};
	char allroute[8] = {0};

	tcapi_get("WanInfo_Common", "CycleNum", s_num);
	if ( tcapi_get("WanInfo_Common", "CycleValue", attrVal) < 0 )
		return -1;
	if ( tcapi_get("WanInfo_Common", "AllRoute", allroute) < 0 )
		return -1;

	if ( 0 != strcmp(allroute, "1") )
		return -1;

	return 0;
}
int8 getIPForwardModeEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char s_enable[8] = {0};

	if ( tcapi_get("Wan_Common", "IPForwardModeEnable", s_enable) < 0 )
		strcpy(s_enable, "No");

	strcpy(s_enable, (0 == strcmp(s_enable, "Yes")) ? "1" : "0");
	return getTheValue(value, type, deviceNode, s_enable);
}
int8 setIPForwardModeEnabledValue(void* value, struct DeviceNode*  deviceNode)
{
	char s_enable[8] = {0};
	uint8 i_enable = *((uint8*)value);

	switch ( i_enable )
	{
		case 0:
			strcpy(s_enable, "No");
			break;
		case 1:
			if ( 0 != checkAllRouteMode() )
				return -1;
			strcpy(s_enable, "Yes");
			break;
		default:
			return -1;
	}

	tcapi_set("Wan_Common", "IPForwardModeEnable", s_enable);
	SetCfgNodeFlag("Wan", -1);
	return 0;
}
int8 getIPForwardModeEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcDeviceInfoAttr.aIPForwardModeEnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setIPForwardModeEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcDeviceInfoAttr.aIPForwardModeEnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}
#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
int8 getMacAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[32] = {"00CACA120808"};	
	get_profile_str("LAN_MAC=", tempValue, sizeof(tempValue), 0, "/etc/mac.conf");
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 getMacAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;
	
	attribute = tcDeviceInfoAttr.aMacAddress;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setMacAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;
		
	attribute = &tcDeviceInfoAttr.aMacAddress;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}
#endif

#if defined(TCSUPPORT_CT_FJ)
int8 getVoIPWANStatusValue(char** value, ParameterType* type,  DeviceNode*  deviceNode)
{
	char voipWanStatus[128] = {0};
	if ( tcapi_get("InfoVoIP_Entry0", "Status", voipWanStatus) < 0 )
		voipWanStatus[0] = '\0';
	return getTheValue(value, type, deviceNode, voipWanStatus);
}
int8 getVoIPWANStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcDeviceInfoAttr.aVoIPWANStatus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setVoIPWANStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcDeviceInfoAttr.aVoIPWANStatus;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}
int8 getOltRegTimeValue(char** value, ParameterType* type,  DeviceNode*  deviceNode)
{
	char OltRegTimeValue[32] = {0}; 
	if ( tcapi_get("deviceAccount_Entry", "OltRegTime", OltRegTimeValue) < 0 )
		strcpy(OltRegTimeValue, "0");
	return getTheValue(value, type, deviceNode, OltRegTimeValue);
}
int8 getOltRegTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcDeviceInfoAttr.aOltRegTime;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setOltRegTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;
	
	attribute = &tcDeviceInfoAttr.aOltRegTime;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}
int8 getITMSRegTimeValue(char** value, ParameterType* type,  DeviceNode*  deviceNode)
{
	char ITMSRegTimeValue[32] = {0}; 
	if ( tcapi_get("deviceAccount_Entry", "ITMSRegTime", ITMSRegTimeValue) < 0 )
		strcpy(ITMSRegTimeValue, "0");
	return getTheValue(value, type, deviceNode, ITMSRegTimeValue);
}
int8 getITMSRegTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcDeviceInfoAttr.aITMSRegTime;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setITMSRegTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcDeviceInfoAttr.aITMSRegTime;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}
int8 getITMSServiceTimeValue(char** value, ParameterType* type,  DeviceNode*  deviceNode)
{
	char ITMSServiceTimeValue[32] = {0}; 
	if ( tcapi_get("deviceAccount_Entry", "ITMSServiceTime", ITMSServiceTimeValue) < 0 )
		strcpy(ITMSServiceTimeValue, "0");
	return getTheValue(value, type, deviceNode, ITMSServiceTimeValue); 
}
int8 getITMSServiceTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcDeviceInfoAttr.aITMSServiceTime;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setITMSServiceTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcDeviceInfoAttr.aITMSServiceTime;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}
#endif

#if defined(TCSUPPORT_CT_ECN_GZ) || defined(TCSUPPORT_CT_SIMCARD_SEPARATION) || defined(TCSUPPORT_CT_DEV_ACCESS_TYPE) || defined(TCSUPPORT_CT_PPPINFORM)
int8 getECNDeviceTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char devType[32] = {0};

	strcpy(devType, getECNDeviceType());

	return getTheValue(value, type, deviceNode, devType);
}
int8 getECNDeviceTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcDeviceInfoAttr.aECNDeviceType;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setECNDeviceTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcDeviceInfoAttr.aECNDeviceType;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}
#endif

#if defined(TCSUPPORT_CT_ECN_GZ) || defined(TCSUPPORT_CT_SIMCARD_SEPARATION) || defined(TCSUPPORT_CT_DEV_ACCESS_TYPE)
int8 getDevAccessTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char accessType[32] = {0};

	strcpy(accessType, getECNAccessType());

	return getTheValue(value, type, deviceNode, accessType);
}
int8 getDevAccessTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcDeviceInfoAttr.aAccessType;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setDevAccessTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcDeviceInfoAttr.aAccessType;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}
#endif

#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getGPONSerialNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char SerialNumber[65] = {0};
	char tempValue[16] = {0};

	tcapi_get("XPON_LinkCfg", "LinkSta", tempValue);
	if(strcmp(tempValue, "1") == 0){			//GPON
		if ( tcapi_get("GPON_ONU", "SerialNumber", SerialNumber) < 0 )
			SerialNumber[0] = '\0';
	}
	else{		//EPON
		get_profile_str("LAN_MAC=", SerialNumber, sizeof(SerialNumber), 0, "/etc/mac.conf");		
	}
	
	return getTheValue(value, type, deviceNode, SerialNumber);
}
int8 getGPONSerialNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
    uint8 attribute = 0;
    int8 result = 0;

    attribute = tcDeviceInfoAttr.aGPONSerialNumber;
    result = GetTheAttributes(attribute,notification, acl, deviceNode);
    return result;
}
int8 setGPONSerialNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
    uint8* attribute = NULL;
    int8 result = 0;

    attribute = &tcDeviceInfoAttr.aGPONSerialNumber;
    result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
    return result;
}

int8 getBatteryStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char BatteryStatus[24] = "NA";

	return getTheValue(value, type, deviceNode, BatteryStatus);
}
int8 getBatteryStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
    uint8 attribute = 0;
    int8 result = 0;

    attribute = tcDeviceInfoAttr.aBatteryStatus;
    result = GetTheAttributes(attribute,notification, acl, deviceNode);
    return result;
}
int8 setBatteryStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
    uint8* attribute = NULL;
    int8 result = 0;

    attribute = &tcDeviceInfoAttr.aBatteryStatus;
    result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
    return result;
}

int8 getProcessorLoadValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char CpuUsagevalue[80] = {0};
	if ( tcapi_get("Info_SysDevice", "CpuUsage", CpuUsagevalue) < 0 )
			CpuUsagevalue[0] = '\0';

	return getTheValue(value, type, deviceNode, CpuUsagevalue);
}

int8 getProcessorLoadAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
   	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcDeviceInfoAttr.aProcessorLoad;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setProcessorLoadAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
   	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcDeviceInfoAttr.aProcessorLoad;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getMemoryLoadValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char MemUsagevalue[80] = {0};
	if ( tcapi_get("Info_SysDevice", "MemoryUsage", MemUsagevalue) < 0 )
			MemUsagevalue[0] = '\0';
	
	return getTheValue(value, type, deviceNode, MemUsagevalue);
}

int8 getMemoryLoadAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcDeviceInfoAttr.aMemoryLoad;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setMemoryLoadAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcDeviceInfoAttr.aMemoryLoad;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}
#endif

#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
int8 getSIMCardKeyValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char cardKey[130] = {0};
	
	tcapi_get(SIM_CARD_NODE_NAME, "CardKey", cardKey);

	return getTheValue(value, type, deviceNode, cardKey);
}

int8 setSIMCardKeyValue(void* value, struct DeviceNode*  deviceNode)
{
	char *cardKey = (char*)value;
	char cmdbuf[256] = {0};
	char simAuthRet[8] = {0};

	SetCfgNodeFlag("SimCard", -1);	

	if(isCardExist() == 0){
		tcdbg_printf("isCardExist == 0 !!!\n");
		return -12;
	}

	if(strlen(cardKey) >= 128){
		tcdbg_printf("The length of cardKey is more than 128, error!!!\n");
		return -11;
	}

	tcapi_set(SIM_CARD_NODE_NAME, "CardKey", cardKey);
	system("/userfs/bin/simCard auth");
	tcapi_get(SIM_CARD_NODE_NAME, "simAuthRet", simAuthRet);
	if(strcmp(simAuthRet,"1") == 0){	//check pass
		setSimCardAuthFlag(1);
	}else{		//check fail
		tcdbg_printf("Authentication check fail !!!\n");
		return -11;
	}
	
	
	return 0;
}

int8 getSIMCardKeyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
    return 0;
}
int8 setSIMCardKeyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
    return 0;
}

int8 getSIMCardNoValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char cardSn[64] = {0};
	
	tcapi_get(SIM_CARD_NODE_NAME, "CardSN", cardSn);

	return getTheValue(value, type, deviceNode, cardSn);
}



int8 getSIMCardNoAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
    return 0;
}
int8 setSIMCardNoAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
    return 0;
}

int8 getSIMCardStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char cardStatus[18] = {0};
	
	tcapi_get(SIM_CARD_NODE_NAME, "Status", cardStatus);

	return getTheValue(value, type, deviceNode, cardStatus);
}



int8 getSIMCardStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
    return 0;
}
int8 setSIMCardStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
    return 0;
}

int8 getSIMCardExistStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char cardExist[18] = {0};
	
	tcapi_get(SIM_CARD_NODE_NAME, "cardExist", cardExist);

	return getTheValue(value, type, deviceNode, cardExist);
}



int8 getSIMCardExistStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
    return 0;
}

int8 setSIMCardExistStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
    return 0;
}

#endif

#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)

void monitorCollectorInstanceCalculate(int type){	
	char strInstanceNum[8] = {0};
	int instanceNum = 0;

	
	memset(strInstanceNum, 0, sizeof(strInstanceNum));
	tcapi_get(MONITOR_COLLECTOR_COMMON, "InstanceNum", strInstanceNum);

	if(type == 1)		//Add
		instanceNum = atoi(strInstanceNum) + 1;
	else				//del
		instanceNum = atoi(strInstanceNum) - 1;
		
	memset(strInstanceNum, 0, sizeof(strInstanceNum));
	sprintf(strInstanceNum,"%d",instanceNum);
	tcapi_set(MONITOR_COLLECTOR_COMMON, "InstanceNum", strInstanceNum);
	return;
}


int8 checkFreeMonitorCollectorEntry(void)
{
	int i = 0;
	char nodeName[32];
	char tmpBuf[MAX_ATTRIBUTE_LEN];
	int ret = 0;

	for(i=0; i<MONITOR_COLLECTOR_MAXNUM; i++){
		memset(nodeName, 0, sizeof(nodeName));
		memset(tmpBuf, 0, sizeof(tmpBuf));
		sprintf(nodeName, MONITOR_COLLECTOR_ENTRY, i);

		ret = tcapi_get(nodeName, "ParaList0", tmpBuf);
		if( ret < 0 )
			return i;
	}
	
	return -1;
}

int8 InitMonitorCollectorEntryByIndex(int num)
{
	char nodeName[32] = {0};

	sprintf(nodeName, MONITOR_COLLECTOR_ENTRY, num);

	/*Init device alarm entry*/
	
	tcapi_set(nodeName, "ParaListNum", "0");
	tcapi_set(nodeName, "ParaListLen", "0");
	tcapi_set(nodeName, "ParaList0", "");
	tcapi_set(nodeName, "TimeList", "0");
		
	//tcapi_commit(nodeName);
	
	return 0;
}

int8 deleteMonitorCollectorEntryByIndex(uint32 num)
{
	char nodeName[32] = {0};
	
	memset(nodeName, 0, sizeof(nodeName));
	sprintf(nodeName, MONITOR_COLLECTOR_ENTRY, num);

	tcapi_unset(nodeName);
	
	tcapi_commit(MONITOR_COLLECTOR_NODENAME);
	return 0;
}

int cwmpInitMonitorCollectorSubTree( DeviceNodePtr deviceNode )
{
	int i = 0;
	char index[4] = {0};
	int rtnCode = 0;
	char nodeName[32] = {0};
	char tmpBuf[MAX_ATTRIBUTE_LEN] = {0};
	int ret = 0;
	int instanceNum = 0;
	
	if(deviceNode == NULL){
		return -1;
	}

	for(i = 0; i < MONITOR_COLLECTOR_MAXNUM; i++)
	{
		memset(nodeName, 0, sizeof(nodeName));
		memset(tmpBuf, 0, sizeof(tmpBuf));
		memset(index, 0, sizeof(index));

		sprintf( nodeName, MONITOR_COLLECTOR_ENTRY, i );

		/*check if this entry is exist or not*/
		ret = tcapi_get( nodeName, "ParaList0", tmpBuf );
		if ( ret < 0 ) { //it means this entry is not exist.
			continue;
		}
		
		sprintf( index, "%d", i+1 );

		MonitorCollectorConfigIndex[0].name = index;
		
		if ( RegisterNodeFromTable( deviceNode, MonitorCollectorConfigIndex) == NULL ) {
			rtnCode = -1;		//registerNode fail.
			return rtnCode;
		}
	}

	return rtnCode;
}

int8 addMonitorCollectorObject(uint32* num, DeviceNode*  deviceNode)
{
	int number = -1;
	DeviceNodePtr ruleNode = NULL;
	char *objectName = "InternetGatewayDevice.DeviceInfo.X_CT-COM_MonitorCollector.MonitorConfig.";
	int rtnCode = 0;
	
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &ruleNode);
	if(ruleNode == NULL){
		return -1;
	}
	
	if ((number = checkFreeMonitorCollectorEntry()) < 0) {
		tcdbg_printf("\n==>Can't find free Monitor entry for use!\n");
		return -1;
	}
	else{
		tcdbg_printf("\n==> Find free Monitor entry %d to add!\n", number+1);
		InitMonitorCollectorEntryByIndex(number); //enable this entry
		cwmpFreeDeviceTable(ruleNode);		
		monitorCollectorInstanceCalculate(1);
		rtnCode = cwmpInitMonitorCollectorSubTree(ruleNode);

		*num = number + 1;
		if(dbg_cwmp >= DBG_CWMP_L2 ){
			tcdbg_printf("\r\ncwmpParameter.c---[addMonitorCollectorObject]---*num = ", *num);
		}
	}	

	return rtnCode;
}


int8 deleteMonitorCollectorObject(uint32 num, DeviceNode*  deviceNode)
{
	DeviceNodePtr ruleNode = NULL;
	char *objectName = "InternetGatewayDevice.DeviceInfo.X_CT-COM_MonitorCollector.MonitorConfig.";
	int rtnCode = 0;
	
	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &ruleNode);
	if(ruleNode == NULL){
		return -1;
	}
	tcdbg_printf("\n==>Delete Monitor Collector Config object %d--\n", num);
	monitorCollectorInstanceCalculate(0);
	deleteMonitorCollectorEntryByIndex(num - 1);//InstanceNumber - 1, disable this rule and reset its members

	cwmpFreeDeviceTable(ruleNode);
	rtnCode = cwmpInitMonitorCollectorSubTree(ruleNode);

	return rtnCode;
}

int8 getMonitorCollectorEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	tcapi_get(MONITOR_COLLECTOR_COMMON, "Enable", tmpBuf);
	if(strcmp(tmpBuf, "1") == 0){
		strcpy(tempValue, "1"); 
	}
	else{
		strcpy(tempValue, "0"); 
	}

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setMonitorCollectorEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 *enable = NULL;
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	enable = (uint8*)value;
	if( *enable == 1 )
    {   
		strcpy(tmpBuf, "1");
        g_MonitorCollectorEnable = 0;
    }
	else if( *enable == 0 )
    {   
		strcpy(tmpBuf, "0");
        g_MonitorCollectorEnable = -1;
    }
    else
        return -1;

    tcapi_set(MONITOR_COLLECTOR_COMMON, "Enable", tmpBuf);  
    SetCfgNodeFlag( MONITOR_COLLECTOR_NODENAME, -1 );
    
#if defined(TCSUPPORT_CT_PPPOE_EMULATOR)
    tcapi_set("PppoeEmulatorBridge_Entry", "Enable",tmpBuf);
    SetCfgNodeFlag ("PppoeEmulatorBridge", -1); 
#endif

    return 0;
}

int8 getMonitorCollectorEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcMonitorCollectorAttr.aEnable;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setMonitorCollectorEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcMonitorCollectorAttr.aEnable;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= MONITOR_COLLECTOR_ATTR_FLAG;
	return result;
}

int8 getMonitorCollectorServerUrlValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[256] = {0};

	tcapi_get(MONITOR_COLLECTOR_COMMON, "reportUrl", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setMonitorCollectorServerUrlValue(void* value, struct DeviceNode*  deviceNode)
{
	tcapi_set(MONITOR_COLLECTOR_COMMON, "reportUrl",(char*)value);	

	SetCfgNodeFlag( MONITOR_COLLECTOR_NODENAME, -1 );

	return 0;
}

int8 getMonitorCollectorServerUrlAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcMonitorCollectorAttr.aServerURL;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setMonitorCollectorServerUrlAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcMonitorCollectorAttr.aServerURL;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= MONITOR_COLLECTOR_ATTR_FLAG;
	return result;
}

int8 getMonitorCollectorTftpPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};

	tcapi_get(MONITOR_COLLECTOR_COMMON, "TftpPort", tempValue);

	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setMonitorCollectorTftpPortValue(void* value, struct DeviceNode*  deviceNode)
{
	uint32 port = 0;
	char tmpBuf[8] = {0};

	port = *(uint32*)value;
	if ( port < 1 || port > 65535 )
		return -1;
	sprintf(tmpBuf, "%d", port);

	tcapi_set(MONITOR_COLLECTOR_COMMON, "TftpPort", (char*)tmpBuf);	

	SetCfgNodeFlag( MONITOR_COLLECTOR_NODENAME, -1 );

	return 0;
}

int8 getMonitorCollectorTftpPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcMonitorCollectorAttr.aTftpPort;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setMonitorCollectorTftpPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcMonitorCollectorAttr.aTftpPort;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= MONITOR_COLLECTOR_ATTR_FLAG;
	return result;
}



int8 getMonitorCollectorNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[8] = {0};
	char tmpBuf[8] = {0};

	tcapi_get(MONITOR_COLLECTOR_COMMON, "InstanceNum", tmpBuf);
	if(strlen(tmpBuf) ==  0)
		strcpy(tempValue, "0");
	else
		strcpy(tempValue, tmpBuf);
	
	return getTheValue(value, type, deviceNode, tempValue);
}


int8 getMonitorCollectorNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcMonitorCollectorAttr.aMaxInstanceNum;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setMonitorCollectorNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcMonitorCollectorAttr.aMaxInstanceNum;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= MONITOR_COLLECTOR_ATTR_FLAG;
	return result;
}

int checkDeleteParaListItem(char* InformItem)
{
	char pParaListInfo[32] = {0};
	char* pBegin = NULL;
	char* pEnd = NULL;
	int len = 0;	
	int flag = 0;
	ParaListWanInfo pWanInfo;
	
	pBegin = InformItem;
	while((pBegin != NULL)){		
		if((pEnd = strstr(pBegin,")(")) != NULL){
			len = pEnd - pBegin + 1;		
			memset(pParaListInfo,0,sizeof(pParaListInfo));
			strncpy(pParaListInfo,pBegin,len);			
		}else{
			strcpy(pParaListInfo,pBegin);
		}
		sscanf(pParaListInfo,"(%d,%d,%d,%d,%d)",
			&(pWanInfo.isDelete),&(pWanInfo.iParaListItem),&(pWanInfo.ip_ppp),&(pWanInfo.pvcIndex),&(pWanInfo.entryIndex));

		if(pWanInfo.isDelete == 1){
			flag |= 1<<pWanInfo.iParaListItem;
		}

		if(pEnd != NULL)
			pBegin = pEnd + 1;
		else
			pBegin = pEnd;
	}
	
	return flag;
}


int8 getMonitorCollectorParaListValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance = 0;
	//char* tempValue[1024]={0};
	char* paraList = NULL;
	char nodeName[32] = {0};
	char attr[32] = {0};
	char attrValue[256] = {0};
	char tmp[300] = {0};
	int num = 0;
	int len = 0;
	int i = 0;
	int ret = -1;
	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	sprintf(nodeName, MONITOR_COLLECTOR_ENTRY, instance-1);

	memset(tmp, 0, sizeof(tmp));
	tcapi_get(nodeName, "ParaListNum", tmp);
	num = atoi(tmp);

	memset(tmp, 0, sizeof(tmp));
	tcapi_get(nodeName, "ParaListLen", tmp);
	len = atoi(tmp);
	if ( len < 0 )
		len = 0;
	
	paraList = (char*)malloc(len+1);
	memset(paraList, 0, len+1);
	if(paraList == NULL){
		tcdbg_printf("[%s:%d]malloc fail!!!\n",__FUNCTION__,__LINE__);
		return -1;
	}

	while(i < num){	
		memset(attr, 0, sizeof(attr));
		memset(attrValue, 0, sizeof(attrValue));
		sprintf(attr,"ParaList%d",i);
		tcapi_get(nodeName, attr, attrValue);
		if(strlen(attrValue) != 0){			
			if(i != 0){
				strcat(paraList,",");
			}
			
			strcat(paraList,attrValue);
		}
		i++;
		
	}	
	ret = getTheValue(value, type, deviceNode, paraList);
	free(paraList);	
	return ret;
}


int checkParaList(char* buf){
	DeviceNodePtr currentDeviceNode = NULL;
	char* curParaValue = NULL;	
	char paraList[256] = {0};
	char* pBegin = NULL;
	char* pEnd = NULL;
	int len = 0;
//	int flag = 0;
	
	
	pBegin = buf;
	while((pBegin != NULL) ){	
		if((pEnd = strstr(pBegin,",")) != NULL){
			len = pEnd - pBegin;		
			memset(paraList,0,sizeof(paraList));
			strncpy(paraList,pBegin,len);
			
		}else{
			strcpy(paraList,pBegin);
			//flag = 1;
		}

		/* Search node and get value */
		currentDeviceNode = NULL;
		cwmpSearchDeviceNodeByName(paraList, rpc.IGDDeviceRootPtr, &currentDeviceNode);
		if(currentDeviceNode == NULL){					
			tcdbg_printf("search node fail: paraList=%s\n",paraList);
			return -1;
		}

		if(pEnd == NULL){
			//the last paraList
			break;
		}
		
		pBegin = pEnd+1;

	}

	return 0;
}


#if defined(TCSUPPORT_CT_PON_GDV20)
void checkInternetWan(){		
	char nodeName[32], active[10];	
	char serviceList[SERVICE_LIST_LEN] = {'\0'};
	char bridgeMode[32] = {'\0'};	
	char isp[8] = {'\0'};
	int wanActionType = WAN_TYPE_NOCARE;
	int pvc_index,entry_index;
	int ret = -1;
	
	for (pvc_index = 0; pvc_index < TOTAL_REMOTE_DEVICE_NODE; pvc_index++) {
		for (entry_index = 0; entry_index < TOTAL_REMOTE_ENTRY_NODE; entry_index++) {
			sprintf(nodeName, "Wan_PVC%d_Entry%d", pvc_index, entry_index);
			ret = tcapi_get(nodeName, "Active", active);
			if (ret < 0) {
				/*wan is not exist*/
				continue;
			}

			tcapi_get(nodeName, "ServiceList", serviceList);
			if( strstr( serviceList, "INTERNET" ) == NULL ){
				/*not internet wan*/
				continue;
			}
			
			tcapi_get(nodeName, "ISP", isp);
			if ( 0 == strcmp(isp, WAN_ENCAP_PPP)){
				wanActionType = PPP_WAN_TYPE;
			}else if( 0 == strcmp(isp, WAN_ENCAP_BRIDGE)){
				tcapi_get(nodeName, "BridgeMode", bridgeMode);
				if( 0 == strcmp(bridgeMode, "PPPoE_Bridged")){
					wanActionType = PPP_WAN_TYPE;
				}else{
					wanActionType = IP_WAN_TYPE;
				}
			}else{
				wanActionType = IP_WAN_TYPE;
			}


			/*internet wan not in ParaList*/
			setMonitorCollectorParaList(wanActionType,pvc_index,entry_index,WAN_DEV_ADD);	
		}
	}
}
#endif

int8 setMonitorCollectorParaListValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance = 0;
	char *paraList = NULL;
	char nodeName[32] = {0};	
	char attr[32] = {0};
	//char tmpBuf[1024];
	char* p = NULL;		
	char* ptr = NULL;	
	DeviceNodePtr currentDeviceNode = NULL;
	char* curParaValue = NULL;
	char tmp[32] = {0};
	int num = 0;
	int len = 0;
	char tempvalue[2];
	int pvcIndex =	0;
	int entryIndex = 0;	
	int ip_ppp = 0; 		//0:none; 1:ip; 2:ppp

	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	//memset(tmpBuf, 0, sizeof(tmpBuf));

	sprintf(nodeName, MONITOR_COLLECTOR_ENTRY, instance-1);

	paraList = (char*)value;
	if( paraList == NULL ) {		
		tcdbg_printf("[%s:%d]paraList == NULL!!!\n",__FUNCTION__,__LINE__);
		return -1;
	}

	if(checkParaList((char*)value) != 0){
		tcdbg_printf("[%s:%d]checkParaList fail!!!\n",__FUNCTION__,__LINE__);
		return -1;
	}
	
	p = strtok(paraList,",");
	while(p!=NULL){ 			
		//tcdbg_printf("p=%s!!!\n",__FUNCTION__,__LINE__,p);
		len += strlen(p);
		
		memset(attr, 0, sizeof(attr));
		sprintf(attr,"ParaList%d",num);
		tcapi_set(nodeName, attr, p);
	
		p = strtok(NULL,",");
		num++;
	}

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp,"%d",num);
	tcapi_set(nodeName, "ParaListNum", tmp);

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp,"%d",len+num-1);
	tcapi_set(nodeName, "ParaListLen", tmp);
	
#if defined(TCSUPPORT_CT_PON_GDV20)
	checkInternetWan();
#endif
	
	//removeOldFilesByIndex(instance);
	SetCfgNodeFlag( MONITOR_COLLECTOR_NODENAME, -1 );
	
	return 1;
}
int8 getMonitorCollectorParaListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcMonitorCollectorAttr.aParaList[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setMonitorCollectorParaListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcMonitorCollectorAttr.aParaList[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= MONITOR_COLLECTOR_ATTR_FLAG;
	return result;
}

int8 getMonitorCollectorTimeListValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int instance = 0;
	char tempValue[16]={0};
	char nodeName[32] = {0};
	
	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tempValue, 0, sizeof(tempValue));

	sprintf(nodeName, MONITOR_COLLECTOR_ENTRY, instance-1);
	tcapi_get(nodeName, "TimeList", tempValue);
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setMonitorCollectorTimeListValue(void* value, struct DeviceNode*  deviceNode)
{
	int instance = 0;
	char *timeList = NULL;
	char nodeName[32];
	char tmpBuf[16];	

	instance = atoi(deviceNode->parentNode->name);	

	memset(nodeName, 0, sizeof(nodeName));
	memset(tmpBuf, 0, sizeof(tmpBuf));

	sprintf(nodeName, MONITOR_COLLECTOR_ENTRY, instance-1);

	timeList = (char*)value;
	if( timeList == NULL ) 
		return -1;
	
	if(strlen(timeList)==0) {
		tmpBuf[0]= '\0';
	}
	else {
		strcpy(tmpBuf, timeList);
	}
	tcapi_set(nodeName, "TimeList", tmpBuf);

	cwmpMonitorCollectorTaskStop();

	SetCfgNodeFlag( MONITOR_COLLECTOR_NODENAME, -1 );
	
	return 1;
}
int8 getMonitorCollectorTimeListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = tcMonitorCollectorAttr.aTimeList[instance - 1];
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setMonitorCollectorTimeListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	int instance;

	instance = atoi(deviceNode->parentNode->name);		
	attribute = &tcMonitorCollectorAttr.aTimeList[instance - 1];
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag1 |= MONITOR_COLLECTOR_ATTR_FLAG;
	return result;
}

#endif

#if defined(TCSUPPORT_CT_STB_TEST)
int8 getDetectionsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[16] = {0};

	tcapi_get("StbTest_Entry", "WorkStatus", tempValue);
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setDetectionsStateValue(void* value, struct DeviceNode*  deviceNode)
{
	tcapi_set("StbTest_Entry", "WorkStatus", (char*)value);	
	return 0;
}

int8 getDetectionsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcDeviceInfoAttr.aDetectionsState;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDetectionsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &tcDeviceInfoAttr.aDetectionsState;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getDetectionsResultValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[32] = {0};
	char retValue[512] = {0};

	tcapi_get("StbTest_Entry", "DstMAC", tempValue);
	strcat(retValue, tempValue);
	strcat(retValue, ":");
	memset(tempValue, 0 ,sizeof(tempValue));
	tcapi_get("StbTest_Entry", "LostRate", tempValue);
	strcat(retValue, tempValue);
	strcat(retValue, ":");
	memset(tempValue, 0 ,sizeof(tempValue));
	tcapi_get("StbTest_Entry", "ResponseDelay", tempValue);
	strcat(retValue, tempValue);
	strcat(retValue, ":");
	memset(tempValue, 0 ,sizeof(tempValue));
	tcapi_get("StbTest_Entry", "ResponseShake", tempValue);
	strcat(retValue, tempValue);
	
	memset(tempValue, 0 ,sizeof(tempValue));
	tcapi_get("StbTest_Entry", "DstMAC2", tempValue);
	if(strlen(tempValue)>0){
		strcat(retValue, "-");
		strcat(retValue, tempValue);
		strcat(retValue, ":");
		memset(tempValue, 0 ,sizeof(tempValue));
		tcapi_get("StbTest_Entry", "LostRate2", tempValue);
		strcat(retValue, tempValue);
		strcat(retValue, ":");
		memset(tempValue, 0 ,sizeof(tempValue));
		tcapi_get("StbTest_Entry", "ResponseDelay2", tempValue);
		strcat(retValue, tempValue);
		strcat(retValue, ":");
		memset(tempValue, 0 ,sizeof(tempValue));
		tcapi_get("StbTest_Entry", "ResponseShake2", tempValue);
		strcat(retValue, tempValue);		
	}
	return getTheValue(value, type, deviceNode, retValue);	
}

int8 setDetectionsResultValue(void* value, struct DeviceNode*  deviceNode)
{
	return 0;
}

int8 getDetectionsResultAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute;
	int8  result;
	
	attribute = tcDeviceInfoAttr.aDetectionsResult;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setDetectionsResultAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result;
	
	attribute = &tcDeviceInfoAttr.aDetectionsResult;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= DEVICEINFO_ATTR_FLAG;
	return result;
}

int8 getDetectionsPeriodValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	int ret = 0;
	
	ret = tcapi_get("StbTest_Entry", "Period", tempValue);
	
	if (ret != 0)
		strcpy(tempValue, "30");
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setDetectionsPeriodValue(void* value, struct DeviceNode*  deviceNode)
{
	char buf[10] = {0};
	uint32* period = NULL;

	period = (uint32*)value;

	if (*period < 5 || *period > 120)
		return -1;
	
	sprintf(buf, "%d", *period);
	
	tcapi_set("StbTest_Entry", "Period", buf);	
	return 0;
}

int8 getDetectionsFreqencyValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char tempValue[10] = {0};
	int ret = 0;
	
	ret = tcapi_get("StbTest_Entry", "Freqency", tempValue);

	if (ret != 0)
		strcpy(tempValue, "50");
	
	return getTheValue(value, type, deviceNode, tempValue);
}

int8 setDetectionsFreqencyValue(void* value, struct DeviceNode*  deviceNode)
{
	char buf[10] = {0};
	uint32 * Freqency = 0;

	Freqency = (uint32*)value;
	if (*Freqency < 20 || *Freqency > 200)
		return -1;
	
	sprintf(buf, "%d", *Freqency);
	
	tcapi_set("StbTest_Entry", "Freqency", buf);	
	return 0;
}
#endif

