#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <netinet/in.h>
#include <errno.h>
#include <libapi_lib_flowmgr.h>
#include "api_flow_cmd.h"

static help_info_t ghelp_action_info[] = {
    {"ACTION_BIT_MAP",               "ACTION_BIT_MAP INFOMATION"},
	{"E_ACL_ACTION",	                     "E_ACL_ACTION BIT0"},
	{"E_QUEUE_MAPPING_ACTION",     "E_QUEUE_MAPPING_ACTION BIT1"},
	{"E_PRIORITY_REMARK_ACTION", "E_PRIORITY_REMARK_ACTION BIT2"},
	{"E_LIMIT_ACTION",                     "E_LIMIT_ACTION BIT3"},
	{"E_TOS_REMARK_ACTION",           "E_TOS_REMARK_ACTION BIT4"},
};

static help_info_t ghelp_fld_info[] = {
	{"FLD_BIT_MAP",                            "FLD_BIT_MAP INFOMATION"},
	{"ECN_FLD_SRC_PORT",                        "ECN_FLD_SRC_PORT BIT0"},
	{"ECN_FLD_SA_MAC",                            "ECN_FLD_SA_MAC BIT1"},
	{"ECN_FLD_DA_MAC",                            "ECN_FLD_DA_MAC BIT2"},
	{"ECN_FLD_SA_IP",                              "ECN_FLD_SA_IP BIT3"},
	{"ECN_FLD_DA_IP",                              "ECN_FLD_DA_IP BIT4"},
	{"ECN_FLD_VLAN_ID",                          "ECN_FLD_VLAN_ID BIT5"},
	{"ECN_FLD_L2_ETH_TYPE",                  "ECN_FLD_L2_ETH_TYPE BIT6"},
	{"ECN_FLD_IP_PROTOCOL_TYPE",        "ECN_FLD_IP_PROTOCOL_TYPE BIT7"},
	{"ECN_FLD_VLAN_PRIO",                      "ECN_FLD_VLAN_PRIO BIT8"},
	{"ECN_FLD_IPV4_TRAFFIC_CLASS",    "ECN_FLD_IPV4_TRAFFIC_CLASS BIT9"},
	{"ECN_FLD_L4_SA_PORT",                   "ECN_FLD_L4_SA_PORT BIT10"},
	{"ECN_FLD_L4_DA_PORT",                   "ECN_FLD_L4_DA_PORT BIT11"},
	{"ECN_FLD_TTL",                                 "ECN_FLD_TTL BIT12"},
	{"ECN_FLD_IPV6_DSCP",                     "ECN_FLD_IPV6_DSCP BIT13"},
	{"ECN_FLD_IP_VER",                           "ECN_FLD_IP_VER BIT14"},
	{"ECN_FLD_IPV6_FLOW_LABEL",         "ECN_FLD_IPV6_FLOW_LABEL BIT15"},
	{"ECN_FLD_DA_IPV6",                         "ECN_FLD_DA_IPV6 BIT16"},
	{"ECN_FLD_SA_IPV6",                         "ECN_FLD_SA_IPV6 BIT17"},
	{"ECN_FLD_DA_IPV6_HEAD",               "ECN_FLD_DA_IPV6_HEAD BIT18"},
	{"ECN_FLD_SA_IPV6_HEAD",               "ECN_FLD_SA_IPV6_HEAD BIT19"},
	{"ECN_FLD_IPV6_NEXT_HEADER",       "ECN_FLD_IPV6_NEXT_HEADER BIT20"},
	{"ECN_FLD_IPV6_TRAFFIC_CLASS",   "ECN_FLD_IPV6_TRAFFIC_CLASS BIT21"},
	{"ECN_FLD_IPV6_HOP_LIMIT",           "ECN_FLD_IPV6_HOP_LIMIT BIT22"},
	{"ECN_FLD_IPV6_L4_SOURCE_PORT", "ECN_FLD_IPV6_L4_SOURCE_PORT BIT23"},
	{"ECN_FLD_IPV6_L4_DEST_PORT",     "ECN_FLD_IPV6_L4_DEST_PORT BIT24"},
};

