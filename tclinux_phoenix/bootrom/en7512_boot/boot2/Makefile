
####### Variables Declares #########################
CC 		= $(BOOTROM_CC)
LD 		= $(BOOTROM_LD)
NM 		= $(BOOTROM_NM)
OBJCOPY = $(BOOTROM_OBJCOPY)
OBJDUMP = $(BOOTROM_OBJDUMP)
CFLAGS	= $(BOOTROM_CFLAG)
IMG2BIN	= $(BOOTROM_IMG2BIN)

OUTDIR	= ./output

RM	= rm


TOOLCFLAGS	=

TOOLLDFLAGS	= -n

OPT	=  -G 0

TEXT	=

LDFLAGS = -T./ld_boot2.script0

LDFLAGS += -nostdlib -EB  -static

ASFLAGS	=  -D__ASSEMBLY__  -x assembler-with-cpp -G 0

CRT	=

LIBS	=

BOOTRAM_DIR=../bootram
BOOTSRAM_DIR=./


INCLUDES= -I. -I../../bootram/include

CFLAGS += $(INCLUDES)


############################################



.SUFFIXES : .s .S .c .o .out .nm .img .sr .sre .text .bin .scr

all: 
		@if [ ! -d output ]; then \
			mkdir output; \
		fi
		make $(OUTDIR)/boot2.out
		@echo  \#define BOOT_CODE_TIME \"`date `\" > ./banner/mk_time	 	
		$(NM)	./output/boot2.out | sort > ./output/boot2.nm
		$(NM)	--size-sort ./output/boot2.out > ./output/boot2_size.nm
		$(OBJCOPY) -g -Obinary ./output/boot2.out ./output/boot2.img
		$(OBJDUMP) -h -S ./output/boot2.out > ./output/boot2.text


##########################
OBJFILES	= $(OUTDIR)/boot2.o

##Assembly file######################################################
$(OUTDIR)/boot2.o: ./init/boot2.S 
	$(CC) -c $(CFLAGS) $(ASFLAGS) -o $(OUTDIR)/boot2.o ./init/boot2.S


##C file#############################################################

	
##Shrink C file#############################################################
$(OUTDIR)/boot2.out :	 $(OBJFILES)
	$(LD) $(LDFLAGS) $(OBJFILES) $(LIBS) -o $(OUTDIR)/boot2.out

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
	$(RM) -f $(OUTDIR)/boot2.out
	$(RM) -f $(OUTDIR)/boot2_size.nm
	$(RM) -f $(OUTDIR)/boot2.nm
	$(RM) -f $(OUTDIR)/boot2.img
	$(RM) -f $(OUTDIR)/boot2.text
	$(RM) -f $(OUTDIR)/*.o
	
