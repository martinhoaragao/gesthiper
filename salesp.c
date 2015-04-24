/* AVLP -> AVL of products, it's the main AVL
 * AVLPC -> AVL of clients used by AVLP of products
 */
#define _GNU_SOURCE 1 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "salesp.h"
#include "bool.h"
#include "includes/StrList.h"

#define MAX(A,B) ((A > B) ? A : B)

/* -------------------------- STRUCT DEFINITIONS --------------------------*/

/* AVL of clients used by products */
 typedef struct avlpc {
  char client[6];               /* client code */
  Bool n;                       /* normal sale */
  Bool p;                       /* promotion sale */
  int height;                   /* Height of the avl tree */
  struct avlpc * left, * right; /* Subtrees */
 } * AVLPC;

/* AVL of products */
 struct avlp {
  char product[7];              /* the product code */
  int quant;                    /* total quantity bought */
  int qclients;                 /* total number of clients */
  int height;                   /* Height of node */
  struct avlpc * clients;       /* AVL with clients that bought */
  struct avlp * left, * right;  /* Subtrees */
 };

/*------------------------- STATIC FUNCTION DEFINITIONS -------------------------*/
 static AVLP createAVLPNode (char * product, int quant);
 static AVLP rotateAVLPleft (AVLP);
 static AVLP rotateAVLPright (AVLP);
 static AVLP addProduct (AVLP, char *, int);
 static AVLP getProductNode (AVLP, char *);
 static int height_avlp (AVLP);
 static int getBalance_avlp (AVLP);

 static AVLPC createAVLPCNode (char * client, char type);
 static Bool searchAVLPC (AVLPC, char *);

/*------------------------------- PRODUCT NODES FUNCTIONS ------------------------------*/

 static int height_avlp (AVLP avlp)
 {
  if (!avlp) return 0;
  return avlp->height;
}

static int getBalance_avlp (AVLP avlp)
{
  if (!avlp) return 0;
  return height_avlp(avlp->left) - height_avlp(avlp->right);
}

static AVLP rotateAVLPright (AVLP avlp)
{
  AVLP x = avlp->left;
  AVLP T2 = x->right;

    /* Perform rotation */
  x->right = avlp;
  avlp->left = T2;

    /* Update heights */
  avlp->height = MAX(height_avlp(avlp->left), height_avlp(avlp->right))+1;
  x->height = MAX(height_avlp(x->left), height_avlp(x->right))+1;

    /* Return new node */
  return x;
}

/* Left Rotate AVL tree */
static AVLP rotateAVLPleft (AVLP avlp)
{
  AVLP y = avlp->right;
  AVLP T2 = y->left;

    /* Perform rotation */
  y->left = avlp;
  avlp->right = T2;

    /*  Update heights */
  avlp->height = MAX(height_avlp(avlp->left), height_avlp(avlp->right))+1;
  y->height = MAX(height_avlp(y->left), height_avlp(y->right))+1;

    /* Return new node */
  return y;
}

/* Create a product avl node */
static AVLP createAVLPNode (char * product, int quant)
{
  AVLP new = malloc(sizeof(struct avlp));
  strcpy(new->product, product);
  new->quant = quant; new->qclients = 0;
  new->clients = NULL;
  new->height = 0;
  new->left = new->right = NULL;
  return new;
}

/* Auxiliar function to add a product to the AVL */
static AVLP addProduct (AVLP avlp, char * product, int quant)
{
  AVLP result = NULL;           /* Function return value */
  int strcomp, balance, i, j;   /* Result of strcmp, balance factor, auxiliar ints */

  if (avlp == NULL) result = createAVLPNode(product, quant); /* Create a new node */
  else
  {
    strcomp = strcmp(product, avlp->product);

    if (!strcomp)
    {
      avlp->quant += quant;
      return avlp;
    }
    else if (strcomp > 0)
    {
      avlp->right = addProduct(avlp->right, product, quant);
    }
    else
    {
      avlp->left = addProduct(avlp->left, product, quant);
    }

    result = avlp;

    avlp->height = MAX(height_avlp(avlp->left), height_avlp(avlp->right)) + 1;
    balance = getBalance_avlp(avlp);

    /* Perform rotations if needed */
    if ((balance > 1) || (balance < -1))
    {
      if (avlp->left == NULL) i = 0;
      else i = strcmp(product, avlp->left->product);
      if (avlp->right == NULL) j = 0;
      else j = strcmp(product, avlp->right->product);

      if (balance > 1 && i < 0)
        result = rotateAVLPright(avlp);

      if (balance < -1 && j > 0)
        result = rotateAVLPleft(avlp);

      if (balance > 1 && i > 0)
      {
        avlp->left = rotateAVLPleft(avlp->left);
        result = rotateAVLPright(avlp);
      }

      if (balance < -1 && j < 0)
      {
        avlp->right = rotateAVLPright(avlp->right);
        result = rotateAVLPleft(avlp);
      }
    }
  }

  return result;
}

