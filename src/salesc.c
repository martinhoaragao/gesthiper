/*
 * Implementation of a sales structure with an AVL of clients and inside each node there
 * is an array of AVL Trees of the products bought by the client, each node of the Products
 * tree also has the quantity bought
 */
#include <stdlib.h>
#include <string.h>
#include "salesc.h"
#include "../includes/salesstructs.h"
#include "../includes/StrList.h"
#include "../includes/bool.h"
#include "../includes/max.h"

typedef struct productNode {
  /* Product Code */
  char product[7];

  /* Quantity bought */
  int quant;

  int height;
  struct productNode * left;
  struct productNode * right;
} ProductNode;

/* AVL tree node */
typedef struct clientNode {
  /* Client Code */
  char client[6];

  /* Products AVL */
  struct productNode * products[12];

  int height;
  struct clientNode *left;
  struct clientNode *right;
} ClientNode;

/***** SalesC exclusive functions *****/

static SalesC createNode (char *);
static SalesC addClient (SalesC, char *);
static ClientNode * rightRotate (ClientNode *);
static ClientNode * leftRotate (ClientNode *);
static ClientNode * getClient (SalesC, char *);
static ProductNode * leftRotate_P (ProductNode *);
static ProductNode * rightRotate_P (ProductNode *);
static ProductNode * addProduct (ProductNode *, char *, int);
static ProductNode * createProductNode (char *, int);
static int height (ClientNode *);
static int height_P (ProductNode *);
static int getBalance (ClientNode *);
static int getBalance_P (ProductNode *);
static StrList productsOnMonth_aux (ProductNode *, StrList, int *);
static StrList topProducts_aux (ProductNode *, StrList, int *);
static void freeProducts (ProductNode *);

/***** AVL Manipulations *****/

/* Retrieve height of a client node */
static int height (ClientNode * node) {
  if (node == NULL) return 0;
  return node->height;
}

/* Retrieve height a product node */
static int height_P (ProductNode * node) {
  if (node == NULL) return 0;
  return node->height;
}

/* Create a new client node */
static SalesC createNode (char * client) {
  int i;                                                    /* Iterator */
  SalesC new = (SalesC) malloc(sizeof(struct clientNode));  /* Allocat space for new node */
  strcpy(new->client, client);                              /* Save client */
  new->height = 0;
  new->left = new->right = NULL;

  for (i = 0; i < 12; i++)  /* Set products pointers to NULL */
    new->products[i] = NULL;

  return new;
}

/* Right rotate clients AVL tree */
static ClientNode * rightRotate (ClientNode * node) {
  ClientNode * a = node->left;
  ClientNode * b = a->right;

  a->right = node;  /* Perform rotations */
  node->left = b;

  /* Update heights */
  node->height = MAX(height(node->left), height(node->right)) + 1;
  a->height = MAX(height(a->left), height(a->right)) + 1;

  return a; /* Return new node */
}

/* Left Rotate clients AVL tree */
static ClientNode * leftRotate (ClientNode * node) {
  ClientNode * a = node->right;
  ClientNode * b = a->left;

  a->left = node; /* Perform rotations */
  node->right = b;

  /*  Update heights */
  node->height = MAX(height(node->left), height(node->right)) + 1;
  a->height = MAX(height(a->left), height(a->right)) + 1;

  return a; /* Return new node */
}

/* Right rotate products AVL tree */
static ProductNode * rightRotate_P (ProductNode * node) {
  ProductNode * a = node->left;
  ProductNode * b = a->right;

  a->right = node;  /* Perform rotations */
  node->left = b;

  /* Update heights */
  node->height = MAX(height_P(node->left), height_P(node->right)) + 1;
  a->height = MAX(height_P(a->left), height_P(a->right)) + 1;

  return a; /* Return new node */
}

