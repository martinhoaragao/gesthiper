#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "clients.h"

/* Auxiliar function to clientInsert */
static int clientInsertAux (trieNode *, char *);
/* Create a trie node given a char */
static trieNode * createNode (char);
static void cloneAux (int *, trieNode *, trieNode *);

/* Clients structure to save clients who didnt bought anything */
static trieNode * cheapClients = NULL;
/* Clients Trie */
static trieNode * clients;

/****************************************************/

void clientsInit()
{
 clients = createNode('A');
}

/****************************************************/

/* Insert a client in the trie */
int clientInsert(char * client)
{
  /* Check if two initials are uppercase */
  int initials = isupper(client[0]) && isupper(client[1]);
  /* Check if the last three characters are numbers */
  int numbers = isdigit(client[2]) && isdigit(client[3]) && isdigit(client[4]);

  if (initials && numbers) return clientInsertAux(clients, client);
  return 0;
}

/****************************************************/

/* Add a Client to the Trie */
int clientInsertAux(trieNode * trie, char * client) {
  trieNode * aux = trie;

  if (*client == '\0') return 0;

  /* Search for the current letter until it's possible */
  for (; (aux->next) && ((aux->value) < * client); aux = aux->next)
    ;

  /* Letter found */
  if (aux->value == *client)
  {
    /* No children */
    if (aux->children == NULL)
    {
      client++; /* Go to next letter */
      for(; *client != '\0'; aux = aux->children)
      {
        /* Create a node with current letter */
        aux->children = createNode(*client);
        (aux->children)->parent = aux;
        client++; /* Go to next letter */
      }

      return 1; /* Code inserted */
    }

    /* There are children */
    client++; /* Go to next letter */
    return clientInsertAux(aux->children, client);
  }

  /* Actual value bigger than actual letter */
  if (aux->value > *client)
  {
    /* Create node with actual letter */
    trieNode * new = createNode(*client);
    new->parent = aux->parent;
    new->prev = aux->prev;
    new->next = aux;
    aux->prev = new;

    if (new->prev == NULL) (new->parent)->children = new;
    else (new->prev)->next = new;

    client++; /* Go to next letter */
    for (;*client != '\0'; new = new->children)
    {
      new->children = createNode(*client);
      (new->children)->parent = new;
      client++;
    }

    return 1;
  }

  /* Actual value smaller than actual letter */
  if (aux->value < *client)
  {
    trieNode * new = createNode(*client);
    new->parent = aux->parent;
    new->prev = aux;
    new->next = aux->next;
    aux->next = new;


    if (new->next != NULL)
      (new->next)->prev = new;

    client++; /* Go to next letter */
    for (; *client != '\0'; new = new->children)
    {
      (new->children) = createNode(*client);
      (new->children)->parent = new;
      client++;
    }

    return 1; /* Client inserted */
  }
  return 1;
}

/****************************************************/

/* Create a Trie Node */
trieNode * createNode(char clientChar)
{
  trieNode * node = NULL;
  node = (trieNode *) malloc(sizeof(trieNode));

  node->value = clientChar;
  node->next = NULL;
  node->children = NULL;
  node->parent= NULL;
  node->prev= NULL;
  return node;
}

/****************************************************/

/* Search for a client code */
int clientsSearch(char * client)
{
  trieNode * aux = clients;

  while(1)
  {
    trieNode * found = NULL;

    /* End of string reached, string found */
    if (*client == '\0') return 1;

    /* Search for the next char */
    for (; aux && !found; aux = aux->next)
      if (aux->value == *client) found = aux;

    /* The current char was not found, string not found */
    if (found == NULL) return 0;

    aux = found->children;
    client++;
  }
}

/****************************************************/

