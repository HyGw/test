
PLATFORM =


# default is TC3162L2 platform
ifneq ($(TC3262),1)
TC3162L2 = 1
endif

# is TC3262 FPGA board?
#TC3262_FPGA=1

# DDR PLL scan
#PLL_AUTO_SCAN=1
# DDR PLL value
PLL_SPSN_VALUE=16

# DDR PHY
SIS_DDR_PHY=1

#ifeq ($(TC3262),1)
#CROSS_COMPILE = mips-linux-gnu-
#CROSS_COMPILE = mips-linux-uclibc-
#else
#CROSS_COMPILE = mips-linux-
#endif

BOOTRAM_PARAM=

ifeq ($(TC3262),1)
PLATFORM += -DTC3262 
BOOTRAM_PARAM += TC3262=1
ifeq ($(TC3262_FPGA),1)
PLATFORM += -DTC3262_FPGA
BOOTRAM_PARAM += TC3262_FPGA=1
endif
ifeq ($(SIS_DDR_PHY),1)
PLATFORM += -DSIS_DDR_PHY
BOOTRAM_PARAM += SIS_DDR_PHY=1
else
PLATFORM += -DPLL_SPSN_VALUE=$(PLL_SPSN_VALUE) 
ifeq ($(PLL_AUTO_SCAN),1)
PLATFORM += -DPLL_AUTO_SCAN 
BOOTRAM_PARAM += PLL_AUTO_SCAN=1
endif
endif
ifeq ($(RT63365),1)
PLATFORM += -DRT63365
BOOTRAM_PARAM += RT63365=1
endif
ifneq ($(TCSUPPORT_CPU_MT7510),)
PLATFORM += -DMT7510 -DMT75XX_REDUCE_SIZE
BOOTRAM_PARAM += MT75XX_REDUCE_SIZE=1
endif
ifneq ($(TCSUPPORT_CPU_MT7505),)
PLATFORM += -DMT75XX_REDUCE_SIZE
BOOTRAM_PARAM += MT75XX_REDUCE_SIZE=1
endif
ifneq ($(TCSUPPORT_CPU_MT7520),)
PLATFORM += -DMT75XX_REDUCE_SIZE
BOOTRAM_PARAM += MT75XX_REDUCE_SIZE=1
endif
ifneq ($(strip $(TCSUPPORT_CPU_EN7512) $(TCSUPPORT_CPU_EN7521)),)
PLATFORM += -DMT75XX_REDUCE_SIZE
BOOTRAM_PARAM += MT75XX_REDUCE_SIZE=1
BOOTRAM_PARAM += TCSUPPORT_CPU_EN7512=1
PLATFORM += -DTCSUPPORT_CPU_EN7512
ifneq ($(TCSUPPORT_SLM_EN),)
PLATFORM += -DTCSUPPORT_SLM_EN
endif
endif
endif

ifneq ($(TCSUPPORT_CT_PON_C9),)
PLATFORM += -DTCSUPPORT_CT_PON_C9
BOOTRAM_PARAM += TCSUPPORT_CT_PON_C9=1
endif

ifneq ($(TCSUPPORT_CHS),)
PLATFORM += -DTCSUPPORT_CHS
BOOTRAM_PARAM += TCSUPPORT_CHS=1
endif

ifneq ($(TCSUPPORT_C1_MS),)
PLATFORM += -DTCSUPPORT_C1_MS
BOOTRAM_PARAM += TCSUPPORT_C1_MS=1
endif

ifeq ($(TC3162L2),1)
PLATFORM += -DTC3162L2 
BOOTRAM_PARAM += TC3162L2=1
endif

ifeq ($(16M),1)
PLATFORM += -DSDRAM_16M
BOOTRAM_PARAM += 16M=1
else
ifeq ($(32M),1)
PLATFORM += -DSDRAM_32M
BOOTRAM_PARAM += 32M=1
else
ifeq ($(64M),1)
PLATFORM += -DSDRAM_64M
BOOTRAM_PARAM += 64M=1
else
ifneq ($(8M),1)
ifeq ($(TC3262),1)
PLATFORM += -DSDRAM_32M
BOOTRAM_PARAM += 32M=1
else
PLATFORM += -DSDRAM_16M
BOOTRAM_PARAM += 16M=1
endif
else
BOOTRAM_PARAM += 8M=1
endif
endif
endif
endif

