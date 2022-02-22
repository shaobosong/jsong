#
# bd_xjson
#
#

# generate some simple cases for test
CONFIG_TEST=y
# riscv32 build
CONFIG_RISCV32=
# riscv64 build
CONFIG_RISCV64=

ifdef CONFIG_RISCV32
CROSS_PREFIX=riscv32-unknown-elf-
endif
ifdef CONFIG_RISCV64
CROSS_PREFIX=riscv64-unknown-elf-
endif

CC=$(CROSS_PREFIX)gcc
AR=$(CROSS_PREFIX)ar
RANLIB=$(CROSS_PREFIX)ranlib

CFLAGS=-O0 -g3 -D_REENTRANT -Wall -static
TFLAGS:=$(CFLAGS)
CFLAGS+=-I$(CURDIR)/include
LDFLAGS=

OBJS:=$(addprefix lib/, alloc.o bd_xjson_htab.o bd_xjson_impl.o bd_xjson_list.o bd_xjson.o error.o utils.o)
LIB:=libjson.a
USAGE:=usage
TESTS:=test_json_array \
	test_json_object \
	test_json_number \
	test_json_string \
	test_json_true \
	test_json_false \
	test_json_null

PROGS:=$(LIB)
PROGS+=$(USAGE)
ifdef CONFIG_TEST
PROGS+=$(TESTS)
endif

all: $(PROGS)

$(LIB): $(OBJS)
	$(AR) rv $@ $?
	$(RANLIB) $@

%: %.c
	$(CC) $(TFLAGS) -o $@ $< $(LIB)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(TESTS) $(USAGE) *.a lib/*.o
