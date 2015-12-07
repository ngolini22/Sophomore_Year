// /*
//  Checks to see if url is unique
 
//  start at first DNODE
 
//  input - url_list[i]
//  output- 1 if unique, 0 if not unique
 
//  */

// #include <sys/stat.h>
// #include <sys/types.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <strings.h>
// #include "indexer.h"
// #include "list.h"
// #include "hash.h"

// int checkUniqueness(char *word, INVERTED_INDEX *index){
//     int hashIndex, hashIndex2;
    
//     hashIndex = hash1(word)%(MAX_WORD_LEN);
//     if(index->hash[hashIndex] == NULL){ // if there are no DNODES in the hash table at the gievn index, it is unique
//         return 1;
//     } else {
//         WORDNODE *c = index->hash[hashIndex]; // search through collision cluster (DNODES at the same hash index
//         while(c != NULL){ // while still in the DNODE list
//             hashIndex2 = hash1(c->word)%(MAX_WORD_LEN); // rehash next DNODE to test if still in collision cluster
//             if(hashIndex != hashIndex2){ // exit if you've checked all DNODES in the cluster
//                 return 1;
//             }
//             if(strcmp(word, c->word) == 0){ // if url is in current, it is not unique
//                 return 0;
//             } else { // go to next DNODE to check
//                 c = c->next;
//             }
//         }
//     }
//     return 1; // if get to end of list and haven't found
// }