# Makefile     Kyla Widodo     April 23, 2023
# 
# Makefile for common
 
PROG = common.a 
OBJS = pagedir.o index.o word.o
LIBS = common.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I ../libcs50 -I../common
CC = gcc
MAKE = make

$(PROG): $(OBJS)
	ar cr  $(LIBS) $(OBJS)

pagedir.o: pagedir.h ../libcs50/webpage.h ../libcs50/file.h
index.o: index.h ../libcs50/mem.h ../libcs50/file.h  ../libcs50/hashtable.h ../libcs50/counters.h 
word.o: word.h

.PHONY: clean

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)

