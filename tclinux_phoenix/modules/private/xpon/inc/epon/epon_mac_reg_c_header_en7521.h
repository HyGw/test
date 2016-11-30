#ifndef __EPON_MAC_REGS_H__
#define __EPON_MAC_REGS_H__

#include <linux/mtd/rt_flash.h>
#include "common/drv_types.h"

#ifdef UINT32
#undef UINT32
#endif
#ifdef UINT16
#undef UINT16
#endif
#ifdef UINT8
#undef UINT8
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef REG_BASE_C_MODULE
// ----------------- EPON_MAC Bit Field Definitions -------------------


typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_27                    : 5;
        FIELD sniffer_mode              : 1;
        FIELD txoam_favor               : 1;
        FIELD burst_en_dly              : 1;
        FIELD discv_burst_en            : 1;
        FIELD mpcp_fwd                  : 1;
        FIELD bcst_llid_m1_drop         : 1;
        FIELD bcst_llid_m0_drop         : 1;
        FIELD mcst_llid_drop            : 1;
        FIELD all_unicast_llid_pkt_fwd  : 1;
        FIELD fcs_err_fwd               : 1;
        FIELD llid_crc8_err_fwd         : 1;
        FIELD rsv_14                    : 2;
        FIELD rxmpi_stop                : 1;
        FIELD txmpi_stop                : 1;
        FIELD phy_pwr_down              : 1;
        FIELD rx_nml_gate_fwd           : 1;
        FIELD rxmbi_stop                : 1;
        FIELD txmbi_stop                : 1;
        FIELD chk_all_gnt_mode          : 1;
        FIELD loc_cnt_sync_method       : 1;
        FIELD tx_default_rpt            : 1;
        FIELD epon_mac_sw_rst           : 1;
        FIELD epon_oam_cal_in_eth       : 1;
        FIELD epon_mac_lpbk_en          : 1;
        FIELD rpt_txpri_ctrl            : 1;
        FIELD mode_sel                  : 1;
#else
        FIELD mode_sel                  : 1;
        FIELD rpt_txpri_ctrl            : 1;
        FIELD epon_mac_lpbk_en          : 1;
        FIELD epon_oam_cal_in_eth       : 1;
        FIELD epon_mac_sw_rst           : 1;
        FIELD tx_default_rpt            : 1;
        FIELD loc_cnt_sync_method       : 1;
        FIELD chk_all_gnt_mode          : 1;
        FIELD txmbi_stop                : 1;
        FIELD rxmbi_stop                : 1;
        FIELD rx_nml_gate_fwd           : 1;
        FIELD phy_pwr_down              : 1;
        FIELD txmpi_stop                : 1;
        FIELD rxmpi_stop                : 1;
        FIELD rsv_14                    : 2;
        FIELD llid_crc8_err_fwd         : 1;
        FIELD fcs_err_fwd               : 1;
        FIELD all_unicast_llid_pkt_fwd  : 1;
        FIELD mcst_llid_drop            : 1;
        FIELD bcst_llid_m0_drop         : 1;
        FIELD bcst_llid_m1_drop         : 1;
        FIELD mpcp_fwd                  : 1;
        FIELD discv_burst_en            : 1;
        FIELD burst_en_dly              : 1;
        FIELD txoam_favor               : 1;
        FIELD sniffer_mode              : 1;
        FIELD rsv_27                    : 5;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_glb_cfg, *PREG_e_glb_cfg;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_27                    : 5;
        FIELD sniff_fifo_ovrun_int      : 1;
        FIELD reg_ack_done_int          : 1;
        FIELD reg_req_done_int          : 1;
        FIELD reorder1_gnt_int          : 1;
        FIELD b2b_gnt_int               : 1;
        FIELD hidn_gnt_int              : 1;
        FIELD ps_early_wakeup_int       : 1;
        FIELD rx_sleep_allow_int        : 1;
        FIELD ps_wakeup_int             : 1;
        FIELD ps_sleep_int              : 1;
        FIELD txfifo_udrun_int          : 1;
        FIELD rpt_overintvl_int         : 1;
        FIELD mpcp_timeout_int          : 1;
        FIELD timedrft_int              : 1;
        FIELD tod_1pps_int              : 1;
        FIELD tod_updt_int              : 1;
        FIELD ptp_msg_tx_int            : 1;
        FIELD gnt_buf_ovrrun_int        : 1;
        FIELD llid7_rcv_rgst_int        : 1;
        FIELD llid6_rcv_rgst_int        : 1;
        FIELD llid5_rcv_rgst_int        : 1;
        FIELD llid4_rcv_rgst_int        : 1;
        FIELD llid3_rcv_rgst_int        : 1;
        FIELD llid2_rcv_rgst_int        : 1;
        FIELD llid1_rcv_rgst_int        : 1;
        FIELD llid0_rcv_rgst_int        : 1;
        FIELD rcv_dscvry_gate_int       : 1;
#else
        FIELD rcv_dscvry_gate_int       : 1;
        FIELD llid0_rcv_rgst_int        : 1;
        FIELD llid1_rcv_rgst_int        : 1;
        FIELD llid2_rcv_rgst_int        : 1;
        FIELD llid3_rcv_rgst_int        : 1;
        FIELD llid4_rcv_rgst_int        : 1;
        FIELD llid5_rcv_rgst_int        : 1;
        FIELD llid6_rcv_rgst_int        : 1;
        FIELD llid7_rcv_rgst_int        : 1;
        FIELD gnt_buf_ovrrun_int        : 1;
        FIELD ptp_msg_tx_int            : 1;
        FIELD tod_updt_int              : 1;
        FIELD tod_1pps_int              : 1;
        FIELD timedrft_int              : 1;
        FIELD mpcp_timeout_int          : 1;
        FIELD rpt_overintvl_int         : 1;
        FIELD txfifo_udrun_int          : 1;
        FIELD ps_sleep_int              : 1;
        FIELD ps_wakeup_int             : 1;
        FIELD rx_sleep_allow_int        : 1;
        FIELD ps_early_wakeup_int       : 1;
        FIELD hidn_gnt_int              : 1;
        FIELD b2b_gnt_int               : 1;
        FIELD reorder1_gnt_int          : 1;
        FIELD reg_req_done_int          : 1;
        FIELD reg_ack_done_int          : 1;
        FIELD sniff_fifo_ovrun_int      : 1;
        FIELD rsv_27                    : 5;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_int_status, *PREG_e_int_status;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_27                    : 5;
        FIELD sniff_fifo_ovrun_en       : 1;
        FIELD reg_ack_done_en           : 1;
        FIELD reg_req_done_en           : 1;
        FIELD reorder1_gnt_en           : 1;
        FIELD b2b_gnt_en                : 1;
        FIELD hidn_gnt_en               : 1;
        FIELD ps_early_wakeup_en        : 1;
        FIELD rx_sleep_allow_en         : 1;
        FIELD ps_wakeup_en              : 1;
        FIELD ps_sleep_en               : 1;
        FIELD txfifo_udrun_en           : 1;
        FIELD rpt_overintvl_en          : 1;
        FIELD mpcp_timeout_en           : 1;
        FIELD timedrft_en               : 1;
        FIELD tod_1pps_en               : 1;
        FIELD tod_updt_en               : 1;
        FIELD ptp_msg_tx_en             : 1;
        FIELD gnt_buf_ovrrun_en         : 1;
        FIELD llid7_rcv_rgst_en         : 1;
        FIELD llid6_rcv_rgst_en         : 1;
        FIELD llid5_rcv_rgst_en         : 1;
        FIELD llid4_rcv_rgst_en         : 1;
        FIELD llid3_rcv_rgst_en         : 1;
        FIELD llid2_rcv_rgst_en         : 1;
        FIELD llid1_rcv_rgst_en         : 1;
        FIELD llid0_rcv_rgst_en         : 1;
        FIELD rcv_dscvry_gate_en        : 1;
#else
        FIELD rcv_dscvry_gate_en        : 1;
        FIELD llid0_rcv_rgst_en         : 1;
        FIELD llid1_rcv_rgst_en         : 1;
        FIELD llid2_rcv_rgst_en         : 1;
        FIELD llid3_rcv_rgst_en         : 1;
        FIELD llid4_rcv_rgst_en         : 1;
        FIELD llid5_rcv_rgst_en         : 1;
        FIELD llid6_rcv_rgst_en         : 1;
        FIELD llid7_rcv_rgst_en         : 1;
        FIELD gnt_buf_ovrrun_en         : 1;
        FIELD ptp_msg_tx_en             : 1;
        FIELD tod_updt_en               : 1;
        FIELD tod_1pps_en               : 1;
        FIELD timedrft_en               : 1;
        FIELD mpcp_timeout_en           : 1;
        FIELD rpt_overintvl_en          : 1;
        FIELD txfifo_udrun_en           : 1;
        FIELD ps_sleep_en               : 1;
        FIELD ps_wakeup_en              : 1;
        FIELD rx_sleep_allow_en         : 1;
        FIELD ps_early_wakeup_en        : 1;
        FIELD hidn_gnt_en               : 1;
        FIELD b2b_gnt_en                : 1;
        FIELD reorder1_gnt_en           : 1;
        FIELD reg_req_done_en           : 1;
        FIELD reg_ack_done_en           : 1;
        FIELD sniff_fifo_ovrun_en       : 1;
        FIELD rsv_27                    : 5;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_int_en, *PREG_e_int_en;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_16                    : 16;
        FIELD llid7_mpcp_tmo            : 1;
        FIELD llid6_mpcp_tmo            : 1;
        FIELD llid5_mpcp_tmo            : 1;
        FIELD llid4_mpcp_tmo            : 1;
        FIELD llid3_mpcp_tmo            : 1;
        FIELD llid2_mpcp_tmo            : 1;
        FIELD llid1_mpcp_tmo            : 1;
        FIELD llid0_mpcp_tmo            : 1;
        FIELD llid7_rpt_tmo             : 1;
        FIELD llid6_rpt_tmo             : 1;
        FIELD llid5_rpt_tmo             : 1;
        FIELD llid4_rpt_tmo             : 1;
        FIELD llid3_rpt_tmo             : 1;
        FIELD llid2_rpt_tmo             : 1;
        FIELD llid1_rpt_tmo             : 1;
        FIELD llid0_rpt_tmo             : 1;
#else
        FIELD llid0_rpt_tmo             : 1;
        FIELD llid1_rpt_tmo             : 1;
        FIELD llid2_rpt_tmo             : 1;
        FIELD llid3_rpt_tmo             : 1;
        FIELD llid4_rpt_tmo             : 1;
        FIELD llid5_rpt_tmo             : 1;
        FIELD llid6_rpt_tmo             : 1;
        FIELD llid7_rpt_tmo             : 1;
        FIELD llid0_mpcp_tmo            : 1;
        FIELD llid1_mpcp_tmo            : 1;
        FIELD llid2_mpcp_tmo            : 1;
        FIELD llid3_mpcp_tmo            : 1;
        FIELD llid4_mpcp_tmo            : 1;
        FIELD llid5_mpcp_tmo            : 1;
        FIELD llid6_mpcp_tmo            : 1;
        FIELD llid7_mpcp_tmo            : 1;
        FIELD rsv_16                    : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_rpt_mpcp_timeout_llid_idx, *PREG_e_rpt_mpcp_timeout_llid_idx;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD hw_dying_gasp_en          : 1;
        FIELD rsv_28                    : 3;
        FIELD dying_gasp_code           : 8;
        FIELD rsv_17                    : 3;
        FIELD sw_init_dying_gasp        : 1;
        FIELD rsv_10                    : 6;
        FIELD dygsp_num_of_times        : 2;
        FIELD rsv_7                     : 1;
        FIELD dying_gasp_flags          : 7;
#else
        FIELD dying_gasp_flags          : 7;
        FIELD rsv_7                     : 1;
        FIELD dygsp_num_of_times        : 2;
        FIELD rsv_10                    : 6;
        FIELD sw_init_dying_gasp        : 1;
        FIELD rsv_17                    : 3;
        FIELD dying_gasp_code           : 8;
        FIELD rsv_28                    : 3;
        FIELD hw_dying_gasp_en          : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_dyinggsp_cfg, *PREG_e_dyinggsp_cfg;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_16                    : 16;
        FIELD echoed_pending_gnt        : 8;
        FIELD rsv_7                     : 1;
        FIELD pending_gnt_num           : 7;
#else
        FIELD pending_gnt_num           : 7;
        FIELD rsv_7                     : 1;
        FIELD echoed_pending_gnt        : 8;
        FIELD rsv_16                    : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_pending_gnt_num, *PREG_e_pending_gnt_num;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_28                    : 4;
        FIELD llid3_txfec_en            : 1;
        FIELD llid3_dcrypt_en           : 1;
        FIELD llid3_dcrypt_mode         : 1;
        FIELD llid3_oam_lpbk_en         : 1;
        FIELD rsv_20                    : 4;
        FIELD llid2_txfec_en            : 1;
        FIELD llid2_dcrypt_en           : 1;
        FIELD llid2_dcrypt_mode         : 1;
        FIELD llid2_oam_lpbk_en         : 1;
        FIELD rsv_12                    : 4;
        FIELD llid1_txfec_en            : 1;
        FIELD llid1_dcrypt_en           : 1;
        FIELD llid1_dcrypt_mode         : 1;
        FIELD llid1_oam_lpbk_en         : 1;
        FIELD rsv_4                     : 4;
        FIELD llid0_txfec_en            : 1;
        FIELD llid0_dcrypt_en           : 1;
        FIELD llid0_dcrypt_mode         : 1;
        FIELD llid0_oam_lpbk_en         : 1;
#else
        FIELD llid0_oam_lpbk_en         : 1;
        FIELD llid0_dcrypt_mode         : 1;
        FIELD llid0_dcrypt_en           : 1;
        FIELD llid0_txfec_en            : 1;
        FIELD rsv_4                     : 4;
        FIELD llid1_oam_lpbk_en         : 1;
        FIELD llid1_dcrypt_mode         : 1;
        FIELD llid1_dcrypt_en           : 1;
        FIELD llid1_txfec_en            : 1;
        FIELD rsv_12                    : 4;
        FIELD llid2_oam_lpbk_en         : 1;
        FIELD llid2_dcrypt_mode         : 1;
        FIELD llid2_dcrypt_en           : 1;
        FIELD llid2_txfec_en            : 1;
        FIELD rsv_20                    : 4;
        FIELD llid3_oam_lpbk_en         : 1;
        FIELD llid3_dcrypt_mode         : 1;
        FIELD llid3_dcrypt_en           : 1;
        FIELD llid3_txfec_en            : 1;
        FIELD rsv_28                    : 4;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_llid0_3_cfg, *PREG_e_llid0_3_cfg;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_28                    : 4;
        FIELD llid7_txfec_en            : 1;
        FIELD llid7_dcrypt_en           : 1;
        FIELD llid7_dcrypt_mode         : 1;
        FIELD llid7_oam_lpbk_en         : 1;
        FIELD rsv_20                    : 4;
        FIELD llid6_txfec_en            : 1;
        FIELD llid6_dcrypt_en           : 1;
        FIELD llid6_dcrypt_mode         : 1;
        FIELD llid6_oam_lpbk_en         : 1;
        FIELD rsv_12                    : 4;
        FIELD llid5_txfec_en            : 1;
        FIELD llid5_dcrypt_en           : 1;
        FIELD llid5_dcrypt_mode         : 1;
        FIELD llid5_oam_lpbk_en         : 1;
        FIELD rsv_4                     : 4;
        FIELD llid4_txfec_en            : 1;
        FIELD llid4_dcrypt_en           : 1;
        FIELD llid4_dcrypt_mode         : 1;
        FIELD llid4_oam_lpbk_en         : 1;
#else
        FIELD llid4_oam_lpbk_en         : 1;
        FIELD llid4_dcrypt_mode         : 1;
        FIELD llid4_dcrypt_en           : 1;
        FIELD llid4_txfec_en            : 1;
        FIELD rsv_4                     : 4;
        FIELD llid5_oam_lpbk_en         : 1;
        FIELD llid5_dcrypt_mode         : 1;
        FIELD llid5_dcrypt_en           : 1;
        FIELD llid5_txfec_en            : 1;
        FIELD rsv_12                    : 4;
        FIELD llid6_oam_lpbk_en         : 1;
        FIELD llid6_dcrypt_mode         : 1;
        FIELD llid6_dcrypt_en           : 1;
        FIELD llid6_txfec_en            : 1;
        FIELD rsv_20                    : 4;
        FIELD llid7_oam_lpbk_en         : 1;
        FIELD llid7_dcrypt_mode         : 1;
        FIELD llid7_dcrypt_en           : 1;
        FIELD llid7_txfec_en            : 1;
        FIELD rsv_28                    : 4;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_llid4_7_cfg, *PREG_e_llid4_7_cfg;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD mpcp_cmd                  : 2;
        FIELD rsv_17                    : 13;
        FIELD mpcp_cmd_done             : 1;
        FIELD rsv_13                    : 3;
        FIELD rgstr_ack_flg             : 1;
        FIELD rsv_9                     : 3;
        FIELD rgstr_req_flg             : 1;
        FIELD rsv_3                     : 5;
        FIELD tx_mpcp_llid_idx          : 3;
#else
        FIELD tx_mpcp_llid_idx          : 3;
        FIELD rsv_3                     : 5;
        FIELD rgstr_req_flg             : 1;
        FIELD rsv_9                     : 3;
        FIELD rgstr_ack_flg             : 1;
        FIELD rsv_13                    : 3;
        FIELD mpcp_cmd_done             : 1;
        FIELD rsv_17                    : 13;
        FIELD mpcp_cmd                  : 2;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_llid_dscvry_ctrl, *PREG_e_llid_dscvry_ctrl;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD llid0_dscvry_sts          : 2;
        FIELD rsv_26                    : 4;
        FIELD llid0_rgstr_flg_sts       : 2;
        FIELD rsv_17                    : 7;
        FIELD llid0_valid               : 1;
        FIELD llid0_value               : 16;
#else
        FIELD llid0_value               : 16;
        FIELD llid0_valid               : 1;
        FIELD rsv_17                    : 7;
        FIELD llid0_rgstr_flg_sts       : 2;
        FIELD rsv_26                    : 4;
        FIELD llid0_dscvry_sts          : 2;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_llid0_dscvry_sts, *PREG_e_llid0_dscvry_sts;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD llid1_dscvry_sts          : 2;
        FIELD rsv_26                    : 4;
        FIELD llid1_rgstr_flg_sts       : 2;
        FIELD rsv_17                    : 7;
        FIELD llid1_valid               : 1;
        FIELD llid1_value               : 16;
#else
        FIELD llid1_value               : 16;
        FIELD llid1_valid               : 1;
        FIELD rsv_17                    : 7;
        FIELD llid1_rgstr_flg_sts       : 2;
        FIELD rsv_26                    : 4;
        FIELD llid1_dscvry_sts          : 2;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_llid1_dscvry_sts, *PREG_e_llid1_dscvry_sts;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD llid2_dscvry_sts          : 2;
        FIELD rsv_26                    : 4;
        FIELD llid2_rgstr_flg_sts       : 2;
        FIELD rsv_17                    : 7;
        FIELD llid2_valid               : 1;
        FIELD llid2_value               : 16;
#else
        FIELD llid2_value               : 16;
        FIELD llid2_valid               : 1;
        FIELD rsv_17                    : 7;
        FIELD llid2_rgstr_flg_sts       : 2;
        FIELD rsv_26                    : 4;
        FIELD llid2_dscvry_sts          : 2;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_llid2_dscvry_sts, *PREG_e_llid2_dscvry_sts;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD llid3_dscvry_sts          : 2;
        FIELD rsv_26                    : 4;
        FIELD llid3_rgstr_flg_sts       : 2;
        FIELD rsv_17                    : 7;
        FIELD llid3_valid               : 1;
        FIELD llid3_value               : 16;
#else
        FIELD llid3_value               : 16;
        FIELD llid3_valid               : 1;
        FIELD rsv_17                    : 7;
        FIELD llid3_rgstr_flg_sts       : 2;
        FIELD rsv_26                    : 4;
        FIELD llid3_dscvry_sts          : 2;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_llid3_dscvry_sts, *PREG_e_llid3_dscvry_sts;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD llid4_dscvry_sts          : 2;
        FIELD rsv_26                    : 4;
        FIELD llid4_rgstr_flg_sts       : 2;
        FIELD rsv_17                    : 7;
        FIELD llid4_valid               : 1;
        FIELD llid4_value               : 16;
#else
        FIELD llid4_value               : 16;
        FIELD llid4_valid               : 1;
        FIELD rsv_17                    : 7;
        FIELD llid4_rgstr_flg_sts       : 2;
        FIELD rsv_26                    : 4;
        FIELD llid4_dscvry_sts          : 2;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_llid4_dscvry_sts, *PREG_e_llid4_dscvry_sts;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD llid5_dscvry_sts          : 2;
        FIELD rsv_26                    : 4;
        FIELD llid5_rgstr_flg_sts       : 2;
        FIELD rsv_17                    : 7;
        FIELD llid5_valid               : 1;
        FIELD llid5_value               : 16;
#else
        FIELD llid5_value               : 16;
        FIELD llid5_valid               : 1;
        FIELD rsv_17                    : 7;
        FIELD llid5_rgstr_flg_sts       : 2;
        FIELD rsv_26                    : 4;
        FIELD llid5_dscvry_sts          : 2;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_llid5_dscvry_sts, *PREG_e_llid5_dscvry_sts;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD llid6_dscvry_sts          : 2;
        FIELD rsv_26                    : 4;
        FIELD llid6_rgstr_flg_sts       : 2;
        FIELD rsv_17                    : 7;
        FIELD llid6_valid               : 1;
        FIELD llid6_value               : 16;
#else
        FIELD llid6_value               : 16;
        FIELD llid6_valid               : 1;
        FIELD rsv_17                    : 7;
        FIELD llid6_rgstr_flg_sts       : 2;
        FIELD rsv_26                    : 4;
        FIELD llid6_dscvry_sts          : 2;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_llid6_dscvry_sts, *PREG_e_llid6_dscvry_sts;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD llid7_dscvry_sts          : 2;
        FIELD rsv_26                    : 4;
        FIELD llid7_rgstr_flg_sts       : 2;
        FIELD rsv_17                    : 7;
        FIELD llid7_valid               : 1;
        FIELD llid7_value               : 16;
#else
        FIELD llid7_value               : 16;
        FIELD llid7_valid               : 1;
        FIELD rsv_17                    : 7;
        FIELD llid7_rgstr_flg_sts       : 2;
        FIELD rsv_26                    : 4;
        FIELD llid7_dscvry_sts          : 2;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_llid7_dscvry_sts, *PREG_e_llid7_dscvry_sts;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD mac_addr_rwcmd            : 1;
        FIELD rsv_17                    : 14;
        FIELD mac_addr_rwcmd_done       : 1;
        FIELD rsv_4                     : 12;
        FIELD mac_addr_llid_indx        : 3;
        FIELD mac_addr_dw_idx           : 1;
#else
        FIELD mac_addr_dw_idx           : 1;
        FIELD mac_addr_llid_indx        : 3;
        FIELD rsv_4                     : 12;
        FIELD mac_addr_rwcmd_done       : 1;
        FIELD rsv_17                    : 14;
        FIELD mac_addr_rwcmd            : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_mac_addr_cfg, *PREG_e_mac_addr_cfg;

typedef PACKING union
{
    PACKING struct
    {
        FIELD mac_addr_value            : 32;
    } Bits;
    UINT32 Raw;
} REG_e_mac_addr_value, *PREG_e_mac_addr_value;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD key_rwcmd                 : 1;
        FIELD rsv_17                    : 14;
        FIELD key_rwcmd_done            : 1;
        FIELD rsv_6                     : 10;
        FIELD key_llid_index            : 3;
        FIELD key_idx                   : 1;
        FIELD key_dw_indx               : 2;
#else
        FIELD key_dw_indx               : 2;
        FIELD key_idx                   : 1;
        FIELD key_llid_index            : 3;
        FIELD rsv_6                     : 10;
        FIELD key_rwcmd_done            : 1;
        FIELD rsv_17                    : 14;
        FIELD key_rwcmd                 : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_security_key_cfg, *PREG_e_security_key_cfg;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_24                    : 8;
        FIELD key_value                 : 24;
#else
        FIELD key_value                 : 24;
        FIELD rsv_24                    : 8;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_key_value, *PREG_e_key_value;

typedef PACKING union
{
    PACKING struct
    {
        FIELD df_rpt_data               : 32;
    } Bits;
    UINT32 Raw;
} REG_e_rpt_data, *PREG_e_rpt_data;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_9                     : 23;
        FIELD df_rpt_fifo_clr           : 1;
        FIELD rsv_6                     : 2;
        FIELD df_rpt_dt_len             : 6;
#else
        FIELD df_rpt_dt_len             : 6;
        FIELD rsv_6                     : 2;
        FIELD df_rpt_fifo_clr           : 1;
        FIELD rsv_9                     : 23;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_rpt_len, *PREG_e_rpt_len;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD qsize_fec_adj             : 16;
        FIELD llid7_rpt_cfg             : 2;
        FIELD llid6_rpt_cfg             : 2;
        FIELD llid5_rpt_cfg             : 2;
        FIELD llid4_rpt_cfg             : 2;
        FIELD llid3_rpt_cfg             : 2;
        FIELD llid2_rpt_cfg             : 2;
        FIELD llid1_rpt_cfg             : 2;
        FIELD llid0_rpt_cfg             : 2;
#else
        FIELD llid0_rpt_cfg             : 2;
        FIELD llid1_rpt_cfg             : 2;
        FIELD llid2_rpt_cfg             : 2;
        FIELD llid3_rpt_cfg             : 2;
        FIELD llid4_rpt_cfg             : 2;
        FIELD llid5_rpt_cfg             : 2;
        FIELD llid6_rpt_cfg             : 2;
        FIELD llid7_rpt_cfg             : 2;
        FIELD qsize_fec_adj             : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_rpt_cfg, *PREG_e_rpt_cfg;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD qthld_rwcmd               : 1;
        FIELD qthld_rwcmd_done          : 1;
        FIELD rsv_24                    : 6;
        FIELD qthld_value               : 16;
        FIELD qthld_idx                 : 2;
        FIELD rpt_llid_idx              : 3;
        FIELD queue_idx                 : 3;
#else
        FIELD queue_idx                 : 3;
        FIELD rpt_llid_idx              : 3;
        FIELD qthld_idx                 : 2;
        FIELD qthld_value               : 16;
        FIELD rsv_24                    : 6;
        FIELD qthld_rwcmd_done          : 1;
        FIELD qthld_rwcmd               : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_rpt_qthld_cfg, *PREG_e_rpt_qthld_cfg;

typedef PACKING union
{
    PACKING struct
    {
        FIELD local_time                : 32;
    } Bits;
    UINT32 Raw;
} REG_e_local_time, *PREG_e_local_time;

typedef PACKING union
{
    PACKING struct
    {
        FIELD tod_sync_x                : 32;
    } Bits;
    UINT32 Raw;
} REG_e_tod_sync_x, *PREG_e_tod_sync_x;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_19                    : 13;
        FIELD ingrs_latency             : 11;
        FIELD egrs_latency              : 8;
#else
        FIELD egrs_latency              : 8;
        FIELD ingrs_latency             : 11;
        FIELD rsv_19                    : 13;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_tod_ltncy, *PREG_e_tod_ltncy;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD p2p_tx_tagging_done       : 1;
        FIELD rsv_12                    : 19;
        FIELD p2p_tx_pkt_info           : 4;
        FIELD p2p_tx_tag_sec_l8         : 8;
#else
        FIELD p2p_tx_tag_sec_l8         : 8;
        FIELD p2p_tx_pkt_info           : 4;
        FIELD rsv_12                    : 19;
        FIELD p2p_tx_tagging_done       : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_p2p_tx_tag1, *PREG_p2p_tx_tag1;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_25                    : 7;
        FIELD p2p_tx_tag_nsec_29_5      : 25;
#else
        FIELD p2p_tx_tag_nsec_29_5      : 25;
        FIELD rsv_25                    : 7;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_p2p_tx_tag2, *PREG_p2p_tx_tag2;

typedef PACKING union
{
    PACKING struct
    {
        FIELD new_tod_p2p_offset_sec_l32 : 32;
    } Bits;
    UINT32 Raw;
} REG_e_new_tod_p2p_offset_sec_l32, *PREG_e_new_tod_p2p_offset_sec_l32;

typedef PACKING union
{
    PACKING struct
    {
        FIELD new_tod_nsec              : 32;
    } Bits;
    UINT32 Raw;
} REG_e_new_tod_p2p_tod_offset_nsec, *PREG_e_new_tod_p2p_tod_offset_nsec;

typedef PACKING union
{
    PACKING struct
    {
        FIELD tod_p2p_sec_l32           : 32;
    } Bits;
    UINT32 Raw;
} REG_e_tod_p2p_tod_sec_l32, *PREG_e_tod_p2p_tod_sec_l32;

typedef PACKING union
{
    PACKING struct
    {
        FIELD tod_p2p_nsec              : 32;
    } Bits;
    UINT32 Raw;
} REG_e_tod_p2p_tod_nsec, *PREG_e_tod_p2p_tod_nsec;

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
} REG_e_tod_period, *PREG_e_tod_period;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_30                    : 2;
        FIELD pwd_mode                  : 2;
        FIELD rsv_27                    : 1;
        FIELD timedrift_ignore          : 1;
        FIELD onu_wakeup                : 1;
        FIELD pwr_sv_start              : 1;
        FIELD rsv_23                    : 1;
        FIELD pwd_mode_i                : 3;
        FIELD rsv_16                    : 4;
        FIELD slp_duration_max_h        : 16;
#else
        FIELD slp_duration_max_h        : 16;
        FIELD rsv_16                    : 4;
        FIELD pwd_mode_i                : 3;
        FIELD rsv_23                    : 1;
        FIELD pwr_sv_start              : 1;
        FIELD onu_wakeup                : 1;
        FIELD timedrift_ignore          : 1;
        FIELD rsv_27                    : 1;
        FIELD pwd_mode                  : 2;
        FIELD rsv_30                    : 2;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_pwr_sv_cfg, *PREG_e_pwr_sv_cfg;

typedef PACKING union
{
    PACKING struct
    {
        FIELD slp_duration_max_l        : 32;
    } Bits;
    UINT32 Raw;
} REG_e_slp_durt_max, *PREG_e_slp_durt_max;

typedef PACKING union
{
    PACKING struct
    {
        FIELD slp_duration              : 32;
    } Bits;
    UINT32 Raw;
} REG_e_slp_duration, *PREG_e_slp_duration;

typedef PACKING union
{
    PACKING struct
    {
        FIELD act_duration              : 32;
    } Bits;
    UINT32 Raw;
} REG_e_act_duration, *PREG_e_act_duration;

typedef PACKING union
{
    PACKING struct
    {
        FIELD pwron_dly                 : 32;
    } Bits;
    UINT32 Raw;
} REG_e_pwron_dly, *PREG_e_pwron_dly;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_16                    : 16;
        FIELD slp_duration_i            : 16;
#else
        FIELD slp_duration_i            : 16;
        FIELD rsv_16                    : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_slp_duration_i, *PREG_e_slp_duration_i;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD tx_fetch_overhead         : 8;
        FIELD tx_fetch_leadtime         : 8;
        FIELD rsv_12                    : 4;
        FIELD tx_dma_leadtime           : 12;
#else
        FIELD tx_dma_leadtime           : 12;
        FIELD rsv_12                    : 4;
        FIELD tx_fetch_leadtime         : 8;
        FIELD tx_fetch_overhead         : 8;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_txfetch_cfg, *PREG_e_txfetch_cfg;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_17                    : 15;
        FIELD sync_time_updte           : 1;
        FIELD sync_time                 : 16;
#else
        FIELD sync_time                 : 16;
        FIELD sync_time_updte           : 1;
        FIELD rsv_17                    : 15;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_sync_time, *PREG_e_sync_time;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD dscvr_gnt_len             : 8;
        FIELD fec_tail_grd              : 8;
        FIELD tail_grd                  : 8;
        FIELD rsv_6                     : 2;
        FIELD default_ovrhd             : 6;
#else
        FIELD default_ovrhd             : 6;
        FIELD rsv_6                     : 2;
        FIELD tail_grd                  : 8;
        FIELD fec_tail_grd              : 8;
        FIELD dscvr_gnt_len             : 8;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_tx_cal_cnst, *PREG_e_tx_cal_cnst;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_16                    : 16;
        FIELD laser_off_time            : 8;
        FIELD laser_on_time             : 8;
#else
        FIELD laser_on_time             : 8;
        FIELD laser_off_time            : 8;
        FIELD rsv_16                    : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_laser_onoff_time, *PREG_e_laser_onoff_time;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_8                     : 24;
        FIELD guard_thrshld             : 8;
#else
        FIELD guard_thrshld             : 8;
        FIELD rsv_8                     : 24;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_grd_thrshld, *PREG_e_grd_thrshld;

typedef PACKING union
{
    PACKING struct
    {
        FIELD mpcp_timeout_intvl        : 32;
    } Bits;
    UINT32 Raw;
} REG_e_mpcp_timeout_intvl, *PREG_e_mpcp_timeout_intvl;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_24                    : 8;
        FIELD rpt_timeout_intvl         : 24;
#else
        FIELD rpt_timeout_intvl         : 24;
        FIELD rsv_24                    : 8;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_rpt_timeout_intvl, *PREG_e_rpt_timeout_intvl;

typedef PACKING union
{
    PACKING struct
    {
        FIELD max_future_gnt_time       : 32;
    } Bits;
    UINT32 Raw;
} REG_e_max_future_gnt_time, *PREG_e_max_future_gnt_time;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_16                    : 16;
        FIELD min_proc_time             : 16;
#else
        FIELD min_proc_time             : 16;
        FIELD rsv_16                    : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_min_proc_time, *PREG_e_min_proc_time;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD tx_tmstp_adj              : 16;
        FIELD tx_stm_adj                : 16;
#else
        FIELD tx_stm_adj                : 16;
        FIELD tx_tmstp_adj              : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_trx_adjust_time1, *PREG_e_trx_adjust_time1;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD tx_len_adj                : 16;
        FIELD rx_tmstp_adj              : 16;
#else
        FIELD rx_tmstp_adj              : 16;
        FIELD tx_len_adj                : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_trx_adjust_time2, *PREG_e_trx_adjust_time2;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_12                    : 20;
        FIELD probe_bit0_sel            : 4;
        FIELD rsv_5                     : 3;
        FIELD probe_sel                 : 5;
