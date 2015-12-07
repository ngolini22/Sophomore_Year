/*
 updateListLinkToBeVisited(char **url_list, int depth);
 
 - For all of the URLs in the url_list that do not exist already, then add a URLNODE to the NODE list
 
 for(all pointers in the url_list[])
    check if URL is unique
    hashindex = hash(url_list[i]) -- need to modulus this
    if(dict->hash[hashindex] == NULL)
        it is unique
        create DNODE 
            update next, prev, data, and key
            data = URLNODE - create URLNODE and insert into data
            key = url
    else
        while(looping through DNODES with same hash)
            rehash url (which is the key of the DNODE)
            if new hash is not the same as old hash
                it is unique
            go to first/next DNODE pointed to by the first element in the hash table at the index
            compare(url_list[i], URLNODE->url)
            if this strcmp == 0
                not unique
            else
                Go to next DNODE

 
 SEPERATE INTO TWO DIFFERENT FUNCTIONS!!
 
 Edge Case: 
 
 if end of cluster is at the end of the linked list
 
 Functions:
 
 - getDataWithKey(url) - when going down DNODE cluster, see if the url is unique - return pointer to URLNODE
 - addElement(url)
 
 URLNODE *u;
 if getDataWithKey(url) == NULL
    addElement()
 
 */

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "crawler.h"
#include "hash.h"

void updateListLinkToBeVisited(char **URL_LIST, int currentDepth){
    int i = 0;
    int unique;

    while(URL_LIST[i] != NULL){ // traverse through url_list
        unique = checkUniqueness(URL_LIST[i]); // 1 if unique, 0 if not
        if(unique == 1){
            addNode(URL_LIST[i], currentDepth);
        }
        i++;
    }
}

