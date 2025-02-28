# Makefile     Kyla Widodo     April 28, 2023
# 
# Makefile for querier

PROG = querier
OBJS = querier.o 
LIBS = ../common/common.a ../libcs50/libcs50-given.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I ../libcs50 -I../common
CC = gcc
MAKE = make

$(PROG): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

querier.o: ../common/pagedir.h ../libcs50/webpage.h ../libcs50/file.h ../libcs50/hashtable.h ../libcs50/set.h  ../libcs50/mem.h  ../common/index.h ../common/word.h

.PHONY: test valgrind clean

test: $(PROG) testing.sh
		bash -v testing.sh >& testing.out
clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)
	rm -f querier
	rm -f core

valgrind: $(PROG)
	valgrind ./$(PROG) ../data-index/letters-depth-2 ../data-index/indices/letters-2
