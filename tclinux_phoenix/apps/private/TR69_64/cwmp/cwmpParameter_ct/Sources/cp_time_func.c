#include <time.h>
#include "Global_res.h"
#include "cp_time.h"


int8 getTimeNTPServer1Value(char** value, ParameterType* type, DeviceNode*  deviceNode){
	char TimeNTPServer1[32];
	char timeSvrType[8];
	char temp[32];

	memset(TimeNTPServer1, 0, sizeof(TimeNTPServer1));
	memset(timeSvrType, 0, sizeof(timeSvrType));
	memset(temp, 0, sizeof(temp));
	
	tcapi_get("Timezone_Entry", "TYPE", timeSvrType);
	tcapi_get("Timezone_Entry", "SERVER", temp);

	if(!strcmp(timeSvrType, NTP_TIME)){
		if (!strcmp("", temp))
			strcpy(TimeNTPServer1, "0.0.0.0");
		else
			strcpy(TimeNTPServer1, temp);
	}
	else
		sprintf(TimeNTPServer1,"%s","The NTPServer is diable!");	
	
	return getTheValue(value, type, deviceNode, TimeNTPServer1);
}

int8   setTimeNTPServer1Value(void* value, struct DeviceNode*  deviceNode){
	char TimeNTPServer1[32];
	char timeSvrType[8];
	char* TimeServerIp  = NULL;

	if(strlen((char*)value) > 31)
		return -1;
	TimeServerIp = (char*)value;

	memset(TimeNTPServer1, 0, sizeof(TimeNTPServer1));
	memset(timeSvrType, 0, sizeof(timeSvrType));
	
	tcapi_get("Timezone_Entry", "TYPE", timeSvrType);

	if(!strcmp(timeSvrType, NTP_TIME)){
		strcpy(TimeNTPServer1, TimeServerIp);
	}
	else	{
		memset(timeSvrType, 0, sizeof(timeSvrType));
		strcpy(timeSvrType, NTP_TIME);
		strcpy(TimeNTPServer1, TimeServerIp);
	}
	
	tcapi_set("Timezone_Entry", "TYPE", timeSvrType);
	tcapi_set("Timezone_Entry", "SERVER", TimeServerIp);
	
	/*
	 * the following code check whether the current NTP server is available
	 * it will NOT be saved to flash if ther server is not available at present.
	 * BUT, that is not reseanable! 
	 * 1. Maybe the NTP server is available, it can't response CPE's ntp_request temporary such as bad network;
	 * 2. Maybe ACS need to reset this parameter to NULL
	 * 3. etc
	 * 
	 * So, CPE should save the value without checking its validity.
	 * And CPE need only synchronization the time.
	 * On the other hand, the action of synchronization time may be cost much time, ACS maybe timeout making
	 * SetParameterValue RPC failed, thus, CPE send response to ACS at first, CPE synchronization time after 
	 * curent session terminate.
	 *
	 * modify by taoliu@20090325
	 */
/*	if (getDateTime(spTS->service) < 0)
		return -1;
*/

	/*For Timezone cfg-node delay commit*/
	SetCfgNodeFlag( "Timezone_Entry", -1 );
	
	return 0;
}    

int8 getTimeNTPServer1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
	uint8  attribute;
	int8  result;

	attribute = tcTimeAttr.aTimeNTPServer1;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8   setTimeNTPServer1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){
	uint8*  attribute = NULL;
	int8  result;

	attribute = &tcTimeAttr.aTimeNTPServer1;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TIME_ATTR_FLAG;
	return result;
}


int8 getTimeCurrentLocalTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode){
	uint16 CLTyear;
	uint8 CLThour;
	char  CurrentLocalTime[60];
	time_t			ltime;
       struct  tm*		sTime;
	char daylightEnable[8];
	
	time(&ltime);
	sTime = localtime(&ltime);
       CLTyear = sTime->tm_year+1900;
	if (CLTyear < 2000)
		CLTyear = 2000;
	CLThour = sTime->tm_hour;

	/*If Day light saving is enabled, hour will be increased 1.*/
	memset(daylightEnable, 0, sizeof(daylightEnable));
	tcapi_get("Timezone_Entry", "DAYLIGHT", daylightEnable);
	if(!strcmp(daylightEnable, "Enable"))
		CLThour = (CLThour + 1)%24;

	//workssystem prompt that the value should match yyyy-MM-ddTHH:mm:ss.
	sprintf(CurrentLocalTime, "%02d-%02d-%02dT%02d:%02d:%02d", CLTyear,  (sTime->tm_mon + 1), sTime->tm_mday, CLThour, sTime->tm_min,  sTime->tm_sec);	
	return getTheValue(value, type, deviceNode, CurrentLocalTime);
}
 
