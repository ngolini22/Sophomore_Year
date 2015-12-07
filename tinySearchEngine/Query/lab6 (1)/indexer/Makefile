CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
CFLAGS1 = -ggdb -Wall

UTILDIR=../utils/
UTILFLAG=-ltseutil
UTILLIB=$(UTILDIR)libtseutil.a
UTILC=$(UTILDIR)file.c $(UTILDIR)hash.c $(UTILDIR)document.c $(UTILDIR)list.c
UTILH=$(UTILC:.c=.h)

indexer:$(SOURCES) ./indexer.c  ./indexer.h
	$(CC) $(CFLAGS1) -c ./indexer.c ./indexer.h
	$(CC) $(CFLAGS) -o $@ indexer.o  -L $(UTILDIR) $(UTILFLAG)

$(UTILLIB): $(UTILC) $(UTILH)
	cd $(UTILDIR); make;

debug: $(SRCS)
	$(CC) $(CFLAGS) -g -ggdb -c $(SRCS)
	$(CC) $(CFLAGS) -g -ggdb -o $(EXEC) $(OBJS)

clean:
	rm -f *~
	rm -f *#
	rm -f indexer
	rm -f *.o