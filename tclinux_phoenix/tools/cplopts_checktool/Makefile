
CC=gcc
CFLAGS += -DTRENDCHIP
OBJS= cplopts_begin.o cplopts_formcheck.o cplopts_remove.o
EXECS= cplopts_begin cplopts_formckeck cplopts_remove
all:  cplopts_begin cplopts_formcheck cplopts_remove

.c.o:
	$(CC) -c $(CFLAGS) $<

cc: $(OBJS) $(MIPSLIBS)
	$(CC) -o $@ $(OBJS)

clean:
	rm -f *.o *.a $(EXECS)
