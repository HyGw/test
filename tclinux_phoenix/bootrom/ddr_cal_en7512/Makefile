

PLATFORM =

ifeq ($(RT63165),1)
PLATFORM += -DRT63165
endif

ifeq ($(RT63365),1)
PLATFORM += -DRT63365
endif

ifeq ($(MT7510),1)
PLATFORM += -DMT7510
PLATFORM += -DTCSUPPORT_MT7510_FE
endif

ifeq ($(MT7505),1)
PLATFORM += -DMT7505
MIPS16_CODE_SHRINK=1
endif

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
endif





PLATFORM += -DSPRAM_IMG

ifeq ($(DDR_CALI_NAND),1)
PLATFORM += -DDDR_CALI_NAND
endif

#ifeq ($(TC3262),1)
#CROSS_COMPILE = mips-linux-gnu-
#else
#CROSS_COMPILE = mips-linux-
#endif

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

CFLAGS_MIPS16 =

INCLUDES	= -I. -I../bootram/include -I./inc
CFLAGS	= -mips32r2 -msoft-float -fomit-frame-pointer -nostdinc -fno-pic -mno-abicalls $(TOOLCFLAGS) $(OPT) $(INCLUDES) -D__KERNEL__ -Dlinux $(PLATFORM)
CFLAGS	+= -Os 
#mips16 code can not run in ISPRAM, because there are data in text section
#ifeq ($(DDR_CALI_NAND),1)
#LDFLAGS = -nostdlib  -T./ld_spram_nand.script  -EB  -static
#else
LDFLAGS = -nostdlib  -T./ld_spram.script  -EB  -static
#endif

ifeq ($(MIPS16_CODE_SHRINK),1)
CFLAGS_MIPS16 +=-mips16
endif

ASFLAGS	=  -D__ASSEMBLY__  -x assembler-with-cpp -G 0

CRT	=

LIBS	=

.SUFFIXES : .s .S .c .o .out .nm .img .sr .sre .text .bin .scr

all: 
		@if [ ! -d output ]; then \
			mkdir output; \
		fi
		make $(OUTDIR)/boot.out
		@echo  \#define BOOT_CODE_TIME \"`date `\" > ../bootram/banner/mk_time	 	
		$(NM)	./output/boot.out | sort > ./output/boot.nm
		$(OBJCOPY) -g -Obinary ./output/boot.out ./output/boot.img
		$(OBJDUMP) -h -S ./output/boot.out > ./output/boot.text

		cp ./output/boot.img ./spram.img -f

		cp ./spram.img ../spram.bin -f
	

##########################head.o must be the first.
OBJFILES	= $(OUTDIR)/head.o \
		  $(OUTDIR)/setup.o \
		  $(OUTDIR)/main.o  \
		  $(OUTDIR)/init.o \
		  $(OUTDIR)/time.o \
		  $(OUTDIR)/string.o \
		  $(OUTDIR)/dramc.o \
		  $(OUTDIR)/dramc_dq_dqs_cal.o \
		  $(OUTDIR)/dramc_dle_cal.o \
		  $(OUTDIR)/dramc_dqs_gw_cal.o \
		$(OUTDIR)/en7512_dramc_init.o \
		$(OUTDIR)/spram.o



##Assembly file######################################################
$(OUTDIR)/int-handler.o: ./init/int-handler.S 
	$(CC) -c $(CFLAGS) $(ASFLAGS) -o $(OUTDIR)/int-handler.o ./init/int-handler.S
$(OUTDIR)/head.o: ../bootram/arch/mips/kernel/head.S 
	$(CC) -c $(CFLAGS) $(ASFLAGS) -o $(OUTDIR)/head.o ../bootram/arch/mips/kernel/head.S




##C file#############################################################
$(OUTDIR)/time.o: ../bootram/init/time.c 
	$(CC) -c $(CFLAGS) $(CFLAGS_MIPS16) -o $(OUTDIR)/time.o ../bootram/init/time.c


$(OUTDIR)/setup.o: ../bootram/arch/mips/kernel/setup.c 
	$(CC) -c $(CFLAGS) $(CFLAGS_MIPS16) -o $(OUTDIR)/setup.o ../bootram/arch/mips/kernel/setup.c

$(OUTDIR)/init.o: ../bootram/io/init.c 
	$(CC) -c $(CFLAGS) $(CFLAGS_MIPS16) -o $(OUTDIR)/init.o ../bootram/io/init.c

$(OUTDIR)/string.o: ../bootram/lib/string.c 
	$(CC) -c $(CFLAGS) $(CFLAGS_MIPS16) -o $(OUTDIR)/string.o ../bootram/lib/string.c

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
	$(RM) -f $(OUTDIR)/time.o
	$(RM) -f $(OUTDIR)/setup.o
	$(RM) -f $(OUTDIR)/init.o
	$(RM) -f $(OUTDIR)/string.o
	$(RM) -f $(OUTDIR)/head.o
	$(RM) -f $(OUTDIR)/boot.nm
	$(RM) -f $(OUTDIR)/boot.img
	$(RM) -f $(OUTDIR)/boot.text
	$(RM) -f $(OUTDIR)/boot.out

