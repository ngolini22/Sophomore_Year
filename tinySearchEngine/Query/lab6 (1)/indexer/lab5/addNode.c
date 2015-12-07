// /*
//  Adds DNODE depending on different edge cases:
 
//     - if it is the first DNODE in the hash table
//     - if it is the first DNODE in the hash[index]
//     - if there is a collision in the hash[index]
 
//  GO THROUGH URL BY URL AND FIND OUT WHY COLLISION ADDITION TO THE LIST IS OPPOSITE FOR index[4] and index[1,6]. ONE PUTS THE INSERTED DNODE TO PREV AND THE OTHERS INSERT DNODE TO NEXT RESPECTIVELY
//  GOTTA FIND OUT WHERE THIS IS MESSING UP
 
//  - not adding last url to DNODE for some reason
 
//  - also URLNODES are messed up see all caps comment above
 
//  */

// #include <sys/stat.h>
// #include <sys/types.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <strings.h>
// #include "hash.h"
// #include "indexer.h"
// #include "list.h"

// /*
// typedef struct _WordNode{
//     struct _WordNode *prev;     // pointer to the previous word
//     struct _WordNode *next;     // pointer to the next word
//     char word[MAX_WORD_LEN];    // the word
//     DocumentNode *page;         // pointer to the first element of the page list
// } __WordNode;

// */
// void addNode(char *word, INVERTED_INDEX *index, int docId){
//     int hashIndex, hashIndex2;

//     hashIndex = hash1(word)%(MAX_WORD_LEN);
    
//     DOCUMENTNODE *dn = malloc(sizeof(DOCUMENTNODE));
//     if(dn == NULL){
//         printf("not enough space for new node.\n");
//         exit(1);
//     }
//     dn->next = NULL;
//     dn->document_id = docId;
//     dn->page_word_frequency = 1;

//     if(index->start == NULL){
//         // initialize first WORDNODE - for the first word in the index
//         WORDNODE* wn = malloc(sizeof(WORDNODE));
//         if(wn == NULL){
//             printf("not enough space for new node.\n");
//             exit(1);
//         }        wn->page = dn;
//         bzero(wn->word, MAX_WORD_LEN);
//         strncpy(wn->word, word, MAX_WORD_LEN);
        
//         // initialize first WORDNODE in index[index]
//         index->start = index->end = wn;
//         index->start->prev = index->start->next = NULL;
//         index->hash[hashIndex] = index->start;
//         index->end->next = NULL;
//         wn->page = dn;

//     }
    
//     if(index->hash[hashIndex] == NULL){
//         // when the hash[index] is empty, add new WORDNODE to the the hash[index] and make it the new index->end
//         WORDNODE *c = index->end;
//         index->end = malloc(sizeof(WORDNODE));
//         if(index->end == NULL){
//             printf("not enough space for new node.\n");
//             exit(1);
//         }        
//         WORDNODE *d = index->end;
//         c->next = d;
//         d->prev = c;
//         bzero(d->word, MAX_WORD_LEN);
//         strncpy(d->word, word, MAX_WORD_LEN);
//         index->hash[hashIndex] = d;
//         d->page = dn;
//         d->next = NULL;
//     } else{ // hash[index] is occupied, so add it to the doubly linked list of DNODES depending on edge case
//         WORDNODE *c = index->hash[hashIndex];
//         WORDNODE *d = malloc(sizeof(WORDNODE));
//         if(d == NULL){
//             printf("not enough space for new node.\n");
//             exit(1);
//         }        while(c != NULL){ // while still in the DNODE double list
//             hashIndex2 = hash1(c->word)%(MAX_WORD_LEN);
            
//             if((hashIndex != hashIndex2) && (c->next != NULL)){ // if current DNODE index is not the same as the url's index, current is now set to the first node outside of the cluster
//                 c = c->prev;
//                 WORDNODE *i = c->next;
//                 d->next = i;
//                 d->prev = c;
//                 i->prev = d;
//                 c->next = d;
//                 strncpy(d->word, word, MAX_WORD_LEN);
//                 d->page = dn;
//                 break;
//             } else if((hashIndex != hashIndex2) && (c->next == NULL)){
//                 // if the end of the cluster is one away from the end, add new WORDN accordingly
//                 c = c->prev;
//                 c->next = d;
//                 index->end = d;
//                 d->prev = c;
//                 strncpy(d->word, word, MAX_WORD_LEN);
//                 d->page = dn;
//                 break;
//             } else if((hashIndex == hashIndex2) && (c->next == NULL) && (strcmp(word, c->word) != 0)){
//                 // if end of the cluster is also the end of the list, add it to the end of the cluster and the hash table
//                 // bzero(u->url, MAX_URL_LENGTH);
//                 // strncpy(u->url, url, MAX_URL_LENGTH);
//                 c->next = d;
//                 index->end = d;
//                 d->next = NULL;
//                 d->prev = c;
//                 strncpy(d->word, word, MAX_WORD_LEN);
//                 d->page = dn;
//                 break;
//             }else{ // if still in the collision cluster of DNODES with the same hash index
//                 c = c->next;
//                 if(c == NULL){
//                     break;
//                 }
//             }
//         }
//     }
// }