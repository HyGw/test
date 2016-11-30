#CROSS=mips-linux-gnu-

#CC=$(CROSS)gcc
AR=$(CROSS)ar
LD=$(CROSS)ld
STRIP=$(CROSS)strip
RANLIB=$(CROSS)ranlib
SIZE=$(CROSS)size

INCLUDE = -I$(KERNEL_DIR)/include 

#CFLAGS= -mips1 -msoft-float 
#CFLAGS= -mips1 -msoft-float 
#CFLAGS= -mips32r2 -msoft-float -DTRENDCHIP -muclibc
ifeq ($(TC2031_SUPPORT),1)
CFLAGS += -DTC2031_DEBUG
endif

ifneq ($(strip $(TCSUPPORT_CODE_SHRINK)),)
CFLAGS += -Os
endif

ifneq ($(TC_CFLAGS), )
CFLAGS += $(TC_CFLAGS)
endif



LDFLAGS += -L ../lib -ltcapi

OBJS=tcci.o

all: tcci

.c.o:
	$(CC) -c $(CFLAGS) $<

tcci: $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)
	$(STRIP) tcci
clean:
	rm -f *.o *.a $(EXECS)
