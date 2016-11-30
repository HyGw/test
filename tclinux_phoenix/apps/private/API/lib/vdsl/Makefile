##################################
#####Configured directories#######
##################################
INCLUDE			= -I$(MODULES_VDSL_WAN_DIR)

##################################
########Configured output#########
##################################
OBJS		=	api_lib_vdslmgr.o
HEADER		=	libapi_lib_vdslmgr.h
LINK_NAME	=	libapi_lib_vdslmgr.so
SONAME		=	$(LINK_NAME).1
REAL_NAME	=	$(SONAME).0.0

##################################
########Configured flags##########
##################################
RM				= /bin/rm -f
LN      		= /bin/ln -s
CFLAGS 			+=-fPIC -Wall $(INCLUDE) $(TC_CFLAGS)
#CFLAGS			+= -D API_LIB_VDSLMGR_DBG
API_LIB_CFLAGS	= -shared -Wl,-soname,$(SONAME)

all: $(REAL_NAME) install

.SUFFIXES:  .c .o
.c.o:
	$(CC) $(CFLAGS) -c $<
	
$(REAL_NAME): clean $(OBJS)
	$(CC) $(API_LIB_CFLAGS) -o $(REAL_NAME) $(OBJS)
	$(STRIP) --strip-unneeded $(REAL_NAME)

install:
	install $(REAL_NAME) $(APP_API_LIB_OUTPUT_DIR)
	$(LN) $(REAL_NAME) $(APP_API_LIB_OUTPUT_DIR)/$(LINK_NAME)
	$(LN) $(REAL_NAME) $(APP_API_LIB_OUTPUT_DIR)/$(SONAME)
	cp $(HEADER) $(APP_API_LIB_OUTPUT_DIR)
	cp  $(APP_API_LIB_OUTPUT_DIR)/$(LINK_NAME)* $(FILESYSTEM_DIR)/lib/
	
clean:
	$(RM) $(OBJS) $(REAL_NAME)
	$(RM) $(APP_API_LIB_OUTPUT_DIR)/$(LINK_NAME)
	$(RM) $(APP_API_LIB_OUTPUT_DIR)/$(SONAME)
	$(RM) $(APP_API_LIB_OUTPUT_DIR)/$(REAL_NAME)
	$(RM) $(APP_API_LIB_OUTPUT_DIR)/$(HEADER)
	$(RM) $(FILESYSTEM_DIR)/lib/$(LINK_NAME)*

