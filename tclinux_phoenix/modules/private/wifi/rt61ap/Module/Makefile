#################################################################################
 # Ralink Technology, Inc.	                                         	#
 # 4F, No. 2 Technology 5th Rd.                                          	#
 # Science-based Industrial Park                                         	#
 # Hsin-chu, Taiwan, R.O.C.                                              	#
 #                                                                       	#
 # (c) Copyright 2005, Ralink Technology, Inc.                           	#
 #                                                                       	#
 # All rights reserved. Ralink's source code is an unpublished work and the	#
 # use of a copyright notice does not imply otherwise. This source code		#
 # contains confidential trade secret material of Ralink Tech. Any attempt	#
 # or participation in deciphering, decoding, reverse engineering or in any	#
 # way altering the source code is stricitly prohibited, unless the prior	#
 # written consent of Ralink Technology, Inc. is obtained.			#
#################################################################################

KERNELDIR=$(KERNEL_DIR)

# Support AP client function
#HAS_APCLIENT=y

# Support IGMP snooping function
HAS_IGMPSNOOPING=y

# Support Block Net-If during TxSw queue full.
HAS_BLOCK_NET_IF=y

# Support Carrier Detection
#HAS_CARRIER_DETECTION=y

#ifdef WSC_SUPPORT
# Support WSC function
HAS_WSC=y
#endif // WSC_SUPPORT //

# Support LLTD
#HAS_LLTD=y

WFLAGS = -DMBSS_SUPPORT -DWDS_SUPPORT -DWMM_SUPPORT -DWIFI_TEST -Wall -Wstrict-prototypes -Wno-trigraphs 
#WFLAGS+= -DAGGREGATION_SUPPORT
WFLAGS+= -DBIG_ENDIAN 
WFLAGS+= -DTC_SOC 
#WFLAGS+= -DRALINK_ATE 
#CFLAGS+= -DDBG

ifeq ($(HAS_APCLIENT),y)
WFLAGS += -DAPCLI_SUPPORT
endif

ifeq ($(HAS_IGMPSNOOPING),y)
WFLAGS += -DIGMP_SNOOP_SUPPORT
endif

ifeq ($(HAS_BLOCK_NET_IF),y)
WFLAGS += -DBLOCK_NET_IF
endif

ifeq ($(HAS_CARRIER_DETECTION),y)
WFLAGS += -DCARRIER_DETECTION_SUPPORT
endif

#ifdef WSC_SUPPORT
ifeq ($(HAS_WSC),y)
WFLAGS += -DWSC_SUPPORT
endif
#endif // WSC_SUPPORT //

ifeq ($(HAS_LLTD),y)
WFLAGS += -DLLTD_SUPPORT
endif

CFLAGS+= $(WFLAGS)
#TRENDCHIP
#CFLAGS+= -DTRENDCHIP
#

obj-m := rt61ap.o

rt61ap-objs :=	rtmp_main.o	\
				mlme.o		\
				connect.o	\
				sync.o 		\
				assoc.o		\
				rtmp_task.o	\
				auth.o		\
				auth_rsp.o	\
				rtmp_data.o	\
				rtmp_init.o	\
				sanity.o	\
				rtmp_wep.o	\
				rtmp_info.o	\
				eeprom.o	\
				rtmp_tkip.o	\
				wpa.o		\
				md5.o		\
				soft_ap.o       \
                                dls.o   

#ifdef WSC_SUPPORT
ifeq ($(HAS_WSC),y)
rt61ap-objs += 	wsc.o	\
				wsc_tlv.o	\
				sha2.o	\
				hmac.o	\
				evp_enc.o	\
				dh_key.o
endif
#endif // WSC_SUPPORT //

ifeq ($(HAS_APCLIENT),y)
rt61ap-objs += 	sta_auth.o	\
				sta_assoc.o	\
				sta_sync.o	\
				sta_ctrl.o	\
				rtmp_apcli.o	\
				rtmp_apcli_iparp.o	\
				rtmp_apcli_pppoe.o	\
				sta_mlme.o
endif

ifeq ($(HAS_IGMPSNOOPING),y)
rt61ap-objs += 	rtmp_igmpSnoop.o
endif

ifeq ($(HAS_BLOCK_NET_IF),y)
rt61ap-objs += 	netif_block.o
endif

all: 
	$(MAKE) -C $(KERNELDIR) M=`pwd` modules
	mips-linux-strip --strip-unneeded rt61ap.ko	
#	make -C /lib/modules/$(shell uname -r)/build SUBDIRS=$(shell pwd) modules

clean:
	rm -f *.o *~ .*.cmd *.ko *.mod.c
	
