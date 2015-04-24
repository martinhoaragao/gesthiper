#ifndef products_h
#define products_h

#include "../includes/bool.h"

typedef struct node ProductsCat;

typedef struct listnode PList;

/* Initiates the product catalog */
ProductsCat* initProductsCat();

/* Inserts a client in the trie */
ProductsCat* insert_product(ProductsCat *root, char key[]);

/* Searches if a product is in a catalog given its code */
Bool searchProduct(ProductsCat *prodcat, char key[]);

/* Searches if a product is in a catalog give its code, but also marks the product as bought */
Bool search4Product(ProductsCat *prodcat, char key[]);

/* Search all products code given the initial letter */
PList* searchI(ProductsCat *prodcat, char c);

/* Returns a list with all the products not bought */
PList* productsNotBought(ProductsCat *procat);

/* Removes a product from the catalog given its code */
ProductsCat* removeProduct(ProductsCat *prodcat, char key[]);

/* Deletes the product catalog */
ProductsCat* deleteProductCatalog(ProductsCat *prodcat);

/* Given a product catalog, returns the number of products that were never bought */
int numOfProducts(ProductsCat *cat);

/* Function to retrieve codes from the Products List */
char* getCode(PList *p, int n);

/*Function to retrieve the how many code the Products List has */
int getQnt(PList *p);

/* Frees all the space occupied by the list */
void freeList(PList *p);

#endif
