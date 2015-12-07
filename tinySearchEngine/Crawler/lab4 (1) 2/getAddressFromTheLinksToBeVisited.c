/*
 
 Finds the next url to visit by traversing through the DNODES. Finds first DNODE with URLNODE who's visited = 0
 
 */


#include <stdio.h>
#include <stdlib.h>
#include "crawler.h"


char *getAddressFromTheLinksToBeVisited(int *depth){
    int flag = 0;
    DNODE *d;
    URLNODE *u;
    for(d = dict->start; d != NULL; d = d->next){ // look through all of the urls
        u = d->data;
        if(u->visited == 0){ // if the url's visited flag is on
            *depth = u->depth;
            flag = 1; // set flag to extract url from URLNODE
            break;
        }
    }
    if(flag == 0)
        return NULL; // if all URLNODES have been visited, return NULL
    else{
        char *nextURL = malloc(MAX_URL_LENGTH);
        nextURL = u->url;
        return nextURL;
    }
}