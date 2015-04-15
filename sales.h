#ifndef SALES_H
#define SALES_H

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
Sales insertClient (Sales, char *);
Sales removeClient (Sales, char *);
Sales insertProduct (Sales, char *, char *, int);
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
