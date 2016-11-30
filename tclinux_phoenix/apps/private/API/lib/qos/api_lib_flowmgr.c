/*
	
*/
#include <ecnt_lib_dbg.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <netinet/in.h>
#include "libapi_lib_flowmgr.h"

#define CLASSIFY_IOC_MAGIC		        'e'
#define TRAFFIC_CLASSIFY_ADD_ENTRY  	_IOW(CLASSIFY_IOC_MAGIC,   0, unsigned long)
#define TRAFFIC_CLASSIFY_DEL_ENTRY  	_IOW(CLASSIFY_IOC_MAGIC,   1, unsigned long)
#define TRAFFIC_CLASSIFY_CLEAN  	    _IOW(CLASSIFY_IOC_MAGIC,   2, unsigned long)

#define ECNT_LIB_ACL_PRINT_ERROR(fmt, ...)  ECNT_LIB_DBG_PRINT(ECNT_DBG_ERROR,  ECNT_DBG_TYPE_ACL, fmt, ##__VA_ARGS__)
#define ECNT_LIB_ACL_PRINT_WARN(fmt, ...)   ECNT_LIB_DBG_PRINT(ECNT_DBG_WARN,   ECNT_DBG_TYPE_ACL, fmt, ##__VA_ARGS__)
#define ECNT_LIB_ACL_PRINT_NOTICE(fmt, ...) ECNT_LIB_DBG_PRINT(ECNT_DBG_NOTICE, ECNT_DBG_TYPE_ACL, fmt, ##__VA_ARGS__)
#define ECNT_LIB_ACL_PRINT_DEBUG(fmt, ...)  ECNT_LIB_DBG_PRINT(ECNT_DBG_DEBUG,  ECNT_DBG_TYPE_ACL, fmt, ##__VA_ARGS__)

#define DEV_NAME  "/dev/traffic_classify"

ECNT_LIB_FLOW_RET flowmgr_lib_ioctl(unsigned int cmd, ecn_traffic_classify_info_t *port_rule)
{
	int fd = -1;

	fd = open(DEV_NAME, O_RDWR);
	if (fd < 0)
	{
		ECNT_LIB_ACL_PRINT_ERROR("Couldn't open %s, fd = %d.\n", DEV_NAME, fd);
		return ECNT_E_FLOW_FAIL;
	}

	if (ioctl(fd, cmd, port_rule) < 0) 
	{
		ECNT_LIB_ACL_PRINT_ERROR("do flowmgr_lib_ioctl failed.Reason is %s\n",strerror(errno));
		close(fd);
		return ECNT_E_FLOW_FAIL;
	}

	close(fd);
	return ECNT_E_FLOW_SUCCESS; 

}


ECNT_LIB_FLOW_RET flowmgr_lib_add(ecn_traffic_classify_info_t *port_rule) {

	int      res = ECNT_E_FLOW_SUCCESS;
	unsigned char cmd[1024] = {0};
	
	assert(port_rule != NULL);
	memset(cmd, 0, sizeof(cmd));

	res = flowmgr_lib_ioctl(TRAFFIC_CLASSIFY_ADD_ENTRY, port_rule);
	if(res != ECNT_E_FLOW_SUCCESS) {
		return res;
	}
	
	strcpy(cmd,"/userfs/bin/hw_nat  -!");
	system(cmd);
	
	return res;
}

ECNT_LIB_FLOW_RET flowmgr_lib_del(ecn_traffic_classify_info_t *port_rule) {
	
	int      res = ECNT_E_FLOW_SUCCESS;
	unsigned char cmd[1024] = {0};

	assert(port_rule != NULL);

	res = flowmgr_lib_ioctl(TRAFFIC_CLASSIFY_DEL_ENTRY, port_rule);
	if(res != ECNT_E_FLOW_SUCCESS) {
		return res;
	}
	
	strcpy(cmd,"/userfs/bin/hw_nat  -!");
	system(cmd);

	return res;
}


ECNT_LIB_FLOW_RET flowmgr_lib_clean(ecn_traffic_classify_info_t *port_rule) {
	
	int      res = ECNT_E_FLOW_SUCCESS;
	unsigned char cmd[1024] = {0};

	assert(port_rule != NULL);

	res = flowmgr_lib_ioctl(TRAFFIC_CLASSIFY_CLEAN, port_rule);
	if(res != ECNT_E_FLOW_SUCCESS) {
		return res;
	}
	
	strcpy(cmd,"/userfs/bin/hw_nat  -!");
	system(cmd);

	return res;

}


