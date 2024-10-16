#
# json-c
#
# Copyright (c) 2022-2024 ShaoBo Song
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

CC=$(CROSS_PREFIX)gcc
AR=$(CROSS_PREFIX)ar
RANLIB=$(CROSS_PREFIX)ranlib

CFLAGS=-I$(CURDIR)/include -O0 -g -D_REENTRANT -DCONFIG_LOG_FILE=\"json.log\" -Wall -MMD -std=c99
LDFLAGS=

OBJS:=$(addprefix lib/, json_htab.o json_impl.o json_list.o json.o json_utils.o)
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
PROGS+=$(TESTS)

all: $(PROGS)

$(LIB): $(OBJS)
	$(AR) rv $@ $?
	$(RANLIB) $@

%: %.c $(LIB)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(PROGS) *.log *.d lib/*.o lib/*.d

-include $(wildcard *.d)
-include $(wildcard lib/*.d)
