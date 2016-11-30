# Support ATE function
ifneq ($(strip $(TCSUPPORT_WLAN_ATE)),)
HAS_ATE=y
else
HAS_ATE=n
endif

# Support ATE NEW TXCONT solution
# NOTE : RT2xxx does not support this feature !
HAS_NEW_TXCONT=n

# Support QA ATE function
ifneq ($(strip $(TCSUPPORT_WLAN_QATOOL)),)
HAS_QA_SUPPORT=y
else
HAS_QA_SUPPORT=n
endif

HAS_RSSI_FEEDBACK=n

# Support XLINK mode
HAS_XLINK=n

# Support WSC function
ifneq ($(strip $(TCSUPPORT_WLAN_WPS)),)
HAS_WSC=y
HAS_WSC_V2=y
else
HAS_WSC=n
HAS_WSC_V2=n
endif
HAS_WSC_LED=n


# Support LLTD function
HAS_LLTD=y

ifneq ($(strip $(TCSUPPORT_WLAN_WDS)),)
# Support WDS function
HAS_WDS=y
else
HAS_WDS=n
endif

ifneq ($(strip $(TCSUPPORT_WLAN_8021X)),)
# Support 8021X function
HAS_8021X=y
else
HAS_8021X=n
endif


# Support AP-Client function
HAS_APCLI=n



#Support Net interface block while Tx-Sw queue full
HAS_BLOCK_NET_IF=n

#Support drop-mask function
HAS_MASK_SUPPORT=y

#Support multi-client function
MULTI_CLIENT_SUPPORT=y

#Support IGMP-Snooping function.
ifneq ($(strip $(TCSUPPORT_WLAN_IGMPSNOOP)),)
HAS_IGMP_SNOOP_SUPPORT=y
else
HAS_IGMP_SNOOP_SUPPORT=n
endif

#Support DFS function
HAS_DFS_SUPPORT=n

#Support Carrier-Sense function
HAS_CS_SUPPORT=n


# Support user specific transmit rate of Multicast packet.
HAS_MCAST_RATE_SPECIFIC_SUPPORT=n

# Support for Multiple Cards
HAS_MC_SUPPORT=n

#Support for PCI-MSI
HAS_MSI_SUPPORT=n

# Support for WMM ACM v1.1
HAS_WMM_ACM_SUPPORT=n

#Support for IEEE802.11e DLS
HAS_QOS_DLS_SUPPORT=n

#Support for EXT_CHANNEL
ifneq ($(strip $(TCSUPPORT_SINGLE_SKU)),)
HAS_EXT_BUILD_CHANNEL_LIST=y
else
HAS_EXT_BUILD_CHANNEL_LIST=n
endif

#Support for IDS 
HAS_IDS_SUPPORT=n


#Support for Net-SNMP
HAS_SNMP_SUPPORT=n

#Support features of 802.11n Draft3
HAS_DOT11N_DRAFT3_SUPPORT=y

#Support features of Single SKU. 
ifneq ($(strip $(TCSUPPORT_SINGLE_SKU)),)
HAS_SINGLE_SKU_SUPPORT=y
else
HAS_SINGLE_SKU_SUPPORT=n
endif

#Support features of 802.11n
HAS_DOT11_N_SUPPORT=y

#Support for WAPI
HAS_WAPI_SUPPORT=n


#Support for 2860/2880 co-exist 
HAS_RT2880_RT2860_COEXIST=n

HAS_KTHREAD_SUPPORT=n




#Support for Auto channel select enhance
HAS_AUTO_CH_SELECT_ENHANCE=n

#Support statistics count
HAS_STATS_COUNT=y


#Energe Monitor for Bandwidth Adaptation
#HAS_ED_MONITOR_SUPPORT=y
#Support Antenna Diversity
HAS_ANTENNA_DIVERSITY_SUPPORT=n

#Client support WDS function
HAS_CLIENT_WDS_SUPPORT=n

#Support for Bridge Fast Path & Bridge Fast Path function open to other module
HAS_BGFP_SUPPORT=n
HAS_BGFP_OPEN_SUPPORT=n

# Support HOSTAPD function
HAS_HOSTAPD_SUPPORT=n

#Support GreenAP function
ifneq ($(strip $(TCSUPPORT_POWERSAVE_ENABLE)),)
HAS_GREENAP_SUPPORT=y
else
HAS_GREENAP_SUPPORT=n
endif
#Support MAC80211 LINUX-only function
HAS_CFG80211_SUPPORT=n

#Support RFKILL hardware block/unblock LINUX-only function
HAS_RFKILL_HW_SUPPORT=n



HAS_RTMP_FLASH_SUPPORT=n

HAS_RESOURCE_PRE_ALLOC=n
#################################################

CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld

WFLAGS := -DAGGREGATION_SUPPORT -DPIGGYBACK_SUPPORT -DWMM_SUPPORT  -DLINUX -Wall -Wstrict-prototypes -Wno-trigraphs 
WFLAGS += -DSYSTEM_LOG_SUPPORT -DLED_CONTROL_SUPPORT

ifeq ($(HAS_MASK_SUPPORT),y)
WFLAGS += -DDROP_MASK_SUPPORT
endif

