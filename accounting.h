#include "tests/accountingtest.h"

/* AVL tree node */
typedef struct node{
  /* Saves the product code, ex. AB2345 */
  char code[6];
  /* Number of normal sales */
  int normalNumber;
  /* How much normal billed sales */
  double normalMoney;
  /* Number of sales in promotion */
  int promotionNumber;
  /* How much promotion billed sales */
  double promotionMoney;
  /* Height of the node */
  int height;
  struct node *left;
  struct node *right;
} ProductNode;

/* 12 trees for each Month sales */
typedef ProductNode * ProductSales[12];



int initAccounting();
int insertProductSale(Tokens *);
int removeProductSale(char *);
bool searchProductsale(char *);
double getMonthSale(int, char, char *);
