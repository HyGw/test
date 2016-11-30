##################################
#####Configured directories#######
##################################
INCLUDE			= -I$(APP_API_LIB_OUTPUT_DIR)
				  
LIBDIR			= -L $(APP_API_LIB_OUTPUT_DIR) -lapi_lib_switchmgr
USRBINDIR		= $(FILESYSTEM_DIR)/userfs/bin

##################################
########Configured flags##########
##################################
RM				= /bin/rm -f
SWITCH_CFLAGS	= -O2 -Wall $(INCLUDE) $(TC_CFLAGS) -DBIG_ENDIAN
ifneq ($(COMPILEOPTION_CFLAGS), )
SWITCH_CFLAGS+=$(COMPILEOPTION_CFLAGS)
endif


##################################
########Configured output#########
##################################
OBJS	=	switchmgr.o
TARGETS	=	switchmgr

all: $(TARGETS) install

.SUFFIXES:  .c .o
.c.o:
	$(CC) $(SWITCH_CFLAGS) -c $<
	
$(TARGETS): clean $(OBJS)
	$(CC) $(SWITCH_CFLAGS) -o $(TARGETS) $(OBJS) $(LIBDIR) $(COMPILEOPTION_LDFLAGS)
	$(STRIP) --strip-unneeded $(TARGETS)

install:
	install $(TARGETS) $(USRBINDIR)
	
clean:
	$(RM) $(OBJS) $(TARGETS)
	$(RM) $(USRBINDIR)/$(TARGETS)

