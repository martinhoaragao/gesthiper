#include <stdio.h>
#include <string.h>

#include "accounting.h"

int main() {

    FILE * fp = fopen("salesfile.txt", "r");

    if ( fp == NULL ){
    printf("O ficheiro não existe!\n");
    } 
    else {
        char sale[40];

        while ( fgets(sale, 40 ,fp) )
            insert_product(sale);
    }
}