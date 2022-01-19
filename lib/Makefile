SRCDIR := $(CURDIR)/..
INCLUDE = $(SRCDIR)/include

CC = gcc
CFLAGS = -I$(INCLUDE) -g3 -O0 -D_REENTRANT -Wall -static
RANLIB = ranlib

# Following is the main library, built from all the object files
# in the lib/ directories.
LIBTARGET = $(SRCDIR)/libsimp.a

LIB_OBJS = 	bd_xjson_base.o bd_xjson.o bd_xjson_list.o bd_xjson_htab.o \
			bd_xjson_alloc.o error.o alloc.o

all:	${LIB_OBJS}
		ar rv ${LIBTARGET} $?
		${RANLIB} ${LIBTARGET}

clean:
		rm -rf ${LIB_OBJS}
