/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology	5th	Rd.
 * Science-based Industrial	Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2005, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of Ralink Tech. Any attempt
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

    Module Name:
    rt_config.h

    Abstract:
    Central header file to maintain all include files for all driver routines.

    Revision History:
    Who         When          What
    --------    ----------    ----------------------------------------------
    Rory Chen   02-10-2005    Porting from RT2500

*/

#ifndef __RT_CONFIG_H__
#define __RT_CONFIG_H__

#define PROFILE_PATH                "/etc/Wireless/RT61AP/RT61AP.dat"
#define NIC_DEVICE_NAME             "RT61AP"
#define RT2561_IMAGE_FILE_NAME      "/etc/Wireless/RT61AP/RT2561.bin"
// Super mode, RT2561S: super(high throughput, aggregiation, piggy back), RT2561T: Package type(TQFP)
#define RT2561S_IMAGE_FILE_NAME     "/etc/Wireless/RT61AP/RT2561S.bin"
#define RT2661_IMAGE_FILE_NAME      "/etc/Wireless/RT61AP/RT2661.bin"
#define RALINK_PASSPHRASE           "Ralink"
#define DRIVER_VERSION				"1.1.2.0"

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>

#include <linux/init.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/ethtool.h>
#include <linux/wireless.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/if_arp.h>
#include <linux/ctype.h>
#ifdef TC_SOC
#include <linux/workqueue.h>
#include <linux/mtd/rt_flash.h>
#endif

// load firmware
#define __KERNEL_SYSCALLS__
#include <linux/unistd.h>
#include <asm/uaccess.h>

#if defined(RTL865X_FAST_PATH) && !defined(RTL865X_SOC)
#error ("Use RTL865X_FAST_PATH also need to define RTL865X_SOC")
#endif

#ifdef RTL865X_SOC
#include <re865x/rtl865x/mbuf.h>
#include <re865x/rtl865x/rtl8651_tblDrvFwd.h>
#include <re865x/rtl865x/rtl8651_tblAsicDrv.h>
#include <re865x/rtl865x/rtl8651_tblDrv.h>

#ifdef RTL865X_FAST_PATH
#include "rtl865x_fast.h"
#endif

#ifndef CONFIG_8139CP_VID
#define CONFIG_8139CP_VID	0x09
#endif

#endif	/* RTL865X_SOC */

#ifndef ULONG
#define CHAR            char
#define INT             int
#define SHORT           int
#define UINT            u32
#define ULONG           u32
#define USHORT          u16
#define UCHAR           u8


#define UMAXLONG        unsigned long


#ifndef RTL865X_SOC
#define uint32			u32
#define uint8			u8
#endif

#define BOOLEAN         u8
//#define LARGE_INTEGER s64
#define VOID            void
#define LONG            int
#define LONGLONG        s64
#define ULONGLONG       u64
typedef VOID            *PVOID;
typedef CHAR            *PCHAR;
typedef UCHAR           *PUCHAR;
typedef USHORT          *PUSHORT;
typedef LONG            *PLONG;
typedef ULONG           *PULONG;

typedef union _LARGE_INTEGER {
    struct {
        ULONG LowPart;
        LONG HighPart;
    }vv;
    struct {
        ULONG LowPart;
        LONG HighPart;
    } u;
    s64 QuadPart;
} LARGE_INTEGER;

#endif

#define ETH_LENGTH_OF_ADDRESS	6

#define IN
#define OUT

#define TRUE        1
#define FALSE       0

#define NDIS_STATUS                             INT
#define NDIS_STATUS_SUCCESS                     0x00
#define NDIS_STATUS_FAILURE                     0x01
#define NDIS_STATUS_RESOURCES                   0x03

#include    "rtmp_type.h"
#include    "rtmp_def.h"
#include    "rt2661.h"
#include    "rtmp.h"
#include    "mlme.h"
#include    "oid.h"
#include    "wpa.h"
#include    "md5.h"

#ifdef WSC_SUPPORT
// WSC security code
#include	"sha2.h"
#include	"hmac.h"
#include	"dh_key.h"
#include	"evp_enc.h"

#include    "wsc.h"
#include    "wsc_tlv.h"
#endif // WSC_SUPPORT //

#ifdef APCLI_SUPPORT
#include    "sta_mlme.h"
#endif

#define	NIC2661_PCI_DEVICE_ID	0x0401
#define NIC2561S_PCI_DEVICE_ID		0x0301
#define NIC2561_PCI_DEVICE_ID		0x0302

#define	NIC_PCI_VENDOR_ID		0x1814

#define MEM_ALLOC_FLAG      (GFP_DMA | GFP_ATOMIC)

#endif  // __RT_CONFIG_H__

