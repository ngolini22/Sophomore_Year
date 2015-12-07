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
#include "html.h"
#include "hash.h"
#include "header.h"

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
    }
    printf("DONE CRAWLING!\n");
    
//    cleanUp();
    
    printf("DONE CLEANING\n");
    
    /*
    WHILE ( URLToBeVisited = *getAddressFromTheLinksToBeVisited(current_depth)* ) DO
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
    */
    
    
    return 0;
}