ifeq ($(MULTI_CLIENT_SUPPORT),y)
WFLAGS += -DMULTI_CLIENT_SUPPORT
endif

ifeq ($(HAS_RESOURCE_PRE_ALLOC),y)
WFLAGS += -DRESOURCE_PRE_ALLOC
endif

ifeq ($(HAS_KTHREAD_SUPPORT),y)
WFLAGS += -DKTHREAD_SUPPORT
endif

ifeq ($(HAS_RTMP_FLASH_SUPPORT),y)
WFLAGS += -DRTMP_FLASH_SUPPORT
endif

ifeq ($(HAS_ED_MONITOR_SUPPORT),y)
WFLAGS += -DED_MONITOR
endif
# config for AP mode

ifeq ($(RT28xx_MODE),AP)
WFLAGS += -DCONFIG_AP_SUPPORT  -DUAPSD_AP_SUPPORT -DMBSS_SUPPORT -DIAPP_SUPPORT -DAP_SCAN_SUPPORT
#WFLAGS += -DDBG
#WFLAGS += -DMEMORY_OPTIMIZATION

ifeq ($(HAS_HOSTAPD_SUPPORT),y)
WFLAGS += -DHOSTAPD_SUPPORT
endif

ifeq ($(HAS_ATE),y)
WFLAGS += -DRALINK_ATE
WFLAGS += -DCONFIG_RT2880_ATE_CMD_NEW
ifeq ($(HAS_NEW_TXCONT),y)
WFLAGS += -DNEW_TXCONT
endif
ifeq ($(HAS_QA_SUPPORT),y)
WFLAGS += -DRALINK_QA
endif
endif

ifeq ($(HAS_RSSI_FEEDBACK),y)
WFLAGS += -DRSSI_FEEDBACK
endif




ifeq ($(HAS_WSC),y)
WFLAGS += -DWSC_AP_SUPPORT
ifeq ($(HAS_WSC_V2),y)
WFLAGS += -DWSC_V2_SUPPORT
endif
ifeq ($(HAS_WSC_LED),y)
WFLAGS += -DWSC_LED_SUPPORT
endif
endif


ifeq ($(HAS_WDS),y)
WFLAGS += -DWDS_SUPPORT
endif

ifeq ($(HAS_8021X),y)
WFLAGS += -DDOT1X_SUPPORT
endif

ifeq ($(HAS_APCLI),y)
WFLAGS += -DAPCLI_SUPPORT -DMAT_SUPPORT -DAP_SCAN_SUPPORT
#ifeq ($(HAS_ETH_CONVERT_SUPPORT), y)
#WFLAGS += -DETH_CONVERT_SUPPORT
endif

ifeq ($(HAS_IGMP_SNOOP_SUPPORT),y)
WFLAGS += -DIGMP_SNOOP_SUPPORT
endif

ifeq ($(HAS_CS_SUPPORT),y)
WFLAGS += -DCARRIER_DETECTION_SUPPORT
endif

ifeq ($(HAS_MCAST_RATE_SPECIFIC_SUPPORT), y)
WFLAGS += -DMCAST_RATE_SPECIFIC
endif

ifeq ($(CHIPSET),2860)
ifeq ($(HAS_MSI_SUPPORT),y)
WFLAGS += -DPCI_MSI_SUPPORT
endif
endif

ifeq ($(HAS_WMM_ACM_SUPPORT),y)
WFLAGS += -DWMM_ACM_SUPPORT
endif

ifeq ($(HAS_QOS_DLS_SUPPORT),y)
WFLAGS += -DQOS_DLS_SUPPORT
endif

ifeq ($(HAS_SNMP_SUPPORT),y)
WFLAGS += -DSNMP_SUPPORT
endif


ifeq ($(HAS_SINGLE_SKU_SUPPORT),y)
WFLAGS += -DSINGLE_SKU
endif

ifeq ($(HAS_DOT11_N_SUPPORT),y)
WFLAGS += -DDOT11_N_SUPPORT
ifeq ($(HAS_DOT11N_DRAFT3_SUPPORT),y)
WFLAGS += -DDOT11N_DRAFT3
endif
endif
ifeq ($(HAS_GREENAP_SUPPORT),y)
WFLAGS += -DGREENAP_SUPPORT
endif

ifeq ($(HAS_AUTO_CH_SELECT_ENHANCE),y)
WFLAGS += -DAUTO_CH_SELECT_ENHANCE
endif

ifeq ($(HAS_STATS_COUNT),y)
WFLAGS += -DSTATS_COUNT_SUPPORT
endif

ifeq ($(HAS_CFG80211_SUPPORT),y)
WFLAGS += -DRT_CFG80211_SUPPORT
ifeq ($(HAS_RFKILL_HW_SUPPORT),y)
WFLAGS += -DRFKILL_HW_SUPPORT
endif
endif

ifeq ($(OSABL),YES)
WFLAGS += -DOS_ABL_SUPPORT
endif

ifeq ($(HAS_ANTENNA_DIVERSITY_SUPPORT),y)
WFLAGS += -DANT_DIVERSITY_SUPPORT
endif


ifeq ($(LED_WPSSPEC_COMPLY),1)
WFLAGS += -DLED_WPSSPEC_COMPLY
endif

