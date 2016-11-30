####### Variables Declares #########################
CC 		= $(BOOTROM_CC)
LD 		= $(BOOTROM_LD)
NM 		= $(BOOTROM_NM)
OBJCOPY = $(BOOTROM_OBJCOPY)
OBJDUMP = $(BOOTROM_OBJDUMP)
CFLAGS	= $(BOOTROM_CFLAG)
IMG2BIN	= $(BOOTROM_IMG2BIN)

RM	= rm
####################################################


all:

	make -C move_data $(BOOTRAM_PARAMETER)
	make -C boot2 $(BOOTRAM_PARAMETER) LZMA_IMG=1
		
	cp ./boot2/output/boot2.img ./boot2.img -f
	cp ./boot2/output/boot2.text ../boot2.txt -f
	cp ./move_data/output/move_data.img ./move_data.img -f	
	cp ./move_data/output/move_data.text ../move_data.txt -f
	
	$(CC) $(CFLAGS) -c move_data_load.c -o move_data_load.o
	$(CC) $(CFLAGS) -c boot2load.c -o boot2load.o
	
	$(OBJCOPY) --add-section .move_data=move_data.img move_data_load.o
	$(OBJCOPY) --add-section .boot2=boot2.img boot2load.o
	
	
clean:
	make -C move_data clean
	make -C boot2 clean
	$(RM) -f *.o
	$(RM) -f *.img