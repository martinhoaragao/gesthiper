#ifndef SALESSTRUCTS_H
#define SALESSTRUCTS_H

/* Pointer to an array of ints by month */
typedef struct ProductsNStruct{
  int productsBought[12];
} * ProductsN;

/* Pointer to an array of ints by month */
typedef struct ClientsMonthStruct{
  int number[12];
} * ClientsMonth;

#endif
