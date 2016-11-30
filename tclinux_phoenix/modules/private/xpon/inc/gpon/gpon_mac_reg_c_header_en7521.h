#ifndef __gpon_mac_reg_REGS_H__
#define __gpon_mac_reg_REGS_H__
#include <linux/mtd/rt_flash.h>
#include <common/drv_types.h>

#ifdef UINT32
#undef UINT32
#endif
#ifdef __cplusplus
extern "C" {
#endif

#ifndef REG_BASE_C_MODULE
// ----------------- gpon_mac_reg Bit Field Definitions -------------------

#define PLOAMU_CTRL			O3_O4_PLOAMU_CTRL
#define REG_PLOAMU_CTRL		REG_O3_O4_PLOAMU_CTRL
#define PREG_PLOAMU_CTRL	PREG_O3_O4_PLOAMU_CTRL

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_16                    : 16;
        FIELD onu_id_vld                : 1;
        FIELD rsv_8                     : 7;
        FIELD onu_id                    : 8;
#else
        FIELD onu_id                    : 8;
        FIELD rsv_8                     : 7;
        FIELD onu_id_vld                : 1;
        FIELD rsv_16                    : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_ONU_ID, *PREG_G_ONU_ID;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_17                    : 15;
        FIELD us_fec_en                 : 1;
        FIELD rsv_8                     : 8;
        FIELD sr_blk_size               : 8;
#else
        FIELD sr_blk_size               : 8;
        FIELD rsv_8                     : 8;
        FIELD us_fec_en                 : 1;
        FIELD rsv_17                    : 15;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_GBL_CFG, *PREG_G_GBL_CFG;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD bwm_us_fec_err_int        : 1;
        FIELD bwm_stop_time_err_int     : 1;
        FIELD lwi_int                   : 1;
        FIELD fwi_int                   : 1;
        FIELD pop_up_recv_in_O6_int     : 1;
        FIELD onu_us_fec_chg_int        : 1;
        FIELD olt_ds_fec_chg_int        : 1;
        FIELD o5_eqd_adj_done_int       : 1;
        FIELD sfifo_full_int            : 1;
        FIELD bfifo_full_int            : 1;
        FIELD rx_gem_intlv_err_int      : 1;
        FIELD rx_eof_err_int            : 1;
        FIELD tx_late_start_int         : 1;
        FIELD bst_sgl_diff_int          : 1;
        FIELD fifo_err_int              : 1;
        FIELD rx_err_int                : 1;
        FIELD rsv_12                    : 4;
        FIELD dying_gasp_send_int       : 1;
        FIELD tod_1pps_int              : 1;
        FIELD tod_update_done_int       : 1;
        FIELD aes_key_switch_done_int   : 1;
        FIELD los_gem_del_int           : 1;
        FIELD sn_req_crs_int            : 1;
        FIELD sn_onu_send_o4_int        : 1;
        FIELD ranging_req_recv_int      : 1;
        FIELD sn_onu_send_o3_int        : 1;
        FIELD sn_req_recv_int           : 1;
        FIELD ploamu_send_int           : 1;
        FIELD ploamd_recv_int           : 1;
#else
        FIELD ploamd_recv_int           : 1;
        FIELD ploamu_send_int           : 1;
        FIELD sn_req_recv_int           : 1;
        FIELD sn_onu_send_o3_int        : 1;
        FIELD ranging_req_recv_int      : 1;
        FIELD sn_onu_send_o4_int        : 1;
        FIELD sn_req_crs_int            : 1;
        FIELD los_gem_del_int           : 1;
        FIELD aes_key_switch_done_int   : 1;
        FIELD tod_update_done_int       : 1;
        FIELD tod_1pps_int              : 1;
        FIELD dying_gasp_send_int       : 1;
        FIELD rsv_12                    : 4;
        FIELD rx_err_int                : 1;
        FIELD fifo_err_int              : 1;
        FIELD bst_sgl_diff_int          : 1;
        FIELD tx_late_start_int         : 1;
        FIELD rx_eof_err_int            : 1;
        FIELD rx_gem_intlv_err_int      : 1;
        FIELD bfifo_full_int            : 1;
        FIELD sfifo_full_int            : 1;
        FIELD o5_eqd_adj_done_int       : 1;
        FIELD olt_ds_fec_chg_int        : 1;
        FIELD onu_us_fec_chg_int        : 1;
        FIELD pop_up_recv_in_O6_int     : 1;
        FIELD fwi_int                   : 1;
        FIELD lwi_int                   : 1;
        FIELD bwm_stop_time_err_int     : 1;
        FIELD bwm_us_fec_err_int        : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_INT_STATUS, *PREG_G_INT_STATUS;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD bwm_us_fec_err_int_en     : 1;
        FIELD bwm_stop_time_err_int_en  : 1;
        FIELD lwi_int_en                : 1;
        FIELD fwi_int_en                : 1;
        FIELD pop_up_recv_in_O6_int_en  : 1;
        FIELD onu_us_fec_chg_int_en     : 1;
        FIELD olt_ds_fec_chg_int_en     : 1;
        FIELD o5_eqd_adj_done_int_en    : 1;
        FIELD sfifo_full_int_en         : 1;
        FIELD bfifo_full_int_en         : 1;
        FIELD rx_gem_intlv_err_int_en   : 1;
        FIELD rx_eof_err_int_en         : 1;
        FIELD tx_late_start_int_en      : 1;
        FIELD bst_sgl_diff_int_en       : 1;
        FIELD fifo_err_int_en           : 1;
        FIELD rx_err_int_en             : 1;
        FIELD rsv_12                    : 4;
        FIELD dying_gasp_send_int_en    : 1;
        FIELD tod_1pps_int_en           : 1;
        FIELD tod_update_done_int_en    : 1;
        FIELD aes_key_switch_done_int_en : 1;
        FIELD los_gem_del_int_en        : 1;
        FIELD sn_req_crs_int_en         : 1;
        FIELD sn_onu_send_o4_int_en     : 1;
        FIELD ranging_req_recv_int_en   : 1;
        FIELD sn_onu_send_o3_int_en     : 1;
        FIELD sn_req_recv_int_en        : 1;
        FIELD ploamu_send_int_en        : 1;
        FIELD ploamd_recv_int_en        : 1;
#else
        FIELD ploamd_recv_int_en        : 1;
        FIELD ploamu_send_int_en        : 1;
        FIELD sn_req_recv_int_en        : 1;
        FIELD sn_onu_send_o3_int_en     : 1;
        FIELD ranging_req_recv_int_en   : 1;
        FIELD sn_onu_send_o4_int_en     : 1;
        FIELD sn_req_crs_int_en         : 1;
        FIELD los_gem_del_int_en        : 1;
        FIELD aes_key_switch_done_int_en : 1;
        FIELD tod_update_done_int_en    : 1;
        FIELD tod_1pps_int_en           : 1;
        FIELD dying_gasp_send_int_en    : 1;
        FIELD rsv_12                    : 4;
        FIELD rx_err_int_en             : 1;
        FIELD fifo_err_int_en           : 1;
        FIELD bst_sgl_diff_int_en       : 1;
        FIELD tx_late_start_int_en      : 1;
        FIELD rx_eof_err_int_en         : 1;
        FIELD rx_gem_intlv_err_int_en   : 1;
        FIELD bfifo_full_int_en         : 1;
        FIELD sfifo_full_int_en         : 1;
        FIELD o5_eqd_adj_done_int_en    : 1;
        FIELD olt_ds_fec_chg_int_en     : 1;
        FIELD onu_us_fec_chg_int_en     : 1;
        FIELD pop_up_recv_in_O6_int_en  : 1;
        FIELD fwi_int_en                : 1;
        FIELD lwi_int_en                : 1;
        FIELD bwm_stop_time_err_int_en  : 1;
        FIELD bwm_us_fec_err_int_en     : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_INT_ENABLE, *PREG_G_INT_ENABLE;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD t_cont1_vld               : 1;
        FIELD rsv_28                    : 3;
        FIELD t_cont1_id                : 12;
        FIELD t_cont0_vld               : 1;
        FIELD rsv_12                    : 3;
        FIELD t_cont0_id                : 12;
#else
        FIELD t_cont0_id                : 12;
        FIELD rsv_12                    : 3;
        FIELD t_cont0_vld               : 1;
        FIELD t_cont1_id                : 12;
        FIELD rsv_28                    : 3;
        FIELD t_cont1_vld               : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_TCONT_ID_0_1, *PREG_G_TCONT_ID_0_1;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD t_cont3_vld               : 1;
        FIELD rsv_28                    : 3;
        FIELD t_cont3_id                : 12;
        FIELD t_cont2_vld               : 1;
        FIELD rsv_12                    : 3;
        FIELD t_cont2_id                : 12;
#else
        FIELD t_cont2_id                : 12;
        FIELD rsv_12                    : 3;
        FIELD t_cont2_vld               : 1;
        FIELD t_cont3_id                : 12;
        FIELD rsv_28                    : 3;
        FIELD t_cont3_vld               : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_TCONT_ID_2_3, *PREG_G_TCONT_ID_2_3;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD t_cont5_vld               : 1;
        FIELD rsv_28                    : 3;
        FIELD t_cont5_id                : 12;
        FIELD t_cont4_vld               : 1;
        FIELD rsv_12                    : 3;
        FIELD t_cont4_id                : 12;
#else
        FIELD t_cont4_id                : 12;
        FIELD rsv_12                    : 3;
        FIELD t_cont4_vld               : 1;
        FIELD t_cont5_id                : 12;
        FIELD rsv_28                    : 3;
        FIELD t_cont5_vld               : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_TCONT_ID_4_5, *PREG_G_TCONT_ID_4_5;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD t_cont7_vld               : 1;
        FIELD rsv_28                    : 3;
        FIELD t_cont7_id                : 12;
        FIELD t_cont6_vld               : 1;
        FIELD rsv_12                    : 3;
        FIELD t_cont6_id                : 12;
#else
        FIELD t_cont6_id                : 12;
        FIELD rsv_12                    : 3;
        FIELD t_cont6_vld               : 1;
        FIELD t_cont7_id                : 12;
        FIELD rsv_28                    : 3;
        FIELD t_cont7_vld               : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_TCONT_ID_6_7, *PREG_G_TCONT_ID_6_7;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD t_cont9_vld               : 1;
        FIELD rsv_28                    : 3;
        FIELD t_cont9_id                : 12;
        FIELD t_cont8_vld               : 1;
        FIELD rsv_12                    : 3;
        FIELD t_cont8_id                : 12;
#else
        FIELD t_cont8_id                : 12;
        FIELD rsv_12                    : 3;
        FIELD t_cont8_vld               : 1;
        FIELD t_cont9_id                : 12;
        FIELD rsv_28                    : 3;
        FIELD t_cont9_vld               : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_TCONT_ID_8_9, *PREG_G_TCONT_ID_8_9;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD t_cont11_vld              : 1;
        FIELD rsv_28                    : 3;
        FIELD t_cont11_id               : 12;
        FIELD t_cont10_vld              : 1;
        FIELD rsv_12                    : 3;
        FIELD t_cont10_id               : 12;
#else
        FIELD t_cont10_id               : 12;
        FIELD rsv_12                    : 3;
        FIELD t_cont10_vld              : 1;
        FIELD t_cont11_id               : 12;
        FIELD rsv_28                    : 3;
        FIELD t_cont11_vld              : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_TCONT_ID_10_11, *PREG_G_TCONT_ID_10_11;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD t_cont13_vld              : 1;
        FIELD rsv_28                    : 3;
        FIELD t_cont13_id               : 12;
        FIELD t_cont12_vld              : 1;
        FIELD rsv_12                    : 3;
        FIELD t_cont12_id               : 12;
#else
        FIELD t_cont12_id               : 12;
        FIELD rsv_12                    : 3;
        FIELD t_cont12_vld              : 1;
        FIELD t_cont13_id               : 12;
        FIELD rsv_28                    : 3;
        FIELD t_cont13_vld              : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_TCONT_ID_12_13, *PREG_G_TCONT_ID_12_13;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD t_cont15_vld              : 1;
        FIELD rsv_28                    : 3;
        FIELD t_cont15_id               : 12;
        FIELD t_cont14_vld              : 1;
        FIELD rsv_12                    : 3;
        FIELD t_cont14_id               : 12;
#else
        FIELD t_cont14_id               : 12;
        FIELD rsv_12                    : 3;
        FIELD t_cont14_vld              : 1;
        FIELD t_cont15_id               : 12;
        FIELD rsv_28                    : 3;
        FIELD t_cont15_vld              : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_TCONT_ID_14_15, *PREG_G_TCONT_ID_14_15;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD gpid_cmd                  : 1;
        FIELD rsv_18                    : 13;
        FIELD gpid_encrypt              : 1;
        FIELD gpid_vld                  : 1;
        FIELD rsv_12                    : 4;
        FIELD gem_port_id               : 12;
#else
        FIELD gem_port_id               : 12;
        FIELD rsv_12                    : 4;
        FIELD gpid_vld                  : 1;
        FIELD gpid_encrypt              : 1;
        FIELD rsv_18                    : 13;
        FIELD gpid_cmd                  : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_GEM_PORT_CFG, *PREG_G_GEM_PORT_CFG;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD gpid_cmd_done             : 1;
        FIELD rsv_2                     : 29;
        FIELD gpid_encrypt_sts          : 1;
        FIELD gpid_vld_sts              : 1;
#else
        FIELD gpid_vld_sts              : 1;
        FIELD gpid_encrypt_sts          : 1;
        FIELD rsv_2                     : 29;
        FIELD gpid_cmd_done             : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_GEM_PORT_STS, *PREG_G_GEM_PORT_STS;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_17                    : 15;
        FIELD omci_port_id_vld          : 1;
        FIELD rsv_12                    : 4;
        FIELD omci_gpid                 : 12;
#else
        FIELD omci_gpid                 : 12;
        FIELD rsv_12                    : 4;
        FIELD omci_port_id_vld          : 1;
        FIELD rsv_17                    : 15;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_OMCI_ID, *PREG_G_OMCI_ID;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_9                     : 23;
        FIELD gem_tbl_init_done         : 1;
        FIELD rsv_1                     : 7;
        FIELD gem_tbl_init_start        : 1;
#else
        FIELD gem_tbl_init_start        : 1;
        FIELD rsv_1                     : 7;
        FIELD gem_tbl_init_done         : 1;
        FIELD rsv_9                     : 23;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_GEM_TBL_INIT, *PREG_G_GEM_TBL_INIT;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD ploamu_fifo_udrn          : 1;
        FIELD rsv_24                    : 7;
        FIELD ploamu_fifo_min_avail     : 8;
        FIELD rsv_8                     : 8;
        FIELD ploamu_fifo_avail         : 8;
#else
        FIELD ploamu_fifo_avail         : 8;
        FIELD rsv_8                     : 8;
        FIELD ploamu_fifo_min_avail     : 8;
        FIELD rsv_24                    : 7;
        FIELD ploamu_fifo_udrn          : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_PLOAMu_FIFO_STS, *PREG_G_PLOAMu_FIFO_STS;

typedef PACKING union
{
    PACKING struct
    {
        FIELD ploamu_wdata              : 32;
    } Bits;
    UINT32 Raw;
} REG_G_PLOAMu_WDATA, *PREG_G_PLOAMu_WDATA;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD ploamd_fifo_ovrn          : 1;
        FIELD rsv_24                    : 7;
        FIELD ploamd_fifo_max_used      : 8;
        FIELD rsv_8                     : 8;
        FIELD ploamd_fifo_used          : 8;
#else
        FIELD ploamd_fifo_used          : 8;
        FIELD rsv_8                     : 8;
        FIELD ploamd_fifo_max_used      : 8;
        FIELD rsv_24                    : 7;
        FIELD ploamd_fifo_ovrn          : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_PLOAMd_FIFO_STS, *PREG_G_PLOAMd_FIFO_STS;

typedef PACKING union
{
    PACKING struct
    {
        FIELD ploamd_rdata              : 32;
    } Bits;
    UINT32 Raw;
} REG_G_PLOAMd_RDATA, *PREG_G_PLOAMd_RDATA;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_30                    : 2;
        FIELD aes_spf_cnt               : 30;
#else
        FIELD aes_spf_cnt               : 30;
        FIELD rsv_30                    : 2;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_AES_CFG, *PREG_G_AES_CFG;

typedef PACKING union
{
    PACKING struct
    {
        FIELD aes_active_key0           : 32;
    } Bits;
    UINT32 Raw;
} REG_G_AES_ACTIVE_KEY0, *PREG_G_AES_ACTIVE_KEY0;

typedef PACKING union
{
    PACKING struct
    {
        FIELD aes_active_key1           : 32;
    } Bits;
    UINT32 Raw;
} REG_G_AES_ACTIVE_KEY1, *PREG_G_AES_ACTIVE_KEY1;

typedef PACKING union
{
    PACKING struct
    {
        FIELD aes_active_key2           : 32;
    } Bits;
    UINT32 Raw;
} REG_G_AES_ACTIVE_KEY2, *PREG_G_AES_ACTIVE_KEY2;

typedef PACKING union
{
    PACKING struct
    {
        FIELD aes_active_key3           : 32;
    } Bits;
    UINT32 Raw;
} REG_G_AES_ACTIVE_KEY3, *PREG_G_AES_ACTIVE_KEY3;

typedef PACKING union
{
    PACKING struct
    {
        FIELD aes_shadow_key0           : 32;
    } Bits;
    UINT32 Raw;
} REG_G_AES_SHADOW_KEY0, *PREG_G_AES_SHADOW_KEY0;

typedef PACKING union
{
    PACKING struct
    {
        FIELD aes_shadow_key1           : 32;
    } Bits;
    UINT32 Raw;
} REG_G_AES_SHADOW_KEY1, *PREG_G_AES_SHADOW_KEY1;

typedef PACKING union
{
    PACKING struct
    {
        FIELD aes_shadow_key2           : 32;
    } Bits;
    UINT32 Raw;
} REG_G_AES_SHADOW_KEY2, *PREG_G_AES_SHADOW_KEY2;

typedef PACKING union
{
    PACKING struct
    {
        FIELD aes_shadow_key3           : 32;
    } Bits;
    UINT32 Raw;
} REG_G_AES_SHADOW_KEY3, *PREG_G_AES_SHADOW_KEY3;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD aes_key_switch_by_sw_done : 1;
        FIELD rsv_1                     : 30;
        FIELD aes_key_switch_by_sw_cmd  : 1;
#else
        FIELD aes_key_switch_by_sw_cmd  : 1;
        FIELD rsv_1                     : 30;
        FIELD aes_key_switch_by_sw_done : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_AES_KEY_SWITCH_BY_SW, *PREG_G_AES_KEY_SWITCH_BY_SW;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_8                     : 24;
        FIELD plou_overhead             : 8;
#else
        FIELD plou_overhead             : 8;
        FIELD rsv_8                     : 24;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_PLOu_OVERHEAD, *PREG_G_PLOu_OVERHEAD;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_8                     : 24;
        FIELD guard_bit                 : 8;
#else
        FIELD guard_bit                 : 8;
        FIELD rsv_8                     : 24;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_PLOu_GUARD_BIT, *PREG_G_PLOu_GUARD_BIT;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_16                    : 16;
        FIELD prmb2_bit                 : 8;
        FIELD prmb1_bit                 : 8;
#else
        FIELD prmb1_bit                 : 8;
        FIELD prmb2_bit                 : 8;
        FIELD rsv_16                    : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_PLOu_PRMBL_TYPE1_2, *PREG_G_PLOu_PRMBL_TYPE1_2;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_25                    : 7;
        FIELD ebl_en                    : 1;
        FIELD rsv_16                    : 8;
        FIELD ext_prmb3_o5_num          : 8;
        FIELD ext_prmb3_o3_o4_num       : 8;
#else
        FIELD ext_prmb3_o3_o4_num       : 8;
        FIELD ext_prmb3_o5_num          : 8;
        FIELD rsv_16                    : 8;
        FIELD ebl_en                    : 1;
        FIELD rsv_25                    : 7;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_PLOu_PRMBL_TYPE3, *PREG_G_PLOu_PRMBL_TYPE3;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_8                     : 24;
        FIELD delm_bit                  : 8;
#else
        FIELD delm_bit                  : 8;
        FIELD rsv_8                     : 24;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_PLOu_DELM_BIT, *PREG_G_PLOu_DELM_BIT;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD pre_dly_en                : 1;
        FIELD rsv_16                    : 15;
        FIELD pre_dly                   : 16;
#else
        FIELD pre_dly                   : 16;
        FIELD rsv_16                    : 15;
        FIELD pre_dly_en                : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_PRE_ASSIGNED_DLY, *PREG_G_PRE_ASSIGNED_DLY;

typedef PACKING union
{
    PACKING struct
    {
        FIELD eqd                       : 32;
    } Bits;
    UINT32 Raw;
} REG_G_EQD, *PREG_G_EQD;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_16                    : 16;
        FIELD tresp                     : 16;
#else
        FIELD tresp                     : 16;
        FIELD rsv_16                    : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_RSP_TIME, *PREG_G_RSP_TIME;

typedef PACKING union
{
    PACKING struct
    {
        FIELD vendor_id                 : 32;
    } Bits;
    UINT32 Raw;
} REG_G_VENDOR_ID, *PREG_G_VENDOR_ID;

typedef PACKING union
{
    PACKING struct
    {
        FIELD vs_sn                     : 32;
    } Bits;
    UINT32 Raw;
} REG_G_VS_SN, *PREG_G_VS_SN;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD sn_req_thr                : 8;
        FIELD rsv_18                    : 6;
        FIELD tx_power_mode             : 2;
        FIELD rsv_12                    : 4;
        FIELD rdm_dly                   : 12;
#else
        FIELD rdm_dly                   : 12;
        FIELD rsv_12                    : 4;
        FIELD tx_power_mode             : 2;
        FIELD rsv_18                    : 6;
        FIELD sn_req_thr                : 8;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_SN_MSG_CFG, *PREG_G_SN_MSG_CFG;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_3                     : 29;
        FIELD act_st                    : 3;
#else
        FIELD act_st                    : 3;
        FIELD rsv_3                     : 29;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_ACTIVATION_ST, *PREG_G_ACTIVATION_ST;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_30                    : 2;
        FIELD tod_spf_cnt               : 30;
#else
        FIELD tod_spf_cnt               : 30;
        FIELD rsv_30                    : 2;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_TOD_CFG, *PREG_G_TOD_CFG;

typedef PACKING union
{
    PACKING struct
    {
        FIELD new_tod_sec_l32           : 32;
    } Bits;
    UINT32 Raw;
} REG_G_NEW_TOD_SEC_L32, *PREG_G_NEW_TOD_SEC_L32;

typedef PACKING union
{
    PACKING struct
    {
        FIELD new_tod_nano_sec          : 32;
    } Bits;
    UINT32 Raw;
} REG_G_NEW_TOD_NANO_SEC, *PREG_G_NEW_TOD_NANO_SEC;

typedef PACKING union
{
    PACKING struct
    {
        FIELD cur_tod_sec_l32           : 32;
    } Bits;
    UINT32 Raw;
} REG_G_CUR_TOD_SEC_L32, *PREG_G_CUR_TOD_SEC_L32;

typedef PACKING union
{
    PACKING struct
    {
        FIELD cur_tod_nano_sec          : 32;
    } Bits;
    UINT32 Raw;
} REG_G_CUR_TOD_NANO_SEC, *PREG_G_CUR_TOD_NANO_SEC;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_8                     : 24;
        FIELD tod_period                : 8;
#else
        FIELD tod_period                : 8;
        FIELD rsv_8                     : 24;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_TOD_CLK_PERIOD, *PREG_G_TOD_CLK_PERIOD;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_9                     : 23;
        FIELD tx_fcs_tbl_init_done      : 1;
        FIELD rsv_1                     : 7;
        FIELD tx_fcs_tbl_init_start     : 1;
#else
        FIELD tx_fcs_tbl_init_start     : 1;
        FIELD rsv_1                     : 7;
        FIELD tx_fcs_tbl_init_done      : 1;
        FIELD rsv_9                     : 23;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_TX_FCS_TBL_INIT, *PREG_G_TX_FCS_TBL_INIT;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD mib_cmd                   : 1;
        FIELD mib_cmd_done              : 1;
        FIELD rsv_25                    : 5;
        FIELD mib_read_clr_en           : 1;
        FIELD rsv_19                    : 5;
        FIELD mib_type                  : 3;
        FIELD rsv_12                    : 4;
        FIELD mib_gpid                  : 12;
#else
        FIELD mib_gpid                  : 12;
        FIELD rsv_12                    : 4;
        FIELD mib_type                  : 3;
        FIELD rsv_19                    : 5;
        FIELD mib_read_clr_en           : 1;
        FIELD rsv_25                    : 5;
        FIELD mib_cmd_done              : 1;
        FIELD mib_cmd                   : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_MIB_CTRL_STS, *PREG_G_MIB_CTRL_STS;

typedef PACKING union
{
    PACKING struct
    {
        FIELD mib_rdata_l32             : 32;
    } Bits;
    UINT32 Raw;
} REG_G_MIB_RDATA_L32, *PREG_G_MIB_RDATA_L32;

typedef PACKING union
{
    PACKING struct
    {
        FIELD mib_rdata_h32             : 32;
    } Bits;
    UINT32 Raw;
} REG_G_MIB_RDATA_H32, *PREG_G_MIB_RDATA_H32;

typedef PACKING union
{
    PACKING struct
    {
        FIELD mib_wdata_l32             : 32;
    } Bits;
    UINT32 Raw;
} REG_G_MIB_WDATA_L32, *PREG_G_MIB_WDATA_L32;

typedef PACKING union
{
    PACKING struct
    {
        FIELD mib_wdata_h32             : 32;
    } Bits;
    UINT32 Raw;
} REG_G_MIB_WDATA_H32, *PREG_G_MIB_WDATA_H32;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_9                     : 23;
        FIELD mib_tbl_init_done         : 1;
        FIELD rsv_1                     : 7;
        FIELD mib_tbl_init_start        : 1;
#else
        FIELD mib_tbl_init_start        : 1;
        FIELD rsv_1                     : 7;
        FIELD mib_tbl_init_done         : 1;
        FIELD rsv_9                     : 23;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_MIB_TBL_INIT, *PREG_G_MIB_TBL_INIT;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD gpidx_cmd                 : 1;
        FIELD rsv_24                    : 7;
        FIELD gpidx_tbl_addr            : 8;
        FIELD rsv_9                     : 7;
        FIELD gpidx_tbl_wdata           : 9;
#else
        FIELD gpidx_tbl_wdata           : 9;
        FIELD rsv_9                     : 7;
        FIELD gpidx_tbl_addr            : 8;
        FIELD rsv_24                    : 7;
        FIELD gpidx_cmd                 : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_GPIDX_TBL_CTRL, *PREG_G_GPIDX_TBL_CTRL;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD gpidx_cmd_done            : 1;
        FIELD rsv_9                     : 22;
        FIELD gpidx_tbl_rdata           : 9;
#else
        FIELD gpidx_tbl_rdata           : 9;
        FIELD rsv_9                     : 22;
        FIELD gpidx_cmd_done            : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_GPIDX_TBL_STS, *PREG_G_GPIDX_TBL_STS;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_9                     : 23;
        FIELD gpidx_tbl_init_done       : 1;
        FIELD rsv_1                     : 7;
        FIELD gpidx_tbl_init_start      : 1;
#else
        FIELD gpidx_tbl_init_start      : 1;
        FIELD rsv_1                     : 7;
        FIELD gpidx_tbl_init_done       : 1;
        FIELD rsv_9                     : 23;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_GPIDX_TBL_INIT, *PREG_G_GPIDX_TBL_INIT;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_9                     : 23;
        FIELD mbi_tx_stop               : 1;
        FIELD rsv_1                     : 7;
        FIELD mbi_rx_stop               : 1;
#else
        FIELD mbi_rx_stop               : 1;
        FIELD rsv_1                     : 7;
        FIELD mbi_tx_stop               : 1;
        FIELD rsv_9                     : 23;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_MBI_STOP, *PREG_G_MBI_STOP;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD tcont_cmd                 : 1;
        FIELD rsv_28                    : 3;
        FIELD wr_tcont_id_vld           : 1;
        FIELD rsv_20                    : 7;
        FIELD tcont_id_index            : 4;
        FIELD rsv_12                    : 4;
        FIELD wr_tcont_id               : 12;
#else
        FIELD wr_tcont_id               : 12;
        FIELD rsv_12                    : 4;
        FIELD tcont_id_index            : 4;
        FIELD rsv_20                    : 7;
        FIELD wr_tcont_id_vld           : 1;
        FIELD rsv_28                    : 3;
        FIELD tcont_cmd                 : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_TCONT_ID_16_31_CFG, *PREG_G_TCONT_ID_16_31_CFG;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD tcont_cmd_done            : 1;
        FIELD rsv_17                    : 14;
        FIELD rd_tcont_id_vld           : 1;
        FIELD rsv_12                    : 4;
        FIELD rd_tcont_id               : 12;
#else
        FIELD rd_tcont_id               : 12;
        FIELD rsv_12                    : 4;
        FIELD rd_tcont_id_vld           : 1;
        FIELD rsv_17                    : 14;
        FIELD tcont_cmd_done            : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_G_TCONT_ID_16_31_STS, *PREG_G_TCONT_ID_16_31_STS;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_27                    : 5;
        FIELD mib_frame_type            : 1;
        FIELD gpon_mib_en               : 1;
        FIELD us_no_msg_int_en          : 1;
        FIELD rsv_17                    : 7;
        FIELD rpt_msg_flt               : 1;
        FIELD rsv_12                    : 4;
        FIELD max_rdm_dly               : 12;
#else
        FIELD max_rdm_dly               : 12;
        FIELD rsv_12                    : 4;
        FIELD rpt_msg_flt               : 1;
        FIELD rsv_17                    : 7;
        FIELD us_no_msg_int_en          : 1;
        FIELD gpon_mib_en               : 1;
        FIELD mib_frame_type            : 1;
        FIELD rsv_27                    : 5;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_CAP_SETTING, *PREG_DBG_CAP_SETTING;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD sch_pref_ofs              : 16;
        FIELD dt_pref_ofs               : 16;
#else
        FIELD dt_pref_ofs               : 16;
        FIELD sch_pref_ofs              : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_US_PREF_OFS, *PREG_DBG_US_PREF_OFS;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD phy_rx_dly_sel            : 1;
        FIELD rsv_28                    : 3;
        FIELD fix_phy_rx_dly            : 12;
        FIELD fine_int_dly              : 8;
        FIELD phy_tx_dly                : 8;
#else
        FIELD phy_tx_dly                : 8;
        FIELD fine_int_dly              : 8;
        FIELD fix_phy_rx_dly            : 12;
        FIELD rsv_28                    : 3;
        FIELD phy_rx_dly_sel            : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_DLY, *PREG_DBG_DLY;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_16                    : 16;
        FIELD idle_gem_thld             : 16;
#else
        FIELD idle_gem_thld             : 16;
        FIELD rsv_16                    : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_IDLE_GEM_THLD, *PREG_DBG_IDLE_GEM_THLD;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD us_no_msg_b3_b4           : 16;
        FIELD us_no_msg_type            : 8;
        FIELD rsv_0                     : 8;
#else
        FIELD rsv_0                     : 8;
        FIELD us_no_msg_type            : 8;
        FIELD us_no_msg_b3_b4           : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_US_NO_MSG0, *PREG_DBG_US_NO_MSG0;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD us_no_msg_b5_b6           : 16;
        FIELD us_no_msg_b7_b8           : 16;
#else
        FIELD us_no_msg_b7_b8           : 16;
        FIELD us_no_msg_b5_b6           : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_US_NO_MSG1, *PREG_DBG_US_NO_MSG1;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD us_no_msg_b9_b10          : 16;
        FIELD us_no_msg_b11_b12         : 16;
#else
        FIELD us_no_msg_b11_b12         : 16;
        FIELD us_no_msg_b9_b10          : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_US_NO_MSG2, *PREG_DBG_US_NO_MSG2;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_28                    : 4;
        FIELD dying_gasp_num            : 4;
        FIELD rsv_17                    : 7;
        FIELD dying_gasp_test           : 1;
        FIELD rsv_9                     : 7;
        FIELD hw_dying_gasp_en          : 1;
        FIELD dying_gasp_msg_type       : 8;
#else
        FIELD dying_gasp_msg_type       : 8;
        FIELD hw_dying_gasp_en          : 1;
        FIELD rsv_9                     : 7;
        FIELD dying_gasp_test           : 1;
        FIELD rsv_17                    : 7;
        FIELD dying_gasp_num            : 4;
        FIELD rsv_28                    : 4;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_US_DYING_GASP_CTRL, *PREG_DBG_US_DYING_GASP_CTRL;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD bwm_intvl_invld           : 1;
        FIELD bwm_len_invld             : 1;
        FIELD bwm_start_order_err       : 1;
        FIELD rsv_19                    : 10;
        FIELD bwm_start_order_err_flt_en : 1;
        FIELD bwm_len_vld_chk_en        : 1;
        FIELD min_bst_intvl_en          : 1;
        FIELD min_bst_intvl             : 16;
#else
        FIELD min_bst_intvl             : 16;
        FIELD min_bst_intvl_en          : 1;
        FIELD bwm_len_vld_chk_en        : 1;
        FIELD bwm_start_order_err_flt_en : 1;
        FIELD rsv_19                    : 10;
        FIELD bwm_start_order_err       : 1;
        FIELD bwm_len_invld             : 1;
        FIELD bwm_intvl_invld           : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_BWM_FILTER_CTRL, *PREG_DBG_BWM_FILTER_CTRL;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_25                    : 7;
        FIELD bwm_sfifo_max_used        : 9;
        FIELD rsv_9                     : 7;
        FIELD bwm_sfifo_used            : 9;
#else
        FIELD bwm_sfifo_used            : 9;
        FIELD rsv_9                     : 7;
        FIELD bwm_sfifo_max_used        : 9;
        FIELD rsv_25                    : 7;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_BWM_SFIFO_STS, *PREG_DBG_BWM_SFIFO_STS;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_22                    : 10;
        FIELD ether_fcs_err             : 1;
        FIELD ploamu_wr                 : 1;
        FIELD dbru_wr                   : 1;
        FIELD is_gem_pl                 : 1;
        FIELD is_fcs_only               : 1;
        FIELD is_inst_idle              : 1;
        FIELD rsv_6                     : 10;
        FIELD mbi_pl_fifo_ovrn          : 1;
        FIELD aes_egn3_busy             : 1;
        FIELD aes_egn2_busy             : 1;
        FIELD aes_egn1_busy             : 1;
        FIELD aes_egn0_busy             : 1;
        FIELD aes_rdm_ciph_fifo_ovrn    : 1;
#else
        FIELD aes_rdm_ciph_fifo_ovrn    : 1;
        FIELD aes_egn0_busy             : 1;
        FIELD aes_egn1_busy             : 1;
        FIELD aes_egn2_busy             : 1;
        FIELD aes_egn3_busy             : 1;
        FIELD mbi_pl_fifo_ovrn          : 1;
        FIELD rsv_6                     : 10;
        FIELD is_inst_idle              : 1;
        FIELD is_fcs_only               : 1;
        FIELD is_gem_pl                 : 1;
        FIELD dbru_wr                   : 1;
        FIELD ploamu_wr                 : 1;
        FIELD ether_fcs_err             : 1;
        FIELD rsv_22                    : 10;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_GRP_0, *PREG_DBG_GRP_0;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_25                    : 7;
        FIELD alen_neq_zero             : 1;
        FIELD rsv_20                    : 4;
        FIELD tx_align_fifo_udrn        : 1;
        FIELD rx_sof_sync               : 1;
        FIELD sync_start                : 1;
        FIELD tx_late_start             : 1;
        FIELD rsv_9                     : 7;
        FIELD bst_next_full             : 1;
        FIELD sgl_next_full             : 1;
        FIELD aes_cryp_cnt_fifo_ovrn    : 1;
        FIELD aes_ciph_txt_fifo_ovrn    : 1;
        FIELD mbi_hdr_fifo_ovrn         : 1;
        FIELD ploamd_crc_err            : 1;
        FIELD bwm_uc_err                : 1;
        FIELD blen_err                  : 1;
        FIELD logd                      : 1;
#else
        FIELD logd                      : 1;
        FIELD blen_err                  : 1;
        FIELD bwm_uc_err                : 1;
        FIELD ploamd_crc_err            : 1;
        FIELD mbi_hdr_fifo_ovrn         : 1;
        FIELD aes_ciph_txt_fifo_ovrn    : 1;
        FIELD aes_cryp_cnt_fifo_ovrn    : 1;
        FIELD sgl_next_full             : 1;
        FIELD bst_next_full             : 1;
        FIELD rsv_9                     : 7;
        FIELD tx_late_start             : 1;
        FIELD sync_start                : 1;
        FIELD rx_sof_sync               : 1;
        FIELD tx_align_fifo_udrn        : 1;
        FIELD rsv_20                    : 4;
        FIELD alen_neq_zero             : 1;
        FIELD rsv_25                    : 7;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_GRP_1, *PREG_DBG_GRP_1;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_16                    : 16;
        FIELD tx_align_max_used         : 16;
#else
        FIELD tx_align_max_used         : 16;
        FIELD rsv_16                    : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_TXFIFO_MAX_USED, *PREG_DBG_TXFIFO_MAX_USED;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_20                    : 12;
        FIELD pls_used                  : 1;
        FIELD dba_mode2_used            : 1;
        FIELD dba_mode1_used            : 1;
        FIELD dba_mode0_used            : 1;
        FIELD rsv_8                     : 8;
        FIELD max_assigned_gnt_num      : 8;
#else
        FIELD max_assigned_gnt_num      : 8;
        FIELD rsv_8                     : 8;
        FIELD dba_mode0_used            : 1;
        FIELD dba_mode1_used            : 1;
        FIELD dba_mode2_used            : 1;
        FIELD pls_used                  : 1;
        FIELD rsv_20                    : 12;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_BWM_GNT_STS, *PREG_DBG_BWM_GNT_STS;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_25                    : 7;
        FIELD gpon_sub_probe_sel        : 5;
        FIELD probe_bit0_sel            : 4;
        FIELD bwm_probe_sel             : 4;
        FIELD tx_align_probe_dt_sel     : 1;
        FIELD rx_mpi_mbi_probe_dt_sel   : 1;
        FIELD tx_mpi_mbi_probe_dt_sel   : 1;
        FIELD rx_data_hl_sel            : 1;
        FIELD rsv_5                     : 3;
        FIELD probe_sel                 : 5;
#else
        FIELD probe_sel                 : 5;
        FIELD rsv_5                     : 3;
        FIELD rx_data_hl_sel            : 1;
        FIELD tx_mpi_mbi_probe_dt_sel   : 1;
        FIELD rx_mpi_mbi_probe_dt_sel   : 1;
        FIELD tx_align_probe_dt_sel     : 1;
        FIELD bwm_probe_sel             : 4;
        FIELD probe_bit0_sel            : 4;
        FIELD gpon_sub_probe_sel        : 5;
        FIELD rsv_25                    : 7;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_PROBE_CTRL, *PREG_DBG_PROBE_CTRL;

typedef PACKING union
{
    PACKING struct
    {
        FIELD probe_high32              : 32;
    } Bits;
    UINT32 Raw;
} REG_DBG_PROBE_HIGH32, *PREG_DBG_PROBE_HIGH32;

typedef PACKING union
{
    PACKING struct
    {
        FIELD probe_low32               : 32;
    } Bits;
    UINT32 Raw;
} REG_DBG_PROBE_LOW32, *PREG_DBG_PROBE_LOW32;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_25                    : 7;
        FIELD bwm_bfifo_max_used        : 9;
        FIELD rsv_9                     : 7;
        FIELD bwm_bfifo_used            : 9;
#else
        FIELD bwm_bfifo_used            : 9;
        FIELD rsv_9                     : 7;
        FIELD bwm_bfifo_max_used        : 9;
        FIELD rsv_25                    : 7;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_BWM_BFIFO_STS, *PREG_DBG_BWM_BFIFO_STS;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_10                    : 22;
        FIELD bwm_fec_ctrl              : 1;
        FIELD ignore_atm_cell           : 1;
        FIELD rsv_5                     : 3;
        FIELD o5_eqd_adj_resync_en      : 1;
        FIELD tx_udrn_resync_en         : 1;
        FIELD tx_late_resync_en         : 1;
        FIELD bst_sgl_diff_resync_en    : 1;
        FIELD ignore_ploamd_crc         : 1;
#else
        FIELD ignore_ploamd_crc         : 1;
        FIELD bst_sgl_diff_resync_en    : 1;
        FIELD tx_late_resync_en         : 1;
        FIELD tx_udrn_resync_en         : 1;
        FIELD o5_eqd_adj_resync_en      : 1;
        FIELD rsv_5                     : 3;
        FIELD ignore_atm_cell           : 1;
        FIELD bwm_fec_ctrl              : 1;
        FIELD rsv_10                    : 22;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_ERR_CTRL, *PREG_DBG_ERR_CTRL;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_9                     : 23;
        FIELD rdi_en                    : 1;
        FIELD rsv_1                     : 7;
        FIELD sw_rdi                    : 1;
#else
        FIELD sw_rdi                    : 1;
        FIELD rsv_1                     : 7;
        FIELD rdi_en                    : 1;
        FIELD rsv_9                     : 23;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_SW_RDI_CTRL, *PREG_DBG_SW_RDI_CTRL;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dbg_rx_gem_cnt            : 32;
    } Bits;
    UINT32 Raw;
} REG_DBG_RX_GEM_CNT, *PREG_DBG_RX_GEM_CNT;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dbg_rx_crc_cnt            : 32;
    } Bits;
    UINT32 Raw;
} REG_DBG_RX_CRC_ERR_CNT, *PREG_DBG_RX_CRC_ERR_CNT;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dbg_rx_gtc_cnt            : 32;
    } Bits;
    UINT32 Raw;
} REG_DBG_RX_GTC_CNT, *PREG_DBG_RX_GTC_CNT;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dbg_tx_gem_cnt            : 32;
    } Bits;
    UINT32 Raw;
} REG_DBG_TX_GEM_CNT, *PREG_DBG_TX_GEM_CNT;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dbg_tx_burst_cnt          : 32;
    } Bits;
    UINT32 Raw;
} REG_DBG_TX_BST_CNT, *PREG_DBG_TX_BST_CNT;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dbg_gem_one_err_cnt       : 32;
    } Bits;
    UINT32 Raw;
} REG_DBG_GEM_HEC_ONE_ERR_CNT, *PREG_DBG_GEM_HEC_ONE_ERR_CNT;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dbg_gem_two_err_cnt       : 32;
    } Bits;
    UINT32 Raw;
} REG_DBG_GEM_HEC_TWO_ERR_CNT, *PREG_DBG_GEM_HEC_TWO_ERR_CNT;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dbg_gem_uc_err_cnt        : 32;
    } Bits;
    UINT32 Raw;
} REG_DBG_GEM_HEC_UC_ERR_CNT, *PREG_DBG_GEM_HEC_UC_ERR_CNT;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD dbg_sw_rpt_dba_en         : 1;
        FIELD rsv_28                    : 3;
        FIELD dbg_slight_modify_en      : 1;
        FIELD rsv_0                     : 27;