int8 getTimeCurrentLocalTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
	uint8  attribute;
	int8  result;

	attribute = tcTimeAttr.aTimeCurrentLocalTime;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setTimeCurrentLocalTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){
	uint8*  attribute;
	int8  result;

	attribute = &tcTimeAttr.aTimeCurrentLocalTime;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TIME_ATTR_FLAG;

	return result;
}

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER) || defined(TCSUPPORT_CT_PON_GD)
static char *TimezoneName[] = 
{
	/*(GMT-12:00)*/ "International Date Line West"
	/*(GMT-11:00)*/ , "Midway Island, Samoa"
	/*(GMT-10:00)*/ , "Hawaii"
	/*(GMT-09:00)*/ , "Alaska"
	/*(GMT-08:00)*/ , "Pacific Time, Tijuana"
	/*(GMT-07:00)*/ , "Arizona, Mazatlan"
	/*(GMT-07:00-1)*/ , "Chihuahua, La Paz"
	/*(GMT-07:00-2)*/ , "Mountain Time"
	/*(GMT-06:00)*/ , "Central America"
	/*(GMT-06:00-1)*/ , "Central Time"
	/*(GMT-06:00-2)*/ , "Guadalajara, Mexico City, Monterrey"
	/*(GMT-06:00-3)*/ , "Saskatchewan"
	/*(GMT-05:00)*/ , "Bogota, Lima, Quito"
	/*(GMT-05:00-1)*/ , "Eastern Time"
	/*(GMT-05:00-2)*/ , "Indiana"
	/*(GMT-04:00)*/ , "Atlantic Time"
	/*(GMT-04:00-1)*/ , "Caracas, La Paz"
	/*(GMT-04:00-2)*/ , "Santiago"
	/*(GMT-03:30)*/ , "Newfoundland"
	/*(GMT-03:00)*/ , "Brasilia"
	/*(GMT-03:00-1)*/ , "Buenos Aires, Georgetown"
	/*(GMT-03:00-2)*/ , "Greenland"
	/*(GMT-02:00)*/ , "Mid-Atlantic"
	/*(GMT-01:00)*/ , "Azores"
	/*(GMT-01:00-1)*/ , "Cape Verde Is."
	/*(GMT)*/ , "Casablanca, Monrovia"
	/*(GMT-1)*/ , "Greenwich Mean Time: Dublin, Edinburgh, Lisbon, London"
	/*(GMT+01:00)*/ , "Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna"
	/*(GMT+01:00-1)*/ , "Belgrade, Bratislava, Budapest, Ljubljana, Prague"
	/*(GMT+01:00-2)*/ , "Brussels, Copenhagen, Madrid, Paris"
	/*(GMT+01:00-3)*/ , "Sarajevo, Skopje, Warsaw, Zagreb"
	/*(GMT+01:00-4)*/ , "West Central Africa"
	/*(GMT+02:00)*/ , "Athens, Istanbul, Minsk"
	/*(GMT+02:00-1)*/ , "Bucharest"
	/*(GMT+02:00-2)*/ , "Cairo"
	/*(GMT+02:00-3)*/ , "Harare, Pretoria"
	/*(GMT+02:00-4)*/ , "Helsinki, Kyiv, Riga, Sofia, Tallinn, Vilnius"
	/*(GMT+02:00-5)*/ , "Jerusalem"
	/*(GMT+03:00)*/ , "Baghdad"
	/*(GMT+03:00-1)*/ , "Kuwait, Riyadh"
	/*(GMT+03:00-2)*/ , "Moscow, St. Petersburg, Volgograd"
	/*(GMT+03:00-3)*/ , "Nairobi"
	/*(GMT+03:30)*/ , "Tehran"
	/*(GMT+04:00)*/ , "Abu Dhabi, Muscat"
	/*(GMT+04:00-1)*/ , "Baku, Tbilisi, Yerevan"
	/*(GMT+04:30)*/ , "Kabul"
	/*(GMT+05:00)*/ , "Ekaterinburg"
	/*(GMT+05:00-1)*/ , "Islamabad, Karachi, Tashkent"
	/*(GMT+05:30)*/ , "Chennai, Kolkata, Mumbai, New Delhi"
	/*(GMT+05:45)*/ , "Kathmandu"
	/*(GMT+06:00)*/ , "Almaty, Novosibirsk"
	/*(GMT+06:00-1)*/ , "Astana, Dhaka"
	/*(GMT+06:00-2)*/ , "Sri Jayawardenepura"
	/*(GMT+06:30)*/ , "Rangoon"
	/*(GMT+07:00)*/ , "Bangkok, Hanoi, Jakarta"
	/*(GMT+07:00-1)*/ , "Krasnoyarsk"
	/*(GMT+08:00)*/ , "Beijing, Chongqing, Hong Kong, Urumqi"
	/*(GMT+08:00-1)*/ , "Irkutsk, Ulaan Bataar"
	/*(GMT+08:00-2)*/ , "Kuala Lumpur, Singapore"
	/*(GMT+08:00-3)*/ , "Perth"
	/*(GMT+08:00-4)*/ , "Taipei"
	/*(GMT+09:00)*/ , "Osaka, Sapporo, Tokyo"
	/*(GMT+09:00-1)*/ , "Seoul"
	/*(GMT+09:00-2)*/ , "Yakutsk"
	/*(GMT+09:30)*/ , "Adelaide"
	/*(GMT+09:30-1)*/ , "Darwin"
	/*(GMT+10:00)*/ , "Brisbane"
	/*(GMT+10:00-1)*/ , "Canberra, Melbourne, Sydney"
	/*(GMT+10:00-2)*/ , "Guam, Port Moresby"
	/*(GMT+10:00-3)*/ , "Hobart"
	/*(GMT+10:00-4)*/ , "Vladivostok"
	/*(GMT+11:00)*/ , "Magadan"
	/*(GMT+11:00-1)*/ , "Solomon Is., New Caledonia"
	/*(GMT+12:00)*/ , "Auckland, Wellington"
	/*(GMT+12:00-1)*/ , "Fiji, Kamchatka, Marshall Is."
};

