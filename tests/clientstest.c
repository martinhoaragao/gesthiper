#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>       /* To test execution times */
#include "../clients.h"
#include "../Bool.h"
#include "../includes/clist.h"

int main ()
{
  StrList list;
  ClientsCat cat1, cat2;
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
    validated += (insertClient(cat1, client) == NULL ? 0 : 1);
  }

  time(&ftime);   /* Save final time */

  printf("iTime: %ld | fTime: %ld\n", itime, ftime);
  printf("The program took  %f seconds to save all clients.\n", difftime(ftime, itime));

  /* Close file */
  fclose(fp);

  /* Save catalogue 1 into catalogue 2 to make sure clientInsert works */
  cat2 = insertClient(cat1, "FH920");

  /* Open file again to test searching function */
  fp = fopen(filename, "r");

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


  done = 1;
  do {
    printf("Search for client:\n");
    scanf("%s", client);

    strtok(client, "\n");

    if (client[0] == '0') done = 0;
    else printf("%s -> %s\n", client, (searchClient(cat2, client) == true ? "Found" : "NF"));
  } while (done);

  /* Delete the catalogues from memory */
  cat1 = cat2 = deleteCat(cat1);

  printf("Catalogue was %s.\n", (cat1 == NULL ? "removed" : "not removed"));
  printf("The file '%s' was read.\n", filename);
  printf("%d lines were read.\n", nlines);
  printf("%d lines were validated.\n", validated);

  return 0;
}
