#
#Configured directories
#
#prefix	=	/usr/local
#exec_prefix	=	/usr/local
#libdir	=	$(exec_prefix)/lib
libdir=	../lib
publiclibdir= ../../public/lib
#
#Install commands
#
INSTALL=	/usr/bin/install -c
INSTALL_DIR	=	$(INSTALL) -d
INSTALL_LIB	=	$(LIBTOOL)	$(INSTALL) -m 755

#CROSS=mips-linux-gnu-

#CC=$(CROSS)gcc -mips32r2 -msoft-float -muclibc
AR=$(CROSS)ar
LD=$(CROSS)ld 
STRIP=$(CROSS)strip
RANLIB=$(CROSS)ranlib
SIZE=$(CROSS)size
LIBDIR=$(CROSS_PATH)/lib

DSOFLAGS=	-Wl,-soname,libbdService.so.1,-rpath,$(libdir) -shared $(OPTIM)
LN      =	/bin/ln -s
RM      =	/bin/rm -f
OPTIM	=	-fPIC -Os -g

INCLUDE = -I$(KERNEL_DIR)/include -I../lib -I./include -I./include/linux

#CFLAGS= -mips1 -msoft-float 
CFLAGS +=-fPIC -Os -g  -Wall
LDFLAGS=-L ../lib -ltcapi -L ../lib -lmjson-1.5 -L ../lib -lbundle

ifneq ($(TC_CFLAGS), )
        CFLAGS+=$(TC_CFLAGS)
endif

.SUFFIXES:  .c .o
.c.o:
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

LIBOBJS =       bdService.o
LIBBDSERVICE=       libbdService.so
TARGETS =       $(LIBBDSERVICE)

all:  $(TARGETS)

$(LIBOBJS): bdService.h
bdService.o:bdService.h

libbdService.so: $(LIBOBJS)
	$(CC) $(CFLAGS) $(DSOFLAGS) -o libbdService.so $(LIBOBJS) $(LDFLAGS)
	$(STRIP) --strip-unneeded $(LIBBDSERVICE)
		
clean:
	$(RM) $(OBJS) $(TARGETS) $(LIBOBJS)