int GetTimeZoneNameTr069(char* timezonename)
{
	int i = 0;
	int len = sizeof(TimezoneName) / sizeof(TimezoneName[0]);

	if ( NULL == timezonename )
		return -1;

	for( i=0; i < len; i++ )
	{
		if( 0 == strcmp(TimezoneName[i], timezonename) )
			return i;
	}

	return -1;
}
#endif

static char *TimeZoneTr069[]=
{
	"-12:00",  "-11:00", "-10:00", "-9:00", "-08:00", "-07:00", "-07:00-1", "-07:00-2", "-06:00", "-06:00-1",
	"-06:00-2", "-06:00-3", "-05:00", "-05:00-1", "-05:00-2", "-04:00", "-04:00-1", "-04:00-2", "-03:30",  "-03:00",
	"-03:00-1", "-03:00-2", "-02:00",  "-01:00",  "-01:00-1",  "00:00", "00:00-1", "+01:00", "+01:00-1", "+01:00-2",
	"+01:00-3", "+01:00-4", "+02:00", "+02:00-1", "+02:00-2", "+02:00-3", "+02:00-4", "+02:00-5", "+03:00", "+03:00-1",
	"+03:00-2", "+03:00-3", "+03:30", "+04:00", "+04:00-1", "+04:30", "+05:00", "+05:00-1", "+05:30", "+05:45",
	"+06:00", "+06:00-1", "+06:00-2", "+06:30", "+07:00", "+07:00-1", "+08:00", "+08:00-1", "+08:00-2", "+08:00-3",
	"+08:00-4", "+09:00", "+09:00-1", "+09:00-2", "+09:30", "+09:30-1", "+10:00", "+10:00-1", "+10:00-2", "+10:00-3",
	"+10:00-4", "+11:00", "+11:00-1", "+12:00", "+12:00-1"
};

