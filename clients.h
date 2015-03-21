typedef struct trieNode {
    char value;
    struct trieNode * next;
    struct trieNode * prev;
    struct trieNode * children;
    struct trieNode * parent;
} trieNode;

void clientsInit();
int clientInsert(char *);
trieNode * createNode(char);
int clientsSearch(char *);
int clientInsertAux(trieNode *, char *);
/* void TrieRemove(trieNode_t **root, char *key); */
