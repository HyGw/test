ROOTPATH=$(shell pwd)

GLIBPRELIB = $(ROOTPATH)/../install_glib_prelib
GLIB = $(ROOTPATH)/../install_glib
DBUSLIB = $(ROOTPATH)/../install_dbuslib

CFLAGS += -Wall -std=gnu99 -I$(GLIB)/include/glib-2.0 -I$(GLIB)/lib/glib-2.0/include -I $(DBUSLIB)/include/dbus-1.0 -I$(DBUSLIB)/lib/dbus-1.0/include


LDFLAGS += -L$(GLIB)/lib -lglib-2.0 -lgmodule-2.0 -lgobject-2.0 -lgthread-2.0 -lgio-2.0 -L$(GLIBPRELIB)/lib -lffi -liconv -lintl -lz -L$(DBUSLIB)/lib -ldbus-1


EXE = gdbus-daemon
OBJDIR = ./
SRC = $(wildcard ./*.c)
OBJ = $(SRC:.c=.o)
RM = rm -f
MV = mv -f
CP = cp -f

ARFLAGS = -rcs

all : $(EXE) install

$(EXE) : $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) 

.c.o :
	$(CC) -c $< -o $@ $(CFLAGS)

install:
#copy exec to root path	
	$(CP) $(EXE) ../
	$(CP) $(EXE) /home/test/workdir/tftpdir

clean : 
	$(RM) $(OBJ)
	$(RM) *~ \#*\#
	$(RM) $(EXE)

fclean : clean
	$(RM) $(LIB)

re : fclean all

test : 
	echo $(CFLAGS)
	echo $(LDFLAGS)
	echo $(ARFLAGS)

.PHONY : all clean fclean re 