endif #// endif of RT2860_MODE == AP //

#################################################


#################################################

#################################################

#
# Common compiler flag
#






ifeq ($(HAS_EXT_BUILD_CHANNEL_LIST),y)
WFLAGS += -DEXT_BUILD_CHANNEL_LIST
endif

ifeq ($(HAS_IDS_SUPPORT),y)
WFLAGS += -DIDS_SUPPORT
endif

ifeq ($(HAS_WAPI_SUPPORT),y)
WFLAGS += -DWAPI_SUPPORT -DSOFT_ENCRYPT -DEXPORT_SYMTAB
endif

ifeq ($(OSABL),YES)
WFLAGS += -DEXPORT_SYMTAB
endif

ifeq ($(HAS_CLIENT_WDS_SUPPORT),y)
WFLAGS += -DCLIENT_WDS
endif

ifeq ($(HAS_BGFP_SUPPORT),y)
WFLAGS += -DBG_FT_SUPPORT
endif

ifeq ($(HAS_BGFP_OPEN_SUPPORT),y)
WFLAGS += -DBG_FT_OPEN_SUPPORT
endif


#################################################
# ChipSet specific definitions.
#
ifeq ($(CHIPSET),2860)
WFLAGS +=-DRTMP_MAC_PCI -DRTMP_PCI_SUPPORT -DRT2860 -DRT28xx -DA_BAND_SUPPORT
CHIPSET_DAT = 2860
ifeq ($(HAS_DFS_SUPPORT),y)
WFLAGS += -DDFS_SOFTWARE_SUPPORT
endif
endif

ifeq ($(CHIPSET),3090)
WFLAGS +=-DRTMP_MAC_PCI -DRT30xx -DRT3090  -DRTMP_PCI_SUPPORT -DRTMP_RF_RW_SUPPORT -DRTMP_EFUSE_SUPPORT
CHIPSET_DAT = 2860
endif

ifeq ($(CHIPSET),5390)
WFLAGS +=-DRTMP_MAC_PCI -DRT30xx -DRT33xx -DRT3090 -DRT3390 -DRT53xx -DRT5390 -DRTMP_PCI_SUPPORT -DRTMP_RF_RW_SUPPORT -DRTMP_EFUSE_SUPPORT -DSPECIFIC_BCN_BUF_SUPPORT -DRTMP_FREQ_CALIBRATION_SUPPORT -DRTMP_INTERNAL_TX_ALC -DIQ_CAL_SUPPORT
CHIPSET_DAT = 2860
endif

ifeq ($(CHIPSET),5370)
WFLAGS +=-DRTMP_MAC_USB -DRT30xx -DRT33xx -DRT3070 -DRT3370 -DRT53xx -DRT5370 -DRTMP_USB_SUPPORT -DRTMP_TIMER_TASK_SUPPORT -DRTMP_RF_RW_SUPPORT -DRTMP_EFUSE_SUPPORT -DRTMP_INTERNAL_TX_ALC -DSPECIFIC_BCN_BUF_SUPPORT -DRTMP_FREQ_CALIBRATION_SUPPORT -DIQ_CAL_SUPPORT
CHIPSET_DAT = 2870
endif

ifeq ($(CHIPSET),2870)
WFLAGS +=-DRTMP_MAC_USB -DRTMP_USB_SUPPORT -DRT2870 -DRT28xx -DRTMP_TIMER_TASK_SUPPORT -DA_BAND_SUPPORT
CHIPSET_DAT = 2870
ifeq ($(HAS_DFS_SUPPORT),y)
WFLAGS += -DDFS_SOFTWARE_SUPPORT
endif

endif

ifeq ($(CHIPSET),2070)
WFLAGS +=-DRTMP_MAC_USB -DRT30xx -DRT3070 -DRT2070 -DRTMP_USB_SUPPORT -DRTMP_TIMER_TASK_SUPPORT -DRTMP_RF_RW_SUPPORT -DRTMP_EFUSE_SUPPORT
CHIPSET_DAT = 2870
endif

ifeq ($(CHIPSET),3070)
WFLAGS +=-DRTMP_MAC_USB -DRT30xx -DRT3070 -DRTMP_USB_SUPPORT -DRTMP_TIMER_TASK_SUPPORT -DRTMP_RF_RW_SUPPORT -DRTMP_EFUSE_SUPPORT
CHIPSET_DAT = 2870
endif

ifeq ($(CHIPSET),2880)
WFLAGS += -DRT2880 -DRT28xx -DRTMP_MAC_PCI -DCONFIG_RALINK_RT2880_MP -DRTMP_RBUS_SUPPORT -DMERGE_ARCH_TEAM -DA_BAND_SUPPORT
ifeq ($(HAS_DFS_SUPPORT),y)
WFLAGS += -DDFS_HARDWARE_SUPPORT -DDFS_FCC_BW40_FIX -DDFS_DEBUG
endif
endif

ifeq ($(CHIPSET),3572)
WFLAGS +=-DRTMP_MAC_USB -DRTMP_USB_SUPPORT -DRT2870 -DRT28xx -DRT30xx -DRT35xx -DRTMP_TIMER_TASK_SUPPORT -DRTMP_RF_RW_SUPPORT -DRTMP_EFUSE_SUPPORT -DA_BAND_SUPPORT
CHIPSET_DAT = 2870
ifeq ($(HAS_DFS_SUPPORT),y)
WFLAGS += -DDFS_SOFTWARE_SUPPORT
endif

