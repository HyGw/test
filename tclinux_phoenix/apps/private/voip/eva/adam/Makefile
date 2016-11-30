PWD := $(shell pwd)
ADAM_SRC := adam.c
ADAM_OBJ := $(subst .c,.o,$(ADAM_SRC))

LIB_BASE_OBJECT = $(APP_LIBBASE_DIR)/obj
LIB_SYSS_OBJECT = $(APP_LIBSYSS_DIR)/obj
LIB_SLICUSER_OBJECT = $(APP_LIBSLICUSER_DIR)/obj
LIB_VDSPUSER_OBJECT = $(APP_LIBVDSP_DIR)/obj
LIB_GDI_OBJECT = $(PWD)/../gdi_mtk/obj
LIB_ADAM_OBJECT = $(PWD)/obj

SUBVPATH += $(LIB_BASE_OBJECT) \
            $(LIB_SYSS_OBJECT) \
            $(LIB_SLICUSER_OBJECT) \
            $(LIB_VDSPUSER_OBJECT) \
            $(LIB_GDI_OBJECT) \
            
SUB_OBJ = $(foreach DIR,$(SUBVPATH), $(wildcard $(DIR)/*.o))

ifeq ($(LINK),dynamic)
	TARGET := $(PWD)/libadam.so
else
	TARGET := $(PWD)/libadam.a
endif


INCLUDE = -I$(SYSROOT)/usr/include/ -I$(PWD)/../common

CFLAGS+= -O2 -Wall -fPIC $(INCLUDE)

ifeq ($(DSP),viking)
GDI_LIB_DIR := $(PWD)/../bin
GDI_LIB_LINK := -L$(GDI_LIB_DIR)/ -lgdi_viking 
GDI_LIB := $(GDI_LIB_DIR)/libgdi_viking.a
GDI_INCLUDE = -I$(PWD)/../gdi
CFLAGS += -D OSAL_PTHREADS -D DSPID_VIKING 
endif

ifeq ($(DSP),mtk)
GDI_LIB_DIR := $(PWD)/../bin

GDI_LIB_LINK := -L$(GDI_LIB_DIR)/ -lgdi_mtk 
ifeq ($(LINK),dynamic)
	GDI_LIB := $(GDI_LIB_DIR)/libgdi_mtk.so
else
	GDI_LIB := $(GDI_LIB_DIR)/libgdi_mtk.a 
endif
GDI_INCLUDE = -I$(PWD)/../gdi_mtk
CFLAGS += -D DSPID_MTK
endif

CFLAGS += $(GDI_INCLUDE)  

%.o:%.c
	$(CC) $(CFLAGS) $(GDI_LIB_LINK) -c $< -o $(OBJECT_PATH)/$@

.PHONY: all clean

OBJECT_PATH = $(PWD)/obj
OBJECT = $(addprefix $(OBJECT_PATH)/, $(ADAM_OBJ))
		
all: OBJECT_DIR $(TARGET)
	
$(OBJECT):$(ADAM_OBJ)

# Creating object dir
OBJECT_DIR:
	mkdir -p $(OBJECT_PATH)
	

ifeq ($(LINK),dynamic)
$(TARGET):$(OBJECT)
	$(LD) -shared -o $@  $(OBJECT) $(SUB_OBJ)	 
else
$(TARGET):$(OBJECT)
	$(AR) r $@ $(OBJECT) $(SUB_OBJ)
endif


clean:
	rm -rf *.o  *.so $(TARGET) $(OBJECT_PATH)
