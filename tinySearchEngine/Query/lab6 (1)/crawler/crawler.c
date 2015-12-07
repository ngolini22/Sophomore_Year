/*
NOTES - SINCE DICT IS A global var, didnt know how not to have these functions in their own file.  

*/


/*

  FILE: crawler.c

  Description:

  Inputs: ./crawler [SEED URL] [TARGET DIRECTORY WHERE TO PUT THE DATA] [MAX CRAWLING DEPTH]

  Outputs: For each webpage crawled the crawler program will create a file in the 
  [TARGET DIRECTORY]. The name of the file will start a 1 for the  [SEED URL] 
  and be incremented for each subsequent HTML webpage crawled. 

  Each file (e.g., 10) will include the URL associated with the saved webpage and the
  depth of search in the file. The URL will be on the first line of the file 
  and the depth on the second line. The HTML will for the webpage 
  will start on the third line.

*/


/*
 BUGS:
 
 - initially test first url to see if it's ok
 
 */
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "crawler.h"
#include "../utils/html.h"
#include "../utils/hash.h"
#include "../utils/header.h"
// #include "../utils/dictionary.h"
#include "../utils/files.h"

// Define the dict structure that holds the hash table 
// and the double linked list of DNODES. Each DNODE holds
// a pointer to a URLNODE. This list is used to store
// unique URLs. The search time for this list is O(n).
// To speed that up to O(1) we use the hash table. The
// hash table holds pointers into the list where 
// DNODES with the same key are maintained, assuming
// the hash(key) is not NULL (which implies the URL has
// not been seen before). The hash table provide quick
// access to the point in the list that is relevant
// to the current URL search. 


DICTIONARY* dict;


// This is the table that keeps pointers to a list of URL extracted
// from the current HTML page. NULL pointer represents the end of the
// list of URLs.


/*


(5) *Crawler*
-------------

// Input command processing logic

(1) Command line processing on arguments
    Inform the user if arguments are not present
    IF target_directory does not exist OR depth exceeds max_depth THEN
       Inform user of usage and exit failed

// Initialization of any data structures

(2) *initLists* Initialize any data structure and variables

// Bootstrap part of Crawler for first time through with SEED_URL

(3) page = *getPage(seedURL, current_depth, target_directory)* Get HTML into a string and return as page, 
            also save a file (1..N) with correct format (URL, depth, HTML) 
    IF page == NULL THEN
       *log(PANIC: Cannot crawl SEED_URL)* Inform user
       exit failed
(4) URLsLists = *extractURLs(page, SEED_URL)* Extract all URLs from SEED_URL page.
  
(5) *free(page)* Done with the page so release it

(6) *updateListLinkToBeVisited(URLsLists, current_depth + 1)*  For all the URL 
    in the URLsList that do not exist already in the dictionary then add a DNODE/URLNODE 
    pair to the DNODE list. 

(7) *setURLasVisited(SEED_URL)* Mark the current URL visited in the URLNODE.

// Main processing loop of crawler. While there are URL to visit and the depth is not 
// exceeded keep processing the URLs.

(8) WHILE ( URLToBeVisited = *getAddressFromTheLinksToBeVisited(current_depth)* ) DO
        // Get the next URL to be visited from the DNODE list (first one not visited from start)
 
      IF current_depth > max_depth THEN
    
          // For URLs that are over max_depth, we just set them to visited
          // and continue on --> c call to go back to beginning of while loop
    
          setURLasVisited(URLToBeVisited) Mark the current URL visited in the URLNODE.
          continue;

    page = *getPage(URLToBeVisited, current_depth, target_directory)* Get HTML into a 
            string and return as page, also save a file (1..N) with correct format (URL, depth, HTML) 

    IF page == NULL THEN
       *log(PANIC: Cannot crawl URLToBeVisited)* Inform user
       setURLasVisited(URLToBeVisited) Mark the bad URL as visited in the URLNODE.
       Continue; // We don't want the bad URL to stop us processing the remaining URLs.
   
    URLsLists = *extractURLs(page, URLToBeVisited)* Extract all URLs from current page.
  
    *free(page)* Done with the page so release it

    *updateListLinkToBeVisited(URLsLists, current_depth + 1)* For all the URL 
    in the URLsList that do not exist already in the dictionary then add a DNODE/URLNODE 
    pair to the DNODE list. 

    *setURLasVisited(URLToBeVisited)* Mark the current URL visited in the URLNODE.

    // You must include a sleep delay before crawling the next page 
    // See note below for reason.

    *sleep(INTERVAL_PER_FETCH)* Sneak by the server by sleeping. Use the 
     standard Linux system call

(9)  *log(Nothing more to crawl)

(10) *cleanup* Clean up data structures and make sure all files are closed,
      resources deallocated.
*/


