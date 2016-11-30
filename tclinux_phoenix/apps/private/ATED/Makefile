EXEC = ated
SRCS = ate.c

#PLATFORM = 5VT
#PLATFORM = STAR
#PLATFORM = INF_TWINPASS
#PLATFORM = INF_AR9
#PLATFORM = BROADCOM
#PLATFORM = PC
PLATFORM = TC3162U

ifeq ($(PLATFORM),5VT)
#Please fix the PATH to fit your environment...
CROSS_COMPILE = /opt/crosstool/uClibc_v5te_le_gcc_4_1_1/bin/arm-linux-
endif

ifeq ($(PLATFORM),STAR)
#Please fix the PATH to fit your environment...
CROSS_COMPILE = /opt/star/tools/arm-linux/bin/arm-linux-
endif

ifeq ($(PLATFORM),INF_TWINPASS)
#Please fix the PATH to fit your environment...
CROSS_COMPILE = /opt/uclibc-toolchain/ifx-lxdb-1-1/gcc-3.3.6/toolchain-mips/bin/mips-linux-uclibc-
EXTRA_CFLAGS += -Dbig_endian
endif

ifeq ($(PLATFORM),INF_AR9)
#Please fix the PATH to fit your environment...
CROSS_COMPILE = /opt/uclibc-toolchain/ifx-lxdb26-1.0.2/gcc-3.4.4/toolchain-mips/bin/mips-linux-uclibc-
EXTRA_CFLAGS += -Dbig_endian
endif

ifeq ($(PLATFORM),BROADCOM)
#Please fix the PATH to fit your environment...
CROSS_COMPILE = /opt/uclibc-toolchain/bin/mips-linux-uclibc-
EXTRA_CFLAGS += -Dbig_endian
endif

ifeq ($(PLATFORM),PC)
CROSS_COMPILE =
endif

ifeq ($(PLATFORM),TC3162U) 
#CROSS_COMPILE = mips-linux-
STRIP = $(CROSS_COMPILE)strip
EXTRA_CFLAGS += -Dbig_endian
endif

EXTRA_CFLAGS += -DDBG

CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld

all: $(EXEC)

ifeq ($(PLATFORM),PC)
$(EXEC): $(SRCS) 
	$(CC) -v -Wall $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) -o $@ $(SRCS) $(LDLIBS)
else
$(EXEC): $(SRCS) 
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) -o $@ $(SRCS) $(LDLIBS)
	#cp ated /tftpboot
	$(STRIP) -s $(EXEC)
endif

clean:
	-rm -f $(EXEC) *.o

