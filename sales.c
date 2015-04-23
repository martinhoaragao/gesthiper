/*
Implementation of a sales structure with an AVL of clients and inside each node there
is a AVL for the products bought by those clients
*/
#include <stdlib.h>
#include <string.h>
#include "sales.h"
#include "includes/salesstructs.h"
#include "includes/StrList.h"
#include "bool.h"

typedef struct clientNode ClientNode;
typedef struct productNode ProductNode;

/***** Sales.c exclusive functions *****/

static Sales createNode (char *);
static Sales addClient (Sales, char *);

static int height (ClientNode *);
static int height_P (ProductNode *);
static int max (int, int);
static int getBalance (ClientNode *);

static ClientNode * rightRotate (ClientNode *);
static ClientNode * leftRotate (ClientNode *);
static ClientNode * getClient (Sales, char *);

static ProductNode * leftRotate_P (ProductNode *);
static ProductNode * rightRotate_P (ProductNode *);
static ProductNode * addProduct (ProductNode *, char *, int);
static ProductNode * createProductNode (char *, int);
static ProductNode * getProduct (ProductNode *, char *);
static int getBalance_P (ProductNode *);
static StrList productsOnMonth_aux (ProductNode *, StrList, int *);
static StrList topProducts_aux (ProductNode *, StrList, int *);

/***** AVL Manipulations *****/

/* Retrieve height a node */
static int height (ClientNode * node)
{
  if (node == NULL) return 0;
  return node->height;
}

/* Retrieve height a node */
static int height_P (ProductNode * node)
{
  if (node == NULL) return 0;
  return node->height;
}

/* Return maximum between two numbers */
static int max (int x, int y)
{
  return ((x > y) ? x : y);
}

/* Create a new client node */
static Sales createNode (char * client)
{
  int i;
  Sales cn = (Sales) malloc(sizeof(struct clientNode));
  strcpy(cn->client, client);
  cn->height = 0;
  cn->left = cn->right = NULL;

  /* Set products pointers to NULL */
  for (i = 0; i < 12; i++)
    cn->products[i] = NULL;

  return cn;
}

/* Right rotate AVL tree */
static ClientNode * rightRotate (ClientNode * node)
{
  ClientNode * x = node->left;
  ClientNode * T2 = x->right;

    /* Perform rotation */
  x->right = node;
  node->left = T2;

    /* Update heights */
  node->height = max(height(node->left), height(node->right))+1;
  x->height = max(height(x->left), height(x->right))+1;

    /* Return new node */
  return x;
}

/* Left Rotate AVL tree */
static ClientNode * leftRotate (ClientNode * node)
{
  ClientNode * y = node->right;
  ClientNode * T2 = y->left;

    /* Perform rotation */
  y->left = node;
  node->right = T2;

    /*  Update heights */
  node->height = max(height(node->left), height(node->right))+1;
  y->height = max(height(y->left), height(y->right))+1;

    /* Return new node */
  return y;
}

/* Right rotate AVL tree */
static ProductNode * rightRotate_P (ProductNode * node)
{
  ProductNode* x = node->left;
  ProductNode * T2 = x->right;

    /* Perform rotation */
  x->right = node;
  node->left = T2;

    /* Update heights */
  node->height = max(height_P(node->left), height_P(node->right)) + 1;
  x->height = max(height_P(x->left), height_P(x->right)) + 1;

    /* Return new node */
  return x;
}

/* Left Rotate AVL tree */
static ProductNode * leftRotate_P (ProductNode * node)
{
  ProductNode * y = node->right;
  ProductNode * T2 = y->left;

    /* Perform rotation */
  y->left = node;
  node->right = T2;

    /*  Update heights */
  node->height = max(height_P(node->left), height_P(node->right))+1;
  y->height = max(height_P(y->left), height_P(y->right))+1;

    /* Return new node */
  return y;
}

static int getBalance (ClientNode * node)
{
  if (node == NULL)
    return 0;
  return height(node->left) - height(node->right);
}

static int getBalance_P (ProductNode * node)
{
  if (node == NULL) return 0;
  return height_P(node->left) - height_P(node->right);
}

/*********************************************************************/

/* Initiate the sales structure */
Sales initSales () {
  int i;
  Sales s = (Sales) malloc(sizeof(struct clientNode));
  s->client[0] = '\0';
  s->height = 0;
  s->left = s->right = NULL;

  /* Set product pointers to NULL */
  for (i = 0; i < 12; i++)
    s->products[i] = NULL;

  return s;
}