/* Left Rotate products AVL tree */
static ProductNode * leftRotate_P (ProductNode * node) {
  ProductNode * a = node->right;
  ProductNode * b = a->left;

  a->left = node; /* perform rotations */
  node->right = b;

  /*  Update heights */
  node->height = MAX(height_P(node->left), height_P(node->right))+1;
  a->height = MAX(height_P(a->left), height_P(a->right))+1;

  return a; /* Return new node */
}

/* Compute the balance factor of a clients node, used to know if tree should rotate */
static int getBalance (ClientNode * node) {
  if (node == NULL) return 0;
  return height(node->left) - height(node->right);
}

/* Compute the balance factor of a products node, used to know if tree should rotate */
static int getBalance_P (ProductNode * node) {
  if (node == NULL) return 0;
  return height_P(node->left) - height_P(node->right);
}

/******************** CLIENTS ***********************/

/* Initiate the sales structure */
SalesC initSales () {
  int i;                                                  /* Iterator */
  SalesC s = (SalesC) malloc(sizeof(struct clientNode));  /* Allocate space for root node */
  s->client[0] = '\0';                                    /* Mark empty struct */
  s->height = 0;
  s->left = s->right = NULL;

  for (i = 0; i < 12; i++)  /* Set product pointers to NULL */
    s->products[i] = NULL;

  return s;                 /* Return the root node */
}

/* Function to insert a client in the AVL,
 * also acts as a moderator to check if structure was not initialized */
SalesC insertClientSC (SalesC s, char * client) {
  if (s == NULL) return NULL;         /* Sales structure was not initialized */
  else if (s->client[0] == '\0') {   /* First client to be inserted */
    strcpy(s->client, client);
    return s;
  }

  return addClient(s, client);  /* Insert client in the AVL tree */
}

/* Add a client to the AVL and return the new tree root if tree rotated */
static SalesC addClient (SalesC s, char * client) {
  SalesC result = s;             /* Function return value */
  int strcomp, balance, i, j;       /* strcmp() result, balance factor, auxiliar ints */

  if (s == NULL) {  /* New client */
    result = createNode(client);
  } else {
    strcomp = strcmp(client, s->client);        /* Compare clients */

    if (strcomp) {                                /* New client */
      if (strcomp > 0)                            /* Go to right subtree */
        s->right = addClient(s->right, client);
      else                                        /* Go to left subtree */
        s->left = addClient(s->left, client);

      s->height = MAX(height(s->left), height(s->right)) + 1; /* Update height */
      balance = getBalance(s);                                /* Get balance factor */

      if ((balance > 1) || (balance < -1)) {  /* Needs rotation */
        if (s->left == NULL) i = 0;
        else i = strcmp(client, s->left->client);
        if (s->right ==NULL) j = 0;
        else j = strcmp(client, s->right->client);

        /* Left Left Case */
        if (balance > 1 && i < 0) result = rightRotate(s);

        /* Right Right Case */
        if (balance < -1 && j > 0) result = leftRotate(s);

        /* Left Right Case */
        if (balance > 1 && i > 0) {
          s->left = leftRotate(s->left);
          result = rightRotate(s);
        }
        /* Right Left Case */
        if (balance < -1 && j < 0) {
          s->right = rightRotate(s->right);
          result = leftRotate(s);
        }
      }
    }
  }

  return result;  /* Return node */
}

