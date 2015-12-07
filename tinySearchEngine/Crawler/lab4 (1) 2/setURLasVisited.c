/*
 Searches DNODES to find the one with the same url
 
 Sets URL that was jsut visited as visited
 
 hash url
 
 pointer to DNODE->data->visited
 
 make visited = 1
 
 */

#include <stdio.h>
#include <string.h>
#include "crawler.h"

void setURLasVisited(char *url){
    int flag = 1;
    URLNODE *u;
    DNODE *d = dict->start; // start searching for correct url at beggining of dict
    while(flag){
        u = d->data;  // URLNODE from current DNODE
        if(strcmp(d->key, url) == 0){ // If the url was found
            u->visited = 1; // switch it so visited is true
            flag = 0;
        } else{
            if(d->next == NULL) // stop searching at end of dict
                flag = 0;
            d = d->next;
        }
    }
}