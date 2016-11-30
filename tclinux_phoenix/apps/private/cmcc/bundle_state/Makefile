#
#Configured directories
#
libdir=	../../lib
publiclibdir= ../../../public/lib
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

DSOFLAGS=	-Wl,-soname,libbundleState.so.1,-rpath,$(libdir) -shared $(OPTIM)
LN      =	/bin/ln -s
RM      =	/bin/rm -f
OPTIM	=	-fPIC -Os -g

INCLUDE = -I$(KERNEL_DIR)/include -I../lib -I./include -I./include/linux

#CFLAGS= -mips1 -msoft-float 
CFLAGS +=-fPIC -Os -g  -Wall
LDFLAGS=-L ../../lib -ltcapi -L ../../lib -lmjson-1.5 

ifneq ($(TC_CFLAGS), )
        CFLAGS+=$(TC_CFLAGS)
endif

.SUFFIXES:  .c .o
.c.o:
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

LIBOBJS =       bundleState.o
LIBPARAMODE=       libbundleState.so.1.4
TARGETS =       $(LIBPARAMODE)
SOFTLINK=		libbundleState.so libbundleState.so.1

all:  $(TARGETS)

$(LIBOBJS): bundleState.h
bundleState.o:bundleState.h

libbundleState.so.1.4: $(LIBOBJS)
	$(CC) $(CFLAGS) $(DSOFLAGS) -o libbundleState.so.1.4 $(LIBOBJS) $(LDFLAGS)
	$(STRIP) --strip-unneeded $(LIBPARAMODE)
	$(RM) libbundleState.so libbundleState.so.1
	$(LN) libbundleState.so.1.4 libbundleState.so
	$(LN) libbundleState.so.1.4 libbundleState.so.1

install:
	$(INSTALL_DIR) $(BUILDROOT)$(libdir)
	$(INSTALL_LIB) libbundleState.so.1.4 $(BUILDROOT)$(libdir)
	$(RM) $(BUILDROOT)$(libdir)/libbundleState.so
	$(LN) libbundleState.so.1.4 $(BUILDROOT)$(libdir)/libbundleState.so
	$(RM) $(BUILDROOT)$(libdir)/libbundleState.so.1
	$(LN) libbundleState.so.1.4 $(BUILDROOT)$(libdir)/libbundleState.so.1
	cp bundleState.h $(BUILDROOT)$(libdir)
	cp bundleState.h $(BUILDROOT)$(publiclibdir)
		
clean:
	$(RM) $(OBJS) $(TARGETS) $(LIBOBJS)
