AR=$(CROSS)ar
LD=$(CROSS)ld
STRIP=$(CROSS)strip
RANLIB=$(CROSS)ranlib
SIZE=$(CROSS)size

INCLUDE = -I$(KERNEL_DIR)/include 

ifneq ($(strip $(TCSUPPORT_CODE_SHRINK)),)
CFLAGS += -Os
endif

ifneq ($(TC_CFLAGS), )
CFLAGS += $(TC_CFLAGS)
endif

LDFLAGS += -L ../../lib -ltcapi

OBJS=midware-server.o

all: midware-server

.c.o:
	$(CC) -c $(CFLAGS) $<

midware-server: $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)
	$(STRIP) midware-server
clean:
	rm -f *.o *.a $(EXECS)
