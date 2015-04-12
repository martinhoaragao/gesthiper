/*
Implementation of a sales structure with an AVL of clients and inside each node there
is a AVL for the products bought by those clients
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sales.h"

typedef struct clientNode ClientNode;

/***** Sales.c exclusive functions *****/
static Sales createNode (char *);
static Sales addClient (Sales, char *);
static int height (ClientNode *);
static int max (int, int);
static ClientNode * rightRotate (ClientNode *);
static ClientNode * leftRotate (ClientNode *);

/***** Code *****/

/* Retrieve height a node */
static int height (ClientNode * node)
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
  Sales cn = (Sales) malloc(sizeof(struct clientNode));
  strcpy(cn->client, client);
  cn->height = 0;
  cn->left = cn->right = NULL;

  return cn;
}

/* Righ rotate AVL tree */
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

/*********************************************************************/

/* Initiate the sales structure */
Sales initSales () {
  Sales s = (Sales) malloc(sizeof(struct clientNode));
  s->client[0] = '\0';
  s->height = 0;
  s->left = s->right = NULL;

  return s;
}

/* Function to insert a client in the AVL,
also acts as a moderator to check if structure was not initialized */
Sales insertClient (Sales s, char * client)
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