/* Function to insert a client in the AVL,
also acts as a moderator to check if structure was not initialized */
Sales insertClients (Sales s, char * client)
{
  if (s == NULL) return NULL; /* Sales structure was not initialized */

  return addClient(s, client);
}

/* Add a client to the AVL */
static Sales addClient (Sales s, char * client)
{
  Sales result = NULL;             /* Function return value */
  int strcomp, balance, i, j; /* strcmp() result, balance factor, auxiliar ints */

  if (s == NULL)  /* New client */
  {
    result = createNode(client);
  } else {
    if (s->client[0] == '\0') /* First client to be inserted after initialization */
    {
      strcpy(s->client, client);
      result = s;
    } else {
      strcomp = strcmp(client, s->client);

      if (!strcomp) { result = s; } /* Client already inserted */
      else if (strcomp > 0)
      {
        /* Client trying to be inserted should go to right side of the current node */
        s->right = addClient(s->right, client);
      } else {
        /* Client trying to be inserted should go to left side of the current node */
        s->left = addClient(s->left, client);
      }

      result = s;

      s->height = max(height(s->left), height(s->right)) + 1;
      balance = getBalance(s);

      if ((balance > 1) || (balance < -1))
      {
       /* If this node becomes unbalanced, then there are 4 cases */
      if (s->left == NULL) i = 0;
      else i = strcmp(client, s->left->client);
      if (s->right ==NULL) j = 0;
      else j = strcmp(client, s->right->client);

      /* Left Left Case */
      if (balance > 1 && i < 0)
        result = rightRotate(s);

      /* Right Right Case */
      if (balance < -1 && j > 0)
        result = leftRotate(s);

      /* Left Right Case */
      if (balance > 1 && i > 0)
      {
        s->left = leftRotate(s->left);
        result = rightRotate(s);
      }
      /* Right Left Case */
      if (balance < -1 && j < 0)
      {
        s->right = rightRotate(s->right);
        result = leftRotate(s);
      }
     }
    }
  }
  return result;
}

/* Remove a client from the sales structure */
Sales removeClients (Sales s, char * client)
{
  int strcomp, balance;
  ClientNode * temp;

  if (!s) return s;

  strcomp = strcmp(client, s->client);

  if (strcomp < 0)
    s->left = removeClients(s->left, client);
  else if (strcomp > 0)
    s->right = removeClients(s->right, client);
  else
  {
    if (!s->left || !s->right)
    {
      temp = (s->left ? s->left : s->right);

      if (!temp)
      {
        temp = s;
        s = 0;
      } else *s = *temp;

      free(temp);
    }
    else
    {
      temp = s->right;
      strcpy(s->client, temp->client);
      s->right = removeClients(s->right, temp->client);
    }
  }

  if (!s) return s;

  s->height = max(height(s->left), height(s->right)) + 1;
  balance = getBalance(s);

  if (balance > 1 && getBalance(s->left) >= 0)
    return rightRotate(s);

  if (balance > 1 && getBalance(s->left) < 0)
  {
    s->left = leftRotate(s->left);
    return rightRotate(s);
  }

  if (balance < -1 && getBalance(s->right) <= 0)
    return leftRotate(s);

  if (balance < -1 && getBalance(s->right) > 0)
  {
    s->right = rightRotate(s->right);
    return leftRotate(s);
  }

  return s;
}

/* Get the client node of a given client code */
static ClientNode * getClient (Sales s, char * client)
{
  int strcomp;  /* To store the result of strcmp() */

  if (s == NULL) return NULL; /* Client is not in the sales structure */

  strcomp = strcmp(client, s->client);

  if (strcomp == 0) return s;                               /* Client found */
  else if (strcomp > 0) return getClient(s->right, client); /* Client may be on right tree*/
  else return getClient(s->left, client);                   /* Client may be on left tree */
}

/*************** PRODUCTS CODE ****************/

/* Create a new product node with the give product and the given units */
static ProductNode * createProductNode (char * product, int units)
{
  ProductNode * node = (ProductNode *) malloc(sizeof( ProductNode ));
  strcpy(node->product, product);
  node->left = node->right = NULL;
  node->quant = units;
  node->height = 0;

  return node;
}

