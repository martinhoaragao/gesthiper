#include <stdio.h>
#include <string.h>

#include "accounting.h"

int main() {

    FILE * fp = fopen("salesfile.txt", "r");

    if ( fp == NULL ){
    printf("O ficheiro não existe!\n");
    } 
    else {
        //Init Sales File
        init_accounting();
        char sale[20];
        fgets(sale, 20 ,fp);
 //       while ( fgets(sale, 20 ,fp) != NULL ){
            insert_product(sale);
 //   }
    }
}