ROOTPATH=$(shell pwd)

DEBUG_LOCAL:=y

LXC=lxc
LXCDIR=$(ROOTPATH)/lxc-1.0.8
LXCPREFIX=$(ROOTPATH)/install_lxclib
LXCBIN=$(ROOTPATH)/install_lxcsbin

LIBCAP=libpcap
LIBCAPDIR=$(ROOTPATH)/libcap-2.17
LIBCAPPREFIX=$(ROOTPATH)/install_libcap

DBUS=dbus
DBUSDIR=$(ROOTPATH)/dbus-1.8.8
DBUSPREFIX=$(ROOTPATH)/install_dbuslib
DBUSBIN=$(ROOTPATH)/install_dbusbin

GLIB=glib
#GLIBDIR=$(ROOTPATH)/glib-2.46.2
GLIBDIR=$(ROOTPATH)/glib-2.41.1
GLIB_PRLIB=$(ROOTPATH)/install_glib_prelib
GLIBPREFIX=$(ROOTPATH)/install_glib
GLIBBIN=$(ROOTPATH)/install_glibbin
GLIBCACHEFILE=$(GLIBDIR)/mips.cache

ZLIB =zlib
ZLIBDIR = $(ROOTPATH)/zlib-1.2.8
#ZLIBPREFIX = $(ROOTPATH)/zlib_install
ZLIBPREFIX = $(GLIB_PRLIB)

EXPAT = expat
EXPATDIR = $(ROOTPATH)/expat-2.1.0
EXPATPREFIX = $(ROOTPATH)/install_expat

ICONV = libiconv
ICONVDIR = $(ROOTPATH)/libiconv-1.14
#ICONVPREFIX = $(ROOTPATH)/libiconv_install
ICONVPREFIX = $(GLIB_PRLIB)

FFI = libffi
FFIDIR = $(ROOTPATH)/libffi-3.2.1
#FFIPREFIX = $(ROOTPATH)/libffi_install
FFIPREFIX = $(GLIB_PRLIB)

GETTEXT = gettext
GETTEXTDIR = $(ROOTPATH)/gettext-0.19.7
#GETTEXTPREFIX = $(ROOTPATH)/gettext_install
GETTEXTPREFIX = $(GLIB_PRLIB)

GDBUS=gdbus-test

_TARGET_LEVEL := 0
TARGET_ALL= $(DBUS) $(GLIB) $(LXC) install
TARGET_CLEAN = $(GLIB)-clean $(DBUS)-clean $(LXC)-clean

ifeq ($(strip $(_TARGET_LEVEL)), 1)
TARGET_ALL=$(LXC) install
TARGET_CLEAN = $(LXC)-clean
endif

#all : $(DBUS) $(GLIB) install  $(LXC)
all :  $(TARGET_ALL)
#http://download.chinaunix.net/download.php?id=28466&ResourceID=5757
#notices!!!!!! makefile.rules change gcc to mips-gcc
$(LIBCAP):
	cd $(LIBCAPDIR) &&  make clean all && make prefix=$(LIBCAPPREFIX) install

#cd $(LXCDIR) && ./configure --host=$(COMPILE_TOOLCHAIN) --target=$(COMPILE_TOOLCHAIN) --prefix=$(LXCPREFIX) --exec-prefix=/usr \
#$(LXC): $(LIBCAP)
$(LXC):
	cd $(LXCDIR) && ./configure --help
	#for lxc clean error
	cp -rf lxc-Makefile.backup $(LXCDIR)/config/etc/Makefile.in
	cd $(LXCDIR) && ./configure --help
	if [ -f $(LXCDIR)/_is_configure ] ; then \
	echo "already configred..... "; \
	else \
	cd $(LXCDIR) && ./configure --host=$(COMPILE_TOOLCHAIN) --target=$(COMPILE_TOOLCHAIN) --prefix=$(LXCPREFIX) \
	--disable-apparmor \
	--disable-doc \
	--disable-api-docs \
	--disable-selinux \
	--disable-cgmanager \
	--disable-examples \
	--disable-python \
	--disable-bash \
	--disable-lua \
	--enable-capabilities=yes \
	--disable-seccomp \
	CFLAGS="-I$(LIBCAPPREFIX)/include/sys" \
	LDFLAGS="-L$(LIBCAPPREFIX)/lib" \
	LIBS="-lcap" \
	CC="$(CC)  -D__NR_setns=4344" ;\
	touch $(LXCDIR)/_is_configure; \
	fi
	
	cd $(LXCDIR) && make
