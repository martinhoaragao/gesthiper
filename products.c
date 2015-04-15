#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "products.h"
#include "Boolean.h"

#define ALPHA 26

struct node{
	int qnt;
	char code[6];
	struct node* link[ALPHA];
}; /* ProductsCat */

struct listnode{
	int qnt;
	char **codes;
}; /* PList */

/* Creates a Trie Node */
ProductsCat* new_node(){
	ProductsCat *q = (ProductsCat*)malloc(sizeof(ProductsCat));
	int x;
	for(x = 0; x < ALPHA; x++)
		q->link[x] = NULL;
	q->qnt = 0;
	return q;
}

/* Initiates a product list */
char** initProdList(){
	int i = 0, N = 8000;
	char **c = malloc( N * sizeof( char* ));
	if (c)
		for(; i < N; i++)
			c[i] = malloc(sizeof(c[i]) * 7);
	return c;
}

/* Initiates the product catalog */ 
ProductsCat* initProductsCat(){
	ProductsCat *prodcat = new_node();
	return prodcat;
}

/* Insert a client in the trie */
int insert_product(ProductsCat *prodcat, char key[]){
	int length, index, level = 0;
	ProductsCat *q;

	key = strtok(key, "\r");
	key = strtok(key, "\n");
	key[6]='\0';
	length = strlen(key);

	if (length == 6){
		
		if(prodcat == NULL) prodcat = new_node();
		
		q = prodcat;

		for(;level<length; level++){

			if (!isalpha(key[level])) index = (key[level] + 17) - 'A';
			else index = key[level] - 'A';

			if(q->link[index] == NULL){
				q->link[index] = new_node();
				if(level == 0) q->link[index]->qnt = 1;
			}else if(level==0) q->link[index]->qnt += 1;
			q = q->link[index];
		}
		q->qnt=0;
		strcpy(q->code, key);
		return 1; /* Product inserted */
	}else return 0;
}

/* Searches if a product is in a catalog given its code */
BOOLEAN search(ProductsCat *prodcat, char key[]){
	int length = strlen(key);
	int level = 0, index;
	key[length]='\0';

	if (length == 6){
		ProductsCat *q = prodcat;
		if(prodcat == NULL) return FALSE;

		for(;level < length; level++){

			if (!isalpha(key[level])) index = key[level] - 'A' + 17;
			else index = key[level] - 'A';
			
			if(q->link[index] != NULL)
				q = q->link[index];
			else break;
		}
		if(key[level] == '\0') return TRUE;
		return FALSE;
	}else return FALSE;
}

/* Searches if a product is in a catalog give its code, but also marks the product as bought */
BOOLEAN search2(ProductsCat *prodcat, char key[]){
	int length = strlen(key);
	int level = 0, index;
	key[length]='\0';

	if (length == 6){
		ProductsCat *q = prodcat;
		if(prodcat == NULL) return FALSE;

		for(;level < length; level++){

			if (!isalpha(key[level])) index = key[level] - 'A' + 17;
			else index = key[level] - 'A';

			if(q->link[index] != NULL)
				q = q->link[index];
			else break;
		}
		q->qnt = 1;
		if(key[level] == '\0') return TRUE;
		return FALSE;
	}else return FALSE;
}

/* Search all products code given the initial letter */
PList* searchI(ProductsCat *prodcat, char c){
	int i, j, k, l, m, n;
	PList *p = (PList*)malloc(sizeof(PList));
	ProductsCat *q;

	if(isalpha(c) == 0) return NULL;
	if(!isupper(c)) c = toupper(c);
	
	n = 0;
	p->codes = initProdList();
	p->qnt = prodcat->link[c-'A']->qnt;
	q = prodcat->link[c-'A'];
	
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
							strcpy(p->codes[n], q->link[i]->link[j]->link[k]->link[l]->link[m]->code);
							n++;
						}
					}
				}
			}
		}
	}
	return p;
}

PList* notboughtProducts(ProductsCat *prodcat){
	int i, j, k, l, m, n, o;
	PList *p = (PList*)malloc(sizeof(PList));
	ProductsCat *q;

	o = 0;
	p->codes = initProdList();
	p->qnt = 0;
	q = prodcat;

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
						if (q->link[i]->link[j]->link[k]->link[l]->link[m])
						for (n = 0; n < 10; n++){
							if(q->link[i]->link[j]->link[k]->link[l]->link[m]->link[n])
								if(!q->link[i]->link[j]->link[k]->link[l]->link[m]->link[n]->qnt){
									strcpy(p->codes[o], q->link[i]->link[j]->link[k]->link[l]->link[m]->link[n]->code);
									p->qnt += 1;
									o++;
								}
						}
					}
				}
			}
		}
	}
	return p;
}


/* Removes a product from the catalog given its code */
ProductsCat* removeProduct(ProductsCat *prodcat, char key[]){
	
	int length, level = 0, index;
	ProductsCat *z, *temp;

	length = strlen(key);

	if (length == 6){
		z = prodcat;
		temp = NULL;

		for(;level < length; level++){

			if (!isalpha(key[level])) index = (key[level] + 17) - 'A';
			else index = key[level] - 'A';

			if (level == 4) temp = z;

			if(z->link[index] != NULL)
				z = z->link[index];
			else break;
		}
		if(key[level] == '\0'){
			index = (key[4] + 17) - 'A';
			free(temp->link[index]);
			temp->link[index] = NULL;
		}
	}
	return prodcat;
}

/* Function to retrieve codes from the Products List */
char* getCode(PList *p, int n){
	if (n > -1 && n < p->qnt)
		return p->codes[n];	
	else return NULL;
}
/*Function to retrieve the how many code the Products List has */
int getQnt(PList *p){
	return p->qnt;
}
