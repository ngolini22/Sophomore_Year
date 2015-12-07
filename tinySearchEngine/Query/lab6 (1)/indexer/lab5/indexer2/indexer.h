#ifndef _INDEXER_H_
#define _INDEXER_H_



#define MAX_NUMBER_OF_SLOTS 10000
#define MAX_WORD_LEN 10000

// DATA SCTRUCTURES

typedef struct _DocumentNode {
	struct _DocumentNode *next; // pointer to next member of the list
	int document_id; 			// document identifier
	int page_word_frequency;	// number of occurrences of the word
} _DocumentNode;

typedef struct _DocumentNode DOCUMENTNODE;

typedef struct _WordNode{
	struct _WordNode *prev; 	// pointer to the previous word
	struct _WordNode *next;		// pointer to the next word
	char word[MAX_WORD_LEN]; 	// the word
	DOCUMENTNODE *page;			// pointer to the first element of the page list
} __WordNode;

typedef struct _WordNode WORDNODE;

WORDNODE *WordListl;

typedef struct _INVERTED_INDEX {
	WORDNODE *start;					// Start and end pointer
	WORDNODE *end;						// start of the list
	WORDNODE *hash[MAX_NUMBER_OF_SLOTS];// hash slot
} _INVERTED_INDEX;

typedef struct _INVERTED_INDEX INVERTED_INDEX;

void cleanUp(INVERTED_INDEX *index);

#endif


