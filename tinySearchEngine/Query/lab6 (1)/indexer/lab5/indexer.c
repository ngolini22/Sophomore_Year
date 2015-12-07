/*
Main indexer function:

main for loop:

for all of the files in storedDocuments{
	- load the document into a string;
	- get the document identifier (in string format)
	- set current position to 0
	- while current position = getNextWordFromHTMLDocument(loadedDoc, word, position)
		- update the index with the word and number of occurences
}
*/

/*
Bugs:
- check to see if index is correct

- check to see if all words make it into index
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "indexer.h"
#include "list.h"
#include "file.h"
#include "document.h"

int main(int argc, char* arg[]){
	char targetDir[MAX_WORD_LEN]; // directory to get files from 
	char filename[MAX_WORD_LEN]; // filename for results of indexer
	char result_file[MAX_WORD_LEN];
	int n; // counter for scandir
	int currentPosition; // keeps current location in string when parsing file
	struct dirent **namelist; // creates list of filenames from target directory
	struct stat sb; // used for stat call
	INVERTED_INDEX *index;

	// initialize inverted index
	index = (INVERTED_INDEX*)malloc(sizeof(INVERTED_INDEX)+1);
	if(index == NULL){
		printf("No enough memory at %s:line%d ", __FILE__, __LINE__); 
    	exit(-1); 
	}
	bzero(index, sizeof(INVERTED_INDEX)+1);
	index->start = index->end = NULL;
	// index initialized


	// if the number of args does not match the two possible ways of calling this program
	if(!((argc == 3) || (argc == 4))){
		printf("You did not enter the correct number of arguments.\n");
		return 1;
	}

	// if args is 3, set the arguments to variables and check if they are valid
	if(argc == 3 || argc == 4){
		// set target directory and check if it's available
   		strcpy(targetDir, arg[1]);
   		stat(targetDir, &sb);
    	if(!S_ISDIR(sb.st_mode)){
        	printf("The directory you chose is not available.\n");
        	return 1;
    	}
		strcpy(filename, arg[2]);
	}

	// scan directory into  the namelist
	n = scandir("./data", &namelist, 0, alphasort);

 	char* word;
 	word = malloc(MAX_WORD_LEN+1);
 	if(word == NULL){
		printf("No enough memory at %s:line%d ", __FILE__, __LINE__); \
	    exit(-1); 
	} 
	bzero(word, MAX_WORD_LEN+1);
	currentPosition = 0;
	int len;
	char* loadedDoc;
	int id;
	int i;

	// follow pseudocode from top of file
	for(i = 2; i < (n-2); i++){
		// printf("current filename: %s\n", namelist[i]->d_name);
	 	loadedDoc = loadDocument(namelist[i]->d_name, targetDir);
	 	id = getDocumentId(namelist[i]->d_name);
	 	// printf("docid now being considered: %d\n", id);
	 	currentPosition = 0;
		while((currentPosition = getNextWordFromHTMLDocument(loadedDoc, word, currentPosition))){
			len = makeWord(word);
			updateIndex(word, id, index);
			bzero(word, len+1);
		}
	}

	// save the index into a file
	saveFile(index, filename);

	// write the file with the index back into a new index

	if(argc == 4){
		// if the user asks to make a new file by reading the file with the index
		// back into a new index, and writing the new index to a new file.
		// happens when the user has 4 input arguments
		INVERTED_INDEX *new_index;
		strcpy(result_file, arg[3]);
		new_index = readFile(filename);
		saveFile(new_index, result_file);
	}



	// cleanUp(index);
	return 0;
}



void cleanUp(INVERTED_INDEX *index){
	WORDNODE *wn;
	char *tempWord;
	wn = index->start;

	free(wn->word);

	// tempWord = malloc(sizeof(MAX_WORD_LEN+1));
	// if(tempWord == NULL){
	// 	printf("Not enough storage for tempWord.");
	// }
	// bzero(tempWord, MAX_WORD_LEN);

	// while(wn->next != NULL){
	// 	printf("cleaning...");
	// 	strcpy(tempWord, wn->word);
	// 	wn = wn->next;
	// 	free(tempWord);
	// }










}































