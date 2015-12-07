/*
input: pointer to the inverted index
output: 1 if successful, 0 is false 

- saves the inverted index to a file
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <strings.h>
#include "../indexer/indexer.h"
#include "hash.h"
#include "file.h"
#include "list.h"

int saveFile(INVERTED_INDEX* index, char* filename){
	FILE *ptemp;
	int docCount;

	// open a temp file to write the index into - unsorted
	ptemp = fopen("temp.dat", "w");

	// recurse through the wordnodes
	WORDNODE *c;
	DOCUMENTNODE *d;
	for(c = index->start; c->next != NULL; c = c->next){
		docCount = 0;
		// count number of docs the word is in
		for(d = c->page; d != NULL; d = d->next)
			docCount++;
		// print the word and the wordCount to the file
		fprintf(ptemp, "%s %d ", c->word, docCount);
		// loop through the wordnode's document nodes and print them out in correct format
		for(d = c->page; d != NULL; d = d->next)
			fprintf(ptemp, "%d %d ", d->document_id, d->page_word_frequency);
		fprintf(ptemp, "\n");
	}
	fclose(ptemp);

	// sort the temp file and save the sorted index into the correct file
	char* command;
	command = malloc(sizeof(char)*(17+MAX_WORD_LEN));
	sprintf(command, "sort temp.dat > %s", filename);
	system(command);

	return 1;
}

/*
reads the index.dat file back into a new index 
	- see line by line commenting for pseudocode 

*/


INVERTED_INDEX* readFile(char *filename){
	INVERTED_INDEX *indexN;
	FILE *pfile;
	char *tempstr;
	char *word;
	int i = 0;
	int j = 0;
	int k = 0;
	int docid = 0;
	int freq = 0;
	char temp[MAX_WORD_LEN];

	// create and malloc new index, and two placeholder string variables
	indexN = malloc(sizeof(INVERTED_INDEX));
	if(indexN == NULL){
		printf("No enough memory at %s:line%d ", __FILE__, __LINE__); \
	    exit(-1); 
	}

	// make space for the line (tempstr) and indexed word (word)
	word = malloc(MAX_WORD_LEN);
	if(word == NULL){
		printf("No enough memory at %s:line%d ", __FILE__, __LINE__); \
	    exit(-1); 
	}
	bzero(word, MAX_WORD_LEN);

	tempstr = malloc(MAX_WORD_LEN);
	if(tempstr == NULL){
		printf("No enough memory at %s:line%d ", __FILE__, __LINE__); \
	    exit(-1); 
	}
	bzero(tempstr, MAX_WORD_LEN);

	// open file and read in first line
	pfile = fopen(filename, "r");

	// while going through all of the lines in the indexer
	while(fgets(tempstr, MAX_WORD_LEN, pfile) != NULL){
		i = 0;
		// while going through one line at a time
		while(tempstr[i] != '\n'){
			// extract indexed word
			j = i;
			// skip over blank spaces
			while(tempstr[i] != ' '){
				i++;
			}
			if(j == 0){
				// if at the beginning of the line, extract the word and 
				// the num of doc occurences
				strncpy(word, tempstr+j, i-j);
				word[i] = '\0';
				i++;
				j = i;
				// go to next variable in line
				while(tempstr[i] != ' '){
					i++;
				}
				i++;
			} else{
				// if not at the beginnign of the line...
				// extract the next document id
				strncpy(temp, tempstr+j, i-j);
				temp[i-j] = '\0';
				docid = atoi(temp);
				i++;
				j = i;
				while(tempstr[i] != ' ')
					i++;
				// extract the number of occurences of the word in that file
				strncpy(temp, tempstr+j, i-j);
				temp[i-j] = '\0';
				freq = atoi(temp);
				// add it to the index, and put in for loop to update the word's frequency
				for(k = 0; k < freq; k++){
					updateIndex(word, docid, indexN);
				}
				i++;
			}
		}
	}

	return indexN;
}



