int GetTimeZoneTr069(char* time)
{
	int i = 0;
	int len = sizeof(TimeZoneTr069) / sizeof(TimeZoneTr069[0]);

	if ( NULL == time )
		return -1;

	for( i=0; i < len; i++ )
	{
		if( 0 == strcmp(TimeZoneTr069[i], time) )
			return i;
	}

	return -1;  //the time zone with wrong format
}

int8 getTimeLocalTimeZoneValue(char** value, ParameterType* type, DeviceNode*  deviceNode){
	int  LocalTimeZoneIndex = -1;
	char TimeLocalTimeZone[8];
	char timezoneStr[32];

	memset(TimeLocalTimeZone, 0, sizeof(TimeLocalTimeZone));
	memset(timezoneStr, 0, sizeof(timezoneStr));
	tcapi_get("Timezone_Entry", "TZ", timezoneStr);	

	if ( 0 == strcmp(timezoneStr, "GMT") )
		strcpy(timezoneStr, "GMT00:00");
	else if ( 0 == strcmp(timezoneStr, "GMT-1") )
		strcpy(timezoneStr, "GMT00:00-1");

	LocalTimeZoneIndex = GetTimeZoneTr069(timezoneStr+3);

	if(LocalTimeZoneIndex < 0)
		return -1;
#if defined(TCSUPPORT_CT_PON_GD)
	sprintf(TimeLocalTimeZone,"GMT%s", TimeZoneTr069[LocalTimeZoneIndex]);	
#else
	sprintf(TimeLocalTimeZone,"%s", TimeZoneTr069[LocalTimeZoneIndex]);	
#endif
	return getTheValue(value, type, deviceNode, TimeLocalTimeZone);
}


int8   setTimeLocalTimeZoneValue(void* value, struct DeviceNode*  deviceNode){
	int  LocalTimeZoneIndex = -1;
	char timezoneStr[32];

	memset(timezoneStr, 0, sizeof(timezoneStr));
#if defined(TCSUPPORT_CT_PON_GD)
	strncpy(timezoneStr, (char*)value, sizeof(timezoneStr) - 1);
	if ( NULL == strstr(timezoneStr, "GMT") )
		return -1;
	LocalTimeZoneIndex = GetTimeZoneTr069(&timezoneStr[3]);
	if (LocalTimeZoneIndex < 0)
		return -1;
	if( 0 == strcmp(timezoneStr, "GMT00:00") )
		strcpy(timezoneStr, "GMT");
	else if ( 0 == strcmp(timezoneStr, "GMT00:00-1") )
		strcpy(timezoneStr, "GMT-1");
	tcapi_set("Timezone_Entry", "TZ", timezoneStr);
#else
	if(strcmp((char*)value, "00:00") == 0)
		strcpy(timezoneStr, "GMT");
	else if ( 0 == strcmp((char*)value, "00:00-1") )
		strcpy(timezoneStr, "GMT-1");
	else{
		LocalTimeZoneIndex = GetTimeZoneTr069(value);
		if (LocalTimeZoneIndex < 0)
			return -1;
		else
			sprintf(timezoneStr, "GMT%s",  TimeZoneTr069[LocalTimeZoneIndex]);	
	}
	tcapi_set("Timezone_Entry", "TZ", timezoneStr);
#endif

	SetCfgNodeFlag( "Timezone_Entry", -1 );
	return 0;
}              

int8 getTimeLocalTimeZoneAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode){
	uint8  attribute;
	int8  result;

	attribute = tcTimeAttr.aTimeLocalTimeZone;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8   setTimeLocalTimeZoneAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode){
	uint8*  attribute;
	int8  result;

	attribute = &tcTimeAttr.aTimeLocalTimeZone;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TIME_ATTR_FLAG;

	return result;
}

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
#define MAX_NTP_SYNC_TIME 24*60*60
int8 getTimeNTPServer2Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char TimeNTPServer2[32];
	char timeSvrType[8];
	
	memset(TimeNTPServer2, 0, sizeof(TimeNTPServer2));
	memset(timeSvrType, 0, sizeof(timeSvrType));
	
	tcapi_get("Timezone_Entry", "TYPE", timeSvrType);
	tcapi_get("Timezone_Entry", "SERVER2", TimeNTPServer2);

	if(!strcmp(timeSvrType, NTP_TIME)){
		if (!strcmp("", TimeNTPServer2))
			strcpy(TimeNTPServer2, "0.0.0.0");
	}
	else
		sprintf(TimeNTPServer2,"%s","The NTPServer is diable!");	
	
	return getTheValue(value, type, deviceNode, TimeNTPServer2);
}

