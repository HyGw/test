#include "Global_res.h"
#include "cp_xctcom_dns_speed_limit.h"

/*******************  InternetGatewayDevice.X_CT-COM_DNSSpeedLimit. start ************************/

static NodeFunc DnsSpeedLimitDomain =
{
	NULL,
	NULL,
	getDnsSpeedLimitDomainValue,
	setDnsSpeedLimitDomainValue,
	getDnsSpeedLimitDomainAttribute,
	setDnsSpeedLimitDomainAttribute,
	NULL,
};

static NodeFunc DnsSpeedLimitAction =
{
	NULL,
	NULL,
	getDnsSpeedLimitActionValue,
	setDnsSpeedLimitActionValue,
	getDnsSpeedLimitActionAttribute,
	setDnsSpeedLimitActionAttribute,
	NULL,
};

static NodeFunc DnsSpeedLimitHgwInfo =
{
	NULL,
	NULL,
	getDnsSpeedLimitHgwInfoValue,
	NULL,
	getDnsSpeedLimitHgwInfoAttribute,
	setDnsSpeedLimitHgwInfoAttribute,
	NULL,
};

static NodeFunc DnsSpeedLimitDeviceInfo =
{
	NULL,
	NULL,
	getDnsSpeedLimitDeviceInfoValue,
	NULL,
	getDnsSpeedLimitDeviceInfoAttribute,
	setDnsSpeedLimitDeviceInfoAttribute,
	NULL,
};

CwmpParameterStruct DnsSpeedLimit[] =
{
	{"Domain", StrW, NoLimite, &DnsSpeedLimitDomain, NULL},
	{"LimitAction", StrW, NoLimite, &DnsSpeedLimitAction, NULL},
	{"HgwInfo", StrR, NoLimite, &DnsSpeedLimitHgwInfo, NULL},
	{"DeviceInfo", StrR, NoLimite, &DnsSpeedLimitDeviceInfo, NULL},
	{NULL, -1, -1, NULL, NULL}
};
