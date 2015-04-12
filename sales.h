#ifndef SALES_H
#define SALES_H

/* AVL tree node */
struct clientNode {
  /* Client Code */
  char client[6];


  int height;
  struct clientNode *left;
  struct clientNode *right;
};

struct productNode {
  /* Product Code */
  char prod[7];

  /* Quantity bought, by month */
  int quant[12];

  int height;
  struct clientNode * left;
  struct clientNode * right;
};

typedef struct clientNode * Sales;

Sales initSales ();
Sales insertClient (Sales, char *);
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