static AVLP getProductNode (AVLP avlp, char * product)
{
  int strcomp;

  if (!avlp) return NULL;
  else
  {
    strcomp = strcmp(product, avlp->product);
    if (!strcomp) return avlp;
    else if (strcomp > 0) return getProductNode(avlp->right, product);
    else return getProductNode(avlp->left, product);
  }
}

/* Insert a product in the AVL */
AVLP insertProductAVLP (AVLP avlp, char * product, int quant)
{
  if (!avlp) return NULL; /* AVLP was not initialized */
  else if (!strcmp(avlp->product,"\0")) /* First product to be inserted */
  {
    strcpy(avlp->product, product);
    avlp->quant = quant; avlp->qclients = 0;
    avlp->clients = NULL;
    avlp->left = avlp->right = NULL;
    return avlp;
  }

  return addProduct(avlp, product, quant);
}

/* Initiate a products AVL */
AVLP initSalesP ()
{
  AVLP new = malloc(sizeof(struct avlp));
  strcpy(new->product, "\0");         /* Product Code */
  new->quant = 0;                     /* Set quantity to 0 */
  new->height = 0;                    /* Set node height to 0 */
  new->clients = NULL;                /* Set clients list to NULL */
  new->left = new->right = NULL;      /* No Subtrees */
  return new;
}

/*------------------------ CLIENT NODES FUNCTIONS ------------------------------------*/

static int height_avlpc (AVLPC avlpc)
{
  if (!avlpc) return 0;
  return avlpc->height;
}

static int getBalance_avlpc (AVLPC avlpc)
{
  if (!avlpc) return 0;
  return height_avlpc(avlpc->left) - height_avlpc(avlpc->right);
}

static AVLPC rotateAVLPCright (AVLPC avlpc)
{
  AVLPC x = avlpc->left;
  AVLPC T2 = x->right;

  x->right = avlpc;
  avlpc->left = T2;

  avlpc->height = MAX(height_avlpc(avlpc->left), height_avlpc(avlpc->right)) + 1;
  x->height = MAX(height_avlpc(avlpc->left), height_avlpc(avlpc->right)) + 1;

  return x;
}

static AVLPC rotateAVLPCleft (AVLPC avlpc)
{
  AVLPC y = avlpc->right;
  AVLPC T2 = y->left;

  y->left = avlpc;
  avlpc->right = T2;

  avlpc->height = MAX(height_avlpc(avlpc->left), height_avlpc(avlpc->right))+1;
  y->height = MAX(height_avlpc(y->left), height_avlpc(y->right))+1;

  return y;
}

static AVLPC createAVLPCNode (char * client, char type)
{
  AVLPC new = malloc(sizeof(struct avlpc));
  strcpy(new->client, client);
  if (type == 'N') new->n = true;
  else if (type == 'P') new->p = true;
  new->height = 0;
  new->left = new->right = NULL;
  return new;
}

/* Auxiliar function to add a product to the AVL */
static AVLPC addClient (AVLPC avlpc, char * client, char type)
{
  AVLPC result;                 /* Function return value */
  int strcomp, balance, i, j;   /* Result of strcmp, balance factor, auxiliar ints */

  if (avlpc == NULL) return createAVLPCNode(client, type); /* Create a new node */
  else
  {
    strcomp = strcmp(client, avlpc->client);

    if (!strcomp) /* Client found */
    {
      if (type == 'N') avlpc->n = true;
      else if (type == 'P') avlpc->n = true;
      return avlpc;
    }
    else if (strcomp > 0)
    {
      avlpc->right = addClient(avlpc->right, client, type);
    }
    else
    {
      avlpc->left = addClient(avlpc->left, client, type);
    }

    result = avlpc;

    avlpc->height = MAX(height_avlpc(avlpc->left), height_avlpc(avlpc->right)) + 1;
    balance = getBalance_avlpc(avlpc);

    /* Perform rotations if needed */
    if ((balance > 1) || (balance < -1))
    {
      if (avlpc->left == NULL) i = 0;
      else i = strcmp(client, avlpc->left->client);
      if (avlpc->right == NULL) j = 0;
      else j = strcmp(client, avlpc->right->client);

      if (balance > 1 && i < 0)
        result = rotateAVLPCright(avlpc);

      if (balance < -1 && j > 0)
        result = rotateAVLPCleft(avlpc);

      if (balance > 1 && i > 0)
      {
        avlpc->left = rotateAVLPCleft(avlpc->left);
        result = rotateAVLPCright(avlpc);
      }

      if (balance < -1 && j < 0)
      {
        avlpc->right = rotateAVLPCright(avlpc->right);
        result = rotateAVLPCleft(avlpc);
      }
    }
  }

  return result;
}

