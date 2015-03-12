#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "accounting.h"

//#include "accounting.h"

static ProductSales* data;

int init_accounting(){
    int i=0;
    char teste[40];

    strncpy(teste, "Sou mêmo fixe/ye.yeye", 40); 
    data = (ProductSales*) malloc( sizeof(ProductSales) );

    return 0;
}

int insert_product(char * sep){
    strtok(sep, " "); 
    strncpy(data->code, sep, 40);
    data->normal[0] = atoi( strtok(0, "") );
    printf("%d\n", data->normal[0]); 
    printf("%s\n", strtok(0, "")); 
}
