/* AVLP -> AVL of products, it's the main AVL
 * AVLPC -> AVL of clients used by AVLP of products
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "salesp.h"
#include "../includes/bool.h"
#include "../includes/StrList.h"
#include "../includes/max.h"

/* -------------------------- STRUCT DEFINITIONS --------------------------*/

/* AVL of clients used by products */
typedef struct clientNode {
  char client[6];                       /* client code */
  Bool n;                               /* normal sale */
  Bool p;                               /* promotion sale */
  int height;                           /* Height of the avl tree */
  struct clientNode * left, * right;   /* Subtrees */
} ClientNode;

/* AVL of products */
typedef struct productNode {
  char product[7];                      /* the product code */
  int quant;                            /* total quantity bought */
  int qclients;                         /* total number of clients */
  int height;                           /* Height of node */
  struct clientNode * clients;          /* AVL with clients that bought this product */
  struct productNode * left, * right;   /* Subtrees */
} ProductNode;

typedef int (*cmpPtr)(SalesP, char *, int);

/*------------------------- STATIC FUNCTION DEFINITIONS -------------------------*/
static ProductNode * createProductNode (char * product, int quant);
static ProductNode * leftRotate (ProductNode *);
static ProductNode * rightRotate (ProductNode *);
static ProductNode * addProduct (ProductNode *, char *, int, cmpPtr);
static SalesP insertProduct_aux (SalesP, char *, int, cmpPtr);
static ProductNode * getProductNode (ProductNode *, char *);
static int height (ProductNode *);
static int getBalance (ProductNode *);

static ClientNode * leftRotate_C (ClientNode *);
static ClientNode * rightRotate_C (ClientNode *);
static ClientNode * createClientNode (char * client, char type);
static Bool searchClient (ClientNode *, char *);
static void freeClients (ClientNode *);
static int height_C (ClientNode *);

/*------------------------------- PRODUCT FUNCTIONS ------------------------------*/

/* Compute the height of a productNode */
static int height (ProductNode * node) {
  if (!node) return 0;
  return node->height;
}

/* Compute balance factor of a productNode node */
static int getBalance (ProductNode * node) {
  if (!node) return 0;
  return height(node->left) - height(node->right);
}

/* Right rotate a productNode */
static ProductNode * rightRotate (ProductNode * node) {
  ProductNode * a = node->left;
  ProductNode * b = a->right;

  a->right = node;  /* Perform rotations */
  node->left = b;

  /* Update heights */
  node->height = MAX(height(node->left), height(node->right)) + 1;
  a->height = MAX(height(a->left), height(a->right)) + 1;

  return a; /* Return new node */
}

/* Left rotate a productNode */
static ProductNode * leftRotate (ProductNode * node) {
  ProductNode * a = node->right;
  ProductNode * b = a->left;

  a->left = node; /* Perform rotation */
  node->right = b;

  /*  Update heights */
  node->height = MAX(height(node->left), height(node->right))+1;
  a->height = MAX(height(a->left), height(a->right))+1;

  return a; /* Return new node */
}

/* Create new productNode with the given and the given quantity*/
static ProductNode * createProductNode (char * product, int quant) {
  ProductNode * new = malloc(sizeof(ProductNode));  /* Allocate space for product */
  strcpy(new->product, product);                    /* Save product */
  new->quant = quant; new->qclients = 0;            /* Save quantity */
  new->clients = NULL;
  new->height = 0;
  new->left = new->right = NULL;
  return new;                                       /* Return new node */
}

/* Compare 2 nodes by alphabetical order */
static int alphabeticCompare (ProductNode * node, char * product, int quant) {
  return strcmp(product, node->product);
}

/* Compare 2 nodes by units sold */
static int unitsCompare (ProductNode * node, char * product, int quant) {
  return ((node->quant) - quant);
}

