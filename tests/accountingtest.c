#include <stdio.h>
#include <string.h>

#include "../accounting.h"

int main() {
float yay;
    FILE * fp = fopen("salesfile.txt", "r");

    if ( fp == NULL ){
    printf("O ficheiro não existe!\n");
    }
    else {
        char sale[40];

        while ( fgets(sale, 40 ,fp) )
            insert_product(sale);
    }
    if((searchProduct("QC9889"))==0) printf("NAY\n");
    else printf("YAY\n");
    yay = getMonthSale(4, 'p', "IP8535");
    printf("%f\n", yay);
}
