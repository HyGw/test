PWD := $(shell pwd)
EVA_DIR := $(PWD)/..
EVA_INC := -I$(EVA_DIR)/common



EVCOM_DIR := $(PWD)
EVCOM_SRC := evcom.c evcom_helper.c 
EVCOM_OBJ := $(subst .c,.o,$(EVCOM_SRC))

ifeq ($(DSP),viking)
GDI_DIR := $(EVA_DIR)/gdi
GDI_INC := -I$(GDI_DIR)/
GDI_LIB := -L$(EVA_DIR)/bin -lgdi_viking
CFLAGS += -D DSPID_VIKING 
endif

ifeq ($(DSP),mtk)
GDI_DIR := $(EVA_DIR)/gdi_mtk

GDI_INC := -I$(GDI_DIR)/
GDI_LIB := -L$(EVA_DIR)/bin -lgdi_mtk
CFLAGS += -D DSPID_MTK

DSP_INCLUDE = -I$(MODULES_MTK_FXS3_DIR)/include -I$(MODULES_MTK_OVDSP_DIR)/include
DSP_API_LIB := -L $(MODULES_MTK_FXS3_DIR)/ -lslic_user -L $(MODULES_MTK_OVDSP_DIR)/ -lvdsp_user -L $(APP_MTKSIP_DIR)/install/lib/ -lsyss -L $(APP_VOIP_DIR)/install/lib/ -lbase -L $(LIB_DIR) -ltcapi
endif

ADAM_DIR := $(EVA_DIR)/adam
ADAM_INC := -I$(ADAM_DIR)
ADAM_LIB := -L$(EVA_DIR)/bin -ladam 


INCLUDE = $(EVA_INC) $(ADAM_INC) $(GDI_INC)
CFLAGS += -O2 -Wall -lpthread  -lm $(INCLUDE)

TARGET = evcom

%.o:%.c
	$(CC) $(CFLAGS) $(ADAM_LIB) $(GDI_LIB) $(DSP_API_LIB) -c $<

	
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(EVCOM_OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(EVCOM_OBJ) $(ADAM_LIB) $(GDI_LIB) $(DSP_API_LIB)

	
clean:
	rm -rf *.o $(TARGET) $(EVCOM_OBJ)
	