#&& make install
	/bin/mkdir -p $(LXCBIN)/usr/bin
	/bin/mkdir -p $(LXCBIN)/usr/lib
	/bin/mkdir -p $(LXCBIN)/usr/libexec/lxc
	/bin/mkdir -p $(LXCBIN)/usr/sbin
	/bin/mkdir -p $(LXCBIN)/usr/include/lxc
	/bin/mkdir -p $(LXCBIN)/usr/share/lxc/templates
	/bin/mkdir -p $(LXCBIN)/usr/lib/lxc/rootfs
	/bin/mkdir -p $(LXCBIN)/usr/share/lxc/hooks
	/bin/mkdir -p $(LXCBIN)/usr/var/lib/lxc
	/bin/mkdir -p $(LXCBIN)/usr/var/cache/lxc
	/bin/mkdir -p $(LXCBIN)/usr/lib/pkgconfig
	/bin/mkdir -p $(LXCBIN)/usr/etc/lxc
	/bin/mkdir -p $(LXCBIN)/usr/etc/rc.d/init.d
	/bin/mkdir -p $(LXCBIN)/usr/share/lxc/config
	/bin/mkdir -p $(LXCBIN)/usr/share/lxc
	/bin/mkdir -p $(LXCBIN)/usr/share/lxc/selinux
	
	cd $(LXCDIR)/src/lxc && /usr/bin/install  -c lxc-attach lxc-autostart lxc-cgroup lxc-clone lxc-config lxc-console lxc-create lxc-destroy lxc-execute lxc-freeze lxc-info lxc-monitor lxc-snapshot lxc-start lxc-stop lxc-unfreeze lxc-unshare lxc-usernsexec lxc-wait  $(LXCBIN)/usr/bin
	cd $(LXCDIR)/src/lxc &&	install -c -m 644 lxc.functions $(LXCBIN)/usr/share/lxc	
	cd $(LXCDIR)/src/lxc &&	/usr/bin/install -c lxc-checkconfig legacy/lxc-ls $(LXCBIN)/usr/bin	
	cd $(LXCDIR)/src/lxc && mv liblxc.so liblxc.so.1.0.8 && ln -sf liblxc.so.1.0.8 liblxc.so.1 && ln -sf liblxc.so.1 liblxc.so && cp -rf liblxc.so liblxc.so.1.0.8 liblxc.so.1 $(LXCBIN)/usr/lib -d	
	cd $(LXCDIR)/src/lxc &&	/usr/bin/install -c lxc-monitord lxc-user-nic $(LXCBIN)/usr/libexec/lxc	
	cd $(LXCDIR)/src/lxc &&	/usr/bin/install -c init.lxc $(LXCBIN)/usr/sbin
	cd $(LXCDIR)/src/lxc &&	/usr/bin/install -c -m 644 attach_options.h lxccontainer.h version.h $(LXCBIN)/usr/include/lxc	
	cd $(LXCDIR)/templates &&	/usr/bin/install -c lxc-alpine lxc-altlinux lxc-archlinux lxc-busybox lxc-centos lxc-cirros lxc-debian lxc-download lxc-fedora lxc-gentoo lxc-openmandriva lxc-opensuse lxc-oracle lxc-plamo lxc-sshd lxc-ubuntu lxc-ubuntu-cloud $(LXCBIN)/usr/share/lxc/templates	
	cd $(LXCDIR)/doc/rootfs && /usr/bin/install -c -m 644 README $(LXCBIN)/usr/lib/lxc/rootfs
	cd $(LXCDIR)/hooks/ && /usr/bin/install -c clonehostname mountecryptfsroot ubuntu-cloud-prep squid-deb-proxy-client $(LXCBIN)/usr/share/lxc/hooks
	cd $(LXCDIR) && /usr/bin/install -c -m 644 lxc.pc $(LXCBIN)/usr/lib/pkgconfig	
	cd $(LXCDIR)/config/etc && /usr/bin/install -c -m 644 default.conf $(LXCBIN)/usr/etc/lxc
	cd $(LXCDIR)/config/init/sysvinit/ && /usr/bin/install -c lxc $(LXCBIN)/usr/etc/rc.d/init.d/lxc	
	cd $(LXCDIR)/config/templates && /usr/bin/install -c -m 644 centos.common.conf centos.userns.conf common.seccomp debian.common.conf debian.userns.conf fedora.common.conf fedora.userns.conf gentoo.common.conf gentoo.moresecure.conf gentoo.userns.conf oracle.common.conf oracle.userns.conf plamo.common.conf plamo.userns.conf ubuntu-cloud.common.conf ubuntu-cloud.lucid.conf ubuntu-cloud.userns.conf ubuntu.common.conf ubuntu.lucid.conf ubuntu.userns.conf $(LXCBIN)/usr/share/lxc/config	
	cd $(LXCDIR)/config/yum/ && /usr/bin/install -c -m 644 lxc-patch.py $(LXCBIN)/usr/share/lxc	
	cd $(LXCDIR)/config/selinux && /usr/bin/install -c -m 644 lxc.if lxc.te $(LXCBIN)/usr/share/lxc/selinux	
	
	cp $(LIBCAPPREFIX)/lib/libcap.so $(LXCBIN)/usr/lib -d
	cp $(LIBCAPPREFIX)/lib/libcap.so.2 $(LXCBIN)/usr/lib -d
	cp $(LIBCAPPREFIX)/lib/libcap.so.2.17 $(LXCBIN)/usr/lib -d

