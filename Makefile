#
# bd_xjson
#
# Copyright (c) 2022 ShaoBo Song
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

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

OBJS:=$(addprefix lib/, bd_xjson_htab.o bd_xjson_impl.o bd_xjson_list.o bd_xjson.o utils.o)
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

%: %.c $(LIB)
	$(CC) $(TFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(TESTS) $(USAGE) *.a lib/*.o
