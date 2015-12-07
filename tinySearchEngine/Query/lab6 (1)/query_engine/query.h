#ifndef _QUERY_H_
#define _QUERY_H_


// DATA STRUCTURE

/*
Since WORDNODEs and DOCUMENTNODEs have exactly what is needed
	for ranking the urls, the QHOLDER is a structure that holds
	WORDNODES for only the words from the query, and they point to 
	DOCUMENTNODES for each word
The list of DOCUMENTNODES is the results list of the URLs from the query
	if the two words are 'OR'ed, add every node to this list from 
	both WORDNODES and add freq if there it is not a unique docID
	It the words are 'AND'ed together, find only the docs that overlap
	and add only those to the result list. 
*/

typedef struct _QHOLDER {
	WORDNODE *start;
	WORDNODE *end;
	DOCUMENTNODE *results; // will be the list of docIDs for the query
} _QHOLDER;

typedef struct _QHOLDER QHOLDER;


// FUNCTIONS

/*
function to make query all lowercase.

Loops through all chars in the user input string and switch 
	them to lowercase
*/
void makeLower(char *str);

/*
Starts at index->start, and runs through wordnodes until word is found.
If if doesn't find the word, flag = 0.
If it is found, flag = 1.
Returns 
*/
WORDNODE *searchIndex(int *flag, char *str, INVERTED_INDEX *index);

/*
Takes the int doc_id and makes it a string
Open file with the doc_id name
Extract first line (which should be the url)
Returnt the url
*/
char *getURL(int doc_id, char *target_dir);

// initializes the new index
void initializeQHOLDER(void);

// adds a wordnode to the qholder for each word form the query
// 		that is in the index
// it adds the next WORDNODE to the end of the qholder each time
void addWORDNODE(WORDNODE *wn);

/*
Parses user input into seperate words to account for multiple word searches
*/
int getNextWord(const char* input, int currentPosition, char **wordList, int i);


WORDNODE *searchQHOLDER(int *flag1, char *word);
/*
- wordnodes should be in qholder
- First looks for an OR in the list
- if there is an or
	- add DOCNODES from each wordnode to results
	- if there is overlap with DOCNODES, just add frequencies together
	- for the second worded 
- start at beginning of list
	- if encounter an OR, add 2 to i
	- add when there are still at least two words left not including ORs
	- add DNODES only if they overlap
returns 0 if is not completed
*/
int rankWords(char **wordList, int listSize);

/*
	- add DOCNODES from each wordnode to results
	- if there is overlap with DOCNODES, just add frequencies together
	- for the second worded 
*/
void ORrank(WORDNODE *c1, WORDNODE *c2);

/*
	- if encounter an OR, add 2 to i
	- add when there are still at least two words left not including ORs
	- add DNODES only if they overlap
*/
void ANDrank(WORDNODE *c1, WORDNODE *c2);

/*
Goes through DOCUMENTNODES and prints out the docid and the 
corresponding URLs
*/
void printDOCs(char *target_dir);

/*
recursively goes through list of DOCUMENTNODES and if there is no overlap
among the DOCUMENTNODES, it returns 0
*/
int sortList();



#endif