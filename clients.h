typedef struct {
  // Guarda o código do cliente, ex. AB234
  char code[6];
} Client;

typedef struct
{
  int next[26];
  Client list[26][3000];
} ClientsList;

ClientsList * clients_init();
Client * clients_search(ClientsList *, char c);