static unsigned char a2x(const char c){    
	switch(c)     {        
		case '0'...'9':           
			return (c-'0');       
		case 'a'...'f':          
			return 0xa + (c-'a');        
		case 'A'...'F':           
			return 0xa + (c-'A');       
		default:            
			break;   
	}       
	return 0;
}

static PARSE_RET add_fld(ecn_traffic_classify_info_t *info, char *val, int index) {

	int                     i = 0, j = 0;
	int                         len = 0;
	unsigned long                utmp32;
	PARSE_RET parse_ret = PARSE_SUCCESS;
	unsigned char     mac_str[64] = {0};

	switch(index) {
		case ECN_FLD_SA_MAC:
			sscanf(val, "%s", mac_str);	
			for(i=0; i<ETH_ALEN; i++)	{		
				info->fld_element.src_mac[i] = (((a2x(mac_str[i*3]) << 4) | a2x(mac_str[i*3 + 1])) & 0xFF);	
			}
			break;
			
		case ECN_FLD_DA_MAC: 
			sscanf(val, "%s", mac_str);	
			for(i=0; i<ETH_ALEN; i++)	{		
				info->fld_element.dst_mac[i] = (((a2x(mac_str[i*3]) << 4) | a2x(mac_str[i*3 + 1])) & 0xFF);	
			}
			break;
			
		case ECN_FLD_SA_IP:
			 info->fld_element.ipv4_src_ip.s_addr = inet_addr(val);
			break;
			
		case ECN_FLD_DA_IP:
			 info->fld_element.ipv4_dst_ip.s_addr = inet_addr(val);
			break;
			
		case ECN_FLD_VLAN_ID: 
			if(1 != sscanf(val, "%x", &utmp32)) {
				parse_ret = PARSE_FAIL;
			}else {
				info->fld_element.vid = utmp32 & 0xffff;
			}
			break;
			
		case ECN_FLD_L2_ETH_TYPE: 
			if(1 != sscanf(val, "%x", &utmp32)) {
				parse_ret = PARSE_FAIL;
			}else {
				info->fld_element.ether_type = utmp32 & 0xffff;
			}
			break;
			 
		case ECN_FLD_IP_PROTOCOL_TYPE:
			if(1 != sscanf(val, "%x", &utmp32)) {
				parse_ret = PARSE_FAIL;
			}else {
				info->fld_element.ip_type = utmp32 & 0xff;
			}
			break;
			 
		case ECN_FLD_VLAN_PRIO:
			if(1 != sscanf(val, "%x", &utmp32)) {
				parse_ret = PARSE_FAIL;
			}else {
				info->fld_element.pbit = utmp32 & 0xff;
			}
			break;
			 
		case ECN_FLD_IPV4_TRAFFIC_CLASS:
			if(1 != sscanf(val, "%x", &utmp32)) {
				parse_ret = PARSE_FAIL;
			}else {
				info->fld_element.tos = utmp32 & 0xff;
			}
			break;
			 
		case ECN_FLD_L4_SA_PORT:
			if(1 != sscanf(val, "%x", &utmp32)) {
				parse_ret = PARSE_FAIL;
			}else {
				info->fld_element.l4_ipv4_src_port = utmp32;
			}
			break;
			 
		case ECN_FLD_L4_DA_PORT:
			if(1 != sscanf(val, "%x", &utmp32)) {
				parse_ret = PARSE_FAIL;
			}else {
				info->fld_element.l4_ipv4_dst_port = utmp32;
			}
			break;
			 
		case ECN_FLD_TTL: 
			if(1 != sscanf(val, "%x", &utmp32)) {
				parse_ret = PARSE_FAIL;
			}else {
				info->fld_element.ipv4_ttl = utmp32 & 0xff;
			}
			break;
			 
		case ECN_FLD_IPV6_DSCP:
			if(1 != sscanf(val, "%x", &utmp32)) {
				parse_ret = PARSE_FAIL;
			}else {
				info->fld_element.ipv6_dscp = utmp32 & 0xff;
			}
			break;
			 
	    case ECN_FLD_IP_VER:
			if(1 != sscanf(val, "%x", &utmp32)) {
				parse_ret = PARSE_FAIL;
			}else {
				info->fld_element.ip_version = utmp32 & 0xff;
			}
			break;
			 
		case ECN_FLD_IPV6_FLOW_LABEL:
			/*
			sscanf(val, "%s", mac_str);	
			for(i=0; i<FLOW_LBL_LEN; i++)	{		
				info->fld_element.flow_lbl[i] = (((a2x(mac_str[i*2]) << 4) | a2x(mac_str[i*2 + 1])) & 0xFF);	
			}
			break;
			*/
			utmp32 = strtol(val, NULL, 16);
			if(utmp32 == 0 && errno == EINVAL) {
				parse_ret = PARSE_FAIL;
			}
			else {
				for(i=0; i<FLOW_LBL_LEN; i++)	{		
					info->fld_element.flow_lbl[FLOW_LBL_LEN-i-1] = (utmp32>>i*8)&0xff;	
				}
			}
			break;
			
		case ECN_FLD_DA_IPV6:
			sscanf(val, "%s", mac_str);	
			for(i=0, j=0; i<IPV6_ADDR_LEN; i+=2, j++)	{		
				info->fld_element.ipv6_daddr.s6_addr[i]       = (((a2x(mac_str[j*5]) << 4) | a2x(mac_str[j*5 + 1])) & 0xFF);	
				info->fld_element.ipv6_daddr.s6_addr[i+1] = (((a2x(mac_str[j*5 + 2]) << 4) | a2x(mac_str[j*5 + 3])) & 0xFF);	
			}
			break;
			
		case ECN_FLD_SA_IPV6:	
			sscanf(val, "%s", mac_str);	
			for(i=0, j=0; i<IPV6_ADDR_LEN; i+=2, j++)	{		
				info->fld_element.ipv6_saddr.s6_addr[i]       = (((a2x(mac_str[j*5]) << 4) | a2x(mac_str[j*5 + 1])) & 0xFF);	
				info->fld_element.ipv6_saddr.s6_addr[i+1] = (((a2x(mac_str[j*5 + 2]) << 4) | a2x(mac_str[j*5 + 3])) & 0xFF);	
			}
			break;
			
		case ECN_FLD_DA_IPV6_HEAD:
			sscanf(val, "%s", mac_str);	
			for(i=0, j=0; i<IPV6_ADDR_LEN; i+=2, j++)	{		
				info->fld_element.ipv6_daddr_hdr.s6_addr[i]       = (((a2x(mac_str[j*5]) << 4) | a2x(mac_str[j*5 + 1])) & 0xFF);	
				info->fld_element.ipv6_daddr_hdr.s6_addr[i+1] = (((a2x(mac_str[j*5 + 2]) << 4) | a2x(mac_str[j*5 + 3])) & 0xFF);	
			}
			break;
			
		case ECN_FLD_SA_IPV6_HEAD:
			sscanf(val, "%s", mac_str);	
			for(i=0, j=0; i<IPV6_ADDR_LEN; i+=2, j++)	{		
				info->fld_element.ipv6_saddr_hdr.s6_addr[i]       = (((a2x(mac_str[j*5]) << 4) | a2x(mac_str[j*5 + 1])) & 0xFF);	
				info->fld_element.ipv6_saddr_hdr.s6_addr[i+1] = (((a2x(mac_str[j*5 + 2]) << 4) | a2x(mac_str[j*5 + 3])) & 0xFF);	
			}
			break;
			
	    case ECN_FLD_IPV6_NEXT_HEADER:	
			if(1 != sscanf(val, "%x", &utmp32)) {
				parse_ret = PARSE_FAIL;
			}else {
				info->fld_element.nexthdr = utmp32 & 0xff;
			}
			break;
			
		case ECN_FLD_IPV6_TRAFFIC_CLASS:
			if(1 != sscanf(val, "%x", &utmp32)) {
				parse_ret = PARSE_FAIL;
			}else {
				info->fld_element.ipv6_traffic_class = utmp32 & 0xff;
			}
			break;
			
		case ECN_FLD_IPV6_HOP_LIMIT:
			if(1 != sscanf(val, "%x", &utmp32)) {
				parse_ret = PARSE_FAIL;
			}else {
				info->fld_element.hop_limit = utmp32 & 0xff;
			}
			break;
			
		case ECN_FLD_IPV6_L4_SOURCE_PORT:
			if(1 != sscanf(val, "%x", &utmp32)) {
				parse_ret = PARSE_FAIL;
			}else {
				info->fld_element.l4_ipv6_src_port = utmp32;
			}
			break;
			
		case ECN_FLD_IPV6_L4_DEST_PORT:	
			if(1 != sscanf(val, "%x", &utmp32)) {
				parse_ret = PARSE_FAIL;
			}else {
				info->fld_element.l4_ipv6_dst_port = utmp32;
			}
			break;
			
		case ECN_FLD_SRC_PORT:  
			break;
		case ECN_FLD_PHY_DEST_PORT:      
		case ECN_FLD_DHCPV6_MESSAGE_TYPE: 
		default:
			parse_ret = PARSE_FAIL;
	}
	return parse_ret;
}

 PARSE_RET parse_cmd(char **arg, ecn_traffic_classify_info_t *info) {
	int 				   tmpval = 0;
	int                       tmp = 0;
	int                         i = 0;
	int          array[MAX_LEN] = {0};
	unsigned int mark = 0, remark = 0;
	PARSE_RET               parse_ret;

	/*parse port*/
	if(arg && *arg && (1 == sscanf(*(arg++), "%x", &tmpval))) {
		info->port = tmpval & 0xFF;;
	}
	else {
		return PARSE_FAIL;
	}

	
	/*parse action*/
	if(arg && *arg && (1 == sscanf(*(arg++), "%x", &tmpval))) {
		if(tmpval & E_ACL_ACTION) {
			info->act_element.action_bit_map |= E_ACL_ACTION;

			/*parse wb_flags*/
			if(arg && *arg && (1 == sscanf(*(arg++), "%x", &tmp))) {
				info->act_element.acl_mode = !!tmp;
			}
			else {
				return PARSE_FAIL;
			}
		}
		if(tmpval & E_QUEUE_MAPPING_ACTION) {
			info->act_element.action_bit_map |= E_QUEUE_MAPPING_ACTION;

			/*parse remark_queue*/
			if(arg && *arg && (1 == sscanf(*(arg++), "%x", &tmp))) {
				info->act_element.remark_queue = tmp & 0xFF;
			}
			else {
				return PARSE_FAIL;
			}
		}
		if(tmpval & E_PRIORITY_REMARK_ACTION) {
			info->act_element.action_bit_map |= E_PRIORITY_REMARK_ACTION;

			/*parse remark_pbit*/
			if(arg && *arg && (1 == sscanf(*(arg++), "%x", &tmp))) {
				info->act_element.remark_pbit= tmp & 0xFF;
			}
			else {
				return PARSE_FAIL;
			}
		}
		if(tmpval & E_LIMIT_ACTION) {
			info->act_element.action_bit_map |= E_LIMIT_ACTION;

			/*parse limit_group_id*/
			if(arg && *arg && (1 == sscanf(*(arg++), "%x", &tmp))) {
				info->act_element.limit_group_id = tmp;
			}
			else {
				return PARSE_FAIL;
			}
		}
		if(tmpval & E_TOS_REMARK_ACTION) {
			info->act_element.action_bit_map |= E_TOS_REMARK_ACTION;
	
			/*parse remark_tos*/
			if(arg && *arg && (1 == sscanf(*(arg++), "%x", &tmp))) {
				info->act_element.remark_tos = tmp;
			}
			else {
				return PARSE_FAIL;
			}
		}
	}
	else {
		return PARSE_FAIL;
	}
	
	/*parse fld_map*/
	if(arg && *arg && (1 == sscanf(*(arg++), "%x", &tmpval))) {
		info->fld_element.fld_bit_map = tmpval;
	}
	else {
		return PARSE_FAIL;
	}

	mark = info->fld_element.fld_bit_map;
	do {
		remark = mark % 2;
		mark /= 2;
		array[i] = remark;
		i++;
	}while((mark!=0) && i<MAX_LEN);

	for(i=0; i<MAX_LEN; i++) {
		if(array[i] != 0) {
			if(!(arg && *arg && (add_fld(info, *(arg++), 1<<i) == PARSE_SUCCESS))) {
				return PARSE_FAIL;
			}
		}
	}

	return PARSE_SUCCESS;
}