int main(int numArgs, char *arg[]) {
    struct _URL SEED_URL[MAX_URL_LENGTH];
    char target_directory[MAX_URL_LENGTH];
    int crawlDepth;
    struct stat sb;
    char *page;
    char *URLToBeVisited;
    int currentDepth = 0;

    // If user does not enter three arguments, it cannot be right, so exit
    if(numArgs != 4){
        printf("Incorrect user input. Either too many or too few arguments. Please try again in the following format:\n");
        printf("[SEED_URL] [TARGET_DIRECTORY] [CRAWL_DEPTH]\n");
        return 1;
    }
    
    // set the SEED_URL url name
    strcpy(SEED_URL->url, arg[1]);
    
    // set target directory
    strcpy(target_directory, arg[2]);
    stat(target_directory, &sb);
    if(!S_ISDIR(sb.st_mode)){
        printf("The directory you chose is not available.\n");
        return 1;
    }
    
    // set crawl depth from user input
    crawlDepth = atoi(arg[3]);
    if(crawlDepth > 3){
        printf("Your crawl depth is too large. The crawl depth cannot exceed 3.\n");
        return 1;
    }
    
    // allocate memory and initialize vars and data structures using initLists
    if(initLists() != 1){
        printf("initialization wrong.\n");
        return 1;
    }
    // Show user their inputs
    printf("\n[SEED_URL] %s\n", SEED_URL->url);
    printf("[TARGET DIRECTORY] %s\n", target_directory);
    printf("[CRAWL_DEPTH] %d\n", crawlDepth);
    
    char **URLsLists;
    // start off by getting SEED_URL downloaded and put into the file "0"
    page = getPage(SEED_URL->url, 0, target_directory);
    if(page == NULL){
        printf("PANIC: Cannot crawl SEED_URL");
        return 1;
    } else{ // load non-unique list of urls into URLsLists
        URLsLists = extractURLs(page, SEED_URL->url);
    }
//    prints out initial url list
//    int i = 0;
//    printf("url_list[]: \n");
//    while(URLsLists[i] != NULL){
//        printf("%s\n", URLsLists[i]);
//        i++;
//    }
    free(page);
    updateListLinkToBeVisited(URLsLists, (currentDepth + 1)); // create DNODES for unique urls
    setURLasVisited(SEED_URL->url); // set seed url to be visited
    
    while((URLToBeVisited = getAddressFromTheLinksToBeVisited(&currentDepth))){
        
        printf("URLToBeVisited: %s\n", URLToBeVisited); // print for user the webpage being considered
        // if the url is at a depth higher than the max depth go back to beginning of while loop and set the url to be visited so it is not considered again
        if(currentDepth > crawlDepth){
            printf("depth too high. CONTINUING \n");
            setURLasVisited(URLToBeVisited);
            continue;
        }
        
        // get html and save it into a file named the next number
        page = getPage(URLToBeVisited, currentDepth, target_directory);
        if(page == NULL){
            printf("PANIC: cannot crawl %s\n", URLToBeVisited);
            setURLasVisited(URLToBeVisited);
        } else if (strcmp(page, "a") == 0){ // if getPage returns 'a' then the url is bad - 'a' is just my check to make sure it is a good url
            setURLasVisited(URLToBeVisited);
            printf("bad ur *l\n");
            continue;
        }

        URLsLists = extractURLs(page, URLToBeVisited); // extract urls from new website
        if(URLsLists == NULL){ // if the html contains no urls, go back to beginning of while loop
            setURLasVisited(URLToBeVisited);
            continue;
        }
        
        // finish crawling process
        free(page);
        
        updateListLinkToBeVisited(URLsLists, currentDepth + 1);

        setURLasVisited(URLToBeVisited);

        sleep(1);
        free(URLsLists[0]);
    }
    printf("DONE CRAWLING!\n");
    
    // cleanUp();
    
    printf("DONE CLEANING\n");
    
    return 0;
}

void cleanUp(){
    DNODE *c, *temp;
    c = dict->start;
    while(c != NULL){
        temp = c;
        if(temp != NULL){
            free(temp->data);
            free(temp->key);
        }
        c = c->next;
    }
}

/*
Holds all of the funcitons for dictionary manipulation




Initializes all of the data structures used in the lab:
    url_list[MAX_URL_PER_PAGE]
    urlnode[malloc(sizeof(URLNODE))
    dict(DICTIONARY*)malloc(sizeof(DICTIONARY))
    dnode (dict->end)[malloc(sizeof(DNODE))
    
    sets memory for each structure and fills in the memory with zeroes
*/

// #include <sys/stat.h>
// #include <sys/types.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <strings.h>
// #include "crawler.h"
// #include "dictionary.h"
// #include "../utils/hash.h"

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

/*
 Checks to see if url is unique
 
 start at first DNODE
 
 input - url_list[i]
 output- 1 if unique, 0 if not unique
 
 */

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

void addNode(char *url, int currentDepth){
    int index, index2;
    
    index = hash1(url)%(MAX_URL_LENGTH);
    
    URLNODE *u = malloc(sizeof(URLNODE));
    MALLOC_CHECK(u);
    u->visited = 0;
    u->depth = currentDepth;
    bzero(u->url, MAX_URL_LENGTH);
    strncpy(u->url, url, MAX_URL_LENGTH);

//    printf("url from new URLNODE: %s\n", u->url);
//    printf("index: %d\n", index);
    
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


/*
 
 Finds the next url to visit by traversing through the DNODES. Finds first DNODE with URLNODE who's visited = 0
 
 */

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

/*
 Searches DNODES to find the one with the same url
 
 Sets URL that was jsut visited as visited
 
 hash url
 
 pointer to DNODE->data->visited
 
 make visited = 1
 
 */

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

