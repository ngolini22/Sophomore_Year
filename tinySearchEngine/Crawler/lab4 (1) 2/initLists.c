/*

Initializes all of the data structures used in the lab:
    url_list[MAX_URL_PER_PAGE]
    urlnode[malloc(sizeof(URLNODE))
    dict(DICTIONARY*)malloc(sizeof(DICTIONARY))
    dnode (dict->end)[malloc(sizeof(DNODE))
    
    sets memory for each structure and fills in the memory with zeroes
*/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "crawler.h"

int initLists(void){
    // initialize getPageCount and url list counter
    getPageCount = 0; // keeps track of what the next file should be called
    url_listCount = 0; // keeps track of how long the url_list is
//    currentDepth = 0; // keeps track of the depth while crawling
    
    // initialize dictionary
    dict = (DICTIONARY*)malloc(sizeof(DICTIONARY)+1);
    MALLOC_CHECK(dict);
    bzero(dict, sizeof(DICTIONARY)+1);
    dict->start = dict->end = NULL;
    return 1;
}