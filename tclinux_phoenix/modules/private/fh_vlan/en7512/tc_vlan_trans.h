#ifndef TC_VLANF_TRANS_H_
#define TC_VLANF_TRANS_H_

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#include <linux/skbuff.h>

 
#define FH_MAX_VLAN_NUMBER 8
#define FH_RET_SUCCESS        0
#define FH_RET_FAILED         1

#define FH_RET_VLAN_REACH_MAX       10
#define FH_RET_VLAN_ALREADY_EXIST   11
#define FH_RET_VLAN_NOT_EXIST       12
#define FH_RET_VLAN_CNT_ZERO        13
#define FB_SPECIAL_VLAN         4093
#define setVID(x) 				((x) & 0x0FFF)
#define getVID(x)         		((x) & 0x0FFF)
#define VLAN_HEAD_LEN           4

#define COSNT_FH_VLAN_MAJOR_NUM		253
#define CONST_FH_VLAN_DEV_NAME		"fhvlan"

#define INTERFACE_MAGIC (0xCA)

#define FH_IF_IOS_VLAN_CONFIG_SET _IOW(INTERFACE_MAGIC, 0x01, struct FH_VLAN_CFG_OPERATE_S *)
#define FH_IF_IOG_VLAN_INFO_GET _IOW(INTERFACE_MAGIC, 0x02, struct FH_VLAN_CFG_INFO_S *)

struct FH_VLAN_INFO_S {
    unsigned short  srcVlan;
    unsigned short  dstVlan;
};

struct FH_VLAN_CFG_INFO_S {
	unsigned char vlanCount;  
	struct FH_VLAN_INFO_S vlan[FH_MAX_VLAN_NUMBER];
};

typedef struct {
	struct FH_VLAN_CFG_INFO_S fhVlan;
} FHVLAN_GlbPriv_T ;

#define FH_MAX_VLAN_OPT_ADD 0
#define FH_MAX_VLAN_OPT_DEL 1

struct FH_VLAN_CFG_OPERATE_S {
	unsigned char vlanOperate;  
	struct FH_VLAN_INFO_S vlan;
};

int vlan_transfer_init(void);
int vlan_transfer_exit(void);
int fb_ewan_rx_vlan_proc(struct sk_buff *skb);
int fb_ewan_tx_vlan_proc(struct sk_buff *skb);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

