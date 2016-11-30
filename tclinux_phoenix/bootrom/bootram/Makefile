

PLATFORM =

ifeq ($(TC3162L2),1)
PLATFORM += -DTC3162L2
endif

ifeq ($(TC3162U),1)
PLATFORM += -DTC3162U
endif

ifeq ($(TC3262),1)
PLATFORM += -DTC3262
ifeq ($(TC3262_FPGA),1)
PLATFORM += -DTC3262_FPGA
endif
ifeq ($(SIS_DDR_PHY),1)
PLATFORM += -DSIS_DDR_PHY
endif
ifeq ($(PLL_AUTO_SCAN),1)
PLATFORM += -DPLL_AUTO_SCAN
endif
ifeq ($(RT63365),1)
PLATFORM += -DRT63365
endif
endif

ifeq ($(TC3262),1)
#CROSS_COMPILE = mips-linux-gnu-
#CROSS_COMPILE = mips-linux-uclibc-
else
CROSS_COMPILE = mips-linux-
endif

ifeq ($(TR068_LED),1)
PLATFORM += -DTR068_LED
endif
ifeq ($(16M),1)
PLATFORM += -DSDRAM_16M
else
ifeq ($(32M),1)
PLATFORM += -DSDRAM_32M
else
ifeq ($(64M),1)
PLATFORM += -DSDRAM_64M
else
ifneq ($(8M),1)
ifeq ($(TC3262),1)
PLATFORM += -DSDRAM_32M
else
PLATFORM += -DSDRAM_16M
endif
else
PLATFORM += -DSDRAM_8M
endif
endif
endif
endif


AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC 		= $(CROSS_COMPILE)gcc
LD 		= $(CROSS_COMPILE)ld 
AR		= $(CROSS_COMPILE)ar
NM 		= $(CROSS_COMPILE)nm
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

OUTDIR	= ./output

IMG2BIN	= $(CROSS_COMPILE)img2bin

RM	= rm


TOOLCFLAGS	=

TOOLLDFLAGS	= -n

OPT	=  -G 0

TEXT	=

INCLUDES	= -I. -I./include
ifeq ($(TC3262),1)
CFLAGS	= -mips32r2 -msoft-float -fomit-frame-pointer -nostdinc -fno-pic -mno-abicalls $(TOOLCFLAGS) $(OPT) $(INCLUDES) -D__KERNEL__ -Dlinux -O $(PLATFORM)
CFLAGS_OS = -mips32r2 -msoft-float -fomit-frame-pointer -nostdinc -fno-pic -mno-abicalls $(TOOLCFLAGS) $(OPT) $(INCLUDES) -D__KERNEL__ -Dlinux -Os $(PLATFORM)
else
CFLAGS	= -mips1 -msoft-float -fomit-frame-pointer -nostdinc -fno-pic -mno-abicalls $(TOOLCFLAGS) $(OPT) $(INCLUDES) -D__KERNEL__ -Dlinux -O $(PLATFORM)
endif

CFLAGS += -I$(GLOBAL_INC_DIR)
ifeq ($(TCSUPPORT_FWC_ENV),1)
CFLAGS += -I$(GLOBAL_INC_DIR)/modules
endif

ifeq ($(BOOT_LZMA_SUPPORT),1)
CFLAGS += -DBOOT_LZMA_SUPPORT
ifneq ($(SPI),1)
SPI=1
endif
else
ifneq ($(RT63365),1)
ifneq ($(RT63165),1)
SPI=1
endif
endif
endif

#frankliao added
ifeq ($(SPI),1)
CFLAGS += -DTCSUPPORT_BB_SPI
endif

#frankliao added
ifeq ($(NAND),1)
CFLAGS += -DTCSUPPORT_BB_NAND
endif

ifeq ($(SPI_TEST),1)
CFLAGS += -DSPI_TEST_CMD
#CFLAGS += -DETH_TEST_CMD
endif

ifneq ($(TCSUPPORT_MT7510_FE),)
CFLAGS += -DTCSUPPORT_MT7510_FE
endif

ifneq ($(TCSUPPORT_CPU_MT7510),)
CFLAGS += -DTCSUPPORT_CPU_MT7510=1
endif

ifneq ($(TCSUPPORT_CPU_MT7520),)
CFLAGS += -DTCSUPPORT_CPU_MT7520=1
endif

ifneq ($(TCSUPPORT_CPU_MT7505),)
CFLAGS += -DTCSUPPORT_CPU_MT7505=1
endif

ifneq ($(TCSUPPORT_CPU_EN7512),)
CFLAGS += -DTCSUPPORT_CPU_EN7512=1
endif

ifneq ($(TCSUPPORT_CPU_EN7521),)
CFLAGS += -DTCSUPPORT_CPU_EN7521=1
endif

