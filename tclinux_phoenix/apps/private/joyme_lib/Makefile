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

DSOFLAGS=	-Wl,-soname,libjoyme.so.1,-rpath,$(libdir) -shared $(OPTIM)
LN      =	/bin/ln -s
RM      =	/bin/rm -f
OPTIM	=	-fPIC -Os -g

INCLUDE = -I$(KERNEL_HEADER) -I../../public/linux-atm/lib -I../lib

#CFLAGS= -mips1 -msoft-float 
CFLAGS +=-fPIC -Os -g  -Wall
LDFLAGS=-L ../lib -ltcapi -L ../lib -lmjson-1.5

ifneq ($(TC_CFLAGS), )
        CFLAGS+=$(TC_CFLAGS)
endif

.SUFFIXES:  .c .o
.c.o:
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

LIBOBJS =       joyme.o joyme_open.o
LIBJOYME=       libjoyme.so.1.4
TARGETS =       $(LIBJOYME)
SOFTLINK=		libjoyme.so libjoyme.so.1

all:  $(TARGETS)

$(LIBOBJS): joyme.h
joyme.o:joyme.h

libjoyme.so.1.4: $(LIBOBJS)
	$(CC) $(CFLAGS) $(DSOFLAGS) -o libjoyme.so.1.4 $(LIBOBJS) $(LDFLAGS)
	$(STRIP) --strip-unneeded $(LIBJOYME)
	$(RM) libjoyme.so libjoyme.so.1
	$(LN) libjoyme.so.1.4 libjoyme.so
	$(LN) libjoyme.so.1.4 libjoyme.so.1

install:
	$(INSTALL_DIR) $(BUILDROOT)$(libdir)
	$(INSTALL_LIB) libjoyme.so.1.4 $(BUILDROOT)$(libdir)
	$(RM) $(BUILDROOT)$(libdir)/libjoyme.so
	$(LN) libjoyme.so.1.4 $(BUILDROOT)$(libdir)/libjoyme.so
	$(RM) $(BUILDROOT)$(libdir)/libjoyme.so.1
	$(LN) libjoyme.so.1.4 $(BUILDROOT)$(libdir)/libjoyme.so.1
	cp joyme.h $(BUILDROOT)$(libdir)
	cp joyme.h $(BUILDROOT)$(publiclibdir)
		
clean:
	$(RM) $(OBJS) $(TARGETS) $(LIBOBJS) $(SOFTLINK)
