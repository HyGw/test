#
#Configured directories
#
#
#Install commands
#
INSTALL=	/usr/bin/install -c
INSTALL_DIR	=	$(INSTALL) -d
INSTALL_LIB	=	$(LIBTOOL)	$(INSTALL) -m 755
LN = /bin/ln -s
RM = /bin/rm -f


#CROSS=mips-linux-

#CC=$(CROSS)gcc
AR=$(CROSS)ar
LD=$(CROSS)ld
STRIP=$(CROSS)strip
RANLIB=$(CROSS)ranlib
SIZE=$(CROSS)size
LIBDIR=$(CROSS_PATH)/lib

ifeq ($(strip $(TCSUPPORT_3_18_21_KERNEL)),)
KERNEL_DIR=../../linux

INCLUDE = -I$(KERNEL_DIR)/arch/mips/include -I$(KERNEL_DIR)/include
endif

#CFLAGS= -mips1 -msoft-float -DETHCMD_DBG -DETHCMD_EXTENSION
#CFLAGS= -mips1 -msoft-float 
#CFLAGS += -DSQA_VERIFY
### [MT7530_UPW_REG_UPDATE]	: 20121011 YM_Chan request for MT7530 E2 h/w register spec modification
#CFLAGS += -DMT7530_UPW_REG_UPDATE
ifeq ($(TCSUPPORT_ETH4_WAN_PORT),)
  	CFLAGS += -DVPORT
endif

ifneq ($(TC_CFLAGS), )
        CFLAGS+=$(TC_CFLAGS)
endif

ifneq ($(strip $(TCSUPPORT_CODE_SHRINK)),)
CFLAGS += -Os
endif

OBJS= ethcmd.o libcmd.o ethercmd.o libmii.o ether.o
EXECS= ethcmd

ifneq ($(strip $(TCSUPPORT_MT7530_SWITCH_API)),)
#OBJS += mtkswitch_api.o
LIBSRCS = mtkswitch_api.c
LIBMTKSWITCH = libmtkswitch
LIBOBJS = $(LIBMTKSWITCH).o
LIBSO = $(LIBMTKSWITCH).so.1.0
CFLAGS_LIB = -fPIC -Os -g -Wall
DSOFLAGS=	-Wl,-soname,$(LIBSO) -shared
#SWAPI_LIBKRLDIR=$(TRUNK_DIR)/modules/private/tcphy
SWAPI_LIBKRLDIR=../../../modules/private/tcphy
CFLAGS_LINK_SO = -L. -lmtkswitch
else
LIBOBJS =
LIBSO =
CFLAGS_LINK_SO =
endif

all: $(LIBSO) ethcmd

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDE) $(CFLAGS_LINK_SO) $<

ethcmd: $(OBJS) 
	$(CC) $(OBJS) $(INCLUDE) $(CFLAGS_LINK_SO) -o $@
	$(STRIP) --strip-unneeded $(EXECS)

#static library
mtkswitch_api.a: mtkswitch_api.o
	$(AR) cr $@ $^

#dynamically linked shared library
$(LIBMTKSWITCH).so.1.0 : $(LIBSRCS)
	$(CC) $(CFLAGS) $(CFLAGS_LIB) -c $< -o $(LIBOBJS)
	$(CC) $(CFLAGS) $(CFLAGS_LIB) $(DSOFLAGS) -o $@ $(LIBOBJS)
	$(STRIP) --strip-unneeded $@
	$(RM) $(LIBMTKSWITCH).so $(LIBMTKSWITCH).so.1
	$(LN) $(LIBMTKSWITCH).so.1.0 $(LIBMTKSWITCH).so
	$(LN) $(LIBMTKSWITCH).so.1.0 $(LIBMTKSWITCH).so.1
##	cp $@ $(LIB_DIR)
##	cp $@ $(FILESYSTEM_DIR)/lib
#	cp $(LIBMTKSWITCH).so $(FILESYSTEM_DIR)/lib/
#	cp $(LIBMTKSWITCH).so.* $(FILESYSTEM_DIR)/lib/
#	cp $(LIBMTKSWITCH).so $(LIB_DIR)/
#	cp $(LIBMTKSWITCH).so.* $(LIB_DIR)/
#	cp $(LIBMTKSWITCH).so $(PUBLIC_LIB_DIR)/
#	cp $(LIBMTKSWITCH).so.* $(PUBLIC_LIB_DIR)/ 
#	cp $(SWAPI_LIBKRLDIR)/mtkswitch_api.h $(LIB_DIR)
#	cp $(SWAPI_LIBKRLDIR)/mtkswitch_api.h $(PUBLIC_LIB_DIR)


install:
#	cp $@ $(LIB_DIR)
#	cp $@ $(FILESYSTEM_DIR)/lib
	-cp $(LIBMTKSWITCH).so $(FILESYSTEM_DIR)/lib/
	-cp $(LIBMTKSWITCH).so.* $(FILESYSTEM_DIR)/lib/
	-cp $(LIBMTKSWITCH).so $(LIB_DIR)/
	-cp $(LIBMTKSWITCH).so.* $(LIB_DIR)/
	-cp $(LIBMTKSWITCH).so $(PUBLIC_LIB_DIR)/
	-cp $(LIBMTKSWITCH).so.* $(PUBLIC_LIB_DIR)/ 
	-cp $(SWAPI_LIBKRLDIR)/mtkswitch_api.h $(LIB_DIR)
	-cp $(SWAPI_LIBKRLDIR)/mtkswitch_api.h $(PUBLIC_LIB_DIR)
#	cp $@ $(LIB_DIR)
#	cp $@ $(FILESYSTEM_DIR)/lib
##	$(INSTALL_DIR) $(LIB_DIR)
##	$(INSTALL_LIB) $(LIBMTKSWITCH).so.1.0 $(LIB_DIR)
##	$(RM) $(LIB_DIR)$(LIBMTKSWITCH).so.so
##	$(LN) $(LIBMTKSWITCH).so.1.0 $(LIB_DIR)$(LIBMTKSWITCH).so
##	$(RM) $(LIB_DIR)$(LIBMTKSWITCH).so.1
##	$(LN) $(LIBMTKSWITCH).so.1.0 $(LIB_DIR)$(LIBMTKSWITCH).so.1
#	cp $(SWAPI_LIBKRLDIR)/mtkswitch_api.h $(LIB_DIR)
#	cp $(SWAPI_LIBKRLDIR)/mtkswitch_api.h $(PUBLIC_LIB_DIR)

clean:
	rm -f *.o *.a $(EXECS)
ifneq ($(strip $(TCSUPPORT_MT7530_SWITCH_API)),)
	rm -f $(LIBMTKSWITCH).so
	rm -f $(LIBMTKSWITCH).so.1
	rm -f $(LIBMTKSWITCH).so.1.0
	rm -f $(LIBOBJS)
endif

ifneq ($(strip $(TCSUPPORT_MT7530_SWITCH_API)),)
lib_clean:
	rm -f $(LIBMTKSWITCH).so
	rm -f $(LIBMTKSWITCH).so.1
	rm -f $(LIBMTKSWITCH).so.1.0
	rm -f $(LIBOBJS)
endif
  
