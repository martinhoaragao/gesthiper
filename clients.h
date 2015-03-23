typedef struct trieNode {
    char value;
    struct trieNode * next;
    struct trieNode * prev;
    struct trieNode * children;
    struct trieNode * parent;
} trieNode;

void clientsInit();
int clientInsert(char *);
static trieNode * createNode(char);
int clientsSearch(char *);
static int clientInsertAux(trieNode *, char *);
int clientRemove (char *);
/* void TrieRemove(trieNode_t **root, char *key); */