/* Auxiliar function to add a product to the AVL */
static SalesP addProduct (SalesP sales, char * product, int quant, cmpPtr cmpFunc) {
  ProductNode * result = sales;   /* Function return value */
  int strcomp, balance, i, j;     /* Result of strcmp, balance factor, auxiliar ints */

  if (sales == NULL) result = createProductNode(product, quant);  /* New Product */
  else {
    strcomp = cmpFunc(sales, product, quant);                     /* Compare products */

    if (!strcomp) {           /* Product found */
      sales->quant += quant;  /* Update quantity */
      result = sales;
    }
    else if (strcomp > 0) {
      sales->right = addProduct(sales->right, product, quant, cmpFunc);  /* Go right */
    }
    else {
      sales->left = addProduct(sales->left, product, quant, cmpFunc);    /* Go left */
    }

    sales->height = MAX(height(sales->left), height(sales->right)) + 1; /* Update height */
    balance = getBalance(sales); /* Get balance factor */

    if ((balance > 1) || (balance < -1)) { /* Perform rotations if needed */
      if (sales->left == NULL) i = 0;
      else i = cmpFunc(sales->left, product, quant);
      if (sales->right == NULL) j = 0;
      else j = cmpFunc(sales->right, product, quant);

      if (balance > 1 && i < 0) result = rightRotate(sales);  /* Left left case */
      else if (balance < -1 && j > 0) result = leftRotate(sales); /* Right right case */
      else if (balance > 1 && i > 0) {                                /* Left right case */
        sales->left = leftRotate(sales->left);
        result = rightRotate(sales);
      }
      else if (balance < -1 && j < 0) {                               /* Right left case */
        sales->right = rightRotate(sales->right);
        result = leftRotate(sales);
      }
    }
  }

  return result;
}

static ProductNode * getProductNode (ProductNode * node, char * product) {
  int strcomp;  /* strcmp() result */

  if (node) {
    strcomp = strcmp(product, node->product); /* Compare products */
    if (!strcomp) return node;                /* Product found */
    else if (strcomp > 0) return getProductNode(node->right, product);  /* Go right */
    else return getProductNode(node->left, product);                    /* Go left */
  }

  return NULL;  /* Product not found */
}

/* Auxiliar function to insert product in the AVL */
static SalesP insertProduct_aux (SalesP s, char * product, int quant, cmpPtr cmpFunc) {
  if (s) {
    if (!strcmp(s->product,"\0")) { /* First product to be inserted */
      strcpy(s->product, product);
      s->quant = quant; s->qclients = 0;
      s->clients = NULL;
      s->left = s->right = NULL;
      return s;
    }

    return addProduct(s, product, quant, cmpFunc);
  }

  return NULL;  /* AVL was not initialized */
}

/* Insert product in the AVL */
SalesP insertProductSP (SalesP s, char * product, int quant) {
  return insertProduct_aux(s, product, quant, alphabeticCompare);
}

/*
SalesP insertProductbyUnits (SalesP s, char * product, int quant) {
  return insertProduct_aux(s, product, quant, unitsCompare);
}
*/

/* Initiate a products AVL */
SalesP initSalesP () {
  SalesP new = malloc(sizeof(ProductNode)); /* Space for node */
  strcpy(new->product, "\0");               /* Product Code */
  new->quant = 0;                           /* Set quantity to 0 */
  new->height = 0;                          /* Set node height to 0 */
  new->clients = NULL;                      /* Set clients list to NULL */
  new->left = new->right = NULL;            /* No Subtrees */
  return new;                               /* Return root */
}

/*------------------------ CLIENT NODES FUNCTIONS ------------------------------------*/

/* Height value of a client node */
static int height_C (ClientNode * node) {
  if (!node) return 0;
  return node->height;
}

/* Compute balance factor of a ClientNode */
static int getBalance_C (ClientNode * node) {
  if (!node) return 0;
  return height_C(node->left) - height_C(node->right);
}

/* Right rotate a ClientNode */
static ClientNode * rightRotate_C (ClientNode * node) {
  ClientNode * a = node->left;
  ClientNode * b = a->right;

  a->right = node;  /* Perform rotations */
  node->left = b;

  node->height = MAX(height_C(node->left), height_C(node->right)) + 1;  /* Update heights */
  a->height = MAX(height_C(a->left), height_C(a->right)) + 1;

  return a; /* Return new node */
}

