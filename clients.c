#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "clients.h"

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

int clientRemove (char * client) {
  trieNode * aux = clients;
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
        trieNode * temp = aux;
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
