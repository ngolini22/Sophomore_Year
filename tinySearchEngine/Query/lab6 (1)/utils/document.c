




/*
input: the name of the file to be loaded
output: string containing the loaded document

- Loads HTML doc from a file
- Challenges:
	- use scandir to scan all the files from a single directory

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include "../indexer/indexer.h"
#include "document.h"

char* loadDocument(char* fileName, char* targetDir){
	FILE *pfile;
	long lSize;
	char* buffer;
	size_t result;

	chdir(targetDir);
	// checks if file is null - if so, exit program
	pfile = fopen(fileName, "r");
	if(pfile == NULL){
		printf("The file was not created correctly.\n");
		exit(1);
	}
	// gets size of file to malloc buffer
	fseek(pfile, 0, SEEK_END);
    lSize = ftell(pfile);
    rewind(pfile);
    // make buffer the size of the file and load file contents into buffer
    buffer = (char *)malloc(sizeof(char)*(lSize+1));
    if(buffer == NULL){
  		printf("No enough memory at %s:line%d ", __FILE__, __LINE__); \
	    exit(-1); 
	}

    bzero(buffer, sizeof(char)*(lSize+1));

    result = fread(buffer, sizeof(char), lSize, pfile);
    if(result != lSize){
    	printf("Reading Error.\n");
    	exit(1);
    }
    fclose(pfile);
    chdir("..");

    return buffer;
}


/*
input: name of the file
output: integer identifier


- Generates a document identifier from the name fo the file
- Saves the files using unique identifiers as names
*/

int getDocumentId(char* fileName){
	int id;
	id = atoi(fileName);
	return id;
}


/*
input: 
1) the string containing the loaded document 
2) empty string that is used to return the result 
3) the current position in the string from which the parsing is started/resumed
output: ??? unclear. Possibly current position in string

- parses the string containing the loaded document, skipping the HTML tas and returning the next word
*/


// #include "indexer.h"
// int getNextWordFromHTMLDocument(char* loadedDocument, char *word, int currentPosition){
int getNextWordFromHTMLDocument(const char* loadedDocument, char *word, int currentPosition){
	const char* p1; // start of new word
	const char* p2; // end of new word

	// if(currentPosition++ == EOF){
	// 	return 0;
	// }

	if(currentPosition == 0){
		if(strchr(loadedDocument, '<') != NULL){
			while(loadedDocument[currentPosition] != '<'){
				currentPosition++;
			}
		}
	}
	// skip all non letters and if you get to a '<', skip to the end of the tag '>'
	// printf("current char: %c\n", loadedDocument[currentPosition]);

	while(loadedDocument[currentPosition] && !isalpha(loadedDocument[currentPosition])){
		// printf("in while\n");
		if(loadedDocument[currentPosition] == '<'){ // once you get to the beginning of a flag
			p2 = strchr(&loadedDocument[currentPosition], '>');
			if(!p2){
				return -1;
			}
			currentPosition = p2 - loadedDocument;
			continue;
		}
		currentPosition++;
		if(loadedDocument[currentPosition] == EOF){
			return 0;
		}
	}

	// after this while loop, currentPosition is set to '>'
	// set beginning of word at currentPosition
	p1 = &(loadedDocument[currentPosition]);
	// step through word until no more letters - will give end of the word
	while(loadedDocument[currentPosition] && isalpha(loadedDocument[currentPosition])){
		currentPosition++;
	}
	// step back if went too far
	if(!isalpha(loadedDocument[currentPosition]))
		currentPosition--;
	// current position should be at end of word
	p2 = &(loadedDocument[currentPosition+1]);
	
	// copy the new word from p1 to p2
	strncpy(word, p1, (p2-p1));

 	currentPosition++;
	if(!(loadedDocument[currentPosition] && !isalpha(loadedDocument[currentPosition]))){
		return 0;
	}
	return currentPosition;
}
/*
reallocates space for word so it isn't hardcoded
input - word - to be updated to a new size
output - string length - to be used
*/
int makeWord(char *word){
	int len = 0;
	char *word1; // placeholder word
	len = strlen(word);
	word1 = malloc(sizeof(char)*(len+1)); // malloced to the correct size
	if(word1 == NULL){
		printf("No enough memory at %s:line%d ", __FILE__, __LINE__); \
   		exit(-1); 	
	}
	bzero(word1, len+1); 
	strcpy(word1, word);
	
	bzero(word, len+1);
	strcpy(word, word1); // copy word1 to word to get word1 malloced size
	return len;
}