/* Left rotate a ClientNode */
static ClientNode * leftRotate_C (ClientNode * node) {
  ClientNode * a = node->right;
  ClientNode * b = a->left;

  a->left = node; /* Perform rotations */
  node->right = b;

  node->height = MAX(height_C(node->left), height_C(node->right))+1;  /* Update heights */
  a->height = MAX(height_C(a->left), height_C(a->right))+1;

  return a; /* Return new node */
}

/* Create a ClientNode with the given client and sale type */
static ClientNode * createClientNode (char * client, char type) {
  ClientNode * new = malloc(sizeof(ClientNode));  /* Allocate space for ClientNode */
  strcpy(new->client, client);                    /* Save client */
  if (type == 'N') new->n = true;                 /* Normal sale */
  else if (type == 'P') new->p = true;            /* Promotion sale */
  new->height = 0;
  new->left = new->right = NULL;
  return new;                                     /* Return new node */
}

/* Auxiliar function to add a client to the AVL */
static ClientNode * addClient (ClientNode * node, char * client, char type) {
  ClientNode * result = node;   /* Function return value */
  int strcomp, balance, i, j;   /* Result of strcmp, balance factor, auxiliar ints */

  if (node == NULL) return createClientNode(client, type); /* New client */
  else {
    strcomp = strcmp(client, node->client); /* Compare clients */

    if (!strcomp) { /* Client found , update sale type */
      if (type == 'N') node->n = true;
      else if (type == 'P') node->n = true;
      result = node;
    }
    else if (strcomp > 0) {                                 /* Go right */
      result = node->right = addClient(node->right, client, type);
    }
    else {                                                  /* Go to left */
      result = node->left = addClient(node->left, client, type);
    }

    node->height = MAX(height_C(node->left), height_C(node->right)) + 1; /* Update heights */
    balance = getBalance_C(node);

    if ((balance > 1) || (balance < -1)) {  /* Perform rotations if needed */
      if (node->left == NULL) i = 0;
      else i = strcmp(client, node->left->client);
      if (node->right == NULL) j = 0;
      else j = strcmp(client, node->right->client);

      if (balance > 1 && i < 0) result = rightRotate_C(node);  /* Left left case */
      else if (balance < -1 && j > 0) result = leftRotate_C(node);  /* Right right case */
      else if (balance > 1 && i > 0) {                                  /* Left Right case */
        node->left = leftRotate_C(node->left);
        result = rightRotate_C(node);
      }
      else if (balance < -1 && j < 0) {                                 /* Right left case */
        node->right = rightRotate_C(node->right);
        result = leftRotate_C(node);
      }
    }
  }

  return result;
}

/* Transverse the clients AVL and create the list of clients */
static StrList createClientsList (ClientNode * node, StrList list) {
  int i = 5;
  if (node) { /* node with client */
    list->clients[list->size] = malloc(sizeof(char) * 10);  /* Allocate space for client */
    strcpy(list->clients[list->size], node->client);

    if (node->n) { /* Normal sale */
      strcpy(list->clients[list->size] + i, " N");
      i += 2;
    }
    if (node->p) /* Promotion Sale */
      strcpy(list->clients[list->size] + i, " P");

    (list->size)++;                       /* Increase list size */
    createClientsList(node->right, list);  /* Go to right subtree */
    createClientsList(node->left, list);   /* Go to left subtree */
  }

  return list;
}

/* Insert a client in a given product node */
SalesP insertClientSP (SalesP sales, char * product, char * client, char type) {
  ProductNode * node = getProductNode(sales, product);

  if (node == NULL) return NULL; /* Product does not exist */
  else {
    /* Increment number of clients if client was not inserted already */
    if (searchClient(node->clients, client) == false) (node->qclients)++;
    node->clients = addClient(node->clients, client, type); /* Add client */
  }

  return sales;
}

