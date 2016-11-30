##################################
#####Configured directories#######
##################################
INCLUDE			= -I$(KERNEL_DIR)/include \
				  -I$(KERNEL_DIR)/arch/mips/include \
				  -I$(APP_API_LIB_OUTPUT_DIR)
				  
LIBDIR			= -L $(APP_API_LIB_OUTPUT_DIR) -lapi_lib_utility -lapi_igmpmgr
USRBINDIR		= $(FILESYSTEM_DIR)/userfs/bin

##################################
########Configured flags##########
##################################
RM				= /bin/rm -f
ECNT_IGMP_CFLAGS = -O2 -Wall $(INCLUDE) $(TC_CFLAGS) 

##################################
########Configured output#########
##################################
OBJS	=	ecnt_igmp_cmd.o
TARGETS	=	ecnt_igmp_cmd

all: $(TARGETS) install

.SUFFIXES:  .c .o
.c.o:
	$(CC) $(ECNT_IGMP_CFLAGS) -c $<
	
$(TARGETS): clean $(OBJS)
	$(CC) $(ECNT_IGMP_CFLAGS) -o $(TARGETS) $(OBJS) $(LIBDIR)
	$(STRIP) --strip-unneeded $(TARGETS)

install:
	install $(TARGETS) $(USRBINDIR)
	
clean:
	$(RM) $(OBJS) $(TARGETS)
	$(RM) $(USRBINDIR)/$(TARGETS)

