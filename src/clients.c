#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "clients.h"
#include "../includes/bool.h"
#include "../includes/StrList.h"

/********* STRUCTURE DEFINITIONS ************/
struct node {
  char value;                 /* Character to store */
  struct node * next;         /* Next node */
  struct node * prev;         /* Previous node */
  struct node * children;     /* Children node */
  struct node * parent;       /* Parent node */
};

/********** FUNCTION PROTOTYPES *******/

static ClientsCat createNode (char);


/****************************************************/

/* Initiate clients catalogue with letter 'A' as root */
ClientsCat initClients() {
  return createNode('A');
}

/****************************************************/

/* Insert a client in the trie
Return NULL if client is invalid or the catalogue was not initialized, otherwise
return the Catalogue */
ClientsCat insertClient(ClientsCat cat, char * client) {
  ClientsCat aux = cat;

  /* Verify if client code is valid , strlen ensures this only runs first time*/
  if ((strlen(client) == 6) && (!validateClient(client))) return NULL;

  if (cat == NULL) return NULL;     /* Structure not initialized */

  if (*client == '\0') return cat;  /* Client already inserted */

  /* Search for the current letter until it's possible */
  for (; (aux->next) && ((aux->value) < *client); aux = aux->next)
    ;

  /* Letter found, no children */
  if ((aux->value == *client) && (aux->children == NULL)) {
    client++;                                     /* Go to next char */

    for(; *client != '\0'; aux = aux->children) {
      aux->children = createNode(*client);  /* Create node with current char */
      (aux->children)->parent = aux;        /* Save parent pointer */
      client++;                             /* Go to next char */
    }

    return cat; /* Client inserted */
  }
  else if ((aux->value == *client) && (aux->children != NULL)) {
    client++; /* Go to next char */
    return ( insertClient(aux->children, client) ? cat : NULL);
  }

  if (aux->value > *client) { /* Actual char bigger than node char */
    ClientsCat new = createNode(*client); /* Create node with actual char */
    new->parent = aux->parent;            /* Save parent node pointer */
    new->prev = aux->prev;                /* Save previous node pointer */
    new->next = aux;                      /* Save next node pointer */
    aux->prev = new;                      /* Save node as previous node of next node */

    /* If there are no previous node then this node will be the parents children */
    if (new->prev == NULL) (new->parent)->children = new;
    else (new->prev)->next = new; /* Set new node as next node of previous node */

    client++; /* Go to next char */
    for (;*client != '\0'; new = new->children) {
      new->children = createNode(*client);  /* Create node with actual char */
      (new->children)->parent = new;        /* Save parent node pointer */
      client++;                             /* Go to next char */
    }

    return cat; /* Client inserted */
  }

  if (aux->value < *client) { /* Actual char smaller than node char */
    ClientsCat new = createNode(*client); /* Create node with actual char */
    new->parent = aux->parent;            /* Save parent node pointer */
    new->prev = aux;                      /* Save previous node pointer */
    new->next = aux->next;                /* Save next node pointer */
    aux->next = new;                      /* Save node as next node of previous node */

    if (new->next) (new->next)->prev = new; /* Save node as previous node of next node */

    client++; /* Go to next char */
    for (; *client != '\0'; new = new->children) {
      (new->children) = createNode(*client);  /* Create node with actual char */
      (new->children)->parent = new;          /* Save parent node pointer */
      client++;                               /* Go to next char */
    }

    return cat; /* Client inserted */
  }

  return cat;
}

/****************************************************/

/* Create a Trie Node with the given char */
ClientsCat createNode(char clientChar) {
  ClientsCat node = NULL;
  node = (ClientsCat) malloc(sizeof(struct node));  /* Allocate space for new node */

  node->value = clientChar;                         /* Save the given char */
  node->next = NULL;                                /* Set all pointers to NULL */
  node->children = NULL;
  node->parent = NULL;
  node->prev = NULL;
  return node;
}

/****************************************************/

