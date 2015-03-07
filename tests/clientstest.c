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
  int i = 0, validados = 0;

  if ( fp == NULL )
  {
    printf("O ficheiro não existe!\n");
    return 1;
  }

  for (i = 0; fgets(client, 10, fp); i++)
  {
    validados += clients_insert(clients, client);
  }

  printf("Procuar todos os clientes com a inicial: ");
  scanf("%s", client);
  Client * clientslist = clients_search(clients, client[0]);

  if (clientslist == NULL)
  {
    printf("Nenhum cliente com inicial %c.\n", client[0]);
  } else {
    for (int i = 0; clientslist[i].code[0] != '\0'; i++)
    printf("%s\n", clientslist[i].code);
  }

  printf("O ficheiro %s foi lido.\n", filename);
  printf("Foram lidas %d linhas.\n", i);
  printf("Foram validadas %d linhas.\n", validados);

  return 0;
}
