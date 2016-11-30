##################################
#####Configured directories#######
##################################
INCLUDE			= -I$(KERNEL_DIR)/include \
				  -I$(KERNEL_DIR)/arch/mips/include \
				  -I$(APP_API_LIB_OUTPUT_DIR)
				  
LIBDIR			= -L $(APP_API_LIB_OUTPUT_DIR) -lapi_lib_brtable -lecnt_lib_dbg
USRBINDIR		= $(FILESYSTEM_DIR)/userfs/bin

##################################
########Configured flags##########
##################################
RM				= /bin/rm -f
BRTABLE_CFLAGS	= -O2 -Wall $(INCLUDE) $(TC_CFLAGS) 
ifneq ($(COMPILEOPTION_CFLAGS), )
BRTABLE_CFLAGS+=$(COMPILEOPTION_CFLAGS)
endif

##################################
########Configured output#########
##################################
OBJS	=	bridge_cmd.o
TARGETS	=	brtable

all: $(TARGETS) install

.SUFFIXES:  .c .o
.c.o:
	$(CC) $(BRTABLE_CFLAGS) -c $<
	
$(TARGETS): clean $(OBJS)
	$(CC) $(BRTABLE_CFLAGS) -o $(TARGETS) $(OBJS) $(LIBDIR) $(COMPILEOPTION_LDFLAGS)
	$(STRIP) --strip-unneeded $(TARGETS)

install:
	install $(TARGETS) $(USRBINDIR)
	
clean:
	$(RM) $(OBJS) $(TARGETS)
	$(RM) $(USRBINDIR)/$(TARGETS)
