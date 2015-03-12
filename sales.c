#include <stdio.h>
#include "sales.h"

int main ()
{
  // Variables for sales
  char sale[25];
  char product[6];
  float price = 0;
  int numitems = 0;
  char client[5];
  int month = 0;
  int readsale = 0;
  char c;

  FILE * fp = fopen("teste.txt", "r");

  if ( fp == NULL )
  {
    printf("O ficheiro não existe!\n");
  } else {

      while ( fgets(sale, 26 ,fp) != NULL )
      {
        getproduct( sale, product );
        printf("Produto: %s\n", product);

        // price = getprice( sale );
        // numitems = getnumitems( sale );
        // getclient( sale, client );
        // month = getmonth( sale );
      }

  }

  return 0;
}

void getproduct ( char * sale, char * product )
{
  int i = 0;

  for (i = 0; i < 7 ; i++)
    product[i] = sale[i];
}

/*char * getclient ( char * sale )
{
  char client[5];
  int i, j;

  for ( i = 0; i < 25 && ignoredspaces != 3; i++);

  for ( j = i; j < (i+5); j++)
    client[j-i] = sale[j];
}*/