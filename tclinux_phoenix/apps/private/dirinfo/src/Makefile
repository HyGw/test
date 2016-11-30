###########################################################################################
#determine the values of the variables according to your application scenario
###########################################################################################
MY_CFLAGS +=
MY_INCLUDE +=-I../../lib -I$(APP_CHARSET_DIR)/include
MY_LIBS +=-lpthread -L../../lib -lmjson-1.5 -L$(LIB_DIR) -lcharset -liconv ../../lib/libtcapi.so  -ldl
#SRCS =
TARGET=dirtest

###########################################################################################
#set environment variables
###########################################################################################
#CROSS?=mips-linux-gnu-
#CC?=$(CROSS)gcc -mips32r2 -msoft-float -muclibc
AR?=$(CROSS)ar
LD?=$(CROSS)ld
STRIP?=$(CROSS)strip
RANLIB?=$(CROSS)ranlib
SIZE?=$(CROSS)size
DEPENDFLAG=-MM

ifneq ($(TC_CFLAGS), )
		CFLAGS+=$(TC_CFLAGS)
endif

ifneq ($(COMPILEOPTION_CFLAGS), )
		CFLAGS+=$(COMPILEOPTION_CFLAGS)
endif

ifneq ($(strip $(TCSUPPORT_CODE_SHRINK)),)
CFLAGS += -Os -Wall $(MY_INCLUDE)
else
CFLAGS += -O2 -Wall $(MY_INCLUDE) 
endif

CFLAGS+=$(MY_CFLAGS)

ifneq ($(COMPILEOPTION_LDFLAGS), )
		LDFLAGS+=$(COMPILEOPTION_LDFLAGS)
endif
LDFLAGS+=$(MY_LIBS)

###########################################################################################
#set srcs, objs and dependss
###########################################################################################
DIRS = $(shell find . -type d)
SRCS ?= $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
OBJS = $(SRCS:%.c=%.o)
DEPENDS=$(SRCS:%.c=%.d)

###########################################################################################
#make all/clean
###########################################################################################
all: $(TARGET)

ifneq ($(MAKECMDGOALS), clean)
# dependency files, one for each *.c, named as *.d
%.d: %.c
	@echo ***********generating dependency file for $<
	@set -e; rm -f $@; \
	$(CC) $(DEPENDFLAG)$(CFLAGS) $< |\
	sed "s?\\(.*\\):?$(basename $<).o $(basename $<).d :?g" \
	> $@ || $(RM) $@
	
-include $(DEPENDS)
endif

%.o: %.c
	@echo ***********compile for $<
	$(CC) $(CFLAGS) -c -o $@ $< 

$(TARGET):$(OBJS)
	@echo ***********link
	$(CC) -Wall -o $(TARGET) $(OBJS) $(LDFLAGS)
	$(STRIP) $(TARGET)
	
clean:
	rm -f $(OBJS) $(DEPENDS) $(TARGET)
	