#else
        FIELD probe_sel                 : 5;
        FIELD rsv_5                     : 3;
        FIELD probe_bit0_sel            : 4;
        FIELD rsv_12                    : 20;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_dbg_prb_sel, *PREG_e_dbg_prb_sel;

typedef PACKING union
{
    PACKING struct
    {
        FIELD probe_h32                 : 32;
    } Bits;
    UINT32 Raw;
} REG_e_dbg_prb_h32, *PREG_e_dbg_prb_h32;

typedef PACKING union
{
    PACKING struct
    {
        FIELD probe_l32                 : 32;
    } Bits;
    UINT32 Raw;
} REG_e_dbg_prb_l32, *PREG_e_dbg_prb_l32;

typedef PACKING union
{
    PACKING struct
    {
        FIELD rxmbi_eth_cnt             : 32;
    } Bits;
    UINT32 Raw;
} REG_e_rxmbi_eth_cnt, *PREG_e_rxmbi_eth_cnt;

typedef PACKING union
{
    PACKING struct
    {
        FIELD rxmpi_eth_cnt             : 32;
    } Bits;
    UINT32 Raw;
} REG_e_rxmpi_eth_cnt, *PREG_e_rxmpi_eth_cnt;

typedef PACKING union
{
    PACKING struct
    {
        FIELD txmbi_eth_cnt             : 32;
    } Bits;
    UINT32 Raw;
} REG_e_txmbi_eth_cnt, *PREG_e_txmbi_eth_cnt;

typedef PACKING union
{
    PACKING struct
    {
        FIELD txmpi_eth_cnt             : 32;
    } Bits;
    UINT32 Raw;
} REG_e_txmpi_eth_cnt, *PREG_e_txmpi_eth_cnt;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rx_oam_cnt                : 16;
        FIELD tx_oam_cnt                : 16;
#else
        FIELD tx_oam_cnt                : 16;
        FIELD rx_oam_cnt                : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_oam_stat, *PREG_e_oam_stat;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD mpcp_err_cnt              : 8;
        FIELD mpcp_rgst_cnt             : 8;
        FIELD mpcp_dscv_gate_cnt        : 16;
#else
        FIELD mpcp_dscv_gate_cnt        : 16;
        FIELD mpcp_rgst_cnt             : 8;
        FIELD mpcp_err_cnt              : 8;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_mpcp_stat, *PREG_e_mpcp_stat;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_16                    : 16;
        FIELD mpcp_rgst_req_cnt         : 8;
        FIELD mpcp_rgst_ack_cnt         : 8;
#else
        FIELD mpcp_rgst_ack_cnt         : 8;
        FIELD mpcp_rgst_req_cnt         : 8;
        FIELD rsv_16                    : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_mpcp_rgst_stat, *PREG_e_mpcp_rgst_stat;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_23                    : 9;
        FIELD max_gnt_pending_cnt       : 7;
        FIELD rsv_7                     : 9;
        FIELD cur_gnt_pending_cnt       : 7;
#else
        FIELD cur_gnt_pending_cnt       : 7;
        FIELD rsv_7                     : 9;
        FIELD max_gnt_pending_cnt       : 7;
        FIELD rsv_23                    : 9;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_gnt_pending_stat, *PREG_e_gnt_pending_stat;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD max_gnt_length            : 16;
        FIELD min_gnt_length            : 16;
#else
        FIELD min_gnt_length            : 16;
        FIELD max_gnt_length            : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_gnt_length_stat, *PREG_e_gnt_length_stat;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD b2b_gnt_cnt               : 16;
        FIELD hdn_gnt_cnt               : 16;
#else
        FIELD hdn_gnt_cnt               : 16;
        FIELD b2b_gnt_cnt               : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_gnt_type_stat, *PREG_e_gnt_type_stat;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_16                    : 16;
        FIELD cur_time_drift            : 8;
        FIELD max_time_drift            : 8;
#else
        FIELD max_time_drift            : 8;
        FIELD cur_time_drift            : 8;
        FIELD rsv_16                    : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_time_drft_stat, *PREG_e_time_drft_stat;

typedef PACKING union
{
    PACKING struct
    {
        FIELD llid0_gnt_cnt             : 32;
    } Bits;
    UINT32 Raw;
} REG_e_llid0_gnt_stat, *PREG_e_llid0_gnt_stat;

typedef PACKING union
{
    PACKING struct
    {
        FIELD llid1_gnt_cnt             : 32;
    } Bits;
    UINT32 Raw;
} REG_e_llid1_gnt_stat, *PREG_e_llid1_gnt_stat;

typedef PACKING union
{
    PACKING struct
    {
        FIELD llid2_gnt_cnt             : 32;
    } Bits;
    UINT32 Raw;
} REG_e_llid2_gnt_stat, *PREG_e_llid2_gnt_stat;

typedef PACKING union
{
    PACKING struct
    {
        FIELD llid3_gnt_cnt             : 32;
    } Bits;
    UINT32 Raw;
} REG_e_llid3_gnt_stat, *PREG_e_llid3_gnt_stat;

typedef PACKING union
{
    PACKING struct
    {
        FIELD llid4_gnt_cnt             : 32;
    } Bits;
    UINT32 Raw;
} REG_e_llid4_gnt_stat, *PREG_e_llid4_gnt_stat;

typedef PACKING union
{
    PACKING struct
    {
        FIELD llid5_gnt_cnt             : 32;
    } Bits;
    UINT32 Raw;
} REG_e_llid5_gnt_stat, *PREG_e_llid5_gnt_stat;

typedef PACKING union
{
    PACKING struct
    {
        FIELD llid6_gnt_cnt             : 32;
    } Bits;
    UINT32 Raw;
} REG_e_llid6_gnt_stat, *PREG_e_llid6_gnt_stat;

typedef PACKING union
{
    PACKING struct
    {
        FIELD llid7_gnt_cnt             : 32;
    } Bits;
    UINT32 Raw;
} REG_e_llid7_gnt_stat, *PREG_e_llid7_gnt_stat;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD snf_mpcp_cap_en           : 1;
        FIELD snf_oam_cap_en            : 1;
        FIELD snf_mpcp_oam_cnt_set      : 30;
#else
        FIELD snf_mpcp_oam_cnt_set      : 30;
        FIELD snf_oam_cap_en            : 1;
        FIELD snf_mpcp_cap_en           : 1;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_snf_mpcp_oam_ctl, *PREG_e_snf_mpcp_oam_ctl;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_25                    : 7;
        FIELD rpt_bitmap_ctrl           : 1;
        FIELD rpt_bitmap_set            : 8;
        FIELD rpt_qsize_fecoff_adj      : 16;
#else
        FIELD rpt_qsize_fecoff_adj      : 16;
        FIELD rpt_bitmap_set            : 8;
        FIELD rpt_bitmap_ctrl           : 1;
        FIELD rsv_25                    : 7;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_rpt_adj, *PREG_e_rpt_adj;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_16                    : 16;
        FIELD rpt_cnt                   : 16;
#else
        FIELD rpt_cnt                   : 16;
        FIELD rsv_16                    : 16;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_rpt_cnt, *PREG_e_rpt_cnt;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dyinggsp_w1_len_type      : 32;
    } Bits;
    UINT32 Raw;
} REG_e_dyinggsp_w1, *PREG_e_dyinggsp_w1;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dyinggsp_w2               : 32;
    } Bits;
    UINT32 Raw;
} REG_e_dyinggsp_w2, *PREG_e_dyinggsp_w2;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dyinggsp_w3               : 32;
    } Bits;
    UINT32 Raw;
} REG_e_dyinggsp_w3, *PREG_e_dyinggsp_w3;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dyinggsp_w4               : 32;
    } Bits;
    UINT32 Raw;
} REG_e_dyinggsp_w4, *PREG_e_dyinggsp_w4;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dyinggsp_w5               : 32;
    } Bits;
    UINT32 Raw;
} REG_e_dyinggsp_w5, *PREG_e_dyinggsp_w5;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dyinggsp_w6               : 32;
    } Bits;
    UINT32 Raw;
} REG_e_dyinggsp_w6, *PREG_e_dyinggsp_w6;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dyinggsp_w7               : 32;
    } Bits;
    UINT32 Raw;
} REG_e_dyinggsp_w7, *PREG_e_dyinggsp_w7;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dyinggsp_w8               : 32;
    } Bits;
    UINT32 Raw;
} REG_e_dyinggsp_w8, *PREG_e_dyinggsp_w8;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dyinggsp_w9               : 32;
    } Bits;
    UINT32 Raw;
} REG_e_dyinggsp_w9, *PREG_e_dyinggsp_w9;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dyinggsp_w10              : 32;
    } Bits;
    UINT32 Raw;
} REG_e_dyinggsp_w10, *PREG_e_dyinggsp_w10;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dyinggsp_w11              : 32;
    } Bits;
    UINT32 Raw;
} REG_e_dyinggsp_w11, *PREG_e_dyinggsp_w11;

typedef PACKING union
{
    PACKING struct
    {
        FIELD dyinggsp_w12              : 32;
    } Bits;
    UINT32 Raw;
} REG_e_dyinggsp_w12, *PREG_e_dyinggsp_w12;

typedef PACKING union
{
    PACKING struct
    {
        FIELD oam_kpalv_w1              : 32;
    } Bits;
    UINT32 Raw;
} REG_e_oam_kpalv_w1, *PREG_e_oam_kpalv_w1;

typedef PACKING union
{
    PACKING struct
    {
        FIELD oam_kpalv_w2              : 32;
    } Bits;
    UINT32 Raw;
} REG_e_oam_kpalv_w2, *PREG_e_oam_kpalv_w2;

typedef PACKING union
{
    PACKING struct
    {
        FIELD oam_kpalv_w3              : 32;
    } Bits;
    UINT32 Raw;
} REG_e_oam_kpalv_w3, *PREG_e_oam_kpalv_w3;

typedef PACKING union
{
    PACKING struct
    {
        FIELD oam_kpalv_w4              : 32;
    } Bits;
    UINT32 Raw;
} REG_e_oam_kpalv_w4, *PREG_e_oam_kpalv_w4;

typedef PACKING union
{
    PACKING struct
    {
        FIELD oam_kpalv_w5              : 32;
    } Bits;
    UINT32 Raw;
} REG_e_oam_kpalv_w5, *PREG_e_oam_kpalv_w5;

typedef PACKING union
{
    PACKING struct
    {
        FIELD oam_kpalv_w6              : 32;
    } Bits;
    UINT32 Raw;
} REG_e_oam_kpalv_w6, *PREG_e_oam_kpalv_w6;

typedef PACKING union
{
    PACKING struct
    {
        FIELD oam_kpalv_w7              : 32;
    } Bits;
    UINT32 Raw;
} REG_e_oam_kpalv_w7, *PREG_e_oam_kpalv_w7;

typedef PACKING union
{
    PACKING struct
    {
        FIELD oam_kpalv_w8              : 32;
    } Bits;
    UINT32 Raw;
} REG_e_oam_kpalv_w8, *PREG_e_oam_kpalv_w8;

typedef PACKING union
{
    PACKING struct
    {
        FIELD oam_kpalv_w9              : 32;
    } Bits;
    UINT32 Raw;
} REG_e_oam_kpalv_w9, *PREG_e_oam_kpalv_w9;

typedef PACKING union
{
    PACKING struct
    {
        FIELD oam_kpalv_w10             : 32;
    } Bits;
    UINT32 Raw;
} REG_e_oam_kpalv_w10, *PREG_e_oam_kpalv_w10;

typedef PACKING union
{
    PACKING struct
    {
        FIELD oam_kpalv_w11             : 32;
    } Bits;
    UINT32 Raw;
} REG_e_oam_kpalv_w11, *PREG_e_oam_kpalv_w11;

typedef PACKING union
{
    PACKING struct
    {
        FIELD oam_kpalv_w12             : 32;
    } Bits;
    UINT32 Raw;
} REG_e_oam_kpalv_w12, *PREG_e_oam_kpalv_w12;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD oam_kpalv_interval        : 24;
        FIELD rsv_3                     : 5;
        FIELD oam_kpalv_sw_trig         : 1;
        FIELD oam_kpalv_sw_cfg          : 1;
        FIELD oam_kpalv_en              : 1;
#else
        FIELD oam_kpalv_en              : 1;
        FIELD oam_kpalv_sw_cfg          : 1;
        FIELD oam_kpalv_sw_trig         : 1;
        FIELD rsv_3                     : 5;
        FIELD oam_kpalv_interval        : 24;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_oam_kpalv_ctrl, *PREG_e_oam_kpalv_ctrl;

typedef PACKING union
{
    PACKING struct
    {
        FIELD tod_1pps_width_ctrl       : 32;
    } Bits;
    UINT32 Raw;
} REG_e_tod_1pps_ctrl, *PREG_e_tod_1pps_ctrl;

typedef PACKING union
{
    PACKING struct
    {
#ifdef __BIG_ENDIAN
        FIELD rsv_8                     : 24;
        FIELD short_pkt_size_ctrl       : 8;
#else
        FIELD short_pkt_size_ctrl       : 8;
        FIELD rsv_8                     : 24;
#endif /* __BIG_ENDIAN */
    } Bits;
    UINT32 Raw;
} REG_e_short_pkt_ctrl, *PREG_e_short_pkt_ctrl;

typedef PACKING union
{
    PACKING struct
    {
        FIELD sniffer_sp_tag            : 32;
    } Bits;
    UINT32 Raw;
} REG_e_sniff_sp_tag, *PREG_e_sniff_sp_tag;

// ----------------- EPON_MAC  Grouping Definitions -------------------
// ----------------- EPON_MAC Register Definition -------------------
typedef volatile PACKING struct
{
    UINT32                          rsv_0000[6144];   // 0000..5FFC
    REG_e_glb_cfg                   e_glb_cfg;        // 6000
    REG_e_int_status                e_int_status;     // 6004
    REG_e_int_en                    e_int_en;         // 6008
    REG_e_rpt_mpcp_timeout_llid_idx e_rpt_mpcp_timeout_llid_idx; // 600C
    REG_e_dyinggsp_cfg              e_dyinggsp_cfg;   // 6010
    REG_e_pending_gnt_num           e_pending_gnt_num; // 6014
    UINT32                          rsv_6018[2];      // 6018..601C
    REG_e_llid0_3_cfg               e_llid0_3_cfg;    // 6020
    REG_e_llid4_7_cfg               e_llid4_7_cfg;    // 6024
    REG_e_llid_dscvry_ctrl          e_llid_dscvry_ctrl; // 6028
    REG_e_llid0_dscvry_sts          e_llid0_dscvry_sts; // 602C
    REG_e_llid1_dscvry_sts          e_llid1_dscvry_sts; // 6030
    REG_e_llid2_dscvry_sts          e_llid2_dscvry_sts; // 6034
    REG_e_llid3_dscvry_sts          e_llid3_dscvry_sts; // 6038
    REG_e_llid4_dscvry_sts          e_llid4_dscvry_sts; // 603C
    REG_e_llid5_dscvry_sts          e_llid5_dscvry_sts; // 6040
    REG_e_llid6_dscvry_sts          e_llid6_dscvry_sts; // 6044
    REG_e_llid7_dscvry_sts          e_llid7_dscvry_sts; // 6048
    UINT32                          rsv_604C;         // 604C
    REG_e_mac_addr_cfg              e_mac_addr_cfg;   // 6050
    REG_e_mac_addr_value            e_mac_addr_value; // 6054
    REG_e_security_key_cfg          e_security_key_cfg; // 6058
    REG_e_key_value                 e_key_value;      // 605C
    REG_e_rpt_data                  e_rpt_data;       // 6060
    REG_e_rpt_len                   e_rpt_len;        // 6064
    REG_e_rpt_cfg                   e_rpt_cfg;        // 6068
    REG_e_rpt_qthld_cfg             e_rpt_qthld_cfg;  // 606C
    UINT32                          rsv_6070[4];      // 6070..607C
    REG_e_local_time                e_local_time;     // 6080
    REG_e_tod_sync_x                e_tod_sync_x;     // 6084
    REG_e_tod_ltncy                 e_tod_ltncy;      // 6088
    REG_p2p_tx_tag1                 p2p_tx_tag1;      // 608C
    REG_p2p_tx_tag2                 p2p_tx_tag2;      // 6090
    REG_e_new_tod_p2p_offset_sec_l32 e_new_tod_p2p_offset_sec_l32; // 6094
    REG_e_new_tod_p2p_tod_offset_nsec e_new_tod_p2p_tod_offset_nsec; // 6098
    REG_e_tod_p2p_tod_sec_l32       e_tod_p2p_tod_sec_l32; // 609C
    REG_e_tod_p2p_tod_nsec          e_tod_p2p_tod_nsec; // 60A0
    REG_e_tod_period                e_tod_period;     // 60A4
    UINT32                          rsv_60A8[2];      // 60A8..60AC
    REG_e_pwr_sv_cfg                e_pwr_sv_cfg;     // 60B0
    REG_e_slp_durt_max              e_slp_durt_max;   // 60B4
    REG_e_slp_duration              e_slp_duration;   // 60B8
    REG_e_act_duration              e_act_duration;   // 60BC
    REG_e_pwron_dly                 e_pwron_dly;      // 60C0
    REG_e_slp_duration_i            e_slp_duration_i; // 60C4
    UINT32                          rsv_60C8[2];      // 60C8..60CC
    REG_e_txfetch_cfg               e_txfetch_cfg;    // 60D0
    REG_e_sync_time                 e_sync_time;      // 60D4
    REG_e_tx_cal_cnst               e_tx_cal_cnst;    // 60D8
    REG_e_laser_onoff_time          e_laser_onoff_time; // 60DC
    REG_e_grd_thrshld               e_grd_thrshld;    // 60E0
    REG_e_mpcp_timeout_intvl        e_mpcp_timeout_intvl; // 60E4
    REG_e_rpt_timeout_intvl         e_rpt_timeout_intvl; // 60E8
    REG_e_max_future_gnt_time       e_max_future_gnt_time; // 60EC
    REG_e_min_proc_time             e_min_proc_time;  // 60F0
    REG_e_trx_adjust_time1          e_trx_adjust_time1; // 60F4
    REG_e_trx_adjust_time2          e_trx_adjust_time2; // 60F8
    UINT32                          rsv_60FC;         // 60FC
    REG_e_dbg_prb_sel               e_dbg_prb_sel;    // 6100
    REG_e_dbg_prb_h32               e_dbg_prb_h32;    // 6104
    REG_e_dbg_prb_l32               e_dbg_prb_l32;    // 6108
    REG_e_rxmbi_eth_cnt             e_rxmbi_eth_cnt;  // 610C
    REG_e_rxmpi_eth_cnt             e_rxmpi_eth_cnt;  // 6110
    REG_e_txmbi_eth_cnt             e_txmbi_eth_cnt;  // 6114
    REG_e_txmpi_eth_cnt             e_txmpi_eth_cnt;  // 6118
    REG_e_oam_stat                  e_oam_stat;       // 611C
    REG_e_mpcp_stat                 e_mpcp_stat;      // 6120
    REG_e_mpcp_rgst_stat            e_mpcp_rgst_stat; // 6124
    REG_e_gnt_pending_stat          e_gnt_pending_stat; // 6128
    REG_e_gnt_length_stat           e_gnt_length_stat; // 612C
    REG_e_gnt_type_stat             e_gnt_type_stat;  // 6130
    REG_e_time_drft_stat            e_time_drft_stat; // 6134
    REG_e_llid0_gnt_stat            e_llid0_gnt_stat; // 6138
    REG_e_llid1_gnt_stat            e_llid1_gnt_stat; // 613C
    REG_e_llid2_gnt_stat            e_llid2_gnt_stat; // 6140
    REG_e_llid3_gnt_stat            e_llid3_gnt_stat; // 6144
    REG_e_llid4_gnt_stat            e_llid4_gnt_stat; // 6148
    REG_e_llid5_gnt_stat            e_llid5_gnt_stat; // 614C
    REG_e_llid6_gnt_stat            e_llid6_gnt_stat; // 6150
    REG_e_llid7_gnt_stat            e_llid7_gnt_stat; // 6154
    UINT32                          rsv_6158[2];      // 6158..615C
    REG_e_snf_mpcp_oam_ctl          e_snf_mpcp_oam_ctl; // 6160
    REG_e_rpt_adj                   e_rpt_adj;        // 6164
    REG_e_rpt_cnt                   e_rpt_cnt;        // 6168
    REG_e_dyinggsp_w1               e_dyinggsp_w1;    // 616C
    REG_e_dyinggsp_w2               e_dyinggsp_w2;    // 6170
    REG_e_dyinggsp_w3               e_dyinggsp_w3;    // 6174
    REG_e_dyinggsp_w4               e_dyinggsp_w4;    // 6178
    REG_e_dyinggsp_w5               e_dyinggsp_w5;    // 617C
    REG_e_dyinggsp_w6               e_dyinggsp_w6;    // 6180
    REG_e_dyinggsp_w7               e_dyinggsp_w7;    // 6184
    REG_e_dyinggsp_w8               e_dyinggsp_w8;    // 6188
    REG_e_dyinggsp_w9               e_dyinggsp_w9;    // 618C
    REG_e_dyinggsp_w10              e_dyinggsp_w10;   // 6190
    REG_e_dyinggsp_w11              e_dyinggsp_w11;   // 6194
    REG_e_dyinggsp_w12              e_dyinggsp_w12;   // 6198
    REG_e_oam_kpalv_w1              e_oam_kpalv_w1;   // 619C
    REG_e_oam_kpalv_w2              e_oam_kpalv_w2;   // 61A0
    REG_e_oam_kpalv_w3              e_oam_kpalv_w3;   // 61A4
    REG_e_oam_kpalv_w4              e_oam_kpalv_w4;   // 61A8
    REG_e_oam_kpalv_w5              e_oam_kpalv_w5;   // 61AC
    REG_e_oam_kpalv_w6              e_oam_kpalv_w6;   // 61B0
    REG_e_oam_kpalv_w7              e_oam_kpalv_w7;   // 61B4
    REG_e_oam_kpalv_w8              e_oam_kpalv_w8;   // 61B8
    REG_e_oam_kpalv_w9              e_oam_kpalv_w9;   // 61BC
    REG_e_oam_kpalv_w10             e_oam_kpalv_w10;  // 61C0
    REG_e_oam_kpalv_w11             e_oam_kpalv_w11;  // 61C4
    REG_e_oam_kpalv_w12             e_oam_kpalv_w12;  // 61C8
    REG_e_oam_kpalv_ctrl            e_oam_kpalv_ctrl; // 61CC
    REG_e_tod_1pps_ctrl             e_tod_1pps_ctrl;  // 61D0
    REG_e_short_pkt_ctrl            e_short_pkt_ctrl; // 61D4
    REG_e_sniff_sp_tag              e_sniff_sp_tag;   // 61D8
    UINT32                          rsv_61DC[8072];   // 61DC..DFF8
    UINT8                           rsv_DFFC;         // DFFC
    UINT16                          rsv_DFFD;         // DFFD
    UINT8                           rsv_E0FF;         // E0FF
    UINT32                          rsv_E100[959];    // E100..EFF8
    UINT8                           rsv_EFFC;         // EFFC
    UINT16                          rsv_EFFD;         // EFFD
}EPON_MAC_REGS, *PEPON_MAC_REGS;	

// ---------- EPON_MAC Enum Definitions      ----------
// ---------- EPON_MAC C Macro Definitions   ----------
extern PEPON_MAC_REGS g_EPON_MAC_BASE;

#define EPON_MAC_BASE                                          (g_EPON_MAC_BASE)

#define e_glb_cfg                                              INREG32(&EPON_MAC_BASE->e_glb_cfg) // 6000
#define e_int_status                                           INREG32(&EPON_MAC_BASE->e_int_status) // 6004
#define e_int_en                                               INREG32(&EPON_MAC_BASE->e_int_en) // 6008
#define e_rpt_mpcp_timeout_llid_idx                            INREG32(&EPON_MAC_BASE->e_rpt_mpcp_timeout_llid_idx) // 600C
#define e_dyinggsp_cfg                                         INREG32(&EPON_MAC_BASE->e_dyinggsp_cfg) // 6010
#define e_pending_gnt_num                                      INREG32(&EPON_MAC_BASE->e_pending_gnt_num) // 6014
#define e_llid0_3_cfg                                          INREG32(&EPON_MAC_BASE->e_llid0_3_cfg) // 6020
#define e_llid4_7_cfg                                          INREG32(&EPON_MAC_BASE->e_llid4_7_cfg) // 6024
#define e_llid_dscvry_ctrl                                     INREG32(&EPON_MAC_BASE->e_llid_dscvry_ctrl) // 6028
#define e_llid0_dscvry_sts                                     INREG32(&EPON_MAC_BASE->e_llid0_dscvry_sts) // 602C
#define e_llid1_dscvry_sts                                     INREG32(&EPON_MAC_BASE->e_llid1_dscvry_sts) // 6030
#define e_llid2_dscvry_sts                                     INREG32(&EPON_MAC_BASE->e_llid2_dscvry_sts) // 6034
#define e_llid3_dscvry_sts                                     INREG32(&EPON_MAC_BASE->e_llid3_dscvry_sts) // 6038
#define e_llid4_dscvry_sts                                     INREG32(&EPON_MAC_BASE->e_llid4_dscvry_sts) // 603C
#define e_llid5_dscvry_sts                                     INREG32(&EPON_MAC_BASE->e_llid5_dscvry_sts) // 6040
#define e_llid6_dscvry_sts                                     INREG32(&EPON_MAC_BASE->e_llid6_dscvry_sts) // 6044
#define e_llid7_dscvry_sts                                     INREG32(&EPON_MAC_BASE->e_llid7_dscvry_sts) // 6048
#define e_mac_addr_cfg                                         INREG32(&EPON_MAC_BASE->e_mac_addr_cfg) // 6050
#define e_mac_addr_value                                       INREG32(&EPON_MAC_BASE->e_mac_addr_value) // 6054
#define e_security_key_cfg                                     INREG32(&EPON_MAC_BASE->e_security_key_cfg) // 6058
#define e_key_value                                            INREG32(&EPON_MAC_BASE->e_key_value) // 605C
#define e_rpt_data                                             INREG32(&EPON_MAC_BASE->e_rpt_data) // 6060
#define e_rpt_len                                              INREG32(&EPON_MAC_BASE->e_rpt_len) // 6064
#define e_rpt_cfg                                              INREG32(&EPON_MAC_BASE->e_rpt_cfg) // 6068
#define e_rpt_qthld_cfg                                     INREG32(&EPON_MAC_BASE->e_rpt_qthld_cfg) // 606C
#define e_local_time                                           INREG32(&EPON_MAC_BASE->e_local_time) // 6080
#define e_tod_sync_x                                           INREG32(&EPON_MAC_BASE->e_tod_sync_x) // 6084
#define e_tod_ltncy                                            INREG32(&EPON_MAC_BASE->e_tod_ltncy) // 6088
#define p2p_tx_tag1                                            INREG32(&EPON_MAC_BASE->p2p_tx_tag1) // 608C
#define p2p_tx_tag2                                            INREG32(&EPON_MAC_BASE->p2p_tx_tag2) // 6090
#define e_new_tod_p2p_offset_sec_l32                           INREG32(&EPON_MAC_BASE->e_new_tod_p2p_offset_sec_l32) // 6094
#define e_new_tod_p2p_tod_offset_nsec                          INREG32(&EPON_MAC_BASE->e_new_tod_p2p_tod_offset_nsec) // 6098
#define e_tod_p2p_tod_sec_l32                                  INREG32(&EPON_MAC_BASE->e_tod_p2p_tod_sec_l32) // 609C
#define e_tod_p2p_tod_nsec                                     INREG32(&EPON_MAC_BASE->e_tod_p2p_tod_nsec) // 60A0
#define e_tod_period                                           INREG32(&EPON_MAC_BASE->e_tod_period) // 60A4
#define e_pwr_sv_cfg                                           INREG32(&EPON_MAC_BASE->e_pwr_sv_cfg) // 60B0
#define e_slp_durt_max                                         INREG32(&EPON_MAC_BASE->e_slp_durt_max) // 60B4
#define e_slp_duration                                         INREG32(&EPON_MAC_BASE->e_slp_duration) // 60B8
#define e_act_duration                                         INREG32(&EPON_MAC_BASE->e_act_duration) // 60BC
#define e_pwron_dly                                            INREG32(&EPON_MAC_BASE->e_pwron_dly) // 60C0
#define e_slp_duration_i                                       INREG32(&EPON_MAC_BASE->e_slp_duration_i) // 60C4
#define e_txfetch_cfg                                          INREG32(&EPON_MAC_BASE->e_txfetch_cfg) // 60D0
#define e_sync_time                                            INREG32(&EPON_MAC_BASE->e_sync_time) // 60D4
#define e_tx_cal_cnst                                          INREG32(&EPON_MAC_BASE->e_tx_cal_cnst) // 60D8
#define e_laser_onoff_time                                     INREG32(&EPON_MAC_BASE->e_laser_onoff_time) // 60DC
#define e_grd_thrshld                                          INREG32(&EPON_MAC_BASE->e_grd_thrshld) // 60E0
#define e_mpcp_timeout_intvl                                   INREG32(&EPON_MAC_BASE->e_mpcp_timeout_intvl) // 60E4
#define e_rpt_timeout_intvl                                    INREG32(&EPON_MAC_BASE->e_rpt_timeout_intvl) // 60E8
#define e_max_future_gnt_time                                  INREG32(&EPON_MAC_BASE->e_max_future_gnt_time) // 60EC
#define e_min_proc_time                                        INREG32(&EPON_MAC_BASE->e_min_proc_time) // 60F0
#define e_trx_adjust_time1                                     INREG32(&EPON_MAC_BASE->e_trx_adjust_time1) // 60F4
#define e_trx_adjust_time2                                     INREG32(&EPON_MAC_BASE->e_trx_adjust_time2) // 60F8
#define e_dbg_prb_sel                                          INREG32(&EPON_MAC_BASE->e_dbg_prb_sel) // 6100
#define e_dbg_prb_h32                                          INREG32(&EPON_MAC_BASE->e_dbg_prb_h32) // 6104
#define e_dbg_prb_l32                                          INREG32(&EPON_MAC_BASE->e_dbg_prb_l32) // 6108
#define e_rxmbi_eth_cnt                                        INREG32(&EPON_MAC_BASE->e_rxmbi_eth_cnt) // 610C
#define e_rxmpi_eth_cnt                                        INREG32(&EPON_MAC_BASE->e_rxmpi_eth_cnt) // 6110
#define e_txmbi_eth_cnt                                        INREG32(&EPON_MAC_BASE->e_txmbi_eth_cnt) // 6114
#define e_txmpi_eth_cnt                                        INREG32(&EPON_MAC_BASE->e_txmpi_eth_cnt) // 6118
#define e_oam_stat                                             INREG32(&EPON_MAC_BASE->e_oam_stat) // 611C
#define e_mpcp_stat                                            INREG32(&EPON_MAC_BASE->e_mpcp_stat) // 6120
#define e_mpcp_rgst_stat                                       INREG32(&EPON_MAC_BASE->e_mpcp_rgst_stat) // 6124
#define e_gnt_pending_stat                                     INREG32(&EPON_MAC_BASE->e_gnt_pending_stat) // 6128
#define e_gnt_length_stat                                      INREG32(&EPON_MAC_BASE->e_gnt_length_stat) // 612C
#define e_gnt_type_stat                                        INREG32(&EPON_MAC_BASE->e_gnt_type_stat) // 6130
#define e_time_drft_stat                                       INREG32(&EPON_MAC_BASE->e_time_drft_stat) // 6134
#define e_llid0_gnt_stat                                       INREG32(&EPON_MAC_BASE->e_llid0_gnt_stat) // 6138
#define e_llid1_gnt_stat                                       INREG32(&EPON_MAC_BASE->e_llid1_gnt_stat) // 613C
#define e_llid2_gnt_stat                                       INREG32(&EPON_MAC_BASE->e_llid2_gnt_stat) // 6140
#define e_llid3_gnt_stat                                       INREG32(&EPON_MAC_BASE->e_llid3_gnt_stat) // 6144
#define e_llid4_gnt_stat                                       INREG32(&EPON_MAC_BASE->e_llid4_gnt_stat) // 6148
#define e_llid5_gnt_stat                                       INREG32(&EPON_MAC_BASE->e_llid5_gnt_stat) // 614C
#define e_llid6_gnt_stat                                       INREG32(&EPON_MAC_BASE->e_llid6_gnt_stat) // 6150
#define e_llid7_gnt_stat                                       INREG32(&EPON_MAC_BASE->e_llid7_gnt_stat) // 6154
#define e_snf_mpcp_oam_ctl                                     INREG32(&EPON_MAC_BASE->e_snf_mpcp_oam_ctl) // 6160
#define e_rpt_adj                                              INREG32(&EPON_MAC_BASE->e_rpt_adj) // 6164
#define e_rpt_cnt                                              INREG32(&EPON_MAC_BASE->e_rpt_cnt) // 6168
#define e_dyinggsp_w1                                          INREG32(&EPON_MAC_BASE->e_dyinggsp_w1) // 616C
#define e_dyinggsp_w2                                          INREG32(&EPON_MAC_BASE->e_dyinggsp_w2) // 6170
#define e_dyinggsp_w3                                          INREG32(&EPON_MAC_BASE->e_dyinggsp_w3) // 6174
#define e_dyinggsp_w4                                          INREG32(&EPON_MAC_BASE->e_dyinggsp_w4) // 6178
#define e_dyinggsp_w5                                          INREG32(&EPON_MAC_BASE->e_dyinggsp_w5) // 617C
#define e_dyinggsp_w6                                          INREG32(&EPON_MAC_BASE->e_dyinggsp_w6) // 6180
#define e_dyinggsp_w7                                          INREG32(&EPON_MAC_BASE->e_dyinggsp_w7) // 6184
#define e_dyinggsp_w8                                          INREG32(&EPON_MAC_BASE->e_dyinggsp_w8) // 6188
#define e_dyinggsp_w9                                          INREG32(&EPON_MAC_BASE->e_dyinggsp_w9) // 618C
#define e_dyinggsp_w10                                         INREG32(&EPON_MAC_BASE->e_dyinggsp_w10) // 6190
#define e_dyinggsp_w11                                         INREG32(&EPON_MAC_BASE->e_dyinggsp_w11) // 6194
#define e_dyinggsp_w12                                         INREG32(&EPON_MAC_BASE->e_dyinggsp_w12) // 6198
#define e_oam_kpalv_w1                                         INREG32(&EPON_MAC_BASE->e_oam_kpalv_w1) // 619C
#define e_oam_kpalv_w2                                         INREG32(&EPON_MAC_BASE->e_oam_kpalv_w2) // 61A0
#define e_oam_kpalv_w3                                         INREG32(&EPON_MAC_BASE->e_oam_kpalv_w3) // 61A4
#define e_oam_kpalv_w4                                         INREG32(&EPON_MAC_BASE->e_oam_kpalv_w4) // 61A8
#define e_oam_kpalv_w5                                         INREG32(&EPON_MAC_BASE->e_oam_kpalv_w5) // 61AC
#define e_oam_kpalv_w6                                         INREG32(&EPON_MAC_BASE->e_oam_kpalv_w6) // 61B0
#define e_oam_kpalv_w7                                         INREG32(&EPON_MAC_BASE->e_oam_kpalv_w7) // 61B4
#define e_oam_kpalv_w8                                         INREG32(&EPON_MAC_BASE->e_oam_kpalv_w8) // 61B8
#define e_oam_kpalv_w9                                         INREG32(&EPON_MAC_BASE->e_oam_kpalv_w9) // 61BC
#define e_oam_kpalv_w10                                        INREG32(&EPON_MAC_BASE->e_oam_kpalv_w10) // 61C0
#define e_oam_kpalv_w11                                        INREG32(&EPON_MAC_BASE->e_oam_kpalv_w11) // 61C4
#define e_oam_kpalv_w12                                        INREG32(&EPON_MAC_BASE->e_oam_kpalv_w12) // 61C8
#define e_oam_kpalv_ctrl                                       INREG32(&EPON_MAC_BASE->e_oam_kpalv_ctrl) // 61CC
#define e_tod_1pps_ctrl                                        INREG32(&EPON_MAC_BASE->e_tod_1pps_ctrl) // 61D0
#define e_short_pkt_ctrl                                       INREG32(&EPON_MAC_BASE->e_short_pkt_ctrl) // 61D4
#define e_sniff_sp_tag                                         INREG32(&EPON_MAC_BASE->e_sniff_sp_tag) // 61D8