/* Remove a client */
ClientsCat removeClient (ClientsCat cat, char * client) {
  ClientsCat aux = cat;     /* Auxiliar pointer */
  ClientsCat temp = NULL;   /* To save pointers to be freed */
  Bool finished = false;    /* Control while loops */

  if (!validateClient(client)) return cat;  /* Invalid client code */
  if (!cat) return NULL;                    /* Catalogue is empty */

  while ( !finished ) { /* Loop to check if client is in the Trie */
    for (; (*client != '\0') && aux->next && (aux->value < *client); aux = aux->next)
      ;

    if (*client == '\0') finished = true; /* Client found */
    else if (aux->value == *client) {
        client++;                               /* Go to next char */
        if (aux->children) aux = aux->children; /* Go to children */
    }
    else return cat;  /* Client is not in the trie */
  }

  finished = false;
  while ( !finished ) {               /* Loop to remove client from the trie */
    if ( aux->parent == NULL ) {      /* Top level char */
      if (aux->children == NULL)  {   /* No children with the initial */
        if (aux->prev) (aux->prev)->next = aux->next;
        if (aux->next) (aux->next)->prev = aux->prev;
        temp = aux;
        aux = ((aux->prev == NULL) ? aux->next : aux->prev);
        free(temp);
      }

      finished = true;  /* Client removed */
    }
    else {  /* Node with parent */
      if (aux->next) (aux->next)->prev = aux->prev; /* Change next node previous pointer */

      if (aux->prev) (aux->prev)->next = aux->next; /* Change previous node next pointer */
      else (aux->parent)->children = aux->next;     /* Change parent's children pointer */

      temp = aux;                                   /* Save pointer to be freed */
      aux = aux->parent;                            /* Go up in the trie */
      free(temp);                                   /* Free unused node */
    }
  }

  return aux; /* Client removed */
}

/****************************************************/

/* Search for a client code */
Bool searchClient(ClientsCat cat, char * client) {
  ClientsCat aux = cat, found = NULL; /* Auxiliar pointer, pointer to store found node */
  Bool searching = true;              /* Control while loop */

  if (!validateClient(client)) return false;  /* Check if client code is valid */

  while (searching == true) {
    found = NULL;

    if (*client == '\0') { found = cat; searching = false; }/* Client found */

    for (; aux && searching && !found; aux = aux->next) /* Search for current char */
      if (aux->value == *client) found = aux;

    if (found == NULL) searching = false; /* Current char not found, client not found */
    else if (searching) { client++; aux = found->children; }
  }

  return (found != NULL);
}

/****************************************************/

/* Search all clients with given the initial letter and create a lista of
 * those clients */
StrList searchClients (ClientsCat cat, char init) {
  ClientsCat n1, n2, n3, n4;  /* Level pointers */
  char code[6];               /* To build client codes */
  StrList list = NULL;        /* To store client codes */


  if(!isupper(init)) init = toupper(init);  /* Convert initial char to uppercase */

  for (n1 = cat; n1->next && (n1->value) < init; n1 = n1->next)
    ;

  if (n1->value != init) return NULL; /* No clients with the given initial */
  else {
    list = (StrList) malloc(sizeof(struct strlist));  /* Allocate space for lsit */
    list->size = 0;
  }

  for (n1 = n1->children; n1; n1 = n1->next)
    for (n2 = n1->children; n2; n2 = n2->next)
      for (n3 = n2->children; n3; n3 = n3->next)
        for (n4 = n3->children; n4; n4 = n4->next) {
          list->clients[list->size] = (char *) malloc(sizeof(char) * 6);
          sprintf(code, "%c%c%c%c%c", init, n1->value, n2->value, n3->value, n4->value);
          strcpy(list->clients[list->size], code);
          (list->size)++;
        }

  return list;
}

/****************************************************/

/* Compute number of clients in the catalogue */
int numOfClients (ClientsCat cat) {
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

/* Free all the memory used by the clients catalogue */
ClientsCat deleteCat (ClientsCat cat) {
  ClientsCat lv1, lv2, lv3, lv4, lv5, aux;

  for (lv1 = cat; lv1;) {
    for (lv2 = lv1->children; lv2;) {
      for (lv3 = lv2->children; lv3;) {
        for (lv4 = lv3->children; lv4;) {
          for (lv5 = lv4->children; lv5;) {
            aux = lv5;            /* Save pointer to be freed */
            lv5 = lv5->next;      /* Update actual pointer */
            free(aux);            /* Free the node */
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

/* Check if a given client code is valid, return true if it is and false otherwise */
Bool validateClient (char * client) {
  Bool valid = true;

  valid = valid && (isupper(client[0]) && isupper(client[1]));
  valid = valid && (isdigit(client[2]) && isdigit(client[3]) && isdigit(client[4]));

  return valid;
}
