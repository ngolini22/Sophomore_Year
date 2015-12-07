CC = gcc
CFLAGS = -Wall -pedantic -std=c99
SOURCES=./crawler.c
CFILES=./crawler.c 

UTILDIR=../utils/
UTILFLAG=-ltseutil
UTILLIB=$(UTILDIR)libtseutil.a
UTILC=$(UTILDIR)files.c $(UTILDIR)hash.c $(UTILDIR)html.c
UTILH=$(UTILC:.c=.h)

crawler:$(SOURCES) $(UTILDIR)header.h $(UTILLIB)
	$(CC) $(CFLAGS) -o $@ $(CFILES) -L $(UTILDIR) $(UTILFLAG)

$(UTILLIB): $(UTILC) $(UTILH)
	cd $(UTILDIR); make; 

debug: $(SRCS)
	$(CC) $(CFLAGS) -g -ggdb -c $(SRCS)
	$(CC) $(CFLAGS) -g -ggdb -o $(EXEC) $(OBJS)
clean:
	rm -f *~
	rm -f *#
	rm -f ./crawler
	rm -f *.o