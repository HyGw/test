#include "Global_res.h"
#include "cp_landev_dhcpOption.h"

static NodeFunc dhcpOption_Enable=
{
	NULL,
	NULL,
	getdhcpOption_EnableValue, 
	setdhcpOption_EnableValue, 
	getdhcpOption_EnableAttribute, 
	setdhcpOption_EnableAttribute, 
	NULL,
};

static NodeFunc dhcpOption_Tag=
{
	NULL,
	NULL,
	getdhcpOption_TagValue, 
	setdhcpOption_TagValue, 
	getdhcpOption_TagAttribute, 
	setdhcpOption_TagAttribute, 
	NULL,
};

static NodeFunc dhcpOption_Value=
{
	NULL,
	NULL,
	getdhcpOption_ValueValue, 
	setdhcpOption_ValueValue, 
	getdhcpOption_ValueAttribute, 
	setdhcpOption_ValueAttribute, 
	NULL,
};

static NodeFunc dhcpConSer_Enable=
{
	NULL,
	NULL,
	getdhcpConSer_EnableValue, 
	setdhcpConSer_EnableValue, 
	getdhcpConSer_EnableAttribute, 
	setdhcpConSer_EnableAttribute, 
	NULL,
};

static NodeFunc PoolOrder=
{
	NULL,
	NULL,
	getPoolOrderValue, 
	setPoolOrderValue, 
	getPoolOrderAttribute, 
	setPoolOrderAttribute, 
	NULL,
};

static NodeFunc SourceInterface=
{
	NULL,
	NULL,
	getSourceInterfaceValue, 
	setSourceInterfaceValue, 
	getSourceInterfaceAttribute, 
	setSourceInterfaceAttribute, 
	NULL,
};

static NodeFunc VendorClassID=
{
	NULL,
	NULL,
	getVendorClassIDValue, 
	setVendorClassIDValue, 
	getVendorClassIDAttribute, 
	setVendorClassIDAttribute, 
	NULL,
};

static NodeFunc VendorClassIDExclude=
{
	NULL,
	NULL,
	getVendorClassIDExcludeValue, 
	setVendorClassIDExcludeValue, 
	getVendorClassIDExcludeAttribute, 
	setVendorClassIDExcludeAttribute, 
	NULL,
};

static NodeFunc VendorClassIDMode=
{
	NULL,
	NULL,
	getVendorClassIDModeValue, 
	setVendorClassIDModeValue, 
	getVendorClassIDModeAttribute, 
	setVendorClassIDModeAttribute, 
	NULL,
};

static NodeFunc ClientID=
{
	NULL,
	NULL,
	getClientIDValue, 
	setClientIDValue, 
	getClientIDAttribute, 
	setClientIDAttribute, 
	NULL,
};

static NodeFunc ClientIDExclude=
{
	NULL,
	NULL,
	getClientIDExcludeValue, 
	setClientIDExcludeValue, 
	getClientIDExcludeAttribute, 
	setClientIDExcludeAttribute, 
	NULL,
};

static NodeFunc UserClassID=
{
	NULL,
	NULL,
	getUserClassIDValue, 
	setUserClassIDValue, 
	getUserClassIDAttribute, 
	setUserClassIDAttribute, 
	NULL,
};

static NodeFunc UserClassIDExclude=
{
	NULL,
	NULL,
	getUserClassIDExcludeValue, 
	setUserClassIDExcludeValue, 
	getUserClassIDExcludeAttribute, 
	setUserClassIDExcludeAttribute, 
	NULL,
};

static NodeFunc Chaddr=
{
	NULL,
	NULL,
	getChaddrValue, 
	setChaddrValue, 
	getChaddrAttribute, 
	setChaddrAttribute, 
	NULL,
};

static NodeFunc ChaddrMask=
{
	NULL,
	NULL,
	getChaddrMaskValue, 
	setChaddrMaskValue, 
	getChaddrMaskAttribute, 
	setChaddrMaskAttribute, 
	NULL,
};

static NodeFunc ChaddrExclude=
{
	NULL,
	NULL,
	getChaddrExcludeValue, 
	setChaddrExcludeValue, 
	getChaddrExcludeAttribute, 
	setChaddrExcludeAttribute, 
	NULL,
};

static NodeFunc LocallyServed=
{
	NULL,
	NULL,
	getLocallyServedValue, 
	setLocallyServedValue, 
	getLocallyServedAttribute, 
	setLocallyServedAttribute, 
	NULL,
};

static NodeFunc MinAddress=
{
	NULL,
	NULL,
	getMinAddressValue, 
	setMinAddressValue, 
	getMinAddressAttribute, 
	setMinAddressAttribute, 
	NULL,
};

