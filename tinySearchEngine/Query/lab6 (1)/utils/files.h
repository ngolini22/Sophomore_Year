#ifndef _FILES_H_
#define _FILES_H_

#include "../crawler/crawler.h"

// getPage: Assumption: if you are dowloading the file it must be unique. 
// Two cases. First its the SEED URL so its unique. Second, wget only getpage 
//once a URL is computed to be unique. Get the HTML file saved in TEMP 
// and read it into a string that is returned by getPage. Store TEMP
// to a file 1..N and save the URL and depth on the first and second 
// line respectively.

char *getPage(char* url, int depth,  char* path);

// extractURL: Given a string of the HTML page, parse it (you have the code 
// for this GetNextURL) and store all the URLs in the url_list (defined above).
// NULL pointer represents end of list (no more URLs). Return the url_li

char **extractURLs(char* html_buffer, char* current);

// setURLasVisited: Mark the URL as visited in the URLNODE structure.


#endif