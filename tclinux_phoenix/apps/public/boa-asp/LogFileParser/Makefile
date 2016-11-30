CROSS_PATH=mips-linux-
CROSS=$(CROSS_PATH)
CC=$(CROSS)gcc
AR=$(CROSS)ar
LD=$(CROSS)ld
STRIP=$(CROSS)strip
RANLIB=$(CROSS)ranlib
SIZE=$(CROSS)size
LIBDIR=$(CROSS_PATH)/lib

INCLUDE = -I$(KERNEL_DIR)/include

#HAVEMMU = 1
#CC=mips-linux-gcc
#CC=mips-linux-gcc
CFLAGS=-O2 -Wall -msoft-float

ifneq ($(TCSUPPORT_CT_NEWGUI), )
	CFLAGS += -DTCSUPPORT_CT_NEWGUI
endif

#LIBS = -lm libmxml.a -lcrypt
OBJ = LogFileParser.o 

%.o:%.c
	$(CC) $(CFLAGS) -c $<

LogFileParser: LogFileParser.o
	$(CC) -Wall -o LogFileParser $(OBJ) $(LIBS)
	mips-linux-strip LogFileParser
clean:
	rm -f *.o LogFileParser