endif

ifeq ($(CHIPSET),3062)
WFLAGS +=-DRTMP_MAC_PCI -DRT2860 -DRT28xx -DRT30xx -DRT35xx -DRTMP_PCI_SUPPORT -DRTMP_RF_RW_SUPPORT -DRTMP_EFUSE_SUPPORT
CHIPSET_DAT = 2860
endif

ifeq ($(CHIPSET),3562)
WFLAGS +=-DRTMP_MAC_PCI -DRT2860 -DRT28xx -DRT30xx -DRT35xx -DRTMP_PCI_SUPPORT -DRTMP_RF_RW_SUPPORT -DRTMP_EFUSE_SUPPORT -DA_BAND_SUPPORT
ifeq ($(HAS_DFS_SUPPORT),y)
WFLAGS += -DDFS_HARDWARE_SUPPORT  -DDFS_DEBUG 
endif

CHIPSET_DAT = 2860
endif

ifeq ($(CHIPSET),3593)
WFLAGS +=-DRTMP_MAC_PCI -DDOT11N_SS3_SUPPORT -DNEW_RATE_ADAPT_SUPPORT -DRT3593 -DRT28xx -DRT30xx -DRT35xx -DRTMP_PCI_SUPPORT -DRTMP_RF_RW_SUPPORT -DRTMP_EFUSE_SUPPORT -DA_BAND_SUPPORT
CHIPSET_DAT = 2860
endif

ifeq ($(CHIPSET),3390)
WFLAGS +=-DRTMP_MAC_PCI -DRT30xx -DRT33xx -DRT3090 -DRT3390 -DRTMP_PCI_SUPPORT -DRTMP_RF_RW_SUPPORT -DRTMP_EFUSE_SUPPORT -DRTMP_INTERNAL_TX_ALC
CHIPSET_DAT = 2860
endif

ifeq ($(CHIPSET),3370)
WFLAGS +=-DRTMP_MAC_USB -DRT30xx -DRT33xx -DRT3070 -DRT3370 -DRTMP_USB_SUPPORT -DRTMP_TIMER_TASK_SUPPORT -DRTMP_RF_RW_SUPPORT -DRTMP_EFUSE_SUPPORT -DRTMP_INTERNAL_TX_ALC
CHIPSET_DAT = 2870
endif

ifeq ($(CHIPSET), 3052)
WFLAGS += -DRTMP_MAC_PCI -DRTMP_RBUS_SUPPORT -DRT3052 -DRT305x -DRTMP_RF_RW_SUPPORT
CHIPSET_DAT = 2870
endif
#################################################


ifeq ($(PLATFORM),5VT)
#WFLAGS += -DCONFIG_5VT_ENHANCE
endif

ifeq ($(HAS_BLOCK_NET_IF),y)
WFLAGS += -DBLOCK_NET_IF
endif

ifeq ($(HAS_DFS_SUPPORT),y)
WFLAGS += -DDFS_SUPPORT
endif

ifeq ($(HAS_MC_SUPPORT),y)
WFLAGS += -DMULTIPLE_CARD_SUPPORT
endif

ifeq ($(HAS_LLTD),y)
WFLAGS += -DLLTD_SUPPORT
endif

ifeq ($(PLATFORM),RMI)
WFLAGS += -DRT_BIG_ENDIAN 
endif

ifeq ($(PLATFORM),BL2348)
WFLAGS += -DRT_BIG_ENDIAN
endif

ifeq ($(PLATFORM),BLUBB)
WFLAGS += -DRT_BIG_ENDIAN
endif

ifeq ($(PLATFORM),BLPMP)
WFLAGS += -DRT_BIG_ENDIAN
endif

ifeq ($(PLATFORM),RMI_64)
WFLAGS += -DRT_BIG_ENDIAN 
endif
ifeq ($(PLATFORM),IXP)
WFLAGS += -DRT_BIG_ENDIAN
endif

ifeq ($(PLATFORM),IKANOS_V160)
WFLAGS += -DRT_BIG_ENDIAN -DIKANOS_VX_1X0
endif

ifeq ($(PLATFORM),IKANOS_V180)
WFLAGS += -DRT_BIG_ENDIAN -DIKANOS_VX_1X0
endif

ifeq ($(PLATFORM),INF_TWINPASS)
WFLAGS += -DRT_BIG_ENDIAN -DINF_TWINPASS
endif

ifeq ($(PLATFORM),INF_DANUBE)
ifneq (,$(findstring 2.4,$(LINUX_SRC)))
# Linux 2.4
WFLAGS += -DINF_DANUBE -DRT_BIG_ENDIAN
else
# Linux 2.6
WFLAGS += -DRT_BIG_ENDIAN
endif
endif

ifeq ($(PLATFORM),INF_AR9)
WFLAGS += -DRT_BIG_ENDIAN -DINF_AR9
# support MAPI function for AR9.
#WFLAGS += -DAR9_MAPI_SUPPORT
endif

