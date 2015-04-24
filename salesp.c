/* AVLP -> AVL of products, it's the main AVL
 * AVLPC -> AVL of clients used by AVLP of products
 */

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
  new->quant = quant;
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
    avlp->quant = quant;
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
  else
    aux->clients = addClient(aux->clients, client, type);

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

/* Insert product on the list */
static StrList insertPList (AVLP avlp, StrList list, int * quants, int lim)
{
  int i = 0, n = 0, counter = 0;   /* Iterators and counter */
  Bool done = false;  /* Used to check where the new product should be inserted */

  if (avlp != NULL) {

    list->clients[list->size] = malloc(sizeof(char) * 7); /* Space for new product */

    /* Check where to put new client */
    for (i = 0; (i < list->size) && !done && (counter < lim + 1); i++){
      if ((avlp->quant >= quants[i])) done = true;
      if ((i < list->size - 1) && (quants[i] != quants[i+1])) counter++;
    }

    if (counter < lim + 1){
      if (done) {  /* Right shift all strings */
        i--;
        for (n = (list->size) - 1; n >= i; n--) {
          strcpy(list->clients[n+1], list->clients[n]);
          quants[n+1] = quants[n];
        }
      }

      strcpy(list->clients[i], avlp->product);  /* Insert product */
      quants[i] = avlp->quant;                  /* Save quantity */

      (list->size)++;
    }

    insertPList(avlp->left, list, quants, lim);
    insertPList(avlp->right, list, quants, lim);
  }

  return list;
}

/* Querie 12 - List of N most bought products during the year */
StrList topNProducts (AVLP avlp, int n)
{
  StrList list;             /* To store products */
  int * quants, i;    /* Store quantitys for products, iterator */
  list = (StrList) malloc(sizeof(struct strlist));
  list->size = 0;

  /* Calloc sets all the values to zero */
  quants = (int *) calloc(1, sizeof(int) * 200000);

  /* Insert first product */
  list->clients[0] = malloc(sizeof(char) * 7);
  strcpy(list->clients[0], avlp->product);
  quants[0] = avlp->quant;

  insertPList(avlp->right, list, quants, n); /* Right subtree */
  insertPList(avlp->left, list, quants, n);  /* Left subtree */

  /* Check if more than n products should appear */
  for (i = n; (i < list->size) && (quants[i] == quants[i-1]); i++)
    ;
  list->size = i;

  return list;
}
