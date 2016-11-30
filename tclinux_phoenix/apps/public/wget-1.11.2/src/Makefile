# Makefile for `wget' utility
# Copyright (C) 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003,
# 2004, 2005, 2006, 2007, 2008 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with Wget.  If not, see <http://www.gnu.org/licenses/>.

# Additional permission under GNU GPL version 3 section 7

# If you modify this program, or any covered work, by linking or
# combining it with the OpenSSL project's OpenSSL library (or a
# modified version of that library), containing parts covered by the
# terms of the OpenSSL or SSLeay licenses, the Free Software Foundation
# grants you additional permission to convey the resulting work.
# Corresponding Source for a non-source form of such a combination
# shall include the source code for the parts of OpenSSL used as well
# as that of the covered work.

#
# Version: 1.11.2
#

SHELL = /bin/sh

top_builddir = ..

top_srcdir = ..
srcdir     = .


prefix       = /usr/local
exec_prefix  = ${prefix}
bindir       = ${exec_prefix}/bin
sysconfdir   = ${prefix}/etc
datadir      = ${prefix}/share
localedir    = $(datadir)/locale

DESTDIR      =


CPPFLAGS = 
# The following line is losing on some versions of make!
DEFS     = -DHAVE_CONFIG_H -DSYSTEM_WGETRC=\"$(sysconfdir)/wgetrc\" -DLOCALEDIR=\"$(localedir)\"
CFLAGS   =  -O2 -Wall
LDFLAGS  =  
LIBS     = -ldl     
exeext   = 

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PHONEAPP)
ifeq ($(TCSUPPORT_CT_PHONEAPP),1)
    CFLAGS+= -DTCSUPPORT_CT_PHONEAPP
endif
#endif/*TCSUPPORT_COMPILE*/
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_TR143_WGET_DOWNLOAD)
ifeq ($(TCSUPPORT_CT_TR143_WGET_DOWNLOAD),1)
    CFLAGS+= -DTCSUPPORT_CT_TR143_WGET_DOWNLOAD
endif
#endif/*TCSUPPORT_COMPILE*/
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PON_GDV21)
ifeq ($(TCSUPPORT_CT_PON_GDV21),1)
    CFLAGS+= -DTCSUPPORT_CT_PON_GDV21
endif
#endif/*TCSUPPORT_COMPILE*/
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PON_SC)
ifeq ($(TCSUPPORT_CT_PON_SC),1)
    CFLAGS+= -DTCSUPPORT_CT_PON_SC
endif
#endif/*TCSUPPORT_COMPILE*/
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_JOYME)
ifeq ($(TCSUPPORT_CT_JOYME),1)
    CFLAGS+= -DTCSUPPORT_CT_JOYME
endif
#endif/*TCSUPPORT_COMPILE*/
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_PON_ROSTELECOM)
ifeq ($(TCSUPPORT_PON_ROSTELECOM),1)
    CFLAGS+= -DTCSUPPORT_PON_ROSTELECOM
endif
#endif/*TCSUPPORT_COMPILE*/

INCLUDES = -I. -I$(srcdir)

COMPILE = $(CC) $(INCLUDES) $(CPPFLAGS)  $(DEFS) $(CFLAGS) -DHAVE_LIBMATRIXSSL
LINK    = $(CC) $(CFLAGS) $(LDFLAGS) -o $@
INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
RM = rm -f
ETAGS = etags

# Conditional compiles
ALLOCA     = 
MD5_OBJ    =  gen-md5.o gnu-md5.o
OPIE_OBJ   = 
NTLM_OBJ   = 
SSL_OBJ    =  matrixssl.o matrixssl_helper.o libmatrixssl.so libtcapi.so
GETOPT_OBJ = 

OBJ = $(ALLOCA) cmpt.o connect.o convert.o cookies.o              \
      ftp.o ftp-basic.o ftp-ls.o $(OPIE_OBJ) $(GETOPT_OBJ) hash.o \
      host.o html-parse.o html-url.o http.o $(NTLM_OBJ) init.o    \
      log.o main.o $(MD5_OBJ) netrc.o progress.o ptimer.o recur.o \
      res.o retr.o safe-ctype.o snprintf.o spider.o $(SSL_OBJ)    \
      url.o utils.o version.o xmalloc.o

.SUFFIXES:
.SUFFIXES: .c .o

.c.o:
	$(COMPILE) -c $<

# Dependencies for building

wget$(exeext): $(OBJ)
	$(LINK) $(OBJ) $(LIBS)

# We make object files depend on every header.  Rather than attempt to
# track dependencies, everything gets recompiled when a header
# changes.  With a program of Wget's size this doesn't waste much
# time, and it's a lot safer than attempting to get all the
# dependencies right.

$(OBJ): config-post.h config.h connect.h convert.h cookies.h ftp.h \
        gen-md5.h getopt.h gnu-md5.h hash.h host.h html-parse.h    \
        http-ntlm.h init.h log.h mswindows.h netrc.h options.h     \
        progress.h ptimer.h recur.h res.h retr.h safe-ctype.h      \
        spider.h ssl.h sysdep.h url.h utils.h wget.h xmalloc.h

#
# Dependencies for installing
#

install: install.bin

uninstall: uninstall.bin

install.bin: wget$(exeext)
	$(top_srcdir)/mkinstalldirs $(DESTDIR)$(bindir)
	$(INSTALL_PROGRAM) wget$(exeext) $(DESTDIR)$(bindir)/wget$(exeext)

uninstall.bin:
	$(RM) $(DESTDIR)$(bindir)/wget$(exeext)

#
# Dependencies for cleanup
#

clean:
	$(RM) *.o wget$(exeext) *~ *.bak core core.[0-9]*

distclean: clean
	$(RM) Makefile config.h

realclean: distclean
	$(RM) TAGS config.h.in

#
# Dependencies for maintenance
#

subdir = src

Makefile: Makefile.in ../config.status
	cd .. && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= ./config.status

TAGS: *.c *.h
	-$(ETAGS) *.c *.h
