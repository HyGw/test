##################################
#####Configured directories#######
##################################
INCLUDE			= -I$(APP_API_LIB_QDMA_DIR)

##################################
########Configured output#########
##################################
OBJS		=	api_lib_qdmamgr.o
HEADER		=	libapi_lib_qdmamgr.h
HEADER2		=	qdma_wan_lan.h
HEADER3		=	qdma_type.h
LINK_NAME	=	libapi_lib_qdmamgr_lan.so
SONAME		=	$(LINK_NAME).1
REAL_NAME	=	$(SONAME).0.0

##################################
########Configured flags##########
##################################
RM				= /bin/rm -f
LN      		= /bin/ln -s
CFLAGS 			+=-fPIC -Wall $(INCLUDE) $(TC_CFLAGS)
CFLAGS 			+= -D QDMA_LAN
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
	cp $(HEADER2) $(APP_API_LIB_OUTPUT_DIR)
	cp $(HEADER3) $(APP_API_LIB_OUTPUT_DIR)
	cp  $(APP_API_LIB_OUTPUT_DIR)/$(LINK_NAME)* $(FILESYSTEM_DIR)/lib/
	mv api_lib_qdmamgr.o api_lib_qdmamgr_lan.o
	
clean:
	$(RM) api_lib_qdmamgr_lan.o $(REAL_NAME)
	$(RM) $(APP_API_LIB_OUTPUT_DIR)/$(LINK_NAME)
	$(RM) $(APP_API_LIB_OUTPUT_DIR)/$(SONAME)
	$(RM) $(APP_API_LIB_OUTPUT_DIR)/$(REAL_NAME)
	$(RM) $(APP_API_LIB_OUTPUT_DIR)/$(HEADER)
	$(RM) $(APP_API_LIB_OUTPUT_DIR)/$(HEADER2)
	$(RM) $(APP_API_LIB_OUTPUT_DIR)/$(HEADER3)
	$(RM) $(FILESYSTEM_DIR)/lib/$(LINK_NAME)*