ifneq ($(strip $(TCSUPPORT_TOOLCHAIN_VER)),)
#for c code include kenrel
CFLAGS+=-DCONFIG_TOOLCHAIN_VER=$(strip $(TCSUPPORT_TOOLCHAIN_VER))
endif

#DRAM Protection Test Configure
#SPI_DRAM_TEST=1
#CFLAGS += -DDRAM_PROTECT_TEST

ifneq ($(TCSUPPORT_NEW_SPIFLASH),)
CFLAGS += -DTCSUPPORT_NEW_SPIFLASH=1
endif

ifneq ($(TCSUPPORT_NEW_SPIFLASH_DEBUG),)
CFLAGS += -DTCSUPPORT_NEW_SPIFLASH_DEBUG=1
endif

ifneq ($(TCSUPPORT_BOOTROM_LARGE_SIZE),)
CFLAGS += -DTCSUPPORT_BOOTROM_LARGE_SIZE=1
endif

ifneq ($(MT75XX_REDUCE_SIZE),)
CFLAGS += -DMT75XX_REDUCE_SIZE=1
endif
ifneq ($(TCSUPPORT_ADDR_MAPPING),)
CFLAGS += -DTCSUPPORT_ADDR_MAPPING
endif

ifeq ($(SPI_DRAM_TEST),1)
CFLAGS += -DSPI_TEST_CMD -DSPI_DRAM_TEST_CMD
endif

ifneq ($(TCSUPPORT_MT7530_EXTERNAL),)
CFLAGS += -DMT7530_SUPPORT
endif

ifneq ($(MT7505_SWITCH_TBL_VERIRY),)
CFLAGS += -DMT7505_SWITCH_TBL_VERIRY
endif

ifeq ($(MT7530_SLT),1)
CFLAGS += -DMT7530_SLT
endif

ifneq ($(TCSUPPORT_TC2031),)
CFLAGS += -DTC2031_SUPPORT -DTC2102ME_SUPPORT -DTC2101ME_SUPPORT -DTC2101MB_SUPPORT 
else
CFLAGS += -DTC2206_SUPPORT
endif

ifeq ($(TCSUPPORT_FREE_BOOTBASE), 1)
CFLAGS += -DTCSUPPORT_FREE_BOOTBASE
endif
ifeq ($(TCSUPPORT_NAND_BADBLOCK_CHECK), 1)
CFLAGS += -DTCSUPPORT_NAND_BADBLOCK_CHECK
endif

ifeq ($(TCSUPPORT_NAND_RT63368), 1)
CFLAGS += -DTCSUPPORT_NAND_RT63368
endif

ifneq ($(TCSUPPORT_NAND_BMT),)
CFLAGS += -DTCSUPPORT_NAND_BMT
endif

ifneq ($(TCSUPPORT_CT_PON_CN),)
CFLAGS += -DTCSUPPORT_CT_PON_CN
endif

ifeq ($(TCSUPPORT_CT_PON_C9),1)
CFLAGS += -DTCSUPPORT_CT_PON_C9
endif
ifeq ($(CONFIG_DUAL_IMAGE),1)
CFLAGS += -DCONFIG_DUAL_IMAGE
ifneq ($(TCSUPPORT_GPON_DUAL_IMAGE),)
CFLAGS += -DTCSUPPORT_GPON_DUAL_IMAGE
endif
ifeq ($(TCSUPPORT_TTNET),1)
CFLAGS += -DTCSUPPORT_DUAL_IMAGE_ENHANCE
endif
ifeq ($(TCSUPPORT_DUAL_IMAGE_ENHANCE),1)
CFLAGS += -DTCSUPPORT_DUAL_IMAGE_ENHANCE
endif
ifeq ($(TCSUPPORT_DUAL_IMAGE_8M),1)
CFLAGS += -DTCSUPPORT_DUAL_IMAGE_8M
endif

ifneq ($(TCSUPPORT_CT_PON),)
CFLAGS += -DTCSUPPORT_CT_PON
endif

ifeq ($(TCSUPPORT_CHS),1)
CFLAGS += -DTCSUPPORT_CHS
endif

ifneq ($(TCSUPPORT_CT_UPG_PINGPONG),)
CFLAGS += -DTCSUPPORT_CT_UPG_PINGPONG
endif

ifneq ($(TCSUPPORT_NAND_FLASH),)
CFLAGS += -DTCSUPPORT_NAND_FLASH
endif

ifneq ($(TCSUPPORT_CT_DUAL_IMAGE),)
CFLAGS += -DTCSUPPORT_CT_DUAL_IMAGE
endif

ifneq ($(TCSUPPORT_CUC_DUAL_IMAGE),)
CFLAGS += -DTCSUPPORT_CUC_DUAL_IMAGE
endif

endif

ifeq ($(TCSUPPORT_TTNET),1)
CFLAGS += -DTCSUPPORT_TTNET
endif

