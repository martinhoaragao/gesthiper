#ifndef OVERALLSALES_H
#define OVERALLSALES_H

/* Calculated sum up of sales */
typedef struct {
  /* Calculates how many sales */
  int numberSales; 
  /* Number of normal sales */
  int normalNumber;
  /* Number of sales in promotion */
  int promotionNumber;
  /* Total income */
  double income;
} OverallSales;

#endif