int8   setTimeNTPServer2Value(void* value, struct DeviceNode*  deviceNode)
{
	char* TimeNTPServer2  = NULL;

	if(strlen((char*)value) > 31)
		return -1;
	
	TimeNTPServer2 = (char*)value;

	tcapi_set("Timezone_Entry", "TYPE", NTP_TIME);
	tcapi_set("Timezone_Entry", "SERVER2", TimeNTPServer2);
	
	SetCfgNodeFlag( "Timezone_Entry", -1 );
	
	return 0;
}    

int8 getTimeNTPServer2Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;

	attribute = tcTimeAttr.aTimeNTPServer2;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8   setTimeNTPServer2Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;

	attribute = &tcTimeAttr.aTimeNTPServer2;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TIME_ATTR_FLAG;
	return result;
}

int8 getTimeNTPServer3Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char TimeNTPServer3[32];
	char timeSvrType[8];
	
	memset(TimeNTPServer3, 0, sizeof(TimeNTPServer3));
	memset(timeSvrType, 0, sizeof(timeSvrType));

	
	tcapi_get("Timezone_Entry", "TYPE", timeSvrType);
	tcapi_get("Timezone_Entry", "SERVER3", TimeNTPServer3);

	if(!strcmp(timeSvrType, NTP_TIME)){
		if (!strcmp("", TimeNTPServer3))
			strcpy(TimeNTPServer3, "0.0.0.0");
	}
	else
		sprintf(TimeNTPServer3,"%s","The NTPServer is diable!");	
	
	return getTheValue(value, type, deviceNode, TimeNTPServer3);
}

int8   setTimeNTPServer3Value(void* value, struct DeviceNode*  deviceNode)
{
	char* TimeNTPServer3  = NULL;

	if(strlen((char*)value) > 31)
		return -1;
	
	TimeNTPServer3 = (char*)value;

	tcapi_set("Timezone_Entry", "TYPE", NTP_TIME);
	tcapi_set("Timezone_Entry", "SERVER3", TimeNTPServer3);
	
	SetCfgNodeFlag( "Timezone_Entry", -1 );
	
	return 0;
}    

int8 getTimeNTPServer3Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;

	attribute = tcTimeAttr.aTimeNTPServer3;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8   setTimeNTPServer3Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;

	attribute = &tcTimeAttr.aTimeNTPServer3;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TIME_ATTR_FLAG;
	return result;
}

int8 getTimeNTPServer4Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char TimeNTPServer4[32];
	char timeSvrType[8];
	
	memset(TimeNTPServer4, 0, sizeof(TimeNTPServer4));
	memset(timeSvrType, 0, sizeof(timeSvrType));

	
	tcapi_get("Timezone_Entry", "TYPE", timeSvrType);
	tcapi_get("Timezone_Entry", "SERVER4", TimeNTPServer4);

	if(!strcmp(timeSvrType, NTP_TIME)){
		if (!strcmp("", TimeNTPServer4))
			strcpy(TimeNTPServer4, "0.0.0.0");
	}
	else
		sprintf(TimeNTPServer4,"%s","The NTPServer is diable!");	
	
	return getTheValue(value, type, deviceNode, TimeNTPServer4);
}

int8   setTimeNTPServer4Value(void* value, struct DeviceNode*  deviceNode)
{
	char* TimeNTPServer4  = NULL;

	if(strlen((char*)value) > 31)
		return -1;
	
	TimeNTPServer4 = (char*)value;

	tcapi_set("Timezone_Entry", "TYPE", NTP_TIME);
	tcapi_set("Timezone_Entry", "SERVER4", TimeNTPServer4);
	
	SetCfgNodeFlag( "Timezone_Entry", -1 );
	
	return 0;
}    

int8 getTimeNTPServer4Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;

	attribute = tcTimeAttr.aTimeNTPServer4;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8   setTimeNTPServer4Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;

	attribute = &tcTimeAttr.aTimeNTPServer4;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TIME_ATTR_FLAG;
	return result;
}

