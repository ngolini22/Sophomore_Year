#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_

#include "../indexer/indexer.h"

/*
loadDocument()
input: the name of the file to be loaded
output: string containing the loaded document

- Loads HTML doc from a file
- Challenges:
	- use scandir to scan all the files from a single directory
*/
 
char* loadDocument(char* fileName, char* targetDir);

/*
getDocumentId()
input: name of the file
output: integer identifier


- Generates a document identifier from the name fo the file
- Saves the files using unique identifiers as names
*/

int getDocumentId(char* fileName);

/*
getNextWordFromHTMLDocument()
input: 
1) the string containing the loaded document 
2) empty string that is used to return the result 
3) the current position in the string from which the parsing is started/resumed
output: ??? unclear. Possibly current position in string

- parses the string containing the loaded document, skipping the HTML tas and returning the next word
*/

// int getNextWordFromHTMLDocument(char* loadedDocument, char* word, int currentPosition);
int getNextWordFromHTMLDocument(const char* doc, char *word, int pos);

// reallocates space for word so it isn't hardcoded
int makeWord(char *word);



#endif