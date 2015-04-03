#ifndef trie_h 
#define trie_h

#define ALPHA 26


typedef struct node2{
	char code[6];
	struct node2* link[ALPHA];
	int* back;
} Product;

typedef struct listnode2{
	char code[6];
	struct listnode2 * next;

} PList;

Product* new_product();

int insert_product(char key[]);

int search(char key[]);

PList* searchI(char c);

void removeP(char key[]);

#endif
