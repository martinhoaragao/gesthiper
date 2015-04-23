#ifndef SALES_H
#define SALES_H

#include "includes/salesstructs.h"
#include "includes/StrList.h"

struct productNode {
  /* Product Code */
  char product[7];

  /* Quantity bought */
  int quant;

  int height;
  struct productNode * left;
  struct productNode * right;
};

/* AVL tree node */
struct clientNode {
  /* Client Code */
  char client[6];

  /* Products AVL */
  struct productNode * products[12];

  int height;
  struct clientNode *left;
  struct clientNode *right;
};

typedef struct clientNode * Sales;

Sales initSales ();
Sales insertClients (Sales, char *);
Sales removeClients (Sales, char *);
Sales insertProducts (Sales, char *, char *, int, int);
StrList yearlyClients (Sales, StrList);
ProductsN clientMonthlySales (Sales, char *);
StrList productsOnMonth (Sales, char * , int);
StrList topProducts (Sales, char *);
ClientsMonth clientMonthlyPurchases (Sales);
StrList topNProducts (Sales, int);
/* Get product code from string
void getproduct (char * , char * );
// Get product price from string
float getprice (char * );
// Get product quantity from string
int getnumitems (char * );
// Get client code from string
void getclient (char * , char * );
// Get month from string
int getmonth (char * );
*/

#endif
