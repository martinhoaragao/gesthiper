typedef struct {
  // Guarda o código do cliente, ex. AB234
  char code[6];
} Client;

typedef struct
{
  int next[26];
  Client list[26][3000];
} ClientsList;

// Iniciar estrutura para guardar clientes
ClientsList * clients_init();

// Procurar um cliente pela letra inicial
Client * clients_search(ClientsList *, char c);

// Inserir um cliente na lista
int clients_insert(ClientsList *, char *);