#else
        FIELD rsv_0                     : 27;
        FIELD dbg_slight_modify_en      : 1;
        FIELD rsv_28                    : 3;
        FIELD dbg_sw_rpt_dba_en         : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_DBA_BACK_DOOR, *PREG_DBG_DBA_BACK_DOOR;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dbg_dba_total_buf_size    : 32;
    } Bits;
    UINT32 Raw;
} REG_DBG_DBA_TOTAL_BUF_SIZE, *PREG_DBG_DBA_TOTAL_BUF_SIZE;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dbg_dba_green_size        : 32;
    } Bits;
    UINT32 Raw;
} REG_DBG_DBA_GREEN_SIZE, *PREG_DBG_DBA_GREEN_SIZE;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dbg_dba_yellow_size       : 32;
    } Bits;
    UINT32 Raw;
} REG_DBG_DBA_YELLOW_SIZE, *PREG_DBG_DBA_YELLOW_SIZE;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD dbg_yellow_slight_modify_size : 16;
        FIELD dbg_green_slight_modify_size : 16;
#else
        FIELD dbg_green_slight_modify_size : 16;
        FIELD dbg_yellow_slight_modify_size : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_SLIGHT_MODIFY_SIZE_1, *PREG_DBG_SLIGHT_MODIFY_SIZE_1;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_16                    : 16;
        FIELD dbg_total_slight_modify_size : 16;
#else
        FIELD dbg_total_slight_modify_size : 16;
        FIELD rsv_16                    : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_SLIGHT_MODIFY_SIZE_2, *PREG_DBG_SLIGHT_MODIFY_SIZE_2;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_30                    : 2;
        FIELD dbg_ds_spf_cnt            : 30;
#else
        FIELD dbg_ds_spf_cnt            : 30;
        FIELD rsv_30                    : 2;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_DS_SPF_CNT, *PREG_DBG_DS_SPF_CNT;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_2                     : 30;
        FIELD dbg_tx_sync_offset        : 2;
#else
        FIELD dbg_tx_sync_offset        : 2;
        FIELD rsv_2                     : 30;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_TX_SYNC_OFFSET, *PREG_DBG_TX_SYNC_OFFSET;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_9                     : 23;
        FIELD dbg_ext_bst_len_filter_in_o5_en : 1;
        FIELD rsv_1                     : 7;
        FIELD dbg_us_overhead_filter_in_o5_en : 1;
#else
        FIELD dbg_us_overhead_filter_in_o5_en : 1;
        FIELD rsv_1                     : 7;
        FIELD dbg_ext_bst_len_filter_in_o5_en : 1;
        FIELD rsv_9                     : 23;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_PLOAMD_FILTER_IN_O5, *PREG_DBG_PLOAMD_FILTER_IN_O5;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD dbg_sw_resync_en          : 1;
        FIELD rsv_1                     : 30;
        FIELD dbg_sw_resync             : 1;
#else
        FIELD dbg_sw_resync             : 1;
        FIELD rsv_1                     : 30;
        FIELD dbg_sw_resync_en          : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_SW_RESYNC, *PREG_DBG_SW_RESYNC;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD sniff_dbg_tx_rst          : 1;
        FIELD rsv_9                     : 22;
        FIELD sniff_pkt_padding_en      : 1;
        FIELD rsv_1                     : 7;
        FIELD dbg_gtc_eth_extr_en       : 1;
#else
        FIELD dbg_gtc_eth_extr_en       : 1;
        FIELD rsv_1                     : 7;
        FIELD sniff_pkt_padding_en      : 1;
        FIELD rsv_9                     : 22;
        FIELD sniff_dbg_tx_rst          : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_GTC_ETH_EXTR, *PREG_DBG_GTC_ETH_EXTR;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD dbg_extr_rx_ds_eth_type   : 16;
        FIELD dbg_extr_rx_ds_sp         : 16;
#else
        FIELD dbg_extr_rx_ds_sp         : 16;
        FIELD dbg_extr_rx_ds_eth_type   : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_DS_GTC_EXTR_ETH_HDR, *PREG_DBG_DS_GTC_EXTR_ETH_HDR;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD dbg_extr_tx_us_eth_type   : 16;
        FIELD dbg_extr_tx_us_sp         : 16;
#else
        FIELD dbg_extr_tx_us_sp         : 16;
        FIELD dbg_extr_tx_us_eth_type   : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_US_GTC_EXTR_ETH_HDR, *PREG_DBG_US_GTC_EXTR_ETH_HDR;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dbg_extr_rx_cnt           : 32;
    } Bits;
    UINT32 Raw;
} REG_DBG_DS_GTC_EXTR_ETH_CNT, *PREG_DBG_DS_GTC_EXTR_ETH_CNT;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dbg_extr_tx_cnt           : 32;
    } Bits;
    UINT32 Raw;
} REG_DBG_US_GTC_EXTR_ETH_CNT, *PREG_DBG_US_GTC_EXTR_ETH_CNT;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD dbg_ds_all_gem_filter     : 1;
        FIELD rsv_28                    : 3;
        FIELD dbg_ds_gem_filter_exclude_omci : 1;
        FIELD rsv_16                    : 11;
        FIELD dbg_us_gtc_invld_gem_byte : 16;
#else
        FIELD dbg_us_gtc_invld_gem_byte : 16;
        FIELD rsv_16                    : 11;
        FIELD dbg_ds_gem_filter_exclude_omci : 1;
        FIELD rsv_28                    : 3;
        FIELD dbg_ds_all_gem_filter     : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_SNIFF_GTC_GTC_INVLD_GEM_BYTE, *PREG_SNIFF_GTC_GTC_INVLD_GEM_BYTE;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_17                    : 15;
        FIELD dbg_mbi_pl_fifo_write_conflict : 1;
        FIELD rsv_9                     : 7;
        FIELD dbg_mbi_hdr_fifo_write_conflict : 1;
        FIELD rsv_1                     : 7;
        FIELD dbg_us_gtc_info_fifo_ovrn : 1;
#else
        FIELD dbg_us_gtc_info_fifo_ovrn : 1;
        FIELD rsv_1                     : 7;
        FIELD dbg_mbi_hdr_fifo_write_conflict : 1;
        FIELD rsv_9                     : 7;
        FIELD dbg_mbi_pl_fifo_write_conflict : 1;
        FIELD rsv_17                    : 15;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_SNIFF_DBG_REG, *PREG_DBG_SNIFF_DBG_REG;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_7                     : 25;
        FIELD dbg_us_info_tx_send_depth : 7;
#else
        FIELD dbg_us_info_tx_send_depth : 7;
        FIELD rsv_7                     : 25;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_SNIFF_US_INFO_FIFO, *PREG_DBG_SNIFF_US_INFO_FIFO;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD stop_time_of_wido_en      : 1;
        FIELD rsv_16                    : 15;
        FIELD stop_time_of_wido         : 16;
#else
        FIELD stop_time_of_wido         : 16;
        FIELD rsv_16                    : 15;
        FIELD stop_time_of_wido_en      : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_SNIFF_US_HDR_WIDO, *PREG_SNIFF_US_HDR_WIDO;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD tx_us_da_h16              : 16;
        FIELD tx_us_sa_h16              : 16;
#else
        FIELD tx_us_sa_h16              : 16;
        FIELD tx_us_da_h16              : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_SNIFF_TX_DA_SA, *PREG_SNIFF_TX_DA_SA;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rx_ds_da_h16              : 16;
        FIELD rx_ds_sa_h16              : 16;
#else
        FIELD rx_ds_sa_h16              : 16;
        FIELD rx_ds_da_h16              : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_SNIFF_RX_DA_SA, *PREG_SNIFF_RX_DA_SA;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_28                    : 4;
        FIELD tx_eth_pid                : 12;
        FIELD rsv_12                    : 4;
        FIELD rx_eth_pid                : 12;
#else
        FIELD rx_eth_pid                : 12;
        FIELD rsv_12                    : 4;
        FIELD tx_eth_pid                : 12;
        FIELD rsv_28                    : 4;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_SNIFF_TX_RX_PID, *PREG_SNIFF_TX_RX_PID;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_1                     : 31;
        FIELD sniff_cnt_en              : 1;
#else
        FIELD sniff_cnt_en              : 1;
        FIELD rsv_1                     : 31;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_SNIFF_CNT_EN, *PREG_SNIFF_CNT_EN;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD sniff_rx_tpid             : 16;
        FIELD sniff_tx_tpid             : 16;
#else
        FIELD sniff_tx_tpid             : 16;
        FIELD sniff_rx_tpid             : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_SNIFF_RX_TX_TPID, *PREG_SNIFF_RX_TX_TPID;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD asb_tx_eth_4byte_align_en : 1;
        FIELD rsv_28                    : 3;
        FIELD olt_ds_fec                : 1;
        FIELD rsv_24                    : 3;
        FIELD onu_us_fec                : 1;
        FIELD rsv_1                     : 22;
        FIELD gpon_mac_sw_rst_n         : 1;
#else
        FIELD gpon_mac_sw_rst_n         : 1;
        FIELD rsv_1                     : 22;
        FIELD onu_us_fec                : 1;
        FIELD rsv_24                    : 3;
        FIELD olt_ds_fec                : 1;
        FIELD rsv_28                    : 3;
        FIELD asb_tx_eth_4byte_align_en : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_DBG_GPON_MAC_SET, *PREG_DBG_GPON_MAC_SET;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD sleep_filt_all_bwm        : 1;
        FIELD reload_sleep_cnt          : 1;
        FIELD rsv_0                     : 30;
#else
        FIELD rsv_0                     : 30;
        FIELD reload_sleep_cnt          : 1;
        FIELD sleep_filt_all_bwm        : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_SLEEP_GLB_CFG, *PREG_SLEEP_GLB_CFG;

typedef PACKING union
{
    PACKING struct
    {
        FIELD sleep_cnt                 : 32;
    } Bits;
    UINT32 Raw;
} REG_SLEEP_CNT, *PREG_SLEEP_CNT;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_1                     : 31;
        FIELD tx_sync_opt               : 1;
#else
        FIELD tx_sync_opt               : 1;
        FIELD rsv_1                     : 31;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_TX_SYNC_OPT, *PREG_TX_SYNC_OPT;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_9                     : 23;
        FIELD ploamu_ind_ctrl           : 1;
        FIELD rsv_1                     : 7;
        FIELD o3_o4_ploamu_ctrl         : 1;
#else
        FIELD o3_o4_ploamu_ctrl         : 1;
        FIELD rsv_1                     : 7;
        FIELD ploamu_ind_ctrl           : 1;
        FIELD rsv_9                     : 23;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_PLOAMU_CTRL, *PREG_PLOAMU_CTRL;

typedef PACKING union
{
    PACKING struct
    {
        FIELD tod_1pps_width_ctrl       : 32;
    } Bits;
    UINT32 Raw;
} REG_TOD_1PPS_WD_CTRL, *PREG_TOD_1PPS_WD_CTRL;

// ----------------- gpon_mac_reg  Grouping Definitions -------------------
// ----------------- gpon_mac_reg Register Definition -------------------
typedef volatile PACKING struct
{
    UINT32                          rsv_0000[4096];   // 0000..3FFC
    REG_G_ONU_ID                    G_ONU_ID;         // 4000
    REG_G_GBL_CFG                   G_GBL_CFG;        // 4004
    REG_G_INT_STATUS                G_INT_STATUS;     // 4008
    REG_G_INT_ENABLE                G_INT_ENABLE;     // 400C
    UINT32                          rsv_4010[4];      // 4010..401C
    REG_G_TCONT_ID_0_1              G_TCONT_ID_0_1;   // 4020
    REG_G_TCONT_ID_2_3              G_TCONT_ID_2_3;   // 4024
    REG_G_TCONT_ID_4_5              G_TCONT_ID_4_5;   // 4028
    REG_G_TCONT_ID_6_7              G_TCONT_ID_6_7;   // 402C
    REG_G_TCONT_ID_8_9              G_TCONT_ID_8_9;   // 4030
    REG_G_TCONT_ID_10_11            G_TCONT_ID_10_11; // 4034
    REG_G_TCONT_ID_12_13            G_TCONT_ID_12_13; // 4038
    REG_G_TCONT_ID_14_15            G_TCONT_ID_14_15; // 403C
    REG_G_GEM_PORT_CFG              G_GEM_PORT_CFG;   // 4040
    REG_G_GEM_PORT_STS              G_GEM_PORT_STS;   // 4044
    REG_G_OMCI_ID                   G_OMCI_ID;        // 4048
    REG_G_GEM_TBL_INIT              G_GEM_TBL_INIT;   // 404C
    REG_G_PLOAMu_FIFO_STS           G_PLOAMu_FIFO_STS; // 4050
    REG_G_PLOAMu_WDATA              G_PLOAMu_WDATA;   // 4054
    REG_G_PLOAMd_FIFO_STS           G_PLOAMd_FIFO_STS; // 4058
    REG_G_PLOAMd_RDATA              G_PLOAMd_RDATA;   // 405C
    REG_G_AES_CFG                   G_AES_CFG;        // 4060
    REG_G_AES_ACTIVE_KEY0           G_AES_ACTIVE_KEY0; // 4064
    REG_G_AES_ACTIVE_KEY1           G_AES_ACTIVE_KEY1; // 4068
    REG_G_AES_ACTIVE_KEY2           G_AES_ACTIVE_KEY2; // 406C
    REG_G_AES_ACTIVE_KEY3           G_AES_ACTIVE_KEY3; // 4070
    REG_G_AES_SHADOW_KEY0           G_AES_SHADOW_KEY0; // 4074
    REG_G_AES_SHADOW_KEY1           G_AES_SHADOW_KEY1; // 4078
    REG_G_AES_SHADOW_KEY2           G_AES_SHADOW_KEY2; // 407C
    REG_G_AES_SHADOW_KEY3           G_AES_SHADOW_KEY3; // 4080
    REG_G_AES_KEY_SWITCH_BY_SW      G_AES_KEY_SWITCH_BY_SW; // 4084
    UINT32                          rsv_4088[2];      // 4088..408C
    REG_G_PLOu_OVERHEAD             G_PLOu_OVERHEAD;  // 4090
    REG_G_PLOu_GUARD_BIT            G_PLOu_GUARD_BIT; // 4094
    REG_G_PLOu_PRMBL_TYPE1_2        G_PLOu_PRMBL_TYPE1_2; // 4098
    REG_G_PLOu_PRMBL_TYPE3          G_PLOu_PRMBL_TYPE3; // 409C
    REG_G_PLOu_DELM_BIT             G_PLOu_DELM_BIT;  // 40A0
    REG_G_PRE_ASSIGNED_DLY          G_PRE_ASSIGNED_DLY; // 40A4
    REG_G_EQD                       G_EQD;            // 40A8
    REG_G_RSP_TIME                  G_RSP_TIME;       // 40AC
    REG_G_VENDOR_ID                 G_VENDOR_ID;      // 40B0
    REG_G_VS_SN                     G_VS_SN;          // 40B4
    REG_G_SN_MSG_CFG                G_SN_MSG_CFG;     // 40B8
    REG_G_ACTIVATION_ST             G_ACTIVATION_ST;  // 40BC
    UINT32                          rsv_40C0[4];      // 40C0..40CC
    REG_G_TOD_CFG                   G_TOD_CFG;        // 40D0
    REG_G_NEW_TOD_SEC_L32           G_NEW_TOD_SEC_L32; // 40D4
    REG_G_NEW_TOD_NANO_SEC          G_NEW_TOD_NANO_SEC; // 40D8
    REG_G_CUR_TOD_SEC_L32           G_CUR_TOD_SEC_L32; // 40DC
    REG_G_CUR_TOD_NANO_SEC          G_CUR_TOD_NANO_SEC; // 40E0
    REG_G_TOD_CLK_PERIOD            G_TOD_CLK_PERIOD; // 40E4
    UINT32                          rsv_40E8[6];      // 40E8..40FC
    REG_G_TX_FCS_TBL_INIT           G_TX_FCS_TBL_INIT; // 4100
    UINT32                          rsv_4104[7];      // 4104..411C
    REG_G_MIB_CTRL_STS              G_MIB_CTRL_STS;   // 4120
    REG_G_MIB_RDATA_L32             G_MIB_RDATA_L32;  // 4124
    REG_G_MIB_RDATA_H32             G_MIB_RDATA_H32;  // 4128
    REG_G_MIB_WDATA_L32             G_MIB_WDATA_L32;  // 412C
    REG_G_MIB_WDATA_H32             G_MIB_WDATA_H32;  // 4130
    REG_G_MIB_TBL_INIT              G_MIB_TBL_INIT;   // 4134
    UINT32                          rsv_4138[2];      // 4138..413C
    REG_G_GPIDX_TBL_CTRL            G_GPIDX_TBL_CTRL; // 4140
    REG_G_GPIDX_TBL_STS             G_GPIDX_TBL_STS;  // 4144
    REG_G_GPIDX_TBL_INIT            G_GPIDX_TBL_INIT; // 4148
    UINT32                          rsv_414C[5];      // 414C..415C
    REG_G_MBI_STOP                  G_MBI_STOP;       // 4160
    UINT32                          rsv_4164[7];      // 4164..417C
    REG_G_TCONT_ID_16_31_CFG        G_TCONT_ID_16_31_CFG; // 4180
    REG_G_TCONT_ID_16_31_STS        G_TCONT_ID_16_31_STS; // 4184
    UINT32                          rsv_4188[30];     // 4188..41FC
    REG_DBG_CAP_SETTING             DBG_CAP_SETTING;  // 4200
    REG_DBG_US_PREF_OFS             DBG_US_PREF_OFS;  // 4204
    REG_DBG_DLY                     DBG_DLY;          // 4208
    REG_DBG_IDLE_GEM_THLD           DBG_IDLE_GEM_THLD; // 420C
    REG_DBG_US_NO_MSG0              DBG_US_NO_MSG0;   // 4210
    REG_DBG_US_NO_MSG1              DBG_US_NO_MSG1;   // 4214
    REG_DBG_US_NO_MSG2              DBG_US_NO_MSG2;   // 4218
    REG_DBG_US_DYING_GASP_CTRL      DBG_US_DYING_GASP_CTRL; // 421C
    REG_DBG_BWM_FILTER_CTRL         DBG_BWM_FILTER_CTRL; // 4220
    REG_DBG_BWM_SFIFO_STS           DBG_BWM_SFIFO_STS; // 4224
    REG_DBG_GRP_0                   DBG_GRP_0;        // 4228
    REG_DBG_GRP_1                   DBG_GRP_1;        // 422C
    REG_DBG_TXFIFO_MAX_USED         DBG_TXFIFO_MAX_USED; // 4230
    REG_DBG_BWM_GNT_STS             DBG_BWM_GNT_STS;  // 4234
    UINT32                          rsv_4238[2];      // 4238..423C
    REG_DBG_PROBE_CTRL              DBG_PROBE_CTRL;   // 4240
    REG_DBG_PROBE_HIGH32            DBG_PROBE_HIGH32; // 4244
    REG_DBG_PROBE_LOW32             DBG_PROBE_LOW32;  // 4248
    UINT32                          rsv_424C;         // 424C
    REG_DBG_BWM_BFIFO_STS           DBG_BWM_BFIFO_STS; // 4250
    UINT32                          rsv_4254[3];      // 4254..425C
    REG_DBG_ERR_CTRL                DBG_ERR_CTRL;     // 4260
    REG_DBG_SW_RDI_CTRL             DBG_SW_RDI_CTRL;  // 4264
    UINT32                          rsv_4268[38];     // 4268..42FC
    REG_DBG_RX_GEM_CNT              DBG_RX_GEM_CNT;   // 4300
    REG_DBG_RX_CRC_ERR_CNT          DBG_RX_CRC_ERR_CNT; // 4304
    REG_DBG_RX_GTC_CNT              DBG_RX_GTC_CNT;   // 4308
    REG_DBG_TX_GEM_CNT              DBG_TX_GEM_CNT;   // 430C
    REG_DBG_TX_BST_CNT              DBG_TX_BST_CNT;   // 4310
    UINT32                          rsv_4314[7];      // 4314..432C
    REG_DBG_GEM_HEC_ONE_ERR_CNT     DBG_GEM_HEC_ONE_ERR_CNT; // 4330
    REG_DBG_GEM_HEC_TWO_ERR_CNT     DBG_GEM_HEC_TWO_ERR_CNT; // 4334
    REG_DBG_GEM_HEC_UC_ERR_CNT      DBG_GEM_HEC_UC_ERR_CNT; // 4338
    UINT32                          rsv_433C;         // 433C
    REG_DBG_DBA_BACK_DOOR           DBG_DBA_BACK_DOOR; // 4340
    REG_DBG_DBA_TOTAL_BUF_SIZE      DBG_DBA_TOTAL_BUF_SIZE; // 4344
    REG_DBG_DBA_GREEN_SIZE          DBG_DBA_GREEN_SIZE; // 4348
    REG_DBG_DBA_YELLOW_SIZE         DBG_DBA_YELLOW_SIZE; // 434C
    REG_DBG_SLIGHT_MODIFY_SIZE_1    DBG_SLIGHT_MODIFY_SIZE_1; // 4350
    REG_DBG_SLIGHT_MODIFY_SIZE_2    DBG_SLIGHT_MODIFY_SIZE_2; // 4354
    REG_DBG_DS_SPF_CNT              DBG_DS_SPF_CNT;   // 4358
    REG_DBG_TX_SYNC_OFFSET          DBG_TX_SYNC_OFFSET; // 435C
    REG_DBG_PLOAMD_FILTER_IN_O5     DBG_PLOAMD_FILTER_IN_O5; // 4360
    REG_DBG_SW_RESYNC               DBG_SW_RESYNC;    // 4364
    REG_DBG_GTC_ETH_EXTR            DBG_GTC_ETH_EXTR; // 4368
    REG_DBG_DS_GTC_EXTR_ETH_HDR     DBG_DS_GTC_EXTR_ETH_HDR; // 436C
    REG_DBG_US_GTC_EXTR_ETH_HDR     DBG_US_GTC_EXTR_ETH_HDR; // 4370
    REG_DBG_DS_GTC_EXTR_ETH_CNT     DBG_DS_GTC_EXTR_ETH_CNT; // 4374
    REG_DBG_US_GTC_EXTR_ETH_CNT     DBG_US_GTC_EXTR_ETH_CNT; // 4378
    REG_SNIFF_GTC_GTC_INVLD_GEM_BYTE SNIFF_GTC_GTC_INVLD_GEM_BYTE; // 437C
    REG_DBG_SNIFF_DBG_REG           DBG_SNIFF_DBG_REG; // 4380
    REG_DBG_SNIFF_US_INFO_FIFO      DBG_SNIFF_US_INFO_FIFO; // 4384
    REG_SNIFF_US_HDR_WIDO           SNIFF_US_HDR_WIDO; // 4388
    REG_SNIFF_TX_DA_SA              SNIFF_TX_DA_SA;   // 438C
    REG_SNIFF_RX_DA_SA              SNIFF_RX_DA_SA;   // 4390
    REG_SNIFF_TX_RX_PID             SNIFF_TX_RX_PID;  // 4394
    REG_SNIFF_CNT_EN                SNIFF_CNT_EN;     // 4398
    REG_SNIFF_RX_TX_TPID            SNIFF_RX_TX_TPID; // 439C
    REG_DBG_GPON_MAC_SET            DBG_GPON_MAC_SET; // 43A0
    REG_SLEEP_GLB_CFG               SLEEP_GLB_CFG;    // 43A4
    REG_SLEEP_CNT                   SLEEP_CNT;        // 43A8
    UINT32                          rsv_43AC[5];      // 43AC..43BC
    REG_TX_SYNC_OPT                 TX_SYNC_OPT;      // 43C0
    REG_PLOAMU_CTRL                 PLOAMU_CTRL;      // 43C4
    REG_TOD_1PPS_WD_CTRL            TOD_1PPS_WD_CTRL; // 43C8
}gpon_mac_reg_REGS, *Pgpon_mac_reg_REGS;

// ---------- gpon_mac_reg Enum Definitions      ----------
// ---------- gpon_mac_reg C Macro Definitions   ----------
extern Pgpon_mac_reg_REGS g_gpon_mac_reg_BASE;

#define gpon_mac_reg_BASE                                      (g_gpon_mac_reg_BASE)

#define G_ONU_ID                                               INREG32(&gpon_mac_reg_BASE->G_ONU_ID) // 4000
#define G_GBL_CFG                                              INREG32(&gpon_mac_reg_BASE->G_GBL_CFG) // 4004
#define G_INT_STATUS                                           INREG32(&gpon_mac_reg_BASE->G_INT_STATUS) // 4008
#define G_INT_ENABLE                                           INREG32(&gpon_mac_reg_BASE->G_INT_ENABLE) // 400C
#define G_TCONT_ID_0_1                                         INREG32(&gpon_mac_reg_BASE->G_TCONT_ID_0_1) // 4020
#define G_TCONT_ID_2_3                                         INREG32(&gpon_mac_reg_BASE->G_TCONT_ID_2_3) // 4024
#define G_TCONT_ID_4_5                                         INREG32(&gpon_mac_reg_BASE->G_TCONT_ID_4_5) // 4028
#define G_TCONT_ID_6_7                                         INREG32(&gpon_mac_reg_BASE->G_TCONT_ID_6_7) // 402C
#define G_TCONT_ID_8_9                                         INREG32(&gpon_mac_reg_BASE->G_TCONT_ID_8_9) // 4030
#define G_TCONT_ID_10_11                                       INREG32(&gpon_mac_reg_BASE->G_TCONT_ID_10_11) // 4034
#define G_TCONT_ID_12_13                                       INREG32(&gpon_mac_reg_BASE->G_TCONT_ID_12_13) // 4038
#define G_TCONT_ID_14_15                                       INREG32(&gpon_mac_reg_BASE->G_TCONT_ID_14_15) // 403C
#define G_GEM_PORT_CFG                                         INREG32(&gpon_mac_reg_BASE->G_GEM_PORT_CFG) // 4040
#define G_GEM_PORT_STS                                         INREG32(&gpon_mac_reg_BASE->G_GEM_PORT_STS) // 4044
#define G_OMCI_ID                                              INREG32(&gpon_mac_reg_BASE->G_OMCI_ID) // 4048
#define G_GEM_TBL_INIT                                         INREG32(&gpon_mac_reg_BASE->G_GEM_TBL_INIT) // 404C
#define G_PLOAMu_FIFO_STS                                      INREG32(&gpon_mac_reg_BASE->G_PLOAMu_FIFO_STS) // 4050
#define G_PLOAMu_WDATA                                         INREG32(&gpon_mac_reg_BASE->G_PLOAMu_WDATA) // 4054
#define G_PLOAMd_FIFO_STS                                      INREG32(&gpon_mac_reg_BASE->G_PLOAMd_FIFO_STS) // 4058
#define G_PLOAMd_RDATA                                         INREG32(&gpon_mac_reg_BASE->G_PLOAMd_RDATA) // 405C
#define G_AES_CFG                                              INREG32(&gpon_mac_reg_BASE->G_AES_CFG) // 4060
#define G_AES_ACTIVE_KEY0                                      INREG32(&gpon_mac_reg_BASE->G_AES_ACTIVE_KEY0) // 4064
#define G_AES_ACTIVE_KEY1                                      INREG32(&gpon_mac_reg_BASE->G_AES_ACTIVE_KEY1) // 4068
#define G_AES_ACTIVE_KEY2                                      INREG32(&gpon_mac_reg_BASE->G_AES_ACTIVE_KEY2) // 406C
#define G_AES_ACTIVE_KEY3                                      INREG32(&gpon_mac_reg_BASE->G_AES_ACTIVE_KEY3) // 4070
#define G_AES_SHADOW_KEY0                                      INREG32(&gpon_mac_reg_BASE->G_AES_SHADOW_KEY0) // 4074
#define G_AES_SHADOW_KEY1                                      INREG32(&gpon_mac_reg_BASE->G_AES_SHADOW_KEY1) // 4078
#define G_AES_SHADOW_KEY2                                      INREG32(&gpon_mac_reg_BASE->G_AES_SHADOW_KEY2) // 407C
#define G_AES_SHADOW_KEY3                                      INREG32(&gpon_mac_reg_BASE->G_AES_SHADOW_KEY3) // 4080
#define G_AES_KEY_SWITCH_BY_SW                                 INREG32(&gpon_mac_reg_BASE->G_AES_KEY_SWITCH_BY_SW) // 4084
#define G_PLOu_OVERHEAD                                        INREG32(&gpon_mac_reg_BASE->G_PLOu_OVERHEAD) // 4090
#define G_PLOu_GUARD_BIT                                       INREG32(&gpon_mac_reg_BASE->G_PLOu_GUARD_BIT) // 4094
#define G_PLOu_PRMBL_TYPE1_2                                   INREG32(&gpon_mac_reg_BASE->G_PLOu_PRMBL_TYPE1_2) // 4098
#define G_PLOu_PRMBL_TYPE3                                     INREG32(&gpon_mac_reg_BASE->G_PLOu_PRMBL_TYPE3) // 409C
#define G_PLOu_DELM_BIT                                        INREG32(&gpon_mac_reg_BASE->G_PLOu_DELM_BIT) // 40A0
#define G_PRE_ASSIGNED_DLY                                     INREG32(&gpon_mac_reg_BASE->G_PRE_ASSIGNED_DLY) // 40A4
#define G_EQD                                                  INREG32(&gpon_mac_reg_BASE->G_EQD) // 40A8
#define G_RSP_TIME                                             INREG32(&gpon_mac_reg_BASE->G_RSP_TIME) // 40AC
#define G_VENDOR_ID                                            INREG32(&gpon_mac_reg_BASE->G_VENDOR_ID) // 40B0
#define G_VS_SN                                                INREG32(&gpon_mac_reg_BASE->G_VS_SN) // 40B4
#define G_SN_MSG_CFG                                           INREG32(&gpon_mac_reg_BASE->G_SN_MSG_CFG) // 40B8
#define G_ACTIVATION_ST                                        INREG32(&gpon_mac_reg_BASE->G_ACTIVATION_ST) // 40BC
#define G_TOD_CFG                                              INREG32(&gpon_mac_reg_BASE->G_TOD_CFG) // 40D0
#define G_NEW_TOD_SEC_L32                                      INREG32(&gpon_mac_reg_BASE->G_NEW_TOD_SEC_L32) // 40D4
#define G_NEW_TOD_NANO_SEC                                     INREG32(&gpon_mac_reg_BASE->G_NEW_TOD_NANO_SEC) // 40D8
#define G_CUR_TOD_SEC_L32                                      INREG32(&gpon_mac_reg_BASE->G_CUR_TOD_SEC_L32) // 40DC
#define G_CUR_TOD_NANO_SEC                                     INREG32(&gpon_mac_reg_BASE->G_CUR_TOD_NANO_SEC) // 40E0
#define G_TOD_CLK_PERIOD                                       INREG32(&gpon_mac_reg_BASE->G_TOD_CLK_PERIOD) // 40E4
#define G_TX_FCS_TBL_INIT                                      INREG32(&gpon_mac_reg_BASE->G_TX_FCS_TBL_INIT) // 4100
#define G_MIB_CTRL_STS                                         INREG32(&gpon_mac_reg_BASE->G_MIB_CTRL_STS) // 4120
#define G_MIB_RDATA_L32                                        INREG32(&gpon_mac_reg_BASE->G_MIB_RDATA_L32) // 4124
#define G_MIB_RDATA_H32                                        INREG32(&gpon_mac_reg_BASE->G_MIB_RDATA_H32) // 4128
#define G_MIB_WDATA_L32                                        INREG32(&gpon_mac_reg_BASE->G_MIB_WDATA_L32) // 412C
#define G_MIB_WDATA_H32                                        INREG32(&gpon_mac_reg_BASE->G_MIB_WDATA_H32) // 4130
#define G_MIB_TBL_INIT                                         INREG32(&gpon_mac_reg_BASE->G_MIB_TBL_INIT) // 4134
#define G_GPIDX_TBL_CTRL                                       INREG32(&gpon_mac_reg_BASE->G_GPIDX_TBL_CTRL) // 4140
#define G_GPIDX_TBL_STS                                        INREG32(&gpon_mac_reg_BASE->G_GPIDX_TBL_STS) // 4144
#define G_GPIDX_TBL_INIT                                       INREG32(&gpon_mac_reg_BASE->G_GPIDX_TBL_INIT) // 4148
#define G_MBI_STOP                                             INREG32(&gpon_mac_reg_BASE->G_MBI_STOP) // 4160
#define G_TCONT_ID_16_31_CFG                                   INREG32(&gpon_mac_reg_BASE->G_TCONT_ID_16_31_CFG) // 4180
#define G_TCONT_ID_16_31_STS                                   INREG32(&gpon_mac_reg_BASE->G_TCONT_ID_16_31_STS) // 4184
#define DBG_CAP_SETTING                                        INREG32(&gpon_mac_reg_BASE->DBG_CAP_SETTING) // 4200
#define DBG_US_PREF_OFS                                        INREG32(&gpon_mac_reg_BASE->DBG_US_PREF_OFS) // 4204
#define DBG_DLY                                                INREG32(&gpon_mac_reg_BASE->DBG_DLY) // 4208
#define DBG_IDLE_GEM_THLD                                      INREG32(&gpon_mac_reg_BASE->DBG_IDLE_GEM_THLD) // 420C
#define DBG_US_NO_MSG0                                         INREG32(&gpon_mac_reg_BASE->DBG_US_NO_MSG0) // 4210
#define DBG_US_NO_MSG1                                         INREG32(&gpon_mac_reg_BASE->DBG_US_NO_MSG1) // 4214
#define DBG_US_NO_MSG2                                         INREG32(&gpon_mac_reg_BASE->DBG_US_NO_MSG2) // 4218
#define DBG_US_DYING_GASP_CTRL                                 INREG32(&gpon_mac_reg_BASE->DBG_US_DYING_GASP_CTRL) // 421C
#define DBG_BWM_FILTER_CTRL                                    INREG32(&gpon_mac_reg_BASE->DBG_BWM_FILTER_CTRL) // 4220
#define DBG_BWM_SFIFO_STS                                      INREG32(&gpon_mac_reg_BASE->DBG_BWM_SFIFO_STS) // 4224
#define DBG_GRP_0                                              INREG32(&gpon_mac_reg_BASE->DBG_GRP_0) // 4228
#define DBG_GRP_1                                              INREG32(&gpon_mac_reg_BASE->DBG_GRP_1) // 422C
#define DBG_TXFIFO_MAX_USED                                    INREG32(&gpon_mac_reg_BASE->DBG_TXFIFO_MAX_USED) // 4230
#define DBG_BWM_GNT_STS                                        INREG32(&gpon_mac_reg_BASE->DBG_BWM_GNT_STS) // 4234
#define DBG_PROBE_CTRL                                         INREG32(&gpon_mac_reg_BASE->DBG_PROBE_CTRL) // 4240
#define DBG_PROBE_HIGH32                                       INREG32(&gpon_mac_reg_BASE->DBG_PROBE_HIGH32) // 4244
#define DBG_PROBE_LOW32                                        INREG32(&gpon_mac_reg_BASE->DBG_PROBE_LOW32) // 4248
#define DBG_BWM_BFIFO_STS                                      INREG32(&gpon_mac_reg_BASE->DBG_BWM_BFIFO_STS) // 4250
#define DBG_ERR_CTRL                                           INREG32(&gpon_mac_reg_BASE->DBG_ERR_CTRL) // 4260
#define DBG_SW_RDI_CTRL                                        INREG32(&gpon_mac_reg_BASE->DBG_SW_RDI_CTRL) // 4264
#define DBG_RX_GEM_CNT                                         INREG32(&gpon_mac_reg_BASE->DBG_RX_GEM_CNT) // 4300
#define DBG_RX_CRC_ERR_CNT                                     INREG32(&gpon_mac_reg_BASE->DBG_RX_CRC_ERR_CNT) // 4304
#define DBG_RX_GTC_CNT                                         INREG32(&gpon_mac_reg_BASE->DBG_RX_GTC_CNT) // 4308
#define DBG_TX_GEM_CNT                                         INREG32(&gpon_mac_reg_BASE->DBG_TX_GEM_CNT) // 430C
#define DBG_TX_BST_CNT                                         INREG32(&gpon_mac_reg_BASE->DBG_TX_BST_CNT) // 4310
#define DBG_GEM_HEC_ONE_ERR_CNT                                INREG32(&gpon_mac_reg_BASE->DBG_GEM_HEC_ONE_ERR_CNT) // 4330
#define DBG_GEM_HEC_TWO_ERR_CNT                                INREG32(&gpon_mac_reg_BASE->DBG_GEM_HEC_TWO_ERR_CNT) // 4334
#define DBG_GEM_HEC_UC_ERR_CNT                                 INREG32(&gpon_mac_reg_BASE->DBG_GEM_HEC_UC_ERR_CNT) // 4338
#define DBG_DBA_BACK_DOOR                                      INREG32(&gpon_mac_reg_BASE->DBG_DBA_BACK_DOOR) // 4340
#define DBG_DBA_TOTAL_BUF_SIZE                                 INREG32(&gpon_mac_reg_BASE->DBG_DBA_TOTAL_BUF_SIZE) // 4344
#define DBG_DBA_GREEN_SIZE                                     INREG32(&gpon_mac_reg_BASE->DBG_DBA_GREEN_SIZE) // 4348
#define DBG_DBA_YELLOW_SIZE                                    INREG32(&gpon_mac_reg_BASE->DBG_DBA_YELLOW_SIZE) // 434C
#define DBG_SLIGHT_MODIFY_SIZE_1                               INREG32(&gpon_mac_reg_BASE->DBG_SLIGHT_MODIFY_SIZE_1) // 4350
#define DBG_SLIGHT_MODIFY_SIZE_2                               INREG32(&gpon_mac_reg_BASE->DBG_SLIGHT_MODIFY_SIZE_2) // 4354
#define DBG_DS_SPF_CNT                                         INREG32(&gpon_mac_reg_BASE->DBG_DS_SPF_CNT) // 4358
#define DBG_TX_SYNC_OFFSET                                     INREG32(&gpon_mac_reg_BASE->DBG_TX_SYNC_OFFSET) // 435C
#define DBG_PLOAMD_FILTER_IN_O5                                INREG32(&gpon_mac_reg_BASE->DBG_PLOAMD_FILTER_IN_O5) // 4360
#define DBG_SW_RESYNC                                          INREG32(&gpon_mac_reg_BASE->DBG_SW_RESYNC) // 4364
#define DBG_GTC_ETH_EXTR                                       INREG32(&gpon_mac_reg_BASE->DBG_GTC_ETH_EXTR) // 4368
#define DBG_DS_GTC_EXTR_ETH_HDR                                INREG32(&gpon_mac_reg_BASE->DBG_DS_GTC_EXTR_ETH_HDR) // 436C
#define DBG_US_GTC_EXTR_ETH_HDR                                INREG32(&gpon_mac_reg_BASE->DBG_US_GTC_EXTR_ETH_HDR) // 4370
#define DBG_DS_GTC_EXTR_ETH_CNT                                INREG32(&gpon_mac_reg_BASE->DBG_DS_GTC_EXTR_ETH_CNT) // 4374
#define DBG_US_GTC_EXTR_ETH_CNT                                INREG32(&gpon_mac_reg_BASE->DBG_US_GTC_EXTR_ETH_CNT) // 4378
#define SNIFF_GTC_GTC_INVLD_GEM_BYTE                           INREG32(&gpon_mac_reg_BASE->SNIFF_GTC_GTC_INVLD_GEM_BYTE) // 437C
#define DBG_SNIFF_DBG_REG                                      INREG32(&gpon_mac_reg_BASE->DBG_SNIFF_DBG_REG) // 4380
#define DBG_SNIFF_US_INFO_FIFO                                 INREG32(&gpon_mac_reg_BASE->DBG_SNIFF_US_INFO_FIFO) // 4384
#define SNIFF_US_HDR_WIDO                                      INREG32(&gpon_mac_reg_BASE->SNIFF_US_HDR_WIDO) // 4388
#define SNIFF_TX_DA_SA                                         INREG32(&gpon_mac_reg_BASE->SNIFF_TX_DA_SA) // 438C
#define SNIFF_RX_DA_SA                                         INREG32(&gpon_mac_reg_BASE->SNIFF_RX_DA_SA) // 4390
#define SNIFF_TX_RX_PID                                        INREG32(&gpon_mac_reg_BASE->SNIFF_TX_RX_PID) // 4394
#define SNIFF_CNT_EN                                           INREG32(&gpon_mac_reg_BASE->SNIFF_CNT_EN) // 4398
#define SNIFF_RX_TX_TPID                                       INREG32(&gpon_mac_reg_BASE->SNIFF_RX_TX_TPID) // 439C
#define DBG_GPON_MAC_SET                                       INREG32(&gpon_mac_reg_BASE->DBG_GPON_MAC_SET) // 43A0
#define SLEEP_GLB_CFG                                          INREG32(&gpon_mac_reg_BASE->SLEEP_GLB_CFG) // 43A4
#define SLEEP_CNT                                              INREG32(&gpon_mac_reg_BASE->SLEEP_CNT) // 43A8
#define TX_SYNC_OPT                                            INREG32(&gpon_mac_reg_BASE->TX_SYNC_OPT) // 43C0
//#define PLOAMU_CTRL                                            INREG32(&gpon_mac_reg_BASE->PLOAMU_CTRL) // 43C4
#define O3_O4_PLOAMU_CTRL                                      INREG32(&gpon_mac_reg_BASE->O3_O4_PLOAMU_CTRL) // 43C4
#define TOD_1PPS_WD_CTRL                                       INREG32(&gpon_mac_reg_BASE->TOD_1PPS_WD_CTRL) // 43C8

