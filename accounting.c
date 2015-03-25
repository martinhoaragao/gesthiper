#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "accounting.h"

ProductSales bills;

int initAccounting(){
  int i;
  for (i=0; i<12; i++){
    bills[i] = 0;
  }
  return 0;
}

/* Search the product by code in an AVL */
static ProductNode* searchProductSaleAVL(ProductNode* node, char* code){
    int i;
    if (node == NULL) return node;
    i = strcmp(code, node->code);
    if (i==0) return node;
    else if (i < 0) searchProductSaleAVL(node->left, code);
    else searchProductSaleAVL(node->right, code);
}

/* Searches the product through the different trees */
bool searchProductSale(char*code){
  int i;
    for(i=0; i<12 && !searchProductSaleAVL(bills[i], code); i++ );
    if (i<12) return TRUE;
    else return FALSE;
}

double getMonthSale(int m, char t, char* code){
  ProductNode* node;
  node = searchProductSaleAVL(bills[m-1], code);
  printf("%s\n", node->code);

  if (node == NULL) return 0;
  else if (t=='N' || t=='n') return node->normalMoney;
  else return node->promotionMoney;
}

/* Helper function that allocates a new node with the given code and
    NULL left and right pointers. */
static ProductNode* newNode(Tokens *sale)
{
    static double price;
    static int units;
    /* Either a Promotion or a Normal sale */
    static char type;
    ProductNode* node = (ProductNode*)malloc(sizeof(ProductNode));
    strcpy(node->code, sale->productCode);
    price = sale->price;
    type = sale->type;
    units = sale->number;
    if (type == 'N') {
      node->normalMoney = (price*units);
      node->normalNumber = units;
    }
    else {
      node->promotionMoney = (price*units);
      node->promotionNumber = units;
    }
    node->left = NULL;
    node->right = NULL;
    node->height = 1;  /* new node is initially added at leaf */
    return(node);
}

/* A utility function to get height of the tree */
static int height(ProductNode *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

/* A utility function to get maximum of two integers */
static int max(int a, int b)
{
    return (a > b)? a : b;
}

/* A utility function to update the arrays of a node */
static ProductNode* updateNode(ProductNode* node, Tokens * sale){
    static double price;
    static int units;
    /* Either a Promotion or a Normal sale */
    static char type;
    units = sale->number;
    price = sale->price;
    type = sale->type;
    if (type == 'N') {
      node->normalMoney += (price * units);
      node->normalNumber += units;
    }
    else {
      node->promotionMoney += (price * units);
      node->promotionNumber += units;
    }
    return node;
}

/*______________AVL__________
 * A utility function to right rotate subtree rooted with y
 * See the diagram given above.
 */
static ProductNode *rightRotate(ProductNode *y)
{
    ProductNode *x = y->left;
    ProductNode *T2 = x->right;

    /* Perform rotation */
    x->right = y;
    y->left = T2;

    /* Update heights */
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;

    /* Return new root */
    return x;
}

/* A utility function to left rotate subtree rooted with x
 * See the diagram given above. */
ProductNode *leftRotate(ProductNode *x)
{
    ProductNode *y = x->right;
    ProductNode *T2 = y->left;

    /* Perform rotation */
    y->left = x;
    x->right = T2;

    /*  Update heights */
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;

    /* Return new root */
    return y;
}

/* Get Balance factor of node N */
static int getBalance(ProductNode *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

static ProductNode* insertProductSaleAVL(ProductNode* node, Tokens * sale)
{
    int i, j, balance;
    char * code;

    /* 1.  Perform the normal BST rotation */
    if (node == NULL)
        return(newNode(sale));

    /* 1.5 Retrieve the code */
    code = (char *) malloc(sizeof(char) * 8);
    strncpy(code, sale->productCode, 8);

    i = strcmp(code, node->code);
    if (i==0) {
        updateNode(node, sale);
        return node;
    }
    if (i < 0)
        node->left  = insertProductSaleAVL(node->left, sale);
    else
        node->right = insertProductSaleAVL(node->right, sale);

    /* 2. Update height of this ancestor node */
    node->height = max(height(node->left), height(node->right)) + 1;

    /* 3. Get the balance factor of this ancestor node to check whether
       this node became unbalanced */
    balance = getBalance(node);

    /* If this node becomes unbalanced, then there are 4 cases */
    if (node->left == NULL) i = 0;
    else i = strcmp(code, node->left->code);
    if (node->right ==NULL) j =0;
    else j = strcmp(code, node->right->code);

    /* Left Left Case */
    if (balance > 1 && i < 0)
        return rightRotate(node);

    /* Right Right Case */
    if (balance < -1 && j > 0)
        return leftRotate(node);

    /* Left Right Case */
    if (balance > 1 && i > 0)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }

    /* Right Left Case */
    if (balance < -1 && j < 0)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}

int insertProductSale(Tokens * sale){
  int month = sale->month;
  bills[month-1] = insertProductSaleAVL(bills[month-1],sale);
  return 0;
}
