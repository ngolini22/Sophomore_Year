#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <strings.h>
#include "../indexer/indexer.h"
#include "../utils/file.h"
#include "./query.h"

#define START_TEST_CASE  int rs=0


#define SHOULD_BE(x) if(!(x)) {rs=rs+1; \
	printf("Line %d Fails\n", __LINE__); \
  }

#define END_TEST_CASE return rs

#define RUN_TEST(x, y) if (!x()) {              \
    printf("Test %s passed\n", y);              \
} else {                                        \
    printf("Test %s failed\n", y);              \
    cnt = cnt + 1;                              \
}

// Test case 3: calls getNextWord to test user input splicing

int TestgetNextURL(){
	START_TEST_CASE;

	// initialize args to be called in getNextWord
	int i = 0;
	int currentPosition = 0;
	char **wordList;
	wordList = malloc(sizeof(char)*(MAX_WORD_LEN+1));
	if(wordList == NULL){
		printf("No enough memory at %s:line%d ", __FILE__, __LINE__); \
    exit(-1); 
	}
	bzero(wordList, MAX_WORD_LEN);
	char *str = "this is a test string";
	printf("String to be parsed: %s\n", str)
	;
	getNextWord(str, currentPosition, wordList, i);
	SHOULD_BE(wordList[0] == "this");
	SHOULD_BE(wordList[1] == "is");
	SHOULD_BE(wordList[2] == "a");
	SHOULD_BE(wordList[3] == "test");
	SHOULD_BE(wordList[4] == "string");
	END_TEST_CASE;
}

int TestAND1{
	START_TEST_CASE;
	// indexTEST was created using data from a crawl of depth 1 on 
	// 		www.cs.dartmouth.edu/~campbell
	INVERTED_INDEX *indexTEST;

	char **wordList = malloc(sizeof(char)*(MAX_WORD_LEN+1));
	wordList[0] = "a";
	rankWords(wordList, 1);
	SHOULD_BE(qholder->results->document_id == 13);
	SHOULD_BE(qholder->results->page_word_frequency == 77);
	END_TEST_CASE;
}

int TestAND2{
	START_TEST_CASE;
	INVERTED_INDEX *indexTEST;
	char **wordList = malloc(sizeof(char)*(MAX_WORD_LEN+1));
	wordList[0] = "a";
	wordList[1] = "abc";
	rankWords(wordList, 2);
	SHOULD_BE(qholder->results->document_id == 0);
	SHOULD_BE(qholder->results->page_word_frequency == 31);
	END_TEST_CASE;
}

int TestOR1{
	START_TEST_CASE;
	INVERTED_INDEX *indexTEST;
	char **wordList = malloc(sizeof(char)*(MAX_WORD_LEN+1));
	wordList[0] = "absent";
	wordList[1] = "OR";
	wordList[2] = "absolutely";
	rankWords(wordList, 3);
	SHOULD_BE(qholder->results->document_id == 13);
	SHOULD_BE(qholder->results->page_word_frequency == 1);
	SHOULD_BE(qholder->results->next->document_id == 4);
	SHOULD_BE(qholder->results->next->page_word_frequency == 1);
	END_TEST_CASE;
}

int TestOR2{
	START_TEST_CASE;
	INVERTED_INDEX *indexTEST;
	char **wordList = malloc(sizeof(char)*(MAX_WORD_LEN+1));
	wordList[0] = "absent";
	wordList[1] = "OR";
	wordList[2] = "about";
	rankWords(wordList, 3);
	SHOULD_BE(qholder->results->document_id == 6);
	SHOULD_BE(qholder->results->page_word_frequency == 6);
	END_TEST_CASE;
}

int main(int argc, char** argv){
	int cnt;
	RUN_TEST(TestgetNextURL, "getNextWord test case");
	RUN_TEST(TestAND1, "Simple AND search");
	RUN_TEST(TestAND2, "Two AND word search");
	RUN_TEST(TestOR1, "OR search with no common documents");
	RUN_TEST(TestOR2, "OR search with commong documents");
	if(!cnt){
		printf("All passed!\n"); return 0;
	} else{
		printf("Some failed!\n"); return 1;
	}
}