#endif


#define G_ONU_ID_FLD_onu_id_vld                                REG_FLD(1, 15)
#define G_ONU_ID_FLD_onu_id                                    REG_FLD(8, 0)

#define G_GBL_CFG_FLD_us_fec_en                                REG_FLD(1, 16)
#define G_GBL_CFG_FLD_sr_blk_size                              REG_FLD(8, 0)

#define G_INT_STATUS_FLD_bwm_us_fec_err_int                    REG_FLD(1, 31)
#define G_INT_STATUS_FLD_bwm_stop_time_err_int                 REG_FLD(1, 30)
#define G_INT_STATUS_FLD_lwi_int                               REG_FLD(1, 29)
#define G_INT_STATUS_FLD_fwi_int                               REG_FLD(1, 28)
#define G_INT_STATUS_FLD_pop_up_recv_in_O6_int                 REG_FLD(1, 27)
#define G_INT_STATUS_FLD_onu_us_fec_chg_int                    REG_FLD(1, 26)
#define G_INT_STATUS_FLD_olt_ds_fec_chg_int                    REG_FLD(1, 25)
#define G_INT_STATUS_FLD_o5_eqd_adj_done_int                   REG_FLD(1, 24)
#define G_INT_STATUS_FLD_sfifo_full_int                        REG_FLD(1, 23)
#define G_INT_STATUS_FLD_bfifo_full_int                        REG_FLD(1, 22)
#define G_INT_STATUS_FLD_rx_gem_intlv_err_int                  REG_FLD(1, 21)
#define G_INT_STATUS_FLD_rx_eof_err_int                        REG_FLD(1, 20)
#define G_INT_STATUS_FLD_tx_late_start_int                     REG_FLD(1, 19)
#define G_INT_STATUS_FLD_bst_sgl_diff_int                      REG_FLD(1, 18)
#define G_INT_STATUS_FLD_fifo_err_int                          REG_FLD(1, 17)
#define G_INT_STATUS_FLD_rx_err_int                            REG_FLD(1, 16)
#define G_INT_STATUS_FLD_dying_gasp_send_int                   REG_FLD(1, 11)
#define G_INT_STATUS_FLD_tod_1pps_int                          REG_FLD(1, 10)
#define G_INT_STATUS_FLD_tod_update_done_int                   REG_FLD(1, 9)
#define G_INT_STATUS_FLD_aes_key_switch_done_int               REG_FLD(1, 8)
#define G_INT_STATUS_FLD_los_gem_del_int                       REG_FLD(1, 7)
#define G_INT_STATUS_FLD_sn_req_crs_int                        REG_FLD(1, 6)
#define G_INT_STATUS_FLD_sn_onu_send_o4_int                    REG_FLD(1, 5)
#define G_INT_STATUS_FLD_ranging_req_recv_int                  REG_FLD(1, 4)
#define G_INT_STATUS_FLD_sn_onu_send_o3_int                    REG_FLD(1, 3)
#define G_INT_STATUS_FLD_sn_req_recv_int                       REG_FLD(1, 2)
#define G_INT_STATUS_FLD_ploamu_send_int                       REG_FLD(1, 1)
#define G_INT_STATUS_FLD_ploamd_recv_int                       REG_FLD(1, 0)

#define G_INT_ENABLE_FLD_bwm_us_fec_err_int_en                 REG_FLD(1, 31)
#define G_INT_ENABLE_FLD_bwm_stop_time_err_int_en              REG_FLD(1, 30)
#define G_INT_ENABLE_FLD_lwi_int_en                            REG_FLD(1, 29)
#define G_INT_ENABLE_FLD_fwi_int_en                            REG_FLD(1, 28)
#define G_INT_ENABLE_FLD_pop_up_recv_in_O6_int_en              REG_FLD(1, 27)
#define G_INT_ENABLE_FLD_onu_us_fec_chg_int_en                 REG_FLD(1, 26)
#define G_INT_ENABLE_FLD_olt_ds_fec_chg_int_en                 REG_FLD(1, 25)
#define G_INT_ENABLE_FLD_o5_eqd_adj_done_int_en                REG_FLD(1, 24)
#define G_INT_ENABLE_FLD_sfifo_full_int_en                     REG_FLD(1, 23)
#define G_INT_ENABLE_FLD_bfifo_full_int_en                     REG_FLD(1, 22)
#define G_INT_ENABLE_FLD_rx_gem_intlv_err_int_en               REG_FLD(1, 21)
#define G_INT_ENABLE_FLD_rx_eof_err_int_en                     REG_FLD(1, 20)
#define G_INT_ENABLE_FLD_tx_late_start_int_en                  REG_FLD(1, 19)
#define G_INT_ENABLE_FLD_bst_sgl_diff_int_en                   REG_FLD(1, 18)
#define G_INT_ENABLE_FLD_fifo_err_int_en                       REG_FLD(1, 17)
#define G_INT_ENABLE_FLD_rx_err_int_en                         REG_FLD(1, 16)
#define G_INT_ENABLE_FLD_dying_gasp_send_int_en                REG_FLD(1, 11)
#define G_INT_ENABLE_FLD_tod_1pps_int_en                       REG_FLD(1, 10)
#define G_INT_ENABLE_FLD_tod_update_done_int_en                REG_FLD(1, 9)
#define G_INT_ENABLE_FLD_aes_key_switch_done_int_en            REG_FLD(1, 8)
#define G_INT_ENABLE_FLD_los_gem_del_int_en                    REG_FLD(1, 7)
#define G_INT_ENABLE_FLD_sn_req_crs_int_en                     REG_FLD(1, 6)
#define G_INT_ENABLE_FLD_sn_onu_send_o4_int_en                 REG_FLD(1, 5)
#define G_INT_ENABLE_FLD_ranging_req_recv_int_en               REG_FLD(1, 4)
#define G_INT_ENABLE_FLD_sn_onu_send_o3_int_en                 REG_FLD(1, 3)
#define G_INT_ENABLE_FLD_sn_req_recv_int_en                    REG_FLD(1, 2)
#define G_INT_ENABLE_FLD_ploamu_send_int_en                    REG_FLD(1, 1)
#define G_INT_ENABLE_FLD_ploamd_recv_int_en                    REG_FLD(1, 0)

#define G_TCONT_ID_0_1_FLD_t_cont1_vld                         REG_FLD(1, 31)
#define G_TCONT_ID_0_1_FLD_t_cont1_id                          REG_FLD(12, 16)
#define G_TCONT_ID_0_1_FLD_t_cont0_vld                         REG_FLD(1, 15)
#define G_TCONT_ID_0_1_FLD_t_cont0_id                          REG_FLD(12, 0)

#define G_TCONT_ID_2_3_FLD_t_cont3_vld                         REG_FLD(1, 31)
#define G_TCONT_ID_2_3_FLD_t_cont3_id                          REG_FLD(12, 16)
#define G_TCONT_ID_2_3_FLD_t_cont2_vld                         REG_FLD(1, 15)
#define G_TCONT_ID_2_3_FLD_t_cont2_id                          REG_FLD(12, 0)

#define G_TCONT_ID_4_5_FLD_t_cont5_vld                         REG_FLD(1, 31)
#define G_TCONT_ID_4_5_FLD_t_cont5_id                          REG_FLD(12, 16)
#define G_TCONT_ID_4_5_FLD_t_cont4_vld                         REG_FLD(1, 15)
#define G_TCONT_ID_4_5_FLD_t_cont4_id                          REG_FLD(12, 0)

#define G_TCONT_ID_6_7_FLD_t_cont7_vld                         REG_FLD(1, 31)
#define G_TCONT_ID_6_7_FLD_t_cont7_id                          REG_FLD(12, 16)
#define G_TCONT_ID_6_7_FLD_t_cont6_vld                         REG_FLD(1, 15)
#define G_TCONT_ID_6_7_FLD_t_cont6_id                          REG_FLD(12, 0)

#define G_TCONT_ID_8_9_FLD_t_cont9_vld                         REG_FLD(1, 31)
#define G_TCONT_ID_8_9_FLD_t_cont9_id                          REG_FLD(12, 16)
#define G_TCONT_ID_8_9_FLD_t_cont8_vld                         REG_FLD(1, 15)
#define G_TCONT_ID_8_9_FLD_t_cont8_id                          REG_FLD(12, 0)

#define G_TCONT_ID_10_11_FLD_t_cont11_vld                      REG_FLD(1, 31)
#define G_TCONT_ID_10_11_FLD_t_cont11_id                       REG_FLD(12, 16)
#define G_TCONT_ID_10_11_FLD_t_cont10_vld                      REG_FLD(1, 15)
#define G_TCONT_ID_10_11_FLD_t_cont10_id                       REG_FLD(12, 0)

#define G_TCONT_ID_12_13_FLD_t_cont13_vld                      REG_FLD(1, 31)
#define G_TCONT_ID_12_13_FLD_t_cont13_id                       REG_FLD(12, 16)
#define G_TCONT_ID_12_13_FLD_t_cont12_vld                      REG_FLD(1, 15)
#define G_TCONT_ID_12_13_FLD_t_cont12_id                       REG_FLD(12, 0)

#define G_TCONT_ID_14_15_FLD_t_cont15_vld                      REG_FLD(1, 31)
#define G_TCONT_ID_14_15_FLD_t_cont15_id                       REG_FLD(12, 16)
#define G_TCONT_ID_14_15_FLD_t_cont14_vld                      REG_FLD(1, 15)
#define G_TCONT_ID_14_15_FLD_t_cont14_id                       REG_FLD(12, 0)

#define G_GEM_PORT_CFG_FLD_gpid_cmd                            REG_FLD(1, 31)
#define G_GEM_PORT_CFG_FLD_gpid_encrypt                        REG_FLD(1, 17)
#define G_GEM_PORT_CFG_FLD_gpid_vld                            REG_FLD(1, 16)
#define G_GEM_PORT_CFG_FLD_gem_port_id                         REG_FLD(12, 0)

#define G_GEM_PORT_STS_FLD_gpid_cmd_done                       REG_FLD(1, 31)
#define G_GEM_PORT_STS_FLD_gpid_encrypt_sts                    REG_FLD(1, 1)
#define G_GEM_PORT_STS_FLD_gpid_vld_sts                        REG_FLD(1, 0)

#define G_OMCI_ID_FLD_omci_port_id_vld                         REG_FLD(1, 16)
#define G_OMCI_ID_FLD_omci_gpid                                REG_FLD(12, 0)

#define G_GEM_TBL_INIT_FLD_gem_tbl_init_done                   REG_FLD(1, 8)
#define G_GEM_TBL_INIT_FLD_gem_tbl_init_start                  REG_FLD(1, 0)

#define G_PLOAMu_FIFO_STS_FLD_ploamu_fifo_udrn                 REG_FLD(1, 31)
#define G_PLOAMu_FIFO_STS_FLD_ploamu_fifo_min_avail            REG_FLD(8, 16)
#define G_PLOAMu_FIFO_STS_FLD_ploamu_fifo_avail                REG_FLD(8, 0)

#define G_PLOAMu_WDATA_FLD_ploamu_wdata                        REG_FLD(32, 0)

#define G_PLOAMd_FIFO_STS_FLD_ploamd_fifo_ovrn                 REG_FLD(1, 31)
#define G_PLOAMd_FIFO_STS_FLD_ploamd_fifo_max_used             REG_FLD(8, 16)
#define G_PLOAMd_FIFO_STS_FLD_ploamd_fifo_used                 REG_FLD(8, 0)

#define G_PLOAMd_RDATA_FLD_ploamd_rdata                        REG_FLD(32, 0)

#define G_AES_CFG_FLD_aes_spf_cnt                              REG_FLD(30, 0)

#define G_AES_ACTIVE_KEY0_FLD_aes_active_key0                  REG_FLD(32, 0)

#define G_AES_ACTIVE_KEY1_FLD_aes_active_key1                  REG_FLD(32, 0)

#define G_AES_ACTIVE_KEY2_FLD_aes_active_key2                  REG_FLD(32, 0)

#define G_AES_ACTIVE_KEY3_FLD_aes_active_key3                  REG_FLD(32, 0)

#define G_AES_SHADOW_KEY0_FLD_aes_shadow_key0                  REG_FLD(32, 0)

#define G_AES_SHADOW_KEY1_FLD_aes_shadow_key1                  REG_FLD(32, 0)

#define G_AES_SHADOW_KEY2_FLD_aes_shadow_key2                  REG_FLD(32, 0)

#define G_AES_SHADOW_KEY3_FLD_aes_shadow_key3                  REG_FLD(32, 0)

#define G_AES_KEY_SWITCH_BY_SW_FLD_aes_key_switch_by_sw_done   REG_FLD(1, 31)
#define G_AES_KEY_SWITCH_BY_SW_FLD_aes_key_switch_by_sw_cmd    REG_FLD(1, 0)

#define G_PLOu_OVERHEAD_FLD_plou_overhead                      REG_FLD(8, 0)

#define G_PLOu_GUARD_BIT_FLD_guard_bit                         REG_FLD(8, 0)

#define G_PLOu_PRMBL_TYPE1_2_FLD_prmb2_bit                     REG_FLD(8, 8)
#define G_PLOu_PRMBL_TYPE1_2_FLD_prmb1_bit                     REG_FLD(8, 0)

#define G_PLOu_PRMBL_TYPE3_FLD_ebl_en                          REG_FLD(1, 24)
#define G_PLOu_PRMBL_TYPE3_FLD_ext_prmb3_o5_num                REG_FLD(8, 8)
#define G_PLOu_PRMBL_TYPE3_FLD_ext_prmb3_o3_o4_num             REG_FLD(8, 0)

#define G_PLOu_DELM_BIT_FLD_delm_bit                           REG_FLD(8, 0)

#define G_PRE_ASSIGNED_DLY_FLD_pre_dly_en                      REG_FLD(1, 31)
#define G_PRE_ASSIGNED_DLY_FLD_pre_dly                         REG_FLD(16, 0)

#define G_EQD_FLD_eqd                                          REG_FLD(32, 0)

#define G_RSP_TIME_FLD_tresp                                   REG_FLD(16, 0)

#define G_VENDOR_ID_FLD_vendor_id                              REG_FLD(32, 0)

#define G_VS_SN_FLD_vs_sn                                      REG_FLD(32, 0)

#define G_SN_MSG_CFG_FLD_sn_req_thr                            REG_FLD(8, 24)
#define G_SN_MSG_CFG_FLD_tx_power_mode                         REG_FLD(2, 16)
#define G_SN_MSG_CFG_FLD_rdm_dly                               REG_FLD(12, 0)

#define G_ACTIVATION_ST_FLD_act_st                             REG_FLD(3, 0)

#define G_TOD_CFG_FLD_tod_spf_cnt                              REG_FLD(30, 0)

#define G_NEW_TOD_SEC_L32_FLD_new_tod_sec_l32                  REG_FLD(32, 0)

#define G_NEW_TOD_NANO_SEC_FLD_new_tod_nano_sec                REG_FLD(32, 0)

#define G_CUR_TOD_SEC_L32_FLD_cur_tod_sec_l32                  REG_FLD(32, 0)

#define G_CUR_TOD_NANO_SEC_FLD_cur_tod_nano_sec                REG_FLD(32, 0)

#define G_TOD_CLK_PERIOD_FLD_tod_period                        REG_FLD(8, 0)

#define G_TX_FCS_TBL_INIT_FLD_tx_fcs_tbl_init_done             REG_FLD(1, 8)
#define G_TX_FCS_TBL_INIT_FLD_tx_fcs_tbl_init_start            REG_FLD(1, 0)

#define G_MIB_CTRL_STS_FLD_mib_cmd                             REG_FLD(1, 31)
#define G_MIB_CTRL_STS_FLD_mib_cmd_done                        REG_FLD(1, 30)
#define G_MIB_CTRL_STS_FLD_mib_read_clr_en                     REG_FLD(1, 24)
#define G_MIB_CTRL_STS_FLD_mib_type                            REG_FLD(3, 16)
#define G_MIB_CTRL_STS_FLD_mib_gpid                            REG_FLD(12, 0)

#define G_MIB_RDATA_L32_FLD_mib_rdata_l32                      REG_FLD(32, 0)

#define G_MIB_RDATA_H32_FLD_mib_rdata_h32                      REG_FLD(32, 0)

#define G_MIB_WDATA_L32_FLD_mib_wdata_l32                      REG_FLD(32, 0)

#define G_MIB_WDATA_H32_FLD_mib_wdata_h32                      REG_FLD(32, 0)

#define G_MIB_TBL_INIT_FLD_mib_tbl_init_done                   REG_FLD(1, 8)
#define G_MIB_TBL_INIT_FLD_mib_tbl_init_start                  REG_FLD(1, 0)

#define G_GPIDX_TBL_CTRL_FLD_gpidx_cmd                         REG_FLD(1, 31)
#define G_GPIDX_TBL_CTRL_FLD_gpidx_tbl_addr                    REG_FLD(8, 16)
#define G_GPIDX_TBL_CTRL_FLD_gpidx_tbl_wdata                   REG_FLD(9, 0)

#define G_GPIDX_TBL_STS_FLD_gpidx_cmd_done                     REG_FLD(1, 31)
#define G_GPIDX_TBL_STS_FLD_gpidx_tbl_rdata                    REG_FLD(9, 0)

#define G_GPIDX_TBL_INIT_FLD_gpidx_tbl_init_done               REG_FLD(1, 8)
#define G_GPIDX_TBL_INIT_FLD_gpidx_tbl_init_start              REG_FLD(1, 0)

#define G_MBI_STOP_FLD_mbi_tx_stop                             REG_FLD(1, 8)
#define G_MBI_STOP_FLD_mbi_rx_stop                             REG_FLD(1, 0)

#define G_TCONT_ID_16_31_CFG_FLD_tcont_cmd                     REG_FLD(1, 31)
#define G_TCONT_ID_16_31_CFG_FLD_wr_tcont_id_vld               REG_FLD(1, 27)
#define G_TCONT_ID_16_31_CFG_FLD_tcont_id_index                REG_FLD(4, 16)
#define G_TCONT_ID_16_31_CFG_FLD_wr_tcont_id                   REG_FLD(12, 0)

#define G_TCONT_ID_16_31_STS_FLD_tcont_cmd_done                REG_FLD(1, 31)
#define G_TCONT_ID_16_31_STS_FLD_rd_tcont_id_vld               REG_FLD(1, 16)
#define G_TCONT_ID_16_31_STS_FLD_rd_tcont_id                   REG_FLD(12, 0)

#define DBG_CAP_SETTING_FLD_mib_frame_type                     REG_FLD(1, 26)
#define DBG_CAP_SETTING_FLD_gpon_mib_en                        REG_FLD(1, 25)
#define DBG_CAP_SETTING_FLD_us_no_msg_int_en                   REG_FLD(1, 24)
#define DBG_CAP_SETTING_FLD_rpt_msg_flt                        REG_FLD(1, 16)
#define DBG_CAP_SETTING_FLD_max_rdm_dly                        REG_FLD(12, 0)

#define DBG_US_PREF_OFS_FLD_sch_pref_ofs                       REG_FLD(16, 16)
#define DBG_US_PREF_OFS_FLD_dt_pref_ofs                        REG_FLD(16, 0)

#define DBG_DLY_FLD_phy_rx_dly_sel                             REG_FLD(1, 31)
#define DBG_DLY_FLD_fix_phy_rx_dly                             REG_FLD(12, 16)
#define DBG_DLY_FLD_fine_int_dly                               REG_FLD(8, 8)
#define DBG_DLY_FLD_phy_tx_dly                                 REG_FLD(8, 0)

#define DBG_IDLE_GEM_THLD_FLD_idle_gem_thld                    REG_FLD(16, 0)

#define DBG_US_NO_MSG0_FLD_us_no_msg_b3_b4                     REG_FLD(16, 16)
#define DBG_US_NO_MSG0_FLD_us_no_msg_type                      REG_FLD(8, 8)

#define DBG_US_NO_MSG1_FLD_us_no_msg_b5_b6                     REG_FLD(16, 16)
#define DBG_US_NO_MSG1_FLD_us_no_msg_b7_b8                     REG_FLD(16, 0)

#define DBG_US_NO_MSG2_FLD_us_no_msg_b9_b10                    REG_FLD(16, 16)
#define DBG_US_NO_MSG2_FLD_us_no_msg_b11_b12                   REG_FLD(16, 0)

#define DBG_US_DYING_GASP_CTRL_FLD_dying_gasp_num              REG_FLD(4, 24)
#define DBG_US_DYING_GASP_CTRL_FLD_dying_gasp_test             REG_FLD(1, 16)
#define DBG_US_DYING_GASP_CTRL_FLD_hw_dying_gasp_en            REG_FLD(1, 8)
#define DBG_US_DYING_GASP_CTRL_FLD_dying_gasp_msg_type         REG_FLD(8, 0)

#define DBG_BWM_FILTER_CTRL_FLD_bwm_intvl_invld                REG_FLD(1, 31)
#define DBG_BWM_FILTER_CTRL_FLD_bwm_len_invld                  REG_FLD(1, 30)
#define DBG_BWM_FILTER_CTRL_FLD_bwm_start_order_err            REG_FLD(1, 29)
#define DBG_BWM_FILTER_CTRL_FLD_bwm_start_order_err_flt_en     REG_FLD(1, 18)
#define DBG_BWM_FILTER_CTRL_FLD_bwm_len_vld_chk_en             REG_FLD(1, 17)
#define DBG_BWM_FILTER_CTRL_FLD_min_bst_intvl_en               REG_FLD(1, 16)
#define DBG_BWM_FILTER_CTRL_FLD_min_bst_intvl                  REG_FLD(16, 0)

#define DBG_BWM_SFIFO_STS_FLD_bwm_sfifo_max_used               REG_FLD(9, 16)
#define DBG_BWM_SFIFO_STS_FLD_bwm_sfifo_used                   REG_FLD(9, 0)

#define DBG_GRP_0_FLD_ether_fcs_err                            REG_FLD(1, 21)
#define DBG_GRP_0_FLD_ploamu_wr                                REG_FLD(1, 20)
#define DBG_GRP_0_FLD_dbru_wr                                  REG_FLD(1, 19)
#define DBG_GRP_0_FLD_is_gem_pl                                REG_FLD(1, 18)
#define DBG_GRP_0_FLD_is_fcs_only                              REG_FLD(1, 17)
#define DBG_GRP_0_FLD_is_inst_idle                             REG_FLD(1, 16)
#define DBG_GRP_0_FLD_mbi_pl_fifo_ovrn                         REG_FLD(1, 5)
#define DBG_GRP_0_FLD_aes_egn3_busy                            REG_FLD(1, 4)
#define DBG_GRP_0_FLD_aes_egn2_busy                            REG_FLD(1, 3)
#define DBG_GRP_0_FLD_aes_egn1_busy                            REG_FLD(1, 2)
#define DBG_GRP_0_FLD_aes_egn0_busy                            REG_FLD(1, 1)
#define DBG_GRP_0_FLD_aes_rdm_ciph_fifo_ovrn                   REG_FLD(1, 0)

#define DBG_GRP_1_FLD_alen_neq_zero                            REG_FLD(1, 24)
#define DBG_GRP_1_FLD_tx_align_fifo_udrn                       REG_FLD(1, 19)
#define DBG_GRP_1_FLD_rx_sof_sync                              REG_FLD(1, 18)
#define DBG_GRP_1_FLD_sync_start                               REG_FLD(1, 17)
#define DBG_GRP_1_FLD_tx_late_start                            REG_FLD(1, 16)
#define DBG_GRP_1_FLD_bst_next_full                            REG_FLD(1, 8)
#define DBG_GRP_1_FLD_sgl_next_full                            REG_FLD(1, 7)
#define DBG_GRP_1_FLD_aes_cryp_cnt_fifo_ovrn                   REG_FLD(1, 6)
#define DBG_GRP_1_FLD_aes_ciph_txt_fifo_ovrn                   REG_FLD(1, 5)
#define DBG_GRP_1_FLD_mbi_hdr_fifo_ovrn                        REG_FLD(1, 4)
#define DBG_GRP_1_FLD_ploamd_crc_err                           REG_FLD(1, 3)
#define DBG_GRP_1_FLD_bwm_uc_err                               REG_FLD(1, 2)
#define DBG_GRP_1_FLD_blen_err                                 REG_FLD(1, 1)
#define DBG_GRP_1_FLD_logd                                     REG_FLD(1, 0)

#define DBG_TXFIFO_MAX_USED_FLD_tx_align_max_used              REG_FLD(16, 0)

#define DBG_BWM_GNT_STS_FLD_pls_used                           REG_FLD(1, 19)
#define DBG_BWM_GNT_STS_FLD_dba_mode2_used                     REG_FLD(1, 18)
#define DBG_BWM_GNT_STS_FLD_dba_mode1_used                     REG_FLD(1, 17)
#define DBG_BWM_GNT_STS_FLD_dba_mode0_used                     REG_FLD(1, 16)
#define DBG_BWM_GNT_STS_FLD_max_assigned_gnt_num               REG_FLD(8, 0)

#define DBG_PROBE_CTRL_FLD_gpon_sub_probe_sel                  REG_FLD(5, 20)
#define DBG_PROBE_CTRL_FLD_probe_bit0_sel                      REG_FLD(4, 16)
#define DBG_PROBE_CTRL_FLD_bwm_probe_sel                       REG_FLD(4, 12)
#define DBG_PROBE_CTRL_FLD_tx_align_probe_dt_sel               REG_FLD(1, 11)
#define DBG_PROBE_CTRL_FLD_rx_mpi_mbi_probe_dt_sel             REG_FLD(1, 10)
#define DBG_PROBE_CTRL_FLD_tx_mpi_mbi_probe_dt_sel             REG_FLD(1, 9)
#define DBG_PROBE_CTRL_FLD_rx_data_hl_sel                      REG_FLD(1, 8)
#define DBG_PROBE_CTRL_FLD_probe_sel                           REG_FLD(5, 0)

#define DBG_PROBE_HIGH32_FLD_probe_high32                      REG_FLD(32, 0)

#define DBG_PROBE_LOW32_FLD_probe_low32                        REG_FLD(32, 0)

#define DBG_BWM_BFIFO_STS_FLD_bwm_bfifo_max_used               REG_FLD(9, 16)
#define DBG_BWM_BFIFO_STS_FLD_bwm_bfifo_used                   REG_FLD(9, 0)

#define DBG_ERR_CTRL_FLD_bwm_fec_ctrl                          REG_FLD(1, 9)
#define DBG_ERR_CTRL_FLD_ignore_atm_cell                       REG_FLD(1, 8)
#define DBG_ERR_CTRL_FLD_o5_eqd_adj_resync_en                  REG_FLD(1, 4)
#define DBG_ERR_CTRL_FLD_tx_udrn_resync_en                     REG_FLD(1, 3)
#define DBG_ERR_CTRL_FLD_tx_late_resync_en                     REG_FLD(1, 2)
#define DBG_ERR_CTRL_FLD_bst_sgl_diff_resync_en                REG_FLD(1, 1)
#define DBG_ERR_CTRL_FLD_ignore_ploamd_crc                     REG_FLD(1, 0)

#define DBG_SW_RDI_CTRL_FLD_rdi_en                             REG_FLD(1, 8)
#define DBG_SW_RDI_CTRL_FLD_sw_rdi                             REG_FLD(1, 0)

#define DBG_RX_GEM_CNT_FLD_dbg_rx_gem_cnt                      REG_FLD(32, 0)

#define DBG_RX_CRC_ERR_CNT_FLD_dbg_rx_crc_cnt                  REG_FLD(32, 0)

#define DBG_RX_GTC_CNT_FLD_dbg_rx_gtc_cnt                      REG_FLD(32, 0)

#define DBG_TX_GEM_CNT_FLD_dbg_tx_gem_cnt                      REG_FLD(32, 0)

#define DBG_TX_BST_CNT_FLD_dbg_tx_burst_cnt                    REG_FLD(32, 0)

#define DBG_GEM_HEC_ONE_ERR_CNT_FLD_dbg_gem_one_err_cnt        REG_FLD(32, 0)

#define DBG_GEM_HEC_TWO_ERR_CNT_FLD_dbg_gem_two_err_cnt        REG_FLD(32, 0)

#define DBG_GEM_HEC_UC_ERR_CNT_FLD_dbg_gem_uc_err_cnt          REG_FLD(32, 0)

#define DBG_DBA_BACK_DOOR_FLD_dbg_sw_rpt_dba_en                REG_FLD(1, 31)
#define DBG_DBA_BACK_DOOR_FLD_dbg_slight_modify_en             REG_FLD(1, 27)

#define DBG_DBA_TOTAL_BUF_SIZE_FLD_dbg_dba_total_buf_size      REG_FLD(32, 0)

#define DBG_DBA_GREEN_SIZE_FLD_dbg_dba_green_size              REG_FLD(32, 0)

#define DBG_DBA_YELLOW_SIZE_FLD_dbg_dba_yellow_size            REG_FLD(32, 0)

#define DBG_SLIGHT_MODIFY_SIZE_1_FLD_dbg_yellow_slight_modify_size REG_FLD(16, 16)
#define DBG_SLIGHT_MODIFY_SIZE_1_FLD_dbg_green_slight_modify_size REG_FLD(16, 0)

#define DBG_SLIGHT_MODIFY_SIZE_2_FLD_dbg_total_slight_modify_size REG_FLD(16, 0)

#define DBG_DS_SPF_CNT_FLD_dbg_ds_spf_cnt                      REG_FLD(30, 0)

#define DBG_TX_SYNC_OFFSET_FLD_dbg_tx_sync_offset              REG_FLD(2, 0)

#define DBG_PLOAMD_FILTER_IN_O5_FLD_dbg_ext_bst_len_filter_in_o5_en REG_FLD(1, 8)
#define DBG_PLOAMD_FILTER_IN_O5_FLD_dbg_us_overhead_filter_in_o5_en REG_FLD(1, 0)

#define DBG_SW_RESYNC_FLD_dbg_sw_resync_en                     REG_FLD(1, 31)
#define DBG_SW_RESYNC_FLD_dbg_sw_resync                        REG_FLD(1, 0)

