#ifndef AVLP_SALES
#define AVLP_SALES

#include "includes/StrList.h"

typedef struct avlp * AVLP;
typedef struct avlpc * AVPLC;

AVLP initSalesP ();
AVLP insertProductAVLP (AVLP, char *, int);
AVLP insertClientAVLP (AVLP, char *, char *, char);
StrList clientsThatBought (AVLP, char *);

#endif

