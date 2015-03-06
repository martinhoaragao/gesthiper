#include "../clients.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main ()
{
  // Criar estrutura para guardar a lista de clientes
  ClientsList * clients = clients_init();
  char filename[100];

  printf("Qual o nome do ficheiro a ler?\n");
  scanf("%s", filename);

  FILE * fp = fopen(filename, "r");
  char clientInitial = 'A';
  char * client = (char *) malloc(sizeof(char) * 7);
  int i = 0;

  if ( fp == NULL )
  {
    printf("O ficheiro não existe!\n");
    return 1;
  }

  for (i = 0; fgets(client, 10, fp); i++)
  {
    int index = client[0] - 'A';
    char * dest = (clients->list[index][clients->next[index]]).code;
    // Retirar '\n' da string
    strtok(client, "\n");
    strcpy(dest, client);
    (clients->next[index])++;
  }

  for (int i = 0; i < 26; i++)
  {
    for (int j = 0; j < clients->next[i]; j++)
      printf("%s\n", clients->list[i][j].code);
  }

  printf("Procuar todos os clientes com a inicial: ");
  scanf("%s", client);
  Client * clientslist = clients_search(clients, client[0]);

  for (int i = 0; clientslist[i].code[0] != '\0'; i++)
    printf("%s\n", clientslist[i].code);

  return 0;
}
