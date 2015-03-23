typedef struct trieNode {
    char value;
    struct trieNode * next;
    struct trieNode * prev;
    struct trieNode * children;
    struct trieNode * parent;
} trieNode;

/* Initiate clients structure */
void clientsInit();
/* Insert client into the structure */
int clientInsert(char *);
/* Auxiliar function to search for a client */
static int clientInsertAux(trieNode *, char *);
/* Create a trie node with the char passed */
static trieNode * createNode(char);
/* Search for a client code */
int clientsSearch(char *);
/* Remove a client from the structure */
int clientRemove (char *);
