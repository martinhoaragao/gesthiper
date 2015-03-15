#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "accounting.h"

static ProductSales* data;

int init_accounting(){

    data = (ProductSales*) malloc( sizeof(ProductSales) );

    return 0;
}

int insert_product(char * sep){
    strncpy(data->code, strtok(sep, " ") , 40);
    data->normal[0] = atof( strtok(0, "") );
    printf("%f\n", data->normal[0]); 
    printf("%s\n", data->code); 
}
