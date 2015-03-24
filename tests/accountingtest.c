#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../accounting.h"

/*
 * Helper function to trim
 * @param Receives string from sales file
 * @return Tokens in a struct
 */
static Tokens* trimSale(char* s){
  Tokens* trim = (Tokens*) malloc(sizeof(Tokens));
  trim->productCode = strdup( strtok(s, " "));
  trim->price = atof( strtok(0, " "));
  trim->number = atoi( strtok(0, " "));
  trim->type = strtok(0, " ")[0];
  trim->clientCode = strdup (strtok(0, " "));
  trim->month = atoi( strtok(0, " "));

  return trim;
}

static Tokens* validateSale(char* s){
  Tokens* trim = trimSale(s);

  if (
    (trim->price > 0) &&
    (trim->number > 0) &&
    ((trim->type == 'P') || (trim->type == 'N')) &&
    (trim->month > 0) &&
    (trim->month <= 12)
    ) return trim;
  else return 0;
}

int main() {
  double yay;
  Tokens* tk = (Tokens*) malloc(sizeof(Tokens));;
  int linhasValidas, linhas;
  linhasValidas = 0;
  linhas = 0;

  FILE * fp = fopen("salesfile.txt", "r");

  if ( fp == NULL ){
    printf("O ficheiro não existe!\n");
  }
  else {
    char sale[40];

    while ( fgets(sale, 40 ,fp) ){
      tk = validateSale(sale);
      linhas++;
      if (tk!=0) {
        insert_product(tk);
        linhasValidas++;
      }

    }
  }
  if((searchProduct("QC9889"))==0) printf("NAY\n");
  else printf("YAY\n");
  yay = getMonthSale(12, 'P', "IP8535");
  printf("%f\n", yay);
  printf("Foram lidas: %d linhas.\n", linhas);
  printf("São validas: %d linhas.\n", linhasValidas);

  return 0;
}
