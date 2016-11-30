ROOTPATH=$(shell pwd)


libdir=	../lib
privatelibdir= ../../private/lib

#for mjson lib
MJSONLIB = libmjson
MJSONSRCDIR = $(ROOTPATH)/json-1.5
MJSONLIBDIR = $(ROOTPATH)/
MJSONLIBINC = $(ROOTPATH)/mjson-1.5

EXEC = exec-proc
EXECDIR = $(ROOTPATH)/exec-process

ifneq ($(TC_CFLAGS),)
CFLAGS+=$(TC_CFLAGS)
endif

ifneq ($(COMPILEOPTION_CFLAGS), )
CFLAGS+=$(COMPILEOPTION_CFLAGS)
endif

ifneq ($(COMPILEOPTION_LDFLAGS), )
		TCAPILIB+=$(COMPILEOPTION_LDFLAGS)
endif

OBJDIR = ./
SRC = $(wildcard ./src/*.c ./parameter/src/*c)
OBJ = $(SRC:.c=.o)
RM = rm -rf
MV = mv -f
CP = cp -f

all : $(MJSONLIB) 

$(MJSONLIB):
ifneq ($(strip $(TCSUPPORT_NEWTOOLCHAIN)),)
	cd $(MJSONSRCDIR) && autoreconf -ivf && \
	./configure  --prefix=$(ROOTPATH) --libdir=$(MJSONLIBDIR) --includedir=$(MJSONLIBDIR) --disable-static --enable-shared \
	--host=mips-linux-uclibc CFLAGS="-g -O2 -Wall -mips32r2 -muclibc" \
	&& make clean && make && make install
else
	cd $(MJSONSRCDIR) &&\
	./configure  --prefix=$(ROOTPATH) --libdir=$(MJSONLIBDIR) --includedir=$(MJSONLIBDIR) --disable-static --enable-shared \
	--host=mips-linux-uclibc CFLAGS="-g -O2 -Wall -mips32r2 -muclibc" \
	&& make clean && make && make install
endif		


install:
	cp $(MJSONLIBINC)/json.h $(BUILDROOT)$(libdir)
	cp $(MJSONLIBINC)/json_helper.h $(BUILDROOT)$(libdir)
	cp $(MJSONLIBINC)/json.h $(BUILDROOT)$(privatelibdir)
	cp $(MJSONLIBINC)/json_helper.h $(BUILDROOT)$(privatelibdir)
	
clean:
	- cd $(MJSONSRCDIR) && make && make clean
	- $(RM) $(MJSONLIBDIR)/libmjson.*
	- $(RM) $(MJSONLIBDIR)/libmjson-1.5.*
		
.PHONY : all $(EXEC) $(MOBILELIB) $(MJSONLIB) $(EXE) $(SERVEREXE) $(CLIENTEXE) $(PLUGINEXE) clean
