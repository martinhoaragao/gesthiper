#ifndef SALES_H
#define SALES_H

#include "../includes/salesstructs.h"
#include "../includes/StrList.h"

typedef struct clientNode * SalesC;

SalesC initSales ();
SalesC insertClientSC (SalesC, char *);
SalesC removeClientSC (SalesC, char *);
SalesC insertProductSC (SalesC, char *, char *, int, int);
StrList yearlyClients (SalesC, StrList);
ProductsN clientMonthlySales (SalesC, char *);
StrList productsOnMonth (SalesC, char * , int);
StrList topProducts (SalesC, char *);
ClientsMonth clientMonthlyPurchases (SalesC);

#endif
