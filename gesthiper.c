#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h> /* To test execution times */
#include <math.h>  /* To use ceil() */


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
  printf("5: Procurar compras de um produto num mês\n");
  printf("6: Procurar compras num intervalo de meses\n");
  printf("7: Lista de clientes por inicial\n");
  printf("8: Lista de produtos por inicial\n");
  printf("9: Lista de produtos que não foram comprados\n");
  printf("10: Sair\n\n");

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
  Bool done = false;        /* Boolean to control when user has finished */
  int n = 0, lower, total;  /* Iterator, lower bound and total number of clients */
  int clients = 60;         /* Number of clients to be displayed */
  int pages = 0, page = 0;  /* Number of pages, page number */
  char initial, option;     /* Initial letter of client code, menu option */
  StrList list = NULL;      /* String list to save the list */

  if (!cat) { /* Catalogue doesn't exist */
    printf("Clients Catalogue not loaded!\nPress Enter\n");
    while (getchar() != '\n')   /* Flush standar input */
      ;
    getchar();                  /* Wait for Enter */
    system("clear");
  }
  else {
    printf("Initial:\n"); /* Get initial from input */
    scanf(" %c", &initial);

    list = searchClients(cat, initial);
    system("clear");


    if (list == NULL) { /* No clients with the given initial */
      printf("There are no clients with the given initial.\nPress Enter.\n");
      while(getchar() != '\n') /* Flush the standar input */
          ;
      getchar();                /* Wait for enter */
      system("clear");
    } else {
      lower = 0;
      total = list->size;
      pages = (int) ceil((double) total/clients);
      page = 1;

      while (!done)
      {
        if (page < 1) page = 1;
        else if (page > pages) page = pages;

        printf("%d clients found.\n", list->size);

        lower = ((page - 1) * 60);

        /* Print 60 clients */
        for (n = lower; (n < (lower + clients)) && (n < total) ; n+=3)
        {
          printf("%s   ", list->clients[n]);
          if ((n+1) < total) printf("%s   ", list->clients[n+1]);
          if ((n+2) < total) printf("%s", list->clients[n+2]);
          printf("\n");
        }

        printf("Page %d of %d\n", page, pages);
        printf("N: next | B: back | P (enter) [page number]: go to page | M : menu\n");

        scanf(" %c", &option);
        if (option == 'M') done = true;
        else if (option == 'N') page++;
        else if (option == 'B') page--;
        else if (option == 'P')
        {
          scanf(" %d", &page);
        }

        system("clear");
      }

      /* Free all strings in the list */
      for (n = 0; n < list->size; n++)
        free(list->clients[n]);
    }
  }
  free(list); /* Free the list pointer */
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

void productsList (ProductsCat * cat)
{
  PList * p;                        /* Save products list */
  int products = 60;                /* Number of products to be displayed */
  int pages = 0, page = 0;          /* Number of pages, page number */
  int lower = 0, n = 0, total = 0;  /* Lower boundary, iterator, total products found */
  char initial, option;             /* Product initial, menu option */
  Bool done = false;                /* Boolean to control when user has finished */

  printf("Qual a inicial pela qual quer procurar?\n");
  scanf(" %c", &initial);

  system("clear");                  /* Clear terminal view */

  p = searchI(cat, initial);
  total = getQnt(p);

  page = 1;
  pages = (int) ceil((double) total/products);
  
  while(!done)
  {
    if (page < 1) page = 1;
    else if (page > pages) page = pages;
    lower = (page - 1) * products;
    
    printf("%d products found.\n", total);
    for(n = lower ; (n < (lower + products)) && (n < total); n+=3)
    {
      printf("%s   ", getCode(p, n));
      if (n+1 < total) printf("%s   ", getCode(p, n+1));
      if (n+2 < total) printf("%s   ", getCode(p, n+2));
      printf("\n");
    }

    printf("Page %d of %d\n", page, pages);
    printf("N: next | B: back | P (enter) [page number] : go to page | M: menu\n");

    scanf(" %c", &option);
    if ( option == 'N' ) page++;
    else if ( option == 'B' ) page--;
    else if ( option == 'M' ) done = 1;
    else if ( option == 'P' )
    {
      scanf("%d", &page);
    }

    system("clear");
  }
}

/*-------------------------iAccounting--------------------------*/
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
    searchProduct(cat2, trim->productCode)
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
      search4Product(cat2, tk->productCode);
      insertAccounting(cat3, tk);
      validated++;
    }
    else free(tk->productCode);
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

/* Querie 4 */
void productsNotBoughtList (ProductsCat * cat){
  PList * p;                        /* Save products list */
  int products = 60;                /* Number of products to be displayed */
  int pages = 0, page = 0;          /* Number of pages, page number */
  int lower = 0, n = 0, total = 0;  /* Lower boundary, iterator, total products found */
  char initial, option;             /* Product initial, menu option */
  Bool done = false;                /* Boolean to control when user has finished */

  system("clear");                  /* Clear terminal view */

  p = productsNotBought(cat);
  total = getQnt(p);

  page = 1;
  pages = (int) ceil((double) total/products);
  
  while(!done)
  {
    if (page < 1) page = 1;
    else if (page > pages) page = pages;
    lower = (page - 1) * products;
    
    printf("%d products found.\n", total);
    for(n = lower ; (n < (lower + products)) && (n < total); n+=3)
    {
      printf("%s   ", getCode(p, n));
      if (n+1 < total) printf("%s   ", getCode(p, n+1));
      if (n+2 < total) printf("%s   ", getCode(p, n+2));
      printf("\n");
    }

    printf("Page %d of %d\n", page, pages);
    printf("N: next | B: back | P (enter) [page number] : go to page | M: menu\n");

    scanf(" %c", &option);
    if ( option == 'N' ) page++;
    else if ( option == 'B' ) page--;
    else if ( option == 'M' ) done = 1;
    else if ( option == 'P' )
    {
      scanf("%d", &page);
    }

    system("clear");
  }
}



/*--------------------------MAIN--------------------------*/
int main () {
  ClientsCat cat1;
  ProductsCat * cat2;
  Accounting * cat3;
  int choice = 0;
  int done = 0;
  char name1[100], filename[100];
  int month1, month2;
  OverallSales * acctSales; /* Return of accounting info */

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
        scanf("%s", name1);
        printf("\nO Cliente %s\n", searchClient(cat1, name1) ? "existe" : "não existe");
        break;
      case 5:
        printf("Indique o nome do produto e o mês: ");
        scanf("%s", name1);
        scanf("%d", &month1);
        acctSales = getMonthlyProductSales(cat3, month1, name1);
        if(acctSales->normalNumber == -1) printf("O produto %s não foi comprado\n", name1);
        else printf("\nO Produto %s vendeu %d unidades normais e %d em promoção num total de %f euros\n", name1,  acctSales->promotionNumber, acctSales->normalNumber, acctSales->income);
        break;
      case 6:
        printf("Indique o período de meses: ");
        scanf("%d", &month1);
        scanf("%d", &month2);
        acctSales = getSalesbyMonthPeriod(cat3, month1, month2);
        printf("\nDe %d a %d venderam-se %d unidades num total de %f euros\n", month1, month2,  acctSales->promotionNumber + acctSales->normalNumber, acctSales->income);
        break;
      case 7:
        clientsList(cat1); break;
      case 8:
        productsList(cat2); break;
      case 9:
        productsNotBoughtList(cat2); break;
      case 10:
        done = 1; break;
      default:
        break;
    }
  }

  return 0;
}