ifeq ($(LZMA_IMG),1)
CFLAGS += -DLZMA_IMG
endif

ifeq ($(TCSUPPORT_CT),1)
CFLAGS += -DTCSUPPORT_CT
endif

ifeq ($(TCSUPPORT_CT_WAN_PTM),1)
CFLAGS += -DTCSUPPORT_CT_WAN_PTM
endif

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_C7)
ifeq ($(TCSUPPORT_C7),1)
CFLAGS += -DTCSUPPORT_C7
endif
#endif/*TCSUPPORT_COMPILE*/

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_IS_FH_PON)
ifeq ($(TCSUPPORT_IS_FH_PON),1)
CFLAGS += -DTCSUPPORT_IS_FH_PON
endif
#endif/*TCSUPPORT_COMPILE*/

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_FWC_ENV)
ifeq ($(TCSUPPORT_FWC_ENV),1)
CFLAGS += -DTCSUPPORT_FWC_ENV
endif
#endif/*TCSUPPORT_COMPILE*

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CY)
ifeq ($(TCSUPPORT_CY),1)
CFLAGS += -DTCSUPPORT_CY
endif
#endif/*TCSUPPORT_COMPILE*/

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PON_CY)
ifeq ($(TCSUPPORT_CT_PON_CY),1)
CFLAGS += -DTCSUPPORT_CT_PON_CY
endif
#endif/*TCSUPPORT_COMPILE*/

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PON_CAU)
ifeq ($(TCSUPPORT_CT_PON_CAU),1)
CFLAGS += -DTCSUPPORT_CT_PON_CAU
endif
#endif/*TCSUPPORT_COMPILE*/

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PON_C7)
ifeq ($(TCSUPPORT_CT_PON_C7),1)
CFLAGS += -DTCSUPPORT_CT_PON_C7
endif
#endif/*TCSUPPORT_COMPILE*/

ifeq ($(BOOT_LZMA_SUPPORT),1)
CFLAGS += -DBOOT_LZMA_SUPPORT
endif

#Add for include tc_partition.h
ifeq ($(TCSUPPORT_MTD_ENCHANCEMENT),1)
CFLAGS += -DTCSUPPORT_MTD_ENCHANCEMENT
endif

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_BOOTLOADER_UPGRADE)
#add for Bootloader Upgrade
ifeq ($(TCSUPPORT_CT_BOOTLOADER_UPGRADE),1)
CFLAGS += -DTCSUPPORT_CT_BOOTLOADER_UPGRADE
endif
#endif/*TCSUPPORT_COMPILE*/

ifeq ($(TCSUPPORT_C1_OBM),)
ifeq ($(TCSUPPORT_AUTOBENCH),)
CFLAGS += -DTCSUPPORT_MULTI_UPGRADE
ifeq ($(TCSUPPORT_MULTI_UPGRADE_LIONS),1)
	CFLAGS += -DTCSUPPORT_MULTI_UPGRADE_LIONS
endif
endif
endif

ifneq ($(TCSUPPORT_AUTOBENCH),)
CFLAGS += -DTCSUPPORT_AUTOBENCH
endif

#/* Disable UART2 Feature */
#ifneq ($(TCSUPPORT_UART2),)
#CFLAGS += -DTCSUPPORT_UART2
#endif

ifeq ($(TCSUPPORT_MULTI_BOOT),1)
ifeq ($(TCSUPPORT_MULTI_UPGRADE),1)
	echo "TCSUPPORT_MULTI_BOOT and TCSUPPORT_MULTI_UPGRADE can not be compiled at the same time!!!"
else
CFLAGS += -DTCSUPPORT_MULTI_BOOT
endif
endif

ifneq ($(TCSUPPORT_BB_NAND_4K),)
CFLAGS += -DTCSUPPORT_BB_NAND_4K
endif

ifeq ($(DDR_CALI_NAND), 1)
CFLAGS += -DDDR_CALI_NAND
endif

ifeq ($(TCSUPPORT_BOOTROM_BACKDOOR), 1)
CFLAGS += -DBOOTROM_BACKDOOR
ifneq ($(TCSUPPORT_CPU_MT7505),)
CFLAGS += -DBACKDOOR_SWITCH
endif
endif


ifneq ($(TCSUPPORT_CPU_EN7512),)
CFLAGS += -DBOOTROM_BACKDOOR
CFLAGS += -DBACKDOOR_SWITCH
endif

ifneq ($(TCSUPPORT_CPU_EN7521),)
CFLAGS += -DBOOTROM_BACKDOOR
CFLAGS += -DBACKDOOR_SWITCH
endif

#RBUS_COUNTER_TEST = 1
ifeq ($(RBUS_COUNTER_TEST), 1)
CFLAGS += -DRBUS_COUNTER_TEST
endif

