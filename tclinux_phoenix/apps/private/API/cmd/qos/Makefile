##################################
#####Configured directories#######
##################################
INCLUDE			= -I$(KERNEL_DIR)/include \
				  -I$(KERNEL_DIR)/arch/mips/include \
				  -I$(APP_API_LIB_OUTPUT_DIR)
				  
LIBDIR			= -L $(APP_API_LIB_OUTPUT_DIR) -lapi_lib_flowmgr -lecnt_lib_dbg
USRBINDIR		= $(FILESYSTEM_DIR)/userfs/bin

##################################
########Configured flags##########
##################################
RM				= /bin/rm -f
QOS_CFLAGS	= -O2 -Wall $(INCLUDE) $(TC_CFLAGS) 
ifneq ($(COMPILEOPTION_CFLAGS), )
QOS_CFLAGS+=$(COMPILEOPTION_CFLAGS)
endif

##################################
########Configured output#########
##################################
OBJS	=	api_flow_cmd.o
TARGETS	=	acl

all: $(TARGETS) install

.SUFFIXES:  .c .o
.c.o:
	$(CC) $(QOS_CFLAGS) -c $<
	
$(TARGETS): clean $(OBJS)
	$(CC) $(QOS_CFLAGS) -o $(TARGETS) $(OBJS) $(LIBDIR) $(COMPILEOPTION_LDFLAGS)
	$(STRIP) --strip-unneeded $(TARGETS)

install:
	install $(TARGETS) $(USRBINDIR)
	
clean:
	$(RM) $(OBJS) $(TARGETS)
	$(RM) $(USRBINDIR)/$(TARGETS)
