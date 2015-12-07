#ifndef _LIST_H_
#define _LIST_H_

#include "indexer.h"

// includes the addNode and checkUniqueness functions


// adds node to a doubly linked list in a hash table depending on different edge cases

void addNode(char *word, INVERTED_INDEX *index, int docId);

// checks if the word is in the list or not. 
// return 1 if unique, 0 if not unique

int checkUniqueness(char *word, INVERTED_INDEX *index);

/*
updateIndex()
input: 
1) string containing the word 
2) identifier of the document
3) inverted index
output: 1 if successful, 0 if not

- updates the data structure containing the index
*/

int updateIndex(char* word, int documentId, INVERTED_INDEX* index);


/*
Finds the correct DOCUMENTNODE for the correct WORDNODE and updates the frequency. 
If it is from a new document, create a new DOCUMENTNODE and place it at the end of the node list
*/
void updateFreq(char* word, int documentId, INVERTED_INDEX *index);


#endif