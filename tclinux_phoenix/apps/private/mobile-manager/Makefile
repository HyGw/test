ROOTPATH=$(shell pwd)

#for mobile common lib
MOBILELIB = libmobile
COMMONLIBDIR = $(ROOTPATH)/libmobile
COMMONLIBINC = $(COMMONLIBDIR)/include


CFLAGS = -Os -Wall -std=gnu99 \
	 -I./src -I$(COMMONLIBINC) -I../lib -I./parameter/inc -I$(APP_OPENSSL_DIR_V1)/OPSSL/include
	
ifneq ($(TC_CFLAGS),)
CFLAGS+=$(TC_CFLAGS)
endif

ifneq ($(COMPILEOPTION_CFLAGS), )
CFLAGS+=$(COMPILEOPTION_CFLAGS)
endif

LDFLAGS = -lpthread -L ../lib -lmjson-1.5 -L$(COMMONLIBDIR) -lmobile -L $(APP_OPENSSL_DIR_V1)/OPSSL/lib -lssl -lcrypto

ifneq ($(TCSUPPORT_CWMP_FAST_GET),)
TCAPILIB=-L ../lib_cwmp -ltcapi_cwmp
TCAPILIB+=-L ../lib/ -lmxml
else
TCAPILIB=-L ../lib -ltcapi
endif
ifneq ($(TCSUPPORT_CT_JOYME), )
TCAPILIB+= -L ../lib -ljoyme
endif

ifneq ($(COMPILEOPTION_LDFLAGS), )
		TCAPILIB+=$(COMPILEOPTION_LDFLAGS)
endif

EXE = mobile-manager
OBJDIR = ./
SRC = $(wildcard ./src/*.c ./parameter/src/*c)
OBJ = $(SRC:.c=.o)
OBJ += $(wildcard ./src/*.o)
RM = rm -rf
MV = mv -f
CP = cp -f

all : $(EXE)

$(EXE) : $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(TCAPILIB)

.c.o :
	$(CC) -c $< -o $@ $(CFLAGS)
	
clean : $(EXE)-clean
$(EXE)-clean:
	rm -f $(OBJ)
	rm -f $(EXE)
		
.PHONY : all $(EXE) clean