#define DBG_GTC_ETH_EXTR_FLD_sniff_dbg_tx_rst                  REG_FLD(1, 31)
#define DBG_GTC_ETH_EXTR_FLD_sniff_pkt_padding_en              REG_FLD(1, 8)
#define DBG_GTC_ETH_EXTR_FLD_dbg_gtc_eth_extr_en               REG_FLD(1, 0)

#define DBG_DS_GTC_EXTR_ETH_HDR_FLD_dbg_extr_rx_ds_eth_type    REG_FLD(16, 16)
#define DBG_DS_GTC_EXTR_ETH_HDR_FLD_dbg_extr_rx_ds_sp          REG_FLD(16, 0)

#define DBG_US_GTC_EXTR_ETH_HDR_FLD_dbg_extr_tx_us_eth_type    REG_FLD(16, 16)
#define DBG_US_GTC_EXTR_ETH_HDR_FLD_dbg_extr_tx_us_sp          REG_FLD(16, 0)

#define DBG_DS_GTC_EXTR_ETH_CNT_FLD_dbg_extr_rx_cnt            REG_FLD(32, 0)

#define DBG_US_GTC_EXTR_ETH_CNT_FLD_dbg_extr_tx_cnt            REG_FLD(32, 0)

#define SNIFF_GTC_GTC_INVLD_GEM_BYTE_FLD_dbg_ds_all_gem_filter REG_FLD(1, 31)
#define SNIFF_GTC_GTC_INVLD_GEM_BYTE_FLD_dbg_ds_gem_filter_exclude_omci REG_FLD(1, 27)
#define SNIFF_GTC_GTC_INVLD_GEM_BYTE_FLD_dbg_us_gtc_invld_gem_byte REG_FLD(16, 0)

#define DBG_SNIFF_DBG_REG_FLD_dbg_mbi_pl_fifo_write_conflict   REG_FLD(1, 16)
#define DBG_SNIFF_DBG_REG_FLD_dbg_mbi_hdr_fifo_write_conflict  REG_FLD(1, 8)
#define DBG_SNIFF_DBG_REG_FLD_dbg_us_gtc_info_fifo_ovrn        REG_FLD(1, 0)

#define DBG_SNIFF_US_INFO_FIFO_FLD_dbg_us_info_tx_send_depth   REG_FLD(7, 0)

#define SNIFF_US_HDR_WIDO_FLD_stop_time_of_wido_en             REG_FLD(1, 31)
#define SNIFF_US_HDR_WIDO_FLD_stop_time_of_wido                REG_FLD(16, 0)

#define SNIFF_TX_DA_SA_FLD_tx_us_da_h16                        REG_FLD(16, 16)
#define SNIFF_TX_DA_SA_FLD_tx_us_sa_h16                        REG_FLD(16, 0)

#define SNIFF_RX_DA_SA_FLD_rx_ds_da_h16                        REG_FLD(16, 16)
#define SNIFF_RX_DA_SA_FLD_rx_ds_sa_h16                        REG_FLD(16, 0)

#define SNIFF_TX_RX_PID_FLD_tx_eth_pid                         REG_FLD(12, 16)
#define SNIFF_TX_RX_PID_FLD_rx_eth_pid                         REG_FLD(12, 0)

#define SNIFF_CNT_EN_FLD_sniff_cnt_en                          REG_FLD(1, 0)

#define SNIFF_RX_TX_TPID_FLD_sniff_rx_tpid                     REG_FLD(16, 16)
#define SNIFF_RX_TX_TPID_FLD_sniff_tx_tpid                     REG_FLD(16, 0)

#define DBG_GPON_MAC_SET_FLD_asb_tx_eth_4byte_align_en         REG_FLD(1, 31)
#define DBG_GPON_MAC_SET_FLD_olt_ds_fec                        REG_FLD(1, 27)
#define DBG_GPON_MAC_SET_FLD_onu_us_fec                        REG_FLD(1, 23)
#define DBG_GPON_MAC_SET_FLD_gpon_mac_sw_rst_n                 REG_FLD(1, 0)

#define SLEEP_GLB_CFG_FLD_sleep_filt_all_bwm                   REG_FLD(1, 31)
#define SLEEP_GLB_CFG_FLD_reload_sleep_cnt                     REG_FLD(1, 30)

#define SLEEP_CNT_FLD_sleep_cnt                                REG_FLD(32, 0)

#define TX_SYNC_OPT_FLD_tx_sync_opt                            REG_FLD(1, 0)

#define PLOAMU_CTRL_FLD_ploamu_ind_ctrl                        REG_FLD(1, 8)
#define PLOAMU_CTRL_FLD_o3_o4_ploamu_ctrl                      REG_FLD(1, 0)

#define TOD_1PPS_WD_CTRL_FLD_tod_1pps_width_ctrl               REG_FLD(32, 0)

#define G_ONU_ID_GET_onu_id_vld(reg32)                         REG_FLD_GET(G_ONU_ID_FLD_onu_id_vld, (reg32))
#define G_ONU_ID_GET_onu_id(reg32)                             REG_FLD_GET(G_ONU_ID_FLD_onu_id, (reg32))

#define G_GBL_CFG_GET_us_fec_en(reg32)                         REG_FLD_GET(G_GBL_CFG_FLD_us_fec_en, (reg32))
#define G_GBL_CFG_GET_sr_blk_size(reg32)                       REG_FLD_GET(G_GBL_CFG_FLD_sr_blk_size, (reg32))

#define G_INT_STATUS_GET_bwm_us_fec_err_int(reg32)             REG_FLD_GET(G_INT_STATUS_FLD_bwm_us_fec_err_int, (reg32))
#define G_INT_STATUS_GET_bwm_stop_time_err_int(reg32)          REG_FLD_GET(G_INT_STATUS_FLD_bwm_stop_time_err_int, (reg32))
#define G_INT_STATUS_GET_lwi_int(reg32)                        REG_FLD_GET(G_INT_STATUS_FLD_lwi_int, (reg32))
#define G_INT_STATUS_GET_fwi_int(reg32)                        REG_FLD_GET(G_INT_STATUS_FLD_fwi_int, (reg32))
#define G_INT_STATUS_GET_pop_up_recv_in_O6_int(reg32)          REG_FLD_GET(G_INT_STATUS_FLD_pop_up_recv_in_O6_int, (reg32))
#define G_INT_STATUS_GET_onu_us_fec_chg_int(reg32)             REG_FLD_GET(G_INT_STATUS_FLD_onu_us_fec_chg_int, (reg32))
#define G_INT_STATUS_GET_olt_ds_fec_chg_int(reg32)             REG_FLD_GET(G_INT_STATUS_FLD_olt_ds_fec_chg_int, (reg32))
#define G_INT_STATUS_GET_o5_eqd_adj_done_int(reg32)            REG_FLD_GET(G_INT_STATUS_FLD_o5_eqd_adj_done_int, (reg32))
#define G_INT_STATUS_GET_sfifo_full_int(reg32)                 REG_FLD_GET(G_INT_STATUS_FLD_sfifo_full_int, (reg32))
#define G_INT_STATUS_GET_bfifo_full_int(reg32)                 REG_FLD_GET(G_INT_STATUS_FLD_bfifo_full_int, (reg32))
#define G_INT_STATUS_GET_rx_gem_intlv_err_int(reg32)           REG_FLD_GET(G_INT_STATUS_FLD_rx_gem_intlv_err_int, (reg32))
#define G_INT_STATUS_GET_rx_eof_err_int(reg32)                 REG_FLD_GET(G_INT_STATUS_FLD_rx_eof_err_int, (reg32))
#define G_INT_STATUS_GET_tx_late_start_int(reg32)              REG_FLD_GET(G_INT_STATUS_FLD_tx_late_start_int, (reg32))
#define G_INT_STATUS_GET_bst_sgl_diff_int(reg32)               REG_FLD_GET(G_INT_STATUS_FLD_bst_sgl_diff_int, (reg32))
#define G_INT_STATUS_GET_fifo_err_int(reg32)                   REG_FLD_GET(G_INT_STATUS_FLD_fifo_err_int, (reg32))
#define G_INT_STATUS_GET_rx_err_int(reg32)                     REG_FLD_GET(G_INT_STATUS_FLD_rx_err_int, (reg32))
#define G_INT_STATUS_GET_dying_gasp_send_int(reg32)            REG_FLD_GET(G_INT_STATUS_FLD_dying_gasp_send_int, (reg32))
#define G_INT_STATUS_GET_tod_1pps_int(reg32)                   REG_FLD_GET(G_INT_STATUS_FLD_tod_1pps_int, (reg32))
#define G_INT_STATUS_GET_tod_update_done_int(reg32)            REG_FLD_GET(G_INT_STATUS_FLD_tod_update_done_int, (reg32))
#define G_INT_STATUS_GET_aes_key_switch_done_int(reg32)        REG_FLD_GET(G_INT_STATUS_FLD_aes_key_switch_done_int, (reg32))
#define G_INT_STATUS_GET_los_gem_del_int(reg32)                REG_FLD_GET(G_INT_STATUS_FLD_los_gem_del_int, (reg32))
#define G_INT_STATUS_GET_sn_req_crs_int(reg32)                 REG_FLD_GET(G_INT_STATUS_FLD_sn_req_crs_int, (reg32))
#define G_INT_STATUS_GET_sn_onu_send_o4_int(reg32)             REG_FLD_GET(G_INT_STATUS_FLD_sn_onu_send_o4_int, (reg32))
#define G_INT_STATUS_GET_ranging_req_recv_int(reg32)           REG_FLD_GET(G_INT_STATUS_FLD_ranging_req_recv_int, (reg32))
#define G_INT_STATUS_GET_sn_onu_send_o3_int(reg32)             REG_FLD_GET(G_INT_STATUS_FLD_sn_onu_send_o3_int, (reg32))
#define G_INT_STATUS_GET_sn_req_recv_int(reg32)                REG_FLD_GET(G_INT_STATUS_FLD_sn_req_recv_int, (reg32))
#define G_INT_STATUS_GET_ploamu_send_int(reg32)                REG_FLD_GET(G_INT_STATUS_FLD_ploamu_send_int, (reg32))
#define G_INT_STATUS_GET_ploamd_recv_int(reg32)                REG_FLD_GET(G_INT_STATUS_FLD_ploamd_recv_int, (reg32))

#define G_INT_ENABLE_GET_bwm_us_fec_err_int_en(reg32)          REG_FLD_GET(G_INT_ENABLE_FLD_bwm_us_fec_err_int_en, (reg32))
#define G_INT_ENABLE_GET_bwm_stop_time_err_int_en(reg32)       REG_FLD_GET(G_INT_ENABLE_FLD_bwm_stop_time_err_int_en, (reg32))
#define G_INT_ENABLE_GET_lwi_int_en(reg32)                     REG_FLD_GET(G_INT_ENABLE_FLD_lwi_int_en, (reg32))
#define G_INT_ENABLE_GET_fwi_int_en(reg32)                     REG_FLD_GET(G_INT_ENABLE_FLD_fwi_int_en, (reg32))
#define G_INT_ENABLE_GET_pop_up_recv_in_O6_int_en(reg32)       REG_FLD_GET(G_INT_ENABLE_FLD_pop_up_recv_in_O6_int_en, (reg32))
#define G_INT_ENABLE_GET_onu_us_fec_chg_int_en(reg32)          REG_FLD_GET(G_INT_ENABLE_FLD_onu_us_fec_chg_int_en, (reg32))
#define G_INT_ENABLE_GET_olt_ds_fec_chg_int_en(reg32)          REG_FLD_GET(G_INT_ENABLE_FLD_olt_ds_fec_chg_int_en, (reg32))
#define G_INT_ENABLE_GET_o5_eqd_adj_done_int_en(reg32)         REG_FLD_GET(G_INT_ENABLE_FLD_o5_eqd_adj_done_int_en, (reg32))
#define G_INT_ENABLE_GET_sfifo_full_int_en(reg32)              REG_FLD_GET(G_INT_ENABLE_FLD_sfifo_full_int_en, (reg32))
#define G_INT_ENABLE_GET_bfifo_full_int_en(reg32)              REG_FLD_GET(G_INT_ENABLE_FLD_bfifo_full_int_en, (reg32))
#define G_INT_ENABLE_GET_rx_gem_intlv_err_int_en(reg32)        REG_FLD_GET(G_INT_ENABLE_FLD_rx_gem_intlv_err_int_en, (reg32))
#define G_INT_ENABLE_GET_rx_eof_err_int_en(reg32)              REG_FLD_GET(G_INT_ENABLE_FLD_rx_eof_err_int_en, (reg32))
#define G_INT_ENABLE_GET_tx_late_start_int_en(reg32)           REG_FLD_GET(G_INT_ENABLE_FLD_tx_late_start_int_en, (reg32))
#define G_INT_ENABLE_GET_bst_sgl_diff_int_en(reg32)            REG_FLD_GET(G_INT_ENABLE_FLD_bst_sgl_diff_int_en, (reg32))
#define G_INT_ENABLE_GET_fifo_err_int_en(reg32)                REG_FLD_GET(G_INT_ENABLE_FLD_fifo_err_int_en, (reg32))
#define G_INT_ENABLE_GET_rx_err_int_en(reg32)                  REG_FLD_GET(G_INT_ENABLE_FLD_rx_err_int_en, (reg32))
#define G_INT_ENABLE_GET_dying_gasp_send_int_en(reg32)         REG_FLD_GET(G_INT_ENABLE_FLD_dying_gasp_send_int_en, (reg32))
#define G_INT_ENABLE_GET_tod_1pps_int_en(reg32)                REG_FLD_GET(G_INT_ENABLE_FLD_tod_1pps_int_en, (reg32))
#define G_INT_ENABLE_GET_tod_update_done_int_en(reg32)         REG_FLD_GET(G_INT_ENABLE_FLD_tod_update_done_int_en, (reg32))
#define G_INT_ENABLE_GET_aes_key_switch_done_int_en(reg32)     REG_FLD_GET(G_INT_ENABLE_FLD_aes_key_switch_done_int_en, (reg32))
#define G_INT_ENABLE_GET_los_gem_del_int_en(reg32)             REG_FLD_GET(G_INT_ENABLE_FLD_los_gem_del_int_en, (reg32))
#define G_INT_ENABLE_GET_sn_req_crs_int_en(reg32)              REG_FLD_GET(G_INT_ENABLE_FLD_sn_req_crs_int_en, (reg32))
#define G_INT_ENABLE_GET_sn_onu_send_o4_int_en(reg32)          REG_FLD_GET(G_INT_ENABLE_FLD_sn_onu_send_o4_int_en, (reg32))
#define G_INT_ENABLE_GET_ranging_req_recv_int_en(reg32)        REG_FLD_GET(G_INT_ENABLE_FLD_ranging_req_recv_int_en, (reg32))
#define G_INT_ENABLE_GET_sn_onu_send_o3_int_en(reg32)          REG_FLD_GET(G_INT_ENABLE_FLD_sn_onu_send_o3_int_en, (reg32))
#define G_INT_ENABLE_GET_sn_req_recv_int_en(reg32)             REG_FLD_GET(G_INT_ENABLE_FLD_sn_req_recv_int_en, (reg32))
#define G_INT_ENABLE_GET_ploamu_send_int_en(reg32)             REG_FLD_GET(G_INT_ENABLE_FLD_ploamu_send_int_en, (reg32))
#define G_INT_ENABLE_GET_ploamd_recv_int_en(reg32)             REG_FLD_GET(G_INT_ENABLE_FLD_ploamd_recv_int_en, (reg32))

#define G_TCONT_ID_0_1_GET_t_cont1_vld(reg32)                  REG_FLD_GET(G_TCONT_ID_0_1_FLD_t_cont1_vld, (reg32))
#define G_TCONT_ID_0_1_GET_t_cont1_id(reg32)                   REG_FLD_GET(G_TCONT_ID_0_1_FLD_t_cont1_id, (reg32))
#define G_TCONT_ID_0_1_GET_t_cont0_vld(reg32)                  REG_FLD_GET(G_TCONT_ID_0_1_FLD_t_cont0_vld, (reg32))
#define G_TCONT_ID_0_1_GET_t_cont0_id(reg32)                   REG_FLD_GET(G_TCONT_ID_0_1_FLD_t_cont0_id, (reg32))

#define G_TCONT_ID_2_3_GET_t_cont3_vld(reg32)                  REG_FLD_GET(G_TCONT_ID_2_3_FLD_t_cont3_vld, (reg32))
#define G_TCONT_ID_2_3_GET_t_cont3_id(reg32)                   REG_FLD_GET(G_TCONT_ID_2_3_FLD_t_cont3_id, (reg32))
#define G_TCONT_ID_2_3_GET_t_cont2_vld(reg32)                  REG_FLD_GET(G_TCONT_ID_2_3_FLD_t_cont2_vld, (reg32))
#define G_TCONT_ID_2_3_GET_t_cont2_id(reg32)                   REG_FLD_GET(G_TCONT_ID_2_3_FLD_t_cont2_id, (reg32))

#define G_TCONT_ID_4_5_GET_t_cont5_vld(reg32)                  REG_FLD_GET(G_TCONT_ID_4_5_FLD_t_cont5_vld, (reg32))
#define G_TCONT_ID_4_5_GET_t_cont5_id(reg32)                   REG_FLD_GET(G_TCONT_ID_4_5_FLD_t_cont5_id, (reg32))
#define G_TCONT_ID_4_5_GET_t_cont4_vld(reg32)                  REG_FLD_GET(G_TCONT_ID_4_5_FLD_t_cont4_vld, (reg32))
#define G_TCONT_ID_4_5_GET_t_cont4_id(reg32)                   REG_FLD_GET(G_TCONT_ID_4_5_FLD_t_cont4_id, (reg32))

#define G_TCONT_ID_6_7_GET_t_cont7_vld(reg32)                  REG_FLD_GET(G_TCONT_ID_6_7_FLD_t_cont7_vld, (reg32))
#define G_TCONT_ID_6_7_GET_t_cont7_id(reg32)                   REG_FLD_GET(G_TCONT_ID_6_7_FLD_t_cont7_id, (reg32))
#define G_TCONT_ID_6_7_GET_t_cont6_vld(reg32)                  REG_FLD_GET(G_TCONT_ID_6_7_FLD_t_cont6_vld, (reg32))
#define G_TCONT_ID_6_7_GET_t_cont6_id(reg32)                   REG_FLD_GET(G_TCONT_ID_6_7_FLD_t_cont6_id, (reg32))

#define G_TCONT_ID_8_9_GET_t_cont9_vld(reg32)                  REG_FLD_GET(G_TCONT_ID_8_9_FLD_t_cont9_vld, (reg32))
#define G_TCONT_ID_8_9_GET_t_cont9_id(reg32)                   REG_FLD_GET(G_TCONT_ID_8_9_FLD_t_cont9_id, (reg32))
#define G_TCONT_ID_8_9_GET_t_cont8_vld(reg32)                  REG_FLD_GET(G_TCONT_ID_8_9_FLD_t_cont8_vld, (reg32))
#define G_TCONT_ID_8_9_GET_t_cont8_id(reg32)                   REG_FLD_GET(G_TCONT_ID_8_9_FLD_t_cont8_id, (reg32))

#define G_TCONT_ID_10_11_GET_t_cont11_vld(reg32)               REG_FLD_GET(G_TCONT_ID_10_11_FLD_t_cont11_vld, (reg32))
#define G_TCONT_ID_10_11_GET_t_cont11_id(reg32)                REG_FLD_GET(G_TCONT_ID_10_11_FLD_t_cont11_id, (reg32))
#define G_TCONT_ID_10_11_GET_t_cont10_vld(reg32)               REG_FLD_GET(G_TCONT_ID_10_11_FLD_t_cont10_vld, (reg32))
#define G_TCONT_ID_10_11_GET_t_cont10_id(reg32)                REG_FLD_GET(G_TCONT_ID_10_11_FLD_t_cont10_id, (reg32))

#define G_TCONT_ID_12_13_GET_t_cont13_vld(reg32)               REG_FLD_GET(G_TCONT_ID_12_13_FLD_t_cont13_vld, (reg32))
#define G_TCONT_ID_12_13_GET_t_cont13_id(reg32)                REG_FLD_GET(G_TCONT_ID_12_13_FLD_t_cont13_id, (reg32))
#define G_TCONT_ID_12_13_GET_t_cont12_vld(reg32)               REG_FLD_GET(G_TCONT_ID_12_13_FLD_t_cont12_vld, (reg32))
#define G_TCONT_ID_12_13_GET_t_cont12_id(reg32)                REG_FLD_GET(G_TCONT_ID_12_13_FLD_t_cont12_id, (reg32))

#define G_TCONT_ID_14_15_GET_t_cont15_vld(reg32)               REG_FLD_GET(G_TCONT_ID_14_15_FLD_t_cont15_vld, (reg32))
#define G_TCONT_ID_14_15_GET_t_cont15_id(reg32)                REG_FLD_GET(G_TCONT_ID_14_15_FLD_t_cont15_id, (reg32))
#define G_TCONT_ID_14_15_GET_t_cont14_vld(reg32)               REG_FLD_GET(G_TCONT_ID_14_15_FLD_t_cont14_vld, (reg32))
#define G_TCONT_ID_14_15_GET_t_cont14_id(reg32)                REG_FLD_GET(G_TCONT_ID_14_15_FLD_t_cont14_id, (reg32))

#define G_GEM_PORT_CFG_GET_gpid_cmd(reg32)                     REG_FLD_GET(G_GEM_PORT_CFG_FLD_gpid_cmd, (reg32))
#define G_GEM_PORT_CFG_GET_gpid_encrypt(reg32)                 REG_FLD_GET(G_GEM_PORT_CFG_FLD_gpid_encrypt, (reg32))
#define G_GEM_PORT_CFG_GET_gpid_vld(reg32)                     REG_FLD_GET(G_GEM_PORT_CFG_FLD_gpid_vld, (reg32))
#define G_GEM_PORT_CFG_GET_gem_port_id(reg32)                  REG_FLD_GET(G_GEM_PORT_CFG_FLD_gem_port_id, (reg32))

#define G_GEM_PORT_STS_GET_gpid_cmd_done(reg32)                REG_FLD_GET(G_GEM_PORT_STS_FLD_gpid_cmd_done, (reg32))
#define G_GEM_PORT_STS_GET_gpid_encrypt_sts(reg32)             REG_FLD_GET(G_GEM_PORT_STS_FLD_gpid_encrypt_sts, (reg32))
#define G_GEM_PORT_STS_GET_gpid_vld_sts(reg32)                 REG_FLD_GET(G_GEM_PORT_STS_FLD_gpid_vld_sts, (reg32))

#define G_OMCI_ID_GET_omci_port_id_vld(reg32)                  REG_FLD_GET(G_OMCI_ID_FLD_omci_port_id_vld, (reg32))
#define G_OMCI_ID_GET_omci_gpid(reg32)                         REG_FLD_GET(G_OMCI_ID_FLD_omci_gpid, (reg32))

#define G_GEM_TBL_INIT_GET_gem_tbl_init_done(reg32)            REG_FLD_GET(G_GEM_TBL_INIT_FLD_gem_tbl_init_done, (reg32))
#define G_GEM_TBL_INIT_GET_gem_tbl_init_start(reg32)           REG_FLD_GET(G_GEM_TBL_INIT_FLD_gem_tbl_init_start, (reg32))

#define G_PLOAMu_FIFO_STS_GET_ploamu_fifo_udrn(reg32)          REG_FLD_GET(G_PLOAMu_FIFO_STS_FLD_ploamu_fifo_udrn, (reg32))
#define G_PLOAMu_FIFO_STS_GET_ploamu_fifo_min_avail(reg32)     REG_FLD_GET(G_PLOAMu_FIFO_STS_FLD_ploamu_fifo_min_avail, (reg32))
#define G_PLOAMu_FIFO_STS_GET_ploamu_fifo_avail(reg32)         REG_FLD_GET(G_PLOAMu_FIFO_STS_FLD_ploamu_fifo_avail, (reg32))

#define G_PLOAMu_WDATA_GET_ploamu_wdata(reg32)                 REG_FLD_GET(G_PLOAMu_WDATA_FLD_ploamu_wdata, (reg32))

#define G_PLOAMd_FIFO_STS_GET_ploamd_fifo_ovrn(reg32)          REG_FLD_GET(G_PLOAMd_FIFO_STS_FLD_ploamd_fifo_ovrn, (reg32))
#define G_PLOAMd_FIFO_STS_GET_ploamd_fifo_max_used(reg32)      REG_FLD_GET(G_PLOAMd_FIFO_STS_FLD_ploamd_fifo_max_used, (reg32))
#define G_PLOAMd_FIFO_STS_GET_ploamd_fifo_used(reg32)          REG_FLD_GET(G_PLOAMd_FIFO_STS_FLD_ploamd_fifo_used, (reg32))

#define G_PLOAMd_RDATA_GET_ploamd_rdata(reg32)                 REG_FLD_GET(G_PLOAMd_RDATA_FLD_ploamd_rdata, (reg32))

#define G_AES_CFG_GET_aes_spf_cnt(reg32)                       REG_FLD_GET(G_AES_CFG_FLD_aes_spf_cnt, (reg32))

#define G_AES_ACTIVE_KEY0_GET_aes_active_key0(reg32)           REG_FLD_GET(G_AES_ACTIVE_KEY0_FLD_aes_active_key0, (reg32))

#define G_AES_ACTIVE_KEY1_GET_aes_active_key1(reg32)           REG_FLD_GET(G_AES_ACTIVE_KEY1_FLD_aes_active_key1, (reg32))

#define G_AES_ACTIVE_KEY2_GET_aes_active_key2(reg32)           REG_FLD_GET(G_AES_ACTIVE_KEY2_FLD_aes_active_key2, (reg32))

#define G_AES_ACTIVE_KEY3_GET_aes_active_key3(reg32)           REG_FLD_GET(G_AES_ACTIVE_KEY3_FLD_aes_active_key3, (reg32))

#define G_AES_SHADOW_KEY0_GET_aes_shadow_key0(reg32)           REG_FLD_GET(G_AES_SHADOW_KEY0_FLD_aes_shadow_key0, (reg32))

#define G_AES_SHADOW_KEY1_GET_aes_shadow_key1(reg32)           REG_FLD_GET(G_AES_SHADOW_KEY1_FLD_aes_shadow_key1, (reg32))

#define G_AES_SHADOW_KEY2_GET_aes_shadow_key2(reg32)           REG_FLD_GET(G_AES_SHADOW_KEY2_FLD_aes_shadow_key2, (reg32))

#define G_AES_SHADOW_KEY3_GET_aes_shadow_key3(reg32)           REG_FLD_GET(G_AES_SHADOW_KEY3_FLD_aes_shadow_key3, (reg32))

#define G_AES_KEY_SWITCH_BY_SW_GET_aes_key_switch_by_sw_done(reg32) REG_FLD_GET(G_AES_KEY_SWITCH_BY_SW_FLD_aes_key_switch_by_sw_done, (reg32))
#define G_AES_KEY_SWITCH_BY_SW_GET_aes_key_switch_by_sw_cmd(reg32) REG_FLD_GET(G_AES_KEY_SWITCH_BY_SW_FLD_aes_key_switch_by_sw_cmd, (reg32))

#define G_PLOu_OVERHEAD_GET_plou_overhead(reg32)               REG_FLD_GET(G_PLOu_OVERHEAD_FLD_plou_overhead, (reg32))

#define G_PLOu_GUARD_BIT_GET_guard_bit(reg32)                  REG_FLD_GET(G_PLOu_GUARD_BIT_FLD_guard_bit, (reg32))

#define G_PLOu_PRMBL_TYPE1_2_GET_prmb2_bit(reg32)              REG_FLD_GET(G_PLOu_PRMBL_TYPE1_2_FLD_prmb2_bit, (reg32))
#define G_PLOu_PRMBL_TYPE1_2_GET_prmb1_bit(reg32)              REG_FLD_GET(G_PLOu_PRMBL_TYPE1_2_FLD_prmb1_bit, (reg32))

#define G_PLOu_PRMBL_TYPE3_GET_ebl_en(reg32)                   REG_FLD_GET(G_PLOu_PRMBL_TYPE3_FLD_ebl_en, (reg32))
#define G_PLOu_PRMBL_TYPE3_GET_ext_prmb3_o5_num(reg32)         REG_FLD_GET(G_PLOu_PRMBL_TYPE3_FLD_ext_prmb3_o5_num, (reg32))
#define G_PLOu_PRMBL_TYPE3_GET_ext_prmb3_o3_o4_num(reg32)      REG_FLD_GET(G_PLOu_PRMBL_TYPE3_FLD_ext_prmb3_o3_o4_num, (reg32))

#define G_PLOu_DELM_BIT_GET_delm_bit(reg32)                    REG_FLD_GET(G_PLOu_DELM_BIT_FLD_delm_bit, (reg32))

#define G_PRE_ASSIGNED_DLY_GET_pre_dly_en(reg32)               REG_FLD_GET(G_PRE_ASSIGNED_DLY_FLD_pre_dly_en, (reg32))
#define G_PRE_ASSIGNED_DLY_GET_pre_dly(reg32)                  REG_FLD_GET(G_PRE_ASSIGNED_DLY_FLD_pre_dly, (reg32))

#define G_EQD_GET_eqd(reg32)                                   REG_FLD_GET(G_EQD_FLD_eqd, (reg32))

#define G_RSP_TIME_GET_tresp(reg32)                            REG_FLD_GET(G_RSP_TIME_FLD_tresp, (reg32))

#define G_VENDOR_ID_GET_vendor_id(reg32)                       REG_FLD_GET(G_VENDOR_ID_FLD_vendor_id, (reg32))

#define G_VS_SN_GET_vs_sn(reg32)                               REG_FLD_GET(G_VS_SN_FLD_vs_sn, (reg32))

#define G_SN_MSG_CFG_GET_sn_req_thr(reg32)                     REG_FLD_GET(G_SN_MSG_CFG_FLD_sn_req_thr, (reg32))
#define G_SN_MSG_CFG_GET_tx_power_mode(reg32)                  REG_FLD_GET(G_SN_MSG_CFG_FLD_tx_power_mode, (reg32))
#define G_SN_MSG_CFG_GET_rdm_dly(reg32)                        REG_FLD_GET(G_SN_MSG_CFG_FLD_rdm_dly, (reg32))

#define G_ACTIVATION_ST_GET_act_st(reg32)                      REG_FLD_GET(G_ACTIVATION_ST_FLD_act_st, (reg32))

#define G_TOD_CFG_GET_tod_spf_cnt(reg32)                       REG_FLD_GET(G_TOD_CFG_FLD_tod_spf_cnt, (reg32))

#define G_NEW_TOD_SEC_L32_GET_new_tod_sec_l32(reg32)           REG_FLD_GET(G_NEW_TOD_SEC_L32_FLD_new_tod_sec_l32, (reg32))

#define G_NEW_TOD_NANO_SEC_GET_new_tod_nano_sec(reg32)         REG_FLD_GET(G_NEW_TOD_NANO_SEC_FLD_new_tod_nano_sec, (reg32))

#define G_CUR_TOD_SEC_L32_GET_cur_tod_sec_l32(reg32)           REG_FLD_GET(G_CUR_TOD_SEC_L32_FLD_cur_tod_sec_l32, (reg32))

#define G_CUR_TOD_NANO_SEC_GET_cur_tod_nano_sec(reg32)         REG_FLD_GET(G_CUR_TOD_NANO_SEC_FLD_cur_tod_nano_sec, (reg32))

#define G_TOD_CLK_PERIOD_GET_tod_period(reg32)                 REG_FLD_GET(G_TOD_CLK_PERIOD_FLD_tod_period, (reg32))

#define G_TX_FCS_TBL_INIT_GET_tx_fcs_tbl_init_done(reg32)      REG_FLD_GET(G_TX_FCS_TBL_INIT_FLD_tx_fcs_tbl_init_done, (reg32))
#define G_TX_FCS_TBL_INIT_GET_tx_fcs_tbl_init_start(reg32)     REG_FLD_GET(G_TX_FCS_TBL_INIT_FLD_tx_fcs_tbl_init_start, (reg32))

#define G_MIB_CTRL_STS_GET_mib_cmd(reg32)                      REG_FLD_GET(G_MIB_CTRL_STS_FLD_mib_cmd, (reg32))
#define G_MIB_CTRL_STS_GET_mib_cmd_done(reg32)                 REG_FLD_GET(G_MIB_CTRL_STS_FLD_mib_cmd_done, (reg32))
#define G_MIB_CTRL_STS_GET_mib_read_clr_en(reg32)              REG_FLD_GET(G_MIB_CTRL_STS_FLD_mib_read_clr_en, (reg32))
#define G_MIB_CTRL_STS_GET_mib_type(reg32)                     REG_FLD_GET(G_MIB_CTRL_STS_FLD_mib_type, (reg32))
#define G_MIB_CTRL_STS_GET_mib_gpid(reg32)                     REG_FLD_GET(G_MIB_CTRL_STS_FLD_mib_gpid, (reg32))

#define G_MIB_RDATA_L32_GET_mib_rdata_l32(reg32)               REG_FLD_GET(G_MIB_RDATA_L32_FLD_mib_rdata_l32, (reg32))

#define G_MIB_RDATA_H32_GET_mib_rdata_h32(reg32)               REG_FLD_GET(G_MIB_RDATA_H32_FLD_mib_rdata_h32, (reg32))

#define G_MIB_WDATA_L32_GET_mib_wdata_l32(reg32)               REG_FLD_GET(G_MIB_WDATA_L32_FLD_mib_wdata_l32, (reg32))

#define G_MIB_WDATA_H32_GET_mib_wdata_h32(reg32)               REG_FLD_GET(G_MIB_WDATA_H32_FLD_mib_wdata_h32, (reg32))

#define G_MIB_TBL_INIT_GET_mib_tbl_init_done(reg32)            REG_FLD_GET(G_MIB_TBL_INIT_FLD_mib_tbl_init_done, (reg32))
#define G_MIB_TBL_INIT_GET_mib_tbl_init_start(reg32)           REG_FLD_GET(G_MIB_TBL_INIT_FLD_mib_tbl_init_start, (reg32))

#define G_GPIDX_TBL_CTRL_GET_gpidx_cmd(reg32)                  REG_FLD_GET(G_GPIDX_TBL_CTRL_FLD_gpidx_cmd, (reg32))
#define G_GPIDX_TBL_CTRL_GET_gpidx_tbl_addr(reg32)             REG_FLD_GET(G_GPIDX_TBL_CTRL_FLD_gpidx_tbl_addr, (reg32))
#define G_GPIDX_TBL_CTRL_GET_gpidx_tbl_wdata(reg32)            REG_FLD_GET(G_GPIDX_TBL_CTRL_FLD_gpidx_tbl_wdata, (reg32))

#define G_GPIDX_TBL_STS_GET_gpidx_cmd_done(reg32)              REG_FLD_GET(G_GPIDX_TBL_STS_FLD_gpidx_cmd_done, (reg32))
#define G_GPIDX_TBL_STS_GET_gpidx_tbl_rdata(reg32)             REG_FLD_GET(G_GPIDX_TBL_STS_FLD_gpidx_tbl_rdata, (reg32))

#define G_GPIDX_TBL_INIT_GET_gpidx_tbl_init_done(reg32)        REG_FLD_GET(G_GPIDX_TBL_INIT_FLD_gpidx_tbl_init_done, (reg32))
#define G_GPIDX_TBL_INIT_GET_gpidx_tbl_init_start(reg32)       REG_FLD_GET(G_GPIDX_TBL_INIT_FLD_gpidx_tbl_init_start, (reg32))

#define G_MBI_STOP_GET_mbi_tx_stop(reg32)                      REG_FLD_GET(G_MBI_STOP_FLD_mbi_tx_stop, (reg32))
#define G_MBI_STOP_GET_mbi_rx_stop(reg32)                      REG_FLD_GET(G_MBI_STOP_FLD_mbi_rx_stop, (reg32))

#define G_TCONT_ID_16_31_CFG_GET_tcont_cmd(reg32)              REG_FLD_GET(G_TCONT_ID_16_31_CFG_FLD_tcont_cmd, (reg32))
#define G_TCONT_ID_16_31_CFG_GET_wr_tcont_id_vld(reg32)        REG_FLD_GET(G_TCONT_ID_16_31_CFG_FLD_wr_tcont_id_vld, (reg32))
#define G_TCONT_ID_16_31_CFG_GET_tcont_id_index(reg32)         REG_FLD_GET(G_TCONT_ID_16_31_CFG_FLD_tcont_id_index, (reg32))
#define G_TCONT_ID_16_31_CFG_GET_wr_tcont_id(reg32)            REG_FLD_GET(G_TCONT_ID_16_31_CFG_FLD_wr_tcont_id, (reg32))

#define G_TCONT_ID_16_31_STS_GET_tcont_cmd_done(reg32)         REG_FLD_GET(G_TCONT_ID_16_31_STS_FLD_tcont_cmd_done, (reg32))
#define G_TCONT_ID_16_31_STS_GET_rd_tcont_id_vld(reg32)        REG_FLD_GET(G_TCONT_ID_16_31_STS_FLD_rd_tcont_id_vld, (reg32))
#define G_TCONT_ID_16_31_STS_GET_rd_tcont_id(reg32)            REG_FLD_GET(G_TCONT_ID_16_31_STS_FLD_rd_tcont_id, (reg32))

#define DBG_CAP_SETTING_GET_mib_frame_type(reg32)              REG_FLD_GET(DBG_CAP_SETTING_FLD_mib_frame_type, (reg32))
#define DBG_CAP_SETTING_GET_gpon_mib_en(reg32)                 REG_FLD_GET(DBG_CAP_SETTING_FLD_gpon_mib_en, (reg32))
#define DBG_CAP_SETTING_GET_us_no_msg_int_en(reg32)            REG_FLD_GET(DBG_CAP_SETTING_FLD_us_no_msg_int_en, (reg32))
#define DBG_CAP_SETTING_GET_rpt_msg_flt(reg32)                 REG_FLD_GET(DBG_CAP_SETTING_FLD_rpt_msg_flt, (reg32))
#define DBG_CAP_SETTING_GET_max_rdm_dly(reg32)                 REG_FLD_GET(DBG_CAP_SETTING_FLD_max_rdm_dly, (reg32))

