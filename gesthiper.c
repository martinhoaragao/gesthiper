#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>  /* To test execution times */


#include "accounting.h"
#include "clients.h"
#include "products.h"
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

  printf("\nO ficheiro '%s' foi lido.\n", filename);
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


/*--------------------------Products--------------------------*/

ProductsCat * loadCatProducts (char * filename) {
  FILE * fp;
  int nlines, validated = 0;
  char client[100];
  ProductsCat * cat;
  fp = fopen(filename, "r");

  if ( fp == NULL ) { printf("O ficheiro não existe!\n"); return NULL; }

  cat = initProductsCat();;

  for( nlines = 0; fgets(client, 10, fp); nlines++ )
  {
    strtok(client, "\n"); /* Clear '\n' before inserting string */

    validated += ( insert_product(cat, client) == 0 ? 0 : 1);
  }

  printf("\nO ficheiro '%s' foi lido.\n", filename);
  printf("Foram lidas %d linhas.\n", nlines);
  printf("Foram validadas %d linhas.\n", validated);

  return cat;
}


/*--------------------------Accounting--------------------------*/
/*
 * Helper function to trim
 * @param Receives string from sales file
 * @return Tokens in a struct
 */
static Tokens* trimSale(char* s){
  Tokens* trim = (Tokens*) malloc(sizeof(Tokens));
  trim->productCode = (char *) malloc(sizeof(char) * 7);
  trim->clientCode = (char *) malloc(sizeof(char) * 6);

  strncpy(trim->productCode, strtok(s, " "), 7);
  trim->price = atof( strtok(0, " "));
  trim->number = atoi( strtok(0, " "));
  trim->type = strtok(0, " ")[0];
  strncpy(trim->clientCode, strtok(0, " "), 6);
  trim->month = atoi( strtok(0, "\n"));
  return trim;
}

/*
 * Validates a line
 * @param String with all the info for one sale;
 * @return Tokens in a struct
 */
static Tokens* validateSale(ClientsCat cat1, ProductsCat * cat2, char* s){
  Tokens* trim = trimSale(s);
  if (
    /* There are actually same 0€ sales */
    (trim->price >= 0) &&
    (trim->number > 0) &&
    ((trim->type == 'P') || (trim->type == 'N')) &&
    (trim->month > 0) &&
    (trim->month <= 12) &&
    searchClient(cat1, trim->clientCode) &&
    search(cat2, trim->productCode)
    ) return trim;
  else return 0;
}

Accounting * loadSales (ClientsCat cat1, ProductsCat * cat2, char * filename) {
  FILE * fp;
  Tokens * tk;
  int nlines = 0, validated = 0;
  time_t itime, ftime; /* Times for clients and accounting load*/
  double totalbill = 0;
  char sale[40];
  Accounting * cat3;

  time(&itime);
  fp = fopen(filename, "r");
  if ( fp == NULL ){ printf("O ficheiro não existe!\n"); return NULL; }
  cat3 = initAccounting();

  while ( fgets(sale, 40 ,fp) ){
    tk = validateSale(cat1, cat2, sale);
    nlines++;
    if (tk) {
      totalbill += (tk->price * tk->number);
      insertAccounting(cat3, tk);
      validated++;
    }
    free(tk);
  }

  time(&ftime);

  printf("\nO ficheiro '%s' foi lido.\n", filename);
  printf("Demorou: %.f segundos\n", difftime(ftime, itime));
  printf("Foram lidas: %d linhas.\n", nlines);
  printf("Foram validadas %d linhas.\n", validated);
  printf("Total Billing %f\n", totalbill);

  return cat3;
}


/*--------------------------MAIN--------------------------*/
int main () {
  ClientsCat cat1;
  ProductsCat * cat2;
  Accounting * cat3; 
  int choice = 0;
  int done = 0;
  char client[100], filename[100];

  cat1 = loadCatClients ("clientsfile.txt");
  cat2 = loadCatProducts ("productsfile.txt");
  cat3 = loadSales (cat1, cat2, "salesfile.txt"); 

  while (!done) {
    choice = menu();
    switch( choice ) {
      case 1:
        printf("Qual o nome do ficheiro de clientes?\n");
        scanf("%s", filename);
        cat1 = loadCatClients(filename); 
        break;
      case 2:
        printf("Qual o nome do ficheiro de produtos?\n");
        scanf("%s", filename);
        cat2 = loadCatProducts(filename); 
        break;
      case 3:
        printf("Qual o nome do ficheiro de compras?\n");
        scanf("%s", filename);
        cat3 = loadSales(cat1, cat2, filename); 
        break;
      case 4:
        printf("Indique o nome do cliente: ");
        scanf("%s", client);
        printf("\nO Cliente %s\n", searchClient(cat1, client) ? "existe" : "não existe");
        break;
      case 5:
        clientsList(cat1); break;
      case 6:
        done = 1; break;
      default:
        break;
    }
  }

  return 0;
}
