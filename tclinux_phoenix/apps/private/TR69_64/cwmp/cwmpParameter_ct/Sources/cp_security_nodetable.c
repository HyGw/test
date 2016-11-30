#include "Global_res.h"
#include "cp_security.h"


static NodeFunc Security_IPFilterInEnable=
{
	NULL,
	NULL,
	getSecurity_IPFilterInEnableValue, 
	setSecurity_IPFilterInEnableValue, 
	getSecurity_IPFilterInEnableAttribute, 
	setSecurity_IPFilterInEnableAttribute, 
	NULL,
};

static NodeFunc Security_IPFilterInPolicy=
{
	NULL,
	NULL,
	getSecurity_IPFilterInPolicyValue, 
	setSecurity_IPFilterInPolicyValue, 
	getSecurity_IPFilterInPolicyAttribute, 
	setSecurity_IPFilterInPolicyAttribute, 
	NULL,
};

static NodeFunc Security_IPFilterOutEnable=
{
	NULL,
	NULL,
	getSecurity_IPFilterOutEnableValue, 
	setSecurity_IPFilterOutEnableValue, 
	getSecurity_IPFilterOutEnableAttribute, 
	setSecurity_IPFilterOutEnableAttribute, 
	NULL,
};

static NodeFunc Security_IPFilterOutPolicy=
{
	NULL,
	NULL,
	getSecurity_IPFilterOutPolicyValue, 
	setSecurity_IPFilterOutPolicyValue, 
	getSecurity_IPFilterOutPolicyAttribute, 
	setSecurity_IPFilterOutPolicyAttribute, 
	NULL,
};

static NodeFunc Security_IPFilterFuncs=
{
	addSecurity_IPFilterObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitSecurity_IPFilterTableFromFlash
};

static NodeFunc Security_IPFilter_Name=
{
	NULL,
	NULL,
	getSecurity_IPFilter_NameValue, 
	setSecurity_IPFilter_NameValue, 
	getSecurity_IPFilter_NameAttribute, 
	setSecurity_IPFilter_NameAttribute, 
	NULL,
};

static NodeFunc Security_IPFilter_Enable=
{
	NULL,
	NULL,
	getSecurity_IPFilter_EnableValue, 
	setSecurity_IPFilter_EnableValue, 
	getSecurity_IPFilter_EnableAttribute, 
	setSecurity_IPFilter_EnableAttribute, 
	NULL,
};

static NodeFunc Security_IPFilter_SourceIPStart=
{
	NULL,
	NULL,
	getSecurity_IPFilter_SourceIPStartValue, 
	setSecurity_IPFilter_SourceIPStartValue, 
	getSecurity_IPFilter_SourceIPStartAttribute, 
	setSecurity_IPFilter_SourceIPStartAttribute, 
	NULL,
};

static NodeFunc Security_IPFilter_SourceIPEnd=
{
	NULL,
	NULL,
	getSecurity_IPFilter_SourceIPEndValue, 
	setSecurity_IPFilter_SourceIPEndValue, 
	getSecurity_IPFilter_SourceIPEndAttribute, 
	setSecurity_IPFilter_SourceIPEndAttribute, 
	NULL,
};

static NodeFunc Security_IPFilter_DestIPStart=
{
	NULL,
	NULL,
	getSecurity_IPFilter_DestIPStartValue, 
	setSecurity_IPFilter_DestIPStartValue, 
	getSecurity_IPFilter_DestIPStartAttribute, 
	setSecurity_IPFilter_DestIPStartAttribute, 
	NULL,
};

static NodeFunc Security_IPFilter_DestIPEnd=
{
	NULL,
	NULL,
	getSecurity_IPFilter_DestIPEndValue, 
	setSecurity_IPFilter_DestIPEndValue, 
	getSecurity_IPFilter_DestIPEndAttribute, 
	setSecurity_IPFilter_DestIPEndAttribute, 
	NULL,
};