#endif


#define e_glb_cfg_FLD_sniffer_mode                             REG_FLD(1, 26)
#define e_glb_cfg_FLD_txoam_favor                              REG_FLD(1, 25)
#define e_glb_cfg_FLD_burst_en_dly                             REG_FLD(1, 24)
#define e_glb_cfg_FLD_discv_burst_en                           REG_FLD(1, 23)
#define e_glb_cfg_FLD_mpcp_fwd                                 REG_FLD(1, 22)
#define e_glb_cfg_FLD_bcst_llid_m1_drop                        REG_FLD(1, 21)
#define e_glb_cfg_FLD_bcst_llid_m0_drop                        REG_FLD(1, 20)
#define e_glb_cfg_FLD_mcst_llid_drop                           REG_FLD(1, 19)
#define e_glb_cfg_FLD_all_unicast_llid_pkt_fwd                 REG_FLD(1, 18)
#define e_glb_cfg_FLD_fcs_err_fwd                              REG_FLD(1, 17)
#define e_glb_cfg_FLD_llid_crc8_err_fwd                        REG_FLD(1, 16)
#define e_glb_cfg_FLD_rxmpi_stop                               REG_FLD(1, 13)
#define e_glb_cfg_FLD_txmpi_stop                               REG_FLD(1, 12)
#define e_glb_cfg_FLD_phy_pwr_down                             REG_FLD(1, 11)
#define e_glb_cfg_FLD_rx_nml_gate_fwd                          REG_FLD(1, 10)
#define e_glb_cfg_FLD_rxmbi_stop                               REG_FLD(1, 9)
#define e_glb_cfg_FLD_txmbi_stop                               REG_FLD(1, 8)
#define e_glb_cfg_FLD_chk_all_gnt_mode                         REG_FLD(1, 7)
#define e_glb_cfg_FLD_loc_cnt_sync_method                      REG_FLD(1, 6)
#define e_glb_cfg_FLD_tx_default_rpt                           REG_FLD(1, 5)
#define e_glb_cfg_FLD_epon_mac_sw_rst                          REG_FLD(1, 4)
#define e_glb_cfg_FLD_epon_oam_cal_in_eth                      REG_FLD(1, 3)
#define e_glb_cfg_FLD_epon_mac_lpbk_en                         REG_FLD(1, 2)
#define e_glb_cfg_FLD_rpt_txpri_ctrl                           REG_FLD(1, 1)
#define e_glb_cfg_FLD_mode_sel                                 REG_FLD(1, 0)

#define e_int_status_FLD_sniff_fifo_ovrun_int                  REG_FLD(1, 26)
#define e_int_status_FLD_reg_ack_done_int                      REG_FLD(1, 25)
#define e_int_status_FLD_reg_req_done_int                      REG_FLD(1, 24)
#define e_int_status_FLD_reorder1_gnt_int                      REG_FLD(1, 23)
#define e_int_status_FLD_b2b_gnt_int                           REG_FLD(1, 22)
#define e_int_status_FLD_hidn_gnt_int                          REG_FLD(1, 21)
#define e_int_status_FLD_ps_early_wakeup_int                   REG_FLD(1, 20)
#define e_int_status_FLD_rx_sleep_allow_int                    REG_FLD(1, 19)
#define e_int_status_FLD_ps_wakeup_int                         REG_FLD(1, 18)
#define e_int_status_FLD_ps_sleep_int                          REG_FLD(1, 17)
#define e_int_status_FLD_txfifo_udrun_int                      REG_FLD(1, 16)
#define e_int_status_FLD_rpt_overintvl_int                     REG_FLD(1, 15)
#define e_int_status_FLD_mpcp_timeout_int                      REG_FLD(1, 14)
#define e_int_status_FLD_timedrft_int                          REG_FLD(1, 13)
#define e_int_status_FLD_tod_1pps_int                          REG_FLD(1, 12)
#define e_int_status_FLD_tod_updt_int                          REG_FLD(1, 11)
#define e_int_status_FLD_ptp_msg_tx_int                        REG_FLD(1, 10)
#define e_int_status_FLD_gnt_buf_ovrrun_int                    REG_FLD(1, 9)
#define e_int_status_FLD_llid7_rcv_rgst_int                    REG_FLD(1, 8)
#define e_int_status_FLD_llid6_rcv_rgst_int                    REG_FLD(1, 7)
#define e_int_status_FLD_llid5_rcv_rgst_int                    REG_FLD(1, 6)
#define e_int_status_FLD_llid4_rcv_rgst_int                    REG_FLD(1, 5)
#define e_int_status_FLD_llid3_rcv_rgst_int                    REG_FLD(1, 4)
#define e_int_status_FLD_llid2_rcv_rgst_int                    REG_FLD(1, 3)
#define e_int_status_FLD_llid1_rcv_rgst_int                    REG_FLD(1, 2)
#define e_int_status_FLD_llid0_rcv_rgst_int                    REG_FLD(1, 1)
#define e_int_status_FLD_rcv_dscvry_gate_int                   REG_FLD(1, 0)

#define e_int_en_FLD_sniff_fifo_ovrun_en                       REG_FLD(1, 26)
#define e_int_en_FLD_reg_ack_done_en                           REG_FLD(1, 25)
#define e_int_en_FLD_reg_req_done_en                           REG_FLD(1, 24)
#define e_int_en_FLD_reorder1_gnt_en                           REG_FLD(1, 23)
#define e_int_en_FLD_b2b_gnt_en                                REG_FLD(1, 22)
#define e_int_en_FLD_hidn_gnt_en                               REG_FLD(1, 21)
#define e_int_en_FLD_ps_early_wakeup_en                        REG_FLD(1, 20)
#define e_int_en_FLD_rx_sleep_allow_en                         REG_FLD(1, 19)
#define e_int_en_FLD_ps_wakeup_en                              REG_FLD(1, 18)
#define e_int_en_FLD_ps_sleep_en                               REG_FLD(1, 17)
#define e_int_en_FLD_txfifo_udrun_en                           REG_FLD(1, 16)
#define e_int_en_FLD_rpt_overintvl_en                          REG_FLD(1, 15)
#define e_int_en_FLD_mpcp_timeout_en                           REG_FLD(1, 14)
#define e_int_en_FLD_timedrft_en                               REG_FLD(1, 13)
#define e_int_en_FLD_tod_1pps_en                               REG_FLD(1, 12)
#define e_int_en_FLD_tod_updt_en                               REG_FLD(1, 11)
#define e_int_en_FLD_ptp_msg_tx_en                             REG_FLD(1, 10)
#define e_int_en_FLD_gnt_buf_ovrrun_en                         REG_FLD(1, 9)
#define e_int_en_FLD_llid7_rcv_rgst_en                         REG_FLD(1, 8)
#define e_int_en_FLD_llid6_rcv_rgst_en                         REG_FLD(1, 7)
#define e_int_en_FLD_llid5_rcv_rgst_en                         REG_FLD(1, 6)
#define e_int_en_FLD_llid4_rcv_rgst_en                         REG_FLD(1, 5)
#define e_int_en_FLD_llid3_rcv_rgst_en                         REG_FLD(1, 4)
#define e_int_en_FLD_llid2_rcv_rgst_en                         REG_FLD(1, 3)
#define e_int_en_FLD_llid1_rcv_rgst_en                         REG_FLD(1, 2)
#define e_int_en_FLD_llid0_rcv_rgst_en                         REG_FLD(1, 1)
#define e_int_en_FLD_rcv_dscvry_gate_en                        REG_FLD(1, 0)

#define e_rpt_mpcp_timeout_llid_idx_FLD_llid7_mpcp_tmo         REG_FLD(1, 15)
#define e_rpt_mpcp_timeout_llid_idx_FLD_llid6_mpcp_tmo         REG_FLD(1, 14)
#define e_rpt_mpcp_timeout_llid_idx_FLD_llid5_mpcp_tmo         REG_FLD(1, 13)
#define e_rpt_mpcp_timeout_llid_idx_FLD_llid4_mpcp_tmo         REG_FLD(1, 12)
#define e_rpt_mpcp_timeout_llid_idx_FLD_llid3_mpcp_tmo         REG_FLD(1, 11)
#define e_rpt_mpcp_timeout_llid_idx_FLD_llid2_mpcp_tmo         REG_FLD(1, 10)
#define e_rpt_mpcp_timeout_llid_idx_FLD_llid1_mpcp_tmo         REG_FLD(1, 9)
#define e_rpt_mpcp_timeout_llid_idx_FLD_llid0_mpcp_tmo         REG_FLD(1, 8)
#define e_rpt_mpcp_timeout_llid_idx_FLD_llid7_rpt_tmo          REG_FLD(1, 7)
#define e_rpt_mpcp_timeout_llid_idx_FLD_llid6_rpt_tmo          REG_FLD(1, 6)
#define e_rpt_mpcp_timeout_llid_idx_FLD_llid5_rpt_tmo          REG_FLD(1, 5)
#define e_rpt_mpcp_timeout_llid_idx_FLD_llid4_rpt_tmo          REG_FLD(1, 4)
#define e_rpt_mpcp_timeout_llid_idx_FLD_llid3_rpt_tmo          REG_FLD(1, 3)
#define e_rpt_mpcp_timeout_llid_idx_FLD_llid2_rpt_tmo          REG_FLD(1, 2)
#define e_rpt_mpcp_timeout_llid_idx_FLD_llid1_rpt_tmo          REG_FLD(1, 1)
#define e_rpt_mpcp_timeout_llid_idx_FLD_llid0_rpt_tmo          REG_FLD(1, 0)

#define e_dyinggsp_cfg_FLD_hw_dying_gasp_en                    REG_FLD(1, 31)
#define e_dyinggsp_cfg_FLD_dying_gasp_code                     REG_FLD(8, 20)
#define e_dyinggsp_cfg_FLD_sw_init_dying_gasp                  REG_FLD(1, 16)
#define e_dyinggsp_cfg_FLD_dygsp_num_of_times                  REG_FLD(2, 8)
#define e_dyinggsp_cfg_FLD_dying_gasp_flags                    REG_FLD(7, 0)

#define e_pending_gnt_num_FLD_echoed_pending_gnt               REG_FLD(8, 8)
#define e_pending_gnt_num_FLD_pending_gnt_num                  REG_FLD(7, 0)

#define e_llid0_3_cfg_FLD_llid3_txfec_en                       REG_FLD(1, 27)
#define e_llid0_3_cfg_FLD_llid3_dcrypt_en                      REG_FLD(1, 26)
#define e_llid0_3_cfg_FLD_llid3_dcrypt_mode                    REG_FLD(1, 25)
#define e_llid0_3_cfg_FLD_llid3_oam_lpbk_en                    REG_FLD(1, 24)
#define e_llid0_3_cfg_FLD_llid2_txfec_en                       REG_FLD(1, 19)
#define e_llid0_3_cfg_FLD_llid2_dcrypt_en                      REG_FLD(1, 18)
#define e_llid0_3_cfg_FLD_llid2_dcrypt_mode                    REG_FLD(1, 17)
#define e_llid0_3_cfg_FLD_llid2_oam_lpbk_en                    REG_FLD(1, 16)
#define e_llid0_3_cfg_FLD_llid1_txfec_en                       REG_FLD(1, 11)
#define e_llid0_3_cfg_FLD_llid1_dcrypt_en                      REG_FLD(1, 10)
#define e_llid0_3_cfg_FLD_llid1_dcrypt_mode                    REG_FLD(1, 9)
#define e_llid0_3_cfg_FLD_llid1_oam_lpbk_en                    REG_FLD(1, 8)
#define e_llid0_3_cfg_FLD_llid0_txfec_en                       REG_FLD(1, 3)
#define e_llid0_3_cfg_FLD_llid0_dcrypt_en                      REG_FLD(1, 2)
#define e_llid0_3_cfg_FLD_llid0_dcrypt_mode                    REG_FLD(1, 1)
#define e_llid0_3_cfg_FLD_llid0_oam_lpbk_en                    REG_FLD(1, 0)

#define e_llid4_7_cfg_FLD_llid7_txfec_en                       REG_FLD(1, 27)
#define e_llid4_7_cfg_FLD_llid7_dcrypt_en                      REG_FLD(1, 26)
#define e_llid4_7_cfg_FLD_llid7_dcrypt_mode                    REG_FLD(1, 25)
#define e_llid4_7_cfg_FLD_llid7_oam_lpbk_en                    REG_FLD(1, 24)
#define e_llid4_7_cfg_FLD_llid6_txfec_en                       REG_FLD(1, 19)
#define e_llid4_7_cfg_FLD_llid6_dcrypt_en                      REG_FLD(1, 18)
#define e_llid4_7_cfg_FLD_llid6_dcrypt_mode                    REG_FLD(1, 17)
#define e_llid4_7_cfg_FLD_llid6_oam_lpbk_en                    REG_FLD(1, 16)
#define e_llid4_7_cfg_FLD_llid5_txfec_en                       REG_FLD(1, 11)
#define e_llid4_7_cfg_FLD_llid5_dcrypt_en                      REG_FLD(1, 10)
#define e_llid4_7_cfg_FLD_llid5_dcrypt_mode                    REG_FLD(1, 9)
#define e_llid4_7_cfg_FLD_llid5_oam_lpbk_en                    REG_FLD(1, 8)
#define e_llid4_7_cfg_FLD_llid4_txfec_en                       REG_FLD(1, 3)
#define e_llid4_7_cfg_FLD_llid4_dcrypt_en                      REG_FLD(1, 2)
#define e_llid4_7_cfg_FLD_llid4_dcrypt_mode                    REG_FLD(1, 1)
#define e_llid4_7_cfg_FLD_llid4_oam_lpbk_en                    REG_FLD(1, 0)

#define e_llid_dscvry_ctrl_FLD_mpcp_cmd                        REG_FLD(2, 30)
#define e_llid_dscvry_ctrl_FLD_mpcp_cmd_done                   REG_FLD(1, 16)
#define e_llid_dscvry_ctrl_FLD_rgstr_ack_flg                   REG_FLD(1, 12)
#define e_llid_dscvry_ctrl_FLD_rgstr_req_flg                   REG_FLD(1, 8)
#define e_llid_dscvry_ctrl_FLD_tx_mpcp_llid_idx                REG_FLD(3, 0)

#define e_llid0_dscvry_sts_FLD_llid0_dscvry_sts                REG_FLD(2, 30)
#define e_llid0_dscvry_sts_FLD_llid0_rgstr_flg_sts             REG_FLD(2, 24)
#define e_llid0_dscvry_sts_FLD_llid0_valid                     REG_FLD(1, 16)
#define e_llid0_dscvry_sts_FLD_llid0_value                     REG_FLD(16, 0)

#define e_llid1_dscvry_sts_FLD_llid1_dscvry_sts                REG_FLD(2, 30)
#define e_llid1_dscvry_sts_FLD_llid1_rgstr_flg_sts             REG_FLD(2, 24)
#define e_llid1_dscvry_sts_FLD_llid1_valid                     REG_FLD(1, 16)
#define e_llid1_dscvry_sts_FLD_llid1_value                     REG_FLD(16, 0)

#define e_llid2_dscvry_sts_FLD_llid2_dscvry_sts                REG_FLD(2, 30)
#define e_llid2_dscvry_sts_FLD_llid2_rgstr_flg_sts             REG_FLD(2, 24)
#define e_llid2_dscvry_sts_FLD_llid2_valid                     REG_FLD(1, 16)
#define e_llid2_dscvry_sts_FLD_llid2_value                     REG_FLD(16, 0)

#define e_llid3_dscvry_sts_FLD_llid3_dscvry_sts                REG_FLD(2, 30)
#define e_llid3_dscvry_sts_FLD_llid3_rgstr_flg_sts             REG_FLD(2, 24)
#define e_llid3_dscvry_sts_FLD_llid3_valid                     REG_FLD(1, 16)
#define e_llid3_dscvry_sts_FLD_llid3_value                     REG_FLD(16, 0)

#define e_llid4_dscvry_sts_FLD_llid4_dscvry_sts                REG_FLD(2, 30)
#define e_llid4_dscvry_sts_FLD_llid4_rgstr_flg_sts             REG_FLD(2, 24)
#define e_llid4_dscvry_sts_FLD_llid4_valid                     REG_FLD(1, 16)
#define e_llid4_dscvry_sts_FLD_llid4_value                     REG_FLD(16, 0)

#define e_llid5_dscvry_sts_FLD_llid5_dscvry_sts                REG_FLD(2, 30)
#define e_llid5_dscvry_sts_FLD_llid5_rgstr_flg_sts             REG_FLD(2, 24)
#define e_llid5_dscvry_sts_FLD_llid5_valid                     REG_FLD(1, 16)
#define e_llid5_dscvry_sts_FLD_llid5_value                     REG_FLD(16, 0)

#define e_llid6_dscvry_sts_FLD_llid6_dscvry_sts                REG_FLD(2, 30)
#define e_llid6_dscvry_sts_FLD_llid6_rgstr_flg_sts             REG_FLD(2, 24)
#define e_llid6_dscvry_sts_FLD_llid6_valid                     REG_FLD(1, 16)
#define e_llid6_dscvry_sts_FLD_llid6_value                     REG_FLD(16, 0)

#define e_llid7_dscvry_sts_FLD_llid7_dscvry_sts                REG_FLD(2, 30)
#define e_llid7_dscvry_sts_FLD_llid7_rgstr_flg_sts             REG_FLD(2, 24)
#define e_llid7_dscvry_sts_FLD_llid7_valid                     REG_FLD(1, 16)
#define e_llid7_dscvry_sts_FLD_llid7_value                     REG_FLD(16, 0)

#define e_mac_addr_cfg_FLD_mac_addr_rwcmd                      REG_FLD(1, 31)
#define e_mac_addr_cfg_FLD_mac_addr_rwcmd_done                 REG_FLD(1, 16)
#define e_mac_addr_cfg_FLD_mac_addr_llid_indx                  REG_FLD(3, 1)
#define e_mac_addr_cfg_FLD_mac_addr_dw_idx                     REG_FLD(1, 0)

#define e_mac_addr_value_FLD_mac_addr_value                    REG_FLD(32, 0)

#define e_security_key_cfg_FLD_key_rwcmd                       REG_FLD(1, 31)
#define e_security_key_cfg_FLD_key_rwcmd_done                  REG_FLD(1, 16)
#define e_security_key_cfg_FLD_key_llid_index                  REG_FLD(3, 3)
#define e_security_key_cfg_FLD_key_idx                         REG_FLD(1, 2)
#define e_security_key_cfg_FLD_key_dw_indx                     REG_FLD(2, 0)

#define e_key_value_FLD_key_value                              REG_FLD(24, 0)

#define e_rpt_data_FLD_df_rpt_data                             REG_FLD(32, 0)

#define e_rpt_len_FLD_df_rpt_fifo_clr                          REG_FLD(1, 8)
#define e_rpt_len_FLD_df_rpt_dt_len                            REG_FLD(6, 0)

#define e_rpt_cfg_FLD_qsize_fec_adj                            REG_FLD(16, 16)
#define e_rpt_cfg_FLD_llid7_rpt_cfg                            REG_FLD(2, 14)
#define e_rpt_cfg_FLD_llid6_rpt_cfg                            REG_FLD(2, 12)
#define e_rpt_cfg_FLD_llid5_rpt_cfg                            REG_FLD(2, 10)
#define e_rpt_cfg_FLD_llid4_rpt_cfg                            REG_FLD(2, 8)
#define e_rpt_cfg_FLD_llid3_rpt_cfg                            REG_FLD(2, 6)
#define e_rpt_cfg_FLD_llid2_rpt_cfg                            REG_FLD(2, 4)
#define e_rpt_cfg_FLD_llid1_rpt_cfg                            REG_FLD(2, 2)
#define e_rpt_cfg_FLD_llid0_rpt_cfg                            REG_FLD(2, 0)

#define e_rpt_qthld_cfg_FLD_qthld_rwcmd                        REG_FLD(1, 31)
#define e_rpt_qthld_cfg_FLD_qthld_rwcmd_done                   REG_FLD(1, 30)
#define e_rpt_qthld_cfg_FLD_qthld_value                        REG_FLD(16, 8)
#define e_rpt_qthld_cfg_FLD_qthld_idx                          REG_FLD(2, 6)
#define e_rpt_qthld_cfg_FLD_rpt_llid_idx                       REG_FLD(3, 3)
#define e_rpt_qthld_cfg_FLD_queue_idx                          REG_FLD(3, 0)

#define e_local_time_FLD_local_time                            REG_FLD(32, 0)

#define e_tod_sync_x_FLD_tod_sync_x                            REG_FLD(32, 0)

#define e_tod_ltncy_FLD_ingrs_latency                          REG_FLD(11, 8)
#define e_tod_ltncy_FLD_egrs_latency                           REG_FLD(8, 0)

#define p2p_tx_tag1_FLD_p2p_tx_tagging_done                    REG_FLD(1, 31)
#define p2p_tx_tag1_FLD_p2p_tx_pkt_info                        REG_FLD(4, 8)
#define p2p_tx_tag1_FLD_p2p_tx_tag_sec_l8                      REG_FLD(8, 0)

#define p2p_tx_tag2_FLD_p2p_tx_tag_nsec_29_5                   REG_FLD(25, 0)

#define e_new_tod_p2p_offset_sec_l32_FLD_new_tod_p2p_offset_sec_l32 REG_FLD(32, 0)

#define e_new_tod_p2p_tod_offset_nsec_FLD_new_tod_nsec         REG_FLD(32, 0)

#define e_tod_p2p_tod_sec_l32_FLD_tod_p2p_sec_l32              REG_FLD(32, 0)

#define e_tod_p2p_tod_nsec_FLD_tod_p2p_nsec                    REG_FLD(32, 0)

#define e_tod_period_FLD_tod_period                            REG_FLD(8, 0)

#define e_pwr_sv_cfg_FLD_pwd_mode                              REG_FLD(2, 28)
#define e_pwr_sv_cfg_FLD_timedrift_ignore                      REG_FLD(1, 26)
#define e_pwr_sv_cfg_FLD_onu_wakeup                            REG_FLD(1, 25)
#define e_pwr_sv_cfg_FLD_pwr_sv_start                          REG_FLD(1, 24)
#define e_pwr_sv_cfg_FLD_pwd_mode_i                            REG_FLD(3, 20)
#define e_pwr_sv_cfg_FLD_slp_duration_max_h                    REG_FLD(16, 0)

#define e_slp_durt_max_FLD_slp_duration_max_l                  REG_FLD(32, 0)

#define e_slp_duration_FLD_slp_duration                        REG_FLD(32, 0)

#define e_act_duration_FLD_act_duration                        REG_FLD(32, 0)

#define e_pwron_dly_FLD_pwron_dly                              REG_FLD(32, 0)

#define e_slp_duration_i_FLD_slp_duration_i                    REG_FLD(16, 0)

#define e_txfetch_cfg_FLD_tx_fetch_overhead                    REG_FLD(8, 24)
#define e_txfetch_cfg_FLD_tx_fetch_leadtime                    REG_FLD(8, 16)
#define e_txfetch_cfg_FLD_tx_dma_leadtime                      REG_FLD(12, 0)

#define e_sync_time_FLD_sync_time_updte                        REG_FLD(1, 16)
#define e_sync_time_FLD_sync_time                              REG_FLD(16, 0)

#define e_tx_cal_cnst_FLD_dscvr_gnt_len                        REG_FLD(8, 24)
#define e_tx_cal_cnst_FLD_fec_tail_grd                         REG_FLD(8, 16)
#define e_tx_cal_cnst_FLD_tail_grd                             REG_FLD(8, 8)
#define e_tx_cal_cnst_FLD_default_ovrhd                        REG_FLD(6, 0)

#define e_laser_onoff_time_FLD_laser_off_time                  REG_FLD(8, 8)
#define e_laser_onoff_time_FLD_laser_on_time                   REG_FLD(8, 0)

#define e_grd_thrshld_FLD_guard_thrshld                        REG_FLD(8, 0)

#define e_mpcp_timeout_intvl_FLD_mpcp_timeout_intvl            REG_FLD(32, 0)

#define e_rpt_timeout_intvl_FLD_rpt_timeout_intvl              REG_FLD(24, 0)

#define e_max_future_gnt_time_FLD_max_future_gnt_time          REG_FLD(32, 0)

#define e_min_proc_time_FLD_min_proc_time                      REG_FLD(16, 0)

#define e_trx_adjust_time1_FLD_tx_tmstp_adj                    REG_FLD(16, 16)
#define e_trx_adjust_time1_FLD_tx_stm_adj                      REG_FLD(16, 0)

#define e_trx_adjust_time2_FLD_tx_len_adj                      REG_FLD(16, 16)
#define e_trx_adjust_time2_FLD_rx_tmstp_adj                    REG_FLD(16, 0)

#define e_dbg_prb_sel_FLD_probe_bit0_sel                       REG_FLD(4, 8)
#define e_dbg_prb_sel_FLD_probe_sel                            REG_FLD(5, 0)

#define e_dbg_prb_h32_FLD_probe_h32                            REG_FLD(32, 0)

#define e_dbg_prb_l32_FLD_probe_l32                            REG_FLD(32, 0)

#define e_rxmbi_eth_cnt_FLD_rxmbi_eth_cnt                      REG_FLD(32, 0)

#define e_rxmpi_eth_cnt_FLD_rxmpi_eth_cnt                      REG_FLD(32, 0)

#define e_txmbi_eth_cnt_FLD_txmbi_eth_cnt                      REG_FLD(32, 0)

#define e_txmpi_eth_cnt_FLD_txmpi_eth_cnt                      REG_FLD(32, 0)

#define e_oam_stat_FLD_rx_oam_cnt                              REG_FLD(16, 16)
#define e_oam_stat_FLD_tx_oam_cnt                              REG_FLD(16, 0)

#define e_mpcp_stat_FLD_mpcp_err_cnt                           REG_FLD(8, 24)
#define e_mpcp_stat_FLD_mpcp_rgst_cnt                          REG_FLD(8, 16)
#define e_mpcp_stat_FLD_mpcp_dscv_gate_cnt                     REG_FLD(16, 0)

#define e_mpcp_rgst_stat_FLD_mpcp_rgst_req_cnt                 REG_FLD(8, 8)
#define e_mpcp_rgst_stat_FLD_mpcp_rgst_ack_cnt                 REG_FLD(8, 0)

#define e_gnt_pending_stat_FLD_max_gnt_pending_cnt             REG_FLD(7, 16)
#define e_gnt_pending_stat_FLD_cur_gnt_pending_cnt             REG_FLD(7, 0)

#define e_gnt_length_stat_FLD_max_gnt_length                   REG_FLD(16, 16)
#define e_gnt_length_stat_FLD_min_gnt_length                   REG_FLD(16, 0)

#define e_gnt_type_stat_FLD_b2b_gnt_cnt                        REG_FLD(16, 16)
#define e_gnt_type_stat_FLD_hdn_gnt_cnt                        REG_FLD(16, 0)

#define e_time_drft_stat_FLD_cur_time_drift                    REG_FLD(8, 8)
#define e_time_drft_stat_FLD_max_time_drift                    REG_FLD(8, 0)

#define e_llid0_gnt_stat_FLD_llid0_gnt_cnt                     REG_FLD(32, 0)

#define e_llid1_gnt_stat_FLD_llid1_gnt_cnt                     REG_FLD(32, 0)

#define e_llid2_gnt_stat_FLD_llid2_gnt_cnt                     REG_FLD(32, 0)

#define e_llid3_gnt_stat_FLD_llid3_gnt_cnt                     REG_FLD(32, 0)

#define e_llid4_gnt_stat_FLD_llid4_gnt_cnt                     REG_FLD(32, 0)

#define e_llid5_gnt_stat_FLD_llid5_gnt_cnt                     REG_FLD(32, 0)

#define e_llid6_gnt_stat_FLD_llid6_gnt_cnt                     REG_FLD(32, 0)

#define e_llid7_gnt_stat_FLD_llid7_gnt_cnt                     REG_FLD(32, 0)

#define e_snf_mpcp_oam_ctl_FLD_snf_mpcp_cap_en                 REG_FLD(1, 31)
#define e_snf_mpcp_oam_ctl_FLD_snf_oam_cap_en                  REG_FLD(1, 30)
#define e_snf_mpcp_oam_ctl_FLD_snf_mpcp_oam_cnt_set            REG_FLD(30, 0)

#define e_rpt_adj_FLD_rpt_bitmap_ctrl                          REG_FLD(1, 24)
#define e_rpt_adj_FLD_rpt_bitmap_set                           REG_FLD(8, 16)
#define e_rpt_adj_FLD_rpt_qsize_fecoff_adj                     REG_FLD(16, 0)

#define e_rpt_cnt_FLD_rpt_cnt                                  REG_FLD(16, 0)

#define e_dyinggsp_w1_FLD_dyinggsp_w1_len_type                 REG_FLD(32, 0)

#define e_dyinggsp_w2_FLD_dyinggsp_w2                          REG_FLD(32, 0)

#define e_dyinggsp_w3_FLD_dyinggsp_w3                          REG_FLD(32, 0)

#define e_dyinggsp_w4_FLD_dyinggsp_w4                          REG_FLD(32, 0)

#define e_dyinggsp_w5_FLD_dyinggsp_w5                          REG_FLD(32, 0)

#define e_dyinggsp_w6_FLD_dyinggsp_w6                          REG_FLD(32, 0)

#define e_dyinggsp_w7_FLD_dyinggsp_w7                          REG_FLD(32, 0)

#define e_dyinggsp_w8_FLD_dyinggsp_w8                          REG_FLD(32, 0)

#define e_dyinggsp_w9_FLD_dyinggsp_w9                          REG_FLD(32, 0)

#define e_dyinggsp_w10_FLD_dyinggsp_w10                        REG_FLD(32, 0)

#define e_dyinggsp_w11_FLD_dyinggsp_w11                        REG_FLD(32, 0)

#define e_dyinggsp_w12_FLD_dyinggsp_w12                        REG_FLD(32, 0)

#define e_oam_kpalv_w1_FLD_oam_kpalv_w1                        REG_FLD(32, 0)

#define e_oam_kpalv_w2_FLD_oam_kpalv_w2                        REG_FLD(32, 0)

#define e_oam_kpalv_w3_FLD_oam_kpalv_w3                        REG_FLD(32, 0)

#define e_oam_kpalv_w4_FLD_oam_kpalv_w4                        REG_FLD(32, 0)

#define e_oam_kpalv_w5_FLD_oam_kpalv_w5                        REG_FLD(32, 0)

#define e_oam_kpalv_w6_FLD_oam_kpalv_w6                        REG_FLD(32, 0)

#define e_oam_kpalv_w7_FLD_oam_kpalv_w7                        REG_FLD(32, 0)

#define e_oam_kpalv_w8_FLD_oam_kpalv_w8                        REG_FLD(32, 0)

#define e_oam_kpalv_w9_FLD_oam_kpalv_w9                        REG_FLD(32, 0)

#define e_oam_kpalv_w10_FLD_oam_kpalv_w10                      REG_FLD(32, 0)

#define e_oam_kpalv_w11_FLD_oam_kpalv_w11                      REG_FLD(32, 0)

#define e_oam_kpalv_w12_FLD_oam_kpalv_w12                      REG_FLD(32, 0)

#define e_oam_kpalv_ctrl_FLD_oam_kpalv_interval                REG_FLD(24, 8)
#define e_oam_kpalv_ctrl_FLD_oam_kpalv_sw_trig                 REG_FLD(1, 2)
#define e_oam_kpalv_ctrl_FLD_oam_kpalv_sw_cfg                  REG_FLD(1, 1)
#define e_oam_kpalv_ctrl_FLD_oam_kpalv_en                      REG_FLD(1, 0)

#define e_tod_1pps_ctrl_FLD_tod_1pps_width_ctrl                REG_FLD(32, 0)

#define e_short_pkt_ctrl_FLD_short_pkt_size_ctrl               REG_FLD(8, 0)

#define e_sniff_sp_tag_FLD_sniffer_sp_tag                      REG_FLD(32, 0)

