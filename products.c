#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "products.h"

#define ALPHA 26

Product *root = NULL;
Product *root2= NULL;

char key2[6];

Product* new_product(){
	Product *q =(Product*)malloc(sizeof(Product));
	int x;
	for(x = 0; x < ALPHA; x++)
		q->link[x] = NULL;
	q->back = NULL;
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
	key = strtok(key, "\r");
	key = strtok(key, "\n");
	char* key2 = code_ify(key);
	int length = strlen(key2);
	if (length == 6){
		int index, level = 0;
		if(root == NULL) root = new_product();
		Product *q = root;

		for(;level<length; level++){
			index = key2[level] - 'A';

			if(q->link[index] == NULL)
				q->link[index] = new_product();
			q->link[index]->back = (int *)q;
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
		Product *q = root;
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

//Procura produtos pela letra inicial
PList* searchI(char c){
	int i, j, k, l, m, n = 0;
	PList *result = (PList *)malloc(sizeof(PList));
	PList *wurzel = result;

	if(isalpha(c) == 0) return NULL;
	if(!isupper(c)) c = toupper(c);

	Product *q = root->link[c-'A'];
	Product *temp;

	if(q == NULL) return NULL;


	for (i = 0; i < ALPHA; i++){
		if (q->link[i])
		for (j = 0; j < ALPHA; j++){
			if (q->link[i]->link[j])
			for (k = 0; k < 10; k++){
				if (q->link[i]->link[j]->link[k])
				for (l = 0; l < 10; l++){
					if (q->link[i]->link[j]->link[k]->link[l])
					for (m = 0; m < 10; m++){
						if (q->link[i]->link[j]->link[k]->link[l]->link[m]){
							temp = q->link[i]->link[j]->link[k]->link[l];
							strcpy(result->code, temp->link[m]->code);
						}
					}
				}
			}
		}
	}

	return wurzel;

}

void productsClone(){
	int i, j, k, l, m;
	Product *q = root;

	for (i = 0; i < ALPHA; i++){
		if (q->link[i])
		for (j = 0; j < ALPHA; j++){
			if (q->link[i]->link[j])
			for (k = 0; k < 10; k++){
				if (q->link[i]->link[j]->link[k])
				for (l = 0; l < 10; l++){
					if (q->link[i]->link[j]->link[k]->link[l])
					for (m = 0; m < 10; m++){
						if (q->link[i]->link[j]->link[k]->link[l]->link[m]){
							char* key;
							strcpy(key, q->link[i]->link[j]->link[k]->link[l]->link[m]->code);
							key = strtok(key, "\r"); key = strtok(key, "\n");
							char* key2 = code_ify(key);
							int length = strlen(key2);
							if (length == 6){
								int index, level = 0;
								if(root2 == NULL) root2 = new_product();

								Product *z = root2;

								for(;level<length; level++){
									index = key2[level] - 'A';

									if(z->link[index] == NULL)
										z->link[index] = new_product();
									z->link[index]->back = (int *)z;
									z = z->link[index];
								}
								strcpy(z->code, key);
							}
						}
					}
				}
			}
		}
	}
	

}

void removeP(char key[]){
	if(root2 == NULL) productsClone();
	char* key2 = code_ify(key);
	int length = strlen(key2);
	if (length == 6){
		Product *z = root2;
		Product *temp = NULL;

		int level = 0, index;

		for(;level < length; level++){
			index = key2[level] - 'A';

			if (level == 5) temp = z;

			if(z->link[index] != NULL)
				z = z->link[index];
			else break;
		}
		if(key2[level] == '\0'){
			z = temp;
			z->link[key2[5]] = NULL;
		}
	}
}
