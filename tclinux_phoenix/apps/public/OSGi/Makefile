ROOTPATH=$(shell pwd)

NEW_VERSION=YES

ANTLRDIR = $(ROOTPATH)/antlr

JAMVM = jamvm

ifneq ($(strip $(TCSUPPORT_NEWTOOLCHAIN)),)
JAMVMPREFIX = $(ROOTPATH)/jamvm_install_newtoolchain
else
JAMVMPREFIX = $(ROOTPATH)/jamvm_install
endif		

ZLIB = zlib
ZLIBDIR = $(ROOTPATH)/zlib-1.2.8
ifneq ($(strip $(TCSUPPORT_NEWTOOLCHAIN)),)
ZLIBREFIX = $(ROOTPATH)/zlib_install_newtoolchain
else
ZLIBREFIX = $(ROOTPATH)/zlib_install
endif

CLASSPATH = classpath
CLASSPATHLIB = /usr/local/ct
ifneq ($(strip $(TCSUPPORT_NEWTOOLCHAIN)),)
CLASSPATHPREFIX = $(ROOTPATH)/classpath_install_newtoolchain
else
CLASSPATHPREFIX = $(ROOTPATH)/classpath_install
endif

ifeq ($(strip $(NEW_VERSION)),YES)
JAMVMDIR = $(ROOTPATH)/jamvm-2.0.0
CLASSPATHDIR = $(ROOTPATH)/classpath-new
else
JAMVMDIR = $(ROOTPATH)/jamvm-2.0.0
CLASSPATHDIR = $(ROOTPATH)/classpath-0.98
endif

DDNS=ddns
DDNSDIR=$(ROOTPATH)/phddns-2.0.5.19225
DDNSPREFIX=$(ROOTPATH)/ddns_install

MTD-UTILS = mtd-utils
MTD-UTILSDIR = $(ROOTPATH)/mtd-utils-1.4.5
MTD-UTILSREFIX = $(ROOTPATH)/mtd-utils_install

INSTALLDIR =_install/usr/local

CTGWLIBROOT = $(ROOTPATH)/../../private

RM = rm -rf
MV = mv -f
CP = cp -fpR

all : $(CLASSPATH) $(JAMVM) $(DDNS) install
#$(ZLIB)  $(CLASSPATH)
	
#cd $(JAMVMDIR) && ./configure --build=i486-linux-gnu --host=mips-linux --target=mips-linux --program-prefix="" --program-suffix="" --prefix=$(JAMVMPREFIX) --exec-prefix=$(JAMVMPREFIX) --sbindir=/usr/sbin --libexecdir=/usr/lib --sysconfdir=/etc --localstatedir=/var --mandir=/usr/man --infodir=/usr/info --with-java-runtime-library=openjdk8 --disable-tls --disable-int-inlining --disable-shared --without-pic CFLAGS="-g -O2 -Wall -mips32r2 -muclibc"; \
#cd $(JAMVMDIR) && ./configure --enable-tls=no CPPFLAGS=-I"$(ZLIBREFIX)/include" LDFLAGS=-L"$(ZLIBREFIX)/lib" --prefix=$(JAMVMPREFIX) --with-java-runtime-library=gnuclasspath --with-classpath-install-dir=/usr --host=mips-linux; \
#cd $(JAMVMDIR) && make GLIBJ_ZIP=$(CLASSPATHPREFIX)/share/classpath/glibj.zip && make install
	#--disable-tls --includedir=$(ROOTPATH)/jamvm/include --libdir=$(ROOTPATH)/jamvm/lib \
	#--with-classpath-install-dir=$(CLASSPATHLIB)  \
	 #./configure --target=mipsel-openwrt-linux --host=mipsel-openwrt-linux --build=i486-linux-gnu --program-prefix="" --program-suffix="" --prefix=/usr --exec-prefix=/usr --bindir=/usr/bin --sbindir=/usr/sbin --libexecdir=/usr/lib --sysconfdir=/etc --datadir=/usr/share --localstatedir=/var --mandir=/usr/man --infodir=/usr/info --disable-nls   --with-java-runtime-library=gnuclasspath --with-classpath-install-dir=/usr --disable-int-inlining --disable-shared --without-pic

#cd $(JAMVMDIR) && ./configure --build=i486-linux-gnu --host=mips-linux --target=mips-linux --program-prefix="" --program-suffix="" --prefix=$(JAMVMPREFIX) --exec-prefix=$(JAMVMPREFIX) --sbindir=/usr/sbin --libexecdir=/usr/lib --sysconfdir=/etc --localstatedir=/var --mandir=/usr/man --infodir=/usr/info --with-java-runtime-library=gnuclasspath --with-classpath-install-dir=/usr --disable-tls --disable-int-inlining --disable-shared --without-pic CFLAGS="-g -O2 -Wall -mips32r2 -muclibc"; \

