CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
SOURCES=./query.c ./query.h ../indexer/indexer.h
CFILES=./query.c

UTILDIR=../utils/
UTILFLAG=-ltseutil
UTILLIB=$(UTILDIR)libtseutil.a
UTILC=$(UTILDIR)document.c $(UTILDIR)file.c $(UTILDIR)files.c $(UTILDIR)hash.c $(UTILDIR)html.c $(UTILDIR)list.c
UTILH=$(UTILC:.c=.h)

query:$(SOURCES) $(UTILLIB)
	$(CC) $(CFLAGS1) -o $@ $(CFILES) -L $(UTILDIR) $(UTILFLAG)

$(UTILLIB): $(UTILC) $(UTILH)
	cd $(UTILDIR); make;

debug: $(SRCS)
	$(CC) $(CFLAGS) -g -ggdb -c $(SRCS) 
	$(CC) $(CFLAGS) -g -ggdb -o $(EXEC) $(OBJS)

clean:
	rm -f *~
	rm -f *#
	rm -f query
	rm -f *.o