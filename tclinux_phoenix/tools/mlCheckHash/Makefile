
CC=gcc
CFLAGS += -DTRENDCHIP
OBJS=mlCheckHash.o
EXECS=mlCheckHash
all: mlCheckHash

.c.o:
	$(CC) -c $(CFLAGS) $<

cc: $(OBJS) $(MIPSLIBS)
	$(CC) -o $@ $(OBJS)

clean:
	rm -f *.o *.a $(EXECS)
