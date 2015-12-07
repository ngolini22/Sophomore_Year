/*
 Checks to see if url is unique
 
 start at first DNODE
 
 input - url_list[i]
 output- 1 if unique, 0 if not unique
 
 */

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "crawler.h"
#include "hash.h"

int checkUniqueness(char *url){
    int index, index2;
    
    index = hash1(url)%(MAX_URL_LENGTH);
//    printf("url being considered: %s\n", url);
//    printf("index: %d\n",index);
    
    if(dict->hash[index] == NULL){ // if there are no DNODES in the hash table at the gievn index, it is unique
//        printf("UNIQUE\n");
        return 1;
    } else {
        DNODE *c = dict->hash[index]; // search through collision cluster (DNODES at the same hash index
        while(c != NULL){ // while still in the DNODE list
//            printf("CHECKING %s with %s\n", url, c->key);
            index2 = hash1(c->key)%(MAX_URL_LENGTH); // rehash next DNODE to test if still in collision cluster
            if(index != index2){ // exit if you've checked all DNODES in the cluster
//                printf("UNIQUE\n");
//                printf(")
                return 1;
            }
            if(strcmp(url, c->key) == 0){ // if url is in current, it is not unique
//                printf("NOT UNIQUE\n");
                return 0;
            } else { // go to next DNODE to check
                c = c->next;
            }
        }
    }
//    printf("UNIQUE BY DEFAULLT\n");
    return 1; // if get to end of list and haven't found
}