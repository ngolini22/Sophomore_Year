CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
CFLAGS1 = -ggdb -Wall
EXEC = indexer
OBJS = indexer.o document.o file.o hash.o list.o
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)
indexer.o: indexer.c list.h indexer.h document.h file.h
	$(CC) $(CFLAGS1) -c indexer.c
hash.o: hash.c hash.h indexer.h
	$(CC) $(CFLAGS) -c hash.c
document.o: document.c document.h indexer.h
	$(CC) $(CFLAGS) -c document.c
file.o: file.c indexer.h list.h
	$(CC) $(CFLAGS) -c file.c
list.o: list.c list.h hash.h
	$(CC) $(CFLAGS) -c list.c
debug: $(SRCS)
	$(CC) $(CFLAGS) -g -ggdb -c $(SRCS)
	$(CC) $(CFLAGS) -g -ggdb -o $(EXEC) $(OBJS)
clean:
	rm -f *~
	rm -f *#
	rm -f ./indexer
	rm -f *.o