static NodeFunc Security_IPFilter_Protocol=
{
	NULL,
	NULL,
	getSecurity_IPFilter_ProtocolValue, 
	setSecurity_IPFilter_ProtocolValue, 
	getSecurity_IPFilter_ProtocolAttribute, 
	setSecurity_IPFilter_ProtocolAttribute, 
	NULL,
};

static NodeFunc Security_IPFilter_SourcePortStart=
{
	NULL,
	NULL,
	getSecurity_IPFilter_SourcePortStartValue, 
	setSecurity_IPFilter_SourcePortStartValue, 
	getSecurity_IPFilter_SourcePortStartAttribute, 
	setSecurity_IPFilter_SourcePortStartAttribute, 
	NULL,
};

static NodeFunc Security_IPFilter_SourcePortEnd=
{
	NULL,
	NULL,
	getSecurity_IPFilter_SourcePortEndValue, 
	setSecurity_IPFilter_SourcePortEndValue, 
	getSecurity_IPFilter_SourcePortEndAttribute, 
	setSecurity_IPFilter_SourcePortEndAttribute, 
	NULL,
};

static NodeFunc Security_IPFilter_DestPortStart=
{
	NULL,
	NULL,
	getSecurity_IPFilter_DestPortStartValue, 
	setSecurity_IPFilter_DestPortStartValue, 
	getSecurity_IPFilter_DestPortStartAttribute, 
	setSecurity_IPFilter_DestPortStartAttribute, 
	NULL,
};

static NodeFunc Security_IPFilter_DestPortEnd=
{
	NULL,
	NULL,
	getSecurity_IPFilter_DestPortEndValue, 
	setSecurity_IPFilter_DestPortEndValue, 
	getSecurity_IPFilter_DestPortEndAttribute, 
	setSecurity_IPFilter_DestPortEndAttribute, 
	NULL,
};

static NodeFunc Security_UrlFilterEnable=
{
	NULL,
	NULL,
	getSecurity_UrlFilterEnableValue, 
	setSecurity_UrlFilterEnableValue, 
	getSecurity_UrlFilterEnableAttribute, 
	setSecurity_UrlFilterEnableAttribute, 
	NULL,
};

static NodeFunc Security_UrlFilterPolicy=
{
	NULL,
	NULL,
	getSecurity_UrlFilterPolicyValue, 
	setSecurity_UrlFilterPolicyValue, 
	getSecurity_UrlFilterPolicyAttribute, 
	setSecurity_UrlFilterPolicyAttribute, 
	NULL,
};

static NodeFunc Security_UrlFilterFuncs =
{
	addSecurity_UrlFilterObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitSecurity_UrlFilterTableFromFlash
};

static NodeFunc Security_UrlFilterActivate=
{
	NULL,
	NULL,
	getSecurity_UrlFilterValue, 
	setSecurity_UrlFilterValue, 
	getSecurity_UrlFilterAttribute, 
	setSecurity_UrlFilterAttribute, 
	NULL,
};

