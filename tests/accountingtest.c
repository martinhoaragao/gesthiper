#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../accounting.h"
#include "../clients.h"

/*
 * Helper function to trim
 * @param Receives string from sales file
 * @return Tokens in a struct
 */
static Tokens* trimSale(char* s){
  Tokens* trim = (Tokens*) malloc(sizeof(Tokens));
  trim->productCode = (char *) malloc(sizeof(char) * 6);
  trim->clientCode = (char *) malloc(sizeof(char) * 5);


  strncpy(trim->productCode, strtok(s, " "), 6);
  trim->price = atof( strtok(0, " "));
  trim->number = atoi( strtok(0, " "));
  trim->type = strtok(0, " ")[0];
  strncpy(trim->clientCode, strtok(0, " "), 5);
  trim->month = atoi( strtok(0, " "));
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
    (trim->price > 0) &&
    (trim->number > 0) &&
    ((trim->type == 'P') || (trim->type == 'N')) &&
    (trim->month > 0) &&
    (trim->month <= 12) &&
    !clientsSearch(trim->clientCode)
    ) return trim;
  else return 0;
}

int main() {
  double yay;
  int linhasValidas, linhas;
  char filename[100];
  char * client = (char *) malloc(sizeof(char) * 7);
  FILE * fp;

  Tokens* tk = (Tokens*) malloc(sizeof(Tokens));;
  linhasValidas = 0;
  linhas = 0;

  /* CLIENTS INIT*/
  clientsInit();
  printf("What's the name of the file to read?\n");
  scanf("%s", filename);

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



  /* ACCOUNTING */

  fp = fopen("salesfile.txt", "r");

  if ( fp == NULL ){
    printf("O ficheiro não existe!\n");
  }
  else {
    char sale[40];;
    initAccounting();

    while ( fgets(sale, 40 ,fp) ){
      tk = validateSale(sale);
      linhas++;
      if (tk!=0) {
        insertProductSale(tk);
        linhasValidas++;
      }

    }
  }

  fclose(fp);

  if((searchProductSale("QC9889"))==0) printf("NAY\n");
  else printf("YAY\n");
  yay = getMonthSale(12, 'P', "IP8535");
  printf("%f\n", yay);
  printf("Esta cena existe? %d\n", clientsSearch("FU482"));
  printf("Foram lidas: %d linhas.\n", linhas);
  printf("São validas: %d linhas.\n", linhasValidas);

  return 0;
}
