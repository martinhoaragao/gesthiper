#ifndef AVLP_SALES
#define AVLP_SALES

#include "../includes/StrList.h"
typedef struct q12struct {
  StrList list;   /* List to store products */
  int * quants;   /* Array of quantities */
  int * clients;  /* Number of clients */
} * topNP;
typedef struct avlp * AVLP;
typedef struct topNP * q12struct; /* Struct to be used for querie 12 */

AVLP initSalesP ();
AVLP insertProductAVLP (AVLP, char *, int);
AVLP insertClientAVLP (AVLP, char *, char *, char);
StrList clientsThatBought (AVLP, char *);
topNP topNProducts (AVLP, int);

#endif