ifeq ($(PLATFORM),INF_VR9)
WFLAGS += -DRT_BIG_ENDIAN -DINF_AR9 -DINF_VR9
endif

ifeq ($(PLATFORM),CAVM_OCTEON)
WFLAGS += -DRT_BIG_ENDIAN
endif

ifeq ($(PLATFORM),BRCM_6358)
WFLAGS += -DRT_BIG_ENDIAN
endif

ifeq ($(PLATFORM),INF_AMAZON_SE)
WFLAGS += -DRT_BIG_ENDIAN -DINF_AMAZON_SE
endif

ifeq ($(PLATFORM),RALINK_3052)
WFLAGS += -DPLATFORM_RALINK_3052
endif

ifeq ($(PLATFORM),FREESCALE8377)
#EXTRA_CFLAGS := -v -I$(RT28xx_DIR)/include -I$(LINUX_SRC)/include $(WFLAGS)-O2 -Wall -Wstrict-prototypes -Wno-trigraphs 
#export EXTRA_CFLAGS
WFLAGS += -DRT_BIG_ENDIAN
EXTRA_CFLAGS := $(WFLAGS) -I$(RT28xx_DIR)/include
endif

ifeq ($(PLATFORM),ST)
#WFLAGS += -DST
WFLAGS += -DST
endif

#kernel build options for 2.4
# move to Makefile outside LINUX_SRC := /opt/star/kernel/linux-2.4.27-star

ifeq ($(PLATFORM),RALINK_3052)
EXTRA_CFLAGS := -D__KERNEL__ -I$(LINUX_SRC)/include/asm-mips/mach-generic -I$(LINUX_SRC)/include -I$(RT28xx_DIR)/include -Wall -Wstrict-prototypes -Wno-trigraphs -O2 -fno-strict-aliasing -fno-common -fomit-frame-pointer -G 0 -mno-abicalls -fno-pic -pipe  -finline-limit=100000 -march=mips2 -mabi=32 -Wa,--trap -DLINUX -nostdinc -iwithprefix include $(WFLAGS)
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM), RALINK_2880)
EXTRA_CFLAGS := -D__KERNEL__ -I$(LINUX_SRC)/include -I$(RT28xx_DIR)/include -Wall -Wstrict-prototypes -Wno-trigraphs -O2 -fno-strict-aliasing -fno-common -fomit-frame-pointer -G 0 -mno-abicalls -fno-pic -pipe  -finline-limit=100000 -march=mips2 -mabi=32 -Wa,--trap -DLINUX -nostdinc -iwithprefix include $(WFLAGS)
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),STAR)
EXTRA_CFLAGS := -D__KERNEL__ -I$(LINUX_SRC)/include -I$(RT28xx_DIR)/include -Wall -Wstrict-prototypes -Wno-trigraphs -O2 -fno-strict-aliasing -fno-common -Uarm -fno-common -pipe -mapcs-32 -D__LINUX_ARM_ARCH__=4 -march=armv4  -mshort-load-bytes -msoft-float -Uarm -DMODULE -DMODVERSIONS -include $(LINUX_SRC)/include/linux/modversions.h $(WFLAGS)

export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),SIGMA)
EXTRA_CFLAGS := -D__KERNEL__ -I$(RT28xx_DIR)/include -I$(LINUX_SRC)/include -I$(LINUX_SRC)/include/asm/gcc -I$(LINUX_SRC)/include/asm-mips/mach-tango2 -I$(LINUX_SRC)/include/asm-mips/mach-tango2 -DEM86XX_CHIP=EM86XX_CHIPID_TANGO2 -DEM86XX_REVISION=6 -I$(LINUX_SRC)/include/asm-mips/mach-generic -I$(RT2860_DIR)/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -ffreestanding -O2     -fomit-frame-pointer -G 0 -mno-abicalls -fno-pic -pipe  -mabi=32 -march=mips32r2 -Wa,-32 -Wa,-march=mips32r2 -Wa,-mips32r2 -Wa,--trap -DMODULE $(WFLAGS) -DSIGMA863X_PLATFORM

export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),SIGMA_8622)
EXTRA_CFLAGS := -D__KERNEL__ -I$(CROSS_COMPILE_INCLUDE)/include -I$(LINUX_SRC)/include -I$(RT28xx_DIR)/include -Wall -Wstrict-prototypes -Wno-trigraphs -O2 -fno-strict-aliasing -fno-common -fno-common -pipe -fno-builtin -D__linux__ -DNO_MM -mapcs-32 -march=armv4 -mtune=arm7tdmi -msoft-float -DMODULE -mshort-load-bytes -nostdinc -iwithprefix -DMODULE $(WFLAGS)
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),5VT)
EXTRA_CFLAGS := -D__KERNEL__ -I$(LINUX_SRC)/include -I$(RT28xx_DIR)/include -mlittle-endian -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -O3 -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mabi=apcs-gnu -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5te -mtune=arm926ej-s --param max-inline-insns-single=40000  -Uarm -Wdeclaration-after-statement -Wno-pointer-sign -DMODULE $(WFLAGS) 

