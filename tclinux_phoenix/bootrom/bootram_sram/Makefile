

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
CROSS_COMPILE = mips-linux-uclibc-
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

INCLUDES	= -I. -I../bootram/include
ifeq ($(TC3262),1)
CFLAGS	= -mips32r2 -msoft-float -fomit-frame-pointer -nostdinc -fno-pic -mno-abicalls $(TOOLCFLAGS) $(OPT) $(INCLUDES) -D__KERNEL__ -Dlinux -O $(PLATFORM)
CFLAGS_OS = -mips32r2 -msoft-float -fomit-frame-pointer -nostdinc -fno-pic -mno-abicalls $(TOOLCFLAGS) $(OPT) $(INCLUDES) -D__KERNEL__ -Dlinux -Os $(PLATFORM)
else
CFLAGS	= -mips1 -msoft-float -fomit-frame-pointer -nostdinc -fno-pic -mno-abicalls $(TOOLCFLAGS) $(OPT) $(INCLUDES) -D__KERNEL__ -Dlinux -O $(PLATFORM)
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

ifneq ($(TCSUPPORT_NEW_SPIFLASH),)
CFLAGS += -DTCSUPPORT_NEW_SPIFLASH=1
endif

ifneq ($(TCSUPPORT_NEW_SPIFLASH_DEBUG),)
CFLAGS += -DTCSUPPORT_NEW_SPIFLASH_DEBUG=1
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

ifeq ($(BOOTROM_IN_SRAM),1)
CFLAGS += -DBOOTROM_IN_SRAM
CFLAGS += -DMT7505_GDMP
endif

ifneq ($(TCSUPPORT_MT7530_EXTERNAL),)
CFLAGS += -DMT7530_SUPPORT
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


ifneq ($(TCSUPPORT_CT_DUAL_IMAGE),)
CFLAGS += -DTCSUPPORT_CT_DUAL_IMAGE
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
#CFLAGS += -DTCSUPPORT_MULTI_UPGRADE
ifeq ($(TCSUPPORT_MULTI_UPGRADE_LIONS),1)
	CFLAGS += -DTCSUPPORT_MULTI_UPGRADE_LIONS
endif
endif
endif

ifneq ($(TCSUPPORT_AUTOBENCH),)
CFLAGS += -DTCSUPPORT_AUTOBENCH
endif

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


LDFLAGS = -T./ld_sram.script0


LDFLAGS += -nostdlib -EB  -static

ASFLAGS	=  -D__ASSEMBLY__  -x assembler-with-cpp -G 0

CRT	=

LIBS	=

BOOTRAM_DIR=../bootram

.SUFFIXES : .s .S .c .o .out .nm .img .sr .sre .text .bin .scr

all: 
		@if [ ! -d output ]; then \
			mkdir output; \
		fi
		make $(OUTDIR)/boot.out
		@echo  \#define BOOT_CODE_TIME \"`date `\" > ./banner/mk_time	 	
		$(NM)	./output/boot.out | sort > ./output/boot.nm
		$(NM)	--size-sort ./output/boot.out > ./output/boot_size.nm
		$(OBJCOPY) -g -Obinary ./output/boot.out ./output/boot.img
		$(OBJDUMP) -h -S ./output/boot.out > ./output/boot.text

#		cp ./output/boot.img ../boot.img -f
#		../../tools/lzma e ./output/boot.img ../boot.img

##########################head.o must be the first.
OBJFILES	= $(OUTDIR)/head.o \
		  $(OUTDIR)/commands.o \
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
		  $(OUTDIR)/ctype.o

ifeq ($(SPI),1)
ifneq ($(TCSUPPORT_NEW_SPIFLASH),)
OBJFILES	+= $(OUTDIR)/newspiflash.o 
else
OBJFILES	+= $(OUTDIR)/spiflash.o 
endif
endif



##Assembly file######################################################
$(OUTDIR)/int-handler.o: $(BOOTRAM_DIR)/init/int-handler.S 
	$(CC) -c $(CFLAGS) $(ASFLAGS) -o $(OUTDIR)/int-handler.o $(BOOTRAM_DIR)/init/int-handler.S
$(OUTDIR)/head.o: $(BOOTRAM_DIR)/arch/mips/kernel/head.S 
	$(CC) -c $(CFLAGS) $(ASFLAGS) -o $(OUTDIR)/head.o $(BOOTRAM_DIR)/arch/mips/kernel/head.S




##C file#############################################################
$(OUTDIR)/setup.o: $(BOOTRAM_DIR)/arch/mips/kernel/setup.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/setup.o $(BOOTRAM_DIR)/arch/mips/kernel/setup.c
	
$(OUTDIR)/traps.o: $(BOOTRAM_DIR)/arch/mips/kernel/traps.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/traps.o $(BOOTRAM_DIR)/arch/mips/kernel/traps.c

$(OUTDIR)/vsprintf.o: $(BOOTRAM_DIR)/lib/vsprintf.c 
	$(CC) -c $(CFLAGS)  -o $(OUTDIR)/vsprintf.o $(BOOTRAM_DIR)/lib/vsprintf.c

$(OUTDIR)/irq.o: $(BOOTRAM_DIR)/init/irq.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/irq.o $(BOOTRAM_DIR)/init/irq.c	


$(OUTDIR)/string.o: $(BOOTRAM_DIR)/lib/string.c 
	$(CC) -c $(CFLAGS)  -o $(OUTDIR)/string.o $(BOOTRAM_DIR)/lib/string.c

$(OUTDIR)/init.o: $(BOOTRAM_DIR)/io/init.c 
	$(CC) -c $(CFLAGS)  -o $(OUTDIR)/init.o $(BOOTRAM_DIR)/io/init.c
	
$(OUTDIR)/newspiflash.o: $(BOOTRAM_DIR)/flash/newspiflash.c 
	$(CC) -c $(CFLAGS)  -o $(OUTDIR)/newspiflash.o $(BOOTRAM_DIR)/flash/newspiflash.c
	
$(OUTDIR)/time.o: $(BOOTRAM_DIR)/init/time.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/time.o $(BOOTRAM_DIR)/init/time.c

$(OUTDIR)/xmodem.o: $(BOOTRAM_DIR)/lib/xmodem.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/xmodem.o $(BOOTRAM_DIR)/lib/xmodem.c
	
$(OUTDIR)/commands.o: $(BOOTRAM_DIR)/lib/commands.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/commands.o $(BOOTRAM_DIR)/lib/commands.c


$(OUTDIR)/misc_lzma.o: $(BOOTRAM_DIR)/lib/misc_lzma.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/misc_lzma.o $(BOOTRAM_DIR)/lib/misc_lzma.c

$(OUTDIR)/ctype.o: $(BOOTRAM_DIR)/lib/ctype.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/ctype.o $(BOOTRAM_DIR)/lib/ctype.c
	
##Shrink C file#############################################################

$(OUTDIR)/main.o: ./init/main.c 
	$(CC) -c $(CFLAGS) -o $(OUTDIR)/main.o ./init/main.c

	
$(OUTDIR)/flashhal.o: ./flash/flashhal.c 
	$(CC) -c $(CFLAGS)  -o $(OUTDIR)/flashhal.o ./flash/flashhal.c
	

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
	
