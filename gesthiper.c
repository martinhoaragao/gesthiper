#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clients.h"

int menu ();
ClientsCat carregarClientes ();

int main ()
{
  ClientsCat cat;
  int choice = 0;
  int done = 0;
  char cliente[100];

  while (!done)
  {
    choice = menu();
    switch( choice )
    {
      case 1:
        cat = carregarClientes(); break;
      case 4:
        scanf("%s", cliente);
        printf("O Cliente %s\n", searchClient(cat, cliente) ? "existe" : "não existe");
        break;
      case 5:
        done = 1; break;
      default:
        break;
    }
  }

  return 0;
}

int menu ()
{
  int r = 0;

  printf("GESTHIPER\n");
  printf("1: Carregar ficheiro clientes\n");
  printf("2: Carregar ficheiro produtos\n");
  printf("3: Carregar ficheiro compras\n");
  printf("4: Procuar cliente\n");
  printf("5: Sair\n\n");

  scanf("%d", &r);
  return r;
}

ClientsCat carregarClientes ()
{
  FILE * fp;
  int nlines, validated = 0;
  char filename[100], client[100];
  ClientsCat cat;

  printf("Qual o nome do ficheiro de clientes?\n");
  scanf("%s", filename);
  
  fp = fopen(filename, "r");

  if ( fp == NULL ) { printf("O ficheiro não existe!\n"); return NULL; }

  cat = initClients();

  for( nlines = 0; fgets(client, 10, fp); nlines++ )
  {
    strtok(client, "\n"); /* Clear '\n' before inserting string */
    validated += insertClient(cat, client);
  }

  printf("O ficheiro '%s' foi lido.\n", filename);
  printf("Foram lidas %d linhas.\n", nlines);
  printf("Foram validadas %d linhas.\n", validated);

  return cat;
}
