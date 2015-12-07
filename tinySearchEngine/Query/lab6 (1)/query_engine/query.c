/*

Bugs:
- if more than one word, only show unique list


pseudocode:


1) reconstruct index 
while(user has not pressed 'c')
	1.5) ask user for words
	2) get words from command line - look for AND and OR
	3) make all the words lowercase (except for AND or OR)
	4) look at first word
		- search index for the word
		- for each word, make new WORDNODE 
	5) go through all the words from the query search
	6) rank 
	7) sort IDNODES based on max freq
	8) ask for another search

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <strings.h>
#include <ctype.h>
#include <unistd.h>
#include "../indexer/indexer.h"
#include "../utils/file.h"
#include "query.h"

QHOLDER *qholder;

int main(int argc, char* arg[]){
	int flag = 1;
	char index_file[MAX_WORD_LEN];
	char target_dir[MAX_WORD_LEN];
	struct stat sb; // used for stat call
	INVERTED_INDEX *index;

	printf("starting the program\n");
	// initialize the qholder for first time
	initializeQHOLDER();

	// initialize index
	index = (INVERTED_INDEX*)malloc(sizeof(INVERTED_INDEX)+1);
	if(index == NULL){
		printf("No enough memory at %s:line%d ", __FILE__, __LINE__); 
    	exit(-1); 
	}
	bzero(index, sizeof(INVERTED_INDEX)+1);
	index->start = index->end = NULL;

	// input arg check
	// check to see if the user inputs too many or too few args
	if(argc != 3){
		printf("You did not enter the correct number of arguments.\n");
		exit(-1);
	}

	// copy in filename of the printed index
	strcpy(index_file, arg[1]);

	// set target directory and check if it's available
	strcpy(target_dir, arg[2]);
	stat(target_dir, &sb);
	if(!S_ISDIR(sb.st_mode)){
    	printf("The directory you chose is not available.\n");
    	return 1;
	}

	// read file into new index -- now index is loaded from file
	index = readFile(index_file);

	// initialize user input variable
	char *input;
	input = malloc(sizeof(char)*(MAX_WORD_LEN+1));
	if(index == NULL){
		printf("No enough memory at %s:line%d ", __FILE__, __LINE__); 
    	exit(-1); 
	}
	bzero(input, MAX_WORD_LEN+1);

	// Main while loop used in query - see psuedocode above
	// Used to create a node for every word containing a list of the 
	//		documents the word is in. Each node will contain the 
	// 		list of DOCUMENTNODES which contain the document id and 
	//		the frequency of the word in that document. 
	int keepInWhile = 1;
	while(keepInWhile){
		// initialize the QHOLDER so it is empty for every search
		// only want words in the qholder from each different search
		flag = 0;
		// prompt user for a query search
		printf("\nTinySearch >	");
		fgets(input, MAX_WORD_LEN, stdin);
		if(strcmp(input, "c\n") == 0){
			printf("You are now exitting the program.\n");
			break;
		}
		printf("The word you are searching for is %s\n", input);

		// load input words into list of words using getNextWord...
		// create and zero word and list inputs will be written to
		int currentPosition =0;
		int i = 0;
		char **wordList;
		wordList = malloc(sizeof(char)*(MAX_WORD_LEN+1));
   		if(wordList == NULL){
   			printf("No enough memory at %s:line%d ", __FILE__, __LINE__); \
		    exit(-1); 
   		}
   		bzero(wordList, MAX_WORD_LEN);
		
		// Get a list of all the user input words to be searched.
		// The list is called wordList and after getNextWord is called,
		// 		the wordList will be loaded
		getNextWord(input, currentPosition, wordList, i);	

		// 	initialize new word node to be set at the correct 
		// 		position in the index 
		WORDNODE *wn;
		i = 0;
		while(wordList[i] != NULL){
			// if user wrote AND or OR, keep it in list, but don't add node 
			if(strcmp(wordList[i], "OR") == 0){
				printf("continuing...\n");
				i++;
				continue;
			} else if(strcmp(wordList[i], "AND") == 0){
				printf("continuing...\n");
				i++;
				continue;
			} else{
				// find the wordnode form index for the current query word
				makeLower(wordList[i]);
				wn = searchIndex(&flag, wordList[i], index);
				// this flag is for if the word was found or not. 1 is yes.
				if(flag == 0){
					printf("Word not found. Try again.\n");
					break;
				}
				// add WORDNODE to the qholder to be used later
				addWORDNODE(wn);
				i++;
			}
		}
		if(flag == 0)
			continue;
		if(rankWords(wordList, i) == 0){
			continue;
		}
		printDOCs(target_dir);

		bzero(input, MAX_WORD_LEN+1);
		initializeQHOLDER();
	}
	return 0;
}
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
*/

