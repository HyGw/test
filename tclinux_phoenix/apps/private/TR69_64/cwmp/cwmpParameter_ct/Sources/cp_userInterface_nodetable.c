#include "Global_res.h"
#include "cp_userInterface.h"

static NodeFunc WarrantyDate=
{
	NULL,
	NULL,
	getWarrantyDateValue,
	setWarrantyDateValue, 
	getWarrantyDateAttribute, 
	setWarrantyDateAttribute, 
	NULL,
};


/*
	Parent Node: InternetGatewayDevice.UserInterface.
	Parent Node Table Location: cp_topnodetable.c
*/
CwmpParameterStruct UserInterface[] = 
{
	{"WarrantyDate", DateTimeTypeW, NoLimite, &WarrantyDate, NULL},	

	{NULL, -1, -1, NULL, NULL}
};
