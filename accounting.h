#include "tests/accountingtest.h"

/* AVL tree node */
typedef struct treeNode{
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
  struct treeNode *left;
  struct treeNode *right;
} ProductNode;

/* 12 trees for each Month sales */
typedef struct {
  ProductNode * monthAccounting[12];
} Accounting;

/*-------------------------------API-------------------------------*/

/* Allocates and initializes an array of ProductNode to NULL */
Accounting initAccounting();
int insertAccounting(Accounting bills, Tokens *);

/* Removes a sale by ProductCode */
int removeAccounting(Accounting bills, char *);

/* 
 * Searches the product through the different trees 
 * Returns 0 (FALSE) if not found or 1 (TRUE) if found
 */
bool searchAccounting(Accounting, char *);

/* Finds a node and returns the sales of a specific month by promotion or normal */
double * getMonthlySales(Accounting bills, int, char *);
