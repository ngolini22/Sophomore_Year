/*
input: 
1) string containing the word 
2) identifier of the document
3) inverted index
output: 1 if successful, 0 if not

- updates the data structure containing the index



PSEUDOCODE:

- check uniqueness of word using *checkUniqueness()*
- if unique
	- create new WORDNODE
	- create new DOCUMENTNODE
	- hash into hash table - call *addNode()*
- else 
	- create new DOCUMENTNODE
	- find correct WORDNODE
	- add new DOCUMENTNODE to WORDNODE at end of the list
*/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "indexer.h"
#include "hash.h"
#include "list.h"


int updateIndex(char* word, int documentId, INVERTED_INDEX* index){
	int unique;
	int i;

	// make words lowercase
	for(i = 0; word[i]; i++){
		word[i] = tolower(word[i]);
	}
	unique = checkUniqueness(word, index);
	if(unique == 1){ // if the word is unique, add new WORD and DOCUMENTNODE
		addNode(word, index, documentId);
		// printf("added new unique node at hash: %d\n", hashIndex);
	} else{
		updateFreq(word, documentId, index);
	}
	return 1;
}

/*
Finds the correct DOCUMENTNODE for the correct WORDNODE and updates the frequency. 
If it is from a new document, create a new DOCUMENTNODE and place it at the end of the node list
*/

void updateFreq(char* word, int documentId, INVERTED_INDEX *index){
	int flag = 1;
	WORDNODE *wn = index->start;
	DOCUMENTNODE *dn;
	while(flag != 0){ // gp through all word nodes to find the right onw
		if(strcmp(wn->word, word)==0){ // once you get to the correct word nod
			for(dn = wn->page; dn != NULL; dn = dn->next){ // go through doc nodes to update frequency for the correct doc
				if(dn->document_id == documentId){ // if at the correct doc to update
					dn->page_word_frequency++;
					flag = 2;
					break;
				}
			}
			if(flag == 2)
				break;
			else{ 
				// if didn't find the correct doc id, make a new one and add to end of list
				DOCUMENTNODE *dnNEW = malloc(sizeof(DOCUMENTNODE));
				
				for(dn = wn->page; dn->next != NULL; dn = dn->next){} // get to end of list to add new DOCUMENTNODE

				dn->next = malloc(sizeof(DOCUMENTNODE));
				if(dn->next == NULL){
					printf("not enough space for new node.\n");
	    			exit(1);
				}
				dn->next = dnNEW;
				dnNEW->next = NULL;

				dnNEW->page_word_frequency = 1;
				dnNEW->document_id = documentId;
				flag = 0;
			}
		} else{
			if(wn->next == NULL)
				flag = 0;
			wn = wn->next;
		}
	}
}

// /*
//  Checks to see if url is unique
 
//  start at first DNODE
 
//  input - url_list[i]
//  output- 1 if unique, 0 if not unique
 
//  */

int checkUniqueness(char *word, INVERTED_INDEX *index){
    int hashIndex, hashIndex2;
    
    hashIndex = hash1(word)%(MAX_WORD_LEN);
    if(index->hash[hashIndex] == NULL){ // if there are no DNODES in the hash table at the gievn index, it is unique
        return 1;
    } else {
        WORDNODE *c = index->hash[hashIndex]; // search through collision cluster (DNODES at the same hash index
        while(c != NULL){ // while still in the DNODE list
            hashIndex2 = hash1(c->word)%(MAX_WORD_LEN); // rehash next DNODE to test if still in collision cluster
            if(hashIndex != hashIndex2){ // exit if you've checked all DNODES in the cluster
                return 1;
            }
            if(strcmp(word, c->word) == 0){ // if url is in current, it is not unique
                return 0;
            } else { // go to next DNODE to check
                c = c->next;
            }
        }
    }
    return 1; // if get to end of list and haven't found
}


/*
 Adds DNODE depending on different edge cases:
 
    - if it is the first DNODE in the hash table
    - if it is the first DNODE in the hash[index]
    - if there is a collision in the hash[index]
 
 GO THROUGH URL BY URL AND FIND OUT WHY COLLISION ADDITION TO THE LIST IS OPPOSITE FOR index[4] and index[1,6]. ONE PUTS THE INSERTED DNODE TO PREV AND THE OTHERS INSERT DNODE TO NEXT RESPECTIVELY
 GOTTA FIND OUT WHERE THIS IS MESSING UP
 
 - not adding last url to DNODE for some reason
 
 - also URLNODES are messed up see all caps comment above
 
 */