/* Transverse the clients AVL and create the list of clients */
static StrList createListAVLPC (AVLPC avlpc, StrList list)
{
  int i = 5;
  if (avlpc)  /* node with client */
  {
    list->clients[list->size] = malloc(sizeof(char) * 10);
    strcpy(list->clients[list->size], avlpc->client);

    if (avlpc->n == true)
    {
      strcpy(list->clients[list->size] + i, " N ");
      i += 3;
    }
    if (avlpc->p == true )
      strcpy(list->clients[list->size] + i, " P");

    (list->size)++;
    createListAVLPC(avlpc->right, list);
    createListAVLPC(avlpc->left, list);
  }

  return list;
}

AVLP insertClientAVLP (AVLP avlp, char * product, char * client, char type)
{
  AVLP aux = getProductNode(avlp, product);

  if (aux == NULL) return NULL; /* Product does not exist */
  else{
    /* Increment number of clients if client was not inserted */
    if (searchAVLPC(aux->clients, client) == false) (aux->qclients)++;
    aux->clients = addClient(aux->clients, client, type);
  }

  return avlp;
}

/* Query 8 */
StrList clientsThatBought (AVLP avlp, char * product)
{
  AVLP aux;
  AVLPC clients;
  StrList list = malloc(sizeof(struct strlist));
  list->size = 0;

  aux = getProductNode(avlp, product); /* Get the product node */
  clients = aux->clients;

  return createListAVLPC(clients, list);
}

/* Search for a client in the clients AVL */
static Bool searchAVLPC (AVLPC node, char * client) {
  int result; /* To store strcmp result */

  if (!node) return false;  /* Client does not exist */

  result = strcmp(client, node->client);
  if (result == 0) return true; /* Client found */
  else if (result > 0) return (searchAVLPC(node->right, client));
  return searchAVLPC(node->left, client);
}

/* Insert product on the list */
static topNP insertPList (AVLP avlp, topNP aux, int lim) {
  int i = 0, n = 0, counter = 0;   /* Iterators and counter */
  Bool done = false;  /* Used to check where the new product should be inserted */

  if (avlp != NULL) {

    /* Check where to put new client */
    for (i = 0; (i < aux->list->size) && !done && (counter < lim + 1); i++){
      if ((avlp->quant >= aux->quants[i])) done = true;
      if ((i < aux->list->size - 1) && (aux->quants[i] != aux->quants[i+1])) counter++;
    }

    if (counter < lim + 1){
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

      aux->list->clients[i] = strdup(avlp->product);  /* Insert product */
      aux->quants[i] = avlp->quant;                  /* Save quantity */
      aux->clients[i] = avlp->qclients;    /* Save number of clients */

      (aux->list->size)++;
    }

    insertPList(avlp->left, aux, lim);
    insertPList(avlp->right, aux, lim);
  }

  return aux;
}

/* Querie 12 - List of N most bought products during the year */
topNP topNProducts (AVLP avlp, int n) {
  topNP aux = malloc(sizeof(struct q12struct));
  int i = 0;

  /* Initiate struct */
  aux->list = (StrList) malloc(sizeof(struct strlist));
  aux->list->size = 0;
  aux->quants = (int *) calloc(1, sizeof(int) * 200000);
  aux->clients = (int *) calloc(1, sizeof(int) * 200000);

  /* Insert first product */
  aux->list->clients[0] = malloc(sizeof(char) * 7);
  strcpy(aux->list->clients[0], avlp->product);
  aux->quants[0] = avlp->quant;
  aux->clients[0] = avlp->qclients;

  insertPList(avlp->right, aux, n); /* Right subtree */
  insertPList(avlp->left, aux, n);  /* Left subtree */

  /* Check if more than n products should appear */
  for (i = n; (i < aux->list->size) && (aux->quants[i] == aux->quants[i-1]); i++)
    ;
  aux->list->size = i;

  return aux;
}