static NodeFunc MaxAddress=
{
	NULL,
	NULL,
	getMaxAddressValue, 
	setMaxAddressValue, 
	getMaxAddressAttribute, 
	setMaxAddressAttribute, 
	NULL,
};

static NodeFunc ReservedAddresses=
{
	NULL,
	NULL,
	getReservedAddressesValue, 
	setReservedAddressesValue, 
	getReservedAddressesAttribute, 
	setReservedAddressesAttribute, 
	NULL,
};

static NodeFunc dhcpConSer_Mask=
{
	NULL,
	NULL,
	getdhcpConSer_MaskValue, 
	setdhcpConSer_MaskValue, 
	getdhcpConSer_MaskAttribute, 
	setdhcpConSer_MaskAttribute, 
	NULL,
};

static NodeFunc dhcpConSer_DNS=
{
	NULL,
	NULL,
	getdhcpConSer_DNSValue, 
	setdhcpConSer_DNSValue, 
	getdhcpConSer_DNSAttribute, 
	setdhcpConSer_DNSAttribute, 
	NULL,
};

static NodeFunc DomainName=
{
	NULL,
	NULL,
	getDomainNameValue, 
	setDomainNameValue, 
	getDomainNameAttribute, 
	setDomainNameAttribute, 
	NULL,
};

static NodeFunc IPRouters=
{
	NULL,
	NULL,
	getIPRoutersValue, 
	setIPRoutersValue, 
	getIPRoutersAttribute, 
	setIPRoutersAttribute, 
	NULL,
};

static NodeFunc DHCPLeaseTime=
{
	NULL,
	NULL,
	getDHCPLeaseTimeValue, 
	setDHCPLeaseTimeValue, 
	getDHCPLeaseTimeAttribute, 
	setDHCPLeaseTimeAttribute, 
	NULL,
};

static NodeFunc UseAllocatedWAN=
{
	NULL,
	NULL,
	getUseAllocatedWANValue, 
	setUseAllocatedWANValue, 
	getUseAllocatedWANAttribute, 
	setUseAllocatedWANAttribute, 
	NULL,
};

static NodeFunc AssociatedConnection=
{
	NULL,
	NULL,
	getAssociatedConnectionValue, 
	setAssociatedConnectionValue, 
	getAssociatedConnectionAttribute, 
	setAssociatedConnectionAttribute, 
	NULL,
};

static NodeFunc DHCPServerIPAddress=
{
	NULL,
	NULL,
	getDHCPServerIPAddressValue, 
	setDHCPServerIPAddressValue, 
	getDHCPServerIPAddressAttribute, 
	setDHCPServerIPAddressAttribute, 
	NULL,
};

static NodeFunc DHCPStaticAddressNum=
{
	NULL,
	NULL,
	getDHCPStaticAddressNumValue, 
	NULL, 
	getDHCPStaticAddressNumAttribute, 
	setDHCPStaticAddressNumAttribute, 
	NULL,
};

static NodeFunc dhcpConSer_OptionNum=
{
	NULL,
	NULL,
	getdhcpConSer_OptionNumValue, 
	NULL, 
	getdhcpConSer_OptionNumAttribute, 
	setdhcpConSer_OptionNumAttribute, 
	NULL,
};

static NodeFunc dhcpConSer_OptionFuncs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitDHCPConSerOptionSubTree,
};

static NodeFunc conSerOption_Enable=
{
	NULL,
	NULL,
	getconSerOption_EnableValue, 
	setconSerOption_EnableValue, 
	getconSerOption_EnableAttribute, 
	setconSerOption_EnableAttribute, 
	NULL,
};

static NodeFunc conSerOption_Tag=
{
	NULL,
	NULL,
	getconSerOption_TagValue, 
	setconSerOption_TagValue, 
	getconSerOption_TagAttribute, 
	setconSerOption_TagAttribute, 
	NULL,
};

static NodeFunc conSerOption_Value=
{
	NULL,
	NULL,
	getconSerOption_ValueValue, 
	setconSerOption_ValueValue, 
	getconSerOption_ValueAttribute, 
	setconSerOption_ValueAttribute, 
	NULL,
};

/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPOption.{i}.
	Parent Node Table Location: cp_landev_dhcpOption_nodetable.c
*/

CwmpParameterStruct DHCPOption[]= 
{
	{"Enable", BooleanW, NoLimite, &dhcpOption_Enable, NULL},
	{"Tag", UnsignIntW, NoLimite, &dhcpOption_Tag, NULL},
	{"Value", Base64W, NoLimite, &dhcpOption_Value, NULL},
	{NULL, -1, -1, NULL, NULL}
};

