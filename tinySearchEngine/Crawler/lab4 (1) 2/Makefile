CC = gcc
CFLAGS = -Wall -pedantic -std=c99
EXEC = crawler
OBJS = crawler.o extractURLs.o getAddressFromTheLinksToBeVisited.o getPage.o hash.o html.o initLists.o setURLasVisited.o updateListLinkToBeVisited.o addNode.o checkUniqueness.o cleanUp.o
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)
crawler.o: crawler.c crawler.h hash.h html.h header.h
	$(CC) $(CFLAGS) -c crawler.c crawler.h hash.h html.h header.h
extractURLs.o: extractURLs.c html.h crawler.h
	$(CC) $(CFLAGS) -c extractURLs.c html.h crawler.h
getAddressFromTheLinksToBeVisited.o: getAddressFromTheLinksToBeVisited.c crawler.h
	$(CC) $(CFLAGS) -c getAddressFromTheLinksToBeVisited.c crawler.h
getPage.o: getPage.c crawler.h
	$(CC) $(CFLAGS) -c getPage.c crawler.h
hash.o: hash.c
	$(CC) $(CFLAGS) -c hash.c
html.o: html.c
	$(CC) $(CFLAGS) -c html.c
initLists.o: initLists.c crawler.h
	$(CC) $(CFLAGS) -c initLists.c crawler.h
setURLasVisited.o: setURLasVisited.c crawler.h
	$(CC) $(CFLAGS) -c setURLasVisited.c crawler.h
updateListLinkToBeVisited.o: updateListLinkToBeVisited.c crawler.h hash.h
	$(CC) $(CFLAGS) -c updateListLinkToBeVisited.c crawler.h hash.h
addNode.o: addNode.c hash.h crawler.h
	$(CC) $(CFLAGS) -c addNode.c hash.h crawler.h
checkUniqueness.o: checkUniqueness.c crawler.h hash.h
	$(CC) $(CFLAGS) -c checkUniqueness.c crawler.h hash.h
cleanUp.c: cleanUp.c
	$(CC) $(CFLAGS) -c cleanUp.c
debug: $(SRCS)
	$(CC) $(CFLAGS) -g -ggdb -c $(SRCS)
	$(CC) $(CFLAGS) -g -ggdb -o $(EXEC) $(OBJS)
clean:
	rm -f *~
	rm -f *#
	rm -f ./crawler
	rm -f *.o