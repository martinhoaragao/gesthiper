/*
Implementation of a sales structure with an AVL of clients and inside each node there
is a AVL for the products bought by those clients
*/

#include <stdlib.h>
#include <string.h>
#include "sales.h"

/***** Sales.c exclusive functions *****/
Sales createNode (char *);
Sales addClient (Sales, char *);

/***** Code *****/

/* Create a new client node */
Sales createNode (char * client)
{
  Sales cn = (Sales) malloc(sizeof(struct clientNode));
  strcpy(cn->client, client);
  cn->height = 0;
  cn->left = cn->right = NULL;

  return cn;
}

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
Sales addClient (Sales s, char * client)
{
  Sales r = s;
  int strcomp;

  if (s == NULL)
  {
    return createNode(client);
  }

  /* First client to be inserted after initialization */
  if (s->client[0] == '\0')
  {
    strcpy(s->client, client);
    r = s;
  }

  strcomp = strcmp(client, s->client);

  /* Client already inserted */
  if (!strcomp) { r = s; }
  else if (strcomp > 0)
  {
    /* Client trying to be inserted should go to right side of the current node */

    s->right = addClient(s, client);
    /* TODO: Update height and check if rotation is needed */
    r = s;
  } else {
    /* Client trying to be inserted should go to left side of the current node */

    s->left = addClient(s->left, client);
    /* TODO: Update height and check if rotation is needed */
    r = s;
  }

  if (r == NULL) return NULL;
  else return s;
}
