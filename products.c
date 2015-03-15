#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "products.h"

#define ALPHA 26

struct product{
	char code[7];
	struct product* link[ALPHA];
};

struct product* root = NULL;
char key2[7];

struct product* new_product(){
	struct product *q =(struct product*)malloc(sizeof(struct product));
	int x;
	for(x = 0; x < ALPHA; x++)
		q->link[x] = NULL;
	return q;
}

char* code_ify(char key[]){
	strcpy(key2, key);
	int length = strlen(key2);
	int i = 0;
	for(; i < length; i++){
		if(!isalpha(key2[i])) key2[i] += 17;
	}
	return key2;	
}

/*Recebe o código do produto como argumento e tenta inseri-lo na trie.
  Retorna 1 caso consiga inserir o produto.
  Retorna 0 em caso de erro. */
int insert_product(char key[]){
	char* key2 = code_ify(key);
	int length = strlen(key2);
	if (length == 8){
		int index, level = 0;
		if(root == NULL) root = new_product();
		struct product *q = root;

		for(;level<length; level++){
			index = key2[level] - 'A';

			if(q->link[index] == NULL)
				q->link[index] = new_product();
			q = q->link[index];
		}
		strcpy(q->code, key);
		return 1;
	}else return 0;
}

/*Recebo o código do produto como argumento e procura-o na trie.
  Retorna 1 caso o código do produto seja encontrado.
  Retorna 0 caso o código não seja válido.
  Retorna -1 caso o código do produto não seja encontrado.*/
int search(char key[]){
	char* key2 = code_ify(key);
	int length = strlen(key2);
	if (length == 6){
		struct product *q = root;
		if(root == NULL) return -1;
		int level = 0, index;

		for(;level < length; level++){
			index = key2[level] - 'A';
			
			if(q->link[index] != NULL)
				q = q->link[index];
			else break;
		}
		if(key2[level] == '\0') return 1;
		return -1;
	}else return 0;
}