#MT7550_GPIO = 1
ifeq ($(MT7550_GPIO), 1)
CFLAGS += -DMT7550_GPIO
#CFLAGS += -DMT7550_GPIO_TEST
endif

ifeq ($(BOOTROM_LARGE_SIZE), 1)
CFLAGS += -DBOOTROM_LARGE_SIZE
endif

ifeq ($(TCSUPPORT_C1_TRUE),1)
CFLAGS += -DTCSUPPORT_C1_TRUE
endif

ifeq ($(TCSUPPORT_C1_OBM),1)
CFLAGS += -DTCSUPPORT_C1_OBM
endif

ifeq ($(TCSUPPORT_POWERSAVE_ENABLE),1)
CFLAGS += -DTCSUPPORT_POWERSAVE_ENABLE
endif

ifeq ($(TCSUPPORT_C1_MS),1)
CFLAGS += -DTCSUPPORT_C1_MS
endif

ifeq ($(TCSUPPORT_RESERVEAREA_BLOCK),1)
CFLAGS += -DTCSUPPORT_RESERVEAREA_BLOCK=1
endif
ifeq ($(TCSUPPORT_RESERVEAREA_BLOCK),2)
CFLAGS += -DTCSUPPORT_RESERVEAREA_BLOCK=2
endif
ifeq ($(TCSUPPORT_RESERVEAREA_BLOCK),3)
CFLAGS += -DTCSUPPORT_RESERVEAREA_BLOCK=3
endif
ifeq ($(TCSUPPORT_RESERVEAREA_BLOCK),4)
CFLAGS += -DTCSUPPORT_RESERVEAREA_BLOCK=4
endif

ifeq ($(TCSUPPORT_RESERVEAREA_BLOCK),7)
CFLAGS += -DTCSUPPORT_RESERVEAREA_BLOCK=7
endif

ifneq ($(strip $(TCSUPPORT_PRODUCTIONLINE)),)
CFLAGS+=-DTCSUPPORT_PRODUCTIONLINE
endif

ifneq ($(strip $(TCSUPPORT_RESERVEAREA_EXTEND)),)
CFLAGS+=-DTCSUPPORT_RESERVEAREA_EXTEND
endif

ifneq ($(strip $(TCSUPPORT_MTD_PARTITIONS_CMDLINE)),)
CFLAGS+=-DTCSUPPORT_MTD_PARTITIONS_CMDLINE
CFLAGS+=-DTCSUPPORT_PARTITIONS_CMDLINE_STR=\"$(TCSUPPORT_PARTITIONS_CMDLINE_STR)\"
endif

#Not support bootloader compression
ifneq ($(BOOT_LZMA_SUPPORT),1)
LDFLAGS = -T./ld.script0
#Support bootloader compression
else
#For lzma.img
ifeq ($(LZMA_IMG),1)
LDFLAGS = -T./ld.script0
else
#For boot.img, support bootloader compression
ifeq ($(16M),1)
LDFLAGS = -T./ld.script2
else
ifeq ($(32M),1)
LDFLAGS = -T./ld.script3
else
ifeq ($(64M),1)
LDFLAGS = -T./ld.script4
else
LDFLAGS = -T./ld.script1
endif
endif
endif
endif
endif

LDFLAGS += -nostdlib -EB  -static

ASFLAGS	=  -D__ASSEMBLY__  -x assembler-with-cpp -G 0

CRT	=

LIBS	=

.SUFFIXES : .s .S .c .o .out .nm .img .sr .sre .text .bin .scr

all: 
		@if [ ! -d output ]; then \
			mkdir output; \
		fi
		make $(OUTDIR)/boot.out
		@echo  \#define BOOT_CODE_TIME \"`date `\" > ./banner/mk_time	 	
		$(NM)	./output/boot.out | sort > ./output/boot.nm
		$(OBJCOPY) -g -Obinary ./output/boot.out ./output/boot.img
		$(OBJDUMP) -h -S ./output/boot.out > ./output/boot.text

#		cp ./output/boot.img ../boot.img -f
#		../../tools/lzma e ./output/boot.img ../boot.img

##########################head.o must be the first.
# For lzma.img
ifeq ($(LZMA_IMG),1)
OBJFILES	= $(OUTDIR)/head.o \
		  $(OUTDIR)/setup.o \
		  $(OUTDIR)/main.o \
		  $(OUTDIR)/misc_lzma.o 

