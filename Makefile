SRCDIR := $(CURDIR)
INCLUDE = $(SRCDIR)/include

CC = gcc
CFLAGS = -I$(INCLUDE) -g3 -O0 -D_REENTRANT -Wall -static
RANLIB = ranlib

# Following is the main library, built from all the object files
# in the lib/ and directories.
LIBS = $(SRCDIR)/libsimp.a

PROGS = test_case

all:	$(PROGS)

test_case:
		$(CC) $(CFLAGS) -o $@ test_case.c $(LIBS)

clean:
		rm -rf $(PROGS) *.a lib/*.o lib/*.a