/* Insert a product in a given client node*/
Sales insertProducts (Sales s, char * client, char * product, int month, int units)
{
  /* Check if client exists on the AVL */
  ClientNode * node = getClient(s, client);
  month = month - 1;

  if (!node) return NULL;   /* Client is not on the AVL */
  else if (node->products[month] == NULL)
  {
    node->products[month] = createProductNode(product, units); /* First product in given month */
  } else {
    node->products[month] = addProduct(node->products[month], product, units);
  }

  return s;
}

static ProductNode * addProduct (ProductNode * node, char * product, int units)
{
  ProductNode * result = NULL;    /* Store the return value */
  int strcomp = 0, balance, i, j; /* Store the result of strcmp() */

  if (!node) return createProductNode(product, units); /* Insert product on the AVL */
  else
  {
    strcomp = strcmp(product, node->product);

    if (strcomp == 0) { /* Product already inserted, update quantity */
      node->quant += units;
      result = node;
    }
    else if (strcomp > 0)
    {
      result = node->right = addProduct(node->right, product, units);
    }
    else
    {
      result = node->left = addProduct(node->left, product, units);
    }

    result = node;

    /* Update node height and calculate balance factor */
    node->height = max(height_P(node->left), height_P(node->right)) + 1;
    balance = getBalance_P(node);

    if (node->left == NULL) i = 0;
    else i = strcmp(product, (node->left)->product);
    if (node->right == NULL) j = 0;
    else j = strcmp(product, (node->right)->product);

    /* Left Left Case */
    if (balance > 1 && i < 0)
      result = rightRotate_P(node);

      /* Right Right Case */
    if (balance < -1 && j > 0)
      result = leftRotate_P(node);

      /* Left Right Case */
    if (balance > 1 && i > 0)
    {
      node->left = leftRotate_P(node->left);
      result = rightRotate_P(node);
    }
    /* Right Left Case */
    if (balance < -1 && j < 0)
    {
      node->right = rightRotate_P(node->right);
      result = leftRotate_P(node);
    }
  }

  return result;
}

/* Returns a pointer to the node of that product if it exists, NULL otherwise */
static ProductNode *getProduct (ProductNode * node, char * product)
{
  int strcomp;

  if (node == NULL) return NULL;

  strcomp = strcmp(product, node->product);

  if (strcomp == 0) return node;
  else if (strcomp > 0) return getProduct(node->right, product);
  else return getProduct(node->left, product);
}

static StrList productsOnMonth_aux (ProductNode * node, StrList list, int * quants)
{
  int i = 0, n = 0;
  Bool done = false;

  if (!node) return NULL; /* Null node */

  list->clients[list->size] = malloc(sizeof(char) * 7); /* Allocate space for new string */

  if (list->size == 0) {  /* First product to be inserted */
    strcpy(list->clients[0], node->product);
    quants[0] = node->quant;
  } else {
    for (i = 0; (i < list->size) && !done; i++)
      if (node->quant > quants[i])
        done = true;

    if (done) {                                          /* Shift all strings */
      i--;
      for (n = (list->size) - 1; n >= i; n--) {
        strcpy(list->clients[n+1], list->clients[n]);
        quants[n+1] = quants[n];
      }
    }
    strcpy(list->clients[i], node->product);
    quants[i] = node->quant;
  }

  (list->size)++;

  productsOnMonth_aux(node->left, list, quants);
  productsOnMonth_aux(node->right, list, quants);

  return list;
}

/* Query 9 - Determine list of products bought by a client on a given
 * month on descending order */
StrList productsOnMonth (Sales sales, char * client, int month) {
  StrList list = malloc(sizeof(struct strlist));
  ClientNode * node = getClient(sales, client);
  int quants[40000];
  list->size = 0;

  if (node)
    list = productsOnMonth_aux(node->products[month-1], list, quants);

    return list;
}

/* Query 10 - Create a list of strings with the clients that bought
 * items every month of the year
 * !!!!!!!!!!! NEEDS TESTING */
