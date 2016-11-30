# $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/private/wifi/2009_1106_RT3390_LinuxAP_V2.3.2.0_DPA/miniupnpd/pf/Makefile#1 $
# made for GNU Make
CFLAGS = -Wall -g
EXECUTABLES = testobsdrdr

all:	$(EXECUTABLES)

clean:
	rm -f *.o $(EXECUTABLES)

testobsdrdr:	testobsdrdr.o obsdrdr.o

obsdrdr.o:	obsdrdr.c obsdrdr.h

testobsdrdr.o:	testobsdrdr.c obsdrdr.h

