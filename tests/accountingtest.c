#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>       /* To test execution times */

#include "../accounting.h"
#include "../clients.h"

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
static Tokens* validateSale(char* s){
  Tokens* trim = trimSale(s);
  if (
    /* There are actually same 0€ sales */
    (trim->price >= 0) &&
    (trim->number > 0) &&
    ((trim->type == 'P') || (trim->type == 'N')) &&
    (trim->month > 0) &&
    (trim->month <= 12) &&
    clientsSearch(trim->clientCode)
    ) return trim;
  else return 0;
}

int main() {
  /*double yay;*/
  int linhasValidas, linhas;
  char filename[100];
  /* To receive results from Monthly Sales */
  double * monthlysales, totalbill;
  time_t itime, ftime; /* Times for clients and accounting load*/
  char * client = (char *) malloc(sizeof(char) * 7);
  FILE * fp;
  Tokens * tk;

  totalbill = 0;

  linhasValidas = 0;
  linhas = 0;

  /* CLIENTS INIT*/
  printf("What's the name of the file to read?\n");
  scanf("%s", filename);
  time(&itime);

  clientsInit();

  fp = fopen(filename, "r");

  /* File doesn't exist */
  if ( fp == NULL )
  {
    printf("O ficheiro não existe!\n");
    return 1;
  }
  clientsInit();  /* Initiate clients structure */

  while(fgets(client, 10, fp)){
    strtok(client, "\n"); /* Replace '\n' to \0 before inserting string */
    clientInsert(client);
  }
  fclose(fp);
  time(&ftime);
  printf("Took: %.fs\n", difftime(ftime, itime));


  /* ACCOUNTING */
  time(&itime);

  fp = fopen("salesfile.txt", "r");

  if ( fp == NULL ){
    printf("O ficheiro não existe!\n");
  }
  else {
    char sale[40];
    initAccounting();

    while ( fgets(sale, 40 ,fp) ){
      tk = validateSale(sale);
      linhas++;
      if (tk) {
        totalbill += (tk->price * tk->number);
        insertProductSale(tk);
        linhasValidas++;
      }
      free(tk);
    }
  }

  fclose(fp);
  time(&ftime);
  printf("Took: %.fs\n", difftime(ftime, itime));

  printf("Foram lidas: %d linhas.\n", linhas);
  printf("São validas: %d linhas.\n", linhasValidas);
  printf("Total Billing %f\n", totalbill);

  /*printf("%d\n", searchProductSale("NL9818"));*/
  removeProductSale("HZ2772");

  printf("Please type the code and month\n");
  scanf("%s", client);
  scanf("%d", &linhas);
  monthlysales = getMonthlySales(linhas, client);
  printf("\nThe product: %s had:\n%d Normal sales and %d Promotion Sales\nTotal cash:%f\n", client, (int)monthlysales[0], (int)monthlysales[1], monthlysales[2]);

  free(client);
  return 0;
}