#define DBG_US_PREF_OFS_GET_sch_pref_ofs(reg32)                REG_FLD_GET(DBG_US_PREF_OFS_FLD_sch_pref_ofs, (reg32))
#define DBG_US_PREF_OFS_GET_dt_pref_ofs(reg32)                 REG_FLD_GET(DBG_US_PREF_OFS_FLD_dt_pref_ofs, (reg32))

#define DBG_DLY_GET_phy_rx_dly_sel(reg32)                      REG_FLD_GET(DBG_DLY_FLD_phy_rx_dly_sel, (reg32))
#define DBG_DLY_GET_fix_phy_rx_dly(reg32)                      REG_FLD_GET(DBG_DLY_FLD_fix_phy_rx_dly, (reg32))
#define DBG_DLY_GET_fine_int_dly(reg32)                        REG_FLD_GET(DBG_DLY_FLD_fine_int_dly, (reg32))
#define DBG_DLY_GET_phy_tx_dly(reg32)                          REG_FLD_GET(DBG_DLY_FLD_phy_tx_dly, (reg32))

#define DBG_IDLE_GEM_THLD_GET_idle_gem_thld(reg32)             REG_FLD_GET(DBG_IDLE_GEM_THLD_FLD_idle_gem_thld, (reg32))

#define DBG_US_NO_MSG0_GET_us_no_msg_b3_b4(reg32)              REG_FLD_GET(DBG_US_NO_MSG0_FLD_us_no_msg_b3_b4, (reg32))
#define DBG_US_NO_MSG0_GET_us_no_msg_type(reg32)               REG_FLD_GET(DBG_US_NO_MSG0_FLD_us_no_msg_type, (reg32))

#define DBG_US_NO_MSG1_GET_us_no_msg_b5_b6(reg32)              REG_FLD_GET(DBG_US_NO_MSG1_FLD_us_no_msg_b5_b6, (reg32))
#define DBG_US_NO_MSG1_GET_us_no_msg_b7_b8(reg32)              REG_FLD_GET(DBG_US_NO_MSG1_FLD_us_no_msg_b7_b8, (reg32))

#define DBG_US_NO_MSG2_GET_us_no_msg_b9_b10(reg32)             REG_FLD_GET(DBG_US_NO_MSG2_FLD_us_no_msg_b9_b10, (reg32))
#define DBG_US_NO_MSG2_GET_us_no_msg_b11_b12(reg32)            REG_FLD_GET(DBG_US_NO_MSG2_FLD_us_no_msg_b11_b12, (reg32))

#define DBG_US_DYING_GASP_CTRL_GET_dying_gasp_num(reg32)       REG_FLD_GET(DBG_US_DYING_GASP_CTRL_FLD_dying_gasp_num, (reg32))
#define DBG_US_DYING_GASP_CTRL_GET_dying_gasp_test(reg32)      REG_FLD_GET(DBG_US_DYING_GASP_CTRL_FLD_dying_gasp_test, (reg32))
#define DBG_US_DYING_GASP_CTRL_GET_hw_dying_gasp_en(reg32)     REG_FLD_GET(DBG_US_DYING_GASP_CTRL_FLD_hw_dying_gasp_en, (reg32))
#define DBG_US_DYING_GASP_CTRL_GET_dying_gasp_msg_type(reg32)  REG_FLD_GET(DBG_US_DYING_GASP_CTRL_FLD_dying_gasp_msg_type, (reg32))

#define DBG_BWM_FILTER_CTRL_GET_bwm_intvl_invld(reg32)         REG_FLD_GET(DBG_BWM_FILTER_CTRL_FLD_bwm_intvl_invld, (reg32))
#define DBG_BWM_FILTER_CTRL_GET_bwm_len_invld(reg32)           REG_FLD_GET(DBG_BWM_FILTER_CTRL_FLD_bwm_len_invld, (reg32))
#define DBG_BWM_FILTER_CTRL_GET_bwm_start_order_err(reg32)     REG_FLD_GET(DBG_BWM_FILTER_CTRL_FLD_bwm_start_order_err, (reg32))
#define DBG_BWM_FILTER_CTRL_GET_bwm_start_order_err_flt_en(reg32) REG_FLD_GET(DBG_BWM_FILTER_CTRL_FLD_bwm_start_order_err_flt_en, (reg32))
#define DBG_BWM_FILTER_CTRL_GET_bwm_len_vld_chk_en(reg32)      REG_FLD_GET(DBG_BWM_FILTER_CTRL_FLD_bwm_len_vld_chk_en, (reg32))
#define DBG_BWM_FILTER_CTRL_GET_min_bst_intvl_en(reg32)        REG_FLD_GET(DBG_BWM_FILTER_CTRL_FLD_min_bst_intvl_en, (reg32))
#define DBG_BWM_FILTER_CTRL_GET_min_bst_intvl(reg32)           REG_FLD_GET(DBG_BWM_FILTER_CTRL_FLD_min_bst_intvl, (reg32))

#define DBG_BWM_SFIFO_STS_GET_bwm_sfifo_max_used(reg32)        REG_FLD_GET(DBG_BWM_SFIFO_STS_FLD_bwm_sfifo_max_used, (reg32))
#define DBG_BWM_SFIFO_STS_GET_bwm_sfifo_used(reg32)            REG_FLD_GET(DBG_BWM_SFIFO_STS_FLD_bwm_sfifo_used, (reg32))

#define DBG_GRP_0_GET_ether_fcs_err(reg32)                     REG_FLD_GET(DBG_GRP_0_FLD_ether_fcs_err, (reg32))
#define DBG_GRP_0_GET_ploamu_wr(reg32)                         REG_FLD_GET(DBG_GRP_0_FLD_ploamu_wr, (reg32))
#define DBG_GRP_0_GET_dbru_wr(reg32)                           REG_FLD_GET(DBG_GRP_0_FLD_dbru_wr, (reg32))
#define DBG_GRP_0_GET_is_gem_pl(reg32)                         REG_FLD_GET(DBG_GRP_0_FLD_is_gem_pl, (reg32))
#define DBG_GRP_0_GET_is_fcs_only(reg32)                       REG_FLD_GET(DBG_GRP_0_FLD_is_fcs_only, (reg32))
#define DBG_GRP_0_GET_is_inst_idle(reg32)                      REG_FLD_GET(DBG_GRP_0_FLD_is_inst_idle, (reg32))
#define DBG_GRP_0_GET_mbi_pl_fifo_ovrn(reg32)                  REG_FLD_GET(DBG_GRP_0_FLD_mbi_pl_fifo_ovrn, (reg32))
#define DBG_GRP_0_GET_aes_egn3_busy(reg32)                     REG_FLD_GET(DBG_GRP_0_FLD_aes_egn3_busy, (reg32))
#define DBG_GRP_0_GET_aes_egn2_busy(reg32)                     REG_FLD_GET(DBG_GRP_0_FLD_aes_egn2_busy, (reg32))
#define DBG_GRP_0_GET_aes_egn1_busy(reg32)                     REG_FLD_GET(DBG_GRP_0_FLD_aes_egn1_busy, (reg32))
#define DBG_GRP_0_GET_aes_egn0_busy(reg32)                     REG_FLD_GET(DBG_GRP_0_FLD_aes_egn0_busy, (reg32))
#define DBG_GRP_0_GET_aes_rdm_ciph_fifo_ovrn(reg32)            REG_FLD_GET(DBG_GRP_0_FLD_aes_rdm_ciph_fifo_ovrn, (reg32))

#define DBG_GRP_1_GET_alen_neq_zero(reg32)                     REG_FLD_GET(DBG_GRP_1_FLD_alen_neq_zero, (reg32))
#define DBG_GRP_1_GET_tx_align_fifo_udrn(reg32)                REG_FLD_GET(DBG_GRP_1_FLD_tx_align_fifo_udrn, (reg32))
#define DBG_GRP_1_GET_rx_sof_sync(reg32)                       REG_FLD_GET(DBG_GRP_1_FLD_rx_sof_sync, (reg32))
#define DBG_GRP_1_GET_sync_start(reg32)                        REG_FLD_GET(DBG_GRP_1_FLD_sync_start, (reg32))
#define DBG_GRP_1_GET_tx_late_start(reg32)                     REG_FLD_GET(DBG_GRP_1_FLD_tx_late_start, (reg32))
#define DBG_GRP_1_GET_bst_next_full(reg32)                     REG_FLD_GET(DBG_GRP_1_FLD_bst_next_full, (reg32))
#define DBG_GRP_1_GET_sgl_next_full(reg32)                     REG_FLD_GET(DBG_GRP_1_FLD_sgl_next_full, (reg32))
#define DBG_GRP_1_GET_aes_cryp_cnt_fifo_ovrn(reg32)            REG_FLD_GET(DBG_GRP_1_FLD_aes_cryp_cnt_fifo_ovrn, (reg32))
#define DBG_GRP_1_GET_aes_ciph_txt_fifo_ovrn(reg32)            REG_FLD_GET(DBG_GRP_1_FLD_aes_ciph_txt_fifo_ovrn, (reg32))
#define DBG_GRP_1_GET_mbi_hdr_fifo_ovrn(reg32)                 REG_FLD_GET(DBG_GRP_1_FLD_mbi_hdr_fifo_ovrn, (reg32))
#define DBG_GRP_1_GET_ploamd_crc_err(reg32)                    REG_FLD_GET(DBG_GRP_1_FLD_ploamd_crc_err, (reg32))
#define DBG_GRP_1_GET_bwm_uc_err(reg32)                        REG_FLD_GET(DBG_GRP_1_FLD_bwm_uc_err, (reg32))
#define DBG_GRP_1_GET_blen_err(reg32)                          REG_FLD_GET(DBG_GRP_1_FLD_blen_err, (reg32))
#define DBG_GRP_1_GET_logd(reg32)                              REG_FLD_GET(DBG_GRP_1_FLD_logd, (reg32))

#define DBG_TXFIFO_MAX_USED_GET_tx_align_max_used(reg32)       REG_FLD_GET(DBG_TXFIFO_MAX_USED_FLD_tx_align_max_used, (reg32))

#define DBG_BWM_GNT_STS_GET_pls_used(reg32)                    REG_FLD_GET(DBG_BWM_GNT_STS_FLD_pls_used, (reg32))
#define DBG_BWM_GNT_STS_GET_dba_mode2_used(reg32)              REG_FLD_GET(DBG_BWM_GNT_STS_FLD_dba_mode2_used, (reg32))
#define DBG_BWM_GNT_STS_GET_dba_mode1_used(reg32)              REG_FLD_GET(DBG_BWM_GNT_STS_FLD_dba_mode1_used, (reg32))
#define DBG_BWM_GNT_STS_GET_dba_mode0_used(reg32)              REG_FLD_GET(DBG_BWM_GNT_STS_FLD_dba_mode0_used, (reg32))
#define DBG_BWM_GNT_STS_GET_max_assigned_gnt_num(reg32)        REG_FLD_GET(DBG_BWM_GNT_STS_FLD_max_assigned_gnt_num, (reg32))

#define DBG_PROBE_CTRL_GET_gpon_sub_probe_sel(reg32)           REG_FLD_GET(DBG_PROBE_CTRL_FLD_gpon_sub_probe_sel, (reg32))
#define DBG_PROBE_CTRL_GET_probe_bit0_sel(reg32)               REG_FLD_GET(DBG_PROBE_CTRL_FLD_probe_bit0_sel, (reg32))
#define DBG_PROBE_CTRL_GET_bwm_probe_sel(reg32)                REG_FLD_GET(DBG_PROBE_CTRL_FLD_bwm_probe_sel, (reg32))
#define DBG_PROBE_CTRL_GET_tx_align_probe_dt_sel(reg32)        REG_FLD_GET(DBG_PROBE_CTRL_FLD_tx_align_probe_dt_sel, (reg32))
#define DBG_PROBE_CTRL_GET_rx_mpi_mbi_probe_dt_sel(reg32)      REG_FLD_GET(DBG_PROBE_CTRL_FLD_rx_mpi_mbi_probe_dt_sel, (reg32))
#define DBG_PROBE_CTRL_GET_tx_mpi_mbi_probe_dt_sel(reg32)      REG_FLD_GET(DBG_PROBE_CTRL_FLD_tx_mpi_mbi_probe_dt_sel, (reg32))
#define DBG_PROBE_CTRL_GET_rx_data_hl_sel(reg32)               REG_FLD_GET(DBG_PROBE_CTRL_FLD_rx_data_hl_sel, (reg32))
#define DBG_PROBE_CTRL_GET_probe_sel(reg32)                    REG_FLD_GET(DBG_PROBE_CTRL_FLD_probe_sel, (reg32))

#define DBG_PROBE_HIGH32_GET_probe_high32(reg32)               REG_FLD_GET(DBG_PROBE_HIGH32_FLD_probe_high32, (reg32))

#define DBG_PROBE_LOW32_GET_probe_low32(reg32)                 REG_FLD_GET(DBG_PROBE_LOW32_FLD_probe_low32, (reg32))

#define DBG_BWM_BFIFO_STS_GET_bwm_bfifo_max_used(reg32)        REG_FLD_GET(DBG_BWM_BFIFO_STS_FLD_bwm_bfifo_max_used, (reg32))
#define DBG_BWM_BFIFO_STS_GET_bwm_bfifo_used(reg32)            REG_FLD_GET(DBG_BWM_BFIFO_STS_FLD_bwm_bfifo_used, (reg32))

#define DBG_ERR_CTRL_GET_bwm_fec_ctrl(reg32)                   REG_FLD_GET(DBG_ERR_CTRL_FLD_bwm_fec_ctrl, (reg32))
#define DBG_ERR_CTRL_GET_ignore_atm_cell(reg32)                REG_FLD_GET(DBG_ERR_CTRL_FLD_ignore_atm_cell, (reg32))
#define DBG_ERR_CTRL_GET_o5_eqd_adj_resync_en(reg32)           REG_FLD_GET(DBG_ERR_CTRL_FLD_o5_eqd_adj_resync_en, (reg32))
#define DBG_ERR_CTRL_GET_tx_udrn_resync_en(reg32)              REG_FLD_GET(DBG_ERR_CTRL_FLD_tx_udrn_resync_en, (reg32))
#define DBG_ERR_CTRL_GET_tx_late_resync_en(reg32)              REG_FLD_GET(DBG_ERR_CTRL_FLD_tx_late_resync_en, (reg32))
#define DBG_ERR_CTRL_GET_bst_sgl_diff_resync_en(reg32)         REG_FLD_GET(DBG_ERR_CTRL_FLD_bst_sgl_diff_resync_en, (reg32))
#define DBG_ERR_CTRL_GET_ignore_ploamd_crc(reg32)              REG_FLD_GET(DBG_ERR_CTRL_FLD_ignore_ploamd_crc, (reg32))

#define DBG_SW_RDI_CTRL_GET_rdi_en(reg32)                      REG_FLD_GET(DBG_SW_RDI_CTRL_FLD_rdi_en, (reg32))
#define DBG_SW_RDI_CTRL_GET_sw_rdi(reg32)                      REG_FLD_GET(DBG_SW_RDI_CTRL_FLD_sw_rdi, (reg32))

#define DBG_RX_GEM_CNT_GET_dbg_rx_gem_cnt(reg32)               REG_FLD_GET(DBG_RX_GEM_CNT_FLD_dbg_rx_gem_cnt, (reg32))

#define DBG_RX_CRC_ERR_CNT_GET_dbg_rx_crc_cnt(reg32)           REG_FLD_GET(DBG_RX_CRC_ERR_CNT_FLD_dbg_rx_crc_cnt, (reg32))

#define DBG_RX_GTC_CNT_GET_dbg_rx_gtc_cnt(reg32)               REG_FLD_GET(DBG_RX_GTC_CNT_FLD_dbg_rx_gtc_cnt, (reg32))

#define DBG_TX_GEM_CNT_GET_dbg_tx_gem_cnt(reg32)               REG_FLD_GET(DBG_TX_GEM_CNT_FLD_dbg_tx_gem_cnt, (reg32))

#define DBG_TX_BST_CNT_GET_dbg_tx_burst_cnt(reg32)             REG_FLD_GET(DBG_TX_BST_CNT_FLD_dbg_tx_burst_cnt, (reg32))

#define DBG_GEM_HEC_ONE_ERR_CNT_GET_dbg_gem_one_err_cnt(reg32) REG_FLD_GET(DBG_GEM_HEC_ONE_ERR_CNT_FLD_dbg_gem_one_err_cnt, (reg32))

#define DBG_GEM_HEC_TWO_ERR_CNT_GET_dbg_gem_two_err_cnt(reg32) REG_FLD_GET(DBG_GEM_HEC_TWO_ERR_CNT_FLD_dbg_gem_two_err_cnt, (reg32))

#define DBG_GEM_HEC_UC_ERR_CNT_GET_dbg_gem_uc_err_cnt(reg32)   REG_FLD_GET(DBG_GEM_HEC_UC_ERR_CNT_FLD_dbg_gem_uc_err_cnt, (reg32))

#define DBG_DBA_BACK_DOOR_GET_dbg_sw_rpt_dba_en(reg32)         REG_FLD_GET(DBG_DBA_BACK_DOOR_FLD_dbg_sw_rpt_dba_en, (reg32))
#define DBG_DBA_BACK_DOOR_GET_dbg_slight_modify_en(reg32)      REG_FLD_GET(DBG_DBA_BACK_DOOR_FLD_dbg_slight_modify_en, (reg32))

#define DBG_DBA_TOTAL_BUF_SIZE_GET_dbg_dba_total_buf_size(reg32) REG_FLD_GET(DBG_DBA_TOTAL_BUF_SIZE_FLD_dbg_dba_total_buf_size, (reg32))

#define DBG_DBA_GREEN_SIZE_GET_dbg_dba_green_size(reg32)       REG_FLD_GET(DBG_DBA_GREEN_SIZE_FLD_dbg_dba_green_size, (reg32))

#define DBG_DBA_YELLOW_SIZE_GET_dbg_dba_yellow_size(reg32)     REG_FLD_GET(DBG_DBA_YELLOW_SIZE_FLD_dbg_dba_yellow_size, (reg32))

#define DBG_SLIGHT_MODIFY_SIZE_1_GET_dbg_yellow_slight_modify_size(reg32) REG_FLD_GET(DBG_SLIGHT_MODIFY_SIZE_1_FLD_dbg_yellow_slight_modify_size, (reg32))
#define DBG_SLIGHT_MODIFY_SIZE_1_GET_dbg_green_slight_modify_size(reg32) REG_FLD_GET(DBG_SLIGHT_MODIFY_SIZE_1_FLD_dbg_green_slight_modify_size, (reg32))

#define DBG_SLIGHT_MODIFY_SIZE_2_GET_dbg_total_slight_modify_size(reg32) REG_FLD_GET(DBG_SLIGHT_MODIFY_SIZE_2_FLD_dbg_total_slight_modify_size, (reg32))

#define DBG_DS_SPF_CNT_GET_dbg_ds_spf_cnt(reg32)               REG_FLD_GET(DBG_DS_SPF_CNT_FLD_dbg_ds_spf_cnt, (reg32))

#define DBG_TX_SYNC_OFFSET_GET_dbg_tx_sync_offset(reg32)       REG_FLD_GET(DBG_TX_SYNC_OFFSET_FLD_dbg_tx_sync_offset, (reg32))

#define DBG_PLOAMD_FILTER_IN_O5_GET_dbg_ext_bst_len_filter_in_o5_en(reg32) REG_FLD_GET(DBG_PLOAMD_FILTER_IN_O5_FLD_dbg_ext_bst_len_filter_in_o5_en, (reg32))
#define DBG_PLOAMD_FILTER_IN_O5_GET_dbg_us_overhead_filter_in_o5_en(reg32) REG_FLD_GET(DBG_PLOAMD_FILTER_IN_O5_FLD_dbg_us_overhead_filter_in_o5_en, (reg32))

#define DBG_SW_RESYNC_GET_dbg_sw_resync_en(reg32)              REG_FLD_GET(DBG_SW_RESYNC_FLD_dbg_sw_resync_en, (reg32))
#define DBG_SW_RESYNC_GET_dbg_sw_resync(reg32)                 REG_FLD_GET(DBG_SW_RESYNC_FLD_dbg_sw_resync, (reg32))

#define DBG_GTC_ETH_EXTR_GET_sniff_dbg_tx_rst(reg32)           REG_FLD_GET(DBG_GTC_ETH_EXTR_FLD_sniff_dbg_tx_rst, (reg32))
#define DBG_GTC_ETH_EXTR_GET_sniff_pkt_padding_en(reg32)       REG_FLD_GET(DBG_GTC_ETH_EXTR_FLD_sniff_pkt_padding_en, (reg32))
#define DBG_GTC_ETH_EXTR_GET_dbg_gtc_eth_extr_en(reg32)        REG_FLD_GET(DBG_GTC_ETH_EXTR_FLD_dbg_gtc_eth_extr_en, (reg32))

#define DBG_DS_GTC_EXTR_ETH_HDR_GET_dbg_extr_rx_ds_eth_type(reg32) REG_FLD_GET(DBG_DS_GTC_EXTR_ETH_HDR_FLD_dbg_extr_rx_ds_eth_type, (reg32))
#define DBG_DS_GTC_EXTR_ETH_HDR_GET_dbg_extr_rx_ds_sp(reg32)   REG_FLD_GET(DBG_DS_GTC_EXTR_ETH_HDR_FLD_dbg_extr_rx_ds_sp, (reg32))

#define DBG_US_GTC_EXTR_ETH_HDR_GET_dbg_extr_tx_us_eth_type(reg32) REG_FLD_GET(DBG_US_GTC_EXTR_ETH_HDR_FLD_dbg_extr_tx_us_eth_type, (reg32))
#define DBG_US_GTC_EXTR_ETH_HDR_GET_dbg_extr_tx_us_sp(reg32)   REG_FLD_GET(DBG_US_GTC_EXTR_ETH_HDR_FLD_dbg_extr_tx_us_sp, (reg32))

#define DBG_DS_GTC_EXTR_ETH_CNT_GET_dbg_extr_rx_cnt(reg32)     REG_FLD_GET(DBG_DS_GTC_EXTR_ETH_CNT_FLD_dbg_extr_rx_cnt, (reg32))

#define DBG_US_GTC_EXTR_ETH_CNT_GET_dbg_extr_tx_cnt(reg32)     REG_FLD_GET(DBG_US_GTC_EXTR_ETH_CNT_FLD_dbg_extr_tx_cnt, (reg32))

#define SNIFF_GTC_GTC_INVLD_GEM_BYTE_GET_dbg_ds_all_gem_filter(reg32) REG_FLD_GET(SNIFF_GTC_GTC_INVLD_GEM_BYTE_FLD_dbg_ds_all_gem_filter, (reg32))
#define SNIFF_GTC_GTC_INVLD_GEM_BYTE_GET_dbg_ds_gem_filter_exclude_omci(reg32) REG_FLD_GET(SNIFF_GTC_GTC_INVLD_GEM_BYTE_FLD_dbg_ds_gem_filter_exclude_omci, (reg32))
#define SNIFF_GTC_GTC_INVLD_GEM_BYTE_GET_dbg_us_gtc_invld_gem_byte(reg32) REG_FLD_GET(SNIFF_GTC_GTC_INVLD_GEM_BYTE_FLD_dbg_us_gtc_invld_gem_byte, (reg32))

#define DBG_SNIFF_DBG_REG_GET_dbg_mbi_pl_fifo_write_conflict(reg32) REG_FLD_GET(DBG_SNIFF_DBG_REG_FLD_dbg_mbi_pl_fifo_write_conflict, (reg32))
#define DBG_SNIFF_DBG_REG_GET_dbg_mbi_hdr_fifo_write_conflict(reg32) REG_FLD_GET(DBG_SNIFF_DBG_REG_FLD_dbg_mbi_hdr_fifo_write_conflict, (reg32))
#define DBG_SNIFF_DBG_REG_GET_dbg_us_gtc_info_fifo_ovrn(reg32) REG_FLD_GET(DBG_SNIFF_DBG_REG_FLD_dbg_us_gtc_info_fifo_ovrn, (reg32))

#define DBG_SNIFF_US_INFO_FIFO_GET_dbg_us_info_tx_send_depth(reg32) REG_FLD_GET(DBG_SNIFF_US_INFO_FIFO_FLD_dbg_us_info_tx_send_depth, (reg32))

#define SNIFF_US_HDR_WIDO_GET_stop_time_of_wido_en(reg32)      REG_FLD_GET(SNIFF_US_HDR_WIDO_FLD_stop_time_of_wido_en, (reg32))
#define SNIFF_US_HDR_WIDO_GET_stop_time_of_wido(reg32)         REG_FLD_GET(SNIFF_US_HDR_WIDO_FLD_stop_time_of_wido, (reg32))

#define SNIFF_TX_DA_SA_GET_tx_us_da_h16(reg32)                 REG_FLD_GET(SNIFF_TX_DA_SA_FLD_tx_us_da_h16, (reg32))
#define SNIFF_TX_DA_SA_GET_tx_us_sa_h16(reg32)                 REG_FLD_GET(SNIFF_TX_DA_SA_FLD_tx_us_sa_h16, (reg32))

#define SNIFF_RX_DA_SA_GET_rx_ds_da_h16(reg32)                 REG_FLD_GET(SNIFF_RX_DA_SA_FLD_rx_ds_da_h16, (reg32))
#define SNIFF_RX_DA_SA_GET_rx_ds_sa_h16(reg32)                 REG_FLD_GET(SNIFF_RX_DA_SA_FLD_rx_ds_sa_h16, (reg32))

#define SNIFF_TX_RX_PID_GET_tx_eth_pid(reg32)                  REG_FLD_GET(SNIFF_TX_RX_PID_FLD_tx_eth_pid, (reg32))
#define SNIFF_TX_RX_PID_GET_rx_eth_pid(reg32)                  REG_FLD_GET(SNIFF_TX_RX_PID_FLD_rx_eth_pid, (reg32))

#define SNIFF_CNT_EN_GET_sniff_cnt_en(reg32)                   REG_FLD_GET(SNIFF_CNT_EN_FLD_sniff_cnt_en, (reg32))

#define SNIFF_RX_TX_TPID_GET_sniff_rx_tpid(reg32)              REG_FLD_GET(SNIFF_RX_TX_TPID_FLD_sniff_rx_tpid, (reg32))
#define SNIFF_RX_TX_TPID_GET_sniff_tx_tpid(reg32)              REG_FLD_GET(SNIFF_RX_TX_TPID_FLD_sniff_tx_tpid, (reg32))

#define DBG_GPON_MAC_SET_GET_asb_tx_eth_4byte_align_en(reg32)  REG_FLD_GET(DBG_GPON_MAC_SET_FLD_asb_tx_eth_4byte_align_en, (reg32))
#define DBG_GPON_MAC_SET_GET_olt_ds_fec(reg32)                 REG_FLD_GET(DBG_GPON_MAC_SET_FLD_olt_ds_fec, (reg32))
#define DBG_GPON_MAC_SET_GET_onu_us_fec(reg32)                 REG_FLD_GET(DBG_GPON_MAC_SET_FLD_onu_us_fec, (reg32))
#define DBG_GPON_MAC_SET_GET_gpon_mac_sw_rst_n(reg32)          REG_FLD_GET(DBG_GPON_MAC_SET_FLD_gpon_mac_sw_rst_n, (reg32))

#define SLEEP_GLB_CFG_GET_sleep_filt_all_bwm(reg32)            REG_FLD_GET(SLEEP_GLB_CFG_FLD_sleep_filt_all_bwm, (reg32))
#define SLEEP_GLB_CFG_GET_reload_sleep_cnt(reg32)              REG_FLD_GET(SLEEP_GLB_CFG_FLD_reload_sleep_cnt, (reg32))

#define SLEEP_CNT_GET_sleep_cnt(reg32)                         REG_FLD_GET(SLEEP_CNT_FLD_sleep_cnt, (reg32))

#define TX_SYNC_OPT_GET_tx_sync_opt(reg32)                     REG_FLD_GET(TX_SYNC_OPT_FLD_tx_sync_opt, (reg32))

#define PLOAMU_CTRL_GET_ploamu_ind_ctrl(reg32)                 REG_FLD_GET(PLOAMU_CTRL_FLD_ploamu_ind_ctrl, (reg32))
#define PLOAMU_CTRL_GET_o3_o4_ploamu_ctrl(reg32)               REG_FLD_GET(PLOAMU_CTRL_FLD_o3_o4_ploamu_ctrl, (reg32))

#define TOD_1PPS_WD_CTRL_GET_tod_1pps_width_ctrl(reg32)        REG_FLD_GET(TOD_1PPS_WD_CTRL_FLD_tod_1pps_width_ctrl, (reg32))

#define G_ONU_ID_SET_onu_id_vld(reg32, val)                    REG_FLD_SET(G_ONU_ID_FLD_onu_id_vld, (reg32), (val))
#define G_ONU_ID_SET_onu_id(reg32, val)                        REG_FLD_SET(G_ONU_ID_FLD_onu_id, (reg32), (val))

#define G_GBL_CFG_SET_us_fec_en(reg32, val)                    REG_FLD_SET(G_GBL_CFG_FLD_us_fec_en, (reg32), (val))
#define G_GBL_CFG_SET_sr_blk_size(reg32, val)                  REG_FLD_SET(G_GBL_CFG_FLD_sr_blk_size, (reg32), (val))

#define G_INT_STATUS_SET_bwm_us_fec_err_int(reg32, val)        REG_FLD_SET(G_INT_STATUS_FLD_bwm_us_fec_err_int, (reg32), (val))
#define G_INT_STATUS_SET_bwm_stop_time_err_int(reg32, val)     REG_FLD_SET(G_INT_STATUS_FLD_bwm_stop_time_err_int, (reg32), (val))
#define G_INT_STATUS_SET_lwi_int(reg32, val)                   REG_FLD_SET(G_INT_STATUS_FLD_lwi_int, (reg32), (val))
#define G_INT_STATUS_SET_fwi_int(reg32, val)                   REG_FLD_SET(G_INT_STATUS_FLD_fwi_int, (reg32), (val))
#define G_INT_STATUS_SET_pop_up_recv_in_O6_int(reg32, val)     REG_FLD_SET(G_INT_STATUS_FLD_pop_up_recv_in_O6_int, (reg32), (val))
#define G_INT_STATUS_SET_onu_us_fec_chg_int(reg32, val)        REG_FLD_SET(G_INT_STATUS_FLD_onu_us_fec_chg_int, (reg32), (val))
#define G_INT_STATUS_SET_olt_ds_fec_chg_int(reg32, val)        REG_FLD_SET(G_INT_STATUS_FLD_olt_ds_fec_chg_int, (reg32), (val))
#define G_INT_STATUS_SET_o5_eqd_adj_done_int(reg32, val)       REG_FLD_SET(G_INT_STATUS_FLD_o5_eqd_adj_done_int, (reg32), (val))
#define G_INT_STATUS_SET_sfifo_full_int(reg32, val)            REG_FLD_SET(G_INT_STATUS_FLD_sfifo_full_int, (reg32), (val))
#define G_INT_STATUS_SET_bfifo_full_int(reg32, val)            REG_FLD_SET(G_INT_STATUS_FLD_bfifo_full_int, (reg32), (val))
#define G_INT_STATUS_SET_rx_gem_intlv_err_int(reg32, val)      REG_FLD_SET(G_INT_STATUS_FLD_rx_gem_intlv_err_int, (reg32), (val))
#define G_INT_STATUS_SET_rx_eof_err_int(reg32, val)            REG_FLD_SET(G_INT_STATUS_FLD_rx_eof_err_int, (reg32), (val))
#define G_INT_STATUS_SET_tx_late_start_int(reg32, val)         REG_FLD_SET(G_INT_STATUS_FLD_tx_late_start_int, (reg32), (val))
#define G_INT_STATUS_SET_bst_sgl_diff_int(reg32, val)          REG_FLD_SET(G_INT_STATUS_FLD_bst_sgl_diff_int, (reg32), (val))
#define G_INT_STATUS_SET_fifo_err_int(reg32, val)              REG_FLD_SET(G_INT_STATUS_FLD_fifo_err_int, (reg32), (val))
#define G_INT_STATUS_SET_rx_err_int(reg32, val)                REG_FLD_SET(G_INT_STATUS_FLD_rx_err_int, (reg32), (val))
#define G_INT_STATUS_SET_dying_gasp_send_int(reg32, val)       REG_FLD_SET(G_INT_STATUS_FLD_dying_gasp_send_int, (reg32), (val))
#define G_INT_STATUS_SET_tod_1pps_int(reg32, val)              REG_FLD_SET(G_INT_STATUS_FLD_tod_1pps_int, (reg32), (val))
#define G_INT_STATUS_SET_tod_update_done_int(reg32, val)       REG_FLD_SET(G_INT_STATUS_FLD_tod_update_done_int, (reg32), (val))
#define G_INT_STATUS_SET_aes_key_switch_done_int(reg32, val)   REG_FLD_SET(G_INT_STATUS_FLD_aes_key_switch_done_int, (reg32), (val))
#define G_INT_STATUS_SET_los_gem_del_int(reg32, val)           REG_FLD_SET(G_INT_STATUS_FLD_los_gem_del_int, (reg32), (val))
#define G_INT_STATUS_SET_sn_req_crs_int(reg32, val)            REG_FLD_SET(G_INT_STATUS_FLD_sn_req_crs_int, (reg32), (val))
#define G_INT_STATUS_SET_sn_onu_send_o4_int(reg32, val)        REG_FLD_SET(G_INT_STATUS_FLD_sn_onu_send_o4_int, (reg32), (val))
#define G_INT_STATUS_SET_ranging_req_recv_int(reg32, val)      REG_FLD_SET(G_INT_STATUS_FLD_ranging_req_recv_int, (reg32), (val))
#define G_INT_STATUS_SET_sn_onu_send_o3_int(reg32, val)        REG_FLD_SET(G_INT_STATUS_FLD_sn_onu_send_o3_int, (reg32), (val))
#define G_INT_STATUS_SET_sn_req_recv_int(reg32, val)           REG_FLD_SET(G_INT_STATUS_FLD_sn_req_recv_int, (reg32), (val))
#define G_INT_STATUS_SET_ploamu_send_int(reg32, val)           REG_FLD_SET(G_INT_STATUS_FLD_ploamu_send_int, (reg32), (val))
#define G_INT_STATUS_SET_ploamd_recv_int(reg32, val)           REG_FLD_SET(G_INT_STATUS_FLD_ploamd_recv_int, (reg32), (val))

#define G_INT_ENABLE_SET_bwm_us_fec_err_int_en(reg32, val)     REG_FLD_SET(G_INT_ENABLE_FLD_bwm_us_fec_err_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_bwm_stop_time_err_int_en(reg32, val)  REG_FLD_SET(G_INT_ENABLE_FLD_bwm_stop_time_err_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_lwi_int_en(reg32, val)                REG_FLD_SET(G_INT_ENABLE_FLD_lwi_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_fwi_int_en(reg32, val)                REG_FLD_SET(G_INT_ENABLE_FLD_fwi_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_pop_up_recv_in_O6_int_en(reg32, val)  REG_FLD_SET(G_INT_ENABLE_FLD_pop_up_recv_in_O6_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_onu_us_fec_chg_int_en(reg32, val)     REG_FLD_SET(G_INT_ENABLE_FLD_onu_us_fec_chg_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_olt_ds_fec_chg_int_en(reg32, val)     REG_FLD_SET(G_INT_ENABLE_FLD_olt_ds_fec_chg_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_o5_eqd_adj_done_int_en(reg32, val)    REG_FLD_SET(G_INT_ENABLE_FLD_o5_eqd_adj_done_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_sfifo_full_int_en(reg32, val)         REG_FLD_SET(G_INT_ENABLE_FLD_sfifo_full_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_bfifo_full_int_en(reg32, val)         REG_FLD_SET(G_INT_ENABLE_FLD_bfifo_full_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_rx_gem_intlv_err_int_en(reg32, val)   REG_FLD_SET(G_INT_ENABLE_FLD_rx_gem_intlv_err_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_rx_eof_err_int_en(reg32, val)         REG_FLD_SET(G_INT_ENABLE_FLD_rx_eof_err_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_tx_late_start_int_en(reg32, val)      REG_FLD_SET(G_INT_ENABLE_FLD_tx_late_start_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_bst_sgl_diff_int_en(reg32, val)       REG_FLD_SET(G_INT_ENABLE_FLD_bst_sgl_diff_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_fifo_err_int_en(reg32, val)           REG_FLD_SET(G_INT_ENABLE_FLD_fifo_err_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_rx_err_int_en(reg32, val)             REG_FLD_SET(G_INT_ENABLE_FLD_rx_err_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_dying_gasp_send_int_en(reg32, val)    REG_FLD_SET(G_INT_ENABLE_FLD_dying_gasp_send_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_tod_1pps_int_en(reg32, val)           REG_FLD_SET(G_INT_ENABLE_FLD_tod_1pps_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_tod_update_done_int_en(reg32, val)    REG_FLD_SET(G_INT_ENABLE_FLD_tod_update_done_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_aes_key_switch_done_int_en(reg32, val) REG_FLD_SET(G_INT_ENABLE_FLD_aes_key_switch_done_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_los_gem_del_int_en(reg32, val)        REG_FLD_SET(G_INT_ENABLE_FLD_los_gem_del_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_sn_req_crs_int_en(reg32, val)         REG_FLD_SET(G_INT_ENABLE_FLD_sn_req_crs_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_sn_onu_send_o4_int_en(reg32, val)     REG_FLD_SET(G_INT_ENABLE_FLD_sn_onu_send_o4_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_ranging_req_recv_int_en(reg32, val)   REG_FLD_SET(G_INT_ENABLE_FLD_ranging_req_recv_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_sn_onu_send_o3_int_en(reg32, val)     REG_FLD_SET(G_INT_ENABLE_FLD_sn_onu_send_o3_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_sn_req_recv_int_en(reg32, val)        REG_FLD_SET(G_INT_ENABLE_FLD_sn_req_recv_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_ploamu_send_int_en(reg32, val)        REG_FLD_SET(G_INT_ENABLE_FLD_ploamu_send_int_en, (reg32), (val))
#define G_INT_ENABLE_SET_ploamd_recv_int_en(reg32, val)        REG_FLD_SET(G_INT_ENABLE_FLD_ploamd_recv_int_en, (reg32), (val))

