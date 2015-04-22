#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>       /* To test execution times */

#include "../accounting.h"
#include "../clients.h"
#include "../products.h"

  ClientsCat cat1;
  ProductsCat * cat2;


/*
 * Helper function to trim
 * @param Receives string from sales file
 * @return Tokens in a struct
 */
static Tokens * trimSale(char* s){
  Tokens * trim = (Tokens*) malloc(sizeof(Tokens));
  trim->productCode = (char *) malloc(sizeof(char) * 7);
  trim->clientCode = (char *) malloc(sizeof(char) * 6);

  strcpy(trim->productCode, strtok(s, " "));
  trim->price = atof( strtok(0, " "));
  trim->number = atoi( strtok(0, " "));
  trim->type = strtok(0, " ")[0];
  strcpy(trim->clientCode, strtok(0, " "));
  trim->month = atoi( strtok(0, "\n"));
  return trim;
}

/*
 * Validates a line
 * @param String with all the info for one sale;
 * @return Tokens in a struct
 */
static Tokens* validateSale(char* s){
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

int main() {
  /*double yay;*/
  int linhasValidas, linhas, i, validPCodes;
  char filename[100];
  /* To receive results from Monthly Sales */
  double totalbill;
  OverallSales * teste;
  time_t itime, ftime; /* Times for clients and accounting load*/
  char key[6];
  char * client = (char *) malloc(sizeof(char) * 7);

  Accounting * bills;

  FILE * fp;
  Tokens * tk;

  totalbill = 0;

  linhasValidas = 0;
  linhas = 0;

  /* CLIENTS INIT*/
  printf("What's the name of the file to read?\n");
  scanf("%s", filename);
  time(&itime);



  fp = fopen(filename, "r");

  /* File doesn't exist */
  if ( fp == NULL )
  {
    printf("O ficheiro não existe!\n");
    return 1;
  }
    cat1 = initClients();  /* Initiate clients structure */
    cat2 = initProductsCat();

  while(fgets(client, 10, fp)){
    strtok(client, "\n"); /* Replace '\n' to \0 before inserting string */
    insertClient(cat1, client);
  }
  fclose(fp);
  time(&ftime);
  printf("Took: %.fs\n", difftime(ftime, itime));



  /* PRODUCTS INIT */
  printf("Qual o nome do ficheiro a ler?\n");
  scanf("%s", filename);
  
  fp = fopen(filename, "r");
  if (fp == NULL){
    printf("O ficheiro não existe!\n");
    return 1;
  }

  validPCodes = 0;
  i =0;

  while(fgets(key, 10, fp)){
    if( insert_product(cat2, key)) validPCodes ++;
    i ++;
  }
  

  printf("O ficheiro %s foi lido.\n", filename);
  printf("Foram lidas %d linhas.\n", i);
  printf("Foram validadas %d linhas.\n", validPCodes);



  /* ACCOUNTING */
  time(&itime);

  fp = fopen("salesfile.txt", "r");

  if ( fp == NULL ){
    printf("O ficheiro não existe!\n");
  }
  else {
    char sale[40];
    bills = initAccounting();

    while ( fgets(sale, 40 ,fp) ){
      tk = validateSale(sale);
      linhas++;
      if (tk) {
        totalbill += (tk->price * tk->number);
        insertAccounting(bills, tk);
        linhasValidas++;
        free(tk->productCode);
        free(tk->clientCode);
        free(tk);
      }
    }

  time(&ftime);
  printf("Took: %.fs\n", difftime(ftime, itime));

  printf("Foram lidas: %d linhas.\n", linhas);
  printf("São validas: %d linhas.\n", linhasValidas);
  printf("Total Billing %f\n", totalbill);

  printf("\nEstá aqui? %d\n", searchAccounting(bills, "ZM6952"));

  teste = getSalesbyMonthPeriod(bills, 1, 12);
  printf("\nDuring this time period there were: %d sales with total income %f\n", teste->numberSales, teste->income);
  free(teste);

  removeAccounting(bills, "ZM6952");

  printf("\nEstá aqui? %d\n", searchAccounting(bills, "ZM6952"));

  /*printf("Please type the code and month\n");
  scanf("%s", client);
  scanf("%d", &linhas);
  teste = getMonthlyProductSales(bills, linhas, client);
  printf("\nThe product: %s had:\n%d Normal sales and %d Promotion Sales\nTotal cash:%f\n", client, teste->normalNumber, teste->promotionNumber, teste->income);
  */
  freeAccounting (bills);
  deleteCat(cat1);
  }

  fclose(fp);
  free(client);
  
  return 0;
}