static API_RET add_rule(char **arg, ecn_traffic_classify_info_t *info) {

	int          res = 0;
	PARSE_RET  parse_ret;

	parse_ret = parse_cmd(arg, info);
	if(parse_ret == PARSE_FAIL) {
		printf("parse_pre_cmd fail\n");
		return API_FAIL;
	}
	
	res = flowmgr_lib_add(info);
	if(res == 0) {
		return API_SUCCESS;
	}
	
	return API_FAIL;
}



static int del_rule(char **arg, ecn_traffic_classify_info_t *info) {
	int          res = 0;
	PARSE_RET  parse_ret;

	parse_ret = parse_cmd(arg, info);
	if(parse_ret == PARSE_FAIL) {
		printf("parse_pre_cmd fail\n");
		return API_FAIL;
	}
	
	res = flowmgr_lib_del(info);
	if(res == 0) {
		return API_SUCCESS;
	}
	return API_FAIL;

}

static int clean_rule(char **arg, ecn_traffic_classify_info_t *info) {
	int           res = 0;
	int 	   tmpval = 0;
	PARSE_RET   parse_ret;

	/*parse port*/
	if(arg && *arg && (1 == sscanf(*(arg++), "%x", &tmpval))) {
		info->port = tmpval & 0xFF;;
	}
	else {
		return API_FAIL;
	}

	
	/*parse action*/
	if(arg && *arg && (1 == sscanf(*(arg++), "%x", &tmpval))) {
		info->act_element.action_bit_map = tmpval;
	}
	else {
		return API_FAIL;
	}

	res = flowmgr_lib_clean(info);
	if(res == 0) {
		return API_SUCCESS;
	}

	return API_FAIL;
}


