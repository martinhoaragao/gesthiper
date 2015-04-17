/*
Implementation of a sales structure with an AVL of clients and inside each node there
is a AVL for the products bought by those clients
*/
#include <stdlib.h>
#include <string.h>
#include "sales.h"
#include "includes/StrList.h"
#include "bool.h"

typedef struct clientNode ClientNode;
typedef struct productNode ProductNode;

/***** Sales.c exclusive functions *****/

static Sales createNode (char *);
static Sales addClient (Sales, char *);

static int height (ClientNode *);
static int height_P (ProductNode *);
static int max (int, int);
static int getBalance (ClientNode *);

static ClientNode * rightRotate (ClientNode *);
static ClientNode * leftRotate (ClientNode *);
static ClientNode * getClient (Sales, char *);

static ProductNode * leftRotate_P (ProductNode *);
static ProductNode * rightRotate_P (ProductNode *);
static ProductNode * addProduct (ProductNode *, char *);
static ProductNode * createProductNode (char *);
static ProductNode * getProduct (ProductNode *, char *);

/***** AVL Manipulations *****/

/* Retrieve height a node */
static int height (ClientNode * node)
{
  if (node == NULL) return 0;
  return node->height;
}

/* Retrieve height a node */
static int height_P (ProductNode * node)
{
  if (node == NULL) return 0;
  return node->height;
}

/* Return maximum between two numbers */
static int max (int x, int y)
{
  return (x > y ? x : y);
}

/* Create a new client node */
static Sales createNode (char * client)
{
  int i;
  Sales cn = (Sales) malloc(sizeof(struct clientNode));
  strcpy(cn->client, client);
  cn->height = 0;
  cn->left = cn->right = NULL;

  /* Set products pointers to NULL */
  for (i = 0; i < 12; i++)
    cn->products[i] = NULL;

  return cn;
}

/* Right rotate AVL tree */
static ClientNode * rightRotate (ClientNode * node)
{
  ClientNode * aux = NULL;

  aux = node->left;
  node->left = aux->right;
  aux->right = node;

  /* Update heights */
  node->height = max(height(node->left), height(node->right));
  aux->height = max(height(aux->left), height(aux->right));

  return aux;
}

/* Left Rotate AVL tree */
static ClientNode * leftRotate (ClientNode * node)
{
  ClientNode * aux = NULL;

  aux = node->right;
  node->right = aux->left;
  aux->left = node;

  /* Update heights */
  node->height = max(height(node->left), height(node->right));
  aux->height = max(height(aux->left), height(aux->right));

  return aux;
}

/* Right rotate AVL tree */
static ProductNode * rightRotate_P (ProductNode * node)
{
  ProductNode * aux = NULL;

  aux = node->left;
  node->left = aux->right;
  aux->right = node;

  /* Update heights */
  node->height = max(height_P(node->left), height_P(node->right));
  aux->height = max(height_P(aux->left), height_P(aux->right));

  return aux;
}

/* Left Rotate AVL tree */
static ProductNode * leftRotate_P (ProductNode * node)
{
  ProductNode * aux = NULL;

  aux = node->right;
  node->right = aux->left;
  aux->left = node;

  /* Update heights */
  node->height = max(height_P(node->left), height_P(node->right));
  aux->height = max(height_P(aux->left), height_P(aux->right));

  return aux;
}

static int getBalance (ClientNode * node)
{
  if (node == NULL)
    return 0;
  return height(node->left) - height(node->right);
}

/*********************************************************************/

/* Initiate the sales structure */
Sales initSales () {
  int i;
  Sales s = (Sales) malloc(sizeof(struct clientNode));
  s->client[0] = '\0';
  s->height = 0;
  s->left = s->right = NULL;

  /* Set product pointers to NULL */
  for (i = 0; i < 12; i++)
    s->products[i] = NULL;

  return s;
}

/* Function to insert a client in the AVL,
also acts as a moderator to check if structure was not initialized */
Sales insertClients (Sales s, char * client)
{
  /* Sales structure was not initialized */
  if (s == NULL) return NULL;

  return addClient(s, client);
}

/* Add a client to the AVL */
static Sales addClient (Sales s, char * client)
{
  Sales r = NULL;
  int strcomp, balance, i, j;

  if (s == NULL)
  {
    return createNode(client);
  }

  /* First client to be inserted after initialization */
  if (s->client[0] == '\0')
  {
    strcpy(s->client, client);
    r = s;
  } else if (r == NULL) {
    strcomp = strcmp(client, s->client);

    /* Client already inserted */
    if (!strcomp) { r = s; }
    else if (strcomp > 0)
    {
      /* Client trying to be inserted should go to right side of the current node */

      r = s->right = addClient(s->right, client);
    } else {
      /* Client trying to be inserted should go to left side of the current node */

      r = s->left = addClient(s->left, client);
    }

    r = s;

    s->height = max(height(s->left), height(s->right)) + 1;

    balance = height(s->left) - height(s->right);

      /* If this node becomes unbalanced, then there are 4 cases */
    if (s->left == NULL) i = 0;
    else i = strcmp(client, s->left->client);
    if (s->right ==NULL) j =0;
    else j = strcmp(client, s->right->client);

      /* Left Left Case */
    if (balance == 1 && i < 0)
      r = rightRotate(s);

      /* Right Right Case */
    if (balance == -1 && j > 0)
      r = leftRotate(s);

      /* Left Right Case */
    if (balance == 1 && i > 0)
    {
      s->left =  leftRotate(s->left);
      r = rightRotate(s);
    }

      /* Right Left Case */
    if (balance == -1 && j < 0)
    {
      s->right = rightRotate(s->right);
      r = leftRotate(s);
    }
  }


  if (r == NULL) return NULL;
  else return r;
}

