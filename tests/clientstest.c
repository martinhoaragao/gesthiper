#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>       // To test execution times
#include "../clients.h"

int main ()
{
  char filename[100];
  time_t itime, ftime;  // Initial time, final time, Difference

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


  time(&itime);   // Save initial time
  clientsInit(); // Iniciar a estrutura dos clientes

  for( i = 0; fgets(client, 10, fp); i++ )
  {
    strtok(client, "\n"); // Clear '\n' before inserting string
    validados += clientInsert(client);
  }
  time(&ftime);   // Save final time

  printf("iTime: %ld | fTime: %ld\n", itime, ftime);
  printf("O programa demorou %f segundos a guardar os clientes.\n", difftime(ftime, itime));

  fclose(fp);

  fp = fopen(filename, "r");

  while ( fgets(client, 10, fp) )
  {
    strtok(client, "\n"); // Clear '\n' before searching for string
    if ( clientsSearch(client) == 0 )
      printf("%s", client);
  }

  int done = 1;

  do {
    printf("Procurar pelo cliente:\n");
    scanf("%s", client);
    if (client[0] == '0') done = 0;
    else printf("%s\n", clientsSearch(client) ? "Encontrado" : "Não Encontrado");
  } while (done);

  /**
  printf("Procuar todos os clientes com a inicial: ");
  scanf("%s", client);

  time(&itime); // Save initial time
  Client * clientslist = clients_search(client[0]);

  if (clientslist == NULL)
  {
    time(&ftime); // Save final time
    printf("Nenhum cliente com inicial %c.\n", client[0]);
  } else {
    time(&ftime); // Save final time
    for (int i = 0; clientslist[i].code[0] != '\0'; i++)
    printf("%s\n", clientslist[i].code);
  }

  printf("%f segundos para encontrar clientes,\n\n", difftime(ftime, itime));
*/
  printf("O ficheiro %s foi lido.\n", filename);
  printf("Foram lidas %d linhas.\n", i);
  printf("Foram validadas %d linhas.\n", validados);

  return 0;
}
