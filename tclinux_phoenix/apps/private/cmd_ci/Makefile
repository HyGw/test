#CROSS=mips-linux-gnu-

#CC=$(CROSS)gcc
AR=$(CROSS)ar
LD=$(CROSS)ld
STRIP=$(CROSS)strip
RANLIB=$(CROSS)ranlib
SIZE=$(CROSS)size

INCLUDE = -I$(KERNEL_HEADER)

KERNEL_INCLUDES:=$(KERNEL_HEADER)

#CFLAGS= -mips1 -msoft-float 
#CFLAGS= -mips1 -msoft-float 
#CFLAGS= -mips32r2 -msoft-float -DTRENDCHIP -muclibc
ifeq ($(TC2031_SUPPORT),1)
CFLAGS += -DTC2031_DEBUG
endif

ifneq ($(strip $(TC_CFLAGS)),)
CFLAGS += $(TC_CFLAGS)
endif

ATMOBJS=atmcmd.o
ADSLOBJS=adslcmd.o
WLANOBJS=wlancmd.o

all: atmcmd adslcmd wlancmd

.c.o:
	$(CC) -c $(CFLAGS) $< -I$(KERNEL_INCLUDES)

atmcmd: $(ATMOBJS) 
	$(CC) $(ATMOBJS) -o $@
	$(STRIP) --strip-unneeded atmcmd

adslcmd: $(ADSLOBJS) 
	$(CC) $(ADSLOBJS) -o $@
	$(STRIP) --strip-unneeded adslcmd
	
wlancmd: $(WLANOBJS) 
	$(CC) $(WLANOBJS) -o $@
	$(STRIP) --strip-unneeded wlancmd

clean:
	rm -f *.o *.a atmcmd adslcmd wlancmd
