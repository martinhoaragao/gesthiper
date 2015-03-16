#include <stdio.h>
#include <string.h>

#include "accounting.h"

int main() {

    FILE * fp = fopen("salesfile.txt", "r");

    if ( fp == NULL ){
    printf("O ficheiro não existe!\n");
    } 
    else {
        char sale[30];
        fgets(sale, 30 ,fp);
 //       while ( fgets(sale, 20 ,fp) != NULL ){
            insert_product(sale);
 //           fgets(sale, 20 ,fp);
 //           insert_product(sale);
 //   }
    }
}