export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),IKANOS_V160)
EXTRA_CFLAGS := -D__KERNEL__ -I$(LINUX_SRC)/include -I$(LINUX_SRC)/include/asm/gcc -I$(LINUX_SRC)/include/asm-mips/mach-tango2 -I$(LINUX_SRC)/include/asm-mips/mach-tango2 -I$(LINUX_SRC)/include/asm-mips/mach-generic -I$(RT28xx_DIR)/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -ffreestanding -O2 -fomit-frame-pointer -G 0 -mno-abicalls -fno-pic -pipe -march=lx4189 -Wa, -DMODULE $(WFLAGS)
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),IKANOS_V180)
EXTRA_CFLAGS := -D__KERNEL__ -I$(LINUX_SRC)/include -I$(LINUX_SRC)/include/asm/gcc -I$(LINUX_SRC)/include/asm-mips/mach-tango2 -I$(LINUX_SRC)/include/asm-mips/mach-tango2 -I$(LINUX_SRC)/include/asm-mips/mach-generic -I$(RT28xx_DIR)/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -ffreestanding -O2 -fomit-frame-pointer -G 0 -mno-abicalls -fno-pic -pipe -mips32r2 -Wa, -DMODULE $(WFLAGS)
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),INF_TWINPASS)
EXTRA_CFLAGS := -D__KERNEL__ -DMODULE -I$(LINUX_SRC)/include -I$(RT28xx_DIR)/include -Wall -Wstrict-prototypes -Wno-trigraphs -O2 -fomit-frame-pointer -fno-strict-aliasing -fno-common -G 0 -mno-abicalls -fno-pic -march=4kc -mips32 -Wa,--trap -pipe -mlong-calls $(WFLAGS)
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),INF_DANUBE)
	ifneq (,$(findstring 2.4,$(LINUX_SRC)))
	EXTRA_CFLAGS := -I$(RT28xx_DIR)/include $(WFLAGS) -Wundef -fno-strict-aliasing -fno-common -ffreestanding -Os -fomit-frame-pointer -G 0 -mno-abicalls -fno-pic -pipe -msoft-float  -mabi=32 -march=mips32 -Wa,-32 -Wa,-march=mips32 -Wa,-mips32 -Wa,--trap -I$(LINUX_SRC)/include/asm-mips/mach-generic
	else
	EXTRA_CFLAGS := -I$(RT28xx_DIR)/include $(WFLAGS) -Wundef -fno-strict-aliasing -fno-common -ffreestanding -Os -fomit-frame-pointer -G 0 -mno-abicalls -fno-pic -pipe -msoft-float  -mabi=32 -march=mips32r2 -Wa,-32 -Wa,-march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I$(LINUX_SRC)/include/asm-mips/mach-generic
	endif
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),INF_AR9)
EXTRA_CFLAGS := -I$(RT28xx_DIR)/include $(WFLAGS) -Wundef -fno-strict-aliasing -fno-common -fno-pic -ffreestanding -Os -fomit-frame-pointer -G 0 -mno-abicalls -fno-pic -pipe -msoft-float  -mabi=32 -mlong-calls -march=mips32r2 -mtune=34kc -march=mips32r2 -Wa,-32 -Wa,-march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I$(LINUX_SRC)/include/asm-mips/mach-generic
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),INF_VR9)
EXTRA_CFLAGS := -I$(RT28xx_DIR)/include $(WFLAGS) -Wundef -fno-strict-aliasing -fno-common -fno-pic -ffreestanding -Os -fomit-frame-pointer -G 0 -mno-abicalls -fno-pic -pipe -msoft-float  -mabi=32 -mlong-calls -march=mips32r2 -march=mips32r2 -Wa,-32 -Wa,-march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I$(LINUX_SRC)/include/asm-mips/mach-generic
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),BRCM_6358)
EXTRA_CFLAGS := $(WFLAGS) -I$(RT28xx_DIR)/include -nostdinc -iwithprefix include -D__KERNEL__ -Wall -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -I $(LINUX_SRC)/include/asm/gcc -G 0 -mno-abicalls -fno-pic -pipe  -finline-limit=100000 -mabi=32 -march=mips32 -Wa,-32 -Wa,-march=mips32 -Wa,-mips32 -Wa,--trap -I$(LINUX_SRC)/include/asm-mips/mach-bcm963xx -I$(LINUX_SRC)/include/asm-mips/mach-generic  -Os -fomit-frame-pointer -Wdeclaration-after-statement  -DMODULE -mlong-calls
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),INF_AMAZON_SE)
EXTRA_CFLAGS := -D__KERNEL__ -DMODULE=1 -I$(LINUX_SRC)/include -I$(RT28xx_DIR)/include -Wall -Wstrict-prototypes -Wno-trigraphs -O2 -fno-strict-aliasing -fno-common -DCONFIG_IFX_ALG_QOS -DCONFIG_WAN_VLAN_SUPPORT -fomit-frame-pointer -DIFX_PPPOE_FRAME -G 0 -fno-pic -mno-abicalls -mlong-calls -pipe -finline-limit=100000 -mabi=32 -march=mips32 -Wa,-32 -Wa,-march=mips32 -Wa,-mips32 -Wa,--trap -nostdinc -iwithprefix include $(WFLAGS)
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),ST)
EXTRA_CFLAGS := -D__KERNEL__ -I$(LINUX_SRC)/include -I$(RT28xx_DIR)/include -Wall -O2 -Wundef -Wstrict-prototypes -Wno-trigraphs -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-aliasing -fno-common -fomit-frame-pointer -ffreestanding -m4-nofpu -o $(WFLAGS) 
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),PC)
    ifneq (,$(findstring 2.4,$(LINUX_SRC)))
	# Linux 2.4
	EXTRA_CFLAGS := -D__KERNEL__ -I$(LINUX_SRC)/include -I$(RT28xx_DIR)/include -O2 -fomit-frame-pointer -fno-strict-aliasing -fno-common -pipe -mpreferred-stack-boundary=2 -march=i686 -DMODULE -DMODVERSIONS -include $(LINUX_SRC)/include/linux/modversions.h $(WFLAGS)
	export CFLAGS
    else
	# Linux 2.6
	EXTRA_CFLAGS := $(WFLAGS) -I$(RT28xx_DIR)/include
    endif