int8 getTimeNTPServer5Value(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char TimeNTPServer5[32];
	char timeSvrType[8];
	
	memset(TimeNTPServer5, 0, sizeof(TimeNTPServer5));
	memset(timeSvrType, 0, sizeof(timeSvrType));

	
	tcapi_get("Timezone_Entry", "TYPE", timeSvrType);
	tcapi_get("Timezone_Entry", "SERVER5", TimeNTPServer5);

	if(!strcmp(timeSvrType, NTP_TIME)){
		if (!strcmp("", TimeNTPServer5))
			strcpy(TimeNTPServer5, "0.0.0.0");
	}
	else
		sprintf(TimeNTPServer5,"%s","The NTPServer is diable!");	
	
	return getTheValue(value, type, deviceNode, TimeNTPServer5);
}

int8   setTimeNTPServer5Value(void* value, struct DeviceNode*  deviceNode)
{
	char* TimeNTPServer5  = NULL;

	if(strlen((char*)value) > 31)
		return -1;
	
	TimeNTPServer5 = (char*)value;

	tcapi_set("Timezone_Entry", "TYPE", NTP_TIME);
	tcapi_set("Timezone_Entry", "SERVER5", TimeNTPServer5);
	
	SetCfgNodeFlag( "Timezone_Entry", -1 );
	
	return 0;
}    

int8 getTimeNTPServer5Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;

	attribute = tcTimeAttr.aTimeNTPServer5;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8   setTimeNTPServer5Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;

	attribute = &tcTimeAttr.aTimeNTPServer5;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TIME_ATTR_FLAG;
	return result;
}


int8 getNTPIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char syncTime[10] = {0};
	
	memset(syncTime, 0, sizeof(syncTime));
	
	tcapi_get("Timezone_Entry", "SYNCTIME", syncTime);

	if(syncTime[0] == '\0')
		sprintf(syncTime, "%d", 86400);
	else
		sprintf(syncTime, "%d", atoi(syncTime));
	
	return getTheValue(value, type, deviceNode, syncTime);
}

int8   setNTPIntervalValue(void* value, struct DeviceNode*  deviceNode)
{
	char syncTime[10] = {0};

	memset(syncTime, 0x00, sizeof(syncTime));
	
	if(*((uint32*)value) > MAX_NTP_SYNC_TIME)
		return -1;

	sprintf(syncTime, "%d", *((uint32*)value));

	tcapi_set("Timezone_Entry", "SYNCTIME", syncTime);
	
	SetCfgNodeFlag( "Timezone_Entry", -1 );
	
	return 0;
}    

int8 getNTPIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;

	attribute = tcTimeAttr.aNTPInterval;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8   setNTPIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;

	attribute = &tcTimeAttr.aNTPInterval;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TIME_ATTR_FLAG;
	return result;
}

int8 getNTPServerTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char timeSvrType[8] = {0};
	
	memset(timeSvrType, 0, sizeof(timeSvrType));
	
	tcapi_get("Timezone_Entry", "NTPServerType", timeSvrType);

	if(timeSvrType[0] == '\0')
		sprintf(timeSvrType, "%d", 0);

	return getTheValue(value, type, deviceNode, timeSvrType);
}

int8   setNTPServerTypeValue(void* value, struct DeviceNode*  deviceNode)
{
	char timeSvrType[10] = {0};

	uint32 typeValue = 0;

	typeValue = *(uint32*)value;

	if(typeValue != 0 && typeValue != 1 && typeValue != 3
#if defined(TCSUPPORT_VOIP)
		&& typeValue != 2
#endif
	)
		return -1;
	
	sprintf(timeSvrType, "%d", typeValue);

	tcapi_set("Timezone_Entry", "NTPServerType", timeSvrType);
	
	SetCfgNodeFlag( "Timezone_Entry", -1 );
	
	return 0;
}    

int8 getNTPServerTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8  attribute = 0;
	int8  result = 0;

	attribute = tcTimeAttr.aNTPServerType;
	result=GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8   setNTPServerTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8*  attribute = NULL;
	int8  result = 0;

	attribute = &tcTimeAttr.aNTPServerType;
	result=SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TIME_ATTR_FLAG;
	return result;
}

#endif