/* Remove a client from the clients avl tree */
SalesC removeClientSC (SalesC s, char * client) {
  int strcomp, balance, i;  /* strcpm() result, balance factor, iterator */
  ClientNode * temp;        /* Store temporary pointer to be freed */
  ClientNode * result = s;  /* Store function return value */

  if (s) {
    strcomp = strcmp(client, s->client);

    if (strcomp < 0)                                  /* Go to left subtree */
      s->left = removeClientSC(s->left, client);
    else if (strcomp > 0)                             /* Go to right subtree */
      s->right = removeClientSC(s->right, client);
    else {                                            /* Client found */
      if (!s->left || !s->right) {
        temp = (s->left ? s->left : s->right);        /* Check which node to choose */

        if (!temp) {                                  /* No subtrees */
          for (i = 0; i < 12; i++) free(s->products[i]);
          temp = s;
          s = NULL; result = s;
        } else {
          strcpy(s->client, temp->client);

          for (i = 0; i < 12; i++)                    /* Free products pointers */
            free(s->products[i]);

          for (i = 0; i < 12; i++)                    /* Save new product pointers */
            s->products[i] = temp->products[i];

          s->left = temp->left; s->right = temp->right;
        }

        free(temp->client);     /* Free client string */
        free(temp);             /* Free node */
      } else {                                        /* Both subtrees */
        temp = s->right;
        strcpy(s->client, temp->client);
        s->right = removeClientSC(s->right, temp->client);
      }
    }

    if (s) {  /* If node had children */
      s->height = MAX(height(s->left), height(s->right)) + 1; /* Update heights */
      balance = getBalance(s);                                /* Get balance factor */

      if (balance > 1 && getBalance(s->left) >= 0) result = rightRotate(s);
      else if (balance < -1 && getBalance(s->right) <= 0) result = leftRotate(s);
      else if (balance > 1 && getBalance(s->left) < 0) {
        s->left = leftRotate(s->left);
        result = rightRotate(s);
      }
      else if (balance < -1 && getBalance(s->right) > 0) {
        s->right = rightRotate(s->right);
        result = leftRotate(s);
      }
    }
  }

  return result;
}

/* Get the client node of a given client code */
static ClientNode * getClient (SalesC s, char * client) {
  int strcomp;                /* To store the result of strcmp() */

  if (s == NULL) return NULL; /* Client is not in the sales structure */

  strcomp = strcmp(client, s->client);                      /* Compare clients */

  if (strcomp == 0) return s;                               /* Client found */
  else if (strcomp > 0) return getClient(s->right, client); /* Client may be on right tree*/
  else return getClient(s->left, client);                   /* Client may be on left tree */
}

/****************** PRODUCTS ************************/

/* Create a new product node with the give product and the given units */
static ProductNode * createProductNode (char * product, int units) {
  ProductNode * new = (ProductNode *) malloc(sizeof( ProductNode )); /* Allocate space */
  strcpy(new->product, product);                                     /* Save product */
  new->left = new->right = NULL;
  new->quant = units;
  new->height = 0;

  return new;  /* Return new node */
}

/* Insert a product in a given client node*/
SalesC insertProductSC (SalesC s, char * client, char * product, int month, int units) {
  ClientNode * node = getClient(s, client); /* Check if client exists */
  month = month - 1;

  if (!node) return NULL;   /* Client is not on the AVL */
  else if (node->products[month] == NULL) {
    node->products[month] = createProductNode(product, units); /* First product on month */
  } else {
    node->products[month] = addProduct(node->products[month], product, units);
  }

  return s; /* Return the clients AVL root */
}

/* Insert product on a AVL tree */
static ProductNode * addProduct (ProductNode * node, char * product, int units) {
  ProductNode * result = node;    /* Store the return value */
  int strcomp = 0, balance, i, j; /* Store the result of strcmp(), balance factor */

  if (!node) return createProductNode(product, units);  /* New product */
  else {
    strcomp = strcmp(product, node->product);           /* Compare products */

    if (strcomp == 0) { /* Product already inserted, update quantity */
      node->quant += units;
      result = node;
    }
    else if (strcomp > 0) {                 /* Insert on right subtree */
      result = node->right = addProduct(node->right, product, units);
    }
    else {                                  /* Insert on left subtree */
      result = node->left = addProduct(node->left, product, units);
    }

    /* Update node height and calculate balance factor */
    node->height = MAX(height_P(node->left), height_P(node->right)) + 1;
    balance = getBalance_P(node);

    if ((balance > 1) && (balance < -1)) {  /* Rotations needed */
      if (node->left == NULL) i = 0;
      else i = strcmp(product, (node->left)->product);
      if (node->right == NULL) j = 0;
      else j = strcmp(product, (node->right)->product);

      /* Left Left Case */
      if (balance > 1 && i < 0) result = rightRotate_P(node);       /* Left left case */
      else if (balance < -1 && j > 0) result = leftRotate_P(node);  /* Right right case */
      else if (balance > 1 && i > 0) {                              /* Left right case */
        node->left = leftRotate_P(node->left);
        result = rightRotate_P(node);
      }
      else if (balance < -1 && j < 0) {                               /* Right left case */
        node->right = rightRotate_P(node->right);
        result = leftRotate_P(node);
      }
    }
  }

  return result;
}