endif

#If the kernel version of RMI is newer than 2.6.27, please change "CFLAGS" to "EXTRA_FLAGS"
ifeq ($(PLATFORM),RMI)
EXTRA_CFLAGS := -D__KERNEL__ -DMODULE=1 -I$(LINUX_SRC)/include -I$(LINUX_SRC)/include/asm-mips/mach-generic  -I$(RT28xx_DIR)/include -Wall -Wstrict-prototypes -Wno-trigraphs -O2 -fno-strict-aliasing -fno-common -DCONFIG_IFX_ALG_QOS -DCONFIG_WAN_VLAN_SUPPORT -fomit-frame-pointer -DIFX_PPPOE_FRAME -G 0 -fno-pic -mno-abicalls -mlong-calls -pipe -finline-limit=100000 -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -march=xlr -ffreestanding  -march=xlr -Wa,--trap, -nostdinc -iwithprefix include $(WFLAGS)
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),RMI_64)
EXTRA_CFLAGS := -D__KERNEL__ -DMODULE=1 -I$(LINUX_SRC)/include -I$(LINUX_SRC)/include/asm-mips/mach-generic  -I$(RT28xx_DIR)/include -Wall -Wstrict-prototypes -Wno-trigraphs -O2 -fno-strict-aliasing -fno-common -DCONFIG_IFX_ALG_QOS -DCONFIG_WAN_VLAN_SUPPORT -fomit-frame-pointer -DIFX_PPPOE_FRAME -G 0 -fno-pic -mno-abicalls -mlong-calls -pipe -finline-limit=100000 -mabi=64 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -march=xlr -ffreestanding  -march=xlr -Wa,--trap, -nostdinc -iwithprefix include $(WFLAGS)
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),IXP)
	CFLAGS := -v -D__KERNEL__ -DMODULE -I$(LINUX_SRC)/include -I$(RT28xx_DIR)/include -mbig-endian -Wall -Wstrict-prototypes -Wno-trigraphs -O2 -fno-strict-aliasing -fno-common -Uarm -fno-common -pipe -mapcs-32 -D__LINUX_ARM_ARCH__=5 -mcpu=xscale -mtune=xscale -malignment-traps -msoft-float $(WFLAGS)
        EXTRA_CFLAGS := -v $(WFLAGS) -I$(RT28xx_DIR)/include -mbig-endian
	export CFLAGS        
endif

ifeq ($(PLATFORM),SMDK)
        EXTRA_CFLAGS := $(WFLAGS) -I$(RT28xx_DIR)/include
endif

ifeq ($(PLATFORM),CAVM_OCTEON)
	EXTRA_CFLAGS := $(WFLAGS) -I$(RT28xx_DIR)/include \
				    -mabi=64 $(WFLAGS)
export CFLAGS
endif


ifeq ($(PLATFORM),BB_SOC)
WFLAGS += -DRT_BIG_ENDIAN -DBB_SOC -DWIFI_MODULE
ifeq ($(CONFIG_MIPS_TC3262),y)
WFLAGS += -DMULTI_CORE_SUPPORT
endif
ifeq ($(RALINK_PCIE_ADDR_SWAP),1)
WFLAGS += -DBB_PCIE_ADDR_SWAP
endif
ifeq ($(TCSUPPORT_HWNAT_WIFI),1)
WFLAGS += -DBB_RA_HWNAT_WIFI
endif
endif


