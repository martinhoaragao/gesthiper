#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>       /* To test execution times */
#include "../src/clients.h"
#include "../includes/bool.h"
#include "../includes/StrList.h"

int main () {
  StrList list;
  ClientsCat cat1, cat2;
  FILE * fp;
  int nlines = 0, validated = 0, done = 1;
  char client[7];
  clock_t start, stop;  /* Initial time, final time */
  double elapsed_t;      /* Elapsed time */

  fp = fopen("FichClientes.txt", "r");

  if ( fp == NULL ) { /* File doesn't exist */
    printf("O ficheiro 'FichClientes.txt' não existe!\n");
    return 1;
  }

  start = clock();   /* Save initial time */
  cat1 = initClients();  /* Initiate clients structure */

  for( nlines = 0; fgets(client, 10, fp); nlines++ ) {
    strtok(client, "\n"); /* Clear '\n' before inserting string */
    validated += (insertClient(cat1, client) == NULL ? 0 : 1);
  }

  stop = clock();                                 /* Save final time */
  elapsed_t = ((double)stop-start)/CLOCKS_PER_SEC; /* Calculate elapsed time */

  printf("--> %2.5f seconds to save all clients.\n", elapsed_t);

  fclose(fp); /* Close file */

  start = clock();  /* Sace initial time */

  /* Save catalogue 1 into catalogue 2 to make sure clientInsert works */
  cat2 = insertClient(cat1, "FH920");

  /* Open file again to test searching function */
  fp = fopen("FichClientes.txt", "r");

  while ( fgets(client, 10, fp) )
  {
    /* Clear '\n' before searching for string */
    strtok(client, "\n");

    if ( searchClient(cat2, client) == false )
      printf("%s", client);
  }

  /* All clients initiated with letter 'A' */
  list = searchClients(cat1, 'A');
  printf("%d clients with initial letter 'A'\n", list->size);

  /* All clients initiated with letter 'B' */
  list = searchClients(cat1, 'B');
  printf("%d clients with initial letter 'B'\n", list->size);

  /* All clients initiated with letter 'C' */
  list = searchClients(cat1, 'C');
  printf("%d clients with initial letter 'C'\n", list->size);

  /* All clients initiated with letter 'D' */
  list = searchClients(cat1, 'D');
  printf("%d clients with initial letter 'D'\n", list->size);

  /* All clients initiated with letter 'E' */
  list = searchClients(cat1, 'E');
  printf("%d clients with initial letter 'E'\n", list->size);

  /* All clients initiated with letter 'F' */
  list = searchClients(cat1, 'F');
  printf("%d clients with initial letter 'F'\n", list->size);

  /* All clients initiated with letter 'G' */
  list = searchClients(cat1, 'G');
  printf("%d clients with initial letter 'G'\n", (list != NULL ? list->size : 0));

  stop = clock(); /* Save final time */
  elapsed_t = ((double)stop-start)/CLOCKS_PER_SEC;

  printf("--> %2.5f seconds to search for all clients and create lists.\n", elapsed_t);

  done = 1;
  do {
    printf("Search for client:\n");
    scanf("%s", client);

    start = clock();

    strtok(client, "\n");

    if (client[0] == '0') done = 0;
    else {
      printf("%s -> %s\n", client, (searchClient(cat2, client) == true ? "Found" : "NF"));
      stop = clock();
      elapsed_t = ((double)stop-start)/CLOCKS_PER_SEC;
      printf("--> The search took %2.5f seconds\n", elapsed_t);
    }
  } while (done);


  printf("Testing remotion!\n");
  printf("%s \n", (searchClient(cat1, "FH920") == true ? "Found" : "NF"));
  cat1 = removeClient(cat1, "FH920");
  printf("%s \n", (searchClient(cat1, "FH920") == true ? "Found" : "NF"));

  /* Delete the catalogues from memory */
  cat1 = cat2 = deleteCat(cat1);

  printf("Catalogue was %s.\n", (cat1 == NULL ? "removed" : "not removed"));
  printf("The file '%s' was read.\n", "FichClientes.txt");
  printf("%d lines were read.\n", nlines);
  printf("%d lines were validated.\n", validated);

  return 0;
}