#if defined(TCSUPPORT_CUC_CWMP_PARAMETER) || defined(TCSUPPORT_CT_PON_GD)
int8 getNTPEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char Enable[10] = {0};

	if ( tcapi_get("Timezone_Entry", "TYPE", Enable) < 0 )
		strcpy(Enable, "2");

	if( 0 == strcmp(Enable, "0") )
		strcpy(Enable, "1");
	else
		strcpy(Enable, "0");

	return getTheValue(value, type, deviceNode, Enable);
}
int8 setNTPEnableValue(void* value, struct DeviceNode*  deviceNode)
{
	uint8 Enable;
	char s_Enable[10] = {0};

	Enable = *((uint8*)value);

	if( 1 == Enable )
		strcpy(s_Enable, "0");
	else if( 0 == Enable )
		strcpy(s_Enable, "2");
	else
		return -1;

	tcapi_set("Timezone_Entry", "TYPE", s_Enable);
	SetCfgNodeFlag( "Timezone_Entry", -1 );
	return 0;
}
int8 getNTPEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTimeAttr.aEnable;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setNTPEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTimeAttr.aEnable;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TIME_ATTR_FLAG;
	return result;
}

int8 getTimeLocalTimeZoneNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	int  LocalTimeZoneIndex = -1;
	char TimeLocalTimeZoneName[128];
	char timezoneStr[32];

	memset(TimeLocalTimeZoneName, 0, sizeof(TimeLocalTimeZoneName));
	memset(timezoneStr, 0, sizeof(timezoneStr));
	tcapi_get("Timezone_Entry", "TZ", timezoneStr);	

	if ( 0 == strcmp(timezoneStr, "GMT") )
		strcpy(timezoneStr, "GMT00:00");
	else if ( 0 == strcmp(timezoneStr, "GMT-1") )
		strcpy(timezoneStr, "GMT00:00-1");

	LocalTimeZoneIndex = GetTimeZoneTr069(timezoneStr+3);
	if(LocalTimeZoneIndex < 0)
		return -1;

	sprintf(TimeLocalTimeZoneName,"%s", TimezoneName[LocalTimeZoneIndex]);	

	return getTheValue(value, type, deviceNode, TimeLocalTimeZoneName);
}
int8 setTimeLocalTimeZoneNameValue(void* value, struct DeviceNode*  deviceNode)
{
	int  LocalTimeZoneIndex = -1;
	char timezoneStr[32] = {0};

	memset(timezoneStr, 0, sizeof(timezoneStr));
	LocalTimeZoneIndex = GetTimeZoneNameTr069((char*)value);
	if (LocalTimeZoneIndex < 0)
		return -1;
	sprintf(timezoneStr, "GMT%s",  TimeZoneTr069[LocalTimeZoneIndex]);
	if( 0 == strcmp(timezoneStr, "GMT00:00") )
		strcpy(timezoneStr, "GMT");
	else if ( 0 == strcmp(timezoneStr, "GMT00:00-1") )
		strcpy(timezoneStr, "GMT-1");
	tcapi_set("Timezone_Entry", "TZ", timezoneStr);

	SetCfgNodeFlag( "Timezone_Entry", -1 );
	return 0;
}              
int8 getTimeLocalTimeZoneNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTimeAttr.aTimeLocalTimeZoneName;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}
int8 setTimeLocalTimeZoneNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTimeAttr.aTimeLocalTimeZoneName;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TIME_ATTR_FLAG;

	return result;
}
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
int8 getNTPStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode)
{
	char Status[128] = {0};

	if ( tcapi_get("Timezone_Entry", "Status", Status) < 0 )
		Status[0] = '\0';

	return getTheValue(value, type, deviceNode, Status);
}
int8 getNTPStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode)
{
	uint8 attribute = 0;
	int8 result = 0;

	attribute = tcTimeAttr.aStatus;
	result = GetTheAttributes(attribute,notification, acl, deviceNode);
	return result;
}

int8 setNTPStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode)
{
	uint8* attribute = NULL;
	int8 result = 0;

	attribute = &tcTimeAttr.aStatus;
	result = SetTheAttributes(attribute, notificationChange, notification, accessListChange, acl, deviceNode);
	rpc.attrFlags.theSettingFlag0 |= TIME_ATTR_FLAG;
	return result;
}
#endif


