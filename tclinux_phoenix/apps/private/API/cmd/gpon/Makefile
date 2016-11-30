##################################
#####Configured directories#######
##################################
INCLUDE			= -I$(KERNEL_DIR)/include \
				  -I$(KERNEL_DIR)/arch/mips/include \
				  -I$(GLOBAL_INC_DIR)/modules \
				  -I$(APP_API_LIB_OUTPUT_DIR) \
				  -I$(APP_API_LIB_GPON_DIR) 
				  
LIBDIR			= -L $(APP_API_LIB_OUTPUT_DIR) -lapi_lib_utility -lapi_lib_gponmgr -lecnt_lib_dbg -lpthread
USRBINDIR		= $(FILESYSTEM_DIR)/userfs/bin

##################################
########Configured flags##########
##################################
RM				= /bin/rm -f
GPONMGR_CFLAGS	= -O2 -Wall $(INCLUDE) $(TC_CFLAGS)
GPONMGR_CFLAGS	+= -D API_CMD_GPONMGR_DBG=0

##################################
########Configured output#########
##################################
OBJS	=	gponmgr.o gponmgr_get.o gponmgr_set.o
TARGETS	=	gponmgr

all: $(TARGETS) install

.SUFFIXES:  .c .o
.c.o:
	$(CC) $(GPONMGR_CFLAGS) -c $<
	
$(TARGETS): clean $(OBJS)
	$(CC) $(GPONMGR_CFLAGS) -o $(TARGETS) $(OBJS) $(LIBDIR)
	$(STRIP) --strip-unneeded $(TARGETS)

install:
	install $(TARGETS) $(USRBINDIR)
	
clean:
	$(RM) $(OBJS) $(TARGETS)
	$(RM) $(USRBINDIR)/$(TARGETS)

