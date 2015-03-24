#include "tests/accountingtest.h"

// AVL tree node
typedef struct node{
  // Saves the product code, ex. AB2345
  char code[6];
  // How many normal sales distributed by months
  int normalNumber[12];
  // Normal sales distributed by months
  double normalMoney[12];
  // How many promotion sales distributed by months
  int promotionNumber[12];
  // Promotion sales distributed by months
  double promotionMoney[12];
  // Height of the node
  int height;
  struct node *left;
  struct node *right;
} ProductNode;

int init_accounting();
int insert_product(Tokens *);
int searchProduct(char *);
double getMonthSale(int, char, char *);
