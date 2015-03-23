#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>       /* To test execution times */
#include "../clients.h"

int main ()
{
  CList * list;
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
  clientsInit();  /* Initiate clients structure */ 

  for( nlines = 0; fgets(client, 10, fp); nlines++ )
  {
    strtok(client, "\n"); /* Clear '\n' before inserting string */
    validated += clientInsert(client);
  }
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
    if ( clientsSearch(client) == 0 )
      printf("%s", client);
  }

  list = clientSearchByInit('A');

  for (done = 0; list->next; list = list->next)
    printf("%s\n", list->code);

  do {
    printf("Search for client:\n");
    scanf("%s", client);
    if (client[0] == '0') done = 0;
    else printf("%s\n", clientsSearch(client) ? "Found" : "Not Found");
  } while (done);

  printf("The file '%s' was read.\n", filename);
  printf("%d lines were read.\n", nlines);
  printf("%d lines were validated.\n", validated);

  return 0;
}