$(JAMVM): $(ZLIB)
	cd $(JAMVMDIR) && ./configure --help 
	if [ -f $(JAMVMDIR)/_is_configure ] ; then \
	echo "already configred..... "; \
	else \
	cd $(JAMVMDIR) && ./configure --enable-tls=no CPPFLAGS=-I"$(ZLIBREFIX)/include" LDFLAGS=-L"$(ZLIBREFIX)/lib" --prefix=$(JAMVMPREFIX) --with-java-runtime-library=gnuclasspath --with-classpath-install-dir=/usr/osgi --host=mips-linux; \
	touch $(JAMVMDIR)/_is_configure; \
	fi
	cd $(JAMVMDIR) && make && make install
	mips-linux-uclibc-strip $(JAMVMPREFIX)/bin/jamvm
	$(CP) $(JAMVMPREFIX)/bin/jamvm /home/test/workdir/tftpdir
	mkdir -p ./_install/usr/bin ./_install/usr/share/jamvm ./_install/usr/lib
	$(CP) $(JAMVMPREFIX)/bin/jamvm ./_install/usr/bin
	$(CP) $(JAMVMPREFIX)/share/jamvm/classes.zip ./_install/usr/share/jamvm
	ln -fs ./_install/usr/share/classpath/glibj.zip ./_install/usr/lib/rt.jar
	
$(ZLIB):
	echo "test"
	#cd $(ZLIBDIR) && ./configure --help
	if [ -f $(ZLIBDIR)/_is_configure ] ; then \
	echo "already configred..... "; \
	else \
	cd $(ZLIBDIR) && ./configure --prefix=$(ZLIBREFIX) && make && make install; \
	touch $(ZLIBDIR)/_is_configure; \
	fi
	#$(CP) $(ZLIBREFIX)/lib/*.so* ./_install/usr/lib

$(DDNS):
	echo "ddns"
	cd $(DDNSDIR) && ./configure --help
	if [ -f $(DDNSDIR)/_is_configure ] ; then \
	echo "already configred..... "; \
	else \
	cd $(DDNSDIR) && ./configure --prefix=$(DDNSPREFIX) && make && make install; \
	touch $(DDNSDIR)/_is_configure; \
	fi

# aclocal aptitude install automake !!!!!!!!!!!!!!!!!!! 
# aptitude install gettext
# aptitude install libtool

#cd $(CLASSPATHDIR) && ./configure --target=mips-linux-uclibc --host=mips-linux-uclibc --build=i486-linux-gnu --prefix=$(CLASSPATHPREFIX) --disable-nls --without-x --disable-gtk-peer --disable-qt-peer --disable-dssi --disable-plugin --disable-gconf-peer --disable-gjdoc --with-antlr-jar=$(ANTLRDIR)/antlr-3.4-complete.jar --disable-alsa CC="mips-linux-uclibc-gcc -g -O2 -Wall -mips32r2 -muclibc"; \
	#cd $(CLASSPATHDIR) && ./autogen.sh && ./configure --target=mips-linux-uclibc --host=mips-linux-uclibc --build=i486-linux-gnu --prefix=$(CLASSPATHPREFIX) --disable-nls --without-x --disable-gtk-peer --disable-qt-peer --disable-dssi --disable-plugin --disable-gconf-peer --disable-gjdoc --with-antlr-jar=$(ANTLRDIR)/antlr-3.4-complete.jar --disable-alsa CC="mips-linux-uclibc-gcc -g -O2 -Wall -mips32r2 -muclibc"; \

# --disable-examples
	#CFLAGS="-g -O2 -Wall -mips32r2 -muclibc" --enable-jni --build=i686-linux --host=mips-linux-uclibc --target=mips-linux-uclibc 
	#./configure --target=mipsel-openwrt-linux --host=mipsel-openwrt-linux --build=i486-linux-gnu --program-prefix="" --program-suffix="" --prefix=/usr --exec-prefix=/usr --bindir=/usr/bin --sbindir=/usr/sbin --libexecdir=/usr/lib --sysconfdir=/etc --datadir=/usr/share --localstatedir=/var --mandir=/usr/man --infodir=/usr/info --disable-nls   --without-x --disable-gtk-peer --disable-qt-peer --disable-dssi --disable-plugin --disable-gconf-peer --disable-gjdoc --with-antlr-jar=/home/test/osgi-0526/openwrt/barrier_breaker/dl/antlr-3.4-complete.jar
$(CLASSPATH):
	#cd $(CLASSPATHDIR) && ./configure --help
ifneq ($(strip $(TCSUPPORT_NEWTOOLCHAIN)),)
	if [ -f $(CLASSPATHDIR)/_is_configure ] ; then \
	echo "already configred..... "; \
	else \
	cd $(CLASSPATHDIR) && ./autogen.sh &&./configure --host=mips-linux --target=mips-unknown-linux-uclibc --prefix=$(CLASSPATHPREFIX) --disable-gtk-peer --disable-gconf-peer --disable-plugin ; \
	touch $(CLASSPATHDIR)/_is_configure; \
	fi
else
	if [ -f $(CLASSPATHDIR)/_is_configure ] ; then \
	echo "already configred..... "; \
	else \
	cd $(CLASSPATHDIR) && ./autogen.sh && ./configure --host=mips-linux --prefix=$(CLASSPATHPREFIX) --disable-gtk-peer --disable-gconf-peer --disable-plugin ; \
	touch $(CLASSPATHDIR)/_is_configure; \
	fi