int rankWords(char **wordList, int listSize){
	int i = 0;
	int flag1 = 0;
	WORDNODE *c1, *c2;

	// if the list size is 1, just add word->page to qholder->results
	if(listSize == 1){
		c1 = searchQHOLDER(&flag1, wordList[i]);
		qholder->results = c1->page;
	} 

	// do all OR operations first
	while (i < listSize-1){ // use listSize-1 to end so i+1 is still in word list
		if(strcmp(wordList[i], "OR") == 0){
			c1 = searchQHOLDER(&flag1, wordList[i-1]); // get word before or
			c2 = searchQHOLDER(&flag1, wordList[i+1]); // get word after or
			ORrank(c1, c2); // ranks these two words based on the OR ranking scheme
			// once done with the OR'd words, get rid of OR to AND the rest of the words
			c1 = c2 = NULL;
		}
		i++;
	}
  	i = 0;
  	while(i < (listSize-1)){
  		if(strcmp(wordList[i+1], "OR") == 0)
  			i = i+2; // skip over OR
  		else if(strcmp(wordList[i+1], "AND") == 0){
			c1 = searchQHOLDER(&flag1, wordList[i]);
  			c2 = searchQHOLDER(&flag1, wordList[i+2]);
  			ANDrank(c1, c2); 
  			c1 = c2 = NULL;
  			i= i+2;
  			continue;
  		} else{ // if there are two words in a row that are not AND or OR
  			c1 = searchQHOLDER(&flag1, wordList[i]);
  			c2 = searchQHOLDER(&flag1, wordList[i+1]);
  			ANDrank(c1, c2); 
  			c1 = c2 = NULL;
  		}
  		i++;
  	}
  	int flag = 1;
  	flag = sortList();
  	if(flag == 0){
  		return 0;
  	}
}

int sortList(){
	DOCUMENTNODE *d1, *d2, *temp;

	temp = malloc(sizeof(DOCUMENTNODE));
	if(temp == NULL){
		printf("No enough memory at %s:line%d ", __FILE__, __LINE__); 
    	exit(-1); 
	}
	d1 = qholder->results;
	if(d1 == NULL){
		return 0;
	}
	while(d1->next != NULL){
		d2 = d1->next;
		if(d2->page_word_frequency > d1->page_word_frequency){
			temp->next = qholder->results;
			qholder->results = d1->next;
			d1->next = d2->next;
			d2->next = temp->next;
			sortList(); // resursively call until all nodes are in order
		} else{
			d1 = d1->next;
		}
	}
	return 1;
}


/*
	- if encounter an OR, add 2 to i
	- add when there are still at least two words left not including ORs
	- add DNODES only if they overlap
*/
void ANDrank(WORDNODE *c1, WORDNODE *c2){
	DOCUMENTNODE *d1, *d2;
	DOCUMENTNODE *temp1, *temp2;

	for(d1 = c1->page; d1 != NULL; d1 = d1->next){
		temp1 = malloc(sizeof(DOCUMENTNODE));
		if((temp1 == NULL)){
			printf("No enough memory at %s:line%d ", __FILE__, __LINE__); 
	    	exit(-1); 				
		}
		temp1->document_id = d1->document_id;
		temp1->page_word_frequency = d1->page_word_frequency;
		temp1->next = NULL;
		for(d2 = c2->page; d2 != NULL; d2 = d2->next){
			temp2 = malloc(sizeof(DOCUMENTNODE));
			if((temp2 == NULL)){
				printf("No enough memory at %s:line%d ", __FILE__, __LINE__); 
		    	exit(-1); 				
			}
			temp2->document_id = d2->document_id;
			temp2->page_word_frequency = d2->page_word_frequency;
			temp2->next = NULL;

			if(temp1->document_id == temp2->document_id){
				printf("doc1: %d doc2: %d\n", temp1->document_id, temp2->document_id);
				// add to results
				if(qholder->results == NULL){	
					qholder->results = temp1;
					printf("doc being added: %d\n", temp2->document_id);
				} else{
					DOCUMENTNODE *c = qholder->results;
					while(c->next != NULL){
						c = c->next;
					}
					c->next = temp1;
				}
			} 
			// else continue
		}

	}


}

/*
set rankings for the query for an OR call

- add DOCUMENTNODES from each wordnode to results
- if there is overlap with DOCNODES, just add frequencies together
- for the second worded 
*/