#define G_TCONT_ID_0_1_SET_t_cont1_vld(reg32, val)             REG_FLD_SET(G_TCONT_ID_0_1_FLD_t_cont1_vld, (reg32), (val))
#define G_TCONT_ID_0_1_SET_t_cont1_id(reg32, val)              REG_FLD_SET(G_TCONT_ID_0_1_FLD_t_cont1_id, (reg32), (val))
#define G_TCONT_ID_0_1_SET_t_cont0_vld(reg32, val)             REG_FLD_SET(G_TCONT_ID_0_1_FLD_t_cont0_vld, (reg32), (val))
#define G_TCONT_ID_0_1_SET_t_cont0_id(reg32, val)              REG_FLD_SET(G_TCONT_ID_0_1_FLD_t_cont0_id, (reg32), (val))

#define G_TCONT_ID_2_3_SET_t_cont3_vld(reg32, val)             REG_FLD_SET(G_TCONT_ID_2_3_FLD_t_cont3_vld, (reg32), (val))
#define G_TCONT_ID_2_3_SET_t_cont3_id(reg32, val)              REG_FLD_SET(G_TCONT_ID_2_3_FLD_t_cont3_id, (reg32), (val))
#define G_TCONT_ID_2_3_SET_t_cont2_vld(reg32, val)             REG_FLD_SET(G_TCONT_ID_2_3_FLD_t_cont2_vld, (reg32), (val))
#define G_TCONT_ID_2_3_SET_t_cont2_id(reg32, val)              REG_FLD_SET(G_TCONT_ID_2_3_FLD_t_cont2_id, (reg32), (val))

#define G_TCONT_ID_4_5_SET_t_cont5_vld(reg32, val)             REG_FLD_SET(G_TCONT_ID_4_5_FLD_t_cont5_vld, (reg32), (val))
#define G_TCONT_ID_4_5_SET_t_cont5_id(reg32, val)              REG_FLD_SET(G_TCONT_ID_4_5_FLD_t_cont5_id, (reg32), (val))
#define G_TCONT_ID_4_5_SET_t_cont4_vld(reg32, val)             REG_FLD_SET(G_TCONT_ID_4_5_FLD_t_cont4_vld, (reg32), (val))
#define G_TCONT_ID_4_5_SET_t_cont4_id(reg32, val)              REG_FLD_SET(G_TCONT_ID_4_5_FLD_t_cont4_id, (reg32), (val))

#define G_TCONT_ID_6_7_SET_t_cont7_vld(reg32, val)             REG_FLD_SET(G_TCONT_ID_6_7_FLD_t_cont7_vld, (reg32), (val))
#define G_TCONT_ID_6_7_SET_t_cont7_id(reg32, val)              REG_FLD_SET(G_TCONT_ID_6_7_FLD_t_cont7_id, (reg32), (val))
#define G_TCONT_ID_6_7_SET_t_cont6_vld(reg32, val)             REG_FLD_SET(G_TCONT_ID_6_7_FLD_t_cont6_vld, (reg32), (val))
#define G_TCONT_ID_6_7_SET_t_cont6_id(reg32, val)              REG_FLD_SET(G_TCONT_ID_6_7_FLD_t_cont6_id, (reg32), (val))

#define G_TCONT_ID_8_9_SET_t_cont9_vld(reg32, val)             REG_FLD_SET(G_TCONT_ID_8_9_FLD_t_cont9_vld, (reg32), (val))
#define G_TCONT_ID_8_9_SET_t_cont9_id(reg32, val)              REG_FLD_SET(G_TCONT_ID_8_9_FLD_t_cont9_id, (reg32), (val))
#define G_TCONT_ID_8_9_SET_t_cont8_vld(reg32, val)             REG_FLD_SET(G_TCONT_ID_8_9_FLD_t_cont8_vld, (reg32), (val))
#define G_TCONT_ID_8_9_SET_t_cont8_id(reg32, val)              REG_FLD_SET(G_TCONT_ID_8_9_FLD_t_cont8_id, (reg32), (val))

#define G_TCONT_ID_10_11_SET_t_cont11_vld(reg32, val)          REG_FLD_SET(G_TCONT_ID_10_11_FLD_t_cont11_vld, (reg32), (val))
#define G_TCONT_ID_10_11_SET_t_cont11_id(reg32, val)           REG_FLD_SET(G_TCONT_ID_10_11_FLD_t_cont11_id, (reg32), (val))
#define G_TCONT_ID_10_11_SET_t_cont10_vld(reg32, val)          REG_FLD_SET(G_TCONT_ID_10_11_FLD_t_cont10_vld, (reg32), (val))
#define G_TCONT_ID_10_11_SET_t_cont10_id(reg32, val)           REG_FLD_SET(G_TCONT_ID_10_11_FLD_t_cont10_id, (reg32), (val))

#define G_TCONT_ID_12_13_SET_t_cont13_vld(reg32, val)          REG_FLD_SET(G_TCONT_ID_12_13_FLD_t_cont13_vld, (reg32), (val))
#define G_TCONT_ID_12_13_SET_t_cont13_id(reg32, val)           REG_FLD_SET(G_TCONT_ID_12_13_FLD_t_cont13_id, (reg32), (val))
#define G_TCONT_ID_12_13_SET_t_cont12_vld(reg32, val)          REG_FLD_SET(G_TCONT_ID_12_13_FLD_t_cont12_vld, (reg32), (val))
#define G_TCONT_ID_12_13_SET_t_cont12_id(reg32, val)           REG_FLD_SET(G_TCONT_ID_12_13_FLD_t_cont12_id, (reg32), (val))

#define G_TCONT_ID_14_15_SET_t_cont15_vld(reg32, val)          REG_FLD_SET(G_TCONT_ID_14_15_FLD_t_cont15_vld, (reg32), (val))
#define G_TCONT_ID_14_15_SET_t_cont15_id(reg32, val)           REG_FLD_SET(G_TCONT_ID_14_15_FLD_t_cont15_id, (reg32), (val))
#define G_TCONT_ID_14_15_SET_t_cont14_vld(reg32, val)          REG_FLD_SET(G_TCONT_ID_14_15_FLD_t_cont14_vld, (reg32), (val))
#define G_TCONT_ID_14_15_SET_t_cont14_id(reg32, val)           REG_FLD_SET(G_TCONT_ID_14_15_FLD_t_cont14_id, (reg32), (val))

#define G_GEM_PORT_CFG_SET_gpid_cmd(reg32, val)                REG_FLD_SET(G_GEM_PORT_CFG_FLD_gpid_cmd, (reg32), (val))
#define G_GEM_PORT_CFG_SET_gpid_encrypt(reg32, val)            REG_FLD_SET(G_GEM_PORT_CFG_FLD_gpid_encrypt, (reg32), (val))
#define G_GEM_PORT_CFG_SET_gpid_vld(reg32, val)                REG_FLD_SET(G_GEM_PORT_CFG_FLD_gpid_vld, (reg32), (val))
#define G_GEM_PORT_CFG_SET_gem_port_id(reg32, val)             REG_FLD_SET(G_GEM_PORT_CFG_FLD_gem_port_id, (reg32), (val))

#define G_GEM_PORT_STS_SET_gpid_cmd_done(reg32, val)           REG_FLD_SET(G_GEM_PORT_STS_FLD_gpid_cmd_done, (reg32), (val))
#define G_GEM_PORT_STS_SET_gpid_encrypt_sts(reg32, val)        REG_FLD_SET(G_GEM_PORT_STS_FLD_gpid_encrypt_sts, (reg32), (val))
#define G_GEM_PORT_STS_SET_gpid_vld_sts(reg32, val)            REG_FLD_SET(G_GEM_PORT_STS_FLD_gpid_vld_sts, (reg32), (val))

#define G_OMCI_ID_SET_omci_port_id_vld(reg32, val)             REG_FLD_SET(G_OMCI_ID_FLD_omci_port_id_vld, (reg32), (val))
#define G_OMCI_ID_SET_omci_gpid(reg32, val)                    REG_FLD_SET(G_OMCI_ID_FLD_omci_gpid, (reg32), (val))

#define G_GEM_TBL_INIT_SET_gem_tbl_init_done(reg32, val)       REG_FLD_SET(G_GEM_TBL_INIT_FLD_gem_tbl_init_done, (reg32), (val))
#define G_GEM_TBL_INIT_SET_gem_tbl_init_start(reg32, val)      REG_FLD_SET(G_GEM_TBL_INIT_FLD_gem_tbl_init_start, (reg32), (val))

#define G_PLOAMu_FIFO_STS_SET_ploamu_fifo_udrn(reg32, val)     REG_FLD_SET(G_PLOAMu_FIFO_STS_FLD_ploamu_fifo_udrn, (reg32), (val))
#define G_PLOAMu_FIFO_STS_SET_ploamu_fifo_min_avail(reg32, val) REG_FLD_SET(G_PLOAMu_FIFO_STS_FLD_ploamu_fifo_min_avail, (reg32), (val))
#define G_PLOAMu_FIFO_STS_SET_ploamu_fifo_avail(reg32, val)    REG_FLD_SET(G_PLOAMu_FIFO_STS_FLD_ploamu_fifo_avail, (reg32), (val))

#define G_PLOAMu_WDATA_SET_ploamu_wdata(reg32, val)            REG_FLD_SET(G_PLOAMu_WDATA_FLD_ploamu_wdata, (reg32), (val))

#define G_PLOAMd_FIFO_STS_SET_ploamd_fifo_ovrn(reg32, val)     REG_FLD_SET(G_PLOAMd_FIFO_STS_FLD_ploamd_fifo_ovrn, (reg32), (val))
#define G_PLOAMd_FIFO_STS_SET_ploamd_fifo_max_used(reg32, val) REG_FLD_SET(G_PLOAMd_FIFO_STS_FLD_ploamd_fifo_max_used, (reg32), (val))
#define G_PLOAMd_FIFO_STS_SET_ploamd_fifo_used(reg32, val)     REG_FLD_SET(G_PLOAMd_FIFO_STS_FLD_ploamd_fifo_used, (reg32), (val))

#define G_PLOAMd_RDATA_SET_ploamd_rdata(reg32, val)            REG_FLD_SET(G_PLOAMd_RDATA_FLD_ploamd_rdata, (reg32), (val))

#define G_AES_CFG_SET_aes_spf_cnt(reg32, val)                  REG_FLD_SET(G_AES_CFG_FLD_aes_spf_cnt, (reg32), (val))

#define G_AES_ACTIVE_KEY0_SET_aes_active_key0(reg32, val)      REG_FLD_SET(G_AES_ACTIVE_KEY0_FLD_aes_active_key0, (reg32), (val))

#define G_AES_ACTIVE_KEY1_SET_aes_active_key1(reg32, val)      REG_FLD_SET(G_AES_ACTIVE_KEY1_FLD_aes_active_key1, (reg32), (val))

#define G_AES_ACTIVE_KEY2_SET_aes_active_key2(reg32, val)      REG_FLD_SET(G_AES_ACTIVE_KEY2_FLD_aes_active_key2, (reg32), (val))

#define G_AES_ACTIVE_KEY3_SET_aes_active_key3(reg32, val)      REG_FLD_SET(G_AES_ACTIVE_KEY3_FLD_aes_active_key3, (reg32), (val))

#define G_AES_SHADOW_KEY0_SET_aes_shadow_key0(reg32, val)      REG_FLD_SET(G_AES_SHADOW_KEY0_FLD_aes_shadow_key0, (reg32), (val))

#define G_AES_SHADOW_KEY1_SET_aes_shadow_key1(reg32, val)      REG_FLD_SET(G_AES_SHADOW_KEY1_FLD_aes_shadow_key1, (reg32), (val))

#define G_AES_SHADOW_KEY2_SET_aes_shadow_key2(reg32, val)      REG_FLD_SET(G_AES_SHADOW_KEY2_FLD_aes_shadow_key2, (reg32), (val))

#define G_AES_SHADOW_KEY3_SET_aes_shadow_key3(reg32, val)      REG_FLD_SET(G_AES_SHADOW_KEY3_FLD_aes_shadow_key3, (reg32), (val))

#define G_AES_KEY_SWITCH_BY_SW_SET_aes_key_switch_by_sw_done(reg32, val) REG_FLD_SET(G_AES_KEY_SWITCH_BY_SW_FLD_aes_key_switch_by_sw_done, (reg32), (val))
#define G_AES_KEY_SWITCH_BY_SW_SET_aes_key_switch_by_sw_cmd(reg32, val) REG_FLD_SET(G_AES_KEY_SWITCH_BY_SW_FLD_aes_key_switch_by_sw_cmd, (reg32), (val))

#define G_PLOu_OVERHEAD_SET_plou_overhead(reg32, val)          REG_FLD_SET(G_PLOu_OVERHEAD_FLD_plou_overhead, (reg32), (val))

#define G_PLOu_GUARD_BIT_SET_guard_bit(reg32, val)             REG_FLD_SET(G_PLOu_GUARD_BIT_FLD_guard_bit, (reg32), (val))

#define G_PLOu_PRMBL_TYPE1_2_SET_prmb2_bit(reg32, val)         REG_FLD_SET(G_PLOu_PRMBL_TYPE1_2_FLD_prmb2_bit, (reg32), (val))
#define G_PLOu_PRMBL_TYPE1_2_SET_prmb1_bit(reg32, val)         REG_FLD_SET(G_PLOu_PRMBL_TYPE1_2_FLD_prmb1_bit, (reg32), (val))

#define G_PLOu_PRMBL_TYPE3_SET_ebl_en(reg32, val)              REG_FLD_SET(G_PLOu_PRMBL_TYPE3_FLD_ebl_en, (reg32), (val))
#define G_PLOu_PRMBL_TYPE3_SET_ext_prmb3_o5_num(reg32, val)    REG_FLD_SET(G_PLOu_PRMBL_TYPE3_FLD_ext_prmb3_o5_num, (reg32), (val))
#define G_PLOu_PRMBL_TYPE3_SET_ext_prmb3_o3_o4_num(reg32, val) REG_FLD_SET(G_PLOu_PRMBL_TYPE3_FLD_ext_prmb3_o3_o4_num, (reg32), (val))

#define G_PLOu_DELM_BIT_SET_delm_bit(reg32, val)               REG_FLD_SET(G_PLOu_DELM_BIT_FLD_delm_bit, (reg32), (val))

#define G_PRE_ASSIGNED_DLY_SET_pre_dly_en(reg32, val)          REG_FLD_SET(G_PRE_ASSIGNED_DLY_FLD_pre_dly_en, (reg32), (val))
#define G_PRE_ASSIGNED_DLY_SET_pre_dly(reg32, val)             REG_FLD_SET(G_PRE_ASSIGNED_DLY_FLD_pre_dly, (reg32), (val))

#define G_EQD_SET_eqd(reg32, val)                              REG_FLD_SET(G_EQD_FLD_eqd, (reg32), (val))

#define G_RSP_TIME_SET_tresp(reg32, val)                       REG_FLD_SET(G_RSP_TIME_FLD_tresp, (reg32), (val))

#define G_VENDOR_ID_SET_vendor_id(reg32, val)                  REG_FLD_SET(G_VENDOR_ID_FLD_vendor_id, (reg32), (val))

#define G_VS_SN_SET_vs_sn(reg32, val)                          REG_FLD_SET(G_VS_SN_FLD_vs_sn, (reg32), (val))

#define G_SN_MSG_CFG_SET_sn_req_thr(reg32, val)                REG_FLD_SET(G_SN_MSG_CFG_FLD_sn_req_thr, (reg32), (val))
#define G_SN_MSG_CFG_SET_tx_power_mode(reg32, val)             REG_FLD_SET(G_SN_MSG_CFG_FLD_tx_power_mode, (reg32), (val))
#define G_SN_MSG_CFG_SET_rdm_dly(reg32, val)                   REG_FLD_SET(G_SN_MSG_CFG_FLD_rdm_dly, (reg32), (val))

#define G_ACTIVATION_ST_SET_act_st(reg32, val)                 REG_FLD_SET(G_ACTIVATION_ST_FLD_act_st, (reg32), (val))

#define G_TOD_CFG_SET_tod_spf_cnt(reg32, val)                  REG_FLD_SET(G_TOD_CFG_FLD_tod_spf_cnt, (reg32), (val))

#define G_NEW_TOD_SEC_L32_SET_new_tod_sec_l32(reg32, val)      REG_FLD_SET(G_NEW_TOD_SEC_L32_FLD_new_tod_sec_l32, (reg32), (val))

#define G_NEW_TOD_NANO_SEC_SET_new_tod_nano_sec(reg32, val)    REG_FLD_SET(G_NEW_TOD_NANO_SEC_FLD_new_tod_nano_sec, (reg32), (val))

#define G_CUR_TOD_SEC_L32_SET_cur_tod_sec_l32(reg32, val)      REG_FLD_SET(G_CUR_TOD_SEC_L32_FLD_cur_tod_sec_l32, (reg32), (val))

#define G_CUR_TOD_NANO_SEC_SET_cur_tod_nano_sec(reg32, val)    REG_FLD_SET(G_CUR_TOD_NANO_SEC_FLD_cur_tod_nano_sec, (reg32), (val))

#define G_TOD_CLK_PERIOD_SET_tod_period(reg32, val)            REG_FLD_SET(G_TOD_CLK_PERIOD_FLD_tod_period, (reg32), (val))

#define G_TX_FCS_TBL_INIT_SET_tx_fcs_tbl_init_done(reg32, val) REG_FLD_SET(G_TX_FCS_TBL_INIT_FLD_tx_fcs_tbl_init_done, (reg32), (val))
#define G_TX_FCS_TBL_INIT_SET_tx_fcs_tbl_init_start(reg32, val) REG_FLD_SET(G_TX_FCS_TBL_INIT_FLD_tx_fcs_tbl_init_start, (reg32), (val))

#define G_MIB_CTRL_STS_SET_mib_cmd(reg32, val)                 REG_FLD_SET(G_MIB_CTRL_STS_FLD_mib_cmd, (reg32), (val))
#define G_MIB_CTRL_STS_SET_mib_cmd_done(reg32, val)            REG_FLD_SET(G_MIB_CTRL_STS_FLD_mib_cmd_done, (reg32), (val))
#define G_MIB_CTRL_STS_SET_mib_read_clr_en(reg32, val)         REG_FLD_SET(G_MIB_CTRL_STS_FLD_mib_read_clr_en, (reg32), (val))
#define G_MIB_CTRL_STS_SET_mib_type(reg32, val)                REG_FLD_SET(G_MIB_CTRL_STS_FLD_mib_type, (reg32), (val))
#define G_MIB_CTRL_STS_SET_mib_gpid(reg32, val)                REG_FLD_SET(G_MIB_CTRL_STS_FLD_mib_gpid, (reg32), (val))

#define G_MIB_RDATA_L32_SET_mib_rdata_l32(reg32, val)          REG_FLD_SET(G_MIB_RDATA_L32_FLD_mib_rdata_l32, (reg32), (val))

#define G_MIB_RDATA_H32_SET_mib_rdata_h32(reg32, val)          REG_FLD_SET(G_MIB_RDATA_H32_FLD_mib_rdata_h32, (reg32), (val))

#define G_MIB_WDATA_L32_SET_mib_wdata_l32(reg32, val)          REG_FLD_SET(G_MIB_WDATA_L32_FLD_mib_wdata_l32, (reg32), (val))

#define G_MIB_WDATA_H32_SET_mib_wdata_h32(reg32, val)          REG_FLD_SET(G_MIB_WDATA_H32_FLD_mib_wdata_h32, (reg32), (val))

#define G_MIB_TBL_INIT_SET_mib_tbl_init_done(reg32, val)       REG_FLD_SET(G_MIB_TBL_INIT_FLD_mib_tbl_init_done, (reg32), (val))
#define G_MIB_TBL_INIT_SET_mib_tbl_init_start(reg32, val)      REG_FLD_SET(G_MIB_TBL_INIT_FLD_mib_tbl_init_start, (reg32), (val))

#define G_GPIDX_TBL_CTRL_SET_gpidx_cmd(reg32, val)             REG_FLD_SET(G_GPIDX_TBL_CTRL_FLD_gpidx_cmd, (reg32), (val))
#define G_GPIDX_TBL_CTRL_SET_gpidx_tbl_addr(reg32, val)        REG_FLD_SET(G_GPIDX_TBL_CTRL_FLD_gpidx_tbl_addr, (reg32), (val))
#define G_GPIDX_TBL_CTRL_SET_gpidx_tbl_wdata(reg32, val)       REG_FLD_SET(G_GPIDX_TBL_CTRL_FLD_gpidx_tbl_wdata, (reg32), (val))

#define G_GPIDX_TBL_STS_SET_gpidx_cmd_done(reg32, val)         REG_FLD_SET(G_GPIDX_TBL_STS_FLD_gpidx_cmd_done, (reg32), (val))
#define G_GPIDX_TBL_STS_SET_gpidx_tbl_rdata(reg32, val)        REG_FLD_SET(G_GPIDX_TBL_STS_FLD_gpidx_tbl_rdata, (reg32), (val))

#define G_GPIDX_TBL_INIT_SET_gpidx_tbl_init_done(reg32, val)   REG_FLD_SET(G_GPIDX_TBL_INIT_FLD_gpidx_tbl_init_done, (reg32), (val))
#define G_GPIDX_TBL_INIT_SET_gpidx_tbl_init_start(reg32, val)  REG_FLD_SET(G_GPIDX_TBL_INIT_FLD_gpidx_tbl_init_start, (reg32), (val))

#define G_MBI_STOP_SET_mbi_tx_stop(reg32, val)                 REG_FLD_SET(G_MBI_STOP_FLD_mbi_tx_stop, (reg32), (val))
#define G_MBI_STOP_SET_mbi_rx_stop(reg32, val)                 REG_FLD_SET(G_MBI_STOP_FLD_mbi_rx_stop, (reg32), (val))

#define G_TCONT_ID_16_31_CFG_SET_tcont_cmd(reg32, val)         REG_FLD_SET(G_TCONT_ID_16_31_CFG_FLD_tcont_cmd, (reg32), (val))
#define G_TCONT_ID_16_31_CFG_SET_wr_tcont_id_vld(reg32, val)   REG_FLD_SET(G_TCONT_ID_16_31_CFG_FLD_wr_tcont_id_vld, (reg32), (val))
#define G_TCONT_ID_16_31_CFG_SET_tcont_id_index(reg32, val)    REG_FLD_SET(G_TCONT_ID_16_31_CFG_FLD_tcont_id_index, (reg32), (val))
#define G_TCONT_ID_16_31_CFG_SET_wr_tcont_id(reg32, val)       REG_FLD_SET(G_TCONT_ID_16_31_CFG_FLD_wr_tcont_id, (reg32), (val))

#define G_TCONT_ID_16_31_STS_SET_tcont_cmd_done(reg32, val)    REG_FLD_SET(G_TCONT_ID_16_31_STS_FLD_tcont_cmd_done, (reg32), (val))
#define G_TCONT_ID_16_31_STS_SET_rd_tcont_id_vld(reg32, val)   REG_FLD_SET(G_TCONT_ID_16_31_STS_FLD_rd_tcont_id_vld, (reg32), (val))
#define G_TCONT_ID_16_31_STS_SET_rd_tcont_id(reg32, val)       REG_FLD_SET(G_TCONT_ID_16_31_STS_FLD_rd_tcont_id, (reg32), (val))

#define DBG_CAP_SETTING_SET_mib_frame_type(reg32, val)         REG_FLD_SET(DBG_CAP_SETTING_FLD_mib_frame_type, (reg32), (val))
#define DBG_CAP_SETTING_SET_gpon_mib_en(reg32, val)            REG_FLD_SET(DBG_CAP_SETTING_FLD_gpon_mib_en, (reg32), (val))
#define DBG_CAP_SETTING_SET_us_no_msg_int_en(reg32, val)       REG_FLD_SET(DBG_CAP_SETTING_FLD_us_no_msg_int_en, (reg32), (val))
#define DBG_CAP_SETTING_SET_rpt_msg_flt(reg32, val)            REG_FLD_SET(DBG_CAP_SETTING_FLD_rpt_msg_flt, (reg32), (val))
#define DBG_CAP_SETTING_SET_max_rdm_dly(reg32, val)            REG_FLD_SET(DBG_CAP_SETTING_FLD_max_rdm_dly, (reg32), (val))

#define DBG_US_PREF_OFS_SET_sch_pref_ofs(reg32, val)           REG_FLD_SET(DBG_US_PREF_OFS_FLD_sch_pref_ofs, (reg32), (val))
#define DBG_US_PREF_OFS_SET_dt_pref_ofs(reg32, val)            REG_FLD_SET(DBG_US_PREF_OFS_FLD_dt_pref_ofs, (reg32), (val))

#define DBG_DLY_SET_phy_rx_dly_sel(reg32, val)                 REG_FLD_SET(DBG_DLY_FLD_phy_rx_dly_sel, (reg32), (val))
#define DBG_DLY_SET_fix_phy_rx_dly(reg32, val)                 REG_FLD_SET(DBG_DLY_FLD_fix_phy_rx_dly, (reg32), (val))
#define DBG_DLY_SET_fine_int_dly(reg32, val)                   REG_FLD_SET(DBG_DLY_FLD_fine_int_dly, (reg32), (val))
#define DBG_DLY_SET_phy_tx_dly(reg32, val)                     REG_FLD_SET(DBG_DLY_FLD_phy_tx_dly, (reg32), (val))

#define DBG_IDLE_GEM_THLD_SET_idle_gem_thld(reg32, val)        REG_FLD_SET(DBG_IDLE_GEM_THLD_FLD_idle_gem_thld, (reg32), (val))

#define DBG_US_NO_MSG0_SET_us_no_msg_b3_b4(reg32, val)         REG_FLD_SET(DBG_US_NO_MSG0_FLD_us_no_msg_b3_b4, (reg32), (val))
#define DBG_US_NO_MSG0_SET_us_no_msg_type(reg32, val)          REG_FLD_SET(DBG_US_NO_MSG0_FLD_us_no_msg_type, (reg32), (val))

#define DBG_US_NO_MSG1_SET_us_no_msg_b5_b6(reg32, val)         REG_FLD_SET(DBG_US_NO_MSG1_FLD_us_no_msg_b5_b6, (reg32), (val))
#define DBG_US_NO_MSG1_SET_us_no_msg_b7_b8(reg32, val)         REG_FLD_SET(DBG_US_NO_MSG1_FLD_us_no_msg_b7_b8, (reg32), (val))

#define DBG_US_NO_MSG2_SET_us_no_msg_b9_b10(reg32, val)        REG_FLD_SET(DBG_US_NO_MSG2_FLD_us_no_msg_b9_b10, (reg32), (val))
#define DBG_US_NO_MSG2_SET_us_no_msg_b11_b12(reg32, val)       REG_FLD_SET(DBG_US_NO_MSG2_FLD_us_no_msg_b11_b12, (reg32), (val))

#define DBG_US_DYING_GASP_CTRL_SET_dying_gasp_num(reg32, val)  REG_FLD_SET(DBG_US_DYING_GASP_CTRL_FLD_dying_gasp_num, (reg32), (val))
#define DBG_US_DYING_GASP_CTRL_SET_dying_gasp_test(reg32, val) REG_FLD_SET(DBG_US_DYING_GASP_CTRL_FLD_dying_gasp_test, (reg32), (val))
#define DBG_US_DYING_GASP_CTRL_SET_hw_dying_gasp_en(reg32, val) REG_FLD_SET(DBG_US_DYING_GASP_CTRL_FLD_hw_dying_gasp_en, (reg32), (val))
#define DBG_US_DYING_GASP_CTRL_SET_dying_gasp_msg_type(reg32, val) REG_FLD_SET(DBG_US_DYING_GASP_CTRL_FLD_dying_gasp_msg_type, (reg32), (val))

#define DBG_BWM_FILTER_CTRL_SET_bwm_intvl_invld(reg32, val)    REG_FLD_SET(DBG_BWM_FILTER_CTRL_FLD_bwm_intvl_invld, (reg32), (val))
#define DBG_BWM_FILTER_CTRL_SET_bwm_len_invld(reg32, val)      REG_FLD_SET(DBG_BWM_FILTER_CTRL_FLD_bwm_len_invld, (reg32), (val))
#define DBG_BWM_FILTER_CTRL_SET_bwm_start_order_err(reg32, val) REG_FLD_SET(DBG_BWM_FILTER_CTRL_FLD_bwm_start_order_err, (reg32), (val))
#define DBG_BWM_FILTER_CTRL_SET_bwm_start_order_err_flt_en(reg32, val) REG_FLD_SET(DBG_BWM_FILTER_CTRL_FLD_bwm_start_order_err_flt_en, (reg32), (val))
#define DBG_BWM_FILTER_CTRL_SET_bwm_len_vld_chk_en(reg32, val) REG_FLD_SET(DBG_BWM_FILTER_CTRL_FLD_bwm_len_vld_chk_en, (reg32), (val))
#define DBG_BWM_FILTER_CTRL_SET_min_bst_intvl_en(reg32, val)   REG_FLD_SET(DBG_BWM_FILTER_CTRL_FLD_min_bst_intvl_en, (reg32), (val))
#define DBG_BWM_FILTER_CTRL_SET_min_bst_intvl(reg32, val)      REG_FLD_SET(DBG_BWM_FILTER_CTRL_FLD_min_bst_intvl, (reg32), (val))

#define DBG_BWM_SFIFO_STS_SET_bwm_sfifo_max_used(reg32, val)   REG_FLD_SET(DBG_BWM_SFIFO_STS_FLD_bwm_sfifo_max_used, (reg32), (val))
#define DBG_BWM_SFIFO_STS_SET_bwm_sfifo_used(reg32, val)       REG_FLD_SET(DBG_BWM_SFIFO_STS_FLD_bwm_sfifo_used, (reg32), (val))

#define DBG_GRP_0_SET_ether_fcs_err(reg32, val)                REG_FLD_SET(DBG_GRP_0_FLD_ether_fcs_err, (reg32), (val))
#define DBG_GRP_0_SET_ploamu_wr(reg32, val)                    REG_FLD_SET(DBG_GRP_0_FLD_ploamu_wr, (reg32), (val))
#define DBG_GRP_0_SET_dbru_wr(reg32, val)                      REG_FLD_SET(DBG_GRP_0_FLD_dbru_wr, (reg32), (val))
#define DBG_GRP_0_SET_is_gem_pl(reg32, val)                    REG_FLD_SET(DBG_GRP_0_FLD_is_gem_pl, (reg32), (val))
#define DBG_GRP_0_SET_is_fcs_only(reg32, val)                  REG_FLD_SET(DBG_GRP_0_FLD_is_fcs_only, (reg32), (val))
#define DBG_GRP_0_SET_is_inst_idle(reg32, val)                 REG_FLD_SET(DBG_GRP_0_FLD_is_inst_idle, (reg32), (val))
#define DBG_GRP_0_SET_mbi_pl_fifo_ovrn(reg32, val)             REG_FLD_SET(DBG_GRP_0_FLD_mbi_pl_fifo_ovrn, (reg32), (val))
#define DBG_GRP_0_SET_aes_egn3_busy(reg32, val)                REG_FLD_SET(DBG_GRP_0_FLD_aes_egn3_busy, (reg32), (val))
#define DBG_GRP_0_SET_aes_egn2_busy(reg32, val)                REG_FLD_SET(DBG_GRP_0_FLD_aes_egn2_busy, (reg32), (val))
#define DBG_GRP_0_SET_aes_egn1_busy(reg32, val)                REG_FLD_SET(DBG_GRP_0_FLD_aes_egn1_busy, (reg32), (val))
#define DBG_GRP_0_SET_aes_egn0_busy(reg32, val)                REG_FLD_SET(DBG_GRP_0_FLD_aes_egn0_busy, (reg32), (val))
#define DBG_GRP_0_SET_aes_rdm_ciph_fifo_ovrn(reg32, val)       REG_FLD_SET(DBG_GRP_0_FLD_aes_rdm_ciph_fifo_ovrn, (reg32), (val))

#define DBG_GRP_1_SET_alen_neq_zero(reg32, val)                REG_FLD_SET(DBG_GRP_1_FLD_alen_neq_zero, (reg32), (val))
#define DBG_GRP_1_SET_tx_align_fifo_udrn(reg32, val)           REG_FLD_SET(DBG_GRP_1_FLD_tx_align_fifo_udrn, (reg32), (val))
#define DBG_GRP_1_SET_rx_sof_sync(reg32, val)                  REG_FLD_SET(DBG_GRP_1_FLD_rx_sof_sync, (reg32), (val))
#define DBG_GRP_1_SET_sync_start(reg32, val)                   REG_FLD_SET(DBG_GRP_1_FLD_sync_start, (reg32), (val))
#define DBG_GRP_1_SET_tx_late_start(reg32, val)                REG_FLD_SET(DBG_GRP_1_FLD_tx_late_start, (reg32), (val))
#define DBG_GRP_1_SET_bst_next_full(reg32, val)                REG_FLD_SET(DBG_GRP_1_FLD_bst_next_full, (reg32), (val))
#define DBG_GRP_1_SET_sgl_next_full(reg32, val)                REG_FLD_SET(DBG_GRP_1_FLD_sgl_next_full, (reg32), (val))
#define DBG_GRP_1_SET_aes_cryp_cnt_fifo_ovrn(reg32, val)       REG_FLD_SET(DBG_GRP_1_FLD_aes_cryp_cnt_fifo_ovrn, (reg32), (val))
#define DBG_GRP_1_SET_aes_ciph_txt_fifo_ovrn(reg32, val)       REG_FLD_SET(DBG_GRP_1_FLD_aes_ciph_txt_fifo_ovrn, (reg32), (val))
#define DBG_GRP_1_SET_mbi_hdr_fifo_ovrn(reg32, val)            REG_FLD_SET(DBG_GRP_1_FLD_mbi_hdr_fifo_ovrn, (reg32), (val))
#define DBG_GRP_1_SET_ploamd_crc_err(reg32, val)               REG_FLD_SET(DBG_GRP_1_FLD_ploamd_crc_err, (reg32), (val))
#define DBG_GRP_1_SET_bwm_uc_err(reg32, val)                   REG_FLD_SET(DBG_GRP_1_FLD_bwm_uc_err, (reg32), (val))
#define DBG_GRP_1_SET_blen_err(reg32, val)                     REG_FLD_SET(DBG_GRP_1_FLD_blen_err, (reg32), (val))
#define DBG_GRP_1_SET_logd(reg32, val)                         REG_FLD_SET(DBG_GRP_1_FLD_logd, (reg32), (val))

#define DBG_TXFIFO_MAX_USED_SET_tx_align_max_used(reg32, val)  REG_FLD_SET(DBG_TXFIFO_MAX_USED_FLD_tx_align_max_used, (reg32), (val))

#define DBG_BWM_GNT_STS_SET_pls_used(reg32, val)               REG_FLD_SET(DBG_BWM_GNT_STS_FLD_pls_used, (reg32), (val))
#define DBG_BWM_GNT_STS_SET_dba_mode2_used(reg32, val)         REG_FLD_SET(DBG_BWM_GNT_STS_FLD_dba_mode2_used, (reg32), (val))
#define DBG_BWM_GNT_STS_SET_dba_mode1_used(reg32, val)         REG_FLD_SET(DBG_BWM_GNT_STS_FLD_dba_mode1_used, (reg32), (val))
#define DBG_BWM_GNT_STS_SET_dba_mode0_used(reg32, val)         REG_FLD_SET(DBG_BWM_GNT_STS_FLD_dba_mode0_used, (reg32), (val))
#define DBG_BWM_GNT_STS_SET_max_assigned_gnt_num(reg32, val)   REG_FLD_SET(DBG_BWM_GNT_STS_FLD_max_assigned_gnt_num, (reg32), (val))

#define DBG_PROBE_CTRL_SET_gpon_sub_probe_sel(reg32, val)      REG_FLD_SET(DBG_PROBE_CTRL_FLD_gpon_sub_probe_sel, (reg32), (val))
#define DBG_PROBE_CTRL_SET_probe_bit0_sel(reg32, val)          REG_FLD_SET(DBG_PROBE_CTRL_FLD_probe_bit0_sel, (reg32), (val))
#define DBG_PROBE_CTRL_SET_bwm_probe_sel(reg32, val)           REG_FLD_SET(DBG_PROBE_CTRL_FLD_bwm_probe_sel, (reg32), (val))
#define DBG_PROBE_CTRL_SET_tx_align_probe_dt_sel(reg32, val)   REG_FLD_SET(DBG_PROBE_CTRL_FLD_tx_align_probe_dt_sel, (reg32), (val))
#define DBG_PROBE_CTRL_SET_rx_mpi_mbi_probe_dt_sel(reg32, val) REG_FLD_SET(DBG_PROBE_CTRL_FLD_rx_mpi_mbi_probe_dt_sel, (reg32), (val))
#define DBG_PROBE_CTRL_SET_tx_mpi_mbi_probe_dt_sel(reg32, val) REG_FLD_SET(DBG_PROBE_CTRL_FLD_tx_mpi_mbi_probe_dt_sel, (reg32), (val))
#define DBG_PROBE_CTRL_SET_rx_data_hl_sel(reg32, val)          REG_FLD_SET(DBG_PROBE_CTRL_FLD_rx_data_hl_sel, (reg32), (val))
#define DBG_PROBE_CTRL_SET_probe_sel(reg32, val)               REG_FLD_SET(DBG_PROBE_CTRL_FLD_probe_sel, (reg32), (val))

#define DBG_PROBE_HIGH32_SET_probe_high32(reg32, val)          REG_FLD_SET(DBG_PROBE_HIGH32_FLD_probe_high32, (reg32), (val))

#define DBG_PROBE_LOW32_SET_probe_low32(reg32, val)            REG_FLD_SET(DBG_PROBE_LOW32_FLD_probe_low32, (reg32), (val))

#define DBG_BWM_BFIFO_STS_SET_bwm_bfifo_max_used(reg32, val)   REG_FLD_SET(DBG_BWM_BFIFO_STS_FLD_bwm_bfifo_max_used, (reg32), (val))
#define DBG_BWM_BFIFO_STS_SET_bwm_bfifo_used(reg32, val)       REG_FLD_SET(DBG_BWM_BFIFO_STS_FLD_bwm_bfifo_used, (reg32), (val))

