#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "accounting.h"

/* Helper function that allocates a new node with the given code and
    NULL left and right pointers. */
static ProductNode* newNode(Tokens *sale) {
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
  } else {
    node->promotionMoney = (price*units);
    node->promotionNumber = units;
  }

  node->left = NULL;
  node->right = NULL;
  node->height = 1;  /* new node is initially added at leaf */
  return(node);
}

/****************************************************/

/* A utility function to get height of the tree */
static int height(ProductNode *N) {
  if (N == NULL)
    return 0;
  return N->height;
}

/****************************************************/

/* A utility function to get maximum of two integers */
static int max(int a, int b) {
  return (a > b)? a : b;
}

/****************************************************/

/* A utility function to update the information of a node */
static ProductNode* updateNode(ProductNode* node, Tokens * sale) {
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

/****************************************************/

/* A utility function to right rotate subtree nodeed with y */
 static ProductNode *rightRotate(ProductNode *y) {
  ProductNode *x = y->left;
  ProductNode *T2 = x->right;

    /* Perform rotation */
  x->right = y;
  y->left = T2;

    /* Update heights */
  y->height = max(height(y->left), height(y->right))+1;
  x->height = max(height(x->left), height(x->right))+1;

    /* Return new node */
  return x;
}

/****************************************************/

/* A utility function to left rotate subtree nodeed with x */
ProductNode *leftRotate(ProductNode *x) {
  ProductNode *y = x->right;
  ProductNode *T2 = y->left;

    /* Perform rotation */
  y->left = x;
  x->right = T2;

    /*  Update heights */
  x->height = max(height(x->left), height(x->right))+1;
  y->height = max(height(y->left), height(y->right))+1;

    /* Return new node */
  return y;
}

/****************************************************/

/* A utility function to get Balance factor of node N */
static int getBalance(ProductNode *N) {
  if (N == NULL)
    return 0;
  return height(N->left) - height(N->right);
}

/****************************************************/

/* 
 * Insert the product sales information into the avl, either by creating new node
 * or updating an existing one
 */
static ProductNode* insertAccountingAVL(ProductNode* node, Tokens * sale) {
  int i, j, balance;
  char * code;

    /* 1.  Perform the normal BST rotation */
  if (node == NULL)
    return(newNode(sale));

    /* 1.5 Retrieve the code */
  code = (char *) malloc(sizeof(char) * 8);
  strncpy(code, sale->productCode, 8);

    i = strcmp(code, node->code); /* Compare */
  if (i==0) {
    updateNode(node, sale);
    return node;
  }
  if (i < 0)
    node->left  = insertAccountingAVL(node->left, sale);
  else
    node->right = insertAccountingAVL(node->right, sale);

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

/****************************************************/

/* 
 * Given a non-empty binary search tree, return the node with minimum
 * key value found in that tree. Note that the entire tree does not
 * need to be searched. 
 */
   ProductNode * minValueNode(ProductNode * node){
    ProductNode * current = node;

    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
      current = current->left;

    return current;
  }

/****************************************************/
/* 
 * Remove a node by the product's code
 * Returns the new root of the tree with 1 less node if the key was found
 */
  ProductNode* deleteNode(ProductNode * node, char * key) {
    int i, balance;
    ProductNode *temp;

    /* STEP 1: PERFORM STANDARD BST DELETE */

    if (!node) 
      return node;

    i = strcmp(key, node->code); /* Compare */

     /* If the key to be deleted is smaller than the node's key, 
     * then it lies in left subtree */
    if ( i < 0 )
      node->left = deleteNode(node->left, key);

    /* If the key to be deleted is greater than the node's key,
     * then it lies in right subtree */
    else if( i > 0 ){
      node->right = deleteNode(node->right, key);
    }

    /* if key is same as node's key, then this is the node 
     * to be deleted */
    else{
        /* node with only one child or no child */
      if( !node->left || !node->right ){
        temp = node->left ? node->left : node->right;

            /* No child case */
        if (!temp)
        {
          temp = node;
          node = 0;
        }
            else /* One child case */
             *node = *temp; /* Copy the contents of the non-empty child */

        free(temp);
      }
      else
      {
            /* node with two children: Get the inorder successor (smallest
             * in the right subtree) */
        temp = minValueNode(node->right);

            /* Copy the inorder successor's data to this node */
        strcpy(node->code, temp->code);

            /* Delete the inorder successor */
        node->right = deleteNode(node->right, temp->code);
      }
    }

    /* If the tree had only one node then return */
    if (!node)
      return node;

    /* STEP 2: UPDATE HEIGHT OF THE CURRENT NODE */
    node->height = max(height(node->left), height(node->right)) + 1;

    /* STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to check whether
     *  this node became unbalanced) */
    balance = getBalance(node);

    /* If this node becomes unbalanced, then there are 4 cases */

    /* Left Left Case */
    if (balance > 1 && getBalance(node->left) >= 0)
      return rightRotate(node);

    /* Left Right Case */
    if (balance > 1 && getBalance(node->left) < 0)
    {
      node->left =  leftRotate(node->left);
      return rightRotate(node);
    }

    /* Right Right Case */
    if (balance < -1 && getBalance(node->right) <= 0)
      return leftRotate(node);

    /* Right Left Case */
    if (balance < -1 && getBalance(node->right) > 0)
    {
      node->right = rightRotate(node->right);
      return leftRotate(node);
    }

    return node;
  }

/****************************************************/

/* Search the product by code in an AVL */
static ProductNode* searchAccountingAVL(ProductNode* node, char* code) {
  int i;
  if (node == 0) return 0;
  i = strncmp(code, node->code, 7);
  if (i == 0) 
    return node;
  else if (i < 0) 
    return searchAccountingAVL(node->left, code);
  else 
    return searchAccountingAVL(node->right, code);
}


/*-------------------------------API-------------------------------*/

/* Allocates and initializes an array of ProductNode to NULL */
  Accounting initAccounting() {
    int i;
    Accounting * bills = malloc(sizeof(Accounting)) ;
    for (i=0; i<12; i++){
      (* bills).monthAccounting[i] = 0;
    }
    return * bills;
  }

/*-----------------------------------------------------------------*/

/* Inserts a sale into accounting according to the month */ 
  int insertAccounting(Accounting bills, Tokens * sale) {
    int month = sale->month;
    bills.monthAccounting[month-1] = insertAccountingAVL(bills.monthAccounting[month-1],sale);
    return 0;
  }


/*-----------------------------------------------------------------*/

/* Removes a sale by ProductCode */
  int removeAccounting(Accounting bills, char * str) {
    int i;
    for(i=0; i<12; i++ ){
      bills.monthAccounting[i] = deleteNode(bills.monthAccounting[i], str);
    }
    return 0;
  }

/*-----------------------------------------------------------------*/

/* 
 * Searches the product through the different trees 
 * Returns 0 (FALSE) if not found or 1 (TRUE) if found
 */
  bool searchAccounting(Accounting bills, char*code) {
    int i;
    for(i=0; i<12 && !searchAccountingAVL(bills.monthAccounting[i], code); i++ );
      if (i<12) return TRUE;
    else return FALSE;
  }

/*-----------------------------------------------------------------*/

/* Finds a node and returns the sales of a specific month by promotion or normal */
  double * getMonthlySales(Accounting bills, int m, char* code) {
    ProductNode* node;
    static double sales[3];
    node = searchAccountingAVL(bills.monthAccounting[m-1], code);

    if (node == NULL) sales[0] = -1;
    else{
      sales[0] = node->normalNumber;
      sales[1] = node->promotionNumber;
      sales[2] = node->normalMoney + node->promotionMoney;
    }
    return sales;
  }