void ORrank(WORDNODE *c1, WORDNODE *c2){
	DOCUMENTNODE *d1, *d2;
	DOCUMENTNODE *temp;
	int found = 0;


	// load in DOCUMENTNODES from first word into qholder results
	for(d1 = c1->page; d1 != NULL; d1 = d1->next){
		temp = malloc(sizeof(DOCUMENTNODE));
		if((temp == NULL)){
			printf("No enough memory at %s:line%d ", __FILE__, __LINE__); 
	    	exit(-1); 				
		}
		temp->document_id = d1->document_id;
		temp->page_word_frequency = d1->page_word_frequency;
		temp->next = NULL;

		if(qholder->results == NULL){	
			qholder->results = temp;
			// temp->next = NULL;
		} else{
			DOCUMENTNODE *c = qholder->results;
			while(c->next != NULL){
				c = c->next;
			}
			c->next = temp;
		}
	}
	// load in second WORDNODE's DOCUMENTNODES into qholder->results
	// if there is a double DOCUMENTNODE, only update the frequency in 
	// 		the qholder DOCUMENTNODE for that document
	
	for(d2 = c2->page; d2 != NULL; d2 = d2->next){
		temp = malloc(sizeof(DOCUMENTNODE));
		if((temp == NULL)){
			printf("No enough memory at %s:line%d ", __FILE__, __LINE__); 
	    	exit(-1); 				
		}
		temp->document_id = d2->document_id;
		temp->page_word_frequency = d2->page_word_frequency;
		temp->next = NULL;

		DOCUMENTNODE *c = qholder->results;
		while(c != NULL){ // go to end of results list or stop if there's a double
			found = 0;
			// if there is a duplicate DOCUMENTNODE, stop there
			if(c->document_id == d2->document_id){
				c->page_word_frequency = c->page_word_frequency + d2->page_word_frequency;
				found = 1;
				break;
			}else if (c->next == NULL){
				c->next = malloc(sizeof(DOCUMENTNODE));
				if((c->next == NULL)){
					printf("No enough memory at %s:line%d ", __FILE__, __LINE__); 
			    	exit(-1); 				
				}	
				break;
			}else {
				c = c->next;
			}
		}
		// if there was a dublicate DOCUMENTNODE, add frequency of 
		if(found != 1){
			c->next = temp;
			temp->next = NULL;
		}
	}

}

/*
Searches the qholder and returns the WORDNODE for a given word from the qholder

*/
WORDNODE *searchQHOLDER(int *flag1, char *word){
	WORDNODE *c = qholder->start;
	*flag1 = 0;
	while(c != NULL){
		if(strcmp(c->word, word) == 0){
			*flag1 = 1;
			break;
		} else
			c = c->next;
	}
	if(strcmp(qholder->end->word, word) == 0){
		*flag1 = 1;
	}
	if(*flag1 == 1){
		printf("your word was found in qholder\n");
	} else{
		printf("Your word was not found in qholder.\n");
	}
	return c; // return the word node from 
}

/*
function to make query all lowercase.

Loops through all chars in the user input string and switch 
	them to lowercase
*/
void makeLower(char *str){
	int len; 
	int i;
	len = strlen(str);
	for(i = 0; i < len; i++){
		str[i] = tolower(str[i]);

	}
}


/*
wn is set to index->start before function call.
Starts at index->start, and runs until it found the word. 
If if doesn't find the word, return 0. If it does find the word, 
	return 1.

*/
WORDNODE *searchIndex(int *flag, char *str, INVERTED_INDEX *index){
	WORDNODE *c = index->start;
	*flag = 0;
	// while in the index
	while(c->next != NULL){
		if(strcmp(c->word, str) == 0){ // if the wordnode matches the word
			*flag = 1; // set found flag
			break;
		}
		c = c->next;
	}
	if(strcmp(index->end->word, str) == 0){ // if the word is the last word of the index
		*flag = 1;
	}
	if(*flag == 1){
		printf("your word was found\n");
	} else{
		printf("Your word was not found.\n");
	}
	return c; // return the word node from 

}

/*
Takes the int doc_id and makes it a string
Open file with the doc_id name
Extract first line (which should be the url)
Returnt the url
*/