/*
	Parent Node: InternetGatewayDevice.Security.
	Parent Node Table Location: cp_topnodetable.c 
*/
CwmpParameterStruct Security[] = 
{
	{"IPFilterInEnable", BooleanW, NoLimite, &Security_IPFilterInEnable, NULL},
	{"IPFilterInPolicy", UnsignIntW, NoLimite, &Security_IPFilterInPolicy, NULL},
	{"IPFilterOutEnable", BooleanW, NoLimite, &Security_IPFilterOutEnable, NULL},
	{"IPFilterOutPolicy", UnsignIntW, NoLimite, &Security_IPFilterOutPolicy, NULL},
	{"IPFilterIn", ObjectW, NoLimite, &Security_IPFilterFuncs, NULL},
	{"IPFilterOut", ObjectW, NoLimite, &Security_IPFilterFuncs, NULL},
	{"UrlFilterEnable", BooleanW, NoLimite, &Security_UrlFilterEnable, NULL},
	{"UrlFilterPolicy", UnsignIntW, NoLimite, &Security_UrlFilterPolicy, NULL},
	{"ParentalCtrl", ObjectR, NoLimite, NULL, Security_ParentalCtrl},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc Security_IPFilterIndexFuncs=
{
	NULL,
	deleteSecurity_IPFilterObject,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	NULL
};

static NodeFunc Security_UrlFilterIndexFuncs=
{
	NULL,
	deleteSecurity_UrlFilterObject,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	NULL
};

/*
	Parent Node: InternetGatewayDevice.Security.IPFilterIn.
		or	InternetGatewayDevice.Security.IPFilterOut.
	Parent Node Table Location: cp_security_nodetable.c
*/
CwmpParameterStruct Security_IPFilterIndex[] = 
{
	{NULL, ObjectW, NoLimite, &Security_IPFilterIndexFuncs, Security_IPFilter},
	{NULL, -1, -1, NULL, NULL}	
};
/*
	Parent Node: InternetGatewayDevice.Security.ParentalCtrl.Templates.{i}.UrlFilter.
	Parent Node Table Location: cp_security_nodetable.c

*/
CwmpParameterStruct Security_UrlFilterIndex[] = 
{
	{NULL, ObjectW, NoLimite, &Security_UrlFilterIndexFuncs, Security_UrlAddress},
	{NULL, -1, -1, NULL, NULL}	
};

/*
	Parent Node: InternetGatewayDevice.Security.IPFilterIn.{i}.
		or	InternetGatewayDevice.Security.IPFilterOut.{i}.
	Parent Node Table Location: cp_security_nodetable.c
*/
CwmpParameterStruct Security_IPFilter[] = 
{
	{"Name", StrW, NoLimite, &Security_IPFilter_Name, NULL},
	{"Enable", BooleanW, NoLimite, &Security_IPFilter_Enable, NULL},
	{"SourceIPStart", StrW, NoLimite, &Security_IPFilter_SourceIPStart, NULL},
	{"SourceIPEnd", StrW, NoLimite, &Security_IPFilter_SourceIPEnd, NULL},
	{"DestIPStart",StrW,NoLimite,&Security_IPFilter_DestIPStart,NULL},
	{"DestIPEnd",StrW,NoLimite,&Security_IPFilter_DestIPEnd,NULL},
	{"Protocol", StrW, NoLimite, &Security_IPFilter_Protocol, NULL},
	{"SourcePortStart", UnsignIntW, NoLimite, &Security_IPFilter_SourcePortStart, NULL},
	{"SourcePortEnd", UnsignIntW, NoLimite, &Security_IPFilter_SourcePortEnd, NULL},
	{"DestPortStart", UnsignIntW, NoLimite, &Security_IPFilter_DestPortStart, NULL},
	{"DestPortEnd", UnsignIntW, NoLimite, &Security_IPFilter_DestPortEnd, NULL},
	{NULL, -1, -1, NULL, NULL}	
};

/*
	Parent Node: InternetGatewayDevice.Security.ParentalCtrl.
	Parent Node Table Location: cp_security_nodetable.c
*/
CwmpParameterStruct Security_ParentalCtrl[] = 
{
	{"Templates", ObjectR, NoLimite, NULL, Security_Templates},
	{NULL, -1, -1, NULL, NULL}
};
/*
	Parent Node: InternetGatewayDevice.Security.ParentalCtrl.Templates.
	Parent Node Table Location: cp_security_nodetable.c
*/
CwmpParameterStruct Security_Templates[] = 
{
	{"1", ObjectR, NoLimite, NULL, Security_UrlFilter},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.Security.ParentalCtrl.Templates.{i}.
	Parent Node Table Location: cp_security_nodetable.c
*/
CwmpParameterStruct Security_UrlFilter[] = 
{
	{"UrlFilter", ObjectW, NoLimite, &Security_UrlFilterFuncs, NULL},
	{NULL, -1, -1, NULL, NULL}
};
/*
	Parent Node: InternetGatewayDevice.Security.ParentalCtrl.Templates.{i}.UrlFilter.{i}.
	Parent Node Table Location: cp_security_nodetable.c
*/
CwmpParameterStruct Security_UrlAddress[] = 
{
	{"UrlAddress", StrW, NoLimite, &Security_UrlFilterActivate, NULL},
	{NULL, -1, -1, NULL, NULL}
};
