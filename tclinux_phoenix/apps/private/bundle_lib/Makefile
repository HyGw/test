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
	$(INSTALL_LIB) libbundle.so.1.4 $(BUILDROOT)$(libdir)
	$(RM) $(BUILDROOT)$(libdir)/libbundle.so
	$(LN) libbundle.so.1.4 $(BUILDROOT)$(libdir)/libbundle.so
	$(RM) $(BUILDROOT)$(libdir)/libbundle.so.1
	$(LN) libbundle.so.1.4 $(BUILDROOT)$(libdir)/libbundle.so.1
	cp bundle.h $(BUILDROOT)$(libdir)
	cp bundle.h $(BUILDROOT)$(publiclibdir)

