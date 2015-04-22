#ifndef AVLP_SALES
#define AVLP_SALES

typedef struct avlp * AVLP;
typedef struct avlpc * AVPLC;

AVLP initSalesP ();
AVLP insertProduct (AVLP, char *, int);
AVLP insertClient (AVLP, char *, char *, int);


#endif

