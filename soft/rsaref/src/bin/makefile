# This is a makefile for UNIX

#Comment out next line if you want to build for local machine
#BUILDTOOL_PREFIX=sparc-elf-

# extension for object files
O = o

# commands
CC = $(BUILDTOOL_PREFIX)gcc
LIB = $(BUILDTOOL_PREFIX)ar
RANLIB = $(BUILDTOOL_PREFIX)ranlib

# name of temporary library script
TEMPFILE = $(TEMP)/temp.mak

# standard include directory
STDINCDIR=/usr/include

# The places to look for include files (in order).
INCL =  -I. -I$(RSAREFDIR) #-I$(STDINCDIR)

# name of main executable to build
PROG = all

# Normal C flags.
CFLAGS = $(INCL) -O -c -DPROTOTYPES=1 -g
MFLAGS = -I. -I$(RSAREFDIR)

# The location of the common source directory.
RSAREFDIR = ../source/
RSAREFLIB = rsaref.a

# The location of the demo source directory.
RDEMODIR = ../rdemo/

all : rdemo dhdemo encrypt genkeys

clean : 
	rm -rf *.o rdemo dhdemo encrypt genkeys *.a *~

genkeys : genkeys.$(O) $(RSAREFLIB)
	$(CC) -o $@ genkeys.$(O) $(RSAREFLIB)

rdemo : rdemo.$(O) $(RSAREFLIB)
	$(CC) -o $@ rdemo.$(O) $(RSAREFLIB)

dhdemo : dhdemo.$(O) $(RSAREFLIB)
	$(CC) -o $@ dhdemo.$(O) $(RSAREFLIB)

encrypt : encrypt.$(O) $(RSAREFLIB)
	$(CC) -o $@ encrypt.$(O) $(RSAREFLIB)

$(RSAREFLIB) : desc.$(O) digit.$(O) md2c.$(O) md5c.$(O) nn.$(O) prime.$(O)\
  rsa.$(O) r_encode.$(O) r_dh.$(O) r_enhanc.$(O) r_keygen.$(O) r_random.$(O)\
  r_stdlib.$(O)
	$(LIB) r $@ $?
	$(RANLIB) $@

rdemo.$(O) : $(RDEMODIR)rdemo.c $(RSAREFDIR)global.h $(RSAREFDIR)rsaref.h
	$(CC) $(CFLAGS) $(RDEMODIR)rdemo.c

dhdemo.$(O) : $(RDEMODIR)dhdemo.c $(RSAREFDIR)global.h $(RSAREFDIR)rsaref.h
	$(CC) $(CFLAGS) $(RDEMODIR)dhdemo.c

encrypt.$(O) : $(RDEMODIR)encrypt.c $(RSAREFDIR)global.h $(RSAREFDIR)rsaref.h
	$(CC) $(CFLAGS) $(RDEMODIR)encrypt.c

genkeys.$(O) : $(RDEMODIR)genkeys.c $(RSAREFDIR)global.h $(RSAREFDIR)rsaref.h
	$(CC) $(CFLAGS) $(RDEMODIR)genkeys.c

include $(RSAREFDIR)targets.mak
