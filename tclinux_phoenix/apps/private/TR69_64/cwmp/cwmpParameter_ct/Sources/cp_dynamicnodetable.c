#include "Global_res.h"
#include "cp_dynamicnodetable.h"

CwmpDynamicNodeStruct DynamicNode_table[] =
{
#if 0
		{"InternetGatewayDevice.", DeviceInfo_D, Static},
		{"InternetGatewayDevice.", DeviceInfo_D, Dynamic},
		{"InternetGatewayDevice.LANDevice.", DeviceInfo_D, Static},
		{"InternetGatewayDevice.LANDevice.", DeviceInfo_D, Dynamic},
#endif
		/*You can Add Dynamic node before*/
		{NULL, NULL, -1},
};
