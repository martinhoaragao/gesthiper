#include "tests/accountingtest.h"

#include "bool.h"
#include "includes/overallsales.h"

/* 12 trees for each Month sales */
typedef struct {
  struct treeNode * monthAccounting[12];
  /* How many sales were done for each month */
  int sales[12];
} Accounting;

/*-------------------------------API-------------------------------*/

/* Allocates and initializes an array of ProductNode to NULL */
Accounting * initAccounting();
Accounting * insertAccounting(Accounting *, Tokens *);

/* Removes a sale by ProductCode */
int removeAccounting(Accounting *, char *);

/* 
 * Searches the product through the different trees 
 * Returns 0 (FALSE) if not found or 1 (TRUE) if found
 */
Bool searchAccounting(Accounting *, char *);

/* Finds a node and returns the sales of a specific month by promotion or normal */
OverallSales * getMonthlyProductSales(Accounting *, int, char *);

/* Calculates how many sales and how much they were worth for a given month period */
OverallSales * getSalesbyMonthPeriod(Accounting *, int, int);

/* Free an Accounting array */
void freeAccounting (Accounting *);