/* Helper function which insert the product code on the string list
 * if node is not NULL */
static StrList productsOnMonth_aux (ProductNode * node, StrList list, int * quants) {
  int i = 0, n = 0;     /* Iterator to shift strings */
  Bool done = false;    /* Helper to stop for loop */

  if (node) {
    list->clients[list->size] = malloc(sizeof(char) * 7); /* Allocate space for product */

    for (i = 0; (i < list->size) && !done; i++)
      if (node->quant > quants[i]) done = true; /* Insertion position found */

    if (done) {                                 /* Right shift all strings */
      i--;
      for (n = (list->size) - 1; n >= i; n--) {
        list->clients[n+1] = list->clients[n];
        quants[n+1] = quants[n];
      }
    }

    strcpy(list->clients[i], node->product);  /* Save product */
    quants[i] = node->quant;                  /* Save quantity */

    (list->size)++;                                   /* Update size */
    productsOnMonth_aux(node->left, list, quants);    /* Go to left subtree */
    productsOnMonth_aux(node->right, list, quants);   /* Go to right subtree */
  }

  return list;
}

/* Querie 9 - Determine list of products bought by a client on a given
 * month on descending order */
StrList productsOnMonth (SalesC sales, char * client, int month) {
  StrList list = NULL;                                /* List to store products */
  int * quants;                                       /* Array of quantitities */
  ClientNode * node = getClient(sales, client);       /* Check if client exists */

  if (node) { /* Client exists */
    quants = calloc(1, sizeof(int) * 40000);
    list = (StrList) malloc(sizeof(struct strlist));            /* Allocate space for list */
    list->clients[0] = malloc(sizeof(char) * 7);
    strcpy(list->clients[0], node->products[month-1]->product); /* Insert first product */
    list->size = 1;
    quants[0] = node->products[month-1]->quant;                 /* Save quantity */

    list = productsOnMonth_aux(node->products[month-1]->right, list, quants);
    list = productsOnMonth_aux(node->products[month-1]->left, list, quants);

  }

    return list;
}

/* Querie 10 - Create a list of strings with the clients that bought
 * items every month of the year
 * !!!!!!!!!!! NEEDS TESTING */
StrList yearlyClients (SalesC sales, StrList list) {
  int i;                /* Iterator */
  Bool bought = true;   /* Control if client bought every month */

  if (sales) {
    for (i = 0; (i < 12) && bought; i++)  /* Check if client bought every month */
      bought = bought && (sales->products[i] != NULL);

    if (bought) { /* Add client to list */
      list->clients[list->size] = malloc(sizeof(char) * 7); /* Allocate space for client */
      strcpy(list->clients[list->size], sales->client);
      (list->size)++;
    }

    yearlyClients(sales->left, list);
    yearlyClients(sales->right, list);
  }

  return list;
}

