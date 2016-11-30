#ifndef _QOS_TYPE_H_
#define _QOS_TYPE_H_

#define	 MAX_BUF_LEN				64
#define  MAX_RULE_NUM				16

#define  EBT_CHK_TYPE				1
#define  DEV_XMIT_CHK_TYPE			2


#define QOSTYPE_IOC_MAGIC 			'g'
#define QOSTYPE_MAJOR 				111               
#define QOSTYPE_IOC_SET_TYPE _IOR(QOSTYPE_IOC_MAGIC,10,unsigned long)

#if defined(TCSUPPORT_CT_DS_LIMIT)
#define QOSDSLIMIT_DEV_NAME				"qos_dslimit"
#define QOSDSLIMIT_IOC_MAGIC 			'g'
#define QOSDSLIMIT_MAJOR 				112   
#define QOSDSLIMIT_IOC_MODECFG_UP _IOR(QOSDSLIMIT_IOC_MAGIC, 0, unsigned long)
#define QOSDSLIMIT_IOC_DATACFG_UP _IOR(QOSDSLIMIT_IOC_MAGIC, 1, unsigned long)
#define QOSDSLIMIT_IOC_MODECFG_DW _IOR(QOSDSLIMIT_IOC_MAGIC, 2, unsigned long)
#define QOSDSLIMIT_IOC_DATACFG_DW _IOR(QOSDSLIMIT_IOC_MAGIC, 3, unsigned long)
#define QOSDSLIMIT_IOC_QUEUECFG_DW _IOR(QOSDSLIMIT_IOC_MAGIC, 4, unsigned long)

#define DSLIMIT_MODE_DISABLE 0
#define DSLIMIT_MODE_IFACE 1
#define DSLIMIT_MODE_VLANID 2
#define DSLIMIT_MODE_IPRANGE 3
#define DSLIMIT_MAX_QUEUE_NUM 6
#define DSLIMIT_UP 1
#define DSLIMIT_DW 2

struct dsnode
{
#define DSLIMIT_MAX_NODES 6
	union
	{
		 /* only use pair[0] for LAN interface.
		1-4: LAN1-LAN4, 5-8: SSID1-SSID4
		 */
		unsigned char ds_lan_pair[16];

		/* use for IPv6 address
		    #NOT SUPPORT v6 address range mode#
		*/
		unsigned char ds_v6_pair[16];

#define DS_VLANID_UNTAG 4096
		/* only use pair[0] for VLANID */
		unsigned short ds_vlan_pair[8];

		/* only use pair[0] & pair[1] for IP & IP Range */
		unsigned int ds_v4_pair[4];
	} ds_pair;
	unsigned char ds_used; /* ds node used; 0 : un-used, 1 : used */
	unsigned char ds_queue; /* pair queue index */
#define DS_IPTYPE_IPV4 1
#define DS_IPTYPE_IPV6 2
	unsigned char ds_iptype; /* ds node iptype; 1: ipv4, 2: ipv6 */
	unsigned char reserved[1];
#define DS_QUEUE_PER_SPEED 512
	int     ds_queue_speed; /* pair queue speed */
#define ds_lanif ds_pair.ds_lan_pair
#define ds_vlanid ds_pair.ds_vlan_pair
#define ds_ip4addr ds_pair.ds_v4_pair
#define ds_ip6addr ds_pair.ds_v6_pair
};
#endif

/* type_name: wan_if, rtp_proto */
struct qos_match
{
	char type_name[MAX_BUF_LEN];
	char type_value[MAX_BUF_LEN];
};

/* qm[0]: wan_if, qm[1]: rtp_proto */
struct qos_action
{
	struct qos_match qm[2];
	/* if match, set dscp here */
	int dscp_flag;
	unsigned int dscp;
};

struct qos_type
{
	int rule_no;
	struct qos_match qm[2];
};

int qostype_chk(int chkType, int rule_no, char *wan_if, int rtp_match);
int set_tos(int rule_no, unsigned int tos);
int get_tos(int rule_no, unsigned int *tos);
int unset_tos(int rule_no);
#if defined(TCSUPPORT_CT_QOS)
int qostype_wan_check(int rule_no, struct net_device *dev);
#endif


#endif

