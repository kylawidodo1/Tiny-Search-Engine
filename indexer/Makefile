# Makefile     Kyla Widodo     April 28, 2023
# 
# Makefile for indexer

PROG = indexer
OBJS = indexer.o 
LIBS = ../common/common.a ../libcs50/libcs50-given.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I ../libcs50 -I../common
CC = gcc
MAKE = make

$(PROG): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indextest: indextest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indexer.o: ../common/pagedir.h ../libcs50/webpage.h ../libcs50/file.h ../common/index.h ../common/word.h

indextest.o: ../common/index.h ../common/word.h

all: indexer indextest

.PHONY: test valgrind clean

test: $(PROG) testing.sh
		bash -v testing.sh >& testing.out
clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)
	rm -f index
	rm -f test
	rm -f compiletest
	rm -rf ../data-index

valgrind: $(PROG)

