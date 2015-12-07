#ifndef _FILE_H_
#define _FILE_H_

#include "indexer.h"

/*
saveFile()
input: pointer to the inverted index
output: 1 if successful, 0 is false 

- saves the inverted index to a file
*/

int saveFile(INVERTED_INDEX* index, char* filename);


/*
reads the index.dat file back into a new index

*/

int makeWord(char *word);

/*
reads the index.dat file back into a new index 
	- see line by line commenting for pseudocode 

*/

INVERTED_INDEX* readFile(char *filename);


#endif