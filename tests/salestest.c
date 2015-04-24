#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../src/sales.h"
#include "../src/clients.h"
#include "../src/products.h"
#include "../includes/bool.h"
#include "../includes/StrList.h"

typedef struct {
  char* productCode;
  double price;
  int number;
  char type;
  char* clientCode;
  int month;
} Tokens;

int invalidc = 0;

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
  if (searchClient(cat1, trim->clientCode) == false) invalidc++;
  if (
    /* There are actually same 0€ sales */
    (trim->price >= 0) &&
    (trim->number > 0) &&
    ((trim->type == 'P') || (trim->type == 'N')) &&
    (trim->month > 0) &&
    (trim->month <= 12) &&
    searchClient(cat1, trim->clientCode) &&
    searchProduct(cat2, trim->productCode)
    ) return trim;
  else return 0;
}

/* Read the sales file and fill the sales structure */
Sales loadSales (ClientsCat cat1, ProductsCat * cat2, char * filename) {
  FILE * fp;
  Tokens * tk;
  int nlines = 0, validated = 0;
  double totalbill = 0;
  char sale[40];
  Sales sales = initSales();

  fp = fopen(filename, "r");
  if ( fp == NULL ){ printf("O ficheiro não existe!\n"); return NULL; }

  while ( fgets(sale, 40 ,fp) ){
    tk = validateSale(cat1, cat2, sale);
    nlines++;
    if (tk) {
      sales = insertClients(sales, tk->clientCode);
      insertProducts(sales, tk->clientCode, tk->productCode, tk->month, tk->number);
      totalbill += (tk->price * tk->number);
      validated++;
    }
    else free(& (tk->productCode));
    free(tk);
  }

  printf("\nO ficheiro '%s' foi lido.\n", filename);
  printf("Foram lidas: %d linhas.\n", nlines);
  printf("Foram validadas %d linhas.\n", validated);
  printf("Total Billing %f\n", totalbill);

  return sales;
}

int main ()
{
  clock_t start, stop;
  double elapsed_t;
  int i;
  StrList list = malloc(sizeof(struct strlist));
  ClientsCat clients = initClients();
  ProductsCat * products = initProductsCat();
  Sales sales = initSales();

  clients = loadCatClients("FichClientes.txt");
  products = loadCatProducts("FichProdutos.txt");
  start = clock();
  sales = loadSales(clients, products, "Compras.txt");
  stop = clock();
  elapsed_t = ((double)stop - start)/CLOCKS_PER_SEC;

  printf("%2.5f to build sales structure\n", elapsed_t);

  printf("%d Invalid clients\n", invalidc);

  list = yearlyClients(sales, list);

  for (i = 0; i < list->size; i++)
    printf("%s   ", list->clients[i]);

  return 0;
}