ifeq ($(PLATFORM),BB_SOC)
EXTRA_CFLAGS += -I$(RT28xx_DIR)/include $(WFLAGS)
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),DM6446)
	EXTRA_CFLAGS := -nostdinc -iwithprefix include -D__KERNEL__ -I$(RT28xx_DIR)/include -I$(LINUX_SRC)/include  -Wall -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Os -fno-omit-frame-pointer -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mlittle-endian -mabi=apcs-gnu -D__LINUX_ARM_ARCH__=5 -march=armv5te -mtune=arm9tdmi -msoft-float -Uarm -Wdeclaration-after-statement -c -o $(WFLAGS)
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),BL2348)
EXTRA_CFLAGS := -D__KERNEL__ -I$(RT28xx_DIR)/include -I$(LINUX_SRC)/include -I$(LINUX_SRC)/include/asm/gcc -I$(LINUX_SRC)/include/asm-mips/mach-tango2 -I$(LINUX_SRC)/include/asm-mips/mach-tango2 -DEM86XX_CHIP=EM86XX_CHIPID_TANGO2 -DEM86XX_REVISION=6 -I$(LINUX_SRC)/include/asm-mips/mach-generic -I$(RT2860_DIR)/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -ffreestanding -O2     -fomit-frame-pointer -G 0 -mno-abicalls -fno-pic -pipe  -mabi=32 -march=mips32r2 -Wa,-32 -Wa,-march=mips32r2 -Wa,-mips32r2 -Wa,--trap -DMODULE $(WFLAGS) -DSIGMA863X_PLATFORM -DEXPORT_SYMTAB -DPLATFORM_BL2348
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),BLUBB)
EXTRA_CFLAGS := -D__KERNEL__ -I$(RT28xx_DIR)/include -I$(LINUX_SRC)/include -I$(LINUX_SRC)/include/asm/gcc -I$(LINUX_SRC)/include/asm-mips/mach-tango2 -I$(LINUX_SRC)/include/asm-mips/mach-tango2 -DEM86XX_CHIP=EM86XX_CHIPID_TANGO2 -DEM86XX_REVISION=6 -I$(LINUX_SRC)/include/asm-mips/mach-generic -I$(RT2860_DIR)/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -ffreestanding -O2     -fomit-frame-pointer -G 0 -mno-abicalls -fno-pic -pipe  -mabi=32 -march=mips32r2 -Wa,-32 -Wa,-march=mips32r2 -Wa,-mips32r2 -Wa,--trap -DMODULE $(WFLAGS) -DSIGMA863X_PLATFORM -DEXPORT_SYMTAB -DPLATFORM_BL2348
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),BLPMP)
EXTRA_CFLAGS := -D__KERNEL__ -I$(RT28xx_DIR)/include -I$(LINUX_SRC)/include -I$(LINUX_SRC)/include/asm/gcc -I$(LINUX_SRC)/include/asm-mips/mach-tango2 -I$(LINUX_SRC)/include/asm-mips/mach-tango2 -DEM86XX_CHIP=EM86XX_CHIPID_TANGO2 -DEM86XX_REVISION=6 -I$(LINUX_SRC)/include/asm-mips/mach-generic -I$(RT2860_DIR)/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -ffreestanding -O2     -fomit-frame-pointer -G 0 -mno-abicalls -fno-pic -pipe  -mabi=32 -march=mips32r2 -Wa,-32 -Wa,-march=mips32r2 -Wa,-mips32r2 -Wa,--trap -DMODULE $(WFLAGS) -DSIGMA863X_PLATFORM -DEXPORT_SYMTAB
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),MT85XX)
    ifneq (,$(findstring 2.4,$(LINUX_SRC)))
	# Linux 2.4
	CFLAGS := -D__KERNEL__ -I$(LINUX_SRC)/include -I$(RT28xx_DIR)/include -O2 -fomit-frame-pointer -fno-strict-aliasing -fno-common -pipe -mpreferred-stack-boundary=2 -march=i686 -DMODULE -DMODVERSIONS -include $(LINUX_SRC)/include/linux/modversions.h $(WFLAGS)
	export CFLAGS
    else
	# Linux 2.6
	EXTRA_CFLAGS += $(WFLAGS) -I$(RT28xx_DIR)/include
	EXTRA_CFLAGS += -D _NO_TYPEDEF_BOOL_ \
	                -D _NO_TYPEDEF_UCHAR_ \
	                -D _NO_TYPEDEF_UINT8_ \
	                -D _NO_TYPEDEF_UINT16_ \
	                -D _NO_TYPEDEF_UINT32_ \
	                -D _NO_TYPEDEF_UINT64_ \
	                -D _NO_TYPEDEF_CHAR_ \
	                -D _NO_TYPEDEF_INT32_ \
	                -D _NO_TYPEDEF_INT64_ \
	                
    endif
endif

ifeq ($(PLATFORM),NXP_TV550)
    ifneq (,$(findstring 2.4,$(LINUX_SRC)))
        # Linux 2.4
        CFLAGS := -D__KERNEL__ -I$(LINUX_SRC)/include -I$(RT28xx_DIR)/include -O2 -fomit-frame-pointer -fno-strict-aliasing -fno-common -pipe -mpreferred-stack-boundary=2 -march=mips -DMODULE -DMODVERSIONS -include $(LINUX_SRC)/include/linux/modversions.h $(WFLAGS)
        export CFLAGS
    else
        # Linux 2.6
        EXTRA_CFLAGS := $(WFLAGS) -I$(RT28xx_DIR)/include
    endif
endif

ifeq ($(PLATFORM),MVL5)
EXTRA_CFLAGS := -D__KERNEL__ -I$(LINUX_SRC)/include -I$(RT28xx_DIR)/include -mlittle-endian -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -O3 -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5te -mtune=arm926ej-s --param max-inline-insns-single=40000  -Uarm -Wdeclaration-after-statement -Wno-pointer-sign -DMODULE $(WFLAGS) 
export EXTRA_CFLAGS
endif