ifeq ($(NAND),1)
#OBJFILES   += $(OUTDIR)/ctype.o 
#OBJFILES   += $(OUTDIR)/string.o
ifeq ($(EN7512_NAND),1)
OBJFILES   +=$(OUTDIR)/string.o		  
OBJFILES   +=$(OUTDIR)/init.o
OBJFILES   +=$(OUTDIR)/vsprintf.o
OBJFILES   +=$(OUTDIR)/ctype.o 
OBJFILES   +=$(OUTDIR)/spi_controller.o
OBJFILES   +=$(OUTDIR)/spi_nand_flash.o
else
ifeq ($(MT75XX_NAND),1)
OBJFILES   += $(OUTDIR)/mt7510_nandflash.o
else
OBJFILES   += $(OUTDIR)/nandflash.o 
endif
endif
ifeq ($(DDR_CALI_NAND), 1)
OBJFILES	+= $(OUTDIR)/spram.o
endif
endif		  

# For boot.img
else
OBJFILES	= $(OUTDIR)/head.o \
		  $(OUTDIR)/commands.o \
		  $(OUTDIR)/eth.o \
		  $(OUTDIR)/arp.o \
		  $(OUTDIR)/ip.o \
		  $(OUTDIR)/udp.o \
		  $(OUTDIR)/icmp.o \
		  $(OUTDIR)/tftp.o \
		  $(OUTDIR)/tftpput.o \
		  $(OUTDIR)/skbuff.o \
		  $(OUTDIR)/irq.o \
		  $(OUTDIR)/int-handler.o \
		  $(OUTDIR)/setup.o \
		  $(OUTDIR)/main.o  \
		  $(OUTDIR)/traps.o \
		  $(OUTDIR)/vsprintf.o \
		  $(OUTDIR)/init.o \
		  $(OUTDIR)/time.o \
		  $(OUTDIR)/xmodem.o \
		  $(OUTDIR)/string.o  \
		  $(OUTDIR)/flashhal.o \
		  $(OUTDIR)/flash.o \
		  $(OUTDIR)/ctype.o \
		  $(OUTDIR)/tcswitch.o 

ifeq ($(MT7550_GPIO),1)
OBJFILES	+= $(OUTDIR)/MT7550_api.o
endif

ifeq ($(MT7530_SLT),1)
OBJFILES	+= $(OUTDIR)/switch_slt_main.o
endif

ifeq ($(SPI),1)
ifneq ($(TCSUPPORT_NEW_SPIFLASH),)
OBJFILES	+= $(OUTDIR)/newspiflash.o 
else
OBJFILES	+= $(OUTDIR)/spiflash.o 
endif
endif

ifeq ($(TCSUPPORT_MTD_PARTITIONS_CMDLINE),1)
OBJFILES   += $(OUTDIR)/mtd_partition_parse.o
endif

ifeq ($(NAND),1)
ifeq ($(EN7512_NAND),1)
OBJFILES   += $(OUTDIR)/spi_controller.o
OBJFILES   += $(OUTDIR)/spi_nand_flash.o
else
ifeq ($(MT75XX_NAND),1)
OBJFILES   += $(OUTDIR)/mt7510_nandflash.o
else
ifeq ($(TCSUPPORT_NAND_RT63368),1)
OBJFILES   += $(OUTDIR)/nand_bmt.o 
else
OBJFILES	+= $(OUTDIR)/nandflash.o 
endif
endif
endif
ifneq ($(TCSUPPORT_NAND_BMT),)
OBJFILES   += $(OUTDIR)/bmt.o
endif
endif

ifeq ($(SPI_DRAM_TEST),1)
OBJFILES	+= $(OUTDIR)/dramtest.o 
ifneq ($(TCSUPPORT_ADDR_MAPPING),)
OBJFILES    += $(OUTDIR)/MMU.o
endif
endif

ifeq ($(BOOT_LZMA_SUPPORT),1)
OBJFILES	+= $(OUTDIR)/lwip_mem.o \
		  $(OUTDIR)/lwip_memp.o \
		  $(OUTDIR)/lwip_netif.o \
		  $(OUTDIR)/lwip_pbuf.o \
		  $(OUTDIR)/lwip_stats.o \
		  $(OUTDIR)/lwip_udp.o \
		  $(OUTDIR)/lwip_inet.o \
		  $(OUTDIR)/lwip_raw.o \
		  $(OUTDIR)/lwip_sys.o \
		  $(OUTDIR)/lwip_tcp.o \
		  $(OUTDIR)/lwip_tcp_in.o \
		  $(OUTDIR)/lwip_tcp_out.o \
		  $(OUTDIR)/lwip_dhcp.o \
		  $(OUTDIR)/lwip_icmp.o \
		  $(OUTDIR)/lwip_ip.o \
		  $(OUTDIR)/lwip_ip_addr.o \
		  $(OUTDIR)/lwip_ip_frag.o \
		  $(OUTDIR)/lwip_etharp.o \
		  $(OUTDIR)/lwip_http.o
endif

endif