$(EXPAT):
	echo "test"
	cd $(EXPATDIR) && ./configure --help
	if [ -f $(EXPATDIR)/_is_configure ] ; then \
	echo "already configred..... "; \
	else \
	cd $(EXPATDIR) && ./configure --host=$(COMPILE_TOOLCHAIN) --target=$(COMPILE_TOOLCHAIN) --prefix=$(EXPATPREFIX) && make && make install; \
	touch $(EXPATDIR)/_is_configure; \
	fi

$(ICONV):
	cd $(ICONVDIR) && ./configure --help
	if [ -f $(ICONVDIR)/_is_configure ] ; then \
	echo "already configred..... "; \
	else \
	cd $(ICONVDIR) && ./configure --host=$(COMPILE_TOOLCHAIN) --target=$(COMPILE_TOOLCHAIN) --prefix=$(ICONVPREFIX) && make && make install; \
	touch $(ICONVDIR)/_is_configure; \
	fi

$(ZLIB):
	#cd $(ZLIBDIR) && ./configure --help
	if [ -f $(ZLIBDIR)/_is_configure ] ; then \
	echo "already configred..... "; \
	else \
	cd $(ZLIBDIR) && ./configure --prefix=$(ZLIBPREFIX) && make && make install; \
	touch $(ZLIBDIR)/_is_configure; \
	fi

$(FFI):
	cd $(FFIDIR) && ./configure --help
	if [ -f $(FFIDIR)/_is_configure ] ; then \
	echo "already configred..... "; \
	else \
	cd $(FFIDIR) && ./configure --host=$(COMPILE_TOOLCHAIN) --target=$(COMPILE_TOOLCHAIN) --prefix=$(FFIPREFIX) --includedir=$(FFIPREFIX)/include && make && make install; \
	touch $(FFIDIR)/_is_configure; \
	fi

$(GETTEXT):
	cd $(GETTEXTDIR) && ./configure --help
	if [ -f $(GETTEXTDIR)/_is_configure ] ; then \
	echo "already configred..... "; \
	else \
	cd $(GETTEXTDIR)  && ./configure --host=$(COMPILE_TOOLCHAIN) --target=$(COMPILE_TOOLCHAIN) --prefix=$(GETTEXTPREFIX) \
	CFLAGS="-I$(ICONVPREFIX)/include " \
	LDFLAGS="-L$(ICONVPREFIX)/lib " ;\
	touch $(GETTEXTDIR)/_is_configure; \
	cd $(GETTEXTDIR)  && make && make install; \
	fi

#--with-dbus-user=root \
	#-disable-tests \
	#--prefix=/usr \
	#--bindir=/usr/bin \
	#--sbindir=/usr/sbin \
	#--libdir=/usr/lib \
	#install
