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

DSOFLAGS=	-Wl,-soname,libcompileoption.so.1,-rpath,$(libdir) -shared $(OPTIM)
LN      =	/bin/ln -s
RM      =	/bin/rm -f
OPTIM	=	-fPIC -Os -g


#INCLUDE = -I$(KERNEL_DIR)/include 

#CFLAGS= -mips1 -msoft-float 
CFLAGS +=-fPIC -Os -g  -Wall

ifneq ($(TC_CFLAGS), )
        CFLAGS+=$(TC_CFLAGS)
endif

.SUFFIXES:  .c .o
.c.o:
	$(CC) $(CFLAGS) -c $<

LIBOBJS =       libcompileoption.o
LIBTCAPI=       libcompileoption.so.1.4
TARGETS =       $(LIBTCAPI)
SOFTLINK=		libcompileoption.so libcompileoption.so.1

all:  $(TARGETS)

$(LIBOBJS): libcompileoption.h
libcompileoption.o:libcompileoption.h

libcompileoption.so.1.4: $(LIBOBJS)
	$(CC) $(CFLAGS) $(DSOFLAGS) -o libcompileoption.so.1.4 $(LIBOBJS)
	$(STRIP) --strip-unneeded $(LIBTCAPI)
	$(RM) libcompileoption.so libcompileoption.so.1
	$(LN) libcompileoption.so.1.4 libcompileoption.so
	$(LN) libcompileoption.so.1.4 libcompileoption.so.1

install:
	$(INSTALL_DIR) $(BUILDROOT)$(libdir)
	$(INSTALL_LIB) libcompileoption.so.1.4 $(BUILDROOT)$(libdir)
	$(RM) $(BUILDROOT)$(libdir)/libcompileoption.so
	$(LN) libcompileoption.so.1.4 $(BUILDROOT)$(libdir)/libcompileoption.so
	$(RM) $(BUILDROOT)$(libdir)/libcompileoption.so.1
	$(LN) libcompileoption.so.1.4 $(BUILDROOT)$(libdir)/libcompileoption.so.1
	cp libcompileoption.h $(BUILDROOT)$(libdir)
	cp libcompileoption.h $(BUILDROOT)$(publiclibdir)
		
clean:
	$(RM) $(OBJS) $(TARGETS) $(LIBOBJS) $(SOFTLINK)
