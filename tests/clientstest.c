#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>       /* To test execution times */
#include "../clients.h"

int main ()
{
  CList * list;
  ClientsCat cat1;
  FILE * fp;
  int nlines = 0, validated = 0, done = 1;
  char * client = (char *) malloc(sizeof(char) * 7);
  char filename[100];
  time_t itime, ftime;  /* Initial time, final time, Difference */

  printf("What's the name of the file to read?\n");
  scanf("%s", filename);

  fp = fopen(filename, "r");

  /* File doesn't exist */
  if ( fp == NULL )
  {
    printf("O ficheiro não existe!\n");
    return 1;
  }

  time(&itime);   /* Save initial time */
  cat1 = initClients();  /* Initiate clients structure */

  for( nlines = 0; fgets(client, 10, fp); nlines++ )
  {
    strtok(client, "\n"); /* Clear '\n' before inserting string */
    validated += insertClient(cat1, client);
  }

  /* Clone the clients catalogue to a new one */

  time(&ftime);   /* Save final time */

  printf("iTime: %ld | fTime: %ld\n", itime, ftime);
  printf("The program took  %f seconds to save all clients.\n", difftime(ftime, itime));

  /* Close file */
  fclose(fp);

  /* Open file again to test searching function */
  fp = fopen(filename, "r");

  while ( fgets(client, 10, fp) )
  {
    strtok(client, "\n"); /* Clear '\n' before searching for string */
    if ( searchClient(cat1, client) == 0 )
      printf("%s", client);
  }

  /* All clients initiated with letter 'A' */
  list = searchClients(cat1, 'A');
  for (done = 0; list && list->next; list = list->next)
    done++;
  printf("%d clients with initial letter 'A'\n", done);

  /* All clients initiated with letter 'B' */
  list = searchClients(cat1, 'B');
  for (done = 0; list && list->next; list = list->next)
    done++;
  printf("%d clients with initial letter 'B'\n", done);

  /* All clients initiated with letter 'C' */
  list = searchClients(cat1, 'C');
  for (done = 0; list && list->next; list = list->next)
    done++;
  printf("%d clients with initial letter 'C'\n", done);

  /* All clients initiated with letter 'D' */
  list = searchClients(cat1, 'D');
  for (done = 0; list && list->next; list = list->next)
    done++;
  printf("%d clients with initial letter 'D'\n", done);

  /* All clients initiated with letter 'E' */
  list = searchClients(cat1, 'E');
  for (done = 0; list && list->next; list = list->next)
    done++;
  printf("%d clients with initial letter 'E'\n", done);

  /* All clients initiated with letter 'F' */
  list = searchClients(cat1, 'F');
  for (done = 0; list && list->next; list = list->next)
    done++;
  printf("%d clients with initial letter 'F'\n", done);

  /* All clients initiated with letter 'G' */
  list = searchClients(cat1, 'G');
  for (done = 0; list && list->next; list = list->next)
    done++;
  printf("%d clients with initial letter 'G'\n", done);


  done = 1;
  do {
    printf("Search for client:\n");
    scanf("%s", client);
    if (client[0] == '0') done = 0;
    else printf("%s -> %s\n", client, searchClient(cat1, client) ? "Found" : "Not Found");
  } while (done);

  printf("The file '%s' was read.\n", filename);
  printf("%d lines were read.\n", nlines);
  printf("%d lines were validated.\n", validated);

  return 0;
}