ifneq ($(LZMA_IMG),1)
ifeq ($(TC3262),1)
ifeq ($(EN7512),1)
OBJFILES        += $(OUTDIR)/7512_eth.o
OBJFILES	+= $(OUTDIR)/gpio.o
else
ifeq ($(RT63365),1)
OBJFILES	+= $(OUTDIR)/femac.o 
OBJFILES	+= $(OUTDIR)/gpio.o
else
OBJFILES	+= $(OUTDIR)/tc3262gmac.o 
endif
endif
else
ifeq ($(TC3162L2),1)
OBJFILES	+= $(OUTDIR)/tc3162l2mac.o 
else
ifeq ($(TC3262),1)
OBJFILES	+= $(OUTDIR)/tc3162l2mac.o 
else
OBJFILES	+= $(OUTDIR)/mac.o 
endif
endif
endif
endif

ifneq ($(LZMA_IMG),1)
ifeq ($(GZIP),1)
OBJFILES	+= $(OUTDIR)/load.o 
else
OBJFILES	+= $(OUTDIR)/misc_lzma.o 
endif
endif

##Assembly file######################################################
$(OUTDIR)/int-handler.o: ./init/int-handler.S 
	$(CC) -c $(CFLAGS) $(ASFLAGS) -o $(OUTDIR)/int-handler.o ./init/int-handler.S
$(OUTDIR)/head.o: ./arch/mips/kernel/head.S 
	$(CC) -c $(CFLAGS) $(ASFLAGS) -o $(OUTDIR)/head.o ./arch/mips/kernel/head.S




##C file#############################################################
$(OUTDIR)/irq.o: ./init/irq.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/irq.o ./init/irq.c

$(OUTDIR)/time.o: ./init/time.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/time.o ./init/time.c

$(OUTDIR)/xmodem.o: ./lib/xmodem.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/xmodem.o ./lib/xmodem.c

$(OUTDIR)/commands.o: ./lib/commands.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/commands.o ./lib/commands.c
	
$(OUTDIR)/dramtest.o: ./lib/dramtest.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/dramtest.o ./lib/dramtest.c	
$(OUTDIR)/MMU.o: ./lib/MMU.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/MMU.o ./lib/MMU.c	

ifeq ($(TC3262),1)
ifeq ($(EN7512),1)
$(OUTDIR)/7512_eth.o: ./net/7512_eth.c
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/7512_eth.o ./net/7512_eth.c
$(OUTDIR)/gpio.o: ./lib/gpio.c
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/gpio.o ./lib/gpio.c
else
ifeq ($(RT63365),1)
$(OUTDIR)/femac.o: ./net/femac.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/femac.o ./net/femac.c
$(OUTDIR)/gpio.o: ./lib/gpio.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/gpio.o ./lib/gpio.c
else
$(OUTDIR)/tc3262gmac.o: ./net/tc3262gmac.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/tc3262gmac.o ./net/tc3262gmac.c
endif
endif
else
ifeq ($(TC3162L2),1)
$(OUTDIR)/tc3162l2mac.o: ./net/tc3162l2mac.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/tc3162l2mac.o ./net/tc3162l2mac.c
else
$(OUTDIR)/mac.o: ./net/mac.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/mac.o ./net/mac.c
endif
endif

$(OUTDIR)/eth.o: ./net/eth.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/eth.o ./net/eth.c

$(OUTDIR)/arp.o: ./net/arp.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/arp.o ./net/arp.c

$(OUTDIR)/ip.o: ./net/ip.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/ip.o ./net/ip.c

$(OUTDIR)/udp.o: ./net/udp.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/udp.o ./net/udp.c

$(OUTDIR)/icmp.o: ./net/icmp.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/icmp.o ./net/icmp.c

$(OUTDIR)/tftp.o: ./net/tftp.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/tftp.o ./net/tftp.c

$(OUTDIR)/tftpput.o: ./net/tftpput.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/tftpput.o ./net/tftpput.c

$(OUTDIR)/skbuff.o: ./net/skbuff.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/skbuff.o ./net/skbuff.c

$(OUTDIR)/tcswitch.o: ./net/tcswitch.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/tcswitch.o ./net/tcswitch.c

ifeq ($(MT7530_SLT),1)
$(OUTDIR)/switch_slt_main.o: ./net/switch_slt_main.c
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/switch_slt_main.o ./net/switch_slt_main.c
endif

$(OUTDIR)/load.o: ./lib/load.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/load.o ./lib/load.c

$(OUTDIR)/misc_lzma.o: ./lib/misc_lzma.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/misc_lzma.o ./lib/misc_lzma.c

$(OUTDIR)/setup.o: ./arch/mips/kernel/setup.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/setup.o ./arch/mips/kernel/setup.c

ifeq ($(LZMA_IMG),1)
$(OUTDIR)/main.o: ../lzma/main.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/main.o ../lzma/main.c
else
$(OUTDIR)/main.o: ./init/main.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/main.o ./init/main.c
endif