#define e_glb_cfg_GET_sniffer_mode(reg32)                      REG_FLD_GET(e_glb_cfg_FLD_sniffer_mode, (reg32))
#define e_glb_cfg_GET_txoam_favor(reg32)                       REG_FLD_GET(e_glb_cfg_FLD_txoam_favor, (reg32))
#define e_glb_cfg_GET_burst_en_dly(reg32)                      REG_FLD_GET(e_glb_cfg_FLD_burst_en_dly, (reg32))
#define e_glb_cfg_GET_discv_burst_en(reg32)                    REG_FLD_GET(e_glb_cfg_FLD_discv_burst_en, (reg32))
#define e_glb_cfg_GET_mpcp_fwd(reg32)                          REG_FLD_GET(e_glb_cfg_FLD_mpcp_fwd, (reg32))
#define e_glb_cfg_GET_bcst_llid_m1_drop(reg32)                 REG_FLD_GET(e_glb_cfg_FLD_bcst_llid_m1_drop, (reg32))
#define e_glb_cfg_GET_bcst_llid_m0_drop(reg32)                 REG_FLD_GET(e_glb_cfg_FLD_bcst_llid_m0_drop, (reg32))
#define e_glb_cfg_GET_mcst_llid_drop(reg32)                    REG_FLD_GET(e_glb_cfg_FLD_mcst_llid_drop, (reg32))
#define e_glb_cfg_GET_all_unicast_llid_pkt_fwd(reg32)          REG_FLD_GET(e_glb_cfg_FLD_all_unicast_llid_pkt_fwd, (reg32))
#define e_glb_cfg_GET_fcs_err_fwd(reg32)                       REG_FLD_GET(e_glb_cfg_FLD_fcs_err_fwd, (reg32))
#define e_glb_cfg_GET_llid_crc8_err_fwd(reg32)                 REG_FLD_GET(e_glb_cfg_FLD_llid_crc8_err_fwd, (reg32))
#define e_glb_cfg_GET_rxmpi_stop(reg32)                        REG_FLD_GET(e_glb_cfg_FLD_rxmpi_stop, (reg32))
#define e_glb_cfg_GET_txmpi_stop(reg32)                        REG_FLD_GET(e_glb_cfg_FLD_txmpi_stop, (reg32))
#define e_glb_cfg_GET_phy_pwr_down(reg32)                      REG_FLD_GET(e_glb_cfg_FLD_phy_pwr_down, (reg32))
#define e_glb_cfg_GET_rx_nml_gate_fwd(reg32)                   REG_FLD_GET(e_glb_cfg_FLD_rx_nml_gate_fwd, (reg32))
#define e_glb_cfg_GET_rxmbi_stop(reg32)                        REG_FLD_GET(e_glb_cfg_FLD_rxmbi_stop, (reg32))
#define e_glb_cfg_GET_txmbi_stop(reg32)                        REG_FLD_GET(e_glb_cfg_FLD_txmbi_stop, (reg32))
#define e_glb_cfg_GET_chk_all_gnt_mode(reg32)                  REG_FLD_GET(e_glb_cfg_FLD_chk_all_gnt_mode, (reg32))
#define e_glb_cfg_GET_loc_cnt_sync_method(reg32)               REG_FLD_GET(e_glb_cfg_FLD_loc_cnt_sync_method, (reg32))
#define e_glb_cfg_GET_tx_default_rpt(reg32)                    REG_FLD_GET(e_glb_cfg_FLD_tx_default_rpt, (reg32))
#define e_glb_cfg_GET_epon_mac_sw_rst(reg32)                   REG_FLD_GET(e_glb_cfg_FLD_epon_mac_sw_rst, (reg32))
#define e_glb_cfg_GET_epon_oam_cal_in_eth(reg32)               REG_FLD_GET(e_glb_cfg_FLD_epon_oam_cal_in_eth, (reg32))
#define e_glb_cfg_GET_epon_mac_lpbk_en(reg32)                  REG_FLD_GET(e_glb_cfg_FLD_epon_mac_lpbk_en, (reg32))
#define e_glb_cfg_GET_rpt_txpri_ctrl(reg32)                    REG_FLD_GET(e_glb_cfg_FLD_rpt_txpri_ctrl, (reg32))
#define e_glb_cfg_GET_mode_sel(reg32)                          REG_FLD_GET(e_glb_cfg_FLD_mode_sel, (reg32))

#define e_int_status_GET_sniff_fifo_ovrun_int(reg32)           REG_FLD_GET(e_int_status_FLD_sniff_fifo_ovrun_int, (reg32))
#define e_int_status_GET_reg_ack_done_int(reg32)               REG_FLD_GET(e_int_status_FLD_reg_ack_done_int, (reg32))
#define e_int_status_GET_reg_req_done_int(reg32)               REG_FLD_GET(e_int_status_FLD_reg_req_done_int, (reg32))
#define e_int_status_GET_reorder1_gnt_int(reg32)               REG_FLD_GET(e_int_status_FLD_reorder1_gnt_int, (reg32))
#define e_int_status_GET_b2b_gnt_int(reg32)                    REG_FLD_GET(e_int_status_FLD_b2b_gnt_int, (reg32))
#define e_int_status_GET_hidn_gnt_int(reg32)                   REG_FLD_GET(e_int_status_FLD_hidn_gnt_int, (reg32))
#define e_int_status_GET_ps_early_wakeup_int(reg32)            REG_FLD_GET(e_int_status_FLD_ps_early_wakeup_int, (reg32))
#define e_int_status_GET_rx_sleep_allow_int(reg32)             REG_FLD_GET(e_int_status_FLD_rx_sleep_allow_int, (reg32))
#define e_int_status_GET_ps_wakeup_int(reg32)                  REG_FLD_GET(e_int_status_FLD_ps_wakeup_int, (reg32))
#define e_int_status_GET_ps_sleep_int(reg32)                   REG_FLD_GET(e_int_status_FLD_ps_sleep_int, (reg32))
#define e_int_status_GET_txfifo_udrun_int(reg32)               REG_FLD_GET(e_int_status_FLD_txfifo_udrun_int, (reg32))
#define e_int_status_GET_rpt_overintvl_int(reg32)              REG_FLD_GET(e_int_status_FLD_rpt_overintvl_int, (reg32))
#define e_int_status_GET_mpcp_timeout_int(reg32)               REG_FLD_GET(e_int_status_FLD_mpcp_timeout_int, (reg32))
#define e_int_status_GET_timedrft_int(reg32)                   REG_FLD_GET(e_int_status_FLD_timedrft_int, (reg32))
#define e_int_status_GET_tod_1pps_int(reg32)                   REG_FLD_GET(e_int_status_FLD_tod_1pps_int, (reg32))
#define e_int_status_GET_tod_updt_int(reg32)                   REG_FLD_GET(e_int_status_FLD_tod_updt_int, (reg32))
#define e_int_status_GET_ptp_msg_tx_int(reg32)                 REG_FLD_GET(e_int_status_FLD_ptp_msg_tx_int, (reg32))
#define e_int_status_GET_gnt_buf_ovrrun_int(reg32)             REG_FLD_GET(e_int_status_FLD_gnt_buf_ovrrun_int, (reg32))
#define e_int_status_GET_llid7_rcv_rgst_int(reg32)             REG_FLD_GET(e_int_status_FLD_llid7_rcv_rgst_int, (reg32))
#define e_int_status_GET_llid6_rcv_rgst_int(reg32)             REG_FLD_GET(e_int_status_FLD_llid6_rcv_rgst_int, (reg32))
#define e_int_status_GET_llid5_rcv_rgst_int(reg32)             REG_FLD_GET(e_int_status_FLD_llid5_rcv_rgst_int, (reg32))
#define e_int_status_GET_llid4_rcv_rgst_int(reg32)             REG_FLD_GET(e_int_status_FLD_llid4_rcv_rgst_int, (reg32))
#define e_int_status_GET_llid3_rcv_rgst_int(reg32)             REG_FLD_GET(e_int_status_FLD_llid3_rcv_rgst_int, (reg32))
#define e_int_status_GET_llid2_rcv_rgst_int(reg32)             REG_FLD_GET(e_int_status_FLD_llid2_rcv_rgst_int, (reg32))
#define e_int_status_GET_llid1_rcv_rgst_int(reg32)             REG_FLD_GET(e_int_status_FLD_llid1_rcv_rgst_int, (reg32))
#define e_int_status_GET_llid0_rcv_rgst_int(reg32)             REG_FLD_GET(e_int_status_FLD_llid0_rcv_rgst_int, (reg32))
#define e_int_status_GET_rcv_dscvry_gate_int(reg32)            REG_FLD_GET(e_int_status_FLD_rcv_dscvry_gate_int, (reg32))

#define e_int_en_GET_sniff_fifo_ovrun_en(reg32)                REG_FLD_GET(e_int_en_FLD_sniff_fifo_ovrun_en, (reg32))
#define e_int_en_GET_reg_ack_done_en(reg32)                    REG_FLD_GET(e_int_en_FLD_reg_ack_done_en, (reg32))
#define e_int_en_GET_reg_req_done_en(reg32)                    REG_FLD_GET(e_int_en_FLD_reg_req_done_en, (reg32))
#define e_int_en_GET_reorder1_gnt_en(reg32)                    REG_FLD_GET(e_int_en_FLD_reorder1_gnt_en, (reg32))
#define e_int_en_GET_b2b_gnt_en(reg32)                         REG_FLD_GET(e_int_en_FLD_b2b_gnt_en, (reg32))
#define e_int_en_GET_hidn_gnt_en(reg32)                        REG_FLD_GET(e_int_en_FLD_hidn_gnt_en, (reg32))
#define e_int_en_GET_ps_early_wakeup_en(reg32)                 REG_FLD_GET(e_int_en_FLD_ps_early_wakeup_en, (reg32))
#define e_int_en_GET_rx_sleep_allow_en(reg32)                  REG_FLD_GET(e_int_en_FLD_rx_sleep_allow_en, (reg32))
#define e_int_en_GET_ps_wakeup_en(reg32)                       REG_FLD_GET(e_int_en_FLD_ps_wakeup_en, (reg32))
#define e_int_en_GET_ps_sleep_en(reg32)                        REG_FLD_GET(e_int_en_FLD_ps_sleep_en, (reg32))
#define e_int_en_GET_txfifo_udrun_en(reg32)                    REG_FLD_GET(e_int_en_FLD_txfifo_udrun_en, (reg32))
#define e_int_en_GET_rpt_overintvl_en(reg32)                   REG_FLD_GET(e_int_en_FLD_rpt_overintvl_en, (reg32))
#define e_int_en_GET_mpcp_timeout_en(reg32)                    REG_FLD_GET(e_int_en_FLD_mpcp_timeout_en, (reg32))
#define e_int_en_GET_timedrft_en(reg32)                        REG_FLD_GET(e_int_en_FLD_timedrft_en, (reg32))
#define e_int_en_GET_tod_1pps_en(reg32)                        REG_FLD_GET(e_int_en_FLD_tod_1pps_en, (reg32))
#define e_int_en_GET_tod_updt_en(reg32)                        REG_FLD_GET(e_int_en_FLD_tod_updt_en, (reg32))
#define e_int_en_GET_ptp_msg_tx_en(reg32)                      REG_FLD_GET(e_int_en_FLD_ptp_msg_tx_en, (reg32))
#define e_int_en_GET_gnt_buf_ovrrun_en(reg32)                  REG_FLD_GET(e_int_en_FLD_gnt_buf_ovrrun_en, (reg32))
#define e_int_en_GET_llid7_rcv_rgst_en(reg32)                  REG_FLD_GET(e_int_en_FLD_llid7_rcv_rgst_en, (reg32))
#define e_int_en_GET_llid6_rcv_rgst_en(reg32)                  REG_FLD_GET(e_int_en_FLD_llid6_rcv_rgst_en, (reg32))
#define e_int_en_GET_llid5_rcv_rgst_en(reg32)                  REG_FLD_GET(e_int_en_FLD_llid5_rcv_rgst_en, (reg32))
#define e_int_en_GET_llid4_rcv_rgst_en(reg32)                  REG_FLD_GET(e_int_en_FLD_llid4_rcv_rgst_en, (reg32))
#define e_int_en_GET_llid3_rcv_rgst_en(reg32)                  REG_FLD_GET(e_int_en_FLD_llid3_rcv_rgst_en, (reg32))
#define e_int_en_GET_llid2_rcv_rgst_en(reg32)                  REG_FLD_GET(e_int_en_FLD_llid2_rcv_rgst_en, (reg32))
#define e_int_en_GET_llid1_rcv_rgst_en(reg32)                  REG_FLD_GET(e_int_en_FLD_llid1_rcv_rgst_en, (reg32))
#define e_int_en_GET_llid0_rcv_rgst_en(reg32)                  REG_FLD_GET(e_int_en_FLD_llid0_rcv_rgst_en, (reg32))
#define e_int_en_GET_rcv_dscvry_gate_en(reg32)                 REG_FLD_GET(e_int_en_FLD_rcv_dscvry_gate_en, (reg32))

#define e_rpt_mpcp_timeout_llid_idx_GET_llid7_mpcp_tmo(reg32)  REG_FLD_GET(e_rpt_mpcp_timeout_llid_idx_FLD_llid7_mpcp_tmo, (reg32))
#define e_rpt_mpcp_timeout_llid_idx_GET_llid6_mpcp_tmo(reg32)  REG_FLD_GET(e_rpt_mpcp_timeout_llid_idx_FLD_llid6_mpcp_tmo, (reg32))
#define e_rpt_mpcp_timeout_llid_idx_GET_llid5_mpcp_tmo(reg32)  REG_FLD_GET(e_rpt_mpcp_timeout_llid_idx_FLD_llid5_mpcp_tmo, (reg32))
#define e_rpt_mpcp_timeout_llid_idx_GET_llid4_mpcp_tmo(reg32)  REG_FLD_GET(e_rpt_mpcp_timeout_llid_idx_FLD_llid4_mpcp_tmo, (reg32))
#define e_rpt_mpcp_timeout_llid_idx_GET_llid3_mpcp_tmo(reg32)  REG_FLD_GET(e_rpt_mpcp_timeout_llid_idx_FLD_llid3_mpcp_tmo, (reg32))
#define e_rpt_mpcp_timeout_llid_idx_GET_llid2_mpcp_tmo(reg32)  REG_FLD_GET(e_rpt_mpcp_timeout_llid_idx_FLD_llid2_mpcp_tmo, (reg32))
#define e_rpt_mpcp_timeout_llid_idx_GET_llid1_mpcp_tmo(reg32)  REG_FLD_GET(e_rpt_mpcp_timeout_llid_idx_FLD_llid1_mpcp_tmo, (reg32))
#define e_rpt_mpcp_timeout_llid_idx_GET_llid0_mpcp_tmo(reg32)  REG_FLD_GET(e_rpt_mpcp_timeout_llid_idx_FLD_llid0_mpcp_tmo, (reg32))
#define e_rpt_mpcp_timeout_llid_idx_GET_llid7_rpt_tmo(reg32)   REG_FLD_GET(e_rpt_mpcp_timeout_llid_idx_FLD_llid7_rpt_tmo, (reg32))
#define e_rpt_mpcp_timeout_llid_idx_GET_llid6_rpt_tmo(reg32)   REG_FLD_GET(e_rpt_mpcp_timeout_llid_idx_FLD_llid6_rpt_tmo, (reg32))
#define e_rpt_mpcp_timeout_llid_idx_GET_llid5_rpt_tmo(reg32)   REG_FLD_GET(e_rpt_mpcp_timeout_llid_idx_FLD_llid5_rpt_tmo, (reg32))
#define e_rpt_mpcp_timeout_llid_idx_GET_llid4_rpt_tmo(reg32)   REG_FLD_GET(e_rpt_mpcp_timeout_llid_idx_FLD_llid4_rpt_tmo, (reg32))
#define e_rpt_mpcp_timeout_llid_idx_GET_llid3_rpt_tmo(reg32)   REG_FLD_GET(e_rpt_mpcp_timeout_llid_idx_FLD_llid3_rpt_tmo, (reg32))
#define e_rpt_mpcp_timeout_llid_idx_GET_llid2_rpt_tmo(reg32)   REG_FLD_GET(e_rpt_mpcp_timeout_llid_idx_FLD_llid2_rpt_tmo, (reg32))
#define e_rpt_mpcp_timeout_llid_idx_GET_llid1_rpt_tmo(reg32)   REG_FLD_GET(e_rpt_mpcp_timeout_llid_idx_FLD_llid1_rpt_tmo, (reg32))
#define e_rpt_mpcp_timeout_llid_idx_GET_llid0_rpt_tmo(reg32)   REG_FLD_GET(e_rpt_mpcp_timeout_llid_idx_FLD_llid0_rpt_tmo, (reg32))

#define e_dyinggsp_cfg_GET_hw_dying_gasp_en(reg32)             REG_FLD_GET(e_dyinggsp_cfg_FLD_hw_dying_gasp_en, (reg32))
#define e_dyinggsp_cfg_GET_dying_gasp_code(reg32)              REG_FLD_GET(e_dyinggsp_cfg_FLD_dying_gasp_code, (reg32))
#define e_dyinggsp_cfg_GET_sw_init_dying_gasp(reg32)           REG_FLD_GET(e_dyinggsp_cfg_FLD_sw_init_dying_gasp, (reg32))
#define e_dyinggsp_cfg_GET_dygsp_num_of_times(reg32)           REG_FLD_GET(e_dyinggsp_cfg_FLD_dygsp_num_of_times, (reg32))
#define e_dyinggsp_cfg_GET_dying_gasp_flags(reg32)             REG_FLD_GET(e_dyinggsp_cfg_FLD_dying_gasp_flags, (reg32))

#define e_pending_gnt_num_GET_echoed_pending_gnt(reg32)        REG_FLD_GET(e_pending_gnt_num_FLD_echoed_pending_gnt, (reg32))
#define e_pending_gnt_num_GET_pending_gnt_num(reg32)           REG_FLD_GET(e_pending_gnt_num_FLD_pending_gnt_num, (reg32))

#define e_llid0_3_cfg_GET_llid3_txfec_en(reg32)                REG_FLD_GET(e_llid0_3_cfg_FLD_llid3_txfec_en, (reg32))
#define e_llid0_3_cfg_GET_llid3_dcrypt_en(reg32)               REG_FLD_GET(e_llid0_3_cfg_FLD_llid3_dcrypt_en, (reg32))
#define e_llid0_3_cfg_GET_llid3_dcrypt_mode(reg32)             REG_FLD_GET(e_llid0_3_cfg_FLD_llid3_dcrypt_mode, (reg32))
#define e_llid0_3_cfg_GET_llid3_oam_lpbk_en(reg32)             REG_FLD_GET(e_llid0_3_cfg_FLD_llid3_oam_lpbk_en, (reg32))
#define e_llid0_3_cfg_GET_llid2_txfec_en(reg32)                REG_FLD_GET(e_llid0_3_cfg_FLD_llid2_txfec_en, (reg32))
#define e_llid0_3_cfg_GET_llid2_dcrypt_en(reg32)               REG_FLD_GET(e_llid0_3_cfg_FLD_llid2_dcrypt_en, (reg32))
#define e_llid0_3_cfg_GET_llid2_dcrypt_mode(reg32)             REG_FLD_GET(e_llid0_3_cfg_FLD_llid2_dcrypt_mode, (reg32))
#define e_llid0_3_cfg_GET_llid2_oam_lpbk_en(reg32)             REG_FLD_GET(e_llid0_3_cfg_FLD_llid2_oam_lpbk_en, (reg32))
#define e_llid0_3_cfg_GET_llid1_txfec_en(reg32)                REG_FLD_GET(e_llid0_3_cfg_FLD_llid1_txfec_en, (reg32))
#define e_llid0_3_cfg_GET_llid1_dcrypt_en(reg32)               REG_FLD_GET(e_llid0_3_cfg_FLD_llid1_dcrypt_en, (reg32))
#define e_llid0_3_cfg_GET_llid1_dcrypt_mode(reg32)             REG_FLD_GET(e_llid0_3_cfg_FLD_llid1_dcrypt_mode, (reg32))
#define e_llid0_3_cfg_GET_llid1_oam_lpbk_en(reg32)             REG_FLD_GET(e_llid0_3_cfg_FLD_llid1_oam_lpbk_en, (reg32))
#define e_llid0_3_cfg_GET_llid0_txfec_en(reg32)                REG_FLD_GET(e_llid0_3_cfg_FLD_llid0_txfec_en, (reg32))
#define e_llid0_3_cfg_GET_llid0_dcrypt_en(reg32)               REG_FLD_GET(e_llid0_3_cfg_FLD_llid0_dcrypt_en, (reg32))
#define e_llid0_3_cfg_GET_llid0_dcrypt_mode(reg32)             REG_FLD_GET(e_llid0_3_cfg_FLD_llid0_dcrypt_mode, (reg32))
#define e_llid0_3_cfg_GET_llid0_oam_lpbk_en(reg32)             REG_FLD_GET(e_llid0_3_cfg_FLD_llid0_oam_lpbk_en, (reg32))

#define e_llid4_7_cfg_GET_llid7_txfec_en(reg32)                REG_FLD_GET(e_llid4_7_cfg_FLD_llid7_txfec_en, (reg32))
#define e_llid4_7_cfg_GET_llid7_dcrypt_en(reg32)               REG_FLD_GET(e_llid4_7_cfg_FLD_llid7_dcrypt_en, (reg32))
#define e_llid4_7_cfg_GET_llid7_dcrypt_mode(reg32)             REG_FLD_GET(e_llid4_7_cfg_FLD_llid7_dcrypt_mode, (reg32))
#define e_llid4_7_cfg_GET_llid7_oam_lpbk_en(reg32)             REG_FLD_GET(e_llid4_7_cfg_FLD_llid7_oam_lpbk_en, (reg32))
#define e_llid4_7_cfg_GET_llid6_txfec_en(reg32)                REG_FLD_GET(e_llid4_7_cfg_FLD_llid6_txfec_en, (reg32))
#define e_llid4_7_cfg_GET_llid6_dcrypt_en(reg32)               REG_FLD_GET(e_llid4_7_cfg_FLD_llid6_dcrypt_en, (reg32))
#define e_llid4_7_cfg_GET_llid6_dcrypt_mode(reg32)             REG_FLD_GET(e_llid4_7_cfg_FLD_llid6_dcrypt_mode, (reg32))
#define e_llid4_7_cfg_GET_llid6_oam_lpbk_en(reg32)             REG_FLD_GET(e_llid4_7_cfg_FLD_llid6_oam_lpbk_en, (reg32))
#define e_llid4_7_cfg_GET_llid5_txfec_en(reg32)                REG_FLD_GET(e_llid4_7_cfg_FLD_llid5_txfec_en, (reg32))
#define e_llid4_7_cfg_GET_llid5_dcrypt_en(reg32)               REG_FLD_GET(e_llid4_7_cfg_FLD_llid5_dcrypt_en, (reg32))
#define e_llid4_7_cfg_GET_llid5_dcrypt_mode(reg32)             REG_FLD_GET(e_llid4_7_cfg_FLD_llid5_dcrypt_mode, (reg32))
#define e_llid4_7_cfg_GET_llid5_oam_lpbk_en(reg32)             REG_FLD_GET(e_llid4_7_cfg_FLD_llid5_oam_lpbk_en, (reg32))
#define e_llid4_7_cfg_GET_llid4_txfec_en(reg32)                REG_FLD_GET(e_llid4_7_cfg_FLD_llid4_txfec_en, (reg32))
#define e_llid4_7_cfg_GET_llid4_dcrypt_en(reg32)               REG_FLD_GET(e_llid4_7_cfg_FLD_llid4_dcrypt_en, (reg32))
#define e_llid4_7_cfg_GET_llid4_dcrypt_mode(reg32)             REG_FLD_GET(e_llid4_7_cfg_FLD_llid4_dcrypt_mode, (reg32))
#define e_llid4_7_cfg_GET_llid4_oam_lpbk_en(reg32)             REG_FLD_GET(e_llid4_7_cfg_FLD_llid4_oam_lpbk_en, (reg32))

#define e_llid_dscvry_ctrl_GET_mpcp_cmd(reg32)                 REG_FLD_GET(e_llid_dscvry_ctrl_FLD_mpcp_cmd, (reg32))
#define e_llid_dscvry_ctrl_GET_mpcp_cmd_done(reg32)            REG_FLD_GET(e_llid_dscvry_ctrl_FLD_mpcp_cmd_done, (reg32))
#define e_llid_dscvry_ctrl_GET_rgstr_ack_flg(reg32)            REG_FLD_GET(e_llid_dscvry_ctrl_FLD_rgstr_ack_flg, (reg32))
#define e_llid_dscvry_ctrl_GET_rgstr_req_flg(reg32)            REG_FLD_GET(e_llid_dscvry_ctrl_FLD_rgstr_req_flg, (reg32))
#define e_llid_dscvry_ctrl_GET_tx_mpcp_llid_idx(reg32)         REG_FLD_GET(e_llid_dscvry_ctrl_FLD_tx_mpcp_llid_idx, (reg32))

#define e_llid0_dscvry_sts_GET_llid0_dscvry_sts(reg32)         REG_FLD_GET(e_llid0_dscvry_sts_FLD_llid0_dscvry_sts, (reg32))
#define e_llid0_dscvry_sts_GET_llid0_rgstr_flg_sts(reg32)      REG_FLD_GET(e_llid0_dscvry_sts_FLD_llid0_rgstr_flg_sts, (reg32))
#define e_llid0_dscvry_sts_GET_llid0_valid(reg32)              REG_FLD_GET(e_llid0_dscvry_sts_FLD_llid0_valid, (reg32))
#define e_llid0_dscvry_sts_GET_llid0_value(reg32)              REG_FLD_GET(e_llid0_dscvry_sts_FLD_llid0_value, (reg32))

#define e_llid1_dscvry_sts_GET_llid1_dscvry_sts(reg32)         REG_FLD_GET(e_llid1_dscvry_sts_FLD_llid1_dscvry_sts, (reg32))
#define e_llid1_dscvry_sts_GET_llid1_rgstr_flg_sts(reg32)      REG_FLD_GET(e_llid1_dscvry_sts_FLD_llid1_rgstr_flg_sts, (reg32))
#define e_llid1_dscvry_sts_GET_llid1_valid(reg32)              REG_FLD_GET(e_llid1_dscvry_sts_FLD_llid1_valid, (reg32))
#define e_llid1_dscvry_sts_GET_llid1_value(reg32)              REG_FLD_GET(e_llid1_dscvry_sts_FLD_llid1_value, (reg32))

#define e_llid2_dscvry_sts_GET_llid2_dscvry_sts(reg32)         REG_FLD_GET(e_llid2_dscvry_sts_FLD_llid2_dscvry_sts, (reg32))
#define e_llid2_dscvry_sts_GET_llid2_rgstr_flg_sts(reg32)      REG_FLD_GET(e_llid2_dscvry_sts_FLD_llid2_rgstr_flg_sts, (reg32))
#define e_llid2_dscvry_sts_GET_llid2_valid(reg32)              REG_FLD_GET(e_llid2_dscvry_sts_FLD_llid2_valid, (reg32))
#define e_llid2_dscvry_sts_GET_llid2_value(reg32)              REG_FLD_GET(e_llid2_dscvry_sts_FLD_llid2_value, (reg32))

#define e_llid3_dscvry_sts_GET_llid3_dscvry_sts(reg32)         REG_FLD_GET(e_llid3_dscvry_sts_FLD_llid3_dscvry_sts, (reg32))
#define e_llid3_dscvry_sts_GET_llid3_rgstr_flg_sts(reg32)      REG_FLD_GET(e_llid3_dscvry_sts_FLD_llid3_rgstr_flg_sts, (reg32))
#define e_llid3_dscvry_sts_GET_llid3_valid(reg32)              REG_FLD_GET(e_llid3_dscvry_sts_FLD_llid3_valid, (reg32))
#define e_llid3_dscvry_sts_GET_llid3_value(reg32)              REG_FLD_GET(e_llid3_dscvry_sts_FLD_llid3_value, (reg32))

#define e_llid4_dscvry_sts_GET_llid4_dscvry_sts(reg32)         REG_FLD_GET(e_llid4_dscvry_sts_FLD_llid4_dscvry_sts, (reg32))
#define e_llid4_dscvry_sts_GET_llid4_rgstr_flg_sts(reg32)      REG_FLD_GET(e_llid4_dscvry_sts_FLD_llid4_rgstr_flg_sts, (reg32))
#define e_llid4_dscvry_sts_GET_llid4_valid(reg32)              REG_FLD_GET(e_llid4_dscvry_sts_FLD_llid4_valid, (reg32))
#define e_llid4_dscvry_sts_GET_llid4_value(reg32)              REG_FLD_GET(e_llid4_dscvry_sts_FLD_llid4_value, (reg32))

#define e_llid5_dscvry_sts_GET_llid5_dscvry_sts(reg32)         REG_FLD_GET(e_llid5_dscvry_sts_FLD_llid5_dscvry_sts, (reg32))
#define e_llid5_dscvry_sts_GET_llid5_rgstr_flg_sts(reg32)      REG_FLD_GET(e_llid5_dscvry_sts_FLD_llid5_rgstr_flg_sts, (reg32))
#define e_llid5_dscvry_sts_GET_llid5_valid(reg32)              REG_FLD_GET(e_llid5_dscvry_sts_FLD_llid5_valid, (reg32))
#define e_llid5_dscvry_sts_GET_llid5_value(reg32)              REG_FLD_GET(e_llid5_dscvry_sts_FLD_llid5_value, (reg32))

#define e_llid6_dscvry_sts_GET_llid6_dscvry_sts(reg32)         REG_FLD_GET(e_llid6_dscvry_sts_FLD_llid6_dscvry_sts, (reg32))
#define e_llid6_dscvry_sts_GET_llid6_rgstr_flg_sts(reg32)      REG_FLD_GET(e_llid6_dscvry_sts_FLD_llid6_rgstr_flg_sts, (reg32))
#define e_llid6_dscvry_sts_GET_llid6_valid(reg32)              REG_FLD_GET(e_llid6_dscvry_sts_FLD_llid6_valid, (reg32))
#define e_llid6_dscvry_sts_GET_llid6_value(reg32)              REG_FLD_GET(e_llid6_dscvry_sts_FLD_llid6_value, (reg32))

#define e_llid7_dscvry_sts_GET_llid7_dscvry_sts(reg32)         REG_FLD_GET(e_llid7_dscvry_sts_FLD_llid7_dscvry_sts, (reg32))
#define e_llid7_dscvry_sts_GET_llid7_rgstr_flg_sts(reg32)      REG_FLD_GET(e_llid7_dscvry_sts_FLD_llid7_rgstr_flg_sts, (reg32))
#define e_llid7_dscvry_sts_GET_llid7_valid(reg32)              REG_FLD_GET(e_llid7_dscvry_sts_FLD_llid7_valid, (reg32))
#define e_llid7_dscvry_sts_GET_llid7_value(reg32)              REG_FLD_GET(e_llid7_dscvry_sts_FLD_llid7_value, (reg32))

#define e_mac_addr_cfg_GET_mac_addr_rwcmd(reg32)               REG_FLD_GET(e_mac_addr_cfg_FLD_mac_addr_rwcmd, (reg32))
#define e_mac_addr_cfg_GET_mac_addr_rwcmd_done(reg32)          REG_FLD_GET(e_mac_addr_cfg_FLD_mac_addr_rwcmd_done, (reg32))
#define e_mac_addr_cfg_GET_mac_addr_llid_indx(reg32)           REG_FLD_GET(e_mac_addr_cfg_FLD_mac_addr_llid_indx, (reg32))
#define e_mac_addr_cfg_GET_mac_addr_dw_idx(reg32)              REG_FLD_GET(e_mac_addr_cfg_FLD_mac_addr_dw_idx, (reg32))

#define e_mac_addr_value_GET_mac_addr_value(reg32)             REG_FLD_GET(e_mac_addr_value_FLD_mac_addr_value, (reg32))

#define e_security_key_cfg_GET_key_rwcmd(reg32)                REG_FLD_GET(e_security_key_cfg_FLD_key_rwcmd, (reg32))
#define e_security_key_cfg_GET_key_rwcmd_done(reg32)           REG_FLD_GET(e_security_key_cfg_FLD_key_rwcmd_done, (reg32))
#define e_security_key_cfg_GET_key_llid_index(reg32)           REG_FLD_GET(e_security_key_cfg_FLD_key_llid_index, (reg32))
#define e_security_key_cfg_GET_key_idx(reg32)                  REG_FLD_GET(e_security_key_cfg_FLD_key_idx, (reg32))
#define e_security_key_cfg_GET_key_dw_indx(reg32)              REG_FLD_GET(e_security_key_cfg_FLD_key_dw_indx, (reg32))

#define e_key_value_GET_key_value(reg32)                       REG_FLD_GET(e_key_value_FLD_key_value, (reg32))

#define e_rpt_data_GET_df_rpt_data(reg32)                      REG_FLD_GET(e_rpt_data_FLD_df_rpt_data, (reg32))

#define e_rpt_len_GET_df_rpt_fifo_clr(reg32)                   REG_FLD_GET(e_rpt_len_FLD_df_rpt_fifo_clr, (reg32))
#define e_rpt_len_GET_df_rpt_dt_len(reg32)                     REG_FLD_GET(e_rpt_len_FLD_df_rpt_dt_len, (reg32))

#define e_rpt_cfg_GET_qsize_fec_adj(reg32)                     REG_FLD_GET(e_rpt_cfg_FLD_qsize_fec_adj, (reg32))
#define e_rpt_cfg_GET_llid7_rpt_cfg(reg32)                     REG_FLD_GET(e_rpt_cfg_FLD_llid7_rpt_cfg, (reg32))
#define e_rpt_cfg_GET_llid6_rpt_cfg(reg32)                     REG_FLD_GET(e_rpt_cfg_FLD_llid6_rpt_cfg, (reg32))
#define e_rpt_cfg_GET_llid5_rpt_cfg(reg32)                     REG_FLD_GET(e_rpt_cfg_FLD_llid5_rpt_cfg, (reg32))
#define e_rpt_cfg_GET_llid4_rpt_cfg(reg32)                     REG_FLD_GET(e_rpt_cfg_FLD_llid4_rpt_cfg, (reg32))
#define e_rpt_cfg_GET_llid3_rpt_cfg(reg32)                     REG_FLD_GET(e_rpt_cfg_FLD_llid3_rpt_cfg, (reg32))
#define e_rpt_cfg_GET_llid2_rpt_cfg(reg32)                     REG_FLD_GET(e_rpt_cfg_FLD_llid2_rpt_cfg, (reg32))
#define e_rpt_cfg_GET_llid1_rpt_cfg(reg32)                     REG_FLD_GET(e_rpt_cfg_FLD_llid1_rpt_cfg, (reg32))
#define e_rpt_cfg_GET_llid0_rpt_cfg(reg32)                     REG_FLD_GET(e_rpt_cfg_FLD_llid0_rpt_cfg, (reg32))