char *getURL(int doc_id, char *target_dir){
	FILE *pfile;
	char *filename;
	char *url;
	
	// initialize filename and url vars
	filename = malloc(sizeof(int)+1);
	if(filename == NULL){
		printf("No enough memory at %s:line%d ", __FILE__, __LINE__); 
    	exit(-1); 		
	}
	bzero(filename, sizeof(int)+1);

	url = malloc(sizeof(MAX_WORD_LEN));
	if(url == NULL){
		printf("No enough memory at %s:line%d ", __FILE__, __LINE__); 
    	exit(-1); 		
	}
	bzero(url, MAX_WORD_LEN);

	// convert doc id to a string called filename
	sprintf(filename, "%d", doc_id);
	
	// go into target directory where the data is	
	chdir(target_dir);

	// get first line of the file which is the urlname
	pfile = fopen(filename, "r");
	fscanf(pfile, "%s", url);
	return url;
}

void initializeQHOLDER(void){

	// initialize QHOLDER to hold QNODES
	qholder = (QHOLDER *)malloc(sizeof(QHOLDER)+1);
	if(qholder == NULL){
		printf("not enough memory for qholder.\n");
		exit(-1);
	}
	bzero(qholder, sizeof(QHOLDER)+1);
	qholder->start = qholder->end = NULL;
	qholder->results = NULL;

}

/*
Adds a word node to the qholder list of wordnodes
- copy current wordnode into a temp wordnode
- if the qholder wordnode list is empty, set qholder start and end pointers
- else find the end of the list in qholder, and add temp to the end of the list
*/

void addWORDNODE(WORDNODE *wn){
	
	// if qholder is empty, put first word in
	WORDNODE *temp = malloc(sizeof(WORDNODE));
	if(temp == NULL){
		printf("not enough memory for qholder.\n");
		exit(-1);		
	}
	// initialize new wordnode with contents from the same wordnode from index
	bzero(temp->word, MAX_WORD_LEN);
	strcpy(temp->word, wn->word);
	temp->page = wn->page;

	// if this is the first wordnode in qholder, initialize start and end
	if(qholder->start == NULL){
		qholder->start = temp;
		qholder->end = temp;
	} else{ // if it is not the first node in qholder, add new wordnode to the end of the list
		WORDNODE *c = qholder->start;
		// bring temp to last WORDNODE
		while(c->next != NULL){
			c = c->next;
		}
		// set wn to the next place in qholder and set qholder end to the new 
		c->next = temp;
		qholder->end = temp;
		temp->next = NULL;
	}
}

/*
Parses user input into seperate words to account for multiple word searches
	- simplified version of getNextWordinHTMLDocument
	- the difference is that this word parser just looks for
	the space characters and parses there, it does not worry about
	any non alpha chars except for spaces
*/
int getNextWord(const char* input, int currentPosition, char **wordList, int i){
	const char *p1, *p2; // pointer to start and end of word for parsing
	char *word;
	word = malloc(sizeof(char)*(MAX_WORD_LEN+1));
 	if(word == NULL){
		printf("No enough memory at %s:line%d ", __FILE__, __LINE__); \
	    exit(-1); 
	} 
	bzero(word, MAX_WORD_LEN+1);
	
	// find first letter char
	while(input[currentPosition] && !isalpha(input[currentPosition]))
		currentPosition++;
	// set beginning of word pointer
	p1 = &(input[currentPosition]);
	// travel to the end of the word
	while(input[currentPosition] && isalpha(input[currentPosition])){
		currentPosition++;
	}
	// if we've gone too far, go back one
	if(!isalpha(input[currentPosition]))
		currentPosition--;
	// set end pointer to end of word
	p2 = &(input[currentPosition+1]);
	// copy in the word from the pointers
	strncpy(word, p1, (p2-p1));
	// go to next char after word	
	currentPosition++;
	
	// if the next character is also null, it is the end of the string
	if(input[currentPosition+1] == 0){
		// printf("word = %s\n", word);
		wordList[i] = word;
		// printf("word[0] = %s\n", wordList[0]);
		return 0;
	} else{ // load word into the wordlist, and get the next word by recursively calling this func again
		// printf("word = %s\n", word);
		wordList[i] = word;
		// printf("word[%d] = %s\n", i, wordList[i]);
		i++;
		return getNextWord(input, currentPosition, wordList, i);
	}
}

void printDOCs(char *target_dir){
	// GETS URL FOR EACH DOCNODE - FOR PRINTING THE OUTPUT

	DOCUMENTNODE *dn; 
	for(dn = qholder->results; dn != NULL; dn = dn->next){
		char *url = malloc(sizeof(MAX_WORD_LEN));
		if(url == NULL){
			printf("No enough memory at %s:line%d ", __FILE__, __LINE__); 
	   		exit(-1); 			
		}
		bzero(url, MAX_WORD_LEN);
		// if(qholder->start == NULL);
		url = getURL(dn->document_id, target_dir);
		printf("DOCUMENT ID: %d URL: %s\n", dn->document_id, url);
	}
}
