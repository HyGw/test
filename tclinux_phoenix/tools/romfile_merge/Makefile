MXML_DIR=mxml-2.4
INC_DIR=mxml-2.4
CC=gcc
LD=ld
AR=ar

all:
	cp -rf ../../apps/public/mxml-2.4 .
	make -C mxml-2.4 clean
	make -C mxml-2.4
#	gcc -c -o main.o main.c -I $(INC_DIR)
#	gcc -static -o main main.o -lmxml -L mxml-2.4
#
	gcc -c -o romfile_merge.o romfile_merge.c -I $(INC_DIR)
	gcc -static -o romfile_merge romfile_merge.o -lmxml -L mxml-2.4

clean:
	rm -f main *.o