static StrList topProducts_aux (ProductNode * node, StrList list, int * quants) {
  int i = 0, n = 0;                     /* Auxiliar ints to find insertion position */
  char * temp;                          /* Save pointer */
  Bool done = false, found = false;     /* For loop control */

  if (node) {
    list->clients[list->size] = malloc(sizeof(char) * 7); /* Allocate space for new string */

    for (i = 0; (i < list->size) && !found; i++)  /* Search if product exists already */
      if (strcmp(node->product, list->clients[i]) == 0) found = true;

    if (found) {  /* Update product info */
      i--;
      quants[i] += node->quant; /* Update quantity */

      for (; (i > 0) && (quants[i] > quants[i-1]); i--) { /* Shift product if needed */
        n = quants[i-1];
        quants[i-1] = quants[i];
        quants[i] = n;
        temp = list->clients[i-1];
        strcpy(list->clients[i-1], node->product);
        list->clients[i] = temp;
      }
    }

    if (!found) { /* New product */
      for (i = 0; (i < list->size) && !done ; i++)  /* Check where to put new product */
        if (node->quant > quants[i]) done = true;   /* Insertion position found */

      if (done) { /* Shift all strings */
        i--;
        for (n = (list->size) - 1; n >= i; n--) {
          list->clients[n+1] = list->clients[n];
          quants[n+1] = quants[n];
        }
      }

      strcpy(list->clients[i], node->product);  /* Save product */
      quants[i] = node->quant;                  /* Save quantity */
      (list->size)++;
    }

    topProducts_aux(node->left, list, quants);
    topProducts_aux(node->right, list, quants);
  }

  return list;
}

/* Querie 13 - Given a client code determine the 3 products he bought the most */
StrList topProducts (SalesC sales, char * client) {
  int * quants, i = 0;                                /* Quantities array, iterator */
  ClientNode * node = getClient(sales, client);       /* Get client node */
  StrList list = NULL;

  if (node) { /* Client exists */
    list = (StrList) malloc(sizeof(struct strlist));  /* Allocate space for list */
    list->size = 0;
    quants = calloc(1, sizeof(int) * 40000);

    for (i = 0; i < 12; i++) {    /* Iterate trough all products bought */
      if (list->size == 0) {      /* First product to be inserted */
        list->clients[0] = strdup(node->products[i]->product);
        quants[0] = node->products[i]->quant;
        list->size = 1;
      }
      topProducts_aux(node->products[i], list, quants);
    }

    /* Check if there are more products with equal quantity sold */
    for (i = 3; (i < list->size) && (quants[i] == quants[i-1]); i++)
      ;

    list->size = i; /* Update size */
  }

  return list;
}

/* Calculates how many products were bought by a client in a certain month */
static int clientMonthSales (ProductNode * node) {
  if(node)
    return (1 + clientMonthSales(node->left) + clientMonthSales(node->right));

  return 0;
 }

/* Querie 5 - Calculates how many products a client bough by month */
ProductsN clientMonthlySales (SalesC sales, char * client) {
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
static ClientsMonth clientsMonthCalculate (SalesC node, ClientsMonth clientsMonth) {
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
ClientsMonth clientMonthlyPurchases (SalesC node) {
  int i;
  ClientsMonth monthlyPurchases;
  monthlyPurchases = (ClientsMonth) malloc(sizeof(struct ClientsMonthStruct));

  for (i = 0; i<12; i++) (monthlyPurchases->number[i])=0;
  monthlyPurchases = clientsMonthCalculate(node, monthlyPurchases);

  return monthlyPurchases;
}

/* Free products AVL tree */
static void freeProducts (ProductNode * node) {
  if (node) {
    freeProducts(node->left);   /* Free left subtree */
    freeProducts(node->right);  /* Free right subtree */

    free(node);                 /* Free product node */
  }
}

/* Free the memory used by the given SalesC */
void freeSalesC (SalesC sales) {
  int i;  /* Iterator */

  if (sales) {
    freeSalesC(sales->right);             /* Free right subtree */
    freeSalesC(sales->left);              /* Free left subtree */

    for (i = 0; i < 12; i++)          /* Free products AVL */
      freeProducts(sales->products[i]);

    free(sales);          /* Free sales node */
  }
}

