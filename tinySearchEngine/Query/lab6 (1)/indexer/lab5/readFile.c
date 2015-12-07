/*
reads the index.dat file back into a new index

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <strings.h>
#include "list.h"
#include "indexer.h"


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

	while(fgets(tempstr, MAX_WORD_LEN, pfile) != NULL){
		i = 0;
		while(tempstr[i] != '\n'){
			// extract indexed word
			j = i;
			while(tempstr[i] != ' '){
				i++;
			}
			if(j == 0){
				strncpy(word, tempstr+j, i-j);
				word[i] = '\0';
				i++;
				j = i;
				while(tempstr[i] != ' '){
					i++;
				}
				i++;
			} else{
				strncpy(temp, tempstr+j, i-j);
				temp[i-j] = '\0';
				docid = atoi(temp);
				i++;
				j = i;
				while(tempstr[i] != ' ')
					i++;
				strncpy(temp, tempstr+j, i-j);
				temp[i-j] = '\0';
				freq = atoi(temp);
				for(k = 0; k < freq; k++){
					updateIndex(word, docid, indexN);
				}
				i++;
			}
		}
	}

	return indexN;
}