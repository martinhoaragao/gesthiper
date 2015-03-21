#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "clients.h"

/* Clients Trie */
trieNode * clients;

/****************************************************/

void clientsInit()
{
 clients = createNode('A');
}

/****************************************************/

/* Insert a client in the trie */
int clientInsert(char * client)
{
  /* !! Check if client code is composed by two letter and 3 numbers */
  return clientInsertAux(clients, client);
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

      (aux->children) = createNode('\0'); /* Finish string*/
      (aux->children)->parent = aux;
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

    (new->children) = createNode('\0');
    (new->children)->parent = new;
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

    (new->children) = createNode('\0'); /* Finish String */
    (new->children)->parent = new;
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

/*
void TrieRemove(trieNode_t **root, char *key)
{
 trieNode_t *tPtr = NULL;
 trieNode_t *tmp = NULL;

 if(NULL == *root || NULL == key)
  return;

tPtr = TrieSearch((*root)->children, key);

if(NULL == tPtr)
{
  printf("Key [%s] not found in trie\n", key);
  return;
}

#ifdef DEBUG
printf("Deleting key [%s] from trie\n", key);
#endif

while(1)
{
  if( tPtr->prev && tPtr->next)
  {
   tmp = tPtr;
   tPtr->next->prev = tPtr->prev;
   tPtr->prev->next = tPtr->next;
#ifdef DEBUG
   printf("Deleted [%c] \n", tmp->key);
#endif
   free(tmp);
   break;
 }
 else if(tPtr->prev && !(tPtr->next))
 {
   tmp = tPtr;
   tPtr->prev->next = NULL;
#ifdef DEBUG
   printf("Deleted [%c] \n", tmp->key);
#endif
   free(tmp);
   break;
 }
 else if(!(tPtr->prev) && tPtr->next)
 {
   tmp = tPtr;
   tPtr->parent->children = tPtr->next;
#ifdef DEBUG
   printf("Deleted [%c] \n", tmp->key);
#endif
   free(tmp);
   break;
 }
 else
 {
   tmp = tPtr;
   tPtr = tPtr->parent;
   tPtr->children = NULL;
#ifdef DEBUG
   printf("Deleted [%c] \n", tmp->key);
#endif
   free(tmp);
 }
}

#ifdef DEBUG
printf("Deleted key [%s] from trie\n", key);
#endif
}


void TrieDestroy( trieNode_t* root )
{
 trieNode_t *tPtr = root;
 trieNode_t *tmp = root;

 while(tPtr)
 {
  while(tPtr->children)
   tPtr = tPtr->children;

 if( tPtr->prev && tPtr->next)
 {
   tmp = tPtr;
   tPtr->next->prev = tPtr->prev;
   tPtr->prev->next = tPtr->next;
#ifdef DEBUG
   printf("Deleted [%c] \n", tmp->key);
#endif
   free(tmp);
 }
 else if(tPtr->prev && !(tPtr->next))
 {
   tmp = tPtr;
   tPtr->prev->next = NULL;
#ifdef DEBUG
   printf("Deleted [%c] \n", tmp->key);
#endif
   free(tmp);
 }
 else if(!(tPtr->prev) && tPtr->next)
 {
   tmp = tPtr;
   tPtr->parent->children = tPtr->next;
   tPtr->next->prev = NULL;
   tPtr = tPtr->next;
#ifdef DEBUG
   printf("Deleted [%c] \n", tmp->key);
#endif
   free(tmp);
 }
 else
 {
   tmp = tPtr;
   if(tPtr->parent == NULL)
   {
    // Root
    free(tmp);
    return;
  }
  tPtr = tPtr->parent;
  tPtr->children = NULL;
#ifdef DEBUG
  printf("Deleted [%c] \n", tmp->key);
#endif
  free(tmp);
}
}
*/
