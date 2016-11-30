#ifndef __FOE_HOOK_H
#define __FOE_HOOK_H

#define FOE_MAGIC_PCI		    0x7273
#define FOE_MAGIC_WLAN		    0x7274
#define FOE_MAGIC_GE		    0x7275
#define FOE_MAGIC_PPE		    0x7276
#define FOE_MAGIC_ATM		    0x7277

#ifdef TCSUPPORT_MT7510_FE
#define FOE_MAGIC_PTM		    0x7278
#define FOE_MAGIC_EPON		    0x7279
#define FOE_MAGIC_GPON		    0x727a
//#define FOE_MAGIC_CRYPTO	    0x727b

#define FOE_MAGIC_CRYPTO_E_1	    0x727b
#define FOE_MAGIC_CRYPTO_D_1	    0x727c
#define FOE_MAGIC_CRYPTO_E_2	    0x727d
#define FOE_MAGIC_CRYPTO_D_2	    0x727e
#define FOE_MAGIC_OFFLOAD	    0x727f
#define FOE_MAGIC_PTM_LAN		    0x7280
#endif

#define	FOE_MAGIC_L2TP_VPN_UPSTREAM	0x7281
#define	FOE_MAGIC_L2TP_VPN_DOWNSTREAM	0x7282

#define	L2TP_VPN_PPP_NAME	"ppp100"
#define	HWNAT_PKT_ERROR		0
#define	HWNAT_PKT_UPSTREAM	1
#define	HWNAT_PKT_DOWNSTREAM	2

#define HWNAT_IPSEC_LEARING 0
#define HWNAT_IPSEC_SPEED 1
#define HWNAT_IPSEC_ROLLBACK 2

#define IPSEC_SKB_CB			47

#define FOE_OPE_GETENTRYNUM 0
#define FOE_OPE_CLEARENTRY  1


#define HWNAT_WLAN_IF_MAXNUM	16 //16 is max wifi interface
#define HWNAT_WLAN_IF_BASE		8 //8 is base
#define HWNAT_WLAN_IF_NUM		4 //8 is maximum
#define HWNAT_WLAN_IF_I_NUM		4 //8 is maximum, foe rai

#define MULTICAST_SPEED_STATE_I 		3	//with lan and with wlan
#define MULTICAST_SPEED_STATE_II		1	//with lan and without wlan
#define MULTICAST_SPEED_STATE_III		2	//without lan and with wlan
#define MULTICAST_SPEED_STATE_IV   		0	//without lan and without wlan

struct port_info {
    unsigned long int txq:4;
    unsigned long int channel:8;
    unsigned long int tsid:8;
    unsigned long int atm_pppoa:1;
    unsigned long int atm_ipoa:1;
    unsigned long int atm_vc_mux:1;
    unsigned long int eth_macSTagEn:1;
	unsigned long int eth_is_wan:1;
    unsigned long int ds_to_qdma:1;
    unsigned long int ds_need_offload:1;
    unsigned long int txq_is_valid:1;
    unsigned long int resv0:4;
    unsigned long int stag:16;
    unsigned long int magic:16;
};

typedef union {
	struct {
#ifdef __BIG_ENDIAN
		unsigned int resv				: 1 ;
		unsigned int tsid				: 5 ;
		unsigned int tse				: 1 ;
		unsigned int dei				: 1 ;
		unsigned int gem				: 12 ;
		unsigned int oam				: 1 ;
		unsigned int channel			: 8 ;
		unsigned int queue				: 3 ;	
#else
		unsigned int queue				: 3 ;	
		unsigned int channel			: 8 ;
		unsigned int oam				: 1 ;
		unsigned int gem				: 12 ;
		unsigned int dei				: 1 ;
		unsigned int tse				: 1 ;
		unsigned int tsid				: 5 ;
		unsigned int resv				: 1 ;
#endif /* __BIG_ENDIAN */

#ifdef __BIG_ENDIAN
		unsigned int ico				: 1 ;
		unsigned int uco				: 1 ;
		unsigned int tco				: 1 ;
		unsigned int tso				: 1 ;
		unsigned int pmap				: 6 ;
		unsigned int fport				: 3 ;
		unsigned int insv				: 1 ;
		unsigned int tpid				: 2 ;	
		unsigned int vid				: 16 ;	
#else
		unsigned int vid				: 16 ;	
		unsigned int tpid				: 2 ;	
		unsigned int insv				: 1 ;
		unsigned int fport				: 3 ;
		unsigned int pmap				: 6 ;
		unsigned int tso				: 1 ;
		unsigned int tco				: 1 ;
		unsigned int uco				: 1 ;
		unsigned int ico				: 1 ;
#endif /* __BIG_ENDIAN */
	} raw ;

	struct {
#ifdef __BIG_ENDIAN
		unsigned int mtr				: 1 ;
		unsigned int fport_ppe			: 3 ;
		unsigned int gem				: 16 ;
		unsigned int oam				: 1 ;
		unsigned int channel_ppe		: 5 ;
		unsigned int channel			:3 ;
		unsigned int queue				: 3 ;	
#else
		unsigned int queue				: 3 ;	
		unsigned int channel			:3 ;
		unsigned int channel_ppe		: 8 ;
		unsigned int oam				: 1 ;
		unsigned int gem				: 16 ;
		unsigned int fport_ppe			: 3 ;
		unsigned int mtr				: 1 ;
#endif /* __BIG_ENDIAN */

#ifdef __BIG_ENDIAN
		unsigned int ico				: 1 ;
		unsigned int uco				: 1 ;
		unsigned int tco				: 1 ;
		unsigned int tso				: 1 ;
		unsigned int mtr_index			: 6 ;
		unsigned int fport				: 3 ;
		unsigned int insv				: 1 ;
		unsigned int tpid				: 2 ;	
		unsigned int vid				: 16 ;	
#else
		unsigned int vid				: 16 ;	
		unsigned int tpid				: 2 ;	
		unsigned int insv				: 1 ;
		unsigned int fport				: 3 ;
		unsigned int mtr_index			: 6 ;
		unsigned int tso				: 1 ;
		unsigned int tco				: 1 ;
		unsigned int uco				: 1 ;
		unsigned int ico				: 1 ;
#endif /* __BIG_ENDIAN */
	} raw1 ;/*tx msg format1 for cpu path ratelimit by ppe meter in EN7526C*/
	
	unsigned int word[2] ;
}FETxMsg_T ;