ifeq ($(GZIP),1)
PLATFORM += -DGZIP
endif

ifeq ($(TR068_LED),1)
BOOTRAM_PARAM += TR068_LED=1
endif

ifeq ($(TCSUPPORT_FREE_BOOTBASE),1)
BOOTRAM_PARAM += TCSUPPORT_FREE_BOOTBASE=1
endif

ifeq ($(CONFIG_DUAL_IMAGE),1)
BOOTRAM_PARAM += CONFIG_DUAL_IMAGE=1
endif


ifeq ($(TCSUPPORT_MULTI_UPGRADE),1)
BOOTRAM_PARAM += TCSUPPORT_MULTI_UPGRADE=1
endif



CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld 
NM = $(CROSS_COMPILE)nm
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

ifeq ($(TC3262),1)
CFLAGS =  -I./bootram/include -D__KERNEL__ -Wall -Wstrict-prototypes -Wno-trigraphs -mno-abicalls -fomit-frame-pointer -fno-strict-aliasing -fno-common -fno-pic -G 0 -mips32r2 -msoft-float -pipe -nostdinc $(PLATFORM)
else
CFLAGS =  -I./bootram/include -D__KERNEL__ -Wall -Wstrict-prototypes -Wno-trigraphs -mno-abicalls -fomit-frame-pointer -fno-strict-aliasing -fno-common -fno-pic -G 0 -mips1 -msoft-float -pipe -nostdinc $(PLATFORM)
endif
CFLAGS_C =  $(CFLAGS) -Os

LDFLAGS = -EB -static -nostdlib -G 0
LDSCRIPT = ld.script
EN7512_LDSCRIPT = ld_spram_en7512.script

ifneq ($(TCSUPPORT_AUTOBENCH),)
BOOTRAM_PARAM += AUTOBENCH_SUPPORT=1
CFLAGS += -DTCSUPPORT_AUTOBENCH
endif

#ifneq ($(TCSUPPORT_DMT_CO),)
#CFLAGS += -DTCSUPPORT_DMT_CO
#endif

# frankliao added 20101115
ifeq ($(SPI),1)
CFLAGS += -DTCSUPPORT_BB_SPI
endif

# frankliao added 20101115
ifeq ($(NAND),1)
CFLAGS += -DTCSUPPORT_BB_NAND
ifneq ($(TCSUPPORT_CPU_MT7510),)
BOOTRAM_PARAM += MT75XX_NAND=1
endif
ifneq ($(TCSUPPORT_CPU_MT7520),)
BOOTRAM_PARAM += MT75XX_NAND=1
endif
ifneq ($(TCSUPPORT_CPU_EN7512),)
BOOTRAM_PARAM +=EN7512_NAND=1
endif
ifneq ($(TCSUPPORT_CPU_EN7521),)
BOOTRAM_PARAM +=EN7512_NAND=1
endif
endif

ifneq ($(TCSUPPORT_AUTOBENCH),)
BOOTRAM_PARAM += AUTOBENCH=1
endif

ifneq ($(TCSUPPORT_BB_NAND_4K),)
CFLAGS += -DTCSUPPORT_BB_NAND_4K
endif

ifneq ($(TCSUPPORT_ADDR_MAPPING),)
CFLAGS += -DTCSUPPORT_ADDR_MAPPING
endif

MIC_FLAGS=-l
ifneq ($(TCSUPPORT_DDR_CALI),)
CFLAGS += -DDDR_CALI
LDSCRIPT = ld_spram.script
ifeq ($(NAND),1)
ifneq ($(strip $(TCSUPPORT_CPU_EN7512) $(TCSUPPORT_CPU_EN7521)),)
else
BOOTRAM_PARAM += DDR_CALI_NAND=1
CFLAGS += -DDDR_CALI_NAND
HEADER_FLAGS+=-DDDR_CALI_NAND
endif
endif
endif

ifneq ($(TCSUPPORT_BOOTROM_BACKDOOR),)
CFLAGS += -DBOOTROM_BACKDOOR
ifneq ($(TCSUPPORT_CPU_MT7505),)
CFLAGS += -DBACKDOOR_SWITCH
endif
endif

