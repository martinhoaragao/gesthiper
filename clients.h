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
int insertClient (ClientsCat, char *);
/* Search for a client code */
int searchClient (ClientsCat, char *);
/* Remove a client from the structure */
int removeClient (ClientsCat, char *);
/* Search clients by initial */
CList * searchClients (ClientsCat, char);
