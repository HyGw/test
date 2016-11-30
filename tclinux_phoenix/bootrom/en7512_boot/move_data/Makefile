
####### Variables Declares #########################
CC 		= $(BOOTROM_CC)
LD 		= $(BOOTROM_LD)
NM 		= $(BOOTROM_NM)
OBJCOPY = $(BOOTROM_OBJCOPY)
OBJDUMP = $(BOOTROM_OBJDUMP)
CFLAGS	= $(BOOTROM_CFLAG)
IMG2BIN	= $(BOOTROM_IMG2BIN)

OUTDIR	= ./output

IMG2BIN	= $(CROSS_COMPILE)img2bin

RM	= rm

TOOLCFLAGS	=

TOOLLDFLAGS	= -n

OPT	=  -G 0

TEXT	=

INCLUDES= -I. -I../../bootram/include

LDFLAGS = -T./ld_move_data.script0

CFLAGS += $(INCLUDES)


LDFLAGS += -nostdlib -EB  -static

ASFLAGS	=  -D__ASSEMBLY__  -x assembler-with-cpp -G 0

ifeq ($(strip $(TCSUPPORT_GCC_VER)),493)
ASFLAGS	+= -O2
endif

CRT	=

LIBS	=

BOOTRAM_DIR=../bootram
BOOTSRAM_DIR=./

.SUFFIXES : .s .S .c .o .out .nm .img .sr .sre .text .bin .scr

all: 
		@if [ ! -d output ]; then \
			mkdir output; \
		fi
		make $(OUTDIR)/move_data.out
		@echo  \#define BOOT_CODE_TIME \"`date `\" > ./banner/mk_time	 	
		$(NM)	./output/move_data.out | sort > ./output/move_data.nm
		$(NM)	--size-sort ./output/move_data.out > ./output/move_data_size.nm
		$(OBJCOPY) -g -Obinary ./output/move_data.out ./output/move_data.img
		$(OBJDUMP) -h -S ./output/move_data.out > ./output/move_data.text

##########################
OBJFILES	= $(OUTDIR)/move_data.o

##Assembly file######################################################
$(OUTDIR)/move_data.o: ./init/move_data.S 
	$(CC) -c $(CFLAGS) $(ASFLAGS) -o $(OUTDIR)/move_data.o ./init/move_data.S


##C file#############################################################

	
##Shrink C file#############################################################
$(OUTDIR)/move_data.out :	 $(OBJFILES)
	$(LD) $(LDFLAGS) $(OBJFILES) $(LIBS) -o $(OUTDIR)/move_data.out

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
	$(RM) -f $(OUTDIR)/move_data.out
	$(RM) -f $(OUTDIR)/move_data.nm
	$(RM) -f $(OUTDIR)/move_data_size.nm
	$(RM) -f $(OUTDIR)/move_data.img
	$(RM) -f $(OUTDIR)/move_data.text
	$(RM) -f $(OUTDIR)/*.o
	