action_info_t action_cmd[] = {
		{"add",     add_rule},
		{"del",     del_rule},
		{"clean", clean_rule},
		{NULL, NULL}
};

int main(int argc, char **argv) {

	int                              i = 0;
	API_RET                            res;
	ecn_traffic_classify_info_t info = {0};

	if(argc < 3) {
		printf("Usage:\n");
		printf("\t%s [add|del] port  action [wb_flags|remark_pbit|remark_queue|limit_group_id|remark_tos] field_map field_value...\n", argv[0]);
		printf("\n");
		for(i=0; i<sizeof(ghelp_action_info)/sizeof(ghelp_action_info[0]); i++) {
			printf("%s\n", ghelp_action_info[i].dat);
		}
		printf("\n");
		for(i=0; i<sizeof(ghelp_fld_info)/sizeof(ghelp_fld_info[0]); i++) {
			printf("%s\n", ghelp_fld_info[i].dat);
		}
		printf("\n***************ALL INPUT HEX******************\n");
		return -1;
	}

	for(i=0; action_cmd[i].name!=NULL; i++) {
		if(!strcmp(action_cmd[i].name, argv[1])) {
			argv += 2;
			res = action_cmd[i].func(argv, &info);
			if(res == API_SUCCESS) {
				printf("%s cmd success!\n", argv[-1]);
			}else {
				printf("%s cmd fail!\n", argv[-1]);
			}
			break;
		}
	}
}
