/*
 
 - initialize url_list
 - implement string parser
 - load all url links from a page into the url_list if it contains the url prefix
    - if valid url is found, malloc a string to copy the url into and then add the pointer to the url_list[].
 
*/

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "html.h"
#include "crawler.h"

char **extractURLs(char *html_buffer, char* currentURL){
    int flag = 1;
    int i = 0;
    int current = 0;
    char **URL_LIST;
    
    
    
    // initialize url_list[] to null
    URL_LIST = malloc(sizeof(char)*MAX_URL_PER_PAGE+1);
    MALLOC_CHECK(URL_LIST);
    bzero(URL_LIST, MAX_URL_PER_PAGE);
    
    while(flag){
        char* result;
        // allocate space for the result buffer
        result = malloc(MAX_URL_LENGTH+1);
        MALLOC_CHECK(result);
        bzero(result, MAX_URL_LENGTH+1);
        
        // load next url into the buffer pointer result
        
        current = GetNextURL(html_buffer, currentURL, result, current);
//        printf("current: %s\n", result);
        if(current == -1){ // if GetNextURL is done searching for urls, exit loop
            flag = 0;
            printf("All URLs found.\n");

        } else if(NormalizeURL(result) == 0){
          
        } else{ // if GetNextURL finds another url, allocate space for it in the url list and add it to list.
            URL_LIST[i] = malloc(MAX_URL_LENGTH);
            MALLOC_CHECK(URL_LIST[i]);
            bzero(URL_LIST[i], MAX_URL_LENGTH);

            if(i == 0){
                printf("currentURL: %s\n", currentURL);
                strncpy(URL_LIST[i], currentURL, MAX_URL_LENGTH);
                i++;
            } else{
                if(strstr(result, URL_PREFIX) != NULL){ // only add it to list if it contains the prefix to stay within t
                    strncpy(URL_LIST[i], result, MAX_URL_LENGTH);
//                  printf("%s\n", url_list[i]);
                    i++;
                }
            }
        }
    }
    // set last character to zero
    URL_LIST[i] = malloc(MAX_URL_LENGTH);
    MALLOC_CHECK(URL_LIST[i]);
    URL_LIST[i] = '\0';
    url_listCount = url_listCount + i;

    return URL_LIST;
}


