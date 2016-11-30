ROOTPATH=$(shell pwd)

MTD=mtd
MTDDIR=$(ROOTPATH)/mtd-utils-1.4.5
MTDPREFIX=$(ROOTPATH)/install_mtd
UBIBINDIR=$(MTDDIR)/$(COMPILE_TOOLCHAIN)/ubi-utils

ZLIB = zlib
ZLIBDIR = $(ROOTPATH)/zlib-1.2.8
ZLIBREFIX = $(ROOTPATH)/install_zlib

LZO = lzo
LZODIR = $(ROOTPATH)/lzo-2.09
LZOREFIX = $(ROOTPATH)/install_lzo

E2FS = e2fsprogs
E2FSDIR = $(ROOTPATH)/e2fsprogs-1.42.13
E2FSREFIX = $(ROOTPATH)/install_e2fs

all: $(MTD)

$(ZLIB):
	echo "test"
	#cd $(ZLIBDIR) && ./configure --help
	if [ -f $(ZLIBDIR)/_is_configure ] ; then \
	echo "already configred..... "; \
	else \
	cd $(ZLIBDIR) && ./configure  --prefix=$(ZLIBREFIX) && make && make install; \
	touch $(ZLIBDIR)/_is_configure; \
	fi

$(E2FS):
	if [ -f $(E2FSDIR)/_is_configure ] ; then \
	echo "already configred..... "; \
	cd $(E2FSDIR) && make && make install-libs ; \
	else \
	cd $(E2FSDIR) && ./configure  --host=$(COMPILE_TOOLCHAIN) --target=$(COMPILE_TOOLCHAIN) --prefix=$(E2FSREFIX) && make && make install-libs ; \
	touch $(E2FSDIR)/_is_configure; \
	fi

$(LZO):
	if [ -f $(LZODIR)/_is_configure ] ; then \
	echo "already configred..... "; \
	else \
	cd $(LZODIR) && ./configure  --host=$(COMPILE_TOOLCHAIN) --target=$(COMPILE_TOOLCHAIN) --prefix=$(LZOREFIX) && make && make install; \
	touch $(LZODIR)/_is_configure; \
	fi

$(MTD): $(ZLIB) $(LZO) $(E2FS)
	#cd $(MTDDIR) && make clean && make WITHOUT_LZO=1 WITHOUT_XATTR=1
	cd $(MTDDIR) && make WITHOUT_XATTR=1 ZLIBCPPFLAGS=-I$(ZLIBREFIX)/include LZOCPPFLAGS="-I$(LZOREFIX)/include -I$(E2FSREFIX)/include" \
	LZOLDFLAGS="-L$(LZOREFIX)/lib -llzo2 -L$(ZLIBREFIX)/lib -lz -L$(E2FSDIR)/lib -luuid -lblkid -le2p -lext2fs"

install:
	cp $(UBIBINDIR)/ubiattach $(FILESYSTEM_DIR)/userfs/bin/
	cp $(UBIBINDIR)/ubiformat $(FILESYSTEM_DIR)/userfs/bin/
	cp $(UBIBINDIR)/ubimkvol $(FILESYSTEM_DIR)/userfs/bin/
	cp $(UBIBINDIR)/ubidetach $(FILESYSTEM_DIR)/userfs/bin/
	cp $(UBIBINDIR)/mtdinfo $(FILESYSTEM_DIR)/userfs/bin/
	cp $(UBIBINDIR)/ubinfo $(FILESYSTEM_DIR)/userfs/bin/
	
	#cp -f $(MTDDIR)/mkfs.jffs2 $(ROOTPATH)/../
clean : $(MTD)-clean all-clean

$(MTD)-clean:
	make -C $(MTDDIR) clean
	rm -rf  $(FILESYSTEM_DIR)/userfs/bin/ubiattach
	rm -rf  $(FILESYSTEM_DIR)/userfs/bin/ubiformat
	rm -rf  $(FILESYSTEM_DIR)/userfs/bin/ubimkvol
	rm -rf  $(FILESYSTEM_DIR)/userfs/bin/ubidetach
	rm -rf  $(FILESYSTEM_DIR)/userfs/bin/mtdinfo
	rm -rf  $(FILESYSTEM_DIR)/userfs/bin/ubinfo

all-clean:
	make -C $(ZLIBDIR) clean
	-rm -rf $(ZLIBREFIX)
	- $(RM) $(ZLIBDIR)/_is_configure 
	
	make -C $(LZODIR) clean
	-rm -rf $(LZOREFIX)
	- $(RM) $(LZODIR)/_is_configure 
	make -C $(E2FSDIR) clean
	-rm -rf $(E2FSREFIX)
	- $(RM) $(E2FSDIR)/_is_configure 
	
	make -C $(MTDDIR) clean

distclean:
	-rm $(LZODIR)
		
.PHONY : all $(MTD) $(MTD)-clean
