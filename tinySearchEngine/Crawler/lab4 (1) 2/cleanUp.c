/*
 Cleans up data structures
 
 
 */
#include <stdio.h>
#include <stdlib.h> 
#include "crawler.h"


void cleanUp(void){
    // Clean up all of the DNODES, URLNODES, then dict at the end
    DNODE *d, *dd;
    URLNODE *u;
    
    for(d = dict->start; d != NULL; d = d->next){
        u = d->data;
        dd = d;
        free(u);
        free(dd);
    }
//    free(dict);
}