$(DBUS): $(EXPAT)
	cd $(DBUSDIR) && ./configure --help
	if [ -f $(DBUSDIR)/_is_configure ] ; then \
	echo "already configred..... "; \
	else \
	cd $(DBUSDIR) && ./configure --host=$(COMPILE_TOOLCHAIN) --target=$(COMPILE_TOOLCHAIN) \
	--prefix=$(DBUSPREFIX) \
	--enable-shared --enable-static \
	--disable-abstract-sockets \
	--disable-ansi \
	--disable-asserts \
	--disable-console-owner-file \
	--disable-doxygen-docs \
	--disable-compiler_coverage \
	--disable-selinux \
	--disable-tests \
	--enable-verbose-mode \
	--disable-xml-docs \
	--with-xml="expat" \
	--with-dbus-user=telecomadmin \
	--with-dbus-daemondir="/usr/sbin" \
	--with-system-socket="/var/run/dbus/system_bus_socket" \
	--with-system-pid-file="/var/run/dbus.pid" \
	--without-x \
	--libexecdir=/usr/lib/dbus-1 \
	--localstatedir=/var \
	--sysconfdir=/etc \
	CFLAGS="-I$(EXPATPREFIX)/include" \
	LDFLAGS="-L$(EXPATPREFIX)/lib"; \
	touch $(DBUSDIR)/_is_configure; \
	fi
	cd $(DBUSDIR) && make && ! make install
	
	- mkdir -p $(DBUSBIN)/usr/sbin/ 
	- mkdir -p $(DBUSBIN)/usr/bin/ 
	- mkdir -p $(DBUSBIN)/lib/
	- mkdir -p $(DBUSBIN)/usr/lib/dbus-1/
	- mkdir -p $(DBUSBIN)/usr/etc/dbus-1/system.d/
	- mkdir -p $(DBUSBIN)/usr/etc/dbus-1/session.d/
	
	cp  $(DBUSDIR)/bus/dbus-daemon  $(DBUSBIN)/usr/sbin/ 
	cp  $(DBUSDIR)/bus/dbus-daemon-launch-helper  $(DBUSBIN)/usr/lib/dbus-1 
	cp  $(DBUSDIR)/tools/.libs/dbus-uuidgen  $(DBUSBIN)/usr/sbin/  
	cp  $(DBUSDIR)/tools/dbus-launch  $(DBUSBIN)/usr/sbin/ 
	cp  $(DBUSDIR)/tools/.libs/dbus-run-session  $(DBUSBIN)/usr/sbin/
	
	cp  $(DBUSDIR)/tools/dbus-cleanup-sockets  $(DBUSBIN)/usr/bin/ 
	cp  $(DBUSDIR)/tools/.libs/dbus-send  $(DBUSBIN)/usr/bin/
	cp  $(DBUSDIR)/tools/.libs/dbus-monitor  $(DBUSBIN)/usr/bin/
	
	cp  $(DBUSDIR)/dbus/.libs/libdbus-1.so  $(DBUSBIN)/lib/ 
	cp  $(DBUSDIR)/dbus/.libs/libdbus-1.so.*  $(DBUSBIN)/lib/
	cp  $(DBUSDIR)/bus/*.conf  $(DBUSBIN)/usr/etc/dbus-1/
	
	#expat lib
	cp $(EXPATPREFIX)/lib/*.so  $(DBUSBIN)/lib/
	cp $(EXPATPREFIX)/lib/*.so.*  $(DBUSBIN)/lib/
	
#aptitude install libglib2.0-dev
#copy libconfig.h from /lib/glib-2.0/include ###############
#$(GLIB): $(ICONV) $(ZLIB) $(FFI) $(GETTEXT) 
$(GLIB): $(ICONV) $(ZLIB) $(FFI) $(GETTEXT) 
	cd $(GLIBDIR) && ./configure --help=recursive 
	if [ -f $(GLIBDIR)/_is_configure ] ; then \
	echo "already configred..... "; \
	else \
	echo ac_cv_type_long_long=ll>$(GLIBCACHEFILE); \
	echo glib_cv_stack_grows=no>>$(GLIBCACHEFILE); \
	echo glib_cv_uscore=no>>$(GLIBCACHEFILE); \
	echo ac_cv_func_posix_getpwuid_r=yes>>$(GLIBCACHEFILE); \
	echo ac_cv_func_posix_getgrgid_r=yes>>$(GLIBCACHEFILE); \
	cd $(GLIBDIR) && CFLAGS="-I$(ICONVPREFIX)/include" \
	LDFLAGS="-L$(ICONVPREFIX)/lib -lffi -liconv -lintl" \
	ZLIB_CFLAGS="-I$(ZLIBPREFIX)/include" \
	ZLIB_LIBS="-L$(ZLIBPREFIX)/lib -lz" \
	LIBFFI_CFLAGS="-I$(FFIPREFIX)/lib/libffi-3.2.1/include" \
	LIBFFI_LIBS="-L$(FFIPREFIX)/lib -lffi" \
	./configure --host=$(COMPILE_TOOLCHAIN) --target=$(COMPILE_TOOLCHAIN) \
	--prefix=$(GLIBPREFIX) \
	--cache-file=$(GLIBCACHEFILE) \
	--enable-shared \
	--enable-static \
	--enable-debug=yes \
	--disable-selinux \
	--enable-iconv=no --with-libiconv=gnu \
	--disable-fam ; \
	touch $(GLIBDIR)/_is_configure; \
	fi
	cd $(GLIBDIR) && make LDFLAGS="-L$(ICONVPREFIX)/lib -lffi -lintl -liconv" lIBS="-lffi -lintl -liconv" && make install
	#cd $(GLIBDIR) && make LDFLAGS="--rpath=$(ICONVPREFIX)/lib -L$(ICONVPREFIX)/lib -lffi -lintl -liconv" lIBS="-lffi -lintl -liconv" && make install
	#glib
	- mkdir -p $(GLIBBIN)/usr/bin
	- mkdir -p $(GLIBBIN)/usr/lib/glib-2.0
	- mkdir -p $(GLIBBIN)/lib/
	cp  $(GLIBPREFIX)/bin/*  $(GLIBBIN)/usr/bin/ 
	cp  $(GLIBPREFIX)/lib/*.so  $(GLIBBIN)/usr/lib/glib-2.0/
	cp  $(GLIBPREFIX)/lib/*.so.*  $(GLIBBIN)/usr/lib/glib-2.0/
	#libs
	cp  $(GLIB_PRLIB)/lib/*.so  $(GLIBBIN)/lib/
	cp  $(GLIB_PRLIB)/lib/*.so.*  $(GLIBBIN)/lib/

install:
ifneq ($(strip $(DEBUG_LOCAL)),)
	cd $(GLIBBIN)/usr/lib && tar cvf  ~/workdir/tftpdir/glib.tar ./glib-2.0
else
	cp -rf $(GLIBBIN)/* $(FILESYSTEM_DIR)/
	cp -rf $(DBUSBIN)/* $(FILESYSTEM_DIR)/
	cp -rf $(LXCBIN)/*  $(FILESYSTEM_DIR)/
endif
	
$(GDBUS):
	make -C $(ROOTPATH)/gdbus-daemon-1.0 clean all
	make -C $(ROOTPATH)/eventfd_test clean all
	make -C $(ROOTPATH)/gdbus-server-test clean all
	#make -C $(ROOTPATH)/gdbus-client-test clean all

#clean : $(GLIB)-clean $(DBUS)-clean

clean : $(TARGET_CLEAN)

$(LXC)-clean:
	- rm -rf $(LXCBIN)/
	- make -C $(LXCDIR) distclean
	- $(RM) $(LXCDIR)/_is_configure

#$(GLIB)-clean: $(FFI)-clean $(ICONV)-clean $(GETTEXT)-clean $(ZLIB)-clean
$(GLIB)-clean: $(FFI)-clean $(ICONV)-clean $(GETTEXT)-clean $(ZLIB)-clean
	- rm -rf $(GLIBBIN)/
	- make -C $(GLIBDIR) clean distclean
	- $(RM) $(GLIBDIR)/_is_configure

$(ZLIB)-clean:
	- make -C $(ZLIBDIR) clean distclean
	- $(RM) $(ZLIBDIR)/_is_configure

$(GETTEXT)-clean:
	- make -C $(GETTEXTDIR) clean distclean
	- $(RM) $(GETTEXTDIR)/_is_configure

$(FFI)-clean:
	- make -C $(FFIDIR) clean distclean
	- $(RM) $(FFIDIR)/_is_configure

$(ICONV)-clean:
	- make -C $(ICONVDIR) clean distclean
	- $(RM) $(ICONVDIR)/_is_configure

#$(DBUS)-clean: $(EXPAT)-clean
$(DBUS)-clean: $(EXPAT)-clean
	- rm -rf $(DBUSBIN)/
	- make -C $(DBUSDIR) clean distclean
	- $(RM) $(DBUSDIR)/_is_configure

$(LIBCAP)-clean:
	- make -C $(LIBCAPDIR) clean distclean
	- $(RM) $(LIBCAPDIR)/_is_configure

$(EXPAT)-clean:
	- make -C $(EXPATDIR) clean distclean
	- $(RM) $(EXPATDIR)/_is_configure

		
.PHONY : all $(DBUS) clean $(EXPAT)-clean

