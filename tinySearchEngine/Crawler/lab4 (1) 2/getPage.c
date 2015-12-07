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
#include <math.h>
#include <unistd.h>
#include "crawler.h"

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
    char* buffer = (char *)malloc(sizeof(char)*lSize+1);
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