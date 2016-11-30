##################################
#####Configured directories#######
##################################
INCLUDE			= -I$(KERNEL_DIR)/include \
				  -I$(KERNEL_DIR)/arch/mips/include \
				  -I$(APP_API_LIB_OUTPUT_DIR)
				  
LIBDIR			= -L $(APP_API_LIB_OUTPUT_DIR) -lapi_lib_utility -lapi_lib_ppe
USRBINDIR		= $(FILESYSTEM_DIR)/userfs/bin

##################################
########Configured flags##########
##################################
RM				= /bin/rm -f
PPE_CFLAGS	= -O2 -Wall $(INCLUDE) $(TC_CFLAGS) -DCONFIG_HNAT_V2 -DBIG_ENDIAN
ifneq ($(COMPILEOPTION_CFLAGS), )
PPE_CFLAGS+=$(COMPILEOPTION_CFLAGS)
endif

##################################
########Configured output#########
##################################
OBJS	=	ppe_main.o
TARGETS	=	ppe

all: $(TARGETS) install

.SUFFIXES:  .c .o
.c.o:
	$(CC) $(PPE_CFLAGS) -c $<
	
$(TARGETS): clean $(OBJS)
	$(CC) $(PPE_CFLAGS) -o $(TARGETS) $(OBJS) $(LIBDIR) $(COMPILEOPTION_LDFLAGS)
	$(STRIP) --strip-unneeded $(TARGETS)

install:
	install $(TARGETS) $(USRBINDIR)
	
clean:
	$(RM) $(OBJS) $(TARGETS)
	$(RM) $(USRBINDIR)/$(TARGETS)

