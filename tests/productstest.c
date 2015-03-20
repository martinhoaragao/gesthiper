#include "../products.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	char filename[100];
	
	printf("Qual o nome do ficheiro a ler?\n");
	scanf("%s", filename);
	
	FILE *fr = fopen(filename, "r");
	if (fr == NULL){
		printf("O ficheiro não existe!\n");
		return 1;
	}

	int validPCodes = 0, i=0, length;
	char key[6];
	while(fgets(key, 10, fr)){
		length = strlen(key); 
		validPCodes += insert_product(key);
		i++;
	}
	

	printf("O ficheiro %s foi lido.\n", filename);
	printf("Foram lidas %d linhas.\n", i);
	printf("Foram validadas %d linhas.\n", validPCodes);
	
	return 0;
}
