CC=gcc

ifndef CFLAGS
CFLAGS = -O2 -Wall -g
endif

# If you want to support multiple radius server for multiple bssid, add following line
#CFLAGS +=  -DMULTIPLE_RADIUS

# If you want to debug daemon, add following line
#CFLAGS +=  -DDBG

OBJS =	rt61apd.o eloop.o eapol_sm.o radius.o md5.o  \
	config.o ieee802_1x.o  \
	sta_info.o   radius_client.o

all: rt61apd

rt61apd: $(OBJS)
	$(CC) -o rt61apd $(OBJS)

clean:
	rm -f core *~ *.o rt61apd *.d

-include $(OBJS:%.o=%.d)
