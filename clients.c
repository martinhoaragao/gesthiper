#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "clients.h"
#include "Bool.h"
#include "includes/clist.h"
#include "includes/StrList.h"

/* Macros to test if client code is valid */
#define INITIALS (isupper(client[0]) && isupper(client[1]))
#define NUMBERS  (isdigit(client[2]) && isdigit(client[3]) && isdigit(client[4]))

/********* STRUCTURE DEFINITIONS ************/
struct node {
  char value;
  struct node * next;
  struct node * prev;
  struct node * children;
  struct node * parent;
};

/********** FUNCTION PROTOTYPES *******/

static ClientsCat createNode (char);


/****************************************************/

ClientsCat initClients()
{
  return createNode('A');
}

/****************************************************/

/* Insert a client in the trie
Return NULL if client is invalid or the catalogue was not initialized, otherwise
return the Catalogue */
ClientsCat insertClient(ClientsCat cat, char * client)
{
  ClientsCat aux = cat;

  /* Verify if client code is valid , needs revision */
  if (strlen(client) == 6)
    if (!INITIALS || !NUMBERS) return NULL;

  /* Structure not initialized */
  if (cat == NULL) return NULL;

  /* Client already inserted */
  if (*client == '\0') return cat;

  /* Search for the current letter until it's possible */
  for (; (aux->next) && ((aux->value) < *client); aux = aux->next)
    ;

  /* Letter found, no children */
  if ((aux->value == *client) && (aux->children == NULL))
  {
    client++; /* Go to next letter */
    for(; *client != '\0'; aux = aux->children)
    {
        /* Create a node with current letter */
      aux->children = createNode(*client);
      (aux->children)->parent = aux;
        client++; /* Go to next letter */
    }

      return cat; /* Code inserted */
  }
  else if ((aux->value == *client) && (aux->children != NULL))
  {
    client++;
    return ( insertClient(aux->children, client) ? cat : NULL);
  }

  /* Actual value bigger than actual letter */
  if (aux->value > *client)
  {
    /* Create node with actual letter */
    ClientsCat new = createNode(*client);
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

    return cat;
  }

  /* Actual value smaller than actual letter */
  if (aux->value < *client)
  {
    ClientsCat new = createNode(*client);
    new->parent = aux->parent;
    new->prev = aux;
    new->next = aux->next;
    aux->next = new;

    if (new->next) (new->next)->prev = new;

    client++; /* Go to next letter */
    for (; *client != '\0'; new = new->children)
    {
      (new->children) = createNode(*client);
      (new->children)->parent = new;
      client++;
    }

    return cat; /* Client inserted */
  }

  return cat;
}

/****************************************************/

/* Create a Trie Node */
ClientsCat createNode(char clientChar)
{
  ClientsCat node = NULL;
  node = (ClientsCat) malloc(sizeof(struct node));

  node->value = clientChar;
  node->next = NULL;
  node->children = NULL;
  node->parent= NULL;
  node->prev= NULL;
  return node;
}

/****************************************************/

/* Remove a client */
int removeClient (ClientsCat cat, char * client) {
  ClientsCat aux = cat;
  ClientsCat temp = NULL;
  int finished = 0;

  /* Invalid client code */
  if ( !INITIALS || !NUMBERS ) return 0;

  /* Go to last node */
  while ( !finished ) {
    for (; aux->next && (aux->value < *client); aux = aux->next)
      ;

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

/* Search for a client code */
Bool searchClient(ClientsCat cat, char * client)
{
  ClientsCat aux;

  /* Check if client code is valid */
  if (!INITIALS || !NUMBERS) return false;

  aux = cat;

  while(1)
  {
    ClientsCat found = NULL;

    /* End of string reached, string found */
    if (*client == '\0') return true;

    /* Search for the next char */
    for (; aux && !found; aux = aux->next) {
      if (aux->value == *client) found = aux;
    }

    /* The current char was not found, string not found */
    if (found == NULL) return false;

    aux = found->children;
    client++;
  }
}

/****************************************************/

/* Search all clients code given the initial letter */
StrList searchClients (ClientsCat cat, char init) {
  ClientsCat n1, n2, n3, n4;
  char code[6];
  StrList list = (StrList) malloc(sizeof(struct strlist));
  list->size = 0;


  if(!isupper(init)) init = toupper(init);
  code[0] = init;

  for (n1 = cat; n1->next && (n1->value)<init; n1 = n1->next)
    ;
  if (n1->value != init) return NULL;

  for (n1 = n1->children; n1; n1 = n1->next)
    for (n2 = n1->children; n2; n2 = n2->next)
      for (n3 = n2->children; n3; n3 = n3->next)
        for (n4 = n3->children; n4; n4 = n4->next)
        {
          list->clients[list->size] = (char *) malloc(sizeof(char) * 6);
          code[1] = n1->value; code[2] = n2->value;
          code[3] = n3->value; code[4] = n4->value; code[5] = '\0';
          strcpy(list->clients[list->size], code);
          (list->size)++;
        }

  list->clients[list->size] = NULL;
  return list;
}

/****************************************************/

int numOfClients (ClientsCat cat)
{
  ClientsCat lv1, lv2, lv3, lv4, lv5;
  int r = 0;

  for (lv1 = cat; lv1; lv1 = lv1->next)
    for (lv2 = lv1->children; lv2; lv2 = lv2->next)
      for (lv3 = lv2->children; lv3; lv3 = lv3->next)
        for (lv4 = lv3->children; lv4; lv4 = lv4->next)
          for (lv5 = lv4->children; lv5; lv5 = lv5->next)
            r++;

  return r;
}

/****************************************************/

ClientsCat deleteCat (ClientsCat cat)
{
  ClientsCat lv1, lv2, lv3, lv4, lv5, aux;

  for (lv1 = cat; lv1;)
  {
    for (lv2 = lv1->children; lv2;)
    {
      for (lv3 = lv2->children; lv3;)
      {
        for (lv4 = lv3->children; lv4;)
        {
          for (lv5 = lv4->children; lv5;)
          {
            aux = lv5;
            lv5 = lv5->next;
            free(aux);
          }
          aux = lv4;
          lv4 = lv4->next;
          free(aux);
        }
        aux = lv3;
        lv3 = lv3->next;
        free(aux);
      }
      aux = lv2;
      lv2 = lv2->next;
      free(aux);
    }
    aux = lv1;
    lv1 = lv1->next;
    free(aux);
  }
  return NULL;
}
