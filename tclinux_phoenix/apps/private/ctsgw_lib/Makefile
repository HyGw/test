libdir=	../lib
publiclibdir= ../../public/lib
#
#Install commands
#
INSTALL=	/usr/bin/install -c
INSTALL_DIR	=	$(INSTALL) -d
INSTALL_LIB	=	$(LIBTOOL)	$(INSTALL) -m 755

AR=$(CROSS)ar
LD=$(CROSS)ld 
STRIP=$(CROSS)strip
RANLIB=$(CROSS)ranlib
SIZE=$(CROSS)size
LIBDIR=$(CROSS_PATH)/lib

LN      =	/bin/ln -s
RM      =	/bin/rm -f

install:
	$(INSTALL_DIR) $(BUILDROOT)$(libdir)
	$(INSTALL_LIB) libctsgw.so.1.4 $(BUILDROOT)$(libdir)
	$(RM) $(BUILDROOT)$(libdir)/libctsgw.so
	$(LN) libctsgw.so.1.4 $(BUILDROOT)$(libdir)/libctsgw.so
	$(RM) $(BUILDROOT)$(libdir)/libctsgw.so.1
	$(LN) libctsgw.so.1.4 $(BUILDROOT)$(libdir)/libctsgw.so.1
	cp ctsgw.h $(BUILDROOT)$(libdir)
	cp ctsgw.h $(BUILDROOT)$(publiclibdir)