endif
	cd $(CLASSPATHDIR) && make && make install
	mkdir -p ./_install/usr/share/classpath/ ./_install/usr/lib/classpath/
	$(CP) $(CLASSPATHPREFIX)/lib/security $(CLASSPATHPREFIX)/lib/logging.properties ./_install/usr/lib/
	$(CP) $(CLASSPATHPREFIX)/lib/classpath/*.so* ./_install/usr/lib/classpath/
	$(CP) $(CLASSPATHPREFIX)/share/classpath/glibj.zip ./_install/usr/share/classpath

install:
	$(RM) ./_install_jffs2/*
	mkdir -p ./_install/usr/bin ./_install/usr/share/jamvm ./_install/usr/lib
	$(CP) $(JAMVMPREFIX)/bin/jamvm ./_install/usr/bin
	$(CP) $(JAMVMPREFIX)/share/jamvm/classes.zip ./_install/usr/share/jamvm
	ln -fs ./_install/usr/share/classpath/glibj.zip ./_install/usr/lib/rt.jar
	#zlib
	$(CP) $(ZLIBREFIX)/lib/*.so* ./_install/usr/lib
	#classpath
	mkdir -p ./_install/usr/share/classpath/ ./_install/usr/lib/classpath/
	$(CP) $(CLASSPATHPREFIX)/lib/security $(CLASSPATHPREFIX)/lib/logging.properties ./_install/usr/lib/
	$(CP) $(CLASSPATHPREFIX)/lib/classpath/*.so* ./_install/usr/lib/classpath/
	$(CP) $(CLASSPATHPREFIX)/share/classpath/glibj.zip ./_install/usr/share/classpath
	mkdir -p ./_install_jffs2/jvm0
	$(CP) ./_install/usr/* ./_install_jffs2/jvm0
	cd ./_install_jffs2 && ln -fs ./jvm0/bin/ ./bin && ln -fs ./jvm0/lib/ ./lib && ln -fs ./jvm0/share/ ./share
	mkdir -p ./_install_jffs2/factory ./_install_jffs2/backup ./_install_jffs2/plugin-b ./_install_jffs2/plugin-c
	$(CP) ./_install_other/config ./_install_jffs2/
	chmod 777 ./plugin_install.sh ./plugin_start.sh ./plugin_del.sh
	$(CP) ./plugin_install.sh ./_install_jffs2/
	$(CP) ./plugin_del.sh ./_install_jffs2/
	$(CP) ./plugin_start.sh ./_install_jffs2/
	#fakeroot mkfs.jffs2  -s 0x800 -e 0x20000 -p 0x280000 -d ./_install_jffs2/ -o osgi.img -b -n
	#framework
	cd $(ROOTPATH)
	mkdir -p ./_install_jffs2/framework0
	rm -rf ./_install_jffs2/framework0/* ./_install_jffs2/felix-framework
	$(CP) ./felix-framework-5.0.0/* ./_install_jffs2/framework0
	cd ./_install_jffs2 && ln -fs ./framework0/ ./felix-framework
	# ctgw libs
	cd $(ROOTPATH)
	mkdir -p ./_install_jffs2/ctgwlibs0
	rm -rf ./_install_jffs2/ctgwlibs0/* ./_install_jffs2/ctgwlib
	cp $(CTGWLIBROOT)/joyme_lib/libjoyme.so.1.4 ./_install_jffs2/ctgwlibs0
	cp $(CTGWLIBROOT)/ctsgw_lib/libctsgw.so.1.4 ./_install_jffs2/ctgwlibs0
	cd ./_install_jffs2 && ln -fs ./ctgwlibs0 ./ctgwlib
	# mobile manager
	cd $(ROOTPATH)
	mkdir -p ./_install_jffs2/mobileapps0
	rm -rf ./_install_jffs2/mobileapps0/* ./_install_jffs2/mobile
	cp $(CTGWLIBROOT)/mobile-manager/mobile-manager ./_install_jffs2/mobileapps0
	cd ./_install_jffs2 && ln -fs ./mobileapps0 ./mobile
	# create default setting for api right1
	cd $(ROOTPATH)
	mkdir -p ./_install_jffs2/def_api_right1
	$(RM) ./_install_yaffs2/
	$(CP) ./_install_jffs2/ ./_install_yaffs2/

clean : $(JAMVM)-clean $(CLASSPATH)-clean

$(JAMVM)-clean:
	make -C $(JAMVMDIR) clean
	#- $(RM) $(JAMVMPREFIX)/*
	- $(RM) $(JAMVMDIR)/_is_configure 

$(CLASSPATH)-clean:
	make -C $(CLASSPATHDIR) clean
	#- $(RM) $(CLASSPATHPREFIX)/*
	- $(RM) $(CLASSPATHDIR)/_is_configure
		
$(DDNS)-clean:
	- make -C $(DDNSDIR) clean distclean
	- $(RM) $(DDNSDIR)/_is_configure
		
.PHONY : all $(CLASSPATH) $(JAMVM) clean $(JAMVM)-clean $(CLASSPATH)-clean
