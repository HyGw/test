#ifndef __API_FLOW_CMD_H__
#define __API_FLOW_CMD_H__

#define MAX_LEN        32
#define ETH_ALEN       6 
#define FLOW_LBL_LEN   3
#define IPV6_ADDR_LEN  16

typedef enum {
	PARSE_FAIL    = -1,
	PARSE_SUCCESS = 0
}PARSE_RET;

typedef enum {
	API_FAIL   = -1,
	API_SUCCESS = 0
}API_RET;

typedef struct _action_info_ {
	
	char *name;
	int (*func)(char **argv, ecn_traffic_classify_info_t *info);
}action_info_t;

typedef struct _help_info_ {
	char *key;
	char *dat;
}help_info_t, *pt_help_info;

#endif