#define e_rpt_qthld_cfg_GET_qthld_rwcmd(reg32)                 REG_FLD_GET(e_rpt_qthld_cfg_FLD_qthld_rwcmd, (reg32))
#define e_rpt_qthld_cfg_GET_qthld_rwcmd_done(reg32)            REG_FLD_GET(e_rpt_qthld_cfg_FLD_qthld_rwcmd_done, (reg32))
#define e_rpt_qthld_cfg_GET_qthld_value(reg32)                 REG_FLD_GET(e_rpt_qthld_cfg_FLD_qthld_value, (reg32))
#define e_rpt_qthld_cfg_GET_qthld_idx(reg32)                   REG_FLD_GET(e_rpt_qthld_cfg_FLD_qthld_idx, (reg32))
#define e_rpt_qthld_cfg_GET_rpt_llid_idx(reg32)                REG_FLD_GET(e_rpt_qthld_cfg_FLD_rpt_llid_idx, (reg32))
#define e_rpt_qthld_cfg_GET_queue_idx(reg32)                   REG_FLD_GET(e_rpt_qthld_cfg_FLD_queue_idx, (reg32))

#define e_local_time_GET_local_time(reg32)                     REG_FLD_GET(e_local_time_FLD_local_time, (reg32))

#define e_tod_sync_x_GET_tod_sync_x(reg32)                     REG_FLD_GET(e_tod_sync_x_FLD_tod_sync_x, (reg32))

#define e_tod_ltncy_GET_ingrs_latency(reg32)                   REG_FLD_GET(e_tod_ltncy_FLD_ingrs_latency, (reg32))
#define e_tod_ltncy_GET_egrs_latency(reg32)                    REG_FLD_GET(e_tod_ltncy_FLD_egrs_latency, (reg32))

#define p2p_tx_tag1_GET_p2p_tx_tagging_done(reg32)             REG_FLD_GET(p2p_tx_tag1_FLD_p2p_tx_tagging_done, (reg32))
#define p2p_tx_tag1_GET_p2p_tx_pkt_info(reg32)                 REG_FLD_GET(p2p_tx_tag1_FLD_p2p_tx_pkt_info, (reg32))
#define p2p_tx_tag1_GET_p2p_tx_tag_sec_l8(reg32)               REG_FLD_GET(p2p_tx_tag1_FLD_p2p_tx_tag_sec_l8, (reg32))

#define p2p_tx_tag2_GET_p2p_tx_tag_nsec_29_5(reg32)            REG_FLD_GET(p2p_tx_tag2_FLD_p2p_tx_tag_nsec_29_5, (reg32))

#define e_new_tod_p2p_offset_sec_l32_GET_new_tod_p2p_offset_sec_l32(reg32) REG_FLD_GET(e_new_tod_p2p_offset_sec_l32_FLD_new_tod_p2p_offset_sec_l32, (reg32))

#define e_new_tod_p2p_tod_offset_nsec_GET_new_tod_nsec(reg32)  REG_FLD_GET(e_new_tod_p2p_tod_offset_nsec_FLD_new_tod_nsec, (reg32))

#define e_tod_p2p_tod_sec_l32_GET_tod_p2p_sec_l32(reg32)       REG_FLD_GET(e_tod_p2p_tod_sec_l32_FLD_tod_p2p_sec_l32, (reg32))

#define e_tod_p2p_tod_nsec_GET_tod_p2p_nsec(reg32)             REG_FLD_GET(e_tod_p2p_tod_nsec_FLD_tod_p2p_nsec, (reg32))

#define e_tod_period_GET_tod_period(reg32)                     REG_FLD_GET(e_tod_period_FLD_tod_period, (reg32))

#define e_pwr_sv_cfg_GET_pwd_mode(reg32)                       REG_FLD_GET(e_pwr_sv_cfg_FLD_pwd_mode, (reg32))
#define e_pwr_sv_cfg_GET_timedrift_ignore(reg32)               REG_FLD_GET(e_pwr_sv_cfg_FLD_timedrift_ignore, (reg32))
#define e_pwr_sv_cfg_GET_onu_wakeup(reg32)                     REG_FLD_GET(e_pwr_sv_cfg_FLD_onu_wakeup, (reg32))
#define e_pwr_sv_cfg_GET_pwr_sv_start(reg32)                   REG_FLD_GET(e_pwr_sv_cfg_FLD_pwr_sv_start, (reg32))
#define e_pwr_sv_cfg_GET_pwd_mode_i(reg32)                     REG_FLD_GET(e_pwr_sv_cfg_FLD_pwd_mode_i, (reg32))
#define e_pwr_sv_cfg_GET_slp_duration_max_h(reg32)             REG_FLD_GET(e_pwr_sv_cfg_FLD_slp_duration_max_h, (reg32))

#define e_slp_durt_max_GET_slp_duration_max_l(reg32)           REG_FLD_GET(e_slp_durt_max_FLD_slp_duration_max_l, (reg32))

#define e_slp_duration_GET_slp_duration(reg32)                 REG_FLD_GET(e_slp_duration_FLD_slp_duration, (reg32))

#define e_act_duration_GET_act_duration(reg32)                 REG_FLD_GET(e_act_duration_FLD_act_duration, (reg32))

#define e_pwron_dly_GET_pwron_dly(reg32)                       REG_FLD_GET(e_pwron_dly_FLD_pwron_dly, (reg32))

#define e_slp_duration_i_GET_slp_duration_i(reg32)             REG_FLD_GET(e_slp_duration_i_FLD_slp_duration_i, (reg32))

#define e_txfetch_cfg_GET_tx_fetch_overhead(reg32)             REG_FLD_GET(e_txfetch_cfg_FLD_tx_fetch_overhead, (reg32))
#define e_txfetch_cfg_GET_tx_fetch_leadtime(reg32)             REG_FLD_GET(e_txfetch_cfg_FLD_tx_fetch_leadtime, (reg32))
#define e_txfetch_cfg_GET_tx_dma_leadtime(reg32)               REG_FLD_GET(e_txfetch_cfg_FLD_tx_dma_leadtime, (reg32))

#define e_sync_time_GET_sync_time_updte(reg32)                 REG_FLD_GET(e_sync_time_FLD_sync_time_updte, (reg32))
#define e_sync_time_GET_sync_time(reg32)                       REG_FLD_GET(e_sync_time_FLD_sync_time, (reg32))

#define e_tx_cal_cnst_GET_dscvr_gnt_len(reg32)                 REG_FLD_GET(e_tx_cal_cnst_FLD_dscvr_gnt_len, (reg32))
#define e_tx_cal_cnst_GET_fec_tail_grd(reg32)                  REG_FLD_GET(e_tx_cal_cnst_FLD_fec_tail_grd, (reg32))
#define e_tx_cal_cnst_GET_tail_grd(reg32)                      REG_FLD_GET(e_tx_cal_cnst_FLD_tail_grd, (reg32))
#define e_tx_cal_cnst_GET_default_ovrhd(reg32)                 REG_FLD_GET(e_tx_cal_cnst_FLD_default_ovrhd, (reg32))

#define e_laser_onoff_time_GET_laser_off_time(reg32)           REG_FLD_GET(e_laser_onoff_time_FLD_laser_off_time, (reg32))
#define e_laser_onoff_time_GET_laser_on_time(reg32)            REG_FLD_GET(e_laser_onoff_time_FLD_laser_on_time, (reg32))

#define e_grd_thrshld_GET_guard_thrshld(reg32)                 REG_FLD_GET(e_grd_thrshld_FLD_guard_thrshld, (reg32))

#define e_mpcp_timeout_intvl_GET_mpcp_timeout_intvl(reg32)     REG_FLD_GET(e_mpcp_timeout_intvl_FLD_mpcp_timeout_intvl, (reg32))

#define e_rpt_timeout_intvl_GET_rpt_timeout_intvl(reg32)       REG_FLD_GET(e_rpt_timeout_intvl_FLD_rpt_timeout_intvl, (reg32))

#define e_max_future_gnt_time_GET_max_future_gnt_time(reg32)   REG_FLD_GET(e_max_future_gnt_time_FLD_max_future_gnt_time, (reg32))

#define e_min_proc_time_GET_min_proc_time(reg32)               REG_FLD_GET(e_min_proc_time_FLD_min_proc_time, (reg32))

#define e_trx_adjust_time1_GET_tx_tmstp_adj(reg32)             REG_FLD_GET(e_trx_adjust_time1_FLD_tx_tmstp_adj, (reg32))
#define e_trx_adjust_time1_GET_tx_stm_adj(reg32)               REG_FLD_GET(e_trx_adjust_time1_FLD_tx_stm_adj, (reg32))

#define e_trx_adjust_time2_GET_tx_len_adj(reg32)               REG_FLD_GET(e_trx_adjust_time2_FLD_tx_len_adj, (reg32))
#define e_trx_adjust_time2_GET_rx_tmstp_adj(reg32)             REG_FLD_GET(e_trx_adjust_time2_FLD_rx_tmstp_adj, (reg32))

#define e_dbg_prb_sel_GET_probe_bit0_sel(reg32)                REG_FLD_GET(e_dbg_prb_sel_FLD_probe_bit0_sel, (reg32))
#define e_dbg_prb_sel_GET_probe_sel(reg32)                     REG_FLD_GET(e_dbg_prb_sel_FLD_probe_sel, (reg32))

#define e_dbg_prb_h32_GET_probe_h32(reg32)                     REG_FLD_GET(e_dbg_prb_h32_FLD_probe_h32, (reg32))

#define e_dbg_prb_l32_GET_probe_l32(reg32)                     REG_FLD_GET(e_dbg_prb_l32_FLD_probe_l32, (reg32))

#define e_rxmbi_eth_cnt_GET_rxmbi_eth_cnt(reg32)               REG_FLD_GET(e_rxmbi_eth_cnt_FLD_rxmbi_eth_cnt, (reg32))

#define e_rxmpi_eth_cnt_GET_rxmpi_eth_cnt(reg32)               REG_FLD_GET(e_rxmpi_eth_cnt_FLD_rxmpi_eth_cnt, (reg32))

#define e_txmbi_eth_cnt_GET_txmbi_eth_cnt(reg32)               REG_FLD_GET(e_txmbi_eth_cnt_FLD_txmbi_eth_cnt, (reg32))

#define e_txmpi_eth_cnt_GET_txmpi_eth_cnt(reg32)               REG_FLD_GET(e_txmpi_eth_cnt_FLD_txmpi_eth_cnt, (reg32))

#define e_oam_stat_GET_rx_oam_cnt(reg32)                       REG_FLD_GET(e_oam_stat_FLD_rx_oam_cnt, (reg32))
#define e_oam_stat_GET_tx_oam_cnt(reg32)                       REG_FLD_GET(e_oam_stat_FLD_tx_oam_cnt, (reg32))

#define e_mpcp_stat_GET_mpcp_err_cnt(reg32)                    REG_FLD_GET(e_mpcp_stat_FLD_mpcp_err_cnt, (reg32))
#define e_mpcp_stat_GET_mpcp_rgst_cnt(reg32)                   REG_FLD_GET(e_mpcp_stat_FLD_mpcp_rgst_cnt, (reg32))
#define e_mpcp_stat_GET_mpcp_dscv_gate_cnt(reg32)              REG_FLD_GET(e_mpcp_stat_FLD_mpcp_dscv_gate_cnt, (reg32))

#define e_mpcp_rgst_stat_GET_mpcp_rgst_req_cnt(reg32)          REG_FLD_GET(e_mpcp_rgst_stat_FLD_mpcp_rgst_req_cnt, (reg32))
#define e_mpcp_rgst_stat_GET_mpcp_rgst_ack_cnt(reg32)          REG_FLD_GET(e_mpcp_rgst_stat_FLD_mpcp_rgst_ack_cnt, (reg32))

#define e_gnt_pending_stat_GET_max_gnt_pending_cnt(reg32)      REG_FLD_GET(e_gnt_pending_stat_FLD_max_gnt_pending_cnt, (reg32))
#define e_gnt_pending_stat_GET_cur_gnt_pending_cnt(reg32)      REG_FLD_GET(e_gnt_pending_stat_FLD_cur_gnt_pending_cnt, (reg32))

#define e_gnt_length_stat_GET_max_gnt_length(reg32)            REG_FLD_GET(e_gnt_length_stat_FLD_max_gnt_length, (reg32))
#define e_gnt_length_stat_GET_min_gnt_length(reg32)            REG_FLD_GET(e_gnt_length_stat_FLD_min_gnt_length, (reg32))

#define e_gnt_type_stat_GET_b2b_gnt_cnt(reg32)                 REG_FLD_GET(e_gnt_type_stat_FLD_b2b_gnt_cnt, (reg32))
#define e_gnt_type_stat_GET_hdn_gnt_cnt(reg32)                 REG_FLD_GET(e_gnt_type_stat_FLD_hdn_gnt_cnt, (reg32))

#define e_time_drft_stat_GET_cur_time_drift(reg32)             REG_FLD_GET(e_time_drft_stat_FLD_cur_time_drift, (reg32))
#define e_time_drft_stat_GET_max_time_drift(reg32)             REG_FLD_GET(e_time_drft_stat_FLD_max_time_drift, (reg32))

#define e_llid0_gnt_stat_GET_llid0_gnt_cnt(reg32)              REG_FLD_GET(e_llid0_gnt_stat_FLD_llid0_gnt_cnt, (reg32))

#define e_llid1_gnt_stat_GET_llid1_gnt_cnt(reg32)              REG_FLD_GET(e_llid1_gnt_stat_FLD_llid1_gnt_cnt, (reg32))

#define e_llid2_gnt_stat_GET_llid2_gnt_cnt(reg32)              REG_FLD_GET(e_llid2_gnt_stat_FLD_llid2_gnt_cnt, (reg32))

#define e_llid3_gnt_stat_GET_llid3_gnt_cnt(reg32)              REG_FLD_GET(e_llid3_gnt_stat_FLD_llid3_gnt_cnt, (reg32))

#define e_llid4_gnt_stat_GET_llid4_gnt_cnt(reg32)              REG_FLD_GET(e_llid4_gnt_stat_FLD_llid4_gnt_cnt, (reg32))

#define e_llid5_gnt_stat_GET_llid5_gnt_cnt(reg32)              REG_FLD_GET(e_llid5_gnt_stat_FLD_llid5_gnt_cnt, (reg32))

#define e_llid6_gnt_stat_GET_llid6_gnt_cnt(reg32)              REG_FLD_GET(e_llid6_gnt_stat_FLD_llid6_gnt_cnt, (reg32))

#define e_llid7_gnt_stat_GET_llid7_gnt_cnt(reg32)              REG_FLD_GET(e_llid7_gnt_stat_FLD_llid7_gnt_cnt, (reg32))

#define e_snf_mpcp_oam_ctl_GET_snf_mpcp_cap_en(reg32)          REG_FLD_GET(e_snf_mpcp_oam_ctl_FLD_snf_mpcp_cap_en, (reg32))
#define e_snf_mpcp_oam_ctl_GET_snf_oam_cap_en(reg32)           REG_FLD_GET(e_snf_mpcp_oam_ctl_FLD_snf_oam_cap_en, (reg32))
#define e_snf_mpcp_oam_ctl_GET_snf_mpcp_oam_cnt_set(reg32)     REG_FLD_GET(e_snf_mpcp_oam_ctl_FLD_snf_mpcp_oam_cnt_set, (reg32))

#define e_rpt_adj_GET_rpt_bitmap_ctrl(reg32)                   REG_FLD_GET(e_rpt_adj_FLD_rpt_bitmap_ctrl, (reg32))
#define e_rpt_adj_GET_rpt_bitmap_set(reg32)                    REG_FLD_GET(e_rpt_adj_FLD_rpt_bitmap_set, (reg32))
#define e_rpt_adj_GET_rpt_qsize_fecoff_adj(reg32)              REG_FLD_GET(e_rpt_adj_FLD_rpt_qsize_fecoff_adj, (reg32))

#define e_rpt_cnt_GET_rpt_cnt(reg32)                           REG_FLD_GET(e_rpt_cnt_FLD_rpt_cnt, (reg32))

#define e_dyinggsp_w1_GET_dyinggsp_w1_len_type(reg32)          REG_FLD_GET(e_dyinggsp_w1_FLD_dyinggsp_w1_len_type, (reg32))

#define e_dyinggsp_w2_GET_dyinggsp_w2(reg32)                   REG_FLD_GET(e_dyinggsp_w2_FLD_dyinggsp_w2, (reg32))

#define e_dyinggsp_w3_GET_dyinggsp_w3(reg32)                   REG_FLD_GET(e_dyinggsp_w3_FLD_dyinggsp_w3, (reg32))

#define e_dyinggsp_w4_GET_dyinggsp_w4(reg32)                   REG_FLD_GET(e_dyinggsp_w4_FLD_dyinggsp_w4, (reg32))

#define e_dyinggsp_w5_GET_dyinggsp_w5(reg32)                   REG_FLD_GET(e_dyinggsp_w5_FLD_dyinggsp_w5, (reg32))

#define e_dyinggsp_w6_GET_dyinggsp_w6(reg32)                   REG_FLD_GET(e_dyinggsp_w6_FLD_dyinggsp_w6, (reg32))

#define e_dyinggsp_w7_GET_dyinggsp_w7(reg32)                   REG_FLD_GET(e_dyinggsp_w7_FLD_dyinggsp_w7, (reg32))

#define e_dyinggsp_w8_GET_dyinggsp_w8(reg32)                   REG_FLD_GET(e_dyinggsp_w8_FLD_dyinggsp_w8, (reg32))

#define e_dyinggsp_w9_GET_dyinggsp_w9(reg32)                   REG_FLD_GET(e_dyinggsp_w9_FLD_dyinggsp_w9, (reg32))

#define e_dyinggsp_w10_GET_dyinggsp_w10(reg32)                 REG_FLD_GET(e_dyinggsp_w10_FLD_dyinggsp_w10, (reg32))

#define e_dyinggsp_w11_GET_dyinggsp_w11(reg32)                 REG_FLD_GET(e_dyinggsp_w11_FLD_dyinggsp_w11, (reg32))

#define e_dyinggsp_w12_GET_dyinggsp_w12(reg32)                 REG_FLD_GET(e_dyinggsp_w12_FLD_dyinggsp_w12, (reg32))

#define e_oam_kpalv_w1_GET_oam_kpalv_w1(reg32)                 REG_FLD_GET(e_oam_kpalv_w1_FLD_oam_kpalv_w1, (reg32))

#define e_oam_kpalv_w2_GET_oam_kpalv_w2(reg32)                 REG_FLD_GET(e_oam_kpalv_w2_FLD_oam_kpalv_w2, (reg32))

#define e_oam_kpalv_w3_GET_oam_kpalv_w3(reg32)                 REG_FLD_GET(e_oam_kpalv_w3_FLD_oam_kpalv_w3, (reg32))

#define e_oam_kpalv_w4_GET_oam_kpalv_w4(reg32)                 REG_FLD_GET(e_oam_kpalv_w4_FLD_oam_kpalv_w4, (reg32))

#define e_oam_kpalv_w5_GET_oam_kpalv_w5(reg32)                 REG_FLD_GET(e_oam_kpalv_w5_FLD_oam_kpalv_w5, (reg32))

#define e_oam_kpalv_w6_GET_oam_kpalv_w6(reg32)                 REG_FLD_GET(e_oam_kpalv_w6_FLD_oam_kpalv_w6, (reg32))

#define e_oam_kpalv_w7_GET_oam_kpalv_w7(reg32)                 REG_FLD_GET(e_oam_kpalv_w7_FLD_oam_kpalv_w7, (reg32))

#define e_oam_kpalv_w8_GET_oam_kpalv_w8(reg32)                 REG_FLD_GET(e_oam_kpalv_w8_FLD_oam_kpalv_w8, (reg32))

#define e_oam_kpalv_w9_GET_oam_kpalv_w9(reg32)                 REG_FLD_GET(e_oam_kpalv_w9_FLD_oam_kpalv_w9, (reg32))

#define e_oam_kpalv_w10_GET_oam_kpalv_w10(reg32)               REG_FLD_GET(e_oam_kpalv_w10_FLD_oam_kpalv_w10, (reg32))

#define e_oam_kpalv_w11_GET_oam_kpalv_w11(reg32)               REG_FLD_GET(e_oam_kpalv_w11_FLD_oam_kpalv_w11, (reg32))

#define e_oam_kpalv_w12_GET_oam_kpalv_w12(reg32)               REG_FLD_GET(e_oam_kpalv_w12_FLD_oam_kpalv_w12, (reg32))

#define e_oam_kpalv_ctrl_GET_oam_kpalv_interval(reg32)         REG_FLD_GET(e_oam_kpalv_ctrl_FLD_oam_kpalv_interval, (reg32))
#define e_oam_kpalv_ctrl_GET_oam_kpalv_sw_trig(reg32)          REG_FLD_GET(e_oam_kpalv_ctrl_FLD_oam_kpalv_sw_trig, (reg32))
#define e_oam_kpalv_ctrl_GET_oam_kpalv_sw_cfg(reg32)           REG_FLD_GET(e_oam_kpalv_ctrl_FLD_oam_kpalv_sw_cfg, (reg32))
#define e_oam_kpalv_ctrl_GET_oam_kpalv_en(reg32)               REG_FLD_GET(e_oam_kpalv_ctrl_FLD_oam_kpalv_en, (reg32))

#define e_tod_1pps_ctrl_GET_tod_1pps_width_ctrl(reg32)         REG_FLD_GET(e_tod_1pps_ctrl_FLD_tod_1pps_width_ctrl, (reg32))

#define e_short_pkt_ctrl_GET_short_pkt_size_ctrl(reg32)        REG_FLD_GET(e_short_pkt_ctrl_FLD_short_pkt_size_ctrl, (reg32))

#define e_sniff_sp_tag_GET_sniffer_sp_tag(reg32)               REG_FLD_GET(e_sniff_sp_tag_FLD_sniffer_sp_tag, (reg32))

#define e_glb_cfg_SET_sniffer_mode(reg32, val)                 REG_FLD_SET(e_glb_cfg_FLD_sniffer_mode, (reg32), (val))
#define e_glb_cfg_SET_txoam_favor(reg32, val)                  REG_FLD_SET(e_glb_cfg_FLD_txoam_favor, (reg32), (val))
#define e_glb_cfg_SET_burst_en_dly(reg32, val)                 REG_FLD_SET(e_glb_cfg_FLD_burst_en_dly, (reg32), (val))
#define e_glb_cfg_SET_discv_burst_en(reg32, val)               REG_FLD_SET(e_glb_cfg_FLD_discv_burst_en, (reg32), (val))
#define e_glb_cfg_SET_mpcp_fwd(reg32, val)                     REG_FLD_SET(e_glb_cfg_FLD_mpcp_fwd, (reg32), (val))
#define e_glb_cfg_SET_bcst_llid_m1_drop(reg32, val)            REG_FLD_SET(e_glb_cfg_FLD_bcst_llid_m1_drop, (reg32), (val))
#define e_glb_cfg_SET_bcst_llid_m0_drop(reg32, val)            REG_FLD_SET(e_glb_cfg_FLD_bcst_llid_m0_drop, (reg32), (val))
#define e_glb_cfg_SET_mcst_llid_drop(reg32, val)               REG_FLD_SET(e_glb_cfg_FLD_mcst_llid_drop, (reg32), (val))
#define e_glb_cfg_SET_all_unicast_llid_pkt_fwd(reg32, val)     REG_FLD_SET(e_glb_cfg_FLD_all_unicast_llid_pkt_fwd, (reg32), (val))
#define e_glb_cfg_SET_fcs_err_fwd(reg32, val)                  REG_FLD_SET(e_glb_cfg_FLD_fcs_err_fwd, (reg32), (val))
#define e_glb_cfg_SET_llid_crc8_err_fwd(reg32, val)            REG_FLD_SET(e_glb_cfg_FLD_llid_crc8_err_fwd, (reg32), (val))
#define e_glb_cfg_SET_rxmpi_stop(reg32, val)                   REG_FLD_SET(e_glb_cfg_FLD_rxmpi_stop, (reg32), (val))
#define e_glb_cfg_SET_txmpi_stop(reg32, val)                   REG_FLD_SET(e_glb_cfg_FLD_txmpi_stop, (reg32), (val))
#define e_glb_cfg_SET_phy_pwr_down(reg32, val)                 REG_FLD_SET(e_glb_cfg_FLD_phy_pwr_down, (reg32), (val))
#define e_glb_cfg_SET_rx_nml_gate_fwd(reg32, val)              REG_FLD_SET(e_glb_cfg_FLD_rx_nml_gate_fwd, (reg32), (val))
#define e_glb_cfg_SET_rxmbi_stop(reg32, val)                   REG_FLD_SET(e_glb_cfg_FLD_rxmbi_stop, (reg32), (val))
#define e_glb_cfg_SET_txmbi_stop(reg32, val)                   REG_FLD_SET(e_glb_cfg_FLD_txmbi_stop, (reg32), (val))
#define e_glb_cfg_SET_chk_all_gnt_mode(reg32, val)             REG_FLD_SET(e_glb_cfg_FLD_chk_all_gnt_mode, (reg32), (val))
#define e_glb_cfg_SET_loc_cnt_sync_method(reg32, val)          REG_FLD_SET(e_glb_cfg_FLD_loc_cnt_sync_method, (reg32), (val))
#define e_glb_cfg_SET_tx_default_rpt(reg32, val)               REG_FLD_SET(e_glb_cfg_FLD_tx_default_rpt, (reg32), (val))
#define e_glb_cfg_SET_epon_mac_sw_rst(reg32, val)              REG_FLD_SET(e_glb_cfg_FLD_epon_mac_sw_rst, (reg32), (val))
#define e_glb_cfg_SET_epon_oam_cal_in_eth(reg32, val)          REG_FLD_SET(e_glb_cfg_FLD_epon_oam_cal_in_eth, (reg32), (val))
#define e_glb_cfg_SET_epon_mac_lpbk_en(reg32, val)             REG_FLD_SET(e_glb_cfg_FLD_epon_mac_lpbk_en, (reg32), (val))
#define e_glb_cfg_SET_rpt_txpri_ctrl(reg32, val)               REG_FLD_SET(e_glb_cfg_FLD_rpt_txpri_ctrl, (reg32), (val))
#define e_glb_cfg_SET_mode_sel(reg32, val)                     REG_FLD_SET(e_glb_cfg_FLD_mode_sel, (reg32), (val))

#define e_int_status_SET_sniff_fifo_ovrun_int(reg32, val)      REG_FLD_SET(e_int_status_FLD_sniff_fifo_ovrun_int, (reg32), (val))
#define e_int_status_SET_reg_ack_done_int(reg32, val)          REG_FLD_SET(e_int_status_FLD_reg_ack_done_int, (reg32), (val))
#define e_int_status_SET_reg_req_done_int(reg32, val)          REG_FLD_SET(e_int_status_FLD_reg_req_done_int, (reg32), (val))
#define e_int_status_SET_reorder1_gnt_int(reg32, val)          REG_FLD_SET(e_int_status_FLD_reorder1_gnt_int, (reg32), (val))
#define e_int_status_SET_b2b_gnt_int(reg32, val)               REG_FLD_SET(e_int_status_FLD_b2b_gnt_int, (reg32), (val))
#define e_int_status_SET_hidn_gnt_int(reg32, val)              REG_FLD_SET(e_int_status_FLD_hidn_gnt_int, (reg32), (val))
#define e_int_status_SET_ps_early_wakeup_int(reg32, val)       REG_FLD_SET(e_int_status_FLD_ps_early_wakeup_int, (reg32), (val))
#define e_int_status_SET_rx_sleep_allow_int(reg32, val)        REG_FLD_SET(e_int_status_FLD_rx_sleep_allow_int, (reg32), (val))
#define e_int_status_SET_ps_wakeup_int(reg32, val)             REG_FLD_SET(e_int_status_FLD_ps_wakeup_int, (reg32), (val))
#define e_int_status_SET_ps_sleep_int(reg32, val)              REG_FLD_SET(e_int_status_FLD_ps_sleep_int, (reg32), (val))
#define e_int_status_SET_txfifo_udrun_int(reg32, val)          REG_FLD_SET(e_int_status_FLD_txfifo_udrun_int, (reg32), (val))
#define e_int_status_SET_rpt_overintvl_int(reg32, val)         REG_FLD_SET(e_int_status_FLD_rpt_overintvl_int, (reg32), (val))
#define e_int_status_SET_mpcp_timeout_int(reg32, val)          REG_FLD_SET(e_int_status_FLD_mpcp_timeout_int, (reg32), (val))
#define e_int_status_SET_timedrft_int(reg32, val)              REG_FLD_SET(e_int_status_FLD_timedrft_int, (reg32), (val))
#define e_int_status_SET_tod_1pps_int(reg32, val)              REG_FLD_SET(e_int_status_FLD_tod_1pps_int, (reg32), (val))
#define e_int_status_SET_tod_updt_int(reg32, val)              REG_FLD_SET(e_int_status_FLD_tod_updt_int, (reg32), (val))
#define e_int_status_SET_ptp_msg_tx_int(reg32, val)            REG_FLD_SET(e_int_status_FLD_ptp_msg_tx_int, (reg32), (val))
#define e_int_status_SET_gnt_buf_ovrrun_int(reg32, val)        REG_FLD_SET(e_int_status_FLD_gnt_buf_ovrrun_int, (reg32), (val))
#define e_int_status_SET_llid7_rcv_rgst_int(reg32, val)        REG_FLD_SET(e_int_status_FLD_llid7_rcv_rgst_int, (reg32), (val))
#define e_int_status_SET_llid6_rcv_rgst_int(reg32, val)        REG_FLD_SET(e_int_status_FLD_llid6_rcv_rgst_int, (reg32), (val))
#define e_int_status_SET_llid5_rcv_rgst_int(reg32, val)        REG_FLD_SET(e_int_status_FLD_llid5_rcv_rgst_int, (reg32), (val))
#define e_int_status_SET_llid4_rcv_rgst_int(reg32, val)        REG_FLD_SET(e_int_status_FLD_llid4_rcv_rgst_int, (reg32), (val))
#define e_int_status_SET_llid3_rcv_rgst_int(reg32, val)        REG_FLD_SET(e_int_status_FLD_llid3_rcv_rgst_int, (reg32), (val))
#define e_int_status_SET_llid2_rcv_rgst_int(reg32, val)        REG_FLD_SET(e_int_status_FLD_llid2_rcv_rgst_int, (reg32), (val))
#define e_int_status_SET_llid1_rcv_rgst_int(reg32, val)        REG_FLD_SET(e_int_status_FLD_llid1_rcv_rgst_int, (reg32), (val))
#define e_int_status_SET_llid0_rcv_rgst_int(reg32, val)        REG_FLD_SET(e_int_status_FLD_llid0_rcv_rgst_int, (reg32), (val))
#define e_int_status_SET_rcv_dscvry_gate_int(reg32, val)       REG_FLD_SET(e_int_status_FLD_rcv_dscvry_gate_int, (reg32), (val))

#define e_int_en_SET_sniff_fifo_ovrun_en(reg32, val)           REG_FLD_SET(e_int_en_FLD_sniff_fifo_ovrun_en, (reg32), (val))
#define e_int_en_SET_reg_ack_done_en(reg32, val)               REG_FLD_SET(e_int_en_FLD_reg_ack_done_en, (reg32), (val))
#define e_int_en_SET_reg_req_done_en(reg32, val)               REG_FLD_SET(e_int_en_FLD_reg_req_done_en, (reg32), (val))
#define e_int_en_SET_reorder1_gnt_en(reg32, val)               REG_FLD_SET(e_int_en_FLD_reorder1_gnt_en, (reg32), (val))
#define e_int_en_SET_b2b_gnt_en(reg32, val)                    REG_FLD_SET(e_int_en_FLD_b2b_gnt_en, (reg32), (val))
#define e_int_en_SET_hidn_gnt_en(reg32, val)                   REG_FLD_SET(e_int_en_FLD_hidn_gnt_en, (reg32), (val))
#define e_int_en_SET_ps_early_wakeup_en(reg32, val)            REG_FLD_SET(e_int_en_FLD_ps_early_wakeup_en, (reg32), (val))
#define e_int_en_SET_rx_sleep_allow_en(reg32, val)             REG_FLD_SET(e_int_en_FLD_rx_sleep_allow_en, (reg32), (val))
#define e_int_en_SET_ps_wakeup_en(reg32, val)                  REG_FLD_SET(e_int_en_FLD_ps_wakeup_en, (reg32), (val))
#define e_int_en_SET_ps_sleep_en(reg32, val)                   REG_FLD_SET(e_int_en_FLD_ps_sleep_en, (reg32), (val))
#define e_int_en_SET_txfifo_udrun_en(reg32, val)               REG_FLD_SET(e_int_en_FLD_txfifo_udrun_en, (reg32), (val))
#define e_int_en_SET_rpt_overintvl_en(reg32, val)              REG_FLD_SET(e_int_en_FLD_rpt_overintvl_en, (reg32), (val))
#define e_int_en_SET_mpcp_timeout_en(reg32, val)               REG_FLD_SET(e_int_en_FLD_mpcp_timeout_en, (reg32), (val))
#define e_int_en_SET_timedrft_en(reg32, val)                   REG_FLD_SET(e_int_en_FLD_timedrft_en, (reg32), (val))
#define e_int_en_SET_tod_1pps_en(reg32, val)                   REG_FLD_SET(e_int_en_FLD_tod_1pps_en, (reg32), (val))
#define e_int_en_SET_tod_updt_en(reg32, val)                   REG_FLD_SET(e_int_en_FLD_tod_updt_en, (reg32), (val))
#define e_int_en_SET_ptp_msg_tx_en(reg32, val)                 REG_FLD_SET(e_int_en_FLD_ptp_msg_tx_en, (reg32), (val))
#define e_int_en_SET_gnt_buf_ovrrun_en(reg32, val)             REG_FLD_SET(e_int_en_FLD_gnt_buf_ovrrun_en, (reg32), (val))
#define e_int_en_SET_llid7_rcv_rgst_en(reg32, val)             REG_FLD_SET(e_int_en_FLD_llid7_rcv_rgst_en, (reg32), (val))
#define e_int_en_SET_llid6_rcv_rgst_en(reg32, val)             REG_FLD_SET(e_int_en_FLD_llid6_rcv_rgst_en, (reg32), (val))
#define e_int_en_SET_llid5_rcv_rgst_en(reg32, val)             REG_FLD_SET(e_int_en_FLD_llid5_rcv_rgst_en, (reg32), (val))
#define e_int_en_SET_llid4_rcv_rgst_en(reg32, val)             REG_FLD_SET(e_int_en_FLD_llid4_rcv_rgst_en, (reg32), (val))
#define e_int_en_SET_llid3_rcv_rgst_en(reg32, val)             REG_FLD_SET(e_int_en_FLD_llid3_rcv_rgst_en, (reg32), (val))
#define e_int_en_SET_llid2_rcv_rgst_en(reg32, val)             REG_FLD_SET(e_int_en_FLD_llid2_rcv_rgst_en, (reg32), (val))
#define e_int_en_SET_llid1_rcv_rgst_en(reg32, val)             REG_FLD_SET(e_int_en_FLD_llid1_rcv_rgst_en, (reg32), (val))
#define e_int_en_SET_llid0_rcv_rgst_en(reg32, val)             REG_FLD_SET(e_int_en_FLD_llid0_rcv_rgst_en, (reg32), (val))
#define e_int_en_SET_rcv_dscvry_gate_en(reg32, val)            REG_FLD_SET(e_int_en_FLD_rcv_dscvry_gate_en, (reg32), (val))

