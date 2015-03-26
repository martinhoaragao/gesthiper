struct node {
  char value;
  struct node * next;
  struct node * prev;
  struct node * children;
  struct node * parent;
};

typedef struct node * ClientsCat;

typedef struct listnode {
  char code[6];
  struct listnode * next;
} CList;

/* Initiate clients structure */
ClientsCat initClients();
/* Insert client into the structure */
int clientInsert(ClientsCat, char *);
/* Search for a client code */
int clientsSearch(ClientsCat, char *);
/* Remove a client from the structure */
int clientRemove (ClientsCat, char *);
/* Search clients by initial */
CList * clientSearchByInit(char);
/* Clone the clients catalogue */
void clientsClone ();
/* Number of clients that did not bought any product */
int poorClients ();
