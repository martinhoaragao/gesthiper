#ifndef AVLP_SALES
#define AVLP_SALES

#include "../includes/StrList.h"

typedef struct productNode * SalesP;
typedef struct q12struct {
  StrList list;   /* List to store products */
  int * quants;   /* Array of quantities */
  int * clients;  /* Number of clients */
} * topNP;
typedef struct topNP * q12struct; /* Struct to be used for querie 12 */

SalesP initSalesP ();
SalesP insertProductSP (SalesP, char *, int);
SalesP insertClientSP (SalesP, char *, char *, char);
StrList clientsThatBought (SalesP, char *);
topNP topNProducts (SalesP, int);
void freeSalesP (SalesP);

#endif