StrList yearlyClients (Sales sales, StrList list)
{
  int i;
  Bool bought = true;

  if (sales == NULL) return NULL; /* Null node */

  /* Check if client bought at least an item every month */
  for (i = 0; i < 12; i++)
    bought = bought && (sales->products[i] != NULL);


  /* Allocate space to save client and copy client code */
  if (bought)
  {
    printf("Hi there!\n");
    list->clients[list->size] = (char *) malloc(sizeof( char ) * 7);
    strcpy(list->clients[list->size], sales->client);
    (list->size)++;
  }

  /* Separate into left and right nodes */
  yearlyClients(sales->left, list);
  yearlyClients(sales->right, list);

  return list;
}

static StrList topProducts_aux (ProductNode * node, StrList list, int * quants)
{
  int i = 0, n = 0;
  char temp[7];
  Bool done = false, found = false;

  if (!node) return NULL; /* Null node */

  list->clients[list->size] = malloc(sizeof(char) * 7); /* Allocate space for new string */

  if (list->size == 0) {  /* First product to be inserted */
    strcpy(list->clients[0], node->product);
    quants[0] = node->quant;
    (list->size)++;
  }
  else {

    /* Search if product exists already */
    for (i = 0; (i < list->size) && !found; i++)
      if (strcmp(node->product, list->clients[i]) == 0) found = true;

    /* Update product */
    if (found)
    {
      i--;
      quants[i] += node->quant;

      for (; (i > 0) && (quants[i] > quants[i-1]); i--)
      {
        n = quants[i-1];
        quants[i-1] = quants[i];
        quants[i] = n;
        strcpy(temp, list->clients[i-1]);
        strcpy(list->clients[i-1], node->product);
        strcmp(list->clients[i], temp);
      }
    }

    /* New product */
    if (found == false)
    {
      for (i = 0; (i < list->size) && !done ; i++) /* where to put new client */
        if (node->quant > quants[i]) done = true;

      if (done) {                                       /* Shift all strings */
        i--;
        for (n = (list->size) - 1; n >= i; n--) {
          strcpy(list->clients[n+1], list->clients[n]);
          quants[n+1] = quants[n];
        }
      }

      strcpy(list->clients[i], node->product);
      quants[i] = node->quant;
      (list->size)++;
    }
  }

  topProducts_aux(node->left, list, quants);
  topProducts_aux(node->right, list, quants);

  return list;
}

/* Query 13 - Given a client code determine the 3 products he bought the most */
StrList topProducts (Sales sales, char * client)
{
  int quants[40000], i = 0;                            /* Quantities array */
  ClientNode * aux = getClient(sales, client);

  StrList list = (StrList) malloc(sizeof(struct strlist));  /* Create list */
  list->size = 0;

  if (!aux) return NULL;  /* If client does no exist */

  for (i = 0; i < 12; i++)
    topProducts_aux(aux->products[i], list, quants);

  if (list->size > 3) list->size = 3; /* Only display top 3 */
  return list;
}

/* Calculates how many products were bought by a client in a certain month */
 static int clientMonthSales (ProductNode * node){
    int i = 0;

    if(!node) return i;
    i++;
    i += clientMonthSales(node->left);
    i += clientMonthSales(node->right);

    return i;
 }

/* Query 5 - Calculates how many products a client bough by month */
ProductsN clientMonthlySales (Sales sales, char * client) {
  ProductsN numbers;
  ClientNode * node;
  int i;

  numbers = (ProductsN) malloc(sizeof(struct ProductsNStruct));
  node = getClient(sales, client);

  if(!node) return 0;

  for(i=0; i<12; i++) {
   numbers->productsBought[i] = clientMonthSales(node->products[i]);
  }

  return numbers;
}

/* Calculates how many clients bought each month */
static ClientsMonth clientsMonthCalculate (Sales node, ClientsMonth clientsMonth) {
  int i;

  if(!node) return clientsMonth;

  for (i = 0; i<12; i++) {
    if(node->products[i]) clientsMonth->number[i]++;
  }

    clientsMonthCalculate(node->left, clientsMonth);
    clientsMonthCalculate(node->right, clientsMonth);

    return clientsMonth;
}

/* Calculates how many clients bought each month and how many times they bought */
ClientsMonth clientMonthlyPurchases (Sales node) {
  int i;
  ClientsMonth monthlyPurchases;
  monthlyPurchases = (ClientsMonth) malloc(sizeof(struct ClientsMonthStruct));

  for (i = 0; i<12; i++) (monthlyPurchases->number[i])=0;
  monthlyPurchases = clientsMonthCalculate(node, monthlyPurchases);

  return monthlyPurchases;
}