#define DBG_ERR_CTRL_SET_bwm_fec_ctrl(reg32, val)              REG_FLD_SET(DBG_ERR_CTRL_FLD_bwm_fec_ctrl, (reg32), (val))
#define DBG_ERR_CTRL_SET_ignore_atm_cell(reg32, val)           REG_FLD_SET(DBG_ERR_CTRL_FLD_ignore_atm_cell, (reg32), (val))
#define DBG_ERR_CTRL_SET_o5_eqd_adj_resync_en(reg32, val)      REG_FLD_SET(DBG_ERR_CTRL_FLD_o5_eqd_adj_resync_en, (reg32), (val))
#define DBG_ERR_CTRL_SET_tx_udrn_resync_en(reg32, val)         REG_FLD_SET(DBG_ERR_CTRL_FLD_tx_udrn_resync_en, (reg32), (val))
#define DBG_ERR_CTRL_SET_tx_late_resync_en(reg32, val)         REG_FLD_SET(DBG_ERR_CTRL_FLD_tx_late_resync_en, (reg32), (val))
#define DBG_ERR_CTRL_SET_bst_sgl_diff_resync_en(reg32, val)    REG_FLD_SET(DBG_ERR_CTRL_FLD_bst_sgl_diff_resync_en, (reg32), (val))
#define DBG_ERR_CTRL_SET_ignore_ploamd_crc(reg32, val)         REG_FLD_SET(DBG_ERR_CTRL_FLD_ignore_ploamd_crc, (reg32), (val))

#define DBG_SW_RDI_CTRL_SET_rdi_en(reg32, val)                 REG_FLD_SET(DBG_SW_RDI_CTRL_FLD_rdi_en, (reg32), (val))
#define DBG_SW_RDI_CTRL_SET_sw_rdi(reg32, val)                 REG_FLD_SET(DBG_SW_RDI_CTRL_FLD_sw_rdi, (reg32), (val))

#define DBG_RX_GEM_CNT_SET_dbg_rx_gem_cnt(reg32, val)          REG_FLD_SET(DBG_RX_GEM_CNT_FLD_dbg_rx_gem_cnt, (reg32), (val))

#define DBG_RX_CRC_ERR_CNT_SET_dbg_rx_crc_cnt(reg32, val)      REG_FLD_SET(DBG_RX_CRC_ERR_CNT_FLD_dbg_rx_crc_cnt, (reg32), (val))

#define DBG_RX_GTC_CNT_SET_dbg_rx_gtc_cnt(reg32, val)          REG_FLD_SET(DBG_RX_GTC_CNT_FLD_dbg_rx_gtc_cnt, (reg32), (val))

#define DBG_TX_GEM_CNT_SET_dbg_tx_gem_cnt(reg32, val)          REG_FLD_SET(DBG_TX_GEM_CNT_FLD_dbg_tx_gem_cnt, (reg32), (val))

#define DBG_TX_BST_CNT_SET_dbg_tx_burst_cnt(reg32, val)        REG_FLD_SET(DBG_TX_BST_CNT_FLD_dbg_tx_burst_cnt, (reg32), (val))

#define DBG_GEM_HEC_ONE_ERR_CNT_SET_dbg_gem_one_err_cnt(reg32, val) REG_FLD_SET(DBG_GEM_HEC_ONE_ERR_CNT_FLD_dbg_gem_one_err_cnt, (reg32), (val))

#define DBG_GEM_HEC_TWO_ERR_CNT_SET_dbg_gem_two_err_cnt(reg32, val) REG_FLD_SET(DBG_GEM_HEC_TWO_ERR_CNT_FLD_dbg_gem_two_err_cnt, (reg32), (val))

#define DBG_GEM_HEC_UC_ERR_CNT_SET_dbg_gem_uc_err_cnt(reg32, val) REG_FLD_SET(DBG_GEM_HEC_UC_ERR_CNT_FLD_dbg_gem_uc_err_cnt, (reg32), (val))

#define DBG_DBA_BACK_DOOR_SET_dbg_sw_rpt_dba_en(reg32, val)    REG_FLD_SET(DBG_DBA_BACK_DOOR_FLD_dbg_sw_rpt_dba_en, (reg32), (val))
#define DBG_DBA_BACK_DOOR_SET_dbg_slight_modify_en(reg32, val) REG_FLD_SET(DBG_DBA_BACK_DOOR_FLD_dbg_slight_modify_en, (reg32), (val))

#define DBG_DBA_TOTAL_BUF_SIZE_SET_dbg_dba_total_buf_size(reg32, val) REG_FLD_SET(DBG_DBA_TOTAL_BUF_SIZE_FLD_dbg_dba_total_buf_size, (reg32), (val))

#define DBG_DBA_GREEN_SIZE_SET_dbg_dba_green_size(reg32, val)  REG_FLD_SET(DBG_DBA_GREEN_SIZE_FLD_dbg_dba_green_size, (reg32), (val))

#define DBG_DBA_YELLOW_SIZE_SET_dbg_dba_yellow_size(reg32, val) REG_FLD_SET(DBG_DBA_YELLOW_SIZE_FLD_dbg_dba_yellow_size, (reg32), (val))

#define DBG_SLIGHT_MODIFY_SIZE_1_SET_dbg_yellow_slight_modify_size(reg32, val) REG_FLD_SET(DBG_SLIGHT_MODIFY_SIZE_1_FLD_dbg_yellow_slight_modify_size, (reg32), (val))
#define DBG_SLIGHT_MODIFY_SIZE_1_SET_dbg_green_slight_modify_size(reg32, val) REG_FLD_SET(DBG_SLIGHT_MODIFY_SIZE_1_FLD_dbg_green_slight_modify_size, (reg32), (val))

#define DBG_SLIGHT_MODIFY_SIZE_2_SET_dbg_total_slight_modify_size(reg32, val) REG_FLD_SET(DBG_SLIGHT_MODIFY_SIZE_2_FLD_dbg_total_slight_modify_size, (reg32), (val))

#define DBG_DS_SPF_CNT_SET_dbg_ds_spf_cnt(reg32, val)          REG_FLD_SET(DBG_DS_SPF_CNT_FLD_dbg_ds_spf_cnt, (reg32), (val))

#define DBG_TX_SYNC_OFFSET_SET_dbg_tx_sync_offset(reg32, val)  REG_FLD_SET(DBG_TX_SYNC_OFFSET_FLD_dbg_tx_sync_offset, (reg32), (val))

#define DBG_PLOAMD_FILTER_IN_O5_SET_dbg_ext_bst_len_filter_in_o5_en(reg32, val) REG_FLD_SET(DBG_PLOAMD_FILTER_IN_O5_FLD_dbg_ext_bst_len_filter_in_o5_en, (reg32), (val))
#define DBG_PLOAMD_FILTER_IN_O5_SET_dbg_us_overhead_filter_in_o5_en(reg32, val) REG_FLD_SET(DBG_PLOAMD_FILTER_IN_O5_FLD_dbg_us_overhead_filter_in_o5_en, (reg32), (val))

#define DBG_SW_RESYNC_SET_dbg_sw_resync_en(reg32, val)         REG_FLD_SET(DBG_SW_RESYNC_FLD_dbg_sw_resync_en, (reg32), (val))
#define DBG_SW_RESYNC_SET_dbg_sw_resync(reg32, val)            REG_FLD_SET(DBG_SW_RESYNC_FLD_dbg_sw_resync, (reg32), (val))

#define DBG_GTC_ETH_EXTR_SET_sniff_dbg_tx_rst(reg32, val)      REG_FLD_SET(DBG_GTC_ETH_EXTR_FLD_sniff_dbg_tx_rst, (reg32), (val))
#define DBG_GTC_ETH_EXTR_SET_sniff_pkt_padding_en(reg32, val)  REG_FLD_SET(DBG_GTC_ETH_EXTR_FLD_sniff_pkt_padding_en, (reg32), (val))
#define DBG_GTC_ETH_EXTR_SET_dbg_gtc_eth_extr_en(reg32, val)   REG_FLD_SET(DBG_GTC_ETH_EXTR_FLD_dbg_gtc_eth_extr_en, (reg32), (val))

#define DBG_DS_GTC_EXTR_ETH_HDR_SET_dbg_extr_rx_ds_eth_type(reg32, val) REG_FLD_SET(DBG_DS_GTC_EXTR_ETH_HDR_FLD_dbg_extr_rx_ds_eth_type, (reg32), (val))
#define DBG_DS_GTC_EXTR_ETH_HDR_SET_dbg_extr_rx_ds_sp(reg32, val) REG_FLD_SET(DBG_DS_GTC_EXTR_ETH_HDR_FLD_dbg_extr_rx_ds_sp, (reg32), (val))

#define DBG_US_GTC_EXTR_ETH_HDR_SET_dbg_extr_tx_us_eth_type(reg32, val) REG_FLD_SET(DBG_US_GTC_EXTR_ETH_HDR_FLD_dbg_extr_tx_us_eth_type, (reg32), (val))
#define DBG_US_GTC_EXTR_ETH_HDR_SET_dbg_extr_tx_us_sp(reg32, val) REG_FLD_SET(DBG_US_GTC_EXTR_ETH_HDR_FLD_dbg_extr_tx_us_sp, (reg32), (val))

#define DBG_DS_GTC_EXTR_ETH_CNT_SET_dbg_extr_rx_cnt(reg32, val) REG_FLD_SET(DBG_DS_GTC_EXTR_ETH_CNT_FLD_dbg_extr_rx_cnt, (reg32), (val))

#define DBG_US_GTC_EXTR_ETH_CNT_SET_dbg_extr_tx_cnt(reg32, val) REG_FLD_SET(DBG_US_GTC_EXTR_ETH_CNT_FLD_dbg_extr_tx_cnt, (reg32), (val))

#define SNIFF_GTC_GTC_INVLD_GEM_BYTE_SET_dbg_ds_all_gem_filter(reg32, val) REG_FLD_SET(SNIFF_GTC_GTC_INVLD_GEM_BYTE_FLD_dbg_ds_all_gem_filter, (reg32), (val))
#define SNIFF_GTC_GTC_INVLD_GEM_BYTE_SET_dbg_ds_gem_filter_exclude_omci(reg32, val) REG_FLD_SET(SNIFF_GTC_GTC_INVLD_GEM_BYTE_FLD_dbg_ds_gem_filter_exclude_omci, (reg32), (val))
#define SNIFF_GTC_GTC_INVLD_GEM_BYTE_SET_dbg_us_gtc_invld_gem_byte(reg32, val) REG_FLD_SET(SNIFF_GTC_GTC_INVLD_GEM_BYTE_FLD_dbg_us_gtc_invld_gem_byte, (reg32), (val))

#define DBG_SNIFF_DBG_REG_SET_dbg_mbi_pl_fifo_write_conflict(reg32, val) REG_FLD_SET(DBG_SNIFF_DBG_REG_FLD_dbg_mbi_pl_fifo_write_conflict, (reg32), (val))
#define DBG_SNIFF_DBG_REG_SET_dbg_mbi_hdr_fifo_write_conflict(reg32, val) REG_FLD_SET(DBG_SNIFF_DBG_REG_FLD_dbg_mbi_hdr_fifo_write_conflict, (reg32), (val))
#define DBG_SNIFF_DBG_REG_SET_dbg_us_gtc_info_fifo_ovrn(reg32, val) REG_FLD_SET(DBG_SNIFF_DBG_REG_FLD_dbg_us_gtc_info_fifo_ovrn, (reg32), (val))

#define DBG_SNIFF_US_INFO_FIFO_SET_dbg_us_info_tx_send_depth(reg32, val) REG_FLD_SET(DBG_SNIFF_US_INFO_FIFO_FLD_dbg_us_info_tx_send_depth, (reg32), (val))

#define SNIFF_US_HDR_WIDO_SET_stop_time_of_wido_en(reg32, val) REG_FLD_SET(SNIFF_US_HDR_WIDO_FLD_stop_time_of_wido_en, (reg32), (val))
#define SNIFF_US_HDR_WIDO_SET_stop_time_of_wido(reg32, val)    REG_FLD_SET(SNIFF_US_HDR_WIDO_FLD_stop_time_of_wido, (reg32), (val))

#define SNIFF_TX_DA_SA_SET_tx_us_da_h16(reg32, val)            REG_FLD_SET(SNIFF_TX_DA_SA_FLD_tx_us_da_h16, (reg32), (val))
#define SNIFF_TX_DA_SA_SET_tx_us_sa_h16(reg32, val)            REG_FLD_SET(SNIFF_TX_DA_SA_FLD_tx_us_sa_h16, (reg32), (val))

#define SNIFF_RX_DA_SA_SET_rx_ds_da_h16(reg32, val)            REG_FLD_SET(SNIFF_RX_DA_SA_FLD_rx_ds_da_h16, (reg32), (val))
#define SNIFF_RX_DA_SA_SET_rx_ds_sa_h16(reg32, val)            REG_FLD_SET(SNIFF_RX_DA_SA_FLD_rx_ds_sa_h16, (reg32), (val))

#define SNIFF_TX_RX_PID_SET_tx_eth_pid(reg32, val)             REG_FLD_SET(SNIFF_TX_RX_PID_FLD_tx_eth_pid, (reg32), (val))
#define SNIFF_TX_RX_PID_SET_rx_eth_pid(reg32, val)             REG_FLD_SET(SNIFF_TX_RX_PID_FLD_rx_eth_pid, (reg32), (val))

#define SNIFF_CNT_EN_SET_sniff_cnt_en(reg32, val)              REG_FLD_SET(SNIFF_CNT_EN_FLD_sniff_cnt_en, (reg32), (val))

#define SNIFF_RX_TX_TPID_SET_sniff_rx_tpid(reg32, val)         REG_FLD_SET(SNIFF_RX_TX_TPID_FLD_sniff_rx_tpid, (reg32), (val))
#define SNIFF_RX_TX_TPID_SET_sniff_tx_tpid(reg32, val)         REG_FLD_SET(SNIFF_RX_TX_TPID_FLD_sniff_tx_tpid, (reg32), (val))

#define DBG_GPON_MAC_SET_SET_asb_tx_eth_4byte_align_en(reg32, val) REG_FLD_SET(DBG_GPON_MAC_SET_FLD_asb_tx_eth_4byte_align_en, (reg32), (val))
#define DBG_GPON_MAC_SET_SET_olt_ds_fec(reg32, val)            REG_FLD_SET(DBG_GPON_MAC_SET_FLD_olt_ds_fec, (reg32), (val))
#define DBG_GPON_MAC_SET_SET_onu_us_fec(reg32, val)            REG_FLD_SET(DBG_GPON_MAC_SET_FLD_onu_us_fec, (reg32), (val))
#define DBG_GPON_MAC_SET_SET_gpon_mac_sw_rst_n(reg32, val)     REG_FLD_SET(DBG_GPON_MAC_SET_FLD_gpon_mac_sw_rst_n, (reg32), (val))

#define SLEEP_GLB_CFG_SET_sleep_filt_all_bwm(reg32, val)       REG_FLD_SET(SLEEP_GLB_CFG_FLD_sleep_filt_all_bwm, (reg32), (val))
#define SLEEP_GLB_CFG_SET_reload_sleep_cnt(reg32, val)         REG_FLD_SET(SLEEP_GLB_CFG_FLD_reload_sleep_cnt, (reg32), (val))

#define SLEEP_CNT_SET_sleep_cnt(reg32, val)                    REG_FLD_SET(SLEEP_CNT_FLD_sleep_cnt, (reg32), (val))

#define TX_SYNC_OPT_SET_tx_sync_opt(reg32, val)                REG_FLD_SET(TX_SYNC_OPT_FLD_tx_sync_opt, (reg32), (val))

#define PLOAMU_CTRL_SET_ploamu_ind_ctrl(reg32, val)            REG_FLD_SET(PLOAMU_CTRL_FLD_ploamu_ind_ctrl, (reg32), (val))
#define PLOAMU_CTRL_SET_o3_o4_ploamu_ctrl(reg32, val)          REG_FLD_SET(PLOAMU_CTRL_FLD_o3_o4_ploamu_ctrl, (reg32), (val))

#define TOD_1PPS_WD_CTRL_SET_tod_1pps_width_ctrl(reg32, val)   REG_FLD_SET(TOD_1PPS_WD_CTRL_FLD_tod_1pps_width_ctrl, (reg32), (val))

#define G_ONU_ID_VAL_onu_id_vld(val)                           REG_FLD_VAL(G_ONU_ID_FLD_onu_id_vld, (val))
#define G_ONU_ID_VAL_onu_id(val)                               REG_FLD_VAL(G_ONU_ID_FLD_onu_id, (val))

#define G_GBL_CFG_VAL_us_fec_en(val)                           REG_FLD_VAL(G_GBL_CFG_FLD_us_fec_en, (val))
#define G_GBL_CFG_VAL_sr_blk_size(val)                         REG_FLD_VAL(G_GBL_CFG_FLD_sr_blk_size, (val))

#define G_INT_STATUS_VAL_bwm_us_fec_err_int(val)               REG_FLD_VAL(G_INT_STATUS_FLD_bwm_us_fec_err_int, (val))
#define G_INT_STATUS_VAL_bwm_stop_time_err_int(val)            REG_FLD_VAL(G_INT_STATUS_FLD_bwm_stop_time_err_int, (val))
#define G_INT_STATUS_VAL_lwi_int(val)                          REG_FLD_VAL(G_INT_STATUS_FLD_lwi_int, (val))
#define G_INT_STATUS_VAL_fwi_int(val)                          REG_FLD_VAL(G_INT_STATUS_FLD_fwi_int, (val))
#define G_INT_STATUS_VAL_pop_up_recv_in_O6_int(val)            REG_FLD_VAL(G_INT_STATUS_FLD_pop_up_recv_in_O6_int, (val))
#define G_INT_STATUS_VAL_onu_us_fec_chg_int(val)               REG_FLD_VAL(G_INT_STATUS_FLD_onu_us_fec_chg_int, (val))
#define G_INT_STATUS_VAL_olt_ds_fec_chg_int(val)               REG_FLD_VAL(G_INT_STATUS_FLD_olt_ds_fec_chg_int, (val))
#define G_INT_STATUS_VAL_o5_eqd_adj_done_int(val)              REG_FLD_VAL(G_INT_STATUS_FLD_o5_eqd_adj_done_int, (val))
#define G_INT_STATUS_VAL_sfifo_full_int(val)                   REG_FLD_VAL(G_INT_STATUS_FLD_sfifo_full_int, (val))
#define G_INT_STATUS_VAL_bfifo_full_int(val)                   REG_FLD_VAL(G_INT_STATUS_FLD_bfifo_full_int, (val))
#define G_INT_STATUS_VAL_rx_gem_intlv_err_int(val)             REG_FLD_VAL(G_INT_STATUS_FLD_rx_gem_intlv_err_int, (val))
#define G_INT_STATUS_VAL_rx_eof_err_int(val)                   REG_FLD_VAL(G_INT_STATUS_FLD_rx_eof_err_int, (val))
#define G_INT_STATUS_VAL_tx_late_start_int(val)                REG_FLD_VAL(G_INT_STATUS_FLD_tx_late_start_int, (val))
#define G_INT_STATUS_VAL_bst_sgl_diff_int(val)                 REG_FLD_VAL(G_INT_STATUS_FLD_bst_sgl_diff_int, (val))
#define G_INT_STATUS_VAL_fifo_err_int(val)                     REG_FLD_VAL(G_INT_STATUS_FLD_fifo_err_int, (val))
#define G_INT_STATUS_VAL_rx_err_int(val)                       REG_FLD_VAL(G_INT_STATUS_FLD_rx_err_int, (val))
#define G_INT_STATUS_VAL_dying_gasp_send_int(val)              REG_FLD_VAL(G_INT_STATUS_FLD_dying_gasp_send_int, (val))
#define G_INT_STATUS_VAL_tod_1pps_int(val)                     REG_FLD_VAL(G_INT_STATUS_FLD_tod_1pps_int, (val))
#define G_INT_STATUS_VAL_tod_update_done_int(val)              REG_FLD_VAL(G_INT_STATUS_FLD_tod_update_done_int, (val))
#define G_INT_STATUS_VAL_aes_key_switch_done_int(val)          REG_FLD_VAL(G_INT_STATUS_FLD_aes_key_switch_done_int, (val))
#define G_INT_STATUS_VAL_los_gem_del_int(val)                  REG_FLD_VAL(G_INT_STATUS_FLD_los_gem_del_int, (val))
#define G_INT_STATUS_VAL_sn_req_crs_int(val)                   REG_FLD_VAL(G_INT_STATUS_FLD_sn_req_crs_int, (val))
#define G_INT_STATUS_VAL_sn_onu_send_o4_int(val)               REG_FLD_VAL(G_INT_STATUS_FLD_sn_onu_send_o4_int, (val))
#define G_INT_STATUS_VAL_ranging_req_recv_int(val)             REG_FLD_VAL(G_INT_STATUS_FLD_ranging_req_recv_int, (val))
#define G_INT_STATUS_VAL_sn_onu_send_o3_int(val)               REG_FLD_VAL(G_INT_STATUS_FLD_sn_onu_send_o3_int, (val))
#define G_INT_STATUS_VAL_sn_req_recv_int(val)                  REG_FLD_VAL(G_INT_STATUS_FLD_sn_req_recv_int, (val))
#define G_INT_STATUS_VAL_ploamu_send_int(val)                  REG_FLD_VAL(G_INT_STATUS_FLD_ploamu_send_int, (val))
#define G_INT_STATUS_VAL_ploamd_recv_int(val)                  REG_FLD_VAL(G_INT_STATUS_FLD_ploamd_recv_int, (val))

#define G_INT_ENABLE_VAL_bwm_us_fec_err_int_en(val)            REG_FLD_VAL(G_INT_ENABLE_FLD_bwm_us_fec_err_int_en, (val))
#define G_INT_ENABLE_VAL_bwm_stop_time_err_int_en(val)         REG_FLD_VAL(G_INT_ENABLE_FLD_bwm_stop_time_err_int_en, (val))
#define G_INT_ENABLE_VAL_lwi_int_en(val)                       REG_FLD_VAL(G_INT_ENABLE_FLD_lwi_int_en, (val))
#define G_INT_ENABLE_VAL_fwi_int_en(val)                       REG_FLD_VAL(G_INT_ENABLE_FLD_fwi_int_en, (val))
#define G_INT_ENABLE_VAL_pop_up_recv_in_O6_int_en(val)         REG_FLD_VAL(G_INT_ENABLE_FLD_pop_up_recv_in_O6_int_en, (val))
#define G_INT_ENABLE_VAL_onu_us_fec_chg_int_en(val)            REG_FLD_VAL(G_INT_ENABLE_FLD_onu_us_fec_chg_int_en, (val))
#define G_INT_ENABLE_VAL_olt_ds_fec_chg_int_en(val)            REG_FLD_VAL(G_INT_ENABLE_FLD_olt_ds_fec_chg_int_en, (val))
#define G_INT_ENABLE_VAL_o5_eqd_adj_done_int_en(val)           REG_FLD_VAL(G_INT_ENABLE_FLD_o5_eqd_adj_done_int_en, (val))
#define G_INT_ENABLE_VAL_sfifo_full_int_en(val)                REG_FLD_VAL(G_INT_ENABLE_FLD_sfifo_full_int_en, (val))
#define G_INT_ENABLE_VAL_bfifo_full_int_en(val)                REG_FLD_VAL(G_INT_ENABLE_FLD_bfifo_full_int_en, (val))
#define G_INT_ENABLE_VAL_rx_gem_intlv_err_int_en(val)          REG_FLD_VAL(G_INT_ENABLE_FLD_rx_gem_intlv_err_int_en, (val))
#define G_INT_ENABLE_VAL_rx_eof_err_int_en(val)                REG_FLD_VAL(G_INT_ENABLE_FLD_rx_eof_err_int_en, (val))
#define G_INT_ENABLE_VAL_tx_late_start_int_en(val)             REG_FLD_VAL(G_INT_ENABLE_FLD_tx_late_start_int_en, (val))
#define G_INT_ENABLE_VAL_bst_sgl_diff_int_en(val)              REG_FLD_VAL(G_INT_ENABLE_FLD_bst_sgl_diff_int_en, (val))
#define G_INT_ENABLE_VAL_fifo_err_int_en(val)                  REG_FLD_VAL(G_INT_ENABLE_FLD_fifo_err_int_en, (val))
#define G_INT_ENABLE_VAL_rx_err_int_en(val)                    REG_FLD_VAL(G_INT_ENABLE_FLD_rx_err_int_en, (val))
#define G_INT_ENABLE_VAL_dying_gasp_send_int_en(val)           REG_FLD_VAL(G_INT_ENABLE_FLD_dying_gasp_send_int_en, (val))
#define G_INT_ENABLE_VAL_tod_1pps_int_en(val)                  REG_FLD_VAL(G_INT_ENABLE_FLD_tod_1pps_int_en, (val))
#define G_INT_ENABLE_VAL_tod_update_done_int_en(val)           REG_FLD_VAL(G_INT_ENABLE_FLD_tod_update_done_int_en, (val))
#define G_INT_ENABLE_VAL_aes_key_switch_done_int_en(val)       REG_FLD_VAL(G_INT_ENABLE_FLD_aes_key_switch_done_int_en, (val))
#define G_INT_ENABLE_VAL_los_gem_del_int_en(val)               REG_FLD_VAL(G_INT_ENABLE_FLD_los_gem_del_int_en, (val))
#define G_INT_ENABLE_VAL_sn_req_crs_int_en(val)                REG_FLD_VAL(G_INT_ENABLE_FLD_sn_req_crs_int_en, (val))
#define G_INT_ENABLE_VAL_sn_onu_send_o4_int_en(val)            REG_FLD_VAL(G_INT_ENABLE_FLD_sn_onu_send_o4_int_en, (val))
#define G_INT_ENABLE_VAL_ranging_req_recv_int_en(val)          REG_FLD_VAL(G_INT_ENABLE_FLD_ranging_req_recv_int_en, (val))
#define G_INT_ENABLE_VAL_sn_onu_send_o3_int_en(val)            REG_FLD_VAL(G_INT_ENABLE_FLD_sn_onu_send_o3_int_en, (val))
#define G_INT_ENABLE_VAL_sn_req_recv_int_en(val)               REG_FLD_VAL(G_INT_ENABLE_FLD_sn_req_recv_int_en, (val))
#define G_INT_ENABLE_VAL_ploamu_send_int_en(val)               REG_FLD_VAL(G_INT_ENABLE_FLD_ploamu_send_int_en, (val))
#define G_INT_ENABLE_VAL_ploamd_recv_int_en(val)               REG_FLD_VAL(G_INT_ENABLE_FLD_ploamd_recv_int_en, (val))

#define G_TCONT_ID_0_1_VAL_t_cont1_vld(val)                    REG_FLD_VAL(G_TCONT_ID_0_1_FLD_t_cont1_vld, (val))
#define G_TCONT_ID_0_1_VAL_t_cont1_id(val)                     REG_FLD_VAL(G_TCONT_ID_0_1_FLD_t_cont1_id, (val))
#define G_TCONT_ID_0_1_VAL_t_cont0_vld(val)                    REG_FLD_VAL(G_TCONT_ID_0_1_FLD_t_cont0_vld, (val))
#define G_TCONT_ID_0_1_VAL_t_cont0_id(val)                     REG_FLD_VAL(G_TCONT_ID_0_1_FLD_t_cont0_id, (val))

#define G_TCONT_ID_2_3_VAL_t_cont3_vld(val)                    REG_FLD_VAL(G_TCONT_ID_2_3_FLD_t_cont3_vld, (val))
#define G_TCONT_ID_2_3_VAL_t_cont3_id(val)                     REG_FLD_VAL(G_TCONT_ID_2_3_FLD_t_cont3_id, (val))
#define G_TCONT_ID_2_3_VAL_t_cont2_vld(val)                    REG_FLD_VAL(G_TCONT_ID_2_3_FLD_t_cont2_vld, (val))
#define G_TCONT_ID_2_3_VAL_t_cont2_id(val)                     REG_FLD_VAL(G_TCONT_ID_2_3_FLD_t_cont2_id, (val))

#define G_TCONT_ID_4_5_VAL_t_cont5_vld(val)                    REG_FLD_VAL(G_TCONT_ID_4_5_FLD_t_cont5_vld, (val))
#define G_TCONT_ID_4_5_VAL_t_cont5_id(val)                     REG_FLD_VAL(G_TCONT_ID_4_5_FLD_t_cont5_id, (val))
#define G_TCONT_ID_4_5_VAL_t_cont4_vld(val)                    REG_FLD_VAL(G_TCONT_ID_4_5_FLD_t_cont4_vld, (val))
#define G_TCONT_ID_4_5_VAL_t_cont4_id(val)                     REG_FLD_VAL(G_TCONT_ID_4_5_FLD_t_cont4_id, (val))

#define G_TCONT_ID_6_7_VAL_t_cont7_vld(val)                    REG_FLD_VAL(G_TCONT_ID_6_7_FLD_t_cont7_vld, (val))
#define G_TCONT_ID_6_7_VAL_t_cont7_id(val)                     REG_FLD_VAL(G_TCONT_ID_6_7_FLD_t_cont7_id, (val))
#define G_TCONT_ID_6_7_VAL_t_cont6_vld(val)                    REG_FLD_VAL(G_TCONT_ID_6_7_FLD_t_cont6_vld, (val))
#define G_TCONT_ID_6_7_VAL_t_cont6_id(val)                     REG_FLD_VAL(G_TCONT_ID_6_7_FLD_t_cont6_id, (val))

#define G_TCONT_ID_8_9_VAL_t_cont9_vld(val)                    REG_FLD_VAL(G_TCONT_ID_8_9_FLD_t_cont9_vld, (val))
#define G_TCONT_ID_8_9_VAL_t_cont9_id(val)                     REG_FLD_VAL(G_TCONT_ID_8_9_FLD_t_cont9_id, (val))
#define G_TCONT_ID_8_9_VAL_t_cont8_vld(val)                    REG_FLD_VAL(G_TCONT_ID_8_9_FLD_t_cont8_vld, (val))
#define G_TCONT_ID_8_9_VAL_t_cont8_id(val)                     REG_FLD_VAL(G_TCONT_ID_8_9_FLD_t_cont8_id, (val))

#define G_TCONT_ID_10_11_VAL_t_cont11_vld(val)                 REG_FLD_VAL(G_TCONT_ID_10_11_FLD_t_cont11_vld, (val))
#define G_TCONT_ID_10_11_VAL_t_cont11_id(val)                  REG_FLD_VAL(G_TCONT_ID_10_11_FLD_t_cont11_id, (val))
#define G_TCONT_ID_10_11_VAL_t_cont10_vld(val)                 REG_FLD_VAL(G_TCONT_ID_10_11_FLD_t_cont10_vld, (val))
#define G_TCONT_ID_10_11_VAL_t_cont10_id(val)                  REG_FLD_VAL(G_TCONT_ID_10_11_FLD_t_cont10_id, (val))

#define G_TCONT_ID_12_13_VAL_t_cont13_vld(val)                 REG_FLD_VAL(G_TCONT_ID_12_13_FLD_t_cont13_vld, (val))
#define G_TCONT_ID_12_13_VAL_t_cont13_id(val)                  REG_FLD_VAL(G_TCONT_ID_12_13_FLD_t_cont13_id, (val))
#define G_TCONT_ID_12_13_VAL_t_cont12_vld(val)                 REG_FLD_VAL(G_TCONT_ID_12_13_FLD_t_cont12_vld, (val))
#define G_TCONT_ID_12_13_VAL_t_cont12_id(val)                  REG_FLD_VAL(G_TCONT_ID_12_13_FLD_t_cont12_id, (val))

#define G_TCONT_ID_14_15_VAL_t_cont15_vld(val)                 REG_FLD_VAL(G_TCONT_ID_14_15_FLD_t_cont15_vld, (val))
#define G_TCONT_ID_14_15_VAL_t_cont15_id(val)                  REG_FLD_VAL(G_TCONT_ID_14_15_FLD_t_cont15_id, (val))
#define G_TCONT_ID_14_15_VAL_t_cont14_vld(val)                 REG_FLD_VAL(G_TCONT_ID_14_15_FLD_t_cont14_vld, (val))
#define G_TCONT_ID_14_15_VAL_t_cont14_id(val)                  REG_FLD_VAL(G_TCONT_ID_14_15_FLD_t_cont14_id, (val))

#define G_GEM_PORT_CFG_VAL_gpid_cmd(val)                       REG_FLD_VAL(G_GEM_PORT_CFG_FLD_gpid_cmd, (val))
#define G_GEM_PORT_CFG_VAL_gpid_encrypt(val)                   REG_FLD_VAL(G_GEM_PORT_CFG_FLD_gpid_encrypt, (val))
#define G_GEM_PORT_CFG_VAL_gpid_vld(val)                       REG_FLD_VAL(G_GEM_PORT_CFG_FLD_gpid_vld, (val))
#define G_GEM_PORT_CFG_VAL_gem_port_id(val)                    REG_FLD_VAL(G_GEM_PORT_CFG_FLD_gem_port_id, (val))

#define G_GEM_PORT_STS_VAL_gpid_cmd_done(val)                  REG_FLD_VAL(G_GEM_PORT_STS_FLD_gpid_cmd_done, (val))
#define G_GEM_PORT_STS_VAL_gpid_encrypt_sts(val)               REG_FLD_VAL(G_GEM_PORT_STS_FLD_gpid_encrypt_sts, (val))
#define G_GEM_PORT_STS_VAL_gpid_vld_sts(val)                   REG_FLD_VAL(G_GEM_PORT_STS_FLD_gpid_vld_sts, (val))

#define G_OMCI_ID_VAL_omci_port_id_vld(val)                    REG_FLD_VAL(G_OMCI_ID_FLD_omci_port_id_vld, (val))
#define G_OMCI_ID_VAL_omci_gpid(val)                           REG_FLD_VAL(G_OMCI_ID_FLD_omci_gpid, (val))

#define G_GEM_TBL_INIT_VAL_gem_tbl_init_done(val)              REG_FLD_VAL(G_GEM_TBL_INIT_FLD_gem_tbl_init_done, (val))
#define G_GEM_TBL_INIT_VAL_gem_tbl_init_start(val)             REG_FLD_VAL(G_GEM_TBL_INIT_FLD_gem_tbl_init_start, (val))

#define G_PLOAMu_FIFO_STS_VAL_ploamu_fifo_udrn(val)            REG_FLD_VAL(G_PLOAMu_FIFO_STS_FLD_ploamu_fifo_udrn, (val))
#define G_PLOAMu_FIFO_STS_VAL_ploamu_fifo_min_avail(val)       REG_FLD_VAL(G_PLOAMu_FIFO_STS_FLD_ploamu_fifo_min_avail, (val))
#define G_PLOAMu_FIFO_STS_VAL_ploamu_fifo_avail(val)           REG_FLD_VAL(G_PLOAMu_FIFO_STS_FLD_ploamu_fifo_avail, (val))

#define G_PLOAMu_WDATA_VAL_ploamu_wdata(val)                   REG_FLD_VAL(G_PLOAMu_WDATA_FLD_ploamu_wdata, (val))

#define G_PLOAMd_FIFO_STS_VAL_ploamd_fifo_ovrn(val)            REG_FLD_VAL(G_PLOAMd_FIFO_STS_FLD_ploamd_fifo_ovrn, (val))
#define G_PLOAMd_FIFO_STS_VAL_ploamd_fifo_max_used(val)        REG_FLD_VAL(G_PLOAMd_FIFO_STS_FLD_ploamd_fifo_max_used, (val))
#define G_PLOAMd_FIFO_STS_VAL_ploamd_fifo_used(val)            REG_FLD_VAL(G_PLOAMd_FIFO_STS_FLD_ploamd_fifo_used, (val))

#define G_PLOAMd_RDATA_VAL_ploamd_rdata(val)                   REG_FLD_VAL(G_PLOAMd_RDATA_FLD_ploamd_rdata, (val))

#define G_AES_CFG_VAL_aes_spf_cnt(val)                         REG_FLD_VAL(G_AES_CFG_FLD_aes_spf_cnt, (val))

#define G_AES_ACTIVE_KEY0_VAL_aes_active_key0(val)             REG_FLD_VAL(G_AES_ACTIVE_KEY0_FLD_aes_active_key0, (val))

#define G_AES_ACTIVE_KEY1_VAL_aes_active_key1(val)             REG_FLD_VAL(G_AES_ACTIVE_KEY1_FLD_aes_active_key1, (val))

#define G_AES_ACTIVE_KEY2_VAL_aes_active_key2(val)             REG_FLD_VAL(G_AES_ACTIVE_KEY2_FLD_aes_active_key2, (val))

#define G_AES_ACTIVE_KEY3_VAL_aes_active_key3(val)             REG_FLD_VAL(G_AES_ACTIVE_KEY3_FLD_aes_active_key3, (val))

#define G_AES_SHADOW_KEY0_VAL_aes_shadow_key0(val)             REG_FLD_VAL(G_AES_SHADOW_KEY0_FLD_aes_shadow_key0, (val))

#define G_AES_SHADOW_KEY1_VAL_aes_shadow_key1(val)             REG_FLD_VAL(G_AES_SHADOW_KEY1_FLD_aes_shadow_key1, (val))

#define G_AES_SHADOW_KEY2_VAL_aes_shadow_key2(val)             REG_FLD_VAL(G_AES_SHADOW_KEY2_FLD_aes_shadow_key2, (val))

#define G_AES_SHADOW_KEY3_VAL_aes_shadow_key3(val)             REG_FLD_VAL(G_AES_SHADOW_KEY3_FLD_aes_shadow_key3, (val))

#define G_AES_KEY_SWITCH_BY_SW_VAL_aes_key_switch_by_sw_done(val) REG_FLD_VAL(G_AES_KEY_SWITCH_BY_SW_FLD_aes_key_switch_by_sw_done, (val))
#define G_AES_KEY_SWITCH_BY_SW_VAL_aes_key_switch_by_sw_cmd(val) REG_FLD_VAL(G_AES_KEY_SWITCH_BY_SW_FLD_aes_key_switch_by_sw_cmd, (val))

#define G_PLOu_OVERHEAD_VAL_plou_overhead(val)                 REG_FLD_VAL(G_PLOu_OVERHEAD_FLD_plou_overhead, (val))

#define G_PLOu_GUARD_BIT_VAL_guard_bit(val)                    REG_FLD_VAL(G_PLOu_GUARD_BIT_FLD_guard_bit, (val))

#define G_PLOu_PRMBL_TYPE1_2_VAL_prmb2_bit(val)                REG_FLD_VAL(G_PLOu_PRMBL_TYPE1_2_FLD_prmb2_bit, (val))
#define G_PLOu_PRMBL_TYPE1_2_VAL_prmb1_bit(val)                REG_FLD_VAL(G_PLOu_PRMBL_TYPE1_2_FLD_prmb1_bit, (val))

