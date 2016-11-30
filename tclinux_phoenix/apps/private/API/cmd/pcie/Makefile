##################################
#####Configured directories#######
##################################
INCLUDE			= -I$(KERNEL_DIR)/include \
				  -I$(KERNEL_DIR)/arch/mips/include \
				  -I$(APP_API_LIB_OUTPUT_DIR)
				  
LIBDIR			= -L $(APP_API_LIB_OUTPUT_DIR) -lapi_lib_utility -lapi_lib_pcie
USRBINDIR		= $(FILESYSTEM_DIR)/userfs/bin

##################################
########Configured flags##########
##################################
RM				= /bin/rm -f
PCIE_CFLAGS	= -O2 -Wall $(INCLUDE) $(TC_CFLAGS) 


##################################
########Configured output#########
##################################
OBJS	=	pcie.o
TARGETS	=	pcie

all: $(TARGETS) install

.SUFFIXES:  .c .o
.c.o:
	$(CC) $(PCIE_CFLAGS) -c $<
	
$(TARGETS): clean $(OBJS)
	$(CC) $(PPE_CFLAGS) -o $(TARGETS) $(OBJS) $(LIBDIR)
	$(STRIP) --strip-unneeded $(TARGETS)

install:
	install $(TARGETS) $(USRBINDIR)
	
clean:
	$(RM) $(OBJS) $(TARGETS)
	$(RM) $(USRBINDIR)/$(TARGETS)