/*
typedef struct _WordNode{
    struct _WordNode *prev;     // pointer to the previous word
    struct _WordNode *next;     // pointer to the next word
    char word[MAX_WORD_LEN];    // the word
    DocumentNode *page;         // pointer to the first element of the page list
} __WordNode;

*/

void addNode(char *word, INVERTED_INDEX *index, int docId){
    int hashIndex, hashIndex2;

    hashIndex = hash1(word)%(MAX_WORD_LEN);
    
    DOCUMENTNODE *dn = malloc(sizeof(DOCUMENTNODE));
    if(dn == NULL){
        printf("not enough space for new node.\n");
        exit(1);
    }
    dn->next = NULL;
    dn->document_id = docId;
    dn->page_word_frequency = 1;

    if(index->start == NULL){
        // initialize first WORDNODE - for the first word in the index
        WORDNODE* wn = malloc(sizeof(WORDNODE));
        if(wn == NULL){
            printf("not enough space for new node.\n");
            exit(1);
        }        wn->page = dn;
        bzero(wn->word, MAX_WORD_LEN);
        strncpy(wn->word, word, MAX_WORD_LEN);
        
        // initialize first WORDNODE in index[index]
        index->start = index->end = wn;
        index->start->prev = index->start->next = NULL;
        index->hash[hashIndex] = index->start;
        index->end->next = NULL;
        wn->page = dn;

    }
    
    if(index->hash[hashIndex] == NULL){
        // when the hash[index] is empty, add new WORDNODE to the the hash[index] and make it the new index->end
        WORDNODE *c = index->end;
        index->end = malloc(sizeof(WORDNODE));
        if(index->end == NULL){
            printf("not enough space for new node.\n");
            exit(1);
        }        
        WORDNODE *d = index->end;
        c->next = d;
        d->prev = c;
        bzero(d->word, MAX_WORD_LEN);
        strncpy(d->word, word, MAX_WORD_LEN);
        index->hash[hashIndex] = d;
        d->page = dn;
        d->next = NULL;
    } else{ // hash[index] is occupied, so add it to the doubly linked list of DNODES depending on edge case
        WORDNODE *c = index->hash[hashIndex];
        WORDNODE *d = malloc(sizeof(WORDNODE));
        if(d == NULL){
            printf("not enough space for new node.\n");
            exit(1);
        }        while(c != NULL){ // while still in the DNODE double list
            hashIndex2 = hash1(c->word)%(MAX_WORD_LEN);
            
            if((hashIndex != hashIndex2) && (c->next != NULL)){ // if current DNODE index is not the same as the url's index, current is now set to the first node outside of the cluster
                c = c->prev;
                WORDNODE *i = c->next;
                d->next = i;
                d->prev = c;
                i->prev = d;
                c->next = d;
                strncpy(d->word, word, MAX_WORD_LEN);
                d->page = dn;
                break;
            } else if((hashIndex != hashIndex2) && (c->next == NULL)){
                // if the end of the cluster is one away from the end, add new WORDN accordingly
                c = c->prev;
                c->next = d;
                index->end = d;
                d->prev = c;
                strncpy(d->word, word, MAX_WORD_LEN);
                d->page = dn;
                break;
            } else if((hashIndex == hashIndex2) && (c->next == NULL) && (strcmp(word, c->word) != 0)){
                // if end of the cluster is also the end of the list, add it to the end of the cluster and the hash table
                // bzero(u->url, MAX_URL_LENGTH);
                // strncpy(u->url, url, MAX_URL_LENGTH);
                c->next = d;
                index->end = d;
                d->next = NULL;
                d->prev = c;
                strncpy(d->word, word, MAX_WORD_LEN);
                d->page = dn;
                break;
            }else{ // if still in the collision cluster of DNODES with the same hash index
                c = c->next;
                if(c == NULL){
                    break;
                }
            }
        }
    }
}