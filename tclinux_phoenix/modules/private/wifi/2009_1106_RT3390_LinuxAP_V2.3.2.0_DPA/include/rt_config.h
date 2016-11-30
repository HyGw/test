/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2004, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	rt_config.h

	Abstract:
	Central header file to maintain all include files for all NDIS
	miniport driver routines.

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
	Paul Lin    08-01-2002    created

*/
#ifndef	__RT_CONFIG_H__
#define	__RT_CONFIG_H__

//#define VENDOR_FEATURE3_SUPPORT  //For AP mode only


#ifdef VENDOR_FEATURE3_SUPPORT
#undef AP_SCAN_SUPPORT
#undef LED_CONTROL_SUPPORT
#undef AP_LOG_EVENT_SUPPORT
#undef DOT1X_SUPPORT
#undef AP_IWPRIV_COMMAND_SUPPORT
#undef WIRELESS_EVENT_SUPPORT
#undef LINK_LIST_SUPPORT
#define INLINE
#ifdef WSC_LED_SUPPORT
#define LED_CONTROL_SUPPORT
#endif // WSC_LED_SUPPORT
#else // Non VENDOR_FEATURE3_SUPPORT//
#define AP_SCAN_SUPPORT
#define LED_CONTROL_SUPPORT
#define AP_LOG_EVENT_SUPPORT
#define DOT1X_SUPPORT
#define AP_IWPRIV_COMMAND_SUPPORT
#define WIRELESS_EVENT_SUPPORT
#define LINK_LIST_SUPPORT
#define INLINE		inline
#endif // VENDOR_FEATURE3_SUPPORT //

#ifdef TC_SOC
#include <linux/workqueue.h>
#include <linux/mtd/rt_flash.h>
#endif

#include "rtmp_type.h"
#include "rtmp_os.h"

#include "rtmp_def.h"
#include "rtmp_chip.h"
#include "rtmp_timer.h"

#include "oid.h"
#include "mlme.h"
#include "crypt_md5.h"
#include "crypt_sha2.h"
#include "crypt_hmac.h"
#include "crypt_aes.h"
#include "crypt_arc4.h"
#include "rtmp_cmd.h"
#include "rtmp.h"
#include "ap.h"
#include "wpa.h"
#include "dfs.h"
#include "chlist.h"
#include "spectrum.h"
#ifdef CONFIG_AP_SUPPORT
#include "ap_autoChSel.h"
#endif // CONFIG_AP_SUPPORT //

#include "eeprom.h"
#if defined(RTMP_PCI_SUPPORT) || defined(RTMP_USB_SUPPORT)
#include "rtmp_mcu.h"
#endif



#undef AP_WSC_INCLUDED
#undef STA_WSC_INCLUDED
#undef WSC_INCLUDED
#ifdef CONFIG_AP_SUPPORT
#ifdef UAPSD_AP_SUPPORT
#include "ap_uapsd.h"
#endif // UAPSD_AP_SUPPORT //

#ifdef MBSS_SUPPORT
#include "ap_mbss.h"
#endif // MBSS_SUPPORT //

#ifdef WDS_SUPPORT
#include "ap_wds.h"
#endif // WDS_SUPPORT //

#ifdef APCLI_SUPPORT
#include "ap_apcli.h"
#endif // APCLI_SUPPORT //

#ifdef WSC_AP_SUPPORT
#define AP_WSC_INCLUDED
#include <asm/tc3162/ledcetrl.h>
#endif // WSC_AP_SUPPORT //

#include "ap_ids.h"
#include "ap_cfg.h"

#ifdef CLIENT_WDS
#include "client_wds.h"
#endif // CLIENT_WDS //
#endif // CONFIG_AP_SUPPORT //

#ifdef MAT_SUPPORT
#include "mat.h"
#endif // MAT_SUPPORT //

#ifdef WMM_ACM_SUPPORT
#include	"acm_extr.h"
#endif // WMM_ACM_SUPPORT //


#ifdef BLOCK_NET_IF
#include "netif_block.h"
#endif // BLOCK_NET_IF //

#ifdef IGMP_SNOOP_SUPPORT
#include "igmp_snoop.h"
#endif // IGMP_SNOOP_SUPPORT //

#ifdef RALINK_ATE
#include "rt_ate.h"
#endif // RALINK_ATE //

#ifdef RALINK_28xx_QA
#ifndef RALINK_ATE
#error "For supporting QA GUI, please set HAS_ATE=y and HAS_QA_SUPPORT=y."
#endif // RALINK_ATE //
#endif // RALINK_28xx_QA //


#ifdef WAPI_SUPPORT
#include "wapi.h"
#endif // WAPI_SUPPORT //



#if defined(AP_WSC_INCLUDED) || defined(STA_WSC_INCLUDED)
#define WSC_INCLUDED
#endif

#ifdef CONFIG_AP_SUPPORT
#ifdef WDS_SUPPORT
#define RALINK_PASSPHRASE	"Ralink"
#endif // WDS_SUPPORT //
#endif // CONFIG_AP_SUPPORT //


#ifdef WSC_INCLUDED
// WSC security code
//#define OLD_DH_ALGORITHM //Change to old dh algorithm
//#define OLD_AES_ALGORITHM //Change to old aes algorithm
#include "crypt_biginteger.h"
#include "crypt_dh.h"
#include "wsc.h"
#include "wsc_tlv.h"
#endif // WSC_INCLUDED //


#ifdef IKANOS_VX_1X0
#include "vr_ikans.h"
#endif // IKANOS_VX_1X0 //




#endif	// __RT_CONFIG_H__