/* Querie 8 - Create List of clients that bought a given product */
StrList clientsThatBought (SalesP sales, char * product) {
  SalesP node;  /* Save productNode */
  StrList list;

  node = getProductNode(sales, product);    /* Get the product node */
  if (node) {
    list = malloc(sizeof(struct strlist));  /* Allocate space for list of strings */
    list->size = 0;
    return createClientsList(node->clients, list);
  }

  return NULL;
}

/* Search for a client in the clients AVL */
static Bool searchClient (ClientNode * node, char * client) {
  int strcomp;               /* To store strcmp result */

  if (node) {
    strcomp = strcmp(client, node->client);                           /* Compare clients */
    if (strcomp == 0) return true;                                    /* Client found */
    else if (strcomp > 0) return (searchClient(node->right, client));  /* Go right */
    return searchClient(node->left, client);                           /* Go left */
  }

  return false; /* Client does not exist */
}

/* Insert product on the list */
static topNP createProductsList (ProductNode * node, topNP aux, int lim) {
  int i = 0, n = 0, counter = 0; /* Iterators and counter */
  Bool done = false;             /* Used to check where the new product should be inserted */

  if (node) {
    /* Check where to put new client */
    for (i = 0; (i < aux->list->size) && !done && (counter < lim + 1); i++){
      if ((node->quant >= aux->quants[i])) done = true;
      if ((i < aux->list->size - 1) && (aux->quants[i] != aux->quants[i+1])) counter++;
    }

    if (counter < lim + 1) {
      if (done) {  /* Right shift all strings */
        i--;
        for (n = i; (n < aux->list->size - 1) && (counter < lim + 1); n++)
          if ((n < aux->list->size - 1) && (aux->quants[i] != aux->quants[i+1])) counter++;

        for (; n >= i; n--) {
          aux->list->clients[n+1] = aux->list->clients[n];
          aux->quants[n+1] = aux->quants[n];
          aux->clients[n+1] = aux->clients[n];
        }
      }

      aux->list->clients[i] = strdup(node->product);  /* Insert product */
      aux->quants[i] = node->quant;                   /* Save quantity */
      aux->clients[i] = node->qclients;               /* Save number of clients */

      (aux->list->size)++;                            /* Increase list size */
    }

    createProductsList(node->left, aux, lim);         /* Go to left subtree */
    createProductsList(node->right, aux, lim);        /* Go to right subtree */
  }

  return aux;
}

/* Querie 12 - List of N most bought products during the year */
topNP topNProducts (SalesP sales, int n) {
  topNP aux = malloc(sizeof(struct q12struct));   /* Allocate space for struct */
  int i = 0;                                      /* Iterator */

  /* Initiate struct */
  aux->list = (StrList) malloc(sizeof(struct strlist)); /* Allocate space for string list */
  aux->list->size = 0;
  aux->quants = (int *) calloc(1, sizeof(int) * 200000);  /* Allocate quants array */
  aux->clients = (int *) calloc(1, sizeof(int) * 200000); /* Allocate clients array */

  /* Insert first product */
  aux->list->clients[0] = malloc(sizeof(char) * 7);
  strcpy(aux->list->clients[0], sales->product);
  aux->quants[0] = sales->quant;
  aux->clients[0] = sales->qclients;

  createProductsList(sales->right, aux, n); /* Right subtree */
  createProductsList(sales->left, aux, n);  /* Left subtree */

  /* Check if more than n products should appear */
  for (i = n; (i < aux->list->size) && (aux->quants[i] == aux->quants[i-1]); i++)
    ;
  aux->list->size = i;

  return aux;
}

/* Free memory used by the Clients AVL Tree */
static void freeClients (ClientNode * node) {
  if (node) {
    freeClients(node->left);   /* Free left subtree */
    freeClients(node->right);  /* Free right subtree */

    free(node);                /* Free node */
  }
}

/* Free memory used by the Products AVL Tree */
void freeSalesP (SalesP sales) {
  if (sales) {
    freeSalesP(sales->left);     /* Free left subtree */
    freeSalesP(sales->right);    /* Free right subtree */

    freeClients(sales->clients); /* Free clients AVL Tree */

    free(sales); /* Free node */
  }
}
