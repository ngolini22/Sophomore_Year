/*
 
 - Stores seed page into a temp file using wget
 - Determine length of temp file in terms of num bytes
 - Malloc a buffer
 - read in the file into the buffer
 - add the 0 at the end of the string
 - return the pointer to the buffer
 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../crawler/crawler.h"
#include "files.h"
#include "html.h"

char *getPage(char* url, int depth, char* path){
    FILE *tempFile; 
    FILE *newFile;
    long lSize;
    size_t result;
    
    // system call to call my wget shell script which saves the html into a temp file
    char command[MAX_URL_LENGTH+13];
    sprintf(command, "%s %s %s", "./wgetPage.sh", url, path);
    if(system(command)){
        printf("unavailable page\n");
        return "a";
    }
    
    // go into user directed path
    chdir(path);
    
    tempFile = fopen("temp.html", "r"); // check if file is null or not
    // only go through with process if temp file made correctly
    if(tempFile == NULL){
        printf("The file was not created correctly.\n");
        exit(1);
    }
    
    // get size of file containing url contents
    fseek(tempFile, 0, SEEK_END);
    lSize = ftell(tempFile);
    rewind(tempFile);

    // create the buffer and allocate appropriate amount of space to it
    char* buffer = (char *)malloc(sizeof(char)*(lSize+1));
    MALLOC_CHECK(buffer);
    memset(buffer, 0, sizeof(char)*lSize+1);
    
    // read in tempFile to buffer and check if it went in correctly
    result = fread(buffer, sizeof(char), lSize, tempFile);
    if(result != lSize){
        printf("Reading error");
        exit (1);
    }
    
    // add extra zero at the end of the string
//    buffer = strcat(buffer, "/0");
    fclose(tempFile);

    char comm[MAX_URL_LENGTH];
    sprintf(comm, "%s %s", "cd", path);
    chdir(path);

    // create file with URL, depth, and HTML contents
    // name the file after the file counter, getPageCount
    char filename[sizeof(char)];
    sprintf(filename, "%d", getPageCount);
    newFile = fopen(filename, "w");
    
    // put url, depth, and html into file named the next number
    fputs(url, newFile);
    printf("url to be put in file: %s\n", url);
    fprintf(newFile, "\n%d\n", depth);
    fputs(buffer, newFile);
    
    getPageCount++;
    
    chdir("..");
    return buffer;
}



/*
 
 - initialize url_list
 - implement string parser
 - load all url links from a page into the url_list if it contains the url prefix
    - if valid url is found, malloc a string to copy the url into and then add the pointer to the url_list[].
 
*/

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