#define e_rpt_mpcp_timeout_llid_idx_SET_llid7_mpcp_tmo(reg32, val) REG_FLD_SET(e_rpt_mpcp_timeout_llid_idx_FLD_llid7_mpcp_tmo, (reg32), (val))
#define e_rpt_mpcp_timeout_llid_idx_SET_llid6_mpcp_tmo(reg32, val) REG_FLD_SET(e_rpt_mpcp_timeout_llid_idx_FLD_llid6_mpcp_tmo, (reg32), (val))
#define e_rpt_mpcp_timeout_llid_idx_SET_llid5_mpcp_tmo(reg32, val) REG_FLD_SET(e_rpt_mpcp_timeout_llid_idx_FLD_llid5_mpcp_tmo, (reg32), (val))
#define e_rpt_mpcp_timeout_llid_idx_SET_llid4_mpcp_tmo(reg32, val) REG_FLD_SET(e_rpt_mpcp_timeout_llid_idx_FLD_llid4_mpcp_tmo, (reg32), (val))
#define e_rpt_mpcp_timeout_llid_idx_SET_llid3_mpcp_tmo(reg32, val) REG_FLD_SET(e_rpt_mpcp_timeout_llid_idx_FLD_llid3_mpcp_tmo, (reg32), (val))
#define e_rpt_mpcp_timeout_llid_idx_SET_llid2_mpcp_tmo(reg32, val) REG_FLD_SET(e_rpt_mpcp_timeout_llid_idx_FLD_llid2_mpcp_tmo, (reg32), (val))
#define e_rpt_mpcp_timeout_llid_idx_SET_llid1_mpcp_tmo(reg32, val) REG_FLD_SET(e_rpt_mpcp_timeout_llid_idx_FLD_llid1_mpcp_tmo, (reg32), (val))
#define e_rpt_mpcp_timeout_llid_idx_SET_llid0_mpcp_tmo(reg32, val) REG_FLD_SET(e_rpt_mpcp_timeout_llid_idx_FLD_llid0_mpcp_tmo, (reg32), (val))
#define e_rpt_mpcp_timeout_llid_idx_SET_llid7_rpt_tmo(reg32, val) REG_FLD_SET(e_rpt_mpcp_timeout_llid_idx_FLD_llid7_rpt_tmo, (reg32), (val))
#define e_rpt_mpcp_timeout_llid_idx_SET_llid6_rpt_tmo(reg32, val) REG_FLD_SET(e_rpt_mpcp_timeout_llid_idx_FLD_llid6_rpt_tmo, (reg32), (val))
#define e_rpt_mpcp_timeout_llid_idx_SET_llid5_rpt_tmo(reg32, val) REG_FLD_SET(e_rpt_mpcp_timeout_llid_idx_FLD_llid5_rpt_tmo, (reg32), (val))
#define e_rpt_mpcp_timeout_llid_idx_SET_llid4_rpt_tmo(reg32, val) REG_FLD_SET(e_rpt_mpcp_timeout_llid_idx_FLD_llid4_rpt_tmo, (reg32), (val))
#define e_rpt_mpcp_timeout_llid_idx_SET_llid3_rpt_tmo(reg32, val) REG_FLD_SET(e_rpt_mpcp_timeout_llid_idx_FLD_llid3_rpt_tmo, (reg32), (val))
#define e_rpt_mpcp_timeout_llid_idx_SET_llid2_rpt_tmo(reg32, val) REG_FLD_SET(e_rpt_mpcp_timeout_llid_idx_FLD_llid2_rpt_tmo, (reg32), (val))
#define e_rpt_mpcp_timeout_llid_idx_SET_llid1_rpt_tmo(reg32, val) REG_FLD_SET(e_rpt_mpcp_timeout_llid_idx_FLD_llid1_rpt_tmo, (reg32), (val))
#define e_rpt_mpcp_timeout_llid_idx_SET_llid0_rpt_tmo(reg32, val) REG_FLD_SET(e_rpt_mpcp_timeout_llid_idx_FLD_llid0_rpt_tmo, (reg32), (val))

#define e_dyinggsp_cfg_SET_hw_dying_gasp_en(reg32, val)        REG_FLD_SET(e_dyinggsp_cfg_FLD_hw_dying_gasp_en, (reg32), (val))
#define e_dyinggsp_cfg_SET_dying_gasp_code(reg32, val)         REG_FLD_SET(e_dyinggsp_cfg_FLD_dying_gasp_code, (reg32), (val))
#define e_dyinggsp_cfg_SET_sw_init_dying_gasp(reg32, val)      REG_FLD_SET(e_dyinggsp_cfg_FLD_sw_init_dying_gasp, (reg32), (val))
#define e_dyinggsp_cfg_SET_dygsp_num_of_times(reg32, val)      REG_FLD_SET(e_dyinggsp_cfg_FLD_dygsp_num_of_times, (reg32), (val))
#define e_dyinggsp_cfg_SET_dying_gasp_flags(reg32, val)        REG_FLD_SET(e_dyinggsp_cfg_FLD_dying_gasp_flags, (reg32), (val))

#define e_pending_gnt_num_SET_echoed_pending_gnt(reg32, val)   REG_FLD_SET(e_pending_gnt_num_FLD_echoed_pending_gnt, (reg32), (val))
#define e_pending_gnt_num_SET_pending_gnt_num(reg32, val)      REG_FLD_SET(e_pending_gnt_num_FLD_pending_gnt_num, (reg32), (val))

#define e_llid0_3_cfg_SET_llid3_txfec_en(reg32, val)           REG_FLD_SET(e_llid0_3_cfg_FLD_llid3_txfec_en, (reg32), (val))
#define e_llid0_3_cfg_SET_llid3_dcrypt_en(reg32, val)          REG_FLD_SET(e_llid0_3_cfg_FLD_llid3_dcrypt_en, (reg32), (val))
#define e_llid0_3_cfg_SET_llid3_dcrypt_mode(reg32, val)        REG_FLD_SET(e_llid0_3_cfg_FLD_llid3_dcrypt_mode, (reg32), (val))
#define e_llid0_3_cfg_SET_llid3_oam_lpbk_en(reg32, val)        REG_FLD_SET(e_llid0_3_cfg_FLD_llid3_oam_lpbk_en, (reg32), (val))
#define e_llid0_3_cfg_SET_llid2_txfec_en(reg32, val)           REG_FLD_SET(e_llid0_3_cfg_FLD_llid2_txfec_en, (reg32), (val))
#define e_llid0_3_cfg_SET_llid2_dcrypt_en(reg32, val)          REG_FLD_SET(e_llid0_3_cfg_FLD_llid2_dcrypt_en, (reg32), (val))
#define e_llid0_3_cfg_SET_llid2_dcrypt_mode(reg32, val)        REG_FLD_SET(e_llid0_3_cfg_FLD_llid2_dcrypt_mode, (reg32), (val))
#define e_llid0_3_cfg_SET_llid2_oam_lpbk_en(reg32, val)        REG_FLD_SET(e_llid0_3_cfg_FLD_llid2_oam_lpbk_en, (reg32), (val))
#define e_llid0_3_cfg_SET_llid1_txfec_en(reg32, val)           REG_FLD_SET(e_llid0_3_cfg_FLD_llid1_txfec_en, (reg32), (val))
#define e_llid0_3_cfg_SET_llid1_dcrypt_en(reg32, val)          REG_FLD_SET(e_llid0_3_cfg_FLD_llid1_dcrypt_en, (reg32), (val))
#define e_llid0_3_cfg_SET_llid1_dcrypt_mode(reg32, val)        REG_FLD_SET(e_llid0_3_cfg_FLD_llid1_dcrypt_mode, (reg32), (val))
#define e_llid0_3_cfg_SET_llid1_oam_lpbk_en(reg32, val)        REG_FLD_SET(e_llid0_3_cfg_FLD_llid1_oam_lpbk_en, (reg32), (val))
#define e_llid0_3_cfg_SET_llid0_txfec_en(reg32, val)           REG_FLD_SET(e_llid0_3_cfg_FLD_llid0_txfec_en, (reg32), (val))
#define e_llid0_3_cfg_SET_llid0_dcrypt_en(reg32, val)          REG_FLD_SET(e_llid0_3_cfg_FLD_llid0_dcrypt_en, (reg32), (val))
#define e_llid0_3_cfg_SET_llid0_dcrypt_mode(reg32, val)        REG_FLD_SET(e_llid0_3_cfg_FLD_llid0_dcrypt_mode, (reg32), (val))
#define e_llid0_3_cfg_SET_llid0_oam_lpbk_en(reg32, val)        REG_FLD_SET(e_llid0_3_cfg_FLD_llid0_oam_lpbk_en, (reg32), (val))

#define e_llid4_7_cfg_SET_llid7_txfec_en(reg32, val)           REG_FLD_SET(e_llid4_7_cfg_FLD_llid7_txfec_en, (reg32), (val))
#define e_llid4_7_cfg_SET_llid7_dcrypt_en(reg32, val)          REG_FLD_SET(e_llid4_7_cfg_FLD_llid7_dcrypt_en, (reg32), (val))
#define e_llid4_7_cfg_SET_llid7_dcrypt_mode(reg32, val)        REG_FLD_SET(e_llid4_7_cfg_FLD_llid7_dcrypt_mode, (reg32), (val))
#define e_llid4_7_cfg_SET_llid7_oam_lpbk_en(reg32, val)        REG_FLD_SET(e_llid4_7_cfg_FLD_llid7_oam_lpbk_en, (reg32), (val))
#define e_llid4_7_cfg_SET_llid6_txfec_en(reg32, val)           REG_FLD_SET(e_llid4_7_cfg_FLD_llid6_txfec_en, (reg32), (val))
#define e_llid4_7_cfg_SET_llid6_dcrypt_en(reg32, val)          REG_FLD_SET(e_llid4_7_cfg_FLD_llid6_dcrypt_en, (reg32), (val))
#define e_llid4_7_cfg_SET_llid6_dcrypt_mode(reg32, val)        REG_FLD_SET(e_llid4_7_cfg_FLD_llid6_dcrypt_mode, (reg32), (val))
#define e_llid4_7_cfg_SET_llid6_oam_lpbk_en(reg32, val)        REG_FLD_SET(e_llid4_7_cfg_FLD_llid6_oam_lpbk_en, (reg32), (val))
#define e_llid4_7_cfg_SET_llid5_txfec_en(reg32, val)           REG_FLD_SET(e_llid4_7_cfg_FLD_llid5_txfec_en, (reg32), (val))
#define e_llid4_7_cfg_SET_llid5_dcrypt_en(reg32, val)          REG_FLD_SET(e_llid4_7_cfg_FLD_llid5_dcrypt_en, (reg32), (val))
#define e_llid4_7_cfg_SET_llid5_dcrypt_mode(reg32, val)        REG_FLD_SET(e_llid4_7_cfg_FLD_llid5_dcrypt_mode, (reg32), (val))
#define e_llid4_7_cfg_SET_llid5_oam_lpbk_en(reg32, val)        REG_FLD_SET(e_llid4_7_cfg_FLD_llid5_oam_lpbk_en, (reg32), (val))
#define e_llid4_7_cfg_SET_llid4_txfec_en(reg32, val)           REG_FLD_SET(e_llid4_7_cfg_FLD_llid4_txfec_en, (reg32), (val))
#define e_llid4_7_cfg_SET_llid4_dcrypt_en(reg32, val)          REG_FLD_SET(e_llid4_7_cfg_FLD_llid4_dcrypt_en, (reg32), (val))
#define e_llid4_7_cfg_SET_llid4_dcrypt_mode(reg32, val)        REG_FLD_SET(e_llid4_7_cfg_FLD_llid4_dcrypt_mode, (reg32), (val))
#define e_llid4_7_cfg_SET_llid4_oam_lpbk_en(reg32, val)        REG_FLD_SET(e_llid4_7_cfg_FLD_llid4_oam_lpbk_en, (reg32), (val))

#define e_llid_dscvry_ctrl_SET_mpcp_cmd(reg32, val)            REG_FLD_SET(e_llid_dscvry_ctrl_FLD_mpcp_cmd, (reg32), (val))
#define e_llid_dscvry_ctrl_SET_mpcp_cmd_done(reg32, val)       REG_FLD_SET(e_llid_dscvry_ctrl_FLD_mpcp_cmd_done, (reg32), (val))
#define e_llid_dscvry_ctrl_SET_rgstr_ack_flg(reg32, val)       REG_FLD_SET(e_llid_dscvry_ctrl_FLD_rgstr_ack_flg, (reg32), (val))
#define e_llid_dscvry_ctrl_SET_rgstr_req_flg(reg32, val)       REG_FLD_SET(e_llid_dscvry_ctrl_FLD_rgstr_req_flg, (reg32), (val))
#define e_llid_dscvry_ctrl_SET_tx_mpcp_llid_idx(reg32, val)    REG_FLD_SET(e_llid_dscvry_ctrl_FLD_tx_mpcp_llid_idx, (reg32), (val))

#define e_llid0_dscvry_sts_SET_llid0_dscvry_sts(reg32, val)    REG_FLD_SET(e_llid0_dscvry_sts_FLD_llid0_dscvry_sts, (reg32), (val))
#define e_llid0_dscvry_sts_SET_llid0_rgstr_flg_sts(reg32, val) REG_FLD_SET(e_llid0_dscvry_sts_FLD_llid0_rgstr_flg_sts, (reg32), (val))
#define e_llid0_dscvry_sts_SET_llid0_valid(reg32, val)         REG_FLD_SET(e_llid0_dscvry_sts_FLD_llid0_valid, (reg32), (val))
#define e_llid0_dscvry_sts_SET_llid0_value(reg32, val)         REG_FLD_SET(e_llid0_dscvry_sts_FLD_llid0_value, (reg32), (val))

#define e_llid1_dscvry_sts_SET_llid1_dscvry_sts(reg32, val)    REG_FLD_SET(e_llid1_dscvry_sts_FLD_llid1_dscvry_sts, (reg32), (val))
#define e_llid1_dscvry_sts_SET_llid1_rgstr_flg_sts(reg32, val) REG_FLD_SET(e_llid1_dscvry_sts_FLD_llid1_rgstr_flg_sts, (reg32), (val))
#define e_llid1_dscvry_sts_SET_llid1_valid(reg32, val)         REG_FLD_SET(e_llid1_dscvry_sts_FLD_llid1_valid, (reg32), (val))
#define e_llid1_dscvry_sts_SET_llid1_value(reg32, val)         REG_FLD_SET(e_llid1_dscvry_sts_FLD_llid1_value, (reg32), (val))

#define e_llid2_dscvry_sts_SET_llid2_dscvry_sts(reg32, val)    REG_FLD_SET(e_llid2_dscvry_sts_FLD_llid2_dscvry_sts, (reg32), (val))
#define e_llid2_dscvry_sts_SET_llid2_rgstr_flg_sts(reg32, val) REG_FLD_SET(e_llid2_dscvry_sts_FLD_llid2_rgstr_flg_sts, (reg32), (val))
#define e_llid2_dscvry_sts_SET_llid2_valid(reg32, val)         REG_FLD_SET(e_llid2_dscvry_sts_FLD_llid2_valid, (reg32), (val))
#define e_llid2_dscvry_sts_SET_llid2_value(reg32, val)         REG_FLD_SET(e_llid2_dscvry_sts_FLD_llid2_value, (reg32), (val))

#define e_llid3_dscvry_sts_SET_llid3_dscvry_sts(reg32, val)    REG_FLD_SET(e_llid3_dscvry_sts_FLD_llid3_dscvry_sts, (reg32), (val))
#define e_llid3_dscvry_sts_SET_llid3_rgstr_flg_sts(reg32, val) REG_FLD_SET(e_llid3_dscvry_sts_FLD_llid3_rgstr_flg_sts, (reg32), (val))
#define e_llid3_dscvry_sts_SET_llid3_valid(reg32, val)         REG_FLD_SET(e_llid3_dscvry_sts_FLD_llid3_valid, (reg32), (val))
#define e_llid3_dscvry_sts_SET_llid3_value(reg32, val)         REG_FLD_SET(e_llid3_dscvry_sts_FLD_llid3_value, (reg32), (val))

#define e_llid4_dscvry_sts_SET_llid4_dscvry_sts(reg32, val)    REG_FLD_SET(e_llid4_dscvry_sts_FLD_llid4_dscvry_sts, (reg32), (val))
#define e_llid4_dscvry_sts_SET_llid4_rgstr_flg_sts(reg32, val) REG_FLD_SET(e_llid4_dscvry_sts_FLD_llid4_rgstr_flg_sts, (reg32), (val))
#define e_llid4_dscvry_sts_SET_llid4_valid(reg32, val)         REG_FLD_SET(e_llid4_dscvry_sts_FLD_llid4_valid, (reg32), (val))
#define e_llid4_dscvry_sts_SET_llid4_value(reg32, val)         REG_FLD_SET(e_llid4_dscvry_sts_FLD_llid4_value, (reg32), (val))

#define e_llid5_dscvry_sts_SET_llid5_dscvry_sts(reg32, val)    REG_FLD_SET(e_llid5_dscvry_sts_FLD_llid5_dscvry_sts, (reg32), (val))
#define e_llid5_dscvry_sts_SET_llid5_rgstr_flg_sts(reg32, val) REG_FLD_SET(e_llid5_dscvry_sts_FLD_llid5_rgstr_flg_sts, (reg32), (val))
#define e_llid5_dscvry_sts_SET_llid5_valid(reg32, val)         REG_FLD_SET(e_llid5_dscvry_sts_FLD_llid5_valid, (reg32), (val))
#define e_llid5_dscvry_sts_SET_llid5_value(reg32, val)         REG_FLD_SET(e_llid5_dscvry_sts_FLD_llid5_value, (reg32), (val))

#define e_llid6_dscvry_sts_SET_llid6_dscvry_sts(reg32, val)    REG_FLD_SET(e_llid6_dscvry_sts_FLD_llid6_dscvry_sts, (reg32), (val))
#define e_llid6_dscvry_sts_SET_llid6_rgstr_flg_sts(reg32, val) REG_FLD_SET(e_llid6_dscvry_sts_FLD_llid6_rgstr_flg_sts, (reg32), (val))
#define e_llid6_dscvry_sts_SET_llid6_valid(reg32, val)         REG_FLD_SET(e_llid6_dscvry_sts_FLD_llid6_valid, (reg32), (val))
#define e_llid6_dscvry_sts_SET_llid6_value(reg32, val)         REG_FLD_SET(e_llid6_dscvry_sts_FLD_llid6_value, (reg32), (val))

#define e_llid7_dscvry_sts_SET_llid7_dscvry_sts(reg32, val)    REG_FLD_SET(e_llid7_dscvry_sts_FLD_llid7_dscvry_sts, (reg32), (val))
#define e_llid7_dscvry_sts_SET_llid7_rgstr_flg_sts(reg32, val) REG_FLD_SET(e_llid7_dscvry_sts_FLD_llid7_rgstr_flg_sts, (reg32), (val))
#define e_llid7_dscvry_sts_SET_llid7_valid(reg32, val)         REG_FLD_SET(e_llid7_dscvry_sts_FLD_llid7_valid, (reg32), (val))
#define e_llid7_dscvry_sts_SET_llid7_value(reg32, val)         REG_FLD_SET(e_llid7_dscvry_sts_FLD_llid7_value, (reg32), (val))

#define e_mac_addr_cfg_SET_mac_addr_rwcmd(reg32, val)          REG_FLD_SET(e_mac_addr_cfg_FLD_mac_addr_rwcmd, (reg32), (val))
#define e_mac_addr_cfg_SET_mac_addr_rwcmd_done(reg32, val)     REG_FLD_SET(e_mac_addr_cfg_FLD_mac_addr_rwcmd_done, (reg32), (val))
#define e_mac_addr_cfg_SET_mac_addr_llid_indx(reg32, val)      REG_FLD_SET(e_mac_addr_cfg_FLD_mac_addr_llid_indx, (reg32), (val))
#define e_mac_addr_cfg_SET_mac_addr_dw_idx(reg32, val)         REG_FLD_SET(e_mac_addr_cfg_FLD_mac_addr_dw_idx, (reg32), (val))

#define e_mac_addr_value_SET_mac_addr_value(reg32, val)        REG_FLD_SET(e_mac_addr_value_FLD_mac_addr_value, (reg32), (val))

#define e_security_key_cfg_SET_key_rwcmd(reg32, val)           REG_FLD_SET(e_security_key_cfg_FLD_key_rwcmd, (reg32), (val))
#define e_security_key_cfg_SET_key_rwcmd_done(reg32, val)      REG_FLD_SET(e_security_key_cfg_FLD_key_rwcmd_done, (reg32), (val))
#define e_security_key_cfg_SET_key_llid_index(reg32, val)      REG_FLD_SET(e_security_key_cfg_FLD_key_llid_index, (reg32), (val))
#define e_security_key_cfg_SET_key_idx(reg32, val)             REG_FLD_SET(e_security_key_cfg_FLD_key_idx, (reg32), (val))
#define e_security_key_cfg_SET_key_dw_indx(reg32, val)         REG_FLD_SET(e_security_key_cfg_FLD_key_dw_indx, (reg32), (val))

#define e_key_value_SET_key_value(reg32, val)                  REG_FLD_SET(e_key_value_FLD_key_value, (reg32), (val))

#define e_rpt_data_SET_df_rpt_data(reg32, val)                 REG_FLD_SET(e_rpt_data_FLD_df_rpt_data, (reg32), (val))

#define e_rpt_len_SET_df_rpt_fifo_clr(reg32, val)              REG_FLD_SET(e_rpt_len_FLD_df_rpt_fifo_clr, (reg32), (val))
#define e_rpt_len_SET_df_rpt_dt_len(reg32, val)                REG_FLD_SET(e_rpt_len_FLD_df_rpt_dt_len, (reg32), (val))

#define e_rpt_cfg_SET_qsize_fec_adj(reg32, val)                REG_FLD_SET(e_rpt_cfg_FLD_qsize_fec_adj, (reg32), (val))
#define e_rpt_cfg_SET_llid7_rpt_cfg(reg32, val)                REG_FLD_SET(e_rpt_cfg_FLD_llid7_rpt_cfg, (reg32), (val))
#define e_rpt_cfg_SET_llid6_rpt_cfg(reg32, val)                REG_FLD_SET(e_rpt_cfg_FLD_llid6_rpt_cfg, (reg32), (val))
#define e_rpt_cfg_SET_llid5_rpt_cfg(reg32, val)                REG_FLD_SET(e_rpt_cfg_FLD_llid5_rpt_cfg, (reg32), (val))
#define e_rpt_cfg_SET_llid4_rpt_cfg(reg32, val)                REG_FLD_SET(e_rpt_cfg_FLD_llid4_rpt_cfg, (reg32), (val))
#define e_rpt_cfg_SET_llid3_rpt_cfg(reg32, val)                REG_FLD_SET(e_rpt_cfg_FLD_llid3_rpt_cfg, (reg32), (val))
#define e_rpt_cfg_SET_llid2_rpt_cfg(reg32, val)                REG_FLD_SET(e_rpt_cfg_FLD_llid2_rpt_cfg, (reg32), (val))
#define e_rpt_cfg_SET_llid1_rpt_cfg(reg32, val)                REG_FLD_SET(e_rpt_cfg_FLD_llid1_rpt_cfg, (reg32), (val))
#define e_rpt_cfg_SET_llid0_rpt_cfg(reg32, val)                REG_FLD_SET(e_rpt_cfg_FLD_llid0_rpt_cfg, (reg32), (val))

#define e_rpt_qthld_cfg_SET_qthld_rwcmd(reg32, val)            REG_FLD_SET(e_rpt_qthld_cfg_FLD_qthld_rwcmd, (reg32), (val))
#define e_rpt_qthld_cfg_SET_qthld_rwcmd_done(reg32, val)       REG_FLD_SET(e_rpt_qthld_cfg_FLD_qthld_rwcmd_done, (reg32), (val))
#define e_rpt_qthld_cfg_SET_qthld_value(reg32, val)            REG_FLD_SET(e_rpt_qthld_cfg_FLD_qthld_value, (reg32), (val))
#define e_rpt_qthld_cfg_SET_qthld_idx(reg32, val)              REG_FLD_SET(e_rpt_qthld_cfg_FLD_qthld_idx, (reg32), (val))
#define e_rpt_qthld_cfg_SET_rpt_llid_idx(reg32, val)           REG_FLD_SET(e_rpt_qthld_cfg_FLD_rpt_llid_idx, (reg32), (val))
#define e_rpt_qthld_cfg_SET_queue_idx(reg32, val)              REG_FLD_SET(e_rpt_qthld_cfg_FLD_queue_idx, (reg32), (val))

#define e_local_time_SET_local_time(reg32, val)                REG_FLD_SET(e_local_time_FLD_local_time, (reg32), (val))

#define e_tod_sync_x_SET_tod_sync_x(reg32, val)                REG_FLD_SET(e_tod_sync_x_FLD_tod_sync_x, (reg32), (val))

#define e_tod_ltncy_SET_ingrs_latency(reg32, val)              REG_FLD_SET(e_tod_ltncy_FLD_ingrs_latency, (reg32), (val))
#define e_tod_ltncy_SET_egrs_latency(reg32, val)               REG_FLD_SET(e_tod_ltncy_FLD_egrs_latency, (reg32), (val))

#define p2p_tx_tag1_SET_p2p_tx_tagging_done(reg32, val)        REG_FLD_SET(p2p_tx_tag1_FLD_p2p_tx_tagging_done, (reg32), (val))
#define p2p_tx_tag1_SET_p2p_tx_pkt_info(reg32, val)            REG_FLD_SET(p2p_tx_tag1_FLD_p2p_tx_pkt_info, (reg32), (val))
#define p2p_tx_tag1_SET_p2p_tx_tag_sec_l8(reg32, val)          REG_FLD_SET(p2p_tx_tag1_FLD_p2p_tx_tag_sec_l8, (reg32), (val))

#define p2p_tx_tag2_SET_p2p_tx_tag_nsec_29_5(reg32, val)       REG_FLD_SET(p2p_tx_tag2_FLD_p2p_tx_tag_nsec_29_5, (reg32), (val))

#define e_new_tod_p2p_offset_sec_l32_SET_new_tod_p2p_offset_sec_l32(reg32, val) REG_FLD_SET(e_new_tod_p2p_offset_sec_l32_FLD_new_tod_p2p_offset_sec_l32, (reg32), (val))

#define e_new_tod_p2p_tod_offset_nsec_SET_new_tod_nsec(reg32, val) REG_FLD_SET(e_new_tod_p2p_tod_offset_nsec_FLD_new_tod_nsec, (reg32), (val))

#define e_tod_p2p_tod_sec_l32_SET_tod_p2p_sec_l32(reg32, val)  REG_FLD_SET(e_tod_p2p_tod_sec_l32_FLD_tod_p2p_sec_l32, (reg32), (val))

#define e_tod_p2p_tod_nsec_SET_tod_p2p_nsec(reg32, val)        REG_FLD_SET(e_tod_p2p_tod_nsec_FLD_tod_p2p_nsec, (reg32), (val))

#define e_tod_period_SET_tod_period(reg32, val)                REG_FLD_SET(e_tod_period_FLD_tod_period, (reg32), (val))

#define e_pwr_sv_cfg_SET_pwd_mode(reg32, val)                  REG_FLD_SET(e_pwr_sv_cfg_FLD_pwd_mode, (reg32), (val))
#define e_pwr_sv_cfg_SET_timedrift_ignore(reg32, val)          REG_FLD_SET(e_pwr_sv_cfg_FLD_timedrift_ignore, (reg32), (val))
#define e_pwr_sv_cfg_SET_onu_wakeup(reg32, val)                REG_FLD_SET(e_pwr_sv_cfg_FLD_onu_wakeup, (reg32), (val))
#define e_pwr_sv_cfg_SET_pwr_sv_start(reg32, val)              REG_FLD_SET(e_pwr_sv_cfg_FLD_pwr_sv_start, (reg32), (val))
#define e_pwr_sv_cfg_SET_pwd_mode_i(reg32, val)                REG_FLD_SET(e_pwr_sv_cfg_FLD_pwd_mode_i, (reg32), (val))
#define e_pwr_sv_cfg_SET_slp_duration_max_h(reg32, val)        REG_FLD_SET(e_pwr_sv_cfg_FLD_slp_duration_max_h, (reg32), (val))

#define e_slp_durt_max_SET_slp_duration_max_l(reg32, val)      REG_FLD_SET(e_slp_durt_max_FLD_slp_duration_max_l, (reg32), (val))

#define e_slp_duration_SET_slp_duration(reg32, val)            REG_FLD_SET(e_slp_duration_FLD_slp_duration, (reg32), (val))

#define e_act_duration_SET_act_duration(reg32, val)            REG_FLD_SET(e_act_duration_FLD_act_duration, (reg32), (val))

#define e_pwron_dly_SET_pwron_dly(reg32, val)                  REG_FLD_SET(e_pwron_dly_FLD_pwron_dly, (reg32), (val))

#define e_slp_duration_i_SET_slp_duration_i(reg32, val)        REG_FLD_SET(e_slp_duration_i_FLD_slp_duration_i, (reg32), (val))

#define e_txfetch_cfg_SET_tx_fetch_overhead(reg32, val)        REG_FLD_SET(e_txfetch_cfg_FLD_tx_fetch_overhead, (reg32), (val))
#define e_txfetch_cfg_SET_tx_fetch_leadtime(reg32, val)        REG_FLD_SET(e_txfetch_cfg_FLD_tx_fetch_leadtime, (reg32), (val))
#define e_txfetch_cfg_SET_tx_dma_leadtime(reg32, val)          REG_FLD_SET(e_txfetch_cfg_FLD_tx_dma_leadtime, (reg32), (val))

#define e_sync_time_SET_sync_time_updte(reg32, val)            REG_FLD_SET(e_sync_time_FLD_sync_time_updte, (reg32), (val))
#define e_sync_time_SET_sync_time(reg32, val)                  REG_FLD_SET(e_sync_time_FLD_sync_time, (reg32), (val))

#define e_tx_cal_cnst_SET_dscvr_gnt_len(reg32, val)            REG_FLD_SET(e_tx_cal_cnst_FLD_dscvr_gnt_len, (reg32), (val))
#define e_tx_cal_cnst_SET_fec_tail_grd(reg32, val)             REG_FLD_SET(e_tx_cal_cnst_FLD_fec_tail_grd, (reg32), (val))
#define e_tx_cal_cnst_SET_tail_grd(reg32, val)                 REG_FLD_SET(e_tx_cal_cnst_FLD_tail_grd, (reg32), (val))
#define e_tx_cal_cnst_SET_default_ovrhd(reg32, val)            REG_FLD_SET(e_tx_cal_cnst_FLD_default_ovrhd, (reg32), (val))

#define e_laser_onoff_time_SET_laser_off_time(reg32, val)      REG_FLD_SET(e_laser_onoff_time_FLD_laser_off_time, (reg32), (val))
#define e_laser_onoff_time_SET_laser_on_time(reg32, val)       REG_FLD_SET(e_laser_onoff_time_FLD_laser_on_time, (reg32), (val))

#define e_grd_thrshld_SET_guard_thrshld(reg32, val)            REG_FLD_SET(e_grd_thrshld_FLD_guard_thrshld, (reg32), (val))

#define e_mpcp_timeout_intvl_SET_mpcp_timeout_intvl(reg32, val) REG_FLD_SET(e_mpcp_timeout_intvl_FLD_mpcp_timeout_intvl, (reg32), (val))

#define e_rpt_timeout_intvl_SET_rpt_timeout_intvl(reg32, val)  REG_FLD_SET(e_rpt_timeout_intvl_FLD_rpt_timeout_intvl, (reg32), (val))

#define e_max_future_gnt_time_SET_max_future_gnt_time(reg32, val) REG_FLD_SET(e_max_future_gnt_time_FLD_max_future_gnt_time, (reg32), (val))

#define e_min_proc_time_SET_min_proc_time(reg32, val)          REG_FLD_SET(e_min_proc_time_FLD_min_proc_time, (reg32), (val))

#define e_trx_adjust_time1_SET_tx_tmstp_adj(reg32, val)        REG_FLD_SET(e_trx_adjust_time1_FLD_tx_tmstp_adj, (reg32), (val))
#define e_trx_adjust_time1_SET_tx_stm_adj(reg32, val)          REG_FLD_SET(e_trx_adjust_time1_FLD_tx_stm_adj, (reg32), (val))

#define e_trx_adjust_time2_SET_tx_len_adj(reg32, val)          REG_FLD_SET(e_trx_adjust_time2_FLD_tx_len_adj, (reg32), (val))
#define e_trx_adjust_time2_SET_rx_tmstp_adj(reg32, val)        REG_FLD_SET(e_trx_adjust_time2_FLD_rx_tmstp_adj, (reg32), (val))

#define e_dbg_prb_sel_SET_probe_bit0_sel(reg32, val)           REG_FLD_SET(e_dbg_prb_sel_FLD_probe_bit0_sel, (reg32), (val))
#define e_dbg_prb_sel_SET_probe_sel(reg32, val)                REG_FLD_SET(e_dbg_prb_sel_FLD_probe_sel, (reg32), (val))