$(OUTDIR)/traps.o: ./arch/mips/kernel/traps.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/traps.o ./arch/mips/kernel/traps.c

ifeq ($(MT7550_GPIO),1)
$(OUTDIR)/MT7550_api.o: ./lib/MT7550_api.c 
	$(CC) -c $(CFLAGS)  -o $(OUTDIR)/MT7550_api.o ./lib/MT7550_api.c
endif

$(OUTDIR)/vsprintf.o: ./lib/vsprintf.c 
	$(CC) -c $(CFLAGS)  -o $(OUTDIR)/vsprintf.o ./lib/vsprintf.c

$(OUTDIR)/init.o: ./io/init.c 
	$(CC) -c $(CFLAGS)  -o $(OUTDIR)/init.o ./io/init.c

$(OUTDIR)/string.o: ./lib/string.c 
	$(CC) -c $(CFLAGS)  -o $(OUTDIR)/string.o ./lib/string.c

$(OUTDIR)/flashhal.o: ./flash/flashhal.c 
	$(CC) -c $(CFLAGS)  -o $(OUTDIR)/flashhal.o ./flash/flashhal.c

$(OUTDIR)/flash.o: ./flash/flash.c 
	$(CC) -c $(CFLAGS)  -o $(OUTDIR)/flash.o ./flash/flash.c

$(OUTDIR)/spiflash.o: ./flash/spiflash.c 
	$(CC) -c $(CFLAGS)  -o $(OUTDIR)/spiflash.o ./flash/spiflash.c

$(OUTDIR)/newspiflash.o: ./flash/newspiflash.c 
	$(CC) -c $(CFLAGS)  -o $(OUTDIR)/newspiflash.o ./flash/newspiflash.c
	
$(OUTDIR)/nandflash.o: ./flash/nandflash.c 
	$(CC) -c $(CFLAGS)  -o $(OUTDIR)/nandflash.o ./flash/nandflash.c

$(OUTDIR)/mt7510_nandflash.o: ./flash/mt7510_nandflash.c 
	$(CC) -c $(CFLAGS)  -o $(OUTDIR)/mt7510_nandflash.o ./flash/mt7510_nandflash.c
	
$(OUTDIR)/mtd_partition_parse.o: ./flash/mtd_partition_parse.c 
	$(CC) -c $(CFLAGS)  -o $(OUTDIR)/mtd_partition_parse.o ./flash/mtd_partition_parse.c
		
$(OUTDIR)/nand_bmt.o: ./flash/nand_bmt.c 
	$(CC) -c $(CFLAGS)  -o $(OUTDIR)/nand_bmt.o ./flash/nand_bmt.c

$(OUTDIR)/bmt.o: ./flash/bmt.c 
	$(CC) -c $(CFLAGS)  -o $(OUTDIR)/bmt.o ./flash/bmt.c	

$(OUTDIR)/ctype.o: ./lib/ctype.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/ctype.o ./lib/ctype.c

$(OUTDIR)/spi_nand_flash.o: ./flash/spi_nand_flash.c 
	$(CC) -c $(CFLAGS)  -o $(OUTDIR)/spi_nand_flash.o ./flash/spi_nand_flash.c
	
$(OUTDIR)/spi_controller.o: ./flash/controller/spi_controller/spi_controller.c 
	$(CC) -c $(CFLAGS)  -o $(OUTDIR)/spi_controller.o ./flash/controller/spi_controller/spi_controller.c 
	

####################### lwip ##########################

$(OUTDIR)/lwip_mem.o: ./lwip-1.1.1/src/core/mem.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/lwip_mem.o ./lwip-1.1.1/src/core/mem.c

$(OUTDIR)/lwip_memp.o: ./lwip-1.1.1/src/core/memp.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/lwip_memp.o ./lwip-1.1.1/src/core/memp.c

$(OUTDIR)/lwip_netif.o: ./lwip-1.1.1/src/core/netif.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/lwip_netif.o ./lwip-1.1.1/src/core/netif.c

$(OUTDIR)/lwip_pbuf.o: ./lwip-1.1.1/src/core/pbuf.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/lwip_pbuf.o ./lwip-1.1.1/src/core/pbuf.c

$(OUTDIR)/lwip_stats.o: ./lwip-1.1.1/src/core/stats.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/lwip_stats.o ./lwip-1.1.1/src/core/stats.c

$(OUTDIR)/lwip_udp.o: ./lwip-1.1.1/src/core/udp.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/lwip_udp.o ./lwip-1.1.1/src/core/udp.c

$(OUTDIR)/lwip_inet.o: ./lwip-1.1.1/src/core/inet.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/lwip_inet.o ./lwip-1.1.1/src/core/inet.c

$(OUTDIR)/lwip_raw.o: ./lwip-1.1.1/src/core/raw.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/lwip_raw.o ./lwip-1.1.1/src/core/raw.c

