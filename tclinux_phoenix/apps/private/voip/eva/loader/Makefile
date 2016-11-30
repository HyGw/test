PWD := $(shell pwd)

LOADER_DIR := $(PWD)
LOADER_SRC := loader.c 
LOADER_OBJ := $(subst .c,.o,$(LOADER_SRC))


INCLUDE = 
CFLAGS += -O2 -Wall -lpthread -lm $(INCLUDE)

TARGET = voip_loader

%.o:%.c
	$(CC) $(CFLAGS) -c $<

	
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(LOADER_OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(LOADER_OBJ) 
	@cp $(LOADER_DIR)/$(TARGET) $(LOADER_DIR)/bin/.
	@cp $(LOADER_DIR)/$(TARGET) $(LOADER_DIR)/../bin/.

	
clean:
	rm -rf *.o $(TARGET) $(LOADER_OBJ)
	
