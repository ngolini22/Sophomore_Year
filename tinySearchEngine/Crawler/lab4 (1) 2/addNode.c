/*
 
The purpose of this function is to add nodes of a double linked list to contain information about each website visited in the crawler portion of the search engine, and are saved into a hash table (the dictionary). This addNode function takes in a url and crawl depth, creades a DNODE for the url, and adds it to the hash table. There are many different edge cases to consider in this dictionary, and this function takes care of them all. This dictionary is to be used later in the indexer portion of the search engine for each word in the webpages to be indexed for use in the query search. I would love to have the opportunity to explain in detail how the search engine was designed, developed, and implemented. 
 
 
 Adds DNODE depending on different edge cases:
 
    - if it is the first DNODE in the hash table
    - if it is the first DNODE in the hash[index]
    - if there is a collision in the hash[index]
 
 */

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "crawler.h"
#include "hash.h"

void addNode(char *url, int currentDepth){
    int index, index2;
    
    index = hash1(url)%(MAX_URL_LENGTH);
    
    URLNODE *u = malloc(sizeof(URLNODE));
    MALLOC_CHECK(u);
    u->visited = 0;
    u->depth = currentDepth;
    bzero(u->url, MAX_URL_LENGTH);
    strncpy(u->url, url, MAX_URL_LENGTH);
    
    if(dict->start == NULL){
        
        // initialize first URLNODE - for the first url in the url_list -- SEED_URL
        URLNODE* n = malloc(sizeof(URLNODE));
        MALLOC_CHECK(n);
        n->depth = 0;
        n->visited = 0;
        bzero(n->url, MAX_URL_LENGTH);
        strncpy(n->url, url, MAX_URL_LENGTH);
        
        // initialize first DNODE in dict[index] - for the first url in the url_list
        dict->start = dict->end = malloc(sizeof(DNODE));
        MALLOC_CHECK(dict->start);
        dict->start->prev = dict->start->next = NULL;
        dict->hash[index] = dict->start;
        dict->start->data = n;
        bzero(dict->start->key, MAX_URL_LENGTH);
        strncpy(dict->start->key, url, MAX_URL_LENGTH);
        dict->end->next = NULL;
    }
    
    if(dict->hash[index] == NULL){
        // when the hash[index] is empty, add new DNODE to the the hash[index] and make it the new dict->end
        DNODE *c = dict->end;
        dict->end = malloc(sizeof(DNODE));
        MALLOC_CHECK(dict->end);
        DNODE *d = dict->end;
        c->next = d;
        d->prev = c;
        bzero(d->key, MAX_URL_LENGTH);
        strncpy(d->key, url, MAX_URL_LENGTH);
        dict->hash[index] = d;
        d->data = u;
        d->next = NULL;
    } else{ // hash[index] is occupied, so add it to the doubly linked list of DNODES depending on edge case
        DNODE *c = dict->hash[index];
        DNODE *d = malloc(sizeof(DNODE));
        MALLOC_CHECK(d);
        while(c != NULL){ // while still in the DNODE double list
            index2 = hash1(c->key)%(MAX_URL_LENGTH);
            
            if((index != index2) && (c->next != NULL)){ // if current DNODE index is not the same as the url's index, current is now set to the first node outside of the cluster
                c = c->prev;
                DNODE *i = c->next;
                bzero(u->url, MAX_URL_LENGTH);
                strncpy(u->url, url, MAX_URL_LENGTH);
                d->next = i;
                d->prev = c;
                i->prev = d;
                c->next = d;
                strncpy(d->key, url, MAX_URL_LENGTH);
                d->data = u;
                break;
            } else if((index != index2) && (c->next == NULL)){
                // if the end of the cluster is one away from the end, add new DNODE accordingly
                c = c->prev;
                bzero(u->url, MAX_URL_LENGTH);
                strncpy(u->url, url, MAX_URL_LENGTH);
                c->next = d;
                dict->end = d;
                d->prev = c;
                strncpy(d->key, url, MAX_URL_LENGTH);
                d->data = u;
                break;
            } else if((index == index2) && (c->next == NULL) && (strcmp(url, c->key) != 0)){
                // if end of the cluster is also the end of the list, add it to the end of the cluster and the hash table
                bzero(u->url, MAX_URL_LENGTH);
                strncpy(u->url, url, MAX_URL_LENGTH);
                c->next = d;
                dict->end = d;
                d->next = NULL;
                d->prev = c;
                strncpy(d->key, url, MAX_URL_LENGTH);
                d->data = u;
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