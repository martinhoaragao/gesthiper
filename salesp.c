/* AVLP -> AVL of products, it's the main AVL
 * AVLPC -> AVL of clients used by AVLP of products
 */

#include <stdlib.h>
#include <string.h>
#include "salesp.h"
#include "bool.h"

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

static AVLPC createAVLPCNode (char * client, int type);

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
  new->left = new->right = NULL;
  return new;
}

/* Auxiliar function to add a product to the AVL */
static AVLP addProduct (AVLP avlp, char * product, int quant)
{
  AVLP result;                  /* Function return value */
  int strcomp, balance, i, j;   /* Result of strcmp, balance factor, auxiliar ints */

  if (avlp == NULL) return createAVLPNode(product, quant); /* Create a new node */
  else
  {
    strcomp = strcmp(product, avlp->product);

    if (!strcomp) return avlp;
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
AVLP insertProduct (AVLP avlp, char * product, int quant)
{
  if (!avlp) return NULL; /* AVLP was not initialized */
  else if (!strcmp(avlp->product,"\0")) /* First product to be inserted */
    return createAVLPNode (product, quant);

  return addProduct(avlp, product, quant);
}

/* Initiate a products AVL */
AVLP initSalesP ()
{
  AVLP new = malloc(sizeof(struct avlp));
  strcpy(new->product, "\0");
  new->quant = 0;
  new->clients = NULL;
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

static AVLPC createAVLPCNode (char * client, int type)
{
  AVLPC new = malloc(sizeof(struct avlpc));
  strcpy(new->client, client);
  if (type == 0) new->n = true;
  else if (type == 1) new->p = true;
  new->left = new->right = NULL;
  return new;
}

/* Auxiliar function to add a product to the AVL */
static AVLPC addClient (AVLPC avlpc, char * client, int type)
{
  AVLPC result;                 /* Function return value */
  int strcomp, balance, i, j;   /* Result of strcmp, balance factor, auxiliar ints */

  if (avlpc == NULL) return createAVLPCNode(client, type); /* Create a new node */
  else
  {
    strcomp = strcmp(client, avlpc->client);

    if (!strcomp) return avlpc;
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

AVLP insertClient (AVLP avlp, char * product, char * client, int type)
{
  AVLP aux = getProductNode(avlp, product);

  if (aux == NULL) return NULL; /* Product does not exist */
  else
    aux->clients = addClient(aux->clients, client, type);

  return avlp;
}