#128K TCBOOT
ifneq ($(TCSUPPORT_BOOTROM_LARGE_SIZE),)
CFLAGS += -DBOOTROM_LARGE_SIZE
BOOTRAM_PARAM += BOOTROM_LARGE_SIZE=1
MIC_FLAGS=-ll
HEADER_FLAGS+=-DBOOTROM_LARGE_SIZE
TRX_FLAGS += BOOTROM_LARGE_SIZE=1
endif

ifneq ($(TCSUPPORT_CPU_MT7505),)
BOOTRAM_PARAM += MT7505=1
CFLAGS += -DMT7505
DDR_CAL_PATH = ddr_cal_mt7505
else
ifneq ($(strip $(TCSUPPORT_CPU_EN7512) $(TCSUPPORT_CPU_EN7521)),)
#BOOTRAM_PARAM += EN7512=1
#CFLAGS += -DEN7512
DDR_CAL_PATH = ddr_cal_en7512
else
DDR_CAL_PATH = ddr_cal
endif
endif

ifneq ($(MT7510_EN7512_FPGA_STAGE),)
BOOTRAM_PARAM += MT7510_FPGA_TEST=1
CFLAGS += -DMT7510_FPGA_TEST
endif


#====Export FLAGS for Child Directories Using ===
export BOOTRAM_PARAMETER	= $(BOOTRAM_PARAM)
export BOOTROM_CFLAG		= $(CFLAGS)
export BOOTROM_CC			= $(CC)
export BOOTROM_LD			= $(LD)
export BOOTROM_NM			= $(NM)
export BOOTROM_OBJCOPY		= $(OBJCOPY)
export BOOTROM_OBJDUMP		= $(OBJDUMP)
export BOOTROM_IMG2BIN		= $(IMG2BIN)
#====Export CFLAGS to child directories==
bootload: clean bootload.c start.S
	sync;sync;sync
#Create calibration bin
ifeq ($(TCSUPPORT_DDR_CALI),1)
	make -C $(DDR_CAL_PATH) $(BOOTRAM_PARAM)
endif
	make -C bootram $(BOOTRAM_PARAM)
	cp ./bootram/output/boot.img ./boot.img -f
	$(CC) $(CFLAGS) -c bootload.c -o bootload.o
	$(OBJCOPY) --add-section .initrd=boot.img bootload.o
	$(CC) $(CFLAGS) -c start.S -o start.o
ifeq ($(TCSUPPORT_DDR_CALI),1)
	$(OBJCOPY) --add-section .spram=spram.bin bootload.o
	$(CC) $(CFLAGS_C) -c ./$(DDR_CAL_PATH)/spram.c -o spram.o
	$(LD) $(LDFLAGS) -T $(LDSCRIPT) start.o bootload.o spram.o -o boot.elf
else
	$(LD) $(LDFLAGS) -T $(LDSCRIPT) start.o bootload.o -o boot.elf
endif
	$(OBJCOPY) -O binary boot.elf boot.bin
	$(OBJDUMP) -h -S  boot.elf > boot.elf.txt
	$(NM) boot.elf  | sort > system.map
ifeq ($(TCSUPPORT_CT_BOOTLOADER_UPGRADE),1)
	./mic_ct -l boot.bin tcboot.bin
else
	./mic -l boot.bin tcboot.bin
endif	
	./byteswap tcboot.bin

bootlzma: clean bootload.c start.S
	sync;sync;sync
#Create calibration bin
ifeq ($(TCSUPPORT_DDR_CALI),1)
	make -C $(DDR_CAL_PATH) $(BOOTRAM_PARAM)
endif
	make -C bootram $(BOOTRAM_PARAM) BOOT_LZMA_SUPPORT=1
	../tools/lzma e ./bootram/output/boot.img ./boot.img
	cp ./bootram/output/boot.text ./boot_main.txt -f
	make -C bootram clean
	make -C bootram $(BOOTRAM_PARAM) LZMA_IMG=1
	cp ./bootram/output/boot.text ./boot_lzma.txt -f
	cp ./bootram/output/boot.img ./lzma.img -f
	$(CC) $(CFLAGS) -c bootload.c -o bootload.o
	$(CC) $(CFLAGS) -c lzmaload.c -o lzmaload.o
	$(OBJCOPY) --add-section .initrd=boot.img bootload.o
	$(OBJCOPY) --add-section .lzma=lzma.img lzmaload.o