#define G_PLOu_PRMBL_TYPE3_VAL_ebl_en(val)                     REG_FLD_VAL(G_PLOu_PRMBL_TYPE3_FLD_ebl_en, (val))
#define G_PLOu_PRMBL_TYPE3_VAL_ext_prmb3_o5_num(val)           REG_FLD_VAL(G_PLOu_PRMBL_TYPE3_FLD_ext_prmb3_o5_num, (val))
#define G_PLOu_PRMBL_TYPE3_VAL_ext_prmb3_o3_o4_num(val)        REG_FLD_VAL(G_PLOu_PRMBL_TYPE3_FLD_ext_prmb3_o3_o4_num, (val))

#define G_PLOu_DELM_BIT_VAL_delm_bit(val)                      REG_FLD_VAL(G_PLOu_DELM_BIT_FLD_delm_bit, (val))

#define G_PRE_ASSIGNED_DLY_VAL_pre_dly_en(val)                 REG_FLD_VAL(G_PRE_ASSIGNED_DLY_FLD_pre_dly_en, (val))
#define G_PRE_ASSIGNED_DLY_VAL_pre_dly(val)                    REG_FLD_VAL(G_PRE_ASSIGNED_DLY_FLD_pre_dly, (val))

#define G_EQD_VAL_eqd(val)                                     REG_FLD_VAL(G_EQD_FLD_eqd, (val))

#define G_RSP_TIME_VAL_tresp(val)                              REG_FLD_VAL(G_RSP_TIME_FLD_tresp, (val))

#define G_VENDOR_ID_VAL_vendor_id(val)                         REG_FLD_VAL(G_VENDOR_ID_FLD_vendor_id, (val))

#define G_VS_SN_VAL_vs_sn(val)                                 REG_FLD_VAL(G_VS_SN_FLD_vs_sn, (val))

#define G_SN_MSG_CFG_VAL_sn_req_thr(val)                       REG_FLD_VAL(G_SN_MSG_CFG_FLD_sn_req_thr, (val))
#define G_SN_MSG_CFG_VAL_tx_power_mode(val)                    REG_FLD_VAL(G_SN_MSG_CFG_FLD_tx_power_mode, (val))
#define G_SN_MSG_CFG_VAL_rdm_dly(val)                          REG_FLD_VAL(G_SN_MSG_CFG_FLD_rdm_dly, (val))

#define G_ACTIVATION_ST_VAL_act_st(val)                        REG_FLD_VAL(G_ACTIVATION_ST_FLD_act_st, (val))

#define G_TOD_CFG_VAL_tod_spf_cnt(val)                         REG_FLD_VAL(G_TOD_CFG_FLD_tod_spf_cnt, (val))

#define G_NEW_TOD_SEC_L32_VAL_new_tod_sec_l32(val)             REG_FLD_VAL(G_NEW_TOD_SEC_L32_FLD_new_tod_sec_l32, (val))

#define G_NEW_TOD_NANO_SEC_VAL_new_tod_nano_sec(val)           REG_FLD_VAL(G_NEW_TOD_NANO_SEC_FLD_new_tod_nano_sec, (val))

#define G_CUR_TOD_SEC_L32_VAL_cur_tod_sec_l32(val)             REG_FLD_VAL(G_CUR_TOD_SEC_L32_FLD_cur_tod_sec_l32, (val))

#define G_CUR_TOD_NANO_SEC_VAL_cur_tod_nano_sec(val)           REG_FLD_VAL(G_CUR_TOD_NANO_SEC_FLD_cur_tod_nano_sec, (val))

#define G_TOD_CLK_PERIOD_VAL_tod_period(val)                   REG_FLD_VAL(G_TOD_CLK_PERIOD_FLD_tod_period, (val))

#define G_TX_FCS_TBL_INIT_VAL_tx_fcs_tbl_init_done(val)        REG_FLD_VAL(G_TX_FCS_TBL_INIT_FLD_tx_fcs_tbl_init_done, (val))
#define G_TX_FCS_TBL_INIT_VAL_tx_fcs_tbl_init_start(val)       REG_FLD_VAL(G_TX_FCS_TBL_INIT_FLD_tx_fcs_tbl_init_start, (val))

#define G_MIB_CTRL_STS_VAL_mib_cmd(val)                        REG_FLD_VAL(G_MIB_CTRL_STS_FLD_mib_cmd, (val))
#define G_MIB_CTRL_STS_VAL_mib_cmd_done(val)                   REG_FLD_VAL(G_MIB_CTRL_STS_FLD_mib_cmd_done, (val))
#define G_MIB_CTRL_STS_VAL_mib_read_clr_en(val)                REG_FLD_VAL(G_MIB_CTRL_STS_FLD_mib_read_clr_en, (val))
#define G_MIB_CTRL_STS_VAL_mib_type(val)                       REG_FLD_VAL(G_MIB_CTRL_STS_FLD_mib_type, (val))
#define G_MIB_CTRL_STS_VAL_mib_gpid(val)                       REG_FLD_VAL(G_MIB_CTRL_STS_FLD_mib_gpid, (val))

#define G_MIB_RDATA_L32_VAL_mib_rdata_l32(val)                 REG_FLD_VAL(G_MIB_RDATA_L32_FLD_mib_rdata_l32, (val))

#define G_MIB_RDATA_H32_VAL_mib_rdata_h32(val)                 REG_FLD_VAL(G_MIB_RDATA_H32_FLD_mib_rdata_h32, (val))

#define G_MIB_WDATA_L32_VAL_mib_wdata_l32(val)                 REG_FLD_VAL(G_MIB_WDATA_L32_FLD_mib_wdata_l32, (val))

#define G_MIB_WDATA_H32_VAL_mib_wdata_h32(val)                 REG_FLD_VAL(G_MIB_WDATA_H32_FLD_mib_wdata_h32, (val))

#define G_MIB_TBL_INIT_VAL_mib_tbl_init_done(val)              REG_FLD_VAL(G_MIB_TBL_INIT_FLD_mib_tbl_init_done, (val))
#define G_MIB_TBL_INIT_VAL_mib_tbl_init_start(val)             REG_FLD_VAL(G_MIB_TBL_INIT_FLD_mib_tbl_init_start, (val))

#define G_GPIDX_TBL_CTRL_VAL_gpidx_cmd(val)                    REG_FLD_VAL(G_GPIDX_TBL_CTRL_FLD_gpidx_cmd, (val))
#define G_GPIDX_TBL_CTRL_VAL_gpidx_tbl_addr(val)               REG_FLD_VAL(G_GPIDX_TBL_CTRL_FLD_gpidx_tbl_addr, (val))
#define G_GPIDX_TBL_CTRL_VAL_gpidx_tbl_wdata(val)              REG_FLD_VAL(G_GPIDX_TBL_CTRL_FLD_gpidx_tbl_wdata, (val))

#define G_GPIDX_TBL_STS_VAL_gpidx_cmd_done(val)                REG_FLD_VAL(G_GPIDX_TBL_STS_FLD_gpidx_cmd_done, (val))
#define G_GPIDX_TBL_STS_VAL_gpidx_tbl_rdata(val)               REG_FLD_VAL(G_GPIDX_TBL_STS_FLD_gpidx_tbl_rdata, (val))

#define G_GPIDX_TBL_INIT_VAL_gpidx_tbl_init_done(val)          REG_FLD_VAL(G_GPIDX_TBL_INIT_FLD_gpidx_tbl_init_done, (val))
#define G_GPIDX_TBL_INIT_VAL_gpidx_tbl_init_start(val)         REG_FLD_VAL(G_GPIDX_TBL_INIT_FLD_gpidx_tbl_init_start, (val))

#define G_MBI_STOP_VAL_mbi_tx_stop(val)                        REG_FLD_VAL(G_MBI_STOP_FLD_mbi_tx_stop, (val))
#define G_MBI_STOP_VAL_mbi_rx_stop(val)                        REG_FLD_VAL(G_MBI_STOP_FLD_mbi_rx_stop, (val))

#define G_TCONT_ID_16_31_CFG_VAL_tcont_cmd(val)                REG_FLD_VAL(G_TCONT_ID_16_31_CFG_FLD_tcont_cmd, (val))
#define G_TCONT_ID_16_31_CFG_VAL_wr_tcont_id_vld(val)          REG_FLD_VAL(G_TCONT_ID_16_31_CFG_FLD_wr_tcont_id_vld, (val))
#define G_TCONT_ID_16_31_CFG_VAL_tcont_id_index(val)           REG_FLD_VAL(G_TCONT_ID_16_31_CFG_FLD_tcont_id_index, (val))
#define G_TCONT_ID_16_31_CFG_VAL_wr_tcont_id(val)              REG_FLD_VAL(G_TCONT_ID_16_31_CFG_FLD_wr_tcont_id, (val))

#define G_TCONT_ID_16_31_STS_VAL_tcont_cmd_done(val)           REG_FLD_VAL(G_TCONT_ID_16_31_STS_FLD_tcont_cmd_done, (val))
#define G_TCONT_ID_16_31_STS_VAL_rd_tcont_id_vld(val)          REG_FLD_VAL(G_TCONT_ID_16_31_STS_FLD_rd_tcont_id_vld, (val))
#define G_TCONT_ID_16_31_STS_VAL_rd_tcont_id(val)              REG_FLD_VAL(G_TCONT_ID_16_31_STS_FLD_rd_tcont_id, (val))

#define DBG_CAP_SETTING_VAL_mib_frame_type(val)                REG_FLD_VAL(DBG_CAP_SETTING_FLD_mib_frame_type, (val))
#define DBG_CAP_SETTING_VAL_gpon_mib_en(val)                   REG_FLD_VAL(DBG_CAP_SETTING_FLD_gpon_mib_en, (val))
#define DBG_CAP_SETTING_VAL_us_no_msg_int_en(val)              REG_FLD_VAL(DBG_CAP_SETTING_FLD_us_no_msg_int_en, (val))
#define DBG_CAP_SETTING_VAL_rpt_msg_flt(val)                   REG_FLD_VAL(DBG_CAP_SETTING_FLD_rpt_msg_flt, (val))
#define DBG_CAP_SETTING_VAL_max_rdm_dly(val)                   REG_FLD_VAL(DBG_CAP_SETTING_FLD_max_rdm_dly, (val))

#define DBG_US_PREF_OFS_VAL_sch_pref_ofs(val)                  REG_FLD_VAL(DBG_US_PREF_OFS_FLD_sch_pref_ofs, (val))
#define DBG_US_PREF_OFS_VAL_dt_pref_ofs(val)                   REG_FLD_VAL(DBG_US_PREF_OFS_FLD_dt_pref_ofs, (val))

#define DBG_DLY_VAL_phy_rx_dly_sel(val)                        REG_FLD_VAL(DBG_DLY_FLD_phy_rx_dly_sel, (val))
#define DBG_DLY_VAL_fix_phy_rx_dly(val)                        REG_FLD_VAL(DBG_DLY_FLD_fix_phy_rx_dly, (val))
#define DBG_DLY_VAL_fine_int_dly(val)                          REG_FLD_VAL(DBG_DLY_FLD_fine_int_dly, (val))
#define DBG_DLY_VAL_phy_tx_dly(val)                            REG_FLD_VAL(DBG_DLY_FLD_phy_tx_dly, (val))

#define DBG_IDLE_GEM_THLD_VAL_idle_gem_thld(val)               REG_FLD_VAL(DBG_IDLE_GEM_THLD_FLD_idle_gem_thld, (val))

#define DBG_US_NO_MSG0_VAL_us_no_msg_b3_b4(val)                REG_FLD_VAL(DBG_US_NO_MSG0_FLD_us_no_msg_b3_b4, (val))
#define DBG_US_NO_MSG0_VAL_us_no_msg_type(val)                 REG_FLD_VAL(DBG_US_NO_MSG0_FLD_us_no_msg_type, (val))

#define DBG_US_NO_MSG1_VAL_us_no_msg_b5_b6(val)                REG_FLD_VAL(DBG_US_NO_MSG1_FLD_us_no_msg_b5_b6, (val))
#define DBG_US_NO_MSG1_VAL_us_no_msg_b7_b8(val)                REG_FLD_VAL(DBG_US_NO_MSG1_FLD_us_no_msg_b7_b8, (val))

#define DBG_US_NO_MSG2_VAL_us_no_msg_b9_b10(val)               REG_FLD_VAL(DBG_US_NO_MSG2_FLD_us_no_msg_b9_b10, (val))
#define DBG_US_NO_MSG2_VAL_us_no_msg_b11_b12(val)              REG_FLD_VAL(DBG_US_NO_MSG2_FLD_us_no_msg_b11_b12, (val))

#define DBG_US_DYING_GASP_CTRL_VAL_dying_gasp_num(val)         REG_FLD_VAL(DBG_US_DYING_GASP_CTRL_FLD_dying_gasp_num, (val))
#define DBG_US_DYING_GASP_CTRL_VAL_dying_gasp_test(val)        REG_FLD_VAL(DBG_US_DYING_GASP_CTRL_FLD_dying_gasp_test, (val))
#define DBG_US_DYING_GASP_CTRL_VAL_hw_dying_gasp_en(val)       REG_FLD_VAL(DBG_US_DYING_GASP_CTRL_FLD_hw_dying_gasp_en, (val))
#define DBG_US_DYING_GASP_CTRL_VAL_dying_gasp_msg_type(val)    REG_FLD_VAL(DBG_US_DYING_GASP_CTRL_FLD_dying_gasp_msg_type, (val))

#define DBG_BWM_FILTER_CTRL_VAL_bwm_intvl_invld(val)           REG_FLD_VAL(DBG_BWM_FILTER_CTRL_FLD_bwm_intvl_invld, (val))
#define DBG_BWM_FILTER_CTRL_VAL_bwm_len_invld(val)             REG_FLD_VAL(DBG_BWM_FILTER_CTRL_FLD_bwm_len_invld, (val))
#define DBG_BWM_FILTER_CTRL_VAL_bwm_start_order_err(val)       REG_FLD_VAL(DBG_BWM_FILTER_CTRL_FLD_bwm_start_order_err, (val))
#define DBG_BWM_FILTER_CTRL_VAL_bwm_start_order_err_flt_en(val) REG_FLD_VAL(DBG_BWM_FILTER_CTRL_FLD_bwm_start_order_err_flt_en, (val))
#define DBG_BWM_FILTER_CTRL_VAL_bwm_len_vld_chk_en(val)        REG_FLD_VAL(DBG_BWM_FILTER_CTRL_FLD_bwm_len_vld_chk_en, (val))
#define DBG_BWM_FILTER_CTRL_VAL_min_bst_intvl_en(val)          REG_FLD_VAL(DBG_BWM_FILTER_CTRL_FLD_min_bst_intvl_en, (val))
#define DBG_BWM_FILTER_CTRL_VAL_min_bst_intvl(val)             REG_FLD_VAL(DBG_BWM_FILTER_CTRL_FLD_min_bst_intvl, (val))

#define DBG_BWM_SFIFO_STS_VAL_bwm_sfifo_max_used(val)          REG_FLD_VAL(DBG_BWM_SFIFO_STS_FLD_bwm_sfifo_max_used, (val))
#define DBG_BWM_SFIFO_STS_VAL_bwm_sfifo_used(val)              REG_FLD_VAL(DBG_BWM_SFIFO_STS_FLD_bwm_sfifo_used, (val))

#define DBG_GRP_0_VAL_ether_fcs_err(val)                       REG_FLD_VAL(DBG_GRP_0_FLD_ether_fcs_err, (val))
#define DBG_GRP_0_VAL_ploamu_wr(val)                           REG_FLD_VAL(DBG_GRP_0_FLD_ploamu_wr, (val))
#define DBG_GRP_0_VAL_dbru_wr(val)                             REG_FLD_VAL(DBG_GRP_0_FLD_dbru_wr, (val))
#define DBG_GRP_0_VAL_is_gem_pl(val)                           REG_FLD_VAL(DBG_GRP_0_FLD_is_gem_pl, (val))
#define DBG_GRP_0_VAL_is_fcs_only(val)                         REG_FLD_VAL(DBG_GRP_0_FLD_is_fcs_only, (val))
#define DBG_GRP_0_VAL_is_inst_idle(val)                        REG_FLD_VAL(DBG_GRP_0_FLD_is_inst_idle, (val))
#define DBG_GRP_0_VAL_mbi_pl_fifo_ovrn(val)                    REG_FLD_VAL(DBG_GRP_0_FLD_mbi_pl_fifo_ovrn, (val))
#define DBG_GRP_0_VAL_aes_egn3_busy(val)                       REG_FLD_VAL(DBG_GRP_0_FLD_aes_egn3_busy, (val))
#define DBG_GRP_0_VAL_aes_egn2_busy(val)                       REG_FLD_VAL(DBG_GRP_0_FLD_aes_egn2_busy, (val))
#define DBG_GRP_0_VAL_aes_egn1_busy(val)                       REG_FLD_VAL(DBG_GRP_0_FLD_aes_egn1_busy, (val))
#define DBG_GRP_0_VAL_aes_egn0_busy(val)                       REG_FLD_VAL(DBG_GRP_0_FLD_aes_egn0_busy, (val))
#define DBG_GRP_0_VAL_aes_rdm_ciph_fifo_ovrn(val)              REG_FLD_VAL(DBG_GRP_0_FLD_aes_rdm_ciph_fifo_ovrn, (val))

#define DBG_GRP_1_VAL_alen_neq_zero(val)                       REG_FLD_VAL(DBG_GRP_1_FLD_alen_neq_zero, (val))
#define DBG_GRP_1_VAL_tx_align_fifo_udrn(val)                  REG_FLD_VAL(DBG_GRP_1_FLD_tx_align_fifo_udrn, (val))
#define DBG_GRP_1_VAL_rx_sof_sync(val)                         REG_FLD_VAL(DBG_GRP_1_FLD_rx_sof_sync, (val))
#define DBG_GRP_1_VAL_sync_start(val)                          REG_FLD_VAL(DBG_GRP_1_FLD_sync_start, (val))
#define DBG_GRP_1_VAL_tx_late_start(val)                       REG_FLD_VAL(DBG_GRP_1_FLD_tx_late_start, (val))
#define DBG_GRP_1_VAL_bst_next_full(val)                       REG_FLD_VAL(DBG_GRP_1_FLD_bst_next_full, (val))
#define DBG_GRP_1_VAL_sgl_next_full(val)                       REG_FLD_VAL(DBG_GRP_1_FLD_sgl_next_full, (val))
#define DBG_GRP_1_VAL_aes_cryp_cnt_fifo_ovrn(val)              REG_FLD_VAL(DBG_GRP_1_FLD_aes_cryp_cnt_fifo_ovrn, (val))
#define DBG_GRP_1_VAL_aes_ciph_txt_fifo_ovrn(val)              REG_FLD_VAL(DBG_GRP_1_FLD_aes_ciph_txt_fifo_ovrn, (val))
#define DBG_GRP_1_VAL_mbi_hdr_fifo_ovrn(val)                   REG_FLD_VAL(DBG_GRP_1_FLD_mbi_hdr_fifo_ovrn, (val))
#define DBG_GRP_1_VAL_ploamd_crc_err(val)                      REG_FLD_VAL(DBG_GRP_1_FLD_ploamd_crc_err, (val))
#define DBG_GRP_1_VAL_bwm_uc_err(val)                          REG_FLD_VAL(DBG_GRP_1_FLD_bwm_uc_err, (val))
#define DBG_GRP_1_VAL_blen_err(val)                            REG_FLD_VAL(DBG_GRP_1_FLD_blen_err, (val))
#define DBG_GRP_1_VAL_logd(val)                                REG_FLD_VAL(DBG_GRP_1_FLD_logd, (val))

#define DBG_TXFIFO_MAX_USED_VAL_tx_align_max_used(val)         REG_FLD_VAL(DBG_TXFIFO_MAX_USED_FLD_tx_align_max_used, (val))

#define DBG_BWM_GNT_STS_VAL_pls_used(val)                      REG_FLD_VAL(DBG_BWM_GNT_STS_FLD_pls_used, (val))
#define DBG_BWM_GNT_STS_VAL_dba_mode2_used(val)                REG_FLD_VAL(DBG_BWM_GNT_STS_FLD_dba_mode2_used, (val))
#define DBG_BWM_GNT_STS_VAL_dba_mode1_used(val)                REG_FLD_VAL(DBG_BWM_GNT_STS_FLD_dba_mode1_used, (val))
#define DBG_BWM_GNT_STS_VAL_dba_mode0_used(val)                REG_FLD_VAL(DBG_BWM_GNT_STS_FLD_dba_mode0_used, (val))
#define DBG_BWM_GNT_STS_VAL_max_assigned_gnt_num(val)          REG_FLD_VAL(DBG_BWM_GNT_STS_FLD_max_assigned_gnt_num, (val))

#define DBG_PROBE_CTRL_VAL_gpon_sub_probe_sel(val)             REG_FLD_VAL(DBG_PROBE_CTRL_FLD_gpon_sub_probe_sel, (val))
#define DBG_PROBE_CTRL_VAL_probe_bit0_sel(val)                 REG_FLD_VAL(DBG_PROBE_CTRL_FLD_probe_bit0_sel, (val))
#define DBG_PROBE_CTRL_VAL_bwm_probe_sel(val)                  REG_FLD_VAL(DBG_PROBE_CTRL_FLD_bwm_probe_sel, (val))
#define DBG_PROBE_CTRL_VAL_tx_align_probe_dt_sel(val)          REG_FLD_VAL(DBG_PROBE_CTRL_FLD_tx_align_probe_dt_sel, (val))
#define DBG_PROBE_CTRL_VAL_rx_mpi_mbi_probe_dt_sel(val)        REG_FLD_VAL(DBG_PROBE_CTRL_FLD_rx_mpi_mbi_probe_dt_sel, (val))
#define DBG_PROBE_CTRL_VAL_tx_mpi_mbi_probe_dt_sel(val)        REG_FLD_VAL(DBG_PROBE_CTRL_FLD_tx_mpi_mbi_probe_dt_sel, (val))
#define DBG_PROBE_CTRL_VAL_rx_data_hl_sel(val)                 REG_FLD_VAL(DBG_PROBE_CTRL_FLD_rx_data_hl_sel, (val))
#define DBG_PROBE_CTRL_VAL_probe_sel(val)                      REG_FLD_VAL(DBG_PROBE_CTRL_FLD_probe_sel, (val))

#define DBG_PROBE_HIGH32_VAL_probe_high32(val)                 REG_FLD_VAL(DBG_PROBE_HIGH32_FLD_probe_high32, (val))

#define DBG_PROBE_LOW32_VAL_probe_low32(val)                   REG_FLD_VAL(DBG_PROBE_LOW32_FLD_probe_low32, (val))

#define DBG_BWM_BFIFO_STS_VAL_bwm_bfifo_max_used(val)          REG_FLD_VAL(DBG_BWM_BFIFO_STS_FLD_bwm_bfifo_max_used, (val))
#define DBG_BWM_BFIFO_STS_VAL_bwm_bfifo_used(val)              REG_FLD_VAL(DBG_BWM_BFIFO_STS_FLD_bwm_bfifo_used, (val))

#define DBG_ERR_CTRL_VAL_bwm_fec_ctrl(val)                     REG_FLD_VAL(DBG_ERR_CTRL_FLD_bwm_fec_ctrl, (val))
#define DBG_ERR_CTRL_VAL_ignore_atm_cell(val)                  REG_FLD_VAL(DBG_ERR_CTRL_FLD_ignore_atm_cell, (val))
#define DBG_ERR_CTRL_VAL_o5_eqd_adj_resync_en(val)             REG_FLD_VAL(DBG_ERR_CTRL_FLD_o5_eqd_adj_resync_en, (val))
#define DBG_ERR_CTRL_VAL_tx_udrn_resync_en(val)                REG_FLD_VAL(DBG_ERR_CTRL_FLD_tx_udrn_resync_en, (val))
#define DBG_ERR_CTRL_VAL_tx_late_resync_en(val)                REG_FLD_VAL(DBG_ERR_CTRL_FLD_tx_late_resync_en, (val))
#define DBG_ERR_CTRL_VAL_bst_sgl_diff_resync_en(val)           REG_FLD_VAL(DBG_ERR_CTRL_FLD_bst_sgl_diff_resync_en, (val))
#define DBG_ERR_CTRL_VAL_ignore_ploamd_crc(val)                REG_FLD_VAL(DBG_ERR_CTRL_FLD_ignore_ploamd_crc, (val))

#define DBG_SW_RDI_CTRL_VAL_rdi_en(val)                        REG_FLD_VAL(DBG_SW_RDI_CTRL_FLD_rdi_en, (val))
#define DBG_SW_RDI_CTRL_VAL_sw_rdi(val)                        REG_FLD_VAL(DBG_SW_RDI_CTRL_FLD_sw_rdi, (val))

#define DBG_RX_GEM_CNT_VAL_dbg_rx_gem_cnt(val)                 REG_FLD_VAL(DBG_RX_GEM_CNT_FLD_dbg_rx_gem_cnt, (val))

#define DBG_RX_CRC_ERR_CNT_VAL_dbg_rx_crc_cnt(val)             REG_FLD_VAL(DBG_RX_CRC_ERR_CNT_FLD_dbg_rx_crc_cnt, (val))

#define DBG_RX_GTC_CNT_VAL_dbg_rx_gtc_cnt(val)                 REG_FLD_VAL(DBG_RX_GTC_CNT_FLD_dbg_rx_gtc_cnt, (val))

#define DBG_TX_GEM_CNT_VAL_dbg_tx_gem_cnt(val)                 REG_FLD_VAL(DBG_TX_GEM_CNT_FLD_dbg_tx_gem_cnt, (val))

#define DBG_TX_BST_CNT_VAL_dbg_tx_burst_cnt(val)               REG_FLD_VAL(DBG_TX_BST_CNT_FLD_dbg_tx_burst_cnt, (val))

#define DBG_GEM_HEC_ONE_ERR_CNT_VAL_dbg_gem_one_err_cnt(val)   REG_FLD_VAL(DBG_GEM_HEC_ONE_ERR_CNT_FLD_dbg_gem_one_err_cnt, (val))

#define DBG_GEM_HEC_TWO_ERR_CNT_VAL_dbg_gem_two_err_cnt(val)   REG_FLD_VAL(DBG_GEM_HEC_TWO_ERR_CNT_FLD_dbg_gem_two_err_cnt, (val))

#define DBG_GEM_HEC_UC_ERR_CNT_VAL_dbg_gem_uc_err_cnt(val)     REG_FLD_VAL(DBG_GEM_HEC_UC_ERR_CNT_FLD_dbg_gem_uc_err_cnt, (val))

#define DBG_DBA_BACK_DOOR_VAL_dbg_sw_rpt_dba_en(val)           REG_FLD_VAL(DBG_DBA_BACK_DOOR_FLD_dbg_sw_rpt_dba_en, (val))
#define DBG_DBA_BACK_DOOR_VAL_dbg_slight_modify_en(val)        REG_FLD_VAL(DBG_DBA_BACK_DOOR_FLD_dbg_slight_modify_en, (val))

#define DBG_DBA_TOTAL_BUF_SIZE_VAL_dbg_dba_total_buf_size(val) REG_FLD_VAL(DBG_DBA_TOTAL_BUF_SIZE_FLD_dbg_dba_total_buf_size, (val))

#define DBG_DBA_GREEN_SIZE_VAL_dbg_dba_green_size(val)         REG_FLD_VAL(DBG_DBA_GREEN_SIZE_FLD_dbg_dba_green_size, (val))

#define DBG_DBA_YELLOW_SIZE_VAL_dbg_dba_yellow_size(val)       REG_FLD_VAL(DBG_DBA_YELLOW_SIZE_FLD_dbg_dba_yellow_size, (val))

#define DBG_SLIGHT_MODIFY_SIZE_1_VAL_dbg_yellow_slight_modify_size(val) REG_FLD_VAL(DBG_SLIGHT_MODIFY_SIZE_1_FLD_dbg_yellow_slight_modify_size, (val))
#define DBG_SLIGHT_MODIFY_SIZE_1_VAL_dbg_green_slight_modify_size(val) REG_FLD_VAL(DBG_SLIGHT_MODIFY_SIZE_1_FLD_dbg_green_slight_modify_size, (val))

#define DBG_SLIGHT_MODIFY_SIZE_2_VAL_dbg_total_slight_modify_size(val) REG_FLD_VAL(DBG_SLIGHT_MODIFY_SIZE_2_FLD_dbg_total_slight_modify_size, (val))

#define DBG_DS_SPF_CNT_VAL_dbg_ds_spf_cnt(val)                 REG_FLD_VAL(DBG_DS_SPF_CNT_FLD_dbg_ds_spf_cnt, (val))

#define DBG_TX_SYNC_OFFSET_VAL_dbg_tx_sync_offset(val)         REG_FLD_VAL(DBG_TX_SYNC_OFFSET_FLD_dbg_tx_sync_offset, (val))

#define DBG_PLOAMD_FILTER_IN_O5_VAL_dbg_ext_bst_len_filter_in_o5_en(val) REG_FLD_VAL(DBG_PLOAMD_FILTER_IN_O5_FLD_dbg_ext_bst_len_filter_in_o5_en, (val))
#define DBG_PLOAMD_FILTER_IN_O5_VAL_dbg_us_overhead_filter_in_o5_en(val) REG_FLD_VAL(DBG_PLOAMD_FILTER_IN_O5_FLD_dbg_us_overhead_filter_in_o5_en, (val))

#define DBG_SW_RESYNC_VAL_dbg_sw_resync_en(val)                REG_FLD_VAL(DBG_SW_RESYNC_FLD_dbg_sw_resync_en, (val))
#define DBG_SW_RESYNC_VAL_dbg_sw_resync(val)                   REG_FLD_VAL(DBG_SW_RESYNC_FLD_dbg_sw_resync, (val))

#define DBG_GTC_ETH_EXTR_VAL_sniff_dbg_tx_rst(val)             REG_FLD_VAL(DBG_GTC_ETH_EXTR_FLD_sniff_dbg_tx_rst, (val))
#define DBG_GTC_ETH_EXTR_VAL_sniff_pkt_padding_en(val)         REG_FLD_VAL(DBG_GTC_ETH_EXTR_FLD_sniff_pkt_padding_en, (val))
#define DBG_GTC_ETH_EXTR_VAL_dbg_gtc_eth_extr_en(val)          REG_FLD_VAL(DBG_GTC_ETH_EXTR_FLD_dbg_gtc_eth_extr_en, (val))

#define DBG_DS_GTC_EXTR_ETH_HDR_VAL_dbg_extr_rx_ds_eth_type(val) REG_FLD_VAL(DBG_DS_GTC_EXTR_ETH_HDR_FLD_dbg_extr_rx_ds_eth_type, (val))
#define DBG_DS_GTC_EXTR_ETH_HDR_VAL_dbg_extr_rx_ds_sp(val)     REG_FLD_VAL(DBG_DS_GTC_EXTR_ETH_HDR_FLD_dbg_extr_rx_ds_sp, (val))

#define DBG_US_GTC_EXTR_ETH_HDR_VAL_dbg_extr_tx_us_eth_type(val) REG_FLD_VAL(DBG_US_GTC_EXTR_ETH_HDR_FLD_dbg_extr_tx_us_eth_type, (val))
#define DBG_US_GTC_EXTR_ETH_HDR_VAL_dbg_extr_tx_us_sp(val)     REG_FLD_VAL(DBG_US_GTC_EXTR_ETH_HDR_FLD_dbg_extr_tx_us_sp, (val))

#define DBG_DS_GTC_EXTR_ETH_CNT_VAL_dbg_extr_rx_cnt(val)       REG_FLD_VAL(DBG_DS_GTC_EXTR_ETH_CNT_FLD_dbg_extr_rx_cnt, (val))

#define DBG_US_GTC_EXTR_ETH_CNT_VAL_dbg_extr_tx_cnt(val)       REG_FLD_VAL(DBG_US_GTC_EXTR_ETH_CNT_FLD_dbg_extr_tx_cnt, (val))

#define SNIFF_GTC_GTC_INVLD_GEM_BYTE_VAL_dbg_ds_all_gem_filter(val) REG_FLD_VAL(SNIFF_GTC_GTC_INVLD_GEM_BYTE_FLD_dbg_ds_all_gem_filter, (val))
#define SNIFF_GTC_GTC_INVLD_GEM_BYTE_VAL_dbg_ds_gem_filter_exclude_omci(val) REG_FLD_VAL(SNIFF_GTC_GTC_INVLD_GEM_BYTE_FLD_dbg_ds_gem_filter_exclude_omci, (val))
#define SNIFF_GTC_GTC_INVLD_GEM_BYTE_VAL_dbg_us_gtc_invld_gem_byte(val) REG_FLD_VAL(SNIFF_GTC_GTC_INVLD_GEM_BYTE_FLD_dbg_us_gtc_invld_gem_byte, (val))

#define DBG_SNIFF_DBG_REG_VAL_dbg_mbi_pl_fifo_write_conflict(val) REG_FLD_VAL(DBG_SNIFF_DBG_REG_FLD_dbg_mbi_pl_fifo_write_conflict, (val))
#define DBG_SNIFF_DBG_REG_VAL_dbg_mbi_hdr_fifo_write_conflict(val) REG_FLD_VAL(DBG_SNIFF_DBG_REG_FLD_dbg_mbi_hdr_fifo_write_conflict, (val))
#define DBG_SNIFF_DBG_REG_VAL_dbg_us_gtc_info_fifo_ovrn(val)   REG_FLD_VAL(DBG_SNIFF_DBG_REG_FLD_dbg_us_gtc_info_fifo_ovrn, (val))

#define DBG_SNIFF_US_INFO_FIFO_VAL_dbg_us_info_tx_send_depth(val) REG_FLD_VAL(DBG_SNIFF_US_INFO_FIFO_FLD_dbg_us_info_tx_send_depth, (val))

#define SNIFF_US_HDR_WIDO_VAL_stop_time_of_wido_en(val)        REG_FLD_VAL(SNIFF_US_HDR_WIDO_FLD_stop_time_of_wido_en, (val))
#define SNIFF_US_HDR_WIDO_VAL_stop_time_of_wido(val)           REG_FLD_VAL(SNIFF_US_HDR_WIDO_FLD_stop_time_of_wido, (val))

#define SNIFF_TX_DA_SA_VAL_tx_us_da_h16(val)                   REG_FLD_VAL(SNIFF_TX_DA_SA_FLD_tx_us_da_h16, (val))
#define SNIFF_TX_DA_SA_VAL_tx_us_sa_h16(val)                   REG_FLD_VAL(SNIFF_TX_DA_SA_FLD_tx_us_sa_h16, (val))

#define SNIFF_RX_DA_SA_VAL_rx_ds_da_h16(val)                   REG_FLD_VAL(SNIFF_RX_DA_SA_FLD_rx_ds_da_h16, (val))
#define SNIFF_RX_DA_SA_VAL_rx_ds_sa_h16(val)                   REG_FLD_VAL(SNIFF_RX_DA_SA_FLD_rx_ds_sa_h16, (val))

#define SNIFF_TX_RX_PID_VAL_tx_eth_pid(val)                    REG_FLD_VAL(SNIFF_TX_RX_PID_FLD_tx_eth_pid, (val))
#define SNIFF_TX_RX_PID_VAL_rx_eth_pid(val)                    REG_FLD_VAL(SNIFF_TX_RX_PID_FLD_rx_eth_pid, (val))

#define SNIFF_CNT_EN_VAL_sniff_cnt_en(val)                     REG_FLD_VAL(SNIFF_CNT_EN_FLD_sniff_cnt_en, (val))

#define SNIFF_RX_TX_TPID_VAL_sniff_rx_tpid(val)                REG_FLD_VAL(SNIFF_RX_TX_TPID_FLD_sniff_rx_tpid, (val))
#define SNIFF_RX_TX_TPID_VAL_sniff_tx_tpid(val)                REG_FLD_VAL(SNIFF_RX_TX_TPID_FLD_sniff_tx_tpid, (val))

#define DBG_GPON_MAC_SET_VAL_asb_tx_eth_4byte_align_en(val)    REG_FLD_VAL(DBG_GPON_MAC_SET_FLD_asb_tx_eth_4byte_align_en, (val))
#define DBG_GPON_MAC_SET_VAL_olt_ds_fec(val)                   REG_FLD_VAL(DBG_GPON_MAC_SET_FLD_olt_ds_fec, (val))
#define DBG_GPON_MAC_SET_VAL_onu_us_fec(val)                   REG_FLD_VAL(DBG_GPON_MAC_SET_FLD_onu_us_fec, (val))
#define DBG_GPON_MAC_SET_VAL_gpon_mac_sw_rst_n(val)            REG_FLD_VAL(DBG_GPON_MAC_SET_FLD_gpon_mac_sw_rst_n, (val))

#define SLEEP_GLB_CFG_VAL_sleep_filt_all_bwm(val)              REG_FLD_VAL(SLEEP_GLB_CFG_FLD_sleep_filt_all_bwm, (val))
#define SLEEP_GLB_CFG_VAL_reload_sleep_cnt(val)                REG_FLD_VAL(SLEEP_GLB_CFG_FLD_reload_sleep_cnt, (val))

#define SLEEP_CNT_VAL_sleep_cnt(val)                           REG_FLD_VAL(SLEEP_CNT_FLD_sleep_cnt, (val))

#define TX_SYNC_OPT_VAL_tx_sync_opt(val)                       REG_FLD_VAL(TX_SYNC_OPT_FLD_tx_sync_opt, (val))

#define PLOAMU_CTRL_VAL_ploamu_ind_ctrl(val)                   REG_FLD_VAL(PLOAMU_CTRL_FLD_ploamu_ind_ctrl, (val))
#define PLOAMU_CTRL_VAL_o3_o4_ploamu_ctrl(val)                 REG_FLD_VAL(PLOAMU_CTRL_FLD_o3_o4_ploamu_ctrl, (val))

#define TOD_1PPS_WD_CTRL_VAL_tod_1pps_width_ctrl(val)          REG_FLD_VAL(TOD_1PPS_WD_CTRL_FLD_tod_1pps_width_ctrl, (val))

#ifdef __cplusplus
}
#endif

#endif // __gpon_mac_reg_REGS_H__