/* Remove a client */
int clientRemove (char * client) {
  trieNode * aux = clients;
  trieNode * temp = NULL;
  int finished = 0;

  /* Check if two initials are uppercase */
  int initials = isupper(client[0]) && isupper(client[1]);
  /* Check if the last three characters are numbers */
  int numbers = isdigit(client[2]) && isdigit(client[3]) && isdigit(client[4]);

  /* Invalid client code */
  if ( !initials || !numbers ) return 0;

  /* Go to last node */
  while ( !finished ) {
    for (; aux->next && (aux->value < *client); aux = aux->next) ;

    if (aux->value == *client) {
      client++;
      if (aux->children != NULL) aux = aux->children;
    } else return 0;

    /* Finished string */
    if (*client == '\0') finished = 1;
  }

  finished = 0;
  while ( !finished ) {
    /* Top level letter */
    if ( aux->parent == NULL && aux->children == NULL ) {
      if ( aux->next ) (aux->next)->prev = aux->prev;
      if ( aux->prev ) (aux->prev)->next = aux->next;
      free(aux);
      finished = 1;
    }

    if ( aux->parent ) {
      /* Only children */
      if ( aux->next == NULL && (aux->parent)->children == aux ) {
        (aux->parent)->children = NULL;
        temp = aux;
        aux = aux->parent;
        free(temp);
      }
      else if ( aux->next != NULL && (aux->parent)->children == aux ) {
        (aux->next)->prev = NULL;
        (aux->parent)->children = aux->next;
        free(aux);
        finished = 1;
      }
      else if ( aux->prev != NULL ) {
        if ( aux->next ) {
          (aux->prev)->next = aux->next;
          (aux->next)->prev = aux->prev;
        } else {
          (aux->prev)->next = NULL;
        }
        finished = 1;
      }
    }

  }

  return 1; /* Client removed */
}

/****************************************************/

/* Search all clients code given the initial letter */
CList * clientSearchByInit (char init) {
  char code[6];
  trieNode * n1, * n2, * n3, * n4;
  CList * result = (CList *) malloc(sizeof(CList));
  CList * root = result;
  result->next = NULL;

  if(!isupper(init)) init = toupper(init);
  code[0] = init;

  n1 = clients;
  for (; n1->next && (n1->value)<init; n1 = n1->next)
    ;
  if (n1->value != init) return NULL;

  for (n1 = n1->children; n1; n1 = n1->next)
    for (n2 = n1->children; n2; n2 = n2->next)
      for (n3 = n2->children; n3; n3 = n3->next)
        for (n4 = n3->children; n4; n4 = n4->next)
        {
          code[1] = n1->value; code[2] = n2->value;
          code[3] = n3->value; code[4] = n4->value; code[5] = '\0';
          strcpy(result->code, code);
          result->next = (CList *) malloc(sizeof(CList));
          result = result->next;
          result->next = NULL;
        }
  return root;
}

/****************************************************/

/* Clone the clients catalogue to the catalogue of clients who bought nothing */
void clientsClone (){
  trieNode * cat;                 /* Top level pointer */
  trieNode * desc;                /* Descender pointer */
  trieNode * lv1, * lv2, * lv3, * lv4, * lv5;
  int st1 = 0, st2 = 0, st3 = 0, st4 = 0, st5 = 0;

  for (lv1 = clients; lv1; lv1 = lv1->next)
  {
    st2 = 0;
    st3 = 0;
    st4 = 0;
    st5 = 0;

    if ( !(st1) ) {
      cheapClients = createNode(clients->value);
      cat = cheapClients;
      st1 = 1;
    } else {
      cloneAux( &st1, cat, lv1 );
    }

    desc = cat;

    for (lv2 = lv1->children; lv2; lv2 = lv2->next)
    {
      cloneAux( &st2, desc, lv2 );

      for (lv3 = lv2->children; lv3; lv3 = lv3->next)
      {
        cloneAux( &st3, desc, lv3 );

        for (lv4 = lv3->children; lv4; lv4 = lv4->next)
        {
          cloneAux( &st4, desc, lv4 );

          for (lv5 = lv4->children; lv5; lv5 = lv5->next)
          {
            cloneAux( &st5, desc, lv5 );
          }
        }
      }
    }
  }
}

/* Auxiliar function to help creating the clone catalog nodes */
void cloneAux ( int * status, trieNode * node, trieNode * originnode ) {
  if ( *status )
  {
    /* First time at the given level */
    node->children = createNode(originnode->value);
    (node->children)->parent = node->parent;
    node = node->children;
    *status = 1;
  } else {
    node->next = createNode(originnode->value);
    (node->next)->prev = node;
    (node->next)->parent = node->parent;
    node = node->next;
  }
}