/*****************************
 * FRAME ENGINE REGISTERS OFFSET *
 *****************************/
#define FE_GLO_CFG_OFF          (0x0000)
#define CDMP_VLAN_CT_OFF		(0x0400)
#define CDM_VLAN_GE_OFF         (0x1400)
#define GDM2_FWD_CFG_OFF		(0x1500)
#define GDM2_MIB_CLR_OFF		(0x1520)
#define GDM2_LEN_CFG_OFF		(0x1524)
#define GDM2_CHN_EN_OFF			(0x152c)
#define GDM2_TX_GET_CNT_OFF		(0x1600)
#define GDM2_TX_OK_CNT_OFF		(0x1604)
#define GDM2_TX_DROP_CNT_OFF	(0x1608)
#define GDM2_TX_OK_BYTE_CNT_OFF	(0x160c)
#define GDM2_RX_OK_CNT_OFF		(0x1650)
#define GDM2_RX_OVER_DROP_CNT_OFF	(0x1654)
#define GDM2_RX_ERROR_DROP_CNT_OFF	(0x1658)
#define GDM2_RX_OK_BYTE_CNT_OFF		(0x165c)
#define GDM2_RX_ETH_CRCE_CNT_OFF	(0x1674)
#define GDM2_RX_ETH_RUNT_CNT_OFF	(0x1680)
#define GDM2_RX_ETH_LONG_CNT_OFF	(0x1684)

	
struct SkbFoeInfo{	
	unsigned short ppe_magic;	
	unsigned short ppe_foe_entry;
	unsigned char ppe_ai;
};

struct sk_buff;
struct net_device;

#ifdef TCSUPPORT_HWNAT_L2VID
extern int (*ra_sw_nat_hook_rx_set_l2lu) (struct sk_buff * skb);
#endif
extern int (*ra_sw_nat_hook_rx) (struct sk_buff * skb);
extern int (*ra_sw_nat_ds_offload) (struct sk_buff * skb, int *dp);
extern int (*ra_sw_nat_hook_update_dp)(int index, int dp);
extern int (*ra_sw_nat_hook_update_vlan)(int index,int outer_vlan,int inner_vlan);

extern int (*ra_sw_nat_hook_save_rxinfo)(struct sk_buff *skb);
extern int (*ra_sw_nat_hook_restore_rxinfo)(struct sk_buff *skb);
extern int (*ra_sw_nat_hook_save_txinfo)(struct sk_buff *skb);
extern int (*ra_sw_nat_hook_restore_txinfo)(struct sk_buff *skb);
extern int (*ra_sw_nat_hook_is_hwnat_pkt)(struct sk_buff *skb);
extern int (*ra_sw_nat_hook_sendto_ppe)(struct sk_buff *skb);
extern int (*ra_sw_nat_hook_set_l2tp_dev)(struct net_device *dev);
extern struct net_device* (*ra_sw_nat_hook_read_l2tp_dev)(void);
extern int (*ra_sw_nat_rtsp_offload_restore) (struct sk_buff * skb);
extern int (*ra_sw_nat_rtsp_data_handle) (struct sk_buff * skb, char *rb_ptr, unsigned int datalen);

#ifdef TCSUPPORT_MT7510_FE
extern int (*ra_sw_nat_hook_tx) (struct sk_buff * skb, struct port_info * pinfo, int magic);
#else
extern int (*ra_sw_nat_hook_tx) (struct sk_buff * skb, int gmac_no);
#endif
extern int (*ra_sw_nat_hook_free) (struct sk_buff * skb);
extern int (*ra_sw_nat_hook_rxinfo) (struct sk_buff * skb, int magic, char *data, int data_length);
extern int (*ra_sw_nat_hook_txq) (struct sk_buff * skb, int txq);
extern int (*ra_sw_nat_hook_magic) (struct sk_buff * skb, int magic);
extern int (*ra_sw_nat_hook_set_magic) (struct sk_buff * skb, int magic);
extern int (*ra_sw_nat_hook_xfer) (struct sk_buff *skb, const struct sk_buff *prev_p);
extern int (*ra_sw_nat_hook_foeentry) (void * inputvalue,int operation);
extern int (*ra_sw_nat_hook_is_alive_pkt)(unsigned int crsn);
#if defined(TCSUPPORT_CPU_MT7520)
extern int (*MT7530LanPortMap2Switch_hook)(int port); 
#endif

#ifdef TCSUPPORT_RA_HWNAT_ENHANCE_HOOK
extern int (*ra_sw_nat_hook_drop_packet) (struct sk_buff * skb);
extern int (*ra_sw_nat_hook_clean_table) (void);
#endif

#ifdef TCSUPPORT_MT7510_FE
extern void (*restore_offload_info_hook)(struct sk_buff *skb, struct port_info *pinfo, int magic);
#endif

extern int (*ra_sw_nat_hook_cpu_meter)(struct sk_buff* skb,FETxMsg_T* txMsg,struct port_info* pinfo,unsigned char dir,unsigned short mtrIndex,unsigned int rateScale);
#endif