#define e_dbg_prb_h32_SET_probe_h32(reg32, val)                REG_FLD_SET(e_dbg_prb_h32_FLD_probe_h32, (reg32), (val))

#define e_dbg_prb_l32_SET_probe_l32(reg32, val)                REG_FLD_SET(e_dbg_prb_l32_FLD_probe_l32, (reg32), (val))

#define e_rxmbi_eth_cnt_SET_rxmbi_eth_cnt(reg32, val)          REG_FLD_SET(e_rxmbi_eth_cnt_FLD_rxmbi_eth_cnt, (reg32), (val))

#define e_rxmpi_eth_cnt_SET_rxmpi_eth_cnt(reg32, val)          REG_FLD_SET(e_rxmpi_eth_cnt_FLD_rxmpi_eth_cnt, (reg32), (val))

#define e_txmbi_eth_cnt_SET_txmbi_eth_cnt(reg32, val)          REG_FLD_SET(e_txmbi_eth_cnt_FLD_txmbi_eth_cnt, (reg32), (val))

#define e_txmpi_eth_cnt_SET_txmpi_eth_cnt(reg32, val)          REG_FLD_SET(e_txmpi_eth_cnt_FLD_txmpi_eth_cnt, (reg32), (val))

#define e_oam_stat_SET_rx_oam_cnt(reg32, val)                  REG_FLD_SET(e_oam_stat_FLD_rx_oam_cnt, (reg32), (val))
#define e_oam_stat_SET_tx_oam_cnt(reg32, val)                  REG_FLD_SET(e_oam_stat_FLD_tx_oam_cnt, (reg32), (val))

#define e_mpcp_stat_SET_mpcp_err_cnt(reg32, val)               REG_FLD_SET(e_mpcp_stat_FLD_mpcp_err_cnt, (reg32), (val))
#define e_mpcp_stat_SET_mpcp_rgst_cnt(reg32, val)              REG_FLD_SET(e_mpcp_stat_FLD_mpcp_rgst_cnt, (reg32), (val))
#define e_mpcp_stat_SET_mpcp_dscv_gate_cnt(reg32, val)         REG_FLD_SET(e_mpcp_stat_FLD_mpcp_dscv_gate_cnt, (reg32), (val))

#define e_mpcp_rgst_stat_SET_mpcp_rgst_req_cnt(reg32, val)     REG_FLD_SET(e_mpcp_rgst_stat_FLD_mpcp_rgst_req_cnt, (reg32), (val))
#define e_mpcp_rgst_stat_SET_mpcp_rgst_ack_cnt(reg32, val)     REG_FLD_SET(e_mpcp_rgst_stat_FLD_mpcp_rgst_ack_cnt, (reg32), (val))

#define e_gnt_pending_stat_SET_max_gnt_pending_cnt(reg32, val) REG_FLD_SET(e_gnt_pending_stat_FLD_max_gnt_pending_cnt, (reg32), (val))
#define e_gnt_pending_stat_SET_cur_gnt_pending_cnt(reg32, val) REG_FLD_SET(e_gnt_pending_stat_FLD_cur_gnt_pending_cnt, (reg32), (val))

#define e_gnt_length_stat_SET_max_gnt_length(reg32, val)       REG_FLD_SET(e_gnt_length_stat_FLD_max_gnt_length, (reg32), (val))
#define e_gnt_length_stat_SET_min_gnt_length(reg32, val)       REG_FLD_SET(e_gnt_length_stat_FLD_min_gnt_length, (reg32), (val))

#define e_gnt_type_stat_SET_b2b_gnt_cnt(reg32, val)            REG_FLD_SET(e_gnt_type_stat_FLD_b2b_gnt_cnt, (reg32), (val))
#define e_gnt_type_stat_SET_hdn_gnt_cnt(reg32, val)            REG_FLD_SET(e_gnt_type_stat_FLD_hdn_gnt_cnt, (reg32), (val))

#define e_time_drft_stat_SET_cur_time_drift(reg32, val)        REG_FLD_SET(e_time_drft_stat_FLD_cur_time_drift, (reg32), (val))
#define e_time_drft_stat_SET_max_time_drift(reg32, val)        REG_FLD_SET(e_time_drft_stat_FLD_max_time_drift, (reg32), (val))

#define e_llid0_gnt_stat_SET_llid0_gnt_cnt(reg32, val)         REG_FLD_SET(e_llid0_gnt_stat_FLD_llid0_gnt_cnt, (reg32), (val))

#define e_llid1_gnt_stat_SET_llid1_gnt_cnt(reg32, val)         REG_FLD_SET(e_llid1_gnt_stat_FLD_llid1_gnt_cnt, (reg32), (val))

#define e_llid2_gnt_stat_SET_llid2_gnt_cnt(reg32, val)         REG_FLD_SET(e_llid2_gnt_stat_FLD_llid2_gnt_cnt, (reg32), (val))

#define e_llid3_gnt_stat_SET_llid3_gnt_cnt(reg32, val)         REG_FLD_SET(e_llid3_gnt_stat_FLD_llid3_gnt_cnt, (reg32), (val))

#define e_llid4_gnt_stat_SET_llid4_gnt_cnt(reg32, val)         REG_FLD_SET(e_llid4_gnt_stat_FLD_llid4_gnt_cnt, (reg32), (val))

#define e_llid5_gnt_stat_SET_llid5_gnt_cnt(reg32, val)         REG_FLD_SET(e_llid5_gnt_stat_FLD_llid5_gnt_cnt, (reg32), (val))

#define e_llid6_gnt_stat_SET_llid6_gnt_cnt(reg32, val)         REG_FLD_SET(e_llid6_gnt_stat_FLD_llid6_gnt_cnt, (reg32), (val))

#define e_llid7_gnt_stat_SET_llid7_gnt_cnt(reg32, val)         REG_FLD_SET(e_llid7_gnt_stat_FLD_llid7_gnt_cnt, (reg32), (val))

#define e_snf_mpcp_oam_ctl_SET_snf_mpcp_cap_en(reg32, val)     REG_FLD_SET(e_snf_mpcp_oam_ctl_FLD_snf_mpcp_cap_en, (reg32), (val))
#define e_snf_mpcp_oam_ctl_SET_snf_oam_cap_en(reg32, val)      REG_FLD_SET(e_snf_mpcp_oam_ctl_FLD_snf_oam_cap_en, (reg32), (val))
#define e_snf_mpcp_oam_ctl_SET_snf_mpcp_oam_cnt_set(reg32, val) REG_FLD_SET(e_snf_mpcp_oam_ctl_FLD_snf_mpcp_oam_cnt_set, (reg32), (val))

#define e_rpt_adj_SET_rpt_bitmap_ctrl(reg32, val)              REG_FLD_SET(e_rpt_adj_FLD_rpt_bitmap_ctrl, (reg32), (val))
#define e_rpt_adj_SET_rpt_bitmap_set(reg32, val)               REG_FLD_SET(e_rpt_adj_FLD_rpt_bitmap_set, (reg32), (val))
#define e_rpt_adj_SET_rpt_qsize_fecoff_adj(reg32, val)         REG_FLD_SET(e_rpt_adj_FLD_rpt_qsize_fecoff_adj, (reg32), (val))

#define e_rpt_cnt_SET_rpt_cnt(reg32, val)                      REG_FLD_SET(e_rpt_cnt_FLD_rpt_cnt, (reg32), (val))

#define e_dyinggsp_w1_SET_dyinggsp_w1_len_type(reg32, val)     REG_FLD_SET(e_dyinggsp_w1_FLD_dyinggsp_w1_len_type, (reg32), (val))

#define e_dyinggsp_w2_SET_dyinggsp_w2(reg32, val)              REG_FLD_SET(e_dyinggsp_w2_FLD_dyinggsp_w2, (reg32), (val))

#define e_dyinggsp_w3_SET_dyinggsp_w3(reg32, val)              REG_FLD_SET(e_dyinggsp_w3_FLD_dyinggsp_w3, (reg32), (val))

#define e_dyinggsp_w4_SET_dyinggsp_w4(reg32, val)              REG_FLD_SET(e_dyinggsp_w4_FLD_dyinggsp_w4, (reg32), (val))

#define e_dyinggsp_w5_SET_dyinggsp_w5(reg32, val)              REG_FLD_SET(e_dyinggsp_w5_FLD_dyinggsp_w5, (reg32), (val))

#define e_dyinggsp_w6_SET_dyinggsp_w6(reg32, val)              REG_FLD_SET(e_dyinggsp_w6_FLD_dyinggsp_w6, (reg32), (val))

#define e_dyinggsp_w7_SET_dyinggsp_w7(reg32, val)              REG_FLD_SET(e_dyinggsp_w7_FLD_dyinggsp_w7, (reg32), (val))

#define e_dyinggsp_w8_SET_dyinggsp_w8(reg32, val)              REG_FLD_SET(e_dyinggsp_w8_FLD_dyinggsp_w8, (reg32), (val))

#define e_dyinggsp_w9_SET_dyinggsp_w9(reg32, val)              REG_FLD_SET(e_dyinggsp_w9_FLD_dyinggsp_w9, (reg32), (val))

#define e_dyinggsp_w10_SET_dyinggsp_w10(reg32, val)            REG_FLD_SET(e_dyinggsp_w10_FLD_dyinggsp_w10, (reg32), (val))

#define e_dyinggsp_w11_SET_dyinggsp_w11(reg32, val)            REG_FLD_SET(e_dyinggsp_w11_FLD_dyinggsp_w11, (reg32), (val))

#define e_dyinggsp_w12_SET_dyinggsp_w12(reg32, val)            REG_FLD_SET(e_dyinggsp_w12_FLD_dyinggsp_w12, (reg32), (val))

#define e_oam_kpalv_w1_SET_oam_kpalv_w1(reg32, val)            REG_FLD_SET(e_oam_kpalv_w1_FLD_oam_kpalv_w1, (reg32), (val))

#define e_oam_kpalv_w2_SET_oam_kpalv_w2(reg32, val)            REG_FLD_SET(e_oam_kpalv_w2_FLD_oam_kpalv_w2, (reg32), (val))

#define e_oam_kpalv_w3_SET_oam_kpalv_w3(reg32, val)            REG_FLD_SET(e_oam_kpalv_w3_FLD_oam_kpalv_w3, (reg32), (val))

#define e_oam_kpalv_w4_SET_oam_kpalv_w4(reg32, val)            REG_FLD_SET(e_oam_kpalv_w4_FLD_oam_kpalv_w4, (reg32), (val))

#define e_oam_kpalv_w5_SET_oam_kpalv_w5(reg32, val)            REG_FLD_SET(e_oam_kpalv_w5_FLD_oam_kpalv_w5, (reg32), (val))

#define e_oam_kpalv_w6_SET_oam_kpalv_w6(reg32, val)            REG_FLD_SET(e_oam_kpalv_w6_FLD_oam_kpalv_w6, (reg32), (val))

#define e_oam_kpalv_w7_SET_oam_kpalv_w7(reg32, val)            REG_FLD_SET(e_oam_kpalv_w7_FLD_oam_kpalv_w7, (reg32), (val))

#define e_oam_kpalv_w8_SET_oam_kpalv_w8(reg32, val)            REG_FLD_SET(e_oam_kpalv_w8_FLD_oam_kpalv_w8, (reg32), (val))

#define e_oam_kpalv_w9_SET_oam_kpalv_w9(reg32, val)            REG_FLD_SET(e_oam_kpalv_w9_FLD_oam_kpalv_w9, (reg32), (val))

#define e_oam_kpalv_w10_SET_oam_kpalv_w10(reg32, val)          REG_FLD_SET(e_oam_kpalv_w10_FLD_oam_kpalv_w10, (reg32), (val))

#define e_oam_kpalv_w11_SET_oam_kpalv_w11(reg32, val)          REG_FLD_SET(e_oam_kpalv_w11_FLD_oam_kpalv_w11, (reg32), (val))

#define e_oam_kpalv_w12_SET_oam_kpalv_w12(reg32, val)          REG_FLD_SET(e_oam_kpalv_w12_FLD_oam_kpalv_w12, (reg32), (val))

#define e_oam_kpalv_ctrl_SET_oam_kpalv_interval(reg32, val)    REG_FLD_SET(e_oam_kpalv_ctrl_FLD_oam_kpalv_interval, (reg32), (val))
#define e_oam_kpalv_ctrl_SET_oam_kpalv_sw_trig(reg32, val)     REG_FLD_SET(e_oam_kpalv_ctrl_FLD_oam_kpalv_sw_trig, (reg32), (val))
#define e_oam_kpalv_ctrl_SET_oam_kpalv_sw_cfg(reg32, val)      REG_FLD_SET(e_oam_kpalv_ctrl_FLD_oam_kpalv_sw_cfg, (reg32), (val))
#define e_oam_kpalv_ctrl_SET_oam_kpalv_en(reg32, val)          REG_FLD_SET(e_oam_kpalv_ctrl_FLD_oam_kpalv_en, (reg32), (val))

#define e_tod_1pps_ctrl_SET_tod_1pps_width_ctrl(reg32, val)    REG_FLD_SET(e_tod_1pps_ctrl_FLD_tod_1pps_width_ctrl, (reg32), (val))

#define e_short_pkt_ctrl_SET_short_pkt_size_ctrl(reg32, val)   REG_FLD_SET(e_short_pkt_ctrl_FLD_short_pkt_size_ctrl, (reg32), (val))

#define e_sniff_sp_tag_SET_sniffer_sp_tag(reg32, val)          REG_FLD_SET(e_sniff_sp_tag_FLD_sniffer_sp_tag, (reg32), (val))

#define e_glb_cfg_VAL_sniffer_mode(val)                        REG_FLD_VAL(e_glb_cfg_FLD_sniffer_mode, (val))
#define e_glb_cfg_VAL_txoam_favor(val)                         REG_FLD_VAL(e_glb_cfg_FLD_txoam_favor, (val))
#define e_glb_cfg_VAL_burst_en_dly(val)                        REG_FLD_VAL(e_glb_cfg_FLD_burst_en_dly, (val))
#define e_glb_cfg_VAL_discv_burst_en(val)                      REG_FLD_VAL(e_glb_cfg_FLD_discv_burst_en, (val))
#define e_glb_cfg_VAL_mpcp_fwd(val)                            REG_FLD_VAL(e_glb_cfg_FLD_mpcp_fwd, (val))
#define e_glb_cfg_VAL_bcst_llid_m1_drop(val)                   REG_FLD_VAL(e_glb_cfg_FLD_bcst_llid_m1_drop, (val))
#define e_glb_cfg_VAL_bcst_llid_m0_drop(val)                   REG_FLD_VAL(e_glb_cfg_FLD_bcst_llid_m0_drop, (val))
#define e_glb_cfg_VAL_mcst_llid_drop(val)                      REG_FLD_VAL(e_glb_cfg_FLD_mcst_llid_drop, (val))
#define e_glb_cfg_VAL_all_unicast_llid_pkt_fwd(val)            REG_FLD_VAL(e_glb_cfg_FLD_all_unicast_llid_pkt_fwd, (val))
#define e_glb_cfg_VAL_fcs_err_fwd(val)                         REG_FLD_VAL(e_glb_cfg_FLD_fcs_err_fwd, (val))
#define e_glb_cfg_VAL_llid_crc8_err_fwd(val)                   REG_FLD_VAL(e_glb_cfg_FLD_llid_crc8_err_fwd, (val))
#define e_glb_cfg_VAL_rxmpi_stop(val)                          REG_FLD_VAL(e_glb_cfg_FLD_rxmpi_stop, (val))
#define e_glb_cfg_VAL_txmpi_stop(val)                          REG_FLD_VAL(e_glb_cfg_FLD_txmpi_stop, (val))
#define e_glb_cfg_VAL_phy_pwr_down(val)                        REG_FLD_VAL(e_glb_cfg_FLD_phy_pwr_down, (val))
#define e_glb_cfg_VAL_rx_nml_gate_fwd(val)                     REG_FLD_VAL(e_glb_cfg_FLD_rx_nml_gate_fwd, (val))
#define e_glb_cfg_VAL_rxmbi_stop(val)                          REG_FLD_VAL(e_glb_cfg_FLD_rxmbi_stop, (val))
#define e_glb_cfg_VAL_txmbi_stop(val)                          REG_FLD_VAL(e_glb_cfg_FLD_txmbi_stop, (val))
#define e_glb_cfg_VAL_chk_all_gnt_mode(val)                    REG_FLD_VAL(e_glb_cfg_FLD_chk_all_gnt_mode, (val))
#define e_glb_cfg_VAL_loc_cnt_sync_method(val)                 REG_FLD_VAL(e_glb_cfg_FLD_loc_cnt_sync_method, (val))
#define e_glb_cfg_VAL_tx_default_rpt(val)                      REG_FLD_VAL(e_glb_cfg_FLD_tx_default_rpt, (val))
#define e_glb_cfg_VAL_epon_mac_sw_rst(val)                     REG_FLD_VAL(e_glb_cfg_FLD_epon_mac_sw_rst, (val))
#define e_glb_cfg_VAL_epon_oam_cal_in_eth(val)                 REG_FLD_VAL(e_glb_cfg_FLD_epon_oam_cal_in_eth, (val))
#define e_glb_cfg_VAL_epon_mac_lpbk_en(val)                    REG_FLD_VAL(e_glb_cfg_FLD_epon_mac_lpbk_en, (val))
#define e_glb_cfg_VAL_rpt_txpri_ctrl(val)                      REG_FLD_VAL(e_glb_cfg_FLD_rpt_txpri_ctrl, (val))
#define e_glb_cfg_VAL_mode_sel(val)                            REG_FLD_VAL(e_glb_cfg_FLD_mode_sel, (val))

#define e_int_status_VAL_sniff_fifo_ovrun_int(val)             REG_FLD_VAL(e_int_status_FLD_sniff_fifo_ovrun_int, (val))
#define e_int_status_VAL_reg_ack_done_int(val)                 REG_FLD_VAL(e_int_status_FLD_reg_ack_done_int, (val))
#define e_int_status_VAL_reg_req_done_int(val)                 REG_FLD_VAL(e_int_status_FLD_reg_req_done_int, (val))
#define e_int_status_VAL_reorder1_gnt_int(val)                 REG_FLD_VAL(e_int_status_FLD_reorder1_gnt_int, (val))
#define e_int_status_VAL_b2b_gnt_int(val)                      REG_FLD_VAL(e_int_status_FLD_b2b_gnt_int, (val))
#define e_int_status_VAL_hidn_gnt_int(val)                     REG_FLD_VAL(e_int_status_FLD_hidn_gnt_int, (val))
#define e_int_status_VAL_ps_early_wakeup_int(val)              REG_FLD_VAL(e_int_status_FLD_ps_early_wakeup_int, (val))
#define e_int_status_VAL_rx_sleep_allow_int(val)               REG_FLD_VAL(e_int_status_FLD_rx_sleep_allow_int, (val))
#define e_int_status_VAL_ps_wakeup_int(val)                    REG_FLD_VAL(e_int_status_FLD_ps_wakeup_int, (val))
#define e_int_status_VAL_ps_sleep_int(val)                     REG_FLD_VAL(e_int_status_FLD_ps_sleep_int, (val))
#define e_int_status_VAL_txfifo_udrun_int(val)                 REG_FLD_VAL(e_int_status_FLD_txfifo_udrun_int, (val))
#define e_int_status_VAL_rpt_overintvl_int(val)                REG_FLD_VAL(e_int_status_FLD_rpt_overintvl_int, (val))
#define e_int_status_VAL_mpcp_timeout_int(val)                 REG_FLD_VAL(e_int_status_FLD_mpcp_timeout_int, (val))
#define e_int_status_VAL_timedrft_int(val)                     REG_FLD_VAL(e_int_status_FLD_timedrft_int, (val))
#define e_int_status_VAL_tod_1pps_int(val)                     REG_FLD_VAL(e_int_status_FLD_tod_1pps_int, (val))
#define e_int_status_VAL_tod_updt_int(val)                     REG_FLD_VAL(e_int_status_FLD_tod_updt_int, (val))
#define e_int_status_VAL_ptp_msg_tx_int(val)                   REG_FLD_VAL(e_int_status_FLD_ptp_msg_tx_int, (val))
#define e_int_status_VAL_gnt_buf_ovrrun_int(val)               REG_FLD_VAL(e_int_status_FLD_gnt_buf_ovrrun_int, (val))
#define e_int_status_VAL_llid7_rcv_rgst_int(val)               REG_FLD_VAL(e_int_status_FLD_llid7_rcv_rgst_int, (val))
#define e_int_status_VAL_llid6_rcv_rgst_int(val)               REG_FLD_VAL(e_int_status_FLD_llid6_rcv_rgst_int, (val))
#define e_int_status_VAL_llid5_rcv_rgst_int(val)               REG_FLD_VAL(e_int_status_FLD_llid5_rcv_rgst_int, (val))
#define e_int_status_VAL_llid4_rcv_rgst_int(val)               REG_FLD_VAL(e_int_status_FLD_llid4_rcv_rgst_int, (val))
#define e_int_status_VAL_llid3_rcv_rgst_int(val)               REG_FLD_VAL(e_int_status_FLD_llid3_rcv_rgst_int, (val))
#define e_int_status_VAL_llid2_rcv_rgst_int(val)               REG_FLD_VAL(e_int_status_FLD_llid2_rcv_rgst_int, (val))
#define e_int_status_VAL_llid1_rcv_rgst_int(val)               REG_FLD_VAL(e_int_status_FLD_llid1_rcv_rgst_int, (val))
#define e_int_status_VAL_llid0_rcv_rgst_int(val)               REG_FLD_VAL(e_int_status_FLD_llid0_rcv_rgst_int, (val))
#define e_int_status_VAL_rcv_dscvry_gate_int(val)              REG_FLD_VAL(e_int_status_FLD_rcv_dscvry_gate_int, (val))

#define e_int_en_VAL_sniff_fifo_ovrun_en(val)                  REG_FLD_VAL(e_int_en_FLD_sniff_fifo_ovrun_en, (val))
#define e_int_en_VAL_reg_ack_done_en(val)                      REG_FLD_VAL(e_int_en_FLD_reg_ack_done_en, (val))
#define e_int_en_VAL_reg_req_done_en(val)                      REG_FLD_VAL(e_int_en_FLD_reg_req_done_en, (val))
#define e_int_en_VAL_reorder1_gnt_en(val)                      REG_FLD_VAL(e_int_en_FLD_reorder1_gnt_en, (val))
#define e_int_en_VAL_b2b_gnt_en(val)                           REG_FLD_VAL(e_int_en_FLD_b2b_gnt_en, (val))
#define e_int_en_VAL_hidn_gnt_en(val)                          REG_FLD_VAL(e_int_en_FLD_hidn_gnt_en, (val))
#define e_int_en_VAL_ps_early_wakeup_en(val)                   REG_FLD_VAL(e_int_en_FLD_ps_early_wakeup_en, (val))
#define e_int_en_VAL_rx_sleep_allow_en(val)                    REG_FLD_VAL(e_int_en_FLD_rx_sleep_allow_en, (val))
#define e_int_en_VAL_ps_wakeup_en(val)                         REG_FLD_VAL(e_int_en_FLD_ps_wakeup_en, (val))
#define e_int_en_VAL_ps_sleep_en(val)                          REG_FLD_VAL(e_int_en_FLD_ps_sleep_en, (val))
#define e_int_en_VAL_txfifo_udrun_en(val)                      REG_FLD_VAL(e_int_en_FLD_txfifo_udrun_en, (val))
#define e_int_en_VAL_rpt_overintvl_en(val)                     REG_FLD_VAL(e_int_en_FLD_rpt_overintvl_en, (val))
#define e_int_en_VAL_mpcp_timeout_en(val)                      REG_FLD_VAL(e_int_en_FLD_mpcp_timeout_en, (val))
#define e_int_en_VAL_timedrft_en(val)                          REG_FLD_VAL(e_int_en_FLD_timedrft_en, (val))
#define e_int_en_VAL_tod_1pps_en(val)                          REG_FLD_VAL(e_int_en_FLD_tod_1pps_en, (val))
#define e_int_en_VAL_tod_updt_en(val)                          REG_FLD_VAL(e_int_en_FLD_tod_updt_en, (val))
#define e_int_en_VAL_ptp_msg_tx_en(val)                        REG_FLD_VAL(e_int_en_FLD_ptp_msg_tx_en, (val))
#define e_int_en_VAL_gnt_buf_ovrrun_en(val)                    REG_FLD_VAL(e_int_en_FLD_gnt_buf_ovrrun_en, (val))
#define e_int_en_VAL_llid7_rcv_rgst_en(val)                    REG_FLD_VAL(e_int_en_FLD_llid7_rcv_rgst_en, (val))
#define e_int_en_VAL_llid6_rcv_rgst_en(val)                    REG_FLD_VAL(e_int_en_FLD_llid6_rcv_rgst_en, (val))
#define e_int_en_VAL_llid5_rcv_rgst_en(val)                    REG_FLD_VAL(e_int_en_FLD_llid5_rcv_rgst_en, (val))
#define e_int_en_VAL_llid4_rcv_rgst_en(val)                    REG_FLD_VAL(e_int_en_FLD_llid4_rcv_rgst_en, (val))
#define e_int_en_VAL_llid3_rcv_rgst_en(val)                    REG_FLD_VAL(e_int_en_FLD_llid3_rcv_rgst_en, (val))
#define e_int_en_VAL_llid2_rcv_rgst_en(val)                    REG_FLD_VAL(e_int_en_FLD_llid2_rcv_rgst_en, (val))
#define e_int_en_VAL_llid1_rcv_rgst_en(val)                    REG_FLD_VAL(e_int_en_FLD_llid1_rcv_rgst_en, (val))
#define e_int_en_VAL_llid0_rcv_rgst_en(val)                    REG_FLD_VAL(e_int_en_FLD_llid0_rcv_rgst_en, (val))
#define e_int_en_VAL_rcv_dscvry_gate_en(val)                   REG_FLD_VAL(e_int_en_FLD_rcv_dscvry_gate_en, (val))

#define e_rpt_mpcp_timeout_llid_idx_VAL_llid7_mpcp_tmo(val)    REG_FLD_VAL(e_rpt_mpcp_timeout_llid_idx_FLD_llid7_mpcp_tmo, (val))
#define e_rpt_mpcp_timeout_llid_idx_VAL_llid6_mpcp_tmo(val)    REG_FLD_VAL(e_rpt_mpcp_timeout_llid_idx_FLD_llid6_mpcp_tmo, (val))
#define e_rpt_mpcp_timeout_llid_idx_VAL_llid5_mpcp_tmo(val)    REG_FLD_VAL(e_rpt_mpcp_timeout_llid_idx_FLD_llid5_mpcp_tmo, (val))
#define e_rpt_mpcp_timeout_llid_idx_VAL_llid4_mpcp_tmo(val)    REG_FLD_VAL(e_rpt_mpcp_timeout_llid_idx_FLD_llid4_mpcp_tmo, (val))
#define e_rpt_mpcp_timeout_llid_idx_VAL_llid3_mpcp_tmo(val)    REG_FLD_VAL(e_rpt_mpcp_timeout_llid_idx_FLD_llid3_mpcp_tmo, (val))
#define e_rpt_mpcp_timeout_llid_idx_VAL_llid2_mpcp_tmo(val)    REG_FLD_VAL(e_rpt_mpcp_timeout_llid_idx_FLD_llid2_mpcp_tmo, (val))
#define e_rpt_mpcp_timeout_llid_idx_VAL_llid1_mpcp_tmo(val)    REG_FLD_VAL(e_rpt_mpcp_timeout_llid_idx_FLD_llid1_mpcp_tmo, (val))
#define e_rpt_mpcp_timeout_llid_idx_VAL_llid0_mpcp_tmo(val)    REG_FLD_VAL(e_rpt_mpcp_timeout_llid_idx_FLD_llid0_mpcp_tmo, (val))
#define e_rpt_mpcp_timeout_llid_idx_VAL_llid7_rpt_tmo(val)     REG_FLD_VAL(e_rpt_mpcp_timeout_llid_idx_FLD_llid7_rpt_tmo, (val))
#define e_rpt_mpcp_timeout_llid_idx_VAL_llid6_rpt_tmo(val)     REG_FLD_VAL(e_rpt_mpcp_timeout_llid_idx_FLD_llid6_rpt_tmo, (val))
#define e_rpt_mpcp_timeout_llid_idx_VAL_llid5_rpt_tmo(val)     REG_FLD_VAL(e_rpt_mpcp_timeout_llid_idx_FLD_llid5_rpt_tmo, (val))
#define e_rpt_mpcp_timeout_llid_idx_VAL_llid4_rpt_tmo(val)     REG_FLD_VAL(e_rpt_mpcp_timeout_llid_idx_FLD_llid4_rpt_tmo, (val))
#define e_rpt_mpcp_timeout_llid_idx_VAL_llid3_rpt_tmo(val)     REG_FLD_VAL(e_rpt_mpcp_timeout_llid_idx_FLD_llid3_rpt_tmo, (val))
#define e_rpt_mpcp_timeout_llid_idx_VAL_llid2_rpt_tmo(val)     REG_FLD_VAL(e_rpt_mpcp_timeout_llid_idx_FLD_llid2_rpt_tmo, (val))
#define e_rpt_mpcp_timeout_llid_idx_VAL_llid1_rpt_tmo(val)     REG_FLD_VAL(e_rpt_mpcp_timeout_llid_idx_FLD_llid1_rpt_tmo, (val))
#define e_rpt_mpcp_timeout_llid_idx_VAL_llid0_rpt_tmo(val)     REG_FLD_VAL(e_rpt_mpcp_timeout_llid_idx_FLD_llid0_rpt_tmo, (val))

#define e_dyinggsp_cfg_VAL_hw_dying_gasp_en(val)               REG_FLD_VAL(e_dyinggsp_cfg_FLD_hw_dying_gasp_en, (val))
#define e_dyinggsp_cfg_VAL_dying_gasp_code(val)                REG_FLD_VAL(e_dyinggsp_cfg_FLD_dying_gasp_code, (val))
#define e_dyinggsp_cfg_VAL_sw_init_dying_gasp(val)             REG_FLD_VAL(e_dyinggsp_cfg_FLD_sw_init_dying_gasp, (val))
#define e_dyinggsp_cfg_VAL_dygsp_num_of_times(val)             REG_FLD_VAL(e_dyinggsp_cfg_FLD_dygsp_num_of_times, (val))
#define e_dyinggsp_cfg_VAL_dying_gasp_flags(val)               REG_FLD_VAL(e_dyinggsp_cfg_FLD_dying_gasp_flags, (val))

#define e_pending_gnt_num_VAL_echoed_pending_gnt(val)          REG_FLD_VAL(e_pending_gnt_num_FLD_echoed_pending_gnt, (val))
#define e_pending_gnt_num_VAL_pending_gnt_num(val)             REG_FLD_VAL(e_pending_gnt_num_FLD_pending_gnt_num, (val))

#define e_llid0_3_cfg_VAL_llid3_txfec_en(val)                  REG_FLD_VAL(e_llid0_3_cfg_FLD_llid3_txfec_en, (val))
#define e_llid0_3_cfg_VAL_llid3_dcrypt_en(val)                 REG_FLD_VAL(e_llid0_3_cfg_FLD_llid3_dcrypt_en, (val))
#define e_llid0_3_cfg_VAL_llid3_dcrypt_mode(val)               REG_FLD_VAL(e_llid0_3_cfg_FLD_llid3_dcrypt_mode, (val))
#define e_llid0_3_cfg_VAL_llid3_oam_lpbk_en(val)               REG_FLD_VAL(e_llid0_3_cfg_FLD_llid3_oam_lpbk_en, (val))
#define e_llid0_3_cfg_VAL_llid2_txfec_en(val)                  REG_FLD_VAL(e_llid0_3_cfg_FLD_llid2_txfec_en, (val))
#define e_llid0_3_cfg_VAL_llid2_dcrypt_en(val)                 REG_FLD_VAL(e_llid0_3_cfg_FLD_llid2_dcrypt_en, (val))
#define e_llid0_3_cfg_VAL_llid2_dcrypt_mode(val)               REG_FLD_VAL(e_llid0_3_cfg_FLD_llid2_dcrypt_mode, (val))
#define e_llid0_3_cfg_VAL_llid2_oam_lpbk_en(val)               REG_FLD_VAL(e_llid0_3_cfg_FLD_llid2_oam_lpbk_en, (val))
#define e_llid0_3_cfg_VAL_llid1_txfec_en(val)                  REG_FLD_VAL(e_llid0_3_cfg_FLD_llid1_txfec_en, (val))
#define e_llid0_3_cfg_VAL_llid1_dcrypt_en(val)                 REG_FLD_VAL(e_llid0_3_cfg_FLD_llid1_dcrypt_en, (val))
#define e_llid0_3_cfg_VAL_llid1_dcrypt_mode(val)               REG_FLD_VAL(e_llid0_3_cfg_FLD_llid1_dcrypt_mode, (val))
#define e_llid0_3_cfg_VAL_llid1_oam_lpbk_en(val)               REG_FLD_VAL(e_llid0_3_cfg_FLD_llid1_oam_lpbk_en, (val))
#define e_llid0_3_cfg_VAL_llid0_txfec_en(val)                  REG_FLD_VAL(e_llid0_3_cfg_FLD_llid0_txfec_en, (val))
#define e_llid0_3_cfg_VAL_llid0_dcrypt_en(val)                 REG_FLD_VAL(e_llid0_3_cfg_FLD_llid0_dcrypt_en, (val))
#define e_llid0_3_cfg_VAL_llid0_dcrypt_mode(val)               REG_FLD_VAL(e_llid0_3_cfg_FLD_llid0_dcrypt_mode, (val))
#define e_llid0_3_cfg_VAL_llid0_oam_lpbk_en(val)               REG_FLD_VAL(e_llid0_3_cfg_FLD_llid0_oam_lpbk_en, (val))

