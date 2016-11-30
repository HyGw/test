PWD := $(shell pwd)

SLIC_CTRL_TEST_DIR := $(PWD)
SLIC_CTRL_TEST_SRC := slic_ctrl_test.c 
SLIC_CTRL_TEST_OBJ := $(subst .c,.o,$(SLIC_CTRL_TEST_SRC))
SLIC_CTRL_API_DIR := $(PWD)/../lib
SLIC_CTRL_API_LIB := -L $(SLIC_CTRL_API_DIR)/ -lslic_ctrl

INCLUDE = -I$(SLIC_CTRL_API_DIR)  -I$(SYSROOT)/usr/include/
CFLAGS += -O2 -Wall -lpthread -lm $(INCLUDE)

TARGET = slic_ctrl_test

%.o:%.c
	$(CC) $(CFLAGS) -c $<

	
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SLIC_CTRL_TEST_OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(SLIC_CTRL_TEST_OBJ) $(SLIC_CTRL_API_LIB) $(SYSROOT)/usr/lib/libm.a  -lrt
	#@cp $(SLIC_CTRL_TEST_DIR)/$(TARGET) $(SLIC_CTRL_TEST_DIR)/bin/.
	@cp $(SLIC_CTRL_TEST_DIR)/$(TARGET) $(SLIC_CTRL_TEST_DIR)/../../bin/.

	
clean:
	rm -rf *.o $(TARGET) $(SLIC_CTRL_TEST_OBJ)
	
