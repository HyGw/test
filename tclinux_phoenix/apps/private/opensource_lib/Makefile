#
#Configured directories
#
#prefix	=	/usr/local
#exec_prefix	=	/usr/local
#libdir	=	$(exec_prefix)/lib
SRC=.
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

VPATH=$(SRC): $(SRC)/src

DSOFLAGS=	-Wl,-soname,libopensource.so.1,-rpath,$(libdir) -shared $(OPTIM)
LN      =	/bin/ln -s
RM      =	/bin/rm -f
OPTIM	=	-fPIC -Os -g


CFLAGS +=-fPIC -Os -g  -Wall

ifneq ($(TC_CFLAGS), )
        CFLAGS+=$(TC_CFLAGS)
endif

ifneq ($(OPENSOURCE_CFLAGS), )
		CFLAGS+=$(OPENSOURCE_CFLAGS)
endif

ifneq ($(COMPILEOPTION_CFLAGS), )
CFLAGS+=$(COMPILEOPTION_CFLAGS)
endif

DSOFLAGS += -L $(libdir) -ltcapi


.SUFFIXES:  .c .o
.c.o:
	$(CC) $(CFLAGS) -c $<

LIBOBJS =       libopensource.o
#LIBOBJS += smuxctl_opensource.o

LIBOBJS +=       libmtdpartition.o
LIBOBJS +=       libcmdci.o

LIBTCAPI=       libopensource.so.1.4
TARGETS =       $(LIBTCAPI)
SOFTLINK=		libopensource.so libopensource.so.1

all:  $(TARGETS)

#$(LIBOBJS): libopensource.h
#libopensource.o:libopensource.h

libopensource.so.1.4: $(LIBOBJS)
	$(CC) $(CFLAGS) $(DSOFLAGS) -o libopensource.so.1.4 $(LIBOBJS)
	$(STRIP) --strip-unneeded $(LIBTCAPI)
	$(RM) libopensource.so libopensource.so.1
	$(LN) libopensource.so.1.4 libopensource.so
	$(LN) libopensource.so.1.4 libopensource.so.1

install:
	$(INSTALL_DIR) $(BUILDROOT)$(libdir)
	$(INSTALL_LIB) libopensource.so.1.4 $(BUILDROOT)$(libdir)
	$(RM) $(BUILDROOT)$(libdir)/libopensource.so
	$(LN) libopensource.so.1.4 $(BUILDROOT)$(libdir)/libopensource.so
	$(RM) $(BUILDROOT)$(libdir)/libopensource.so.1
	$(LN) libopensource.so.1.4 $(BUILDROOT)$(libdir)/libopensource.so.1
	#cp libopensource.h $(BUILDROOT)$(libdir)
	#cp libopensource.h $(BUILDROOT)$(publiclibdir)
		
clean:
	$(RM) $(OBJS) $(TARGETS) $(LIBOBJS) $(SOFTLINK)
