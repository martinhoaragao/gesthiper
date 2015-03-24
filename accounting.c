#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "accounting.h"

ProductNode* tree = NULL;
int merda=2;

/* Helper function to reset an 12 sized array to 0's */
static void resetArray(double* t){
  int i = 0;
  for(i=0; i<12; i++) t[i]=0;
}

/* Search the product by code in an AVL */
static ProductNode* searchProduct_AVL(ProductNode* node, char* code){
    int i;
    if (node == NULL) return node;
    i = strcmp(code, node->code);
    if (i==0) return node;
    else if (i < 0) searchProduct_AVL(node->left, code);
    else searchProduct_AVL(node->right, code);
}

int searchProduct(char*code){
    if (searchProduct_AVL(tree, code)==NULL) return 0;
    else return 1;
}

double getMonthSale(int m, char t, char* code){
  ProductNode* node;
  node = searchProduct_AVL(tree, code);

  if (node == NULL) return 0;
  else if (t=='N' || t=='n') return node->normalMoney[m-1];
  else return node->promotionMoney[m-1];
}

/* Helper function that allocates a new node with the given code and
    NULL left and right pointers. */
static ProductNode* newNode(Tokens *sale)
{
    static double price;
    static int month, units;
    /* Either a Promotion or a Normal sale */
    static char type;
    ProductNode* node = (ProductNode*)malloc(sizeof(ProductNode));
    strcpy(node->code, sale->productCode);
    price = sale->price;
    month = sale->month;
    type = sale->type;
    units = sale->number;
/*    resetArray(node->promotionNumber);*/
    resetArray(node->promotionMoney);
/*    resetArray(node->normalNumber);*/
    resetArray(node->normalMoney);
    if (type == 'N') {
      node->normalMoney[month-1]+= (price*units);
      node->normalNumber[month-1]+=units;
    }
    else {
      node->promotionMoney[month-1]+= (price*units);
      node->promotionNumber[month-1]+= units;
    }
    node->left = NULL;
    node->right = NULL;
    node->height = 1;  // new node is initially added at leaf
    return(node);
}

// A utility function to get height of the tree
static int height(ProductNode *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

// A utility function to get maximum of two integers
static int max(int a, int b)
{
    return (a > b)? a : b;
}

/* A utility function to update the arrays of a node */
static ProductNode* updateNode(ProductNode* node, Tokens * sale){
    static double price;
    static int month;
    /* Either a Promotion or a Normal sale */
    static char type;
    price = sale->price;
    month = sale->month;
    type = sale->type;
    if (type == 'N') node->normalMoney[month-1]+=price;
    else node->promotionMoney[month-1]+=price;
    return node;
}

/*______________AVL__________*/
// A utility function to right rotate subtree rooted with y
// See the diagram given above.
static ProductNode *rightRotate(ProductNode *y)
{
    ProductNode *x = y->left;
    ProductNode *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;

    // Return new root
    return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
ProductNode *leftRotate(ProductNode *x)
{
    ProductNode *y = x->right;
    ProductNode *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    //  Update heights
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;

    // Return new root
    return y;
}

// Get Balance factor of node N
static int getBalance(ProductNode *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

static ProductNode* insert_productAVL(ProductNode* node, Tokens * sale)
{
    int i, j;
    /* 1.  Perform the normal BST rotation */
    if (node == NULL)
        return(newNode(sale));

    /* 1.5 Retrieve the code */
    char* code;
    code = strdup(sale->productCode);

    i = strcmp(code, node->code);
    if (i==0) {
        updateNode(node, sale);
        return node;
    }
    if (i < 0)
        node->left  = insert_productAVL(node->left, sale);
    else
        node->right = insert_productAVL(node->right, sale);

    /* 2. Update height of this ancestor node */
    node->height = max(height(node->left), height(node->right)) + 1;

    /* 3. Get the balance factor of this ancestor node to check whether
       this node became unbalanced */
    int balance = getBalance(node);

    // If this node becomes unbalanced, then there are 4 cases
    if (node->left == NULL) i = 0;
    else i = strcmp(code, node->left->code);
    if (node->right ==NULL) j =0;
    else j = strcmp(code, node->right->code);

    // Left Left Case
    if (balance > 1 && i < 0)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && j > 0)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && i > 0)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && j < 0)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}

int insert_product(Tokens * sale){
    tree = insert_productAVL(tree,sale);
    return 0;
}
