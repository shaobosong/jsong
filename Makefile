SRCDIR := $(CURDIR)
INCLUDE = $(SRCDIR)/include

CC = gcc
CFLAGS = -I$(INCLUDE) -g3 -O0 -D_REENTRANT -Wall -static
RANLIB = ranlib

# Following is the main library, built from all the object files
# in the lib/ and directories.
LIBS = $(SRCDIR)/libsimp.a

PROGS = test_json_array test_json_object test_json_number test_json_string \
		test_json_true test_json_false test_json_null

all:	$(PROGS)

test_json_array:
		$(CC) $(CFLAGS) -o $@ test_json_array.c $(LIBS)

test_json_object:
		$(CC) $(CFLAGS) -o $@ test_json_object.c $(LIBS)

test_json_number:
		$(CC) $(CFLAGS) -o $@ test_json_number.c $(LIBS)

test_json_string:
		$(CC) $(CFLAGS) -o $@ test_json_string.c $(LIBS)

test_json_true:
		$(CC) $(CFLAGS) -o $@ test_json_true.c $(LIBS)

test_json_false:
		$(CC) $(CFLAGS) -o $@ test_json_false.c $(LIBS)

test_json_null:
		$(CC) $(CFLAGS) -o $@ test_json_null.c $(LIBS)

clean:
		rm -rf $(PROGS) *.a lib/*.o lib/*.a