ifneq ($(strip $(TCSUPPORT_CPU_EN7512) $(TCSUPPORT_CPU_EN7521)),)
	make -C en7512_boot $(BOOTRAM_PARAM)
endif

	$(CC) $(CFLAGS) -c -DLZMA_IMG start.S -o start.o
ifeq ($(TCSUPPORT_DDR_CALI),1)
	$(OBJCOPY) --add-section .spram=spram.bin lzmaload.o
ifneq ($(strip $(TCSUPPORT_CPU_EN7512) $(TCSUPPORT_CPU_EN7521)),)
else
	$(CC) $(CFLAGS_C) -c ./$(DDR_CAL_PATH)/spram.c -o spram.o
endif
ifneq ($(strip $(TCSUPPORT_CPU_EN7512) $(TCSUPPORT_CPU_EN7521)),)
	$(LD) $(LDFLAGS) -T $(EN7512_LDSCRIPT) start.o en7512_boot/move_data_load.o en7512_boot/boot2load.o bootload.o lzmaload.o -o boot.elf
else
	$(LD) $(LDFLAGS) -T $(LDSCRIPT) start.o bootload.o lzmaload.o spram.o -o boot.elf
endif

else

ifneq ($(strip $(TCSUPPORT_CPU_EN7512) $(TCSUPPORT_CPU_EN7521)),)
		$(LD) $(LDFLAGS) -T $(EN7512_LDSCRIPT) start.o en7512_boot/move_data_load.o en7512_boot/boot2load.o bootload.o lzmaload.o -o boot.elf
else
	$(LD) $(LDFLAGS) -T $(LDSCRIPT) start.o bootload.o lzmaload.o -o boot.elf
endif
endif
	$(OBJCOPY) -O binary boot.elf boot.bin 
	$(OBJDUMP) -h -S  boot.elf > boot.elf.txt 
	$(NM) boot.elf  | sort > system.map
ifeq ($(TCSUPPORT_CT_BOOTLOADER_UPGRADE),1)
	./mic_ct $(MIC_FLAGS) boot.bin tcboot.bin
else
	./mic $(MIC_FLAGS) boot.bin tcboot.bin
endif		
	./byteswap tcboot.bin
	gcc -c -DTRENDCHIP $(HEADER_FLAGS) header.c
	gcc -o header header.o
	mv tcboot.bin tcboot.bin.swap
	./header system.map tcboot.bin.swap
	make -C ../tools/trx/ clean
	make -C ../tools/trx/ $(TRX_FLAGS)
	mv tcboot.bin tcboot.bin.swap
	../tools/trx/trx -t tcboot.bin.swap
	
bootsram: clean bootload.c start.S
	sync;sync;sync
	make -C bootram_sram clean
	make -C bootram_sram $(BOOTRAM_PARAM) BOOTROM_IN_SRAM=1
	cp ./bootram_sram/output/boot.img ./boot.img -f
	$(CC) $(CFLAGS) -c bootload.c -o bootload.o
	$(OBJCOPY) --add-section .initrd=boot.img bootload.o
	$(CC) $(CFLAGS) -DBOOTROM_IN_SRAM -c start.S -o start.o
	$(LD) $(LDFLAGS) -T $(LDSCRIPT) start.o bootload.o -o boot.elf
	$(OBJCOPY) -O binary boot.elf boot.bin
	$(OBJDUMP) -h -S  boot.elf > boot.elf.txt
	$(NM) boot.elf  | sort > system.map
	./mic -l boot.bin tcboot.bin
	./byteswap tcboot.bin

clean:
	make -C bootram BOOT_LZMA_SUPPORT=1 clean
	make -C $(DDR_CAL_PATH) clean
	make -C bootram_sram clean
	rm -f boot.bin
	rm -f bootload.o start.o spram.o	
	rm -f tcboot.bin
	rm -f boot.bin.swap
	rm -f tcboot.bin.swap
	rm -f boot.img
	rm -f lzma.img	
	rm -f spram.bin
ifneq ($(strip $(TCSUPPORT_CPU_EN7512) $(TCSUPPORT_CPU_EN7521)),)
	make -C en7512_boot clean
endif