/*
      This Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPOption.{i}.
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPOption.
	Parent Node Table Location: cp_landev_nodetable.c
*/

CwmpParameterStruct DHCPOptionIndex[]= 
{
	{NULL, ObjectR, NoLimite, NULL, DHCPOption},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPConditionalServingPool.{i}.
	Parent Node Table Location: cp_landev_dhcpOption_nodetable.c
*/

CwmpParameterStruct DHCPConSerPool[]= 
{
	{"Enable", BooleanW, NoLimite, &dhcpConSer_Enable, NULL},
	{"PoolOrder", UnsignIntW, NoLimite, &PoolOrder, NULL},
	{"SourceInterface", StrW, NoLimite, &SourceInterface, NULL},
	{"VendorClassID", StrW, NoLimite, &VendorClassID, NULL},
	{"VendorClassIDExclude", BooleanW, NoLimite, &VendorClassIDExclude, NULL},
	{"VendorClassIDMode", StrW, NoLimite, &VendorClassIDMode, NULL},
	{"ClientID", StrW, NoLimite, &ClientID, NULL},
	{"ClientIDExclude", BooleanW, NoLimite, &ClientIDExclude, NULL},
	{"UserClassID", StrW, NoLimite, &UserClassID, NULL},
	{"UserClassIDExclude", BooleanW, NoLimite, &UserClassIDExclude, NULL},
	{"Chaddr", StrW, NoLimite, &Chaddr, NULL},
	{"ChaddrMask", StrW, NoLimite, &ChaddrMask, NULL},
	{"ChaddrExclude", BooleanW, NoLimite, &ChaddrExclude, NULL},
	{"LocallyServed", BooleanW, NoLimite, &LocallyServed, NULL},
	{"MinAddress", StrW, NoLimite, &MinAddress, NULL},
	{"MaxAddress", StrW, NoLimite, &MaxAddress, NULL},
	{"ReservedAddresses", StrW, NoLimite, &ReservedAddresses, NULL},
	{"SubnetMask", StrW, NoLimite, &dhcpConSer_Mask, NULL},
	{"DNSServers", StrW, NoLimite, &dhcpConSer_DNS, NULL},
	{"DomainName", StrW, NoLimite, &DomainName, NULL},
	{"IPRouters", StrW, NoLimite, &IPRouters, NULL},
	{"DHCPLeaseTime", IntW, NoLimite, &DHCPLeaseTime, NULL},
	{"UseAllocatedWAN", StrW, NoLimite, &UseAllocatedWAN, NULL},
	{"AssociatedConnection", StrW, NoLimite, &AssociatedConnection, NULL},
	{"DHCPServerIPAddress", StrW, NoLimite, &DHCPServerIPAddress, NULL},
	{"DHCPStaticAddressNumberOfEntries", UnsignIntR, NoLimite, &DHCPStaticAddressNum, NULL},
	{"DHCPOptionNumberOfEntries", UnsignIntR, NoLimite, &dhcpConSer_OptionNum, NULL},
	{"DHCPOption", ObjectR, NoLimite, &dhcpConSer_OptionFuncs, NULL},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPConditionalServingPool.{i}.DHCPOption.{i}.
	Parent Node Table Location: cp_landev_dhcpOption_nodetable.c
*/

CwmpParameterStruct DHCPConSerOption[]= 
{
	{"Enable", BooleanW, NoLimite, &conSerOption_Enable, NULL},
	{"Tag", UnsignIntW, NoLimite, &conSerOption_Tag, NULL},
	{"Value", Base64W, NoLimite, &conSerOption_Value, NULL},
	{NULL, -1, -1, NULL, NULL}
};

/*
      This Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPConditionalServingPool.{i}.
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPConditionalServingPool.
	Parent Node Table Location: cp_landev_nodetable.c
*/

CwmpParameterStruct DHCPConSerPoolIndex[]= 
{
	{"1", ObjectR, NoLimite, NULL, DHCPConSerPool},
	{NULL, -1, -1, NULL, NULL}
};

/*
      This Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPConditionalServingPool.{i}.DHCPOption.{i}.
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPConditionalServingPool.{i}.DHCPOption.
	Parent Node Table Location: cp_landev_dhcpOption_nodetable.c
*/

CwmpParameterStruct DHCPConSerOptionIndex[]= 
{
	{"1", ObjectR, NoLimite, NULL, DHCPConSerOption},
	{NULL, -1, -1, NULL, NULL}
};

