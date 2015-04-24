#ifndef AVLP_SALES
#define AVLP_SALES

#include "includes/StrList.h"

typedef struct avlp * AVLP;
typedef struct topNP * q12struct; /* Struct to be used for querie 12 */

AVLP initSalesP ();
AVLP insertProductAVLP (AVLP, char *, int);
AVLP insertClientAVLP (AVLP, char *, char *, char);
StrList clientsThatBought (AVLP, char *);
StrList topNProducts (AVLP, int);

#endif

