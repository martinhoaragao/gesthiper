#ifndef CLIENTS_H
#define CLIENTS_H

#include "Bool.h"
#include "includes/clist.h"
#include "includes/StrList.h"

typedef struct node * ClientsCat;

/* Initiate clients structure */
ClientsCat initClients();
/* Insert client into the structure */
ClientsCat insertClient (ClientsCat, char *);
/* Search for a client code */
Bool searchClient (ClientsCat, char *);
/* Remove a client from the structure */
int removeClient (ClientsCat, char *);
/* Search clients by initial */
StrList searchClients (ClientsCat, char);
/* Calculate number of clients in a given catalog */
int numOfClients (ClientsCat);
/* Delete a catalogue */
ClientsCat deleteCat(ClientsCat);

#endif