/* Remove a client from the sales structure */
Sales removeClients (Sales s, char * client)
{
  int strcomp, balance;
  ClientNode * temp;

  if (!s) return s;

  strcomp = strcmp(client, s->client);

  if (strcomp < 0)
    s->left = removeClients(s->left, client);
  else if (strcomp > 0)
    s->right = removeClients(s->right, client);
  else
  {
    if (!s->left || !s->right)
    {
      temp = (s->left ? s->left : s->right);

      if (!temp)
      {
        temp = s;
        s = 0;
      } else *s = *temp;

      free(temp);
    }
    else
    {
      temp = s->right;
      strcpy(s->client, temp->client);
      s->right = removeClients(s->right, temp->client);
    }
  }

  if (!s) return s;

  s->height = max(height(s->left), height(s->right)) + 1;
  balance = getBalance(s);

  if (balance > 1 && getBalance(s->left) >= 0)
    return rightRotate(s);

  if (balance > 1 && getBalance(s->left) < 0)
  {
    s->left = leftRotate(s->left);
    return rightRotate(s);
  }

  if (balance < -1 && getBalance(s->right) <= 0)
    return leftRotate(s);

  if (balance < -1 && getBalance(s->right) > 0)
  {
    s->right = rightRotate(s->right);
    return leftRotate(s);
  }

  return s;
}

/* Get the client node of a given client code */
static ClientNode * getClient (Sales s, char * client)
{
  int strcomp;

  if (s == NULL) return NULL;

  strcomp = strcmp(client, s->client);

  if (strcomp == 0) return s;
  else if (strcomp > 0) return getClient(s->right, client);
  else return getClient(s->left, client);
}

/*************** PRODUCTS CODE ****************/

static ProductNode * createProductNode (char * product)
{
  ProductNode * node = (ProductNode *) malloc(sizeof( ProductNode ));
  strcpy(node->product, product);
  node->left = node->right = NULL;
  node->quant = 0;

  return node;
}

/* Insert a product in a given client node*/
Sales insertProducts (Sales s, char * client, char * product, int month)
{
  /* Check if client exists on the AVL */
  ClientNode * node = getClient(s, client);

  if (!node) return NULL;
  else if (node->products[month] == NULL)
  {
    node->products[month] = createProductNode(product);
  } else {
    node->products[month] = addProduct(node->products[month], product);
  }

  return s;
}

static ProductNode * addProduct (ProductNode * node, char * product)
{
  ProductNode * result = node;  /* Store the return value */
  int strcomp = 0, balance, i, j;

  if (!node) result = createProductNode(product);

  /*strcmp(product, node->product);*/

  /* Product already inserted */
  if (!strcomp) { result = node; }
  else if (strcomp > 0)
  {
    result = node->right = addProduct(node->right, product);
  }
  else
  {
    result = node->left = addProduct(node->left, product);
  }

  /* Update node height and calculate balance factor */
  node->height = max(height_P(node->left), height_P(node->right)) + 1;
  balance = height_P(node->left) - height_P(node->right);

  if (node->left == NULL) i = 0;
  else i = strcmp(product, node->left->product);
  if (node->right == NULL) j = 0;
  else j = strcmp(product, node->right->product);

  if (balance == 1 && i < 0)
    result = rightRotate_P(node);

    /* Right Right Case */
  if (balance == -1 && j > 0)
    result = leftRotate_P(node);

    /* Left Right Case */
  if (balance == 1 && i > 0)
  {
    node->left =  leftRotate_P(node->left);
    result = rightRotate_P(node);
  }

    /* Right Left Case */
  if (balance == -1 && j < 0)
  {
    node->right = rightRotate_P(node->right);
    result = leftRotate_P(node);
  }

  return result;
}

/* Returns a pointer to the node of that product if it exists, NULL otherwise */
static ProductNode *getProduct (ProductNode * node, char * product)
{
  int strcomp;

  if (node == NULL) return NULL;

  strcomp = strcmp(product, node->product);

  if (strcomp == 0) return node;
  else if (strcomp > 0) return getProduct(node->right, product);
  else return getProduct(node->left, product);
}

/* Query 10 - Create a list of strings with the clients that bought
 * items every month of the year
 * !!!!!!!!!!! NEEDS TESTING */
StrList yearlyClients (Sales sales, StrList list)
{
  int i;
  Bool bought = true;

  if (sales == NULL) return NULL; /* Null node */

  /* Check if client bought at least an item every month */
  for (i = 0; i < 12; i++)
    bought = bought && (sales->products[i] != NULL);


  /* Allocate space to save client and copy client code */
  if (bought)
  {
    list->clients[list->size] = (char *) malloc(sizeof( char ) * 6);
    strcpy(list->clients[list->size], sales->client);
    (list->size)++;
  }

  /* Separate into left and right nodes */
  yearlyClients(sales->left, list);
  yearlyClients(sales->right, list);

  return list;
}