$(OUTDIR)/lwip_sys.o: ./lwip-1.1.1/src/core/sys.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/lwip_sys.o ./lwip-1.1.1/src/core/sys.c

$(OUTDIR)/lwip_tcp.o: ./lwip-1.1.1/src/core/tcp.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/lwip_tcp.o ./lwip-1.1.1/src/core/tcp.c

$(OUTDIR)/lwip_tcp_out.o: ./lwip-1.1.1/src/core/tcp_out.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/lwip_tcp_out.o ./lwip-1.1.1/src/core/tcp_out.c

$(OUTDIR)/lwip_tcp_in.o: ./lwip-1.1.1/src/core/tcp_in.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/lwip_tcp_in.o ./lwip-1.1.1/src/core/tcp_in.c

$(OUTDIR)/lwip_dhcp.o: ./lwip-1.1.1/src/core/dhcp.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/lwip_dhcp.o ./lwip-1.1.1/src/core/dhcp.c

$(OUTDIR)/lwip_icmp.o: ./lwip-1.1.1/src/core/ipv4/icmp.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/lwip_icmp.o ./lwip-1.1.1/src/core/ipv4/icmp.c

$(OUTDIR)/lwip_ip.o: ./lwip-1.1.1/src/core/ipv4/ip.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/lwip_ip.o ./lwip-1.1.1/src/core/ipv4/ip.c

$(OUTDIR)/lwip_ip_addr.o: ./lwip-1.1.1/src/core/ipv4/ip_addr.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/lwip_ip_addr.o ./lwip-1.1.1/src/core/ipv4/ip_addr.c

$(OUTDIR)/lwip_ip_frag.o: ./lwip-1.1.1/src/core/ipv4/ip_frag.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/lwip_ip_frag.o ./lwip-1.1.1/src/core/ipv4/ip_frag.c

$(OUTDIR)/lwip_etharp.o: ./lwip-1.1.1/src/netif/etharp.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/lwip_etharp.o ./lwip-1.1.1/src/netif/etharp.c

$(OUTDIR)/lwip_http.o: ./lwip-1.1.1/src/http_svr/http.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/lwip_http.o ./lwip-1.1.1/src/http_svr/http.c

$(OUTDIR)/spram.o: ../ddr_cal/spram.c 
	$(CC) -c -Os $(CFLAGS_OS) -o $(OUTDIR)/spram.o ../ddr_cal/spram.c	

$(OUTDIR)/boot.out :	 $(OBJFILES)
	$(LD) $(LDFLAGS) $(OBJFILES) $(LIBS) -o $(OUTDIR)/boot.out

compile :	 $(OBJFILES)



################### SUFFIX RULES ######################
#.scr.bin:
#	$(IMG2BIN) "$(OUTDIR)\$<" "$(OUTDIR)\$@"
#
#.s.out:
#	$(CC) $(CFLAGS) $(ASFLAGS) $(LDFLAGS) -o "$(OUTDIR)\$@" $<
#
#.S.out:
#	$(CC) $(CFLAGS) $(LDFLAGS) -o "$(OUTDIR)\$@" $< $(LIBS)
#
#.c.out:
#	$(CC) $(CFLAGS) $(LDFLAGS) -o "$(OUTDIR)\$@" $< $(LIBS)
#
.s.o:
	$(CC) -c $(CFLAGS) $(ASFLAGS) -o "$(OUTDIR)\$@" $<

.S.o:
	$(CC) -c $(CFLAGS) -o "$(OUTDIR)\$@" $<

.c.o:
	$(CC) -c $(CFLAGS) -o "$(OUTDIR)\$@" $<

.cpp.o:
	$(CC) -c $(CFLAGS) -o "$(OUTDIR)\$@" $<

.out.nm:
	$(NM) -B -n "$(OUTDIR)\$<" > "$(OUTDIR)\$@"

.out.img:
	$(OBJCOPY) -O binary "$(OUTDIR)\$<" "$(OUTDIR)\$@"

.out.text:
	$(OBJDUMP) -h -S -l --show-raw-insn "$(OUTDIR)\$<" > "$(OUTDIR)\$@"

.img.bin:
	$(IMG2BIN) "$(OUTDIR)\$<" "$(OUTDIR)\$@"

clean :
	$(RM) -f $(OBJFILES)
	$(RM) -f $(OUTDIR)/boot.out
	$(RM) -f $(OUTDIR)/tc3162l2mac.o
	$(RM) -f $(OUTDIR)/tc3262gmac.o
	$(RM) -f $(OUTDIR)/boot.nm
	$(RM) -f $(OUTDIR)/boot.img
	$(RM) -f $(OUTDIR)/boot.text
	$(RM) -f $(OUTDIR)/*.o
	