#define e_llid4_7_cfg_VAL_llid7_txfec_en(val)                  REG_FLD_VAL(e_llid4_7_cfg_FLD_llid7_txfec_en, (val))
#define e_llid4_7_cfg_VAL_llid7_dcrypt_en(val)                 REG_FLD_VAL(e_llid4_7_cfg_FLD_llid7_dcrypt_en, (val))
#define e_llid4_7_cfg_VAL_llid7_dcrypt_mode(val)               REG_FLD_VAL(e_llid4_7_cfg_FLD_llid7_dcrypt_mode, (val))
#define e_llid4_7_cfg_VAL_llid7_oam_lpbk_en(val)               REG_FLD_VAL(e_llid4_7_cfg_FLD_llid7_oam_lpbk_en, (val))
#define e_llid4_7_cfg_VAL_llid6_txfec_en(val)                  REG_FLD_VAL(e_llid4_7_cfg_FLD_llid6_txfec_en, (val))
#define e_llid4_7_cfg_VAL_llid6_dcrypt_en(val)                 REG_FLD_VAL(e_llid4_7_cfg_FLD_llid6_dcrypt_en, (val))
#define e_llid4_7_cfg_VAL_llid6_dcrypt_mode(val)               REG_FLD_VAL(e_llid4_7_cfg_FLD_llid6_dcrypt_mode, (val))
#define e_llid4_7_cfg_VAL_llid6_oam_lpbk_en(val)               REG_FLD_VAL(e_llid4_7_cfg_FLD_llid6_oam_lpbk_en, (val))
#define e_llid4_7_cfg_VAL_llid5_txfec_en(val)                  REG_FLD_VAL(e_llid4_7_cfg_FLD_llid5_txfec_en, (val))
#define e_llid4_7_cfg_VAL_llid5_dcrypt_en(val)                 REG_FLD_VAL(e_llid4_7_cfg_FLD_llid5_dcrypt_en, (val))
#define e_llid4_7_cfg_VAL_llid5_dcrypt_mode(val)               REG_FLD_VAL(e_llid4_7_cfg_FLD_llid5_dcrypt_mode, (val))
#define e_llid4_7_cfg_VAL_llid5_oam_lpbk_en(val)               REG_FLD_VAL(e_llid4_7_cfg_FLD_llid5_oam_lpbk_en, (val))
#define e_llid4_7_cfg_VAL_llid4_txfec_en(val)                  REG_FLD_VAL(e_llid4_7_cfg_FLD_llid4_txfec_en, (val))
#define e_llid4_7_cfg_VAL_llid4_dcrypt_en(val)                 REG_FLD_VAL(e_llid4_7_cfg_FLD_llid4_dcrypt_en, (val))
#define e_llid4_7_cfg_VAL_llid4_dcrypt_mode(val)               REG_FLD_VAL(e_llid4_7_cfg_FLD_llid4_dcrypt_mode, (val))
#define e_llid4_7_cfg_VAL_llid4_oam_lpbk_en(val)               REG_FLD_VAL(e_llid4_7_cfg_FLD_llid4_oam_lpbk_en, (val))

#define e_llid_dscvry_ctrl_VAL_mpcp_cmd(val)                   REG_FLD_VAL(e_llid_dscvry_ctrl_FLD_mpcp_cmd, (val))
#define e_llid_dscvry_ctrl_VAL_mpcp_cmd_done(val)              REG_FLD_VAL(e_llid_dscvry_ctrl_FLD_mpcp_cmd_done, (val))
#define e_llid_dscvry_ctrl_VAL_rgstr_ack_flg(val)              REG_FLD_VAL(e_llid_dscvry_ctrl_FLD_rgstr_ack_flg, (val))
#define e_llid_dscvry_ctrl_VAL_rgstr_req_flg(val)              REG_FLD_VAL(e_llid_dscvry_ctrl_FLD_rgstr_req_flg, (val))
#define e_llid_dscvry_ctrl_VAL_tx_mpcp_llid_idx(val)           REG_FLD_VAL(e_llid_dscvry_ctrl_FLD_tx_mpcp_llid_idx, (val))

#define e_llid0_dscvry_sts_VAL_llid0_dscvry_sts(val)           REG_FLD_VAL(e_llid0_dscvry_sts_FLD_llid0_dscvry_sts, (val))
#define e_llid0_dscvry_sts_VAL_llid0_rgstr_flg_sts(val)        REG_FLD_VAL(e_llid0_dscvry_sts_FLD_llid0_rgstr_flg_sts, (val))
#define e_llid0_dscvry_sts_VAL_llid0_valid(val)                REG_FLD_VAL(e_llid0_dscvry_sts_FLD_llid0_valid, (val))
#define e_llid0_dscvry_sts_VAL_llid0_value(val)                REG_FLD_VAL(e_llid0_dscvry_sts_FLD_llid0_value, (val))

#define e_llid1_dscvry_sts_VAL_llid1_dscvry_sts(val)           REG_FLD_VAL(e_llid1_dscvry_sts_FLD_llid1_dscvry_sts, (val))
#define e_llid1_dscvry_sts_VAL_llid1_rgstr_flg_sts(val)        REG_FLD_VAL(e_llid1_dscvry_sts_FLD_llid1_rgstr_flg_sts, (val))
#define e_llid1_dscvry_sts_VAL_llid1_valid(val)                REG_FLD_VAL(e_llid1_dscvry_sts_FLD_llid1_valid, (val))
#define e_llid1_dscvry_sts_VAL_llid1_value(val)                REG_FLD_VAL(e_llid1_dscvry_sts_FLD_llid1_value, (val))

#define e_llid2_dscvry_sts_VAL_llid2_dscvry_sts(val)           REG_FLD_VAL(e_llid2_dscvry_sts_FLD_llid2_dscvry_sts, (val))
#define e_llid2_dscvry_sts_VAL_llid2_rgstr_flg_sts(val)        REG_FLD_VAL(e_llid2_dscvry_sts_FLD_llid2_rgstr_flg_sts, (val))
#define e_llid2_dscvry_sts_VAL_llid2_valid(val)                REG_FLD_VAL(e_llid2_dscvry_sts_FLD_llid2_valid, (val))
#define e_llid2_dscvry_sts_VAL_llid2_value(val)                REG_FLD_VAL(e_llid2_dscvry_sts_FLD_llid2_value, (val))

#define e_llid3_dscvry_sts_VAL_llid3_dscvry_sts(val)           REG_FLD_VAL(e_llid3_dscvry_sts_FLD_llid3_dscvry_sts, (val))
#define e_llid3_dscvry_sts_VAL_llid3_rgstr_flg_sts(val)        REG_FLD_VAL(e_llid3_dscvry_sts_FLD_llid3_rgstr_flg_sts, (val))
#define e_llid3_dscvry_sts_VAL_llid3_valid(val)                REG_FLD_VAL(e_llid3_dscvry_sts_FLD_llid3_valid, (val))
#define e_llid3_dscvry_sts_VAL_llid3_value(val)                REG_FLD_VAL(e_llid3_dscvry_sts_FLD_llid3_value, (val))

#define e_llid4_dscvry_sts_VAL_llid4_dscvry_sts(val)           REG_FLD_VAL(e_llid4_dscvry_sts_FLD_llid4_dscvry_sts, (val))
#define e_llid4_dscvry_sts_VAL_llid4_rgstr_flg_sts(val)        REG_FLD_VAL(e_llid4_dscvry_sts_FLD_llid4_rgstr_flg_sts, (val))
#define e_llid4_dscvry_sts_VAL_llid4_valid(val)                REG_FLD_VAL(e_llid4_dscvry_sts_FLD_llid4_valid, (val))
#define e_llid4_dscvry_sts_VAL_llid4_value(val)                REG_FLD_VAL(e_llid4_dscvry_sts_FLD_llid4_value, (val))

#define e_llid5_dscvry_sts_VAL_llid5_dscvry_sts(val)           REG_FLD_VAL(e_llid5_dscvry_sts_FLD_llid5_dscvry_sts, (val))
#define e_llid5_dscvry_sts_VAL_llid5_rgstr_flg_sts(val)        REG_FLD_VAL(e_llid5_dscvry_sts_FLD_llid5_rgstr_flg_sts, (val))
#define e_llid5_dscvry_sts_VAL_llid5_valid(val)                REG_FLD_VAL(e_llid5_dscvry_sts_FLD_llid5_valid, (val))
#define e_llid5_dscvry_sts_VAL_llid5_value(val)                REG_FLD_VAL(e_llid5_dscvry_sts_FLD_llid5_value, (val))

#define e_llid6_dscvry_sts_VAL_llid6_dscvry_sts(val)           REG_FLD_VAL(e_llid6_dscvry_sts_FLD_llid6_dscvry_sts, (val))
#define e_llid6_dscvry_sts_VAL_llid6_rgstr_flg_sts(val)        REG_FLD_VAL(e_llid6_dscvry_sts_FLD_llid6_rgstr_flg_sts, (val))
#define e_llid6_dscvry_sts_VAL_llid6_valid(val)                REG_FLD_VAL(e_llid6_dscvry_sts_FLD_llid6_valid, (val))
#define e_llid6_dscvry_sts_VAL_llid6_value(val)                REG_FLD_VAL(e_llid6_dscvry_sts_FLD_llid6_value, (val))

#define e_llid7_dscvry_sts_VAL_llid7_dscvry_sts(val)           REG_FLD_VAL(e_llid7_dscvry_sts_FLD_llid7_dscvry_sts, (val))
#define e_llid7_dscvry_sts_VAL_llid7_rgstr_flg_sts(val)        REG_FLD_VAL(e_llid7_dscvry_sts_FLD_llid7_rgstr_flg_sts, (val))
#define e_llid7_dscvry_sts_VAL_llid7_valid(val)                REG_FLD_VAL(e_llid7_dscvry_sts_FLD_llid7_valid, (val))
#define e_llid7_dscvry_sts_VAL_llid7_value(val)                REG_FLD_VAL(e_llid7_dscvry_sts_FLD_llid7_value, (val))

#define e_mac_addr_cfg_VAL_mac_addr_rwcmd(val)                 REG_FLD_VAL(e_mac_addr_cfg_FLD_mac_addr_rwcmd, (val))
#define e_mac_addr_cfg_VAL_mac_addr_rwcmd_done(val)            REG_FLD_VAL(e_mac_addr_cfg_FLD_mac_addr_rwcmd_done, (val))
#define e_mac_addr_cfg_VAL_mac_addr_llid_indx(val)             REG_FLD_VAL(e_mac_addr_cfg_FLD_mac_addr_llid_indx, (val))
#define e_mac_addr_cfg_VAL_mac_addr_dw_idx(val)                REG_FLD_VAL(e_mac_addr_cfg_FLD_mac_addr_dw_idx, (val))

#define e_mac_addr_value_VAL_mac_addr_value(val)               REG_FLD_VAL(e_mac_addr_value_FLD_mac_addr_value, (val))

#define e_security_key_cfg_VAL_key_rwcmd(val)                  REG_FLD_VAL(e_security_key_cfg_FLD_key_rwcmd, (val))
#define e_security_key_cfg_VAL_key_rwcmd_done(val)             REG_FLD_VAL(e_security_key_cfg_FLD_key_rwcmd_done, (val))
#define e_security_key_cfg_VAL_key_llid_index(val)             REG_FLD_VAL(e_security_key_cfg_FLD_key_llid_index, (val))
#define e_security_key_cfg_VAL_key_idx(val)                    REG_FLD_VAL(e_security_key_cfg_FLD_key_idx, (val))
#define e_security_key_cfg_VAL_key_dw_indx(val)                REG_FLD_VAL(e_security_key_cfg_FLD_key_dw_indx, (val))

#define e_key_value_VAL_key_value(val)                         REG_FLD_VAL(e_key_value_FLD_key_value, (val))

#define e_rpt_data_VAL_df_rpt_data(val)                        REG_FLD_VAL(e_rpt_data_FLD_df_rpt_data, (val))

#define e_rpt_len_VAL_df_rpt_fifo_clr(val)                     REG_FLD_VAL(e_rpt_len_FLD_df_rpt_fifo_clr, (val))
#define e_rpt_len_VAL_df_rpt_dt_len(val)                       REG_FLD_VAL(e_rpt_len_FLD_df_rpt_dt_len, (val))

#define e_rpt_cfg_VAL_qsize_fec_adj(val)                       REG_FLD_VAL(e_rpt_cfg_FLD_qsize_fec_adj, (val))
#define e_rpt_cfg_VAL_llid7_rpt_cfg(val)                       REG_FLD_VAL(e_rpt_cfg_FLD_llid7_rpt_cfg, (val))
#define e_rpt_cfg_VAL_llid6_rpt_cfg(val)                       REG_FLD_VAL(e_rpt_cfg_FLD_llid6_rpt_cfg, (val))
#define e_rpt_cfg_VAL_llid5_rpt_cfg(val)                       REG_FLD_VAL(e_rpt_cfg_FLD_llid5_rpt_cfg, (val))
#define e_rpt_cfg_VAL_llid4_rpt_cfg(val)                       REG_FLD_VAL(e_rpt_cfg_FLD_llid4_rpt_cfg, (val))
#define e_rpt_cfg_VAL_llid3_rpt_cfg(val)                       REG_FLD_VAL(e_rpt_cfg_FLD_llid3_rpt_cfg, (val))
#define e_rpt_cfg_VAL_llid2_rpt_cfg(val)                       REG_FLD_VAL(e_rpt_cfg_FLD_llid2_rpt_cfg, (val))
#define e_rpt_cfg_VAL_llid1_rpt_cfg(val)                       REG_FLD_VAL(e_rpt_cfg_FLD_llid1_rpt_cfg, (val))
#define e_rpt_cfg_VAL_llid0_rpt_cfg(val)                       REG_FLD_VAL(e_rpt_cfg_FLD_llid0_rpt_cfg, (val))

#define e_rpt_qthld_cfg_VAL_qthld_rwcmd(val)                   REG_FLD_VAL(e_rpt_qthld_cfg_FLD_qthld_rwcmd, (val))
#define e_rpt_qthld_cfg_VAL_qthld_rwcmd_done(val)              REG_FLD_VAL(e_rpt_qthld_cfg_FLD_qthld_rwcmd_done, (val))
#define e_rpt_qthld_cfg_VAL_qthld_value(val)                   REG_FLD_VAL(e_rpt_qthld_cfg_FLD_qthld_value, (val))
#define e_rpt_qthld_cfg_VAL_qthld_idx(val)                     REG_FLD_VAL(e_rpt_qthld_cfg_FLD_qthld_idx, (val))
#define e_rpt_qthld_cfg_VAL_rpt_llid_idx(val)                  REG_FLD_VAL(e_rpt_qthld_cfg_FLD_rpt_llid_idx, (val))
#define e_rpt_qthld_cfg_VAL_queue_idx(val)                     REG_FLD_VAL(e_rpt_qthld_cfg_FLD_queue_idx, (val))

#define e_local_time_VAL_local_time(val)                       REG_FLD_VAL(e_local_time_FLD_local_time, (val))

#define e_tod_sync_x_VAL_tod_sync_x(val)                       REG_FLD_VAL(e_tod_sync_x_FLD_tod_sync_x, (val))

#define e_tod_ltncy_VAL_ingrs_latency(val)                     REG_FLD_VAL(e_tod_ltncy_FLD_ingrs_latency, (val))
#define e_tod_ltncy_VAL_egrs_latency(val)                      REG_FLD_VAL(e_tod_ltncy_FLD_egrs_latency, (val))

#define p2p_tx_tag1_VAL_p2p_tx_tagging_done(val)               REG_FLD_VAL(p2p_tx_tag1_FLD_p2p_tx_tagging_done, (val))
#define p2p_tx_tag1_VAL_p2p_tx_pkt_info(val)                   REG_FLD_VAL(p2p_tx_tag1_FLD_p2p_tx_pkt_info, (val))
#define p2p_tx_tag1_VAL_p2p_tx_tag_sec_l8(val)                 REG_FLD_VAL(p2p_tx_tag1_FLD_p2p_tx_tag_sec_l8, (val))

#define p2p_tx_tag2_VAL_p2p_tx_tag_nsec_29_5(val)              REG_FLD_VAL(p2p_tx_tag2_FLD_p2p_tx_tag_nsec_29_5, (val))

#define e_new_tod_p2p_offset_sec_l32_VAL_new_tod_p2p_offset_sec_l32(val) REG_FLD_VAL(e_new_tod_p2p_offset_sec_l32_FLD_new_tod_p2p_offset_sec_l32, (val))

#define e_new_tod_p2p_tod_offset_nsec_VAL_new_tod_nsec(val)    REG_FLD_VAL(e_new_tod_p2p_tod_offset_nsec_FLD_new_tod_nsec, (val))

#define e_tod_p2p_tod_sec_l32_VAL_tod_p2p_sec_l32(val)         REG_FLD_VAL(e_tod_p2p_tod_sec_l32_FLD_tod_p2p_sec_l32, (val))

#define e_tod_p2p_tod_nsec_VAL_tod_p2p_nsec(val)               REG_FLD_VAL(e_tod_p2p_tod_nsec_FLD_tod_p2p_nsec, (val))

#define e_tod_period_VAL_tod_period(val)                       REG_FLD_VAL(e_tod_period_FLD_tod_period, (val))

#define e_pwr_sv_cfg_VAL_pwd_mode(val)                         REG_FLD_VAL(e_pwr_sv_cfg_FLD_pwd_mode, (val))
#define e_pwr_sv_cfg_VAL_timedrift_ignore(val)                 REG_FLD_VAL(e_pwr_sv_cfg_FLD_timedrift_ignore, (val))
#define e_pwr_sv_cfg_VAL_onu_wakeup(val)                       REG_FLD_VAL(e_pwr_sv_cfg_FLD_onu_wakeup, (val))
#define e_pwr_sv_cfg_VAL_pwr_sv_start(val)                     REG_FLD_VAL(e_pwr_sv_cfg_FLD_pwr_sv_start, (val))
#define e_pwr_sv_cfg_VAL_pwd_mode_i(val)                       REG_FLD_VAL(e_pwr_sv_cfg_FLD_pwd_mode_i, (val))
#define e_pwr_sv_cfg_VAL_slp_duration_max_h(val)               REG_FLD_VAL(e_pwr_sv_cfg_FLD_slp_duration_max_h, (val))

#define e_slp_durt_max_VAL_slp_duration_max_l(val)             REG_FLD_VAL(e_slp_durt_max_FLD_slp_duration_max_l, (val))

#define e_slp_duration_VAL_slp_duration(val)                   REG_FLD_VAL(e_slp_duration_FLD_slp_duration, (val))

#define e_act_duration_VAL_act_duration(val)                   REG_FLD_VAL(e_act_duration_FLD_act_duration, (val))

#define e_pwron_dly_VAL_pwron_dly(val)                         REG_FLD_VAL(e_pwron_dly_FLD_pwron_dly, (val))

#define e_slp_duration_i_VAL_slp_duration_i(val)               REG_FLD_VAL(e_slp_duration_i_FLD_slp_duration_i, (val))

#define e_txfetch_cfg_VAL_tx_fetch_overhead(val)               REG_FLD_VAL(e_txfetch_cfg_FLD_tx_fetch_overhead, (val))
#define e_txfetch_cfg_VAL_tx_fetch_leadtime(val)               REG_FLD_VAL(e_txfetch_cfg_FLD_tx_fetch_leadtime, (val))
#define e_txfetch_cfg_VAL_tx_dma_leadtime(val)                 REG_FLD_VAL(e_txfetch_cfg_FLD_tx_dma_leadtime, (val))

#define e_sync_time_VAL_sync_time_updte(val)                   REG_FLD_VAL(e_sync_time_FLD_sync_time_updte, (val))
#define e_sync_time_VAL_sync_time(val)                         REG_FLD_VAL(e_sync_time_FLD_sync_time, (val))

#define e_tx_cal_cnst_VAL_dscvr_gnt_len(val)                   REG_FLD_VAL(e_tx_cal_cnst_FLD_dscvr_gnt_len, (val))
#define e_tx_cal_cnst_VAL_fec_tail_grd(val)                    REG_FLD_VAL(e_tx_cal_cnst_FLD_fec_tail_grd, (val))
#define e_tx_cal_cnst_VAL_tail_grd(val)                        REG_FLD_VAL(e_tx_cal_cnst_FLD_tail_grd, (val))
#define e_tx_cal_cnst_VAL_default_ovrhd(val)                   REG_FLD_VAL(e_tx_cal_cnst_FLD_default_ovrhd, (val))

#define e_laser_onoff_time_VAL_laser_off_time(val)             REG_FLD_VAL(e_laser_onoff_time_FLD_laser_off_time, (val))
#define e_laser_onoff_time_VAL_laser_on_time(val)              REG_FLD_VAL(e_laser_onoff_time_FLD_laser_on_time, (val))

#define e_grd_thrshld_VAL_guard_thrshld(val)                   REG_FLD_VAL(e_grd_thrshld_FLD_guard_thrshld, (val))

#define e_mpcp_timeout_intvl_VAL_mpcp_timeout_intvl(val)       REG_FLD_VAL(e_mpcp_timeout_intvl_FLD_mpcp_timeout_intvl, (val))

#define e_rpt_timeout_intvl_VAL_rpt_timeout_intvl(val)         REG_FLD_VAL(e_rpt_timeout_intvl_FLD_rpt_timeout_intvl, (val))

#define e_max_future_gnt_time_VAL_max_future_gnt_time(val)     REG_FLD_VAL(e_max_future_gnt_time_FLD_max_future_gnt_time, (val))

#define e_min_proc_time_VAL_min_proc_time(val)                 REG_FLD_VAL(e_min_proc_time_FLD_min_proc_time, (val))

#define e_trx_adjust_time1_VAL_tx_tmstp_adj(val)               REG_FLD_VAL(e_trx_adjust_time1_FLD_tx_tmstp_adj, (val))
#define e_trx_adjust_time1_VAL_tx_stm_adj(val)                 REG_FLD_VAL(e_trx_adjust_time1_FLD_tx_stm_adj, (val))

#define e_trx_adjust_time2_VAL_tx_len_adj(val)                 REG_FLD_VAL(e_trx_adjust_time2_FLD_tx_len_adj, (val))
#define e_trx_adjust_time2_VAL_rx_tmstp_adj(val)               REG_FLD_VAL(e_trx_adjust_time2_FLD_rx_tmstp_adj, (val))

#define e_dbg_prb_sel_VAL_probe_bit0_sel(val)                  REG_FLD_VAL(e_dbg_prb_sel_FLD_probe_bit0_sel, (val))
#define e_dbg_prb_sel_VAL_probe_sel(val)                       REG_FLD_VAL(e_dbg_prb_sel_FLD_probe_sel, (val))

#define e_dbg_prb_h32_VAL_probe_h32(val)                       REG_FLD_VAL(e_dbg_prb_h32_FLD_probe_h32, (val))

#define e_dbg_prb_l32_VAL_probe_l32(val)                       REG_FLD_VAL(e_dbg_prb_l32_FLD_probe_l32, (val))

#define e_rxmbi_eth_cnt_VAL_rxmbi_eth_cnt(val)                 REG_FLD_VAL(e_rxmbi_eth_cnt_FLD_rxmbi_eth_cnt, (val))

#define e_rxmpi_eth_cnt_VAL_rxmpi_eth_cnt(val)                 REG_FLD_VAL(e_rxmpi_eth_cnt_FLD_rxmpi_eth_cnt, (val))

#define e_txmbi_eth_cnt_VAL_txmbi_eth_cnt(val)                 REG_FLD_VAL(e_txmbi_eth_cnt_FLD_txmbi_eth_cnt, (val))

#define e_txmpi_eth_cnt_VAL_txmpi_eth_cnt(val)                 REG_FLD_VAL(e_txmpi_eth_cnt_FLD_txmpi_eth_cnt, (val))

#define e_oam_stat_VAL_rx_oam_cnt(val)                         REG_FLD_VAL(e_oam_stat_FLD_rx_oam_cnt, (val))
#define e_oam_stat_VAL_tx_oam_cnt(val)                         REG_FLD_VAL(e_oam_stat_FLD_tx_oam_cnt, (val))

#define e_mpcp_stat_VAL_mpcp_err_cnt(val)                      REG_FLD_VAL(e_mpcp_stat_FLD_mpcp_err_cnt, (val))
#define e_mpcp_stat_VAL_mpcp_rgst_cnt(val)                     REG_FLD_VAL(e_mpcp_stat_FLD_mpcp_rgst_cnt, (val))
#define e_mpcp_stat_VAL_mpcp_dscv_gate_cnt(val)                REG_FLD_VAL(e_mpcp_stat_FLD_mpcp_dscv_gate_cnt, (val))

#define e_mpcp_rgst_stat_VAL_mpcp_rgst_req_cnt(val)            REG_FLD_VAL(e_mpcp_rgst_stat_FLD_mpcp_rgst_req_cnt, (val))
#define e_mpcp_rgst_stat_VAL_mpcp_rgst_ack_cnt(val)            REG_FLD_VAL(e_mpcp_rgst_stat_FLD_mpcp_rgst_ack_cnt, (val))

#define e_gnt_pending_stat_VAL_max_gnt_pending_cnt(val)        REG_FLD_VAL(e_gnt_pending_stat_FLD_max_gnt_pending_cnt, (val))
#define e_gnt_pending_stat_VAL_cur_gnt_pending_cnt(val)        REG_FLD_VAL(e_gnt_pending_stat_FLD_cur_gnt_pending_cnt, (val))

#define e_gnt_length_stat_VAL_max_gnt_length(val)              REG_FLD_VAL(e_gnt_length_stat_FLD_max_gnt_length, (val))
#define e_gnt_length_stat_VAL_min_gnt_length(val)              REG_FLD_VAL(e_gnt_length_stat_FLD_min_gnt_length, (val))

#define e_gnt_type_stat_VAL_b2b_gnt_cnt(val)                   REG_FLD_VAL(e_gnt_type_stat_FLD_b2b_gnt_cnt, (val))
#define e_gnt_type_stat_VAL_hdn_gnt_cnt(val)                   REG_FLD_VAL(e_gnt_type_stat_FLD_hdn_gnt_cnt, (val))

#define e_time_drft_stat_VAL_cur_time_drift(val)               REG_FLD_VAL(e_time_drft_stat_FLD_cur_time_drift, (val))
#define e_time_drft_stat_VAL_max_time_drift(val)               REG_FLD_VAL(e_time_drft_stat_FLD_max_time_drift, (val))

#define e_llid0_gnt_stat_VAL_llid0_gnt_cnt(val)                REG_FLD_VAL(e_llid0_gnt_stat_FLD_llid0_gnt_cnt, (val))

#define e_llid1_gnt_stat_VAL_llid1_gnt_cnt(val)                REG_FLD_VAL(e_llid1_gnt_stat_FLD_llid1_gnt_cnt, (val))

#define e_llid2_gnt_stat_VAL_llid2_gnt_cnt(val)                REG_FLD_VAL(e_llid2_gnt_stat_FLD_llid2_gnt_cnt, (val))

#define e_llid3_gnt_stat_VAL_llid3_gnt_cnt(val)                REG_FLD_VAL(e_llid3_gnt_stat_FLD_llid3_gnt_cnt, (val))

#define e_llid4_gnt_stat_VAL_llid4_gnt_cnt(val)                REG_FLD_VAL(e_llid4_gnt_stat_FLD_llid4_gnt_cnt, (val))

#define e_llid5_gnt_stat_VAL_llid5_gnt_cnt(val)                REG_FLD_VAL(e_llid5_gnt_stat_FLD_llid5_gnt_cnt, (val))

#define e_llid6_gnt_stat_VAL_llid6_gnt_cnt(val)                REG_FLD_VAL(e_llid6_gnt_stat_FLD_llid6_gnt_cnt, (val))

#define e_llid7_gnt_stat_VAL_llid7_gnt_cnt(val)                REG_FLD_VAL(e_llid7_gnt_stat_FLD_llid7_gnt_cnt, (val))

#define e_snf_mpcp_oam_ctl_VAL_snf_mpcp_cap_en(val)            REG_FLD_VAL(e_snf_mpcp_oam_ctl_FLD_snf_mpcp_cap_en, (val))
#define e_snf_mpcp_oam_ctl_VAL_snf_oam_cap_en(val)             REG_FLD_VAL(e_snf_mpcp_oam_ctl_FLD_snf_oam_cap_en, (val))
#define e_snf_mpcp_oam_ctl_VAL_snf_mpcp_oam_cnt_set(val)       REG_FLD_VAL(e_snf_mpcp_oam_ctl_FLD_snf_mpcp_oam_cnt_set, (val))

#define e_rpt_adj_VAL_rpt_bitmap_ctrl(val)                     REG_FLD_VAL(e_rpt_adj_FLD_rpt_bitmap_ctrl, (val))
#define e_rpt_adj_VAL_rpt_bitmap_set(val)                      REG_FLD_VAL(e_rpt_adj_FLD_rpt_bitmap_set, (val))
#define e_rpt_adj_VAL_rpt_qsize_fecoff_adj(val)                REG_FLD_VAL(e_rpt_adj_FLD_rpt_qsize_fecoff_adj, (val))

#define e_rpt_cnt_VAL_rpt_cnt(val)                             REG_FLD_VAL(e_rpt_cnt_FLD_rpt_cnt, (val))

#define e_dyinggsp_w1_VAL_dyinggsp_w1_len_type(val)            REG_FLD_VAL(e_dyinggsp_w1_FLD_dyinggsp_w1_len_type, (val))

#define e_dyinggsp_w2_VAL_dyinggsp_w2(val)                     REG_FLD_VAL(e_dyinggsp_w2_FLD_dyinggsp_w2, (val))

#define e_dyinggsp_w3_VAL_dyinggsp_w3(val)                     REG_FLD_VAL(e_dyinggsp_w3_FLD_dyinggsp_w3, (val))

#define e_dyinggsp_w4_VAL_dyinggsp_w4(val)                     REG_FLD_VAL(e_dyinggsp_w4_FLD_dyinggsp_w4, (val))

#define e_dyinggsp_w5_VAL_dyinggsp_w5(val)                     REG_FLD_VAL(e_dyinggsp_w5_FLD_dyinggsp_w5, (val))

#define e_dyinggsp_w6_VAL_dyinggsp_w6(val)                     REG_FLD_VAL(e_dyinggsp_w6_FLD_dyinggsp_w6, (val))

#define e_dyinggsp_w7_VAL_dyinggsp_w7(val)                     REG_FLD_VAL(e_dyinggsp_w7_FLD_dyinggsp_w7, (val))

#define e_dyinggsp_w8_VAL_dyinggsp_w8(val)                     REG_FLD_VAL(e_dyinggsp_w8_FLD_dyinggsp_w8, (val))

#define e_dyinggsp_w9_VAL_dyinggsp_w9(val)                     REG_FLD_VAL(e_dyinggsp_w9_FLD_dyinggsp_w9, (val))

#define e_dyinggsp_w10_VAL_dyinggsp_w10(val)                   REG_FLD_VAL(e_dyinggsp_w10_FLD_dyinggsp_w10, (val))

#define e_dyinggsp_w11_VAL_dyinggsp_w11(val)                   REG_FLD_VAL(e_dyinggsp_w11_FLD_dyinggsp_w11, (val))

#define e_dyinggsp_w12_VAL_dyinggsp_w12(val)                   REG_FLD_VAL(e_dyinggsp_w12_FLD_dyinggsp_w12, (val))

#define e_oam_kpalv_w1_VAL_oam_kpalv_w1(val)                   REG_FLD_VAL(e_oam_kpalv_w1_FLD_oam_kpalv_w1, (val))

#define e_oam_kpalv_w2_VAL_oam_kpalv_w2(val)                   REG_FLD_VAL(e_oam_kpalv_w2_FLD_oam_kpalv_w2, (val))

#define e_oam_kpalv_w3_VAL_oam_kpalv_w3(val)                   REG_FLD_VAL(e_oam_kpalv_w3_FLD_oam_kpalv_w3, (val))

#define e_oam_kpalv_w4_VAL_oam_kpalv_w4(val)                   REG_FLD_VAL(e_oam_kpalv_w4_FLD_oam_kpalv_w4, (val))

#define e_oam_kpalv_w5_VAL_oam_kpalv_w5(val)                   REG_FLD_VAL(e_oam_kpalv_w5_FLD_oam_kpalv_w5, (val))

#define e_oam_kpalv_w6_VAL_oam_kpalv_w6(val)                   REG_FLD_VAL(e_oam_kpalv_w6_FLD_oam_kpalv_w6, (val))

#define e_oam_kpalv_w7_VAL_oam_kpalv_w7(val)                   REG_FLD_VAL(e_oam_kpalv_w7_FLD_oam_kpalv_w7, (val))

#define e_oam_kpalv_w8_VAL_oam_kpalv_w8(val)                   REG_FLD_VAL(e_oam_kpalv_w8_FLD_oam_kpalv_w8, (val))

#define e_oam_kpalv_w9_VAL_oam_kpalv_w9(val)                   REG_FLD_VAL(e_oam_kpalv_w9_FLD_oam_kpalv_w9, (val))

#define e_oam_kpalv_w10_VAL_oam_kpalv_w10(val)                 REG_FLD_VAL(e_oam_kpalv_w10_FLD_oam_kpalv_w10, (val))

#define e_oam_kpalv_w11_VAL_oam_kpalv_w11(val)                 REG_FLD_VAL(e_oam_kpalv_w11_FLD_oam_kpalv_w11, (val))

#define e_oam_kpalv_w12_VAL_oam_kpalv_w12(val)                 REG_FLD_VAL(e_oam_kpalv_w12_FLD_oam_kpalv_w12, (val))

#define e_oam_kpalv_ctrl_VAL_oam_kpalv_interval(val)           REG_FLD_VAL(e_oam_kpalv_ctrl_FLD_oam_kpalv_interval, (val))
#define e_oam_kpalv_ctrl_VAL_oam_kpalv_sw_trig(val)            REG_FLD_VAL(e_oam_kpalv_ctrl_FLD_oam_kpalv_sw_trig, (val))
#define e_oam_kpalv_ctrl_VAL_oam_kpalv_sw_cfg(val)             REG_FLD_VAL(e_oam_kpalv_ctrl_FLD_oam_kpalv_sw_cfg, (val))
#define e_oam_kpalv_ctrl_VAL_oam_kpalv_en(val)                 REG_FLD_VAL(e_oam_kpalv_ctrl_FLD_oam_kpalv_en, (val))

#define e_tod_1pps_ctrl_VAL_tod_1pps_width_ctrl(val)           REG_FLD_VAL(e_tod_1pps_ctrl_FLD_tod_1pps_width_ctrl, (val))

#define e_short_pkt_ctrl_VAL_short_pkt_size_ctrl(val)          REG_FLD_VAL(e_short_pkt_ctrl_FLD_short_pkt_size_ctrl, (val))

#define e_sniff_sp_tag_VAL_sniffer_sp_tag(val)                 REG_FLD_VAL(e_sniff_sp_tag_FLD_sniffer_sp_tag, (val))

#ifdef __cplusplus
}
#endif

#endif // __EPON_MAC_REGS_H__