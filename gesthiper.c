#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "clients.h"
#include "includes/StrList.h"

int menu () {
  int r = 0;

  printf("\nGESTHIPER\n");
  printf("1: Carregar ficheiro clientes\n");
  printf("2: Carregar ficheiro produtos\n");
  printf("3: Carregar ficheiro compras\n");
  printf("4: Procurar cliente\n");
  printf("5: Lista de clientes por inicial\n");
  printf("6: Sair\n\n");

  scanf("%d", &r);
  return r;
}

/****************************************************/

ClientsCat loadCatClients (char * filename) {
  FILE * fp;
  int nlines, validated = 0;
  char client[100];
  ClientsCat cat;
  fp = fopen(filename, "r");

  if ( fp == NULL ) { printf("O ficheiro não existe!\n"); return NULL; }

  cat = initClients();

  for( nlines = 0; fgets(client, 10, fp); nlines++ )
  {
    strtok(client, "\n"); /* Clear '\n' before inserting string */

    /* !!!! Cuidado pois insertClient tambem devolve diferente de NULL se
    cliente for repetido */
    validated += (insertClient(cat, client) == NULL ? 0 : 1);
  }

  printf("O ficheiro '%s' foi lido.\n", filename);
  printf("Foram lidas %d linhas.\n", nlines);
  printf("Foram validadas %d linhas.\n", validated);

  return cat;
}

/****************************************************/

/* Function to handle the search for clients with a given intiail
and displaying it on the screen */
void clientsList (ClientsCat cat)
{
  Bool done = false;
  int n = 0, lower, upper;
  char input[16];
  StrList list = NULL;

  if (!cat) {
    printf("ERROR! There is not clients catalogue loaded!\nPress Enter\n");
    read(0, input, 16);
    system("clear");
  }
  else {
    printf("Initial:\n");
    read(0, input, 16);

    list = searchClients(cat, input[0]);
    system("clear");


    if (list == NULL) {
      printf("There is no client with the given initial.\nPress Enter.\n");
      read(0, input, 16);
      system("clear");
    } else {
      lower = 0;
      upper = list->size;

      while (!done)
      {
        if (lower < 0) lower = 0;
        else if (lower > upper) lower = upper - 20;

        n = lower;

        printf("Displaying clients from %d to %d of %d clients.\n", n, n + 20, upper);

        for (; n < (lower + 20); n++)
          printf("%s\n", list->clients[n]);

        read(0, input, 16);
        strtok(input, "\n");
        if (!strcmp(input, "stop")) done = true;
        else if (!strcmp(input, "next")) lower += 20;
        else if (!strcmp(input, "back")) lower -= 20;
        system("clear");
      }

      for (n = 0; n < list->size; n++)
        free(list->clients[n]);
    }
  }
  free(list);
}

/*--------------------------MAIN--------------------------*/
int main () {
  ClientsCat cat;
  int choice = 0;
  int done = 0;
  char client[100], filename[100];

  cat = loadCatClients ("clientsfile.txt");
  /*loadCatProducts ("productsfile.txt"); */
 /* loadSales ("salesfile.txt"); */

  while (!done) {
    choice = menu();
    switch( choice ) {
      case 1:
        printf("Qual o nome do ficheiro de clientes?\n");
        scanf("%s", filename);
        cat = loadCatClients(filename); 
        break;
      case 4:
        printf("Indique o nome do cliente: ");
        scanf("%s", client);
        printf("\nO Cliente %s\n", searchClient(cat, client) ? "existe" : "não existe");
        break;
      case 5:
        clientsList(cat); break;
      case 6:
        done = 1; break;
      default:
        break;
    }
  }

  return 0;
}
