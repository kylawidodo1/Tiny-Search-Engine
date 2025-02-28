# Makefile     Kyla Widodo     April 23, 2023
# 
# Makefile for crawler 

PROG = crawler
OBJS = crawler.o 
LIBS = ../libcs50/libcs50-given.a ../common/common.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I ../libcs50 -I../common
CC = gcc
MAKE = make

$(PROG): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

#crawler source dependencies 
crawler.o: ../common/pagedir.h ../libcs50/webpage.h ../libcs50/file.h ../libcs50/hashtable.h ../libcs50/bag.h	
.PHONY: test valgrind clean

test: $(PROG) testing.sh
		bash -v testing.sh >& testing.out
clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)

valgrind: $(PROG) valgrind.sh 
	valgrind ./$(PROG) http://cs50tse.cs.dartmouth.edu/tse/letters/ data/letters-depth 2 &> valgrind.out 
