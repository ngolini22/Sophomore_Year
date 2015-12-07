// /*
// input: 
// 1) the string containing the loaded document 
// 2) empty string that is used to return the result 
// 3) the current position in the string from which the parsing is started/resumed
// output: ??? unclear. Possibly current position in string

// - parses the string containing the loaded document, skipping the HTML tas and returning the next word
// */

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>
// // #include "indexer.h"
// // int getNextWordFromHTMLDocument(char* loadedDocument, char *word, int currentPosition){
// int getNextWordFromHTMLDocument(const char* loadedDocument, char *word, int currentPosition){
// 	const char* p1; // start of new word
// 	const char* p2; // end of new word

// 	// if(currentPosition++ == EOF){
// 	// 	return 0;
// 	// }

// 	if(currentPosition == 0){
// 		while(loadedDocument[currentPosition] != '<'){
// 			currentPosition++;
// 		}
// 	}
// 	// skip all non letters and if you get to a '<', skip to the end of the tag '>'
// 	// printf("current char: %c\n", loadedDocument[currentPosition]);

// 	while(loadedDocument[currentPosition] && !isalpha(loadedDocument[currentPosition])){
// 		// printf("in while\n");
// 		if(loadedDocument[currentPosition] == '<'){ // once you get to the beginning of a flag
// 			p2 = strchr(&loadedDocument[currentPosition], '>');
// 			if(!p2){
// 				return -1;
// 			}
// 			currentPosition = p2 - loadedDocument;
// 			continue;
// 		}
// 		currentPosition++;
// 		if(loadedDocument[currentPosition] == EOF){
// 			return 0;
// 		}
// 	}

// 	// after this while loop, currentPosition is set to '>'
// 	// set beginning of word at currentPosition
// 	p1 = &(loadedDocument[currentPosition]);
// 	// step through word until no more letters - will give end of the word
// 	while(loadedDocument[currentPosition] && isalpha(loadedDocument[currentPosition])){
// 		currentPosition++;
// 	}
// 	// step back if went too far
// 	if(!isalpha(loadedDocument[currentPosition]))
// 		currentPosition--;
// 	// current position should be at end of word
// 	p2 = &(loadedDocument[currentPosition+1]);
	
// 	// copy the new word from p1 to p2
// 	strncpy(word, p1, (p2-p1));

//  	currentPosition++;
// 	if(!(loadedDocument[currentPosition] && !isalpha(loadedDocument[currentPosition]))){
// 		return 0;
// 	}
// 	return currentPosition;
// }
// /*
// reallocates space for word so it isn't hardcoded
// input - word - to be updated to a new size
// output - string length - to be used
// */
// int makeWord(char *word){
// 	int len = 0;
// 	char *word1; // placeholder word
// 	len = strlen(word);
// 	word1 = malloc(sizeof(char)*(len+1)); // malloced to the correct size
// 	if(word1 == NULL){
// 		printf("No enough memory at %s:line%d ", __FILE__, __LINE__); 
//    		exit(-1); 	
// 	}
// 	bzero(word1, len+1); 
// 	strcpy(word1, word);
	
// 	bzero(word, len+1);
// 	strcpy(word, word1); // copy word1 to word to get word1 malloced size
// 	return len;
// }

















