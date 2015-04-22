#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h> /* To test execution times */
#include <math.h>  /* To use ceil() */


#include "accounting.h"
#include "clients.h"
#include "products.h"
#include "sales.h"
#include "salesp.h"
#include "includes/StrList.h"
#include "includes/overallsales.h"


static void displayList (StrList);

/* Struct to hold catalogues for more effective load on read file */
typedef struct {
  Accounting * bills;
  Sales salesbyClients;
  ClientsCat goodClients;
  AVLP avlp;
} Catalogues;


int menu () {
  int r = 0;

  printf("\nGESTHIPER\n");
  printf("1: Carregar ficheiro clientes\n");
  printf("2: Carregar ficheiro produtos\n");
  printf("3: Carregar ficheiro compras\n");
  printf("4: Procurar cliente\n");
  printf("5: Procurar produto\n");
  printf("6: Procurar compras de um produto num mês\n");
  printf("7: Procurar compras num intervalo de meses\n");
  printf("8: Lista de clientes por inicial\n");
  printf("9: Lista de produtos por inicial\n");
  printf("10: Lista de produtos que não foram comprados\n");
  printf("11: Lista de clientes que compraram produtos todos os meses\n");
  printf("12: Nº de clientes que não realizaram compras e nº de produtos não comprados\n");
  printf("13: Número de compras por mês, de um cliente\n");
  printf("14: Códigos de cliente que compraram um produto\n");
  printf("15: Produtos comprados por um cliente num dado mês\n");
  printf("16: Top 3 Produtos comprados por um cliente\n");
  printf("17: Sair\n\n");

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

void yearlyClientsList (Sales sales)
{
  Bool done = false;        /* Boolean to control when user has finished */
  int n = 0, lower, total;  /* Iterator, lower bound and total number of clients */
  int clients = 60;         /* Number of clients to be displayed */
  int pages = 0, page = 0;  /* Number of pages, page number */
  char option;             /* Menu option */
  StrList list = NULL;      /* String list to save the list */

  if (!sales) { /* Catalogue doesn't exist */
    printf("Sales structure doesn't exist!\nPress Enter\n");
    while (getchar() != '\n')   /* Flush standar input */
      ;
    getchar();                  /* Wait for Enter */
    system("clear");
  }
  else {
    list = (StrList) malloc(sizeof( struct strlist ));  /* Space for list */
    list = yearlyClients(sales, list);                  /* Create list */
    system("clear");

    if (list == NULL) { /* No clients in the list */
      printf("Not a single client bought at least one item every month.\nPress Enter.\n");
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
  freeList(p);
}

/*-------------------------Accounting--------------------------*/
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
  else {
    free(trim->productCode);
    free(trim->clientCode);
    free(trim);

    return 0;
  }
}

Catalogues * loadSales (ClientsCat cl1, ClientsCat cl2, ProductsCat * cat2, char * filename) {
  FILE * fp;
  Tokens * tk;
  int nlines = 0, validated = 0, nclients = 0;
  clock_t start, stop; /* Times for clients and accounting load*/
  double totalbill = 0;
  char sale[40];
  Catalogues * cats = (Catalogues*) malloc(sizeof(Catalogues));

  start = clock();
  fp = fopen(filename, "r");
  if ( fp == NULL ){ printf("O ficheiro não existe!\n"); return NULL; }
  cats->bills = initAccounting();
  cats->salesbyClients = initSales();
  cats->goodClients = cl2;
  cats->avlp = initSalesP();

  while ( fgets(sale, 40 ,fp) ){
    tk = validateSale(cl1, cat2, sale);
    nlines++;
    if (tk) {
      totalbill += (tk->price * tk->number);
      search4Product(cat2, tk->productCode);
      cats->bills = insertAccounting(cats->bills, tk);
      cats->salesbyClients = insertClients(cats->salesbyClients, tk->clientCode);
      insertProducts(cats->salesbyClients, tk->clientCode, tk->productCode, tk->month, tk->number);
      cats->goodClients = removeClient(cats->goodClients, tk->clientCode);
      cats->avlp = insertProductAVLP(cats->avlp, tk->productCode, tk->number);
      cats->avlp = insertClientAVLP(cats->avlp, tk->productCode, tk->clientCode, tk->type);
      validated++;
      free(tk->productCode);
      free(tk->clientCode);
      free(tk);
    }
  }

  stop = clock();

  printf("\nO ficheiro '%s' foi lido.\n", filename);
  printf("Demorou: %2.5f segundos\n", ((double)stop-start)/CLOCKS_PER_SEC);
  printf("Foram lidas: %d linhas.\n", nlines);
  printf("Foram validadas %d linhas.\n", validated);
  printf("A Facturação total é de %f euros\n", totalbill);

  return cats;
}

/* List of products that were never bought */
void productsNotBoughtList (ProductsCat * cat){
  PList * p;                        /* Save products list */
  int products = 60;                /* Number of products to be displayed */
  int pages = 0, page = 0;          /* Number of pages, page number */
  int lower = 0, n = 0, total = 0;  /* Lower boundary, iterator, total products found */
  char option;                      /* Menu option */
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
  freeList(p);
}

/* Number of clients that didn't bought a single item, number of products
 * that were never bought */
void unusedCandP (ClientsCat cl, ProductsCat *pr)
{
  int unusedC = numOfClients(cl);
  int unusedP = numOfProducts(pr);

  printf("%d clients that bought nothing\n", unusedC);
  printf("%d products that were never bought.\n", unusedP);
  while (getchar() != '\n')   /* Flush standar input */
      ;
  getchar();                  /* Wait for Enter */
  system("clear");
  free(pr);
}

void query8aux (AVLP products, char * product)
{
  StrList list = clientsThatBought(products, product);

  if (list != NULL) displayList(list);
}

/* Function to deal with displaying a list and freeing the list */
void displayList (StrList list)
{
  int lower, total, pages, page, n;
  char option;  /* Menu option */
  Bool done = false;

  lower = 0;
  total = list->size;
  page = 1;
  pages = (int) ceil((double) total/60);

  system("clear");

  while (!done)
  {
    if (page < 1) page = 1;
    else if (page > pages) page = pages;

    lower = ((page - 1) * 60);

    for (n = lower; (n < (lower + 60)) && (n < total) ; n+=3)
    {
      printf("%s   ", list->clients[n]);
      if ((n+1) < total) printf("%s   ", list->clients[n+1]);
      if ((n+2) < total) printf("%s", list->clients[n+2]);
      printf("\n");
    }

    printf("Page %d of %d || %d Strings\n", page, pages, total);
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

  for (n = 0; n < list->size; n++)  /* Free all strings in the list */
    free(list->clients[n]);

  free(list); /* Free the list pointer */
}

static void query5(Sales sales) {
  char s[20];
  ProductsN prodSales; /* Return of client monthly sales info */

  printf("Indique o código do cliente que deseja procurar\n");
  scanf("%s", s);

  prodSales = clientMonthlySales (sales, s);

  if(!prodSales) {
    printf("Código errado, cliente não existe.\n");
    return;
  }
  printf("O cliente efetou as seguintes compras distribuídas por meses:\n\n");
  printf("Janeiro: %5d\n", prodSales->productsBought[0]);
  printf("Fevereiro: %3d\n", prodSales->productsBought[1]);
  printf("Março: %7d\n", prodSales->productsBought[2]);
  printf("Abril: %7d\n", prodSales->productsBought[3]);
  printf("Maio: %8d\n", prodSales->productsBought[4]);
  printf("Junho: %7d\n", prodSales->productsBought[5]);
  printf("Julho: %7d\n", prodSales->productsBought[6]);
  printf("Agosto: %6d\n", prodSales->productsBought[7]);
  printf("Setembro: %4d\n", prodSales->productsBought[8]);
  printf("Outubro: %5d\n", prodSales->productsBought[9]);
  printf("Novembro: %4d\n", prodSales->productsBought[10]);
  printf("Dezembro: %4d\n\n", prodSales->productsBought[11]);
}

/*--------------------------MAIN--------------------------*/
int main () {
  ClientsCat clients, cheapClients; /* cheapClients saves clients that bought nothing */
  ProductsCat * cat2;
  Catalogues * cats;
  AVLP avlp;
  int choice = 0, done = 0;
  char name1[100], filename[100];
  int month1, month2;
  clock_t start, stop;
  OverallSales * acctSales; /* Return of accounting info */

  start = clock();
  clients = loadCatClients("FichClientes.txt");
  cheapClients = loadCatClients("FichClientes.txt");
  cat2 = loadCatProducts ("FichProdutos.txt");
  cats = loadSales (clients, cheapClients, cat2, "Compras.txt");
  cheapClients = cats->goodClients;
  avlp = cats->avlp;
  stop = clock();

  printf("\nO carregamento inicial demorou: %2.5f segundos\n", ((double)stop-start)/CLOCKS_PER_SEC);



  while (!done) {
    choice = menu();
    switch( choice ) {
      case 1:
        printf("Libertando memória\n");
        deleteCat(clients);
        printf("Qual o nome do ficheiro de clientes?\n");
        scanf("%s", filename);
        clients = loadCatClients(filename);
        break;
      case 2:
        printf("Qual o nome do ficheiro de produtos?\n");
        scanf("%s", filename);
        cat2 = loadCatProducts(filename);
        break;
      case 3:
        freeAccounting(cats->bills);
        printf("Qual o nome do novo ficheiro de compras?\n");
        scanf("%s", filename);
        cats = loadSales(clients, cheapClients, cat2, filename);
        break;
      case 4:
        printf("Indique o nome do cliente: ");
        scanf("%s", name1);
        printf("\nO Cliente %s\n", searchClient(clients, name1) ? "existe" : "não existe");
        break;
      case 5:
        printf("Indique o código do produto: ");
        scanf("%s", name1);
        printf("\nO Produto %s\n", searchProduct(cat2, name1) ? "existe" : "não existe");
        break;
      case 6:
        printf("Indique o nome do produto e o mês: ");
        scanf("%s", name1);
        scanf("%d", &month1);
        acctSales = getMonthlyProductSales(cats->bills, month1, name1);
        if(acctSales->normalNumber == -1) printf("O produto %s não foi comprado\n", name1);
        else printf("\nO Produto %s vendeu %d unidades normais e %d em promoção num total de %f euros\n", name1,  acctSales->promotionNumber, acctSales->normalNumber, acctSales->income);
        break;
      case 7:
        printf("Indique o período de meses: ");
        scanf("%d", &month1);
        scanf("%d", &month2);
        acctSales = getSalesbyMonthPeriod(cats->bills, month1, month2);
        printf("\nDe %d a %d venderam-se %d unidades num total de %f euros\n", month1, month2,  acctSales->promotionNumber + acctSales->normalNumber, acctSales->income);
        break;
      case 8:
        clientsList(clients); break;
      case 9:
        productsList(cat2); break;
      case 10:
        productsNotBoughtList(cat2); break;
      case 11:
        yearlyClientsList(cats->salesbyClients); break;
      case 12:
        unusedCandP(cheapClients, cat2); break;  /* Will change function name, add products part */
      case 13:
        query5(cats->salesbyClients); break;
      case 14:
        printf("Produto: ");
        scanf("%s", name1);
        query8aux(avlp, name1);
        break;
      case 15:
        printf("Cliente: \n");
        scanf("%s", name1);
        printf("Mês: \n");
        scanf("%d", &month1);
        displayList(productsOnMonth(cats->salesbyClients, name1, month1)); break;
      case 16:
        printf("Cliente: \n");
        scanf("%s", name1);
        displayList(topProducts(cats->salesbyClients, name1)); break;
      case 17:
        done = 1; break;
      default:
        break;
    }
  }

  deleteCat(clients);
  deleteCat(cheapClients);
  freeAccounting(cats->bills);

  return 0;
}

