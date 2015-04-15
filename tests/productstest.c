#include "products.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	char filename[100];
	char prod[6];
	char letra;
	ProductsCat* cat = initProductsCat();
	ProductsCat* catclone;
	PList *lista;
	int noscope = 0; int quack = 0;
	printf("Qual o nome do ficheiro a ler?\n");
	//scanf("%s", filename);
	
	FILE *fr = fopen("FichProdutos.txt", "r");
	if (fr == NULL){
		printf("O ficheiro não existe!\n");
		return 1;
	}

	int validPCodes = 0, i=0, length;
	char key[6];
	while(fgets(key, 32, fr)){
		length = strlen(key); 
		validPCodes += insert_product(cat, key);
		if(i<194000) search2(cat, key);
		i++;
	}
	printf("O ficheiro %s foi lido.\n", filename);
	printf("Foram lidas %d linhas.\n", i);
	printf("Foram validadas %d linhas.\n", validPCodes);


	printf("Qual o produto que quer procurar?\n");
	scanf("%s", prod);
	printf("%d\n", search(cat, prod));


	printf("Remove code\n");
	scanf("%s", prod);
	if(search(cat, prod)){
		catclone = removeProduct(cat, prod);
	}else printf("error\n");


	PList *xs = notboughtProducts(cat);
	noscope=getQnt(xs);
	printf("%d\n", noscope);
	for(quack=0; quack<noscope; quack++){
		printf("%s\n", getCode(xs, quack));
	}


	/* Querie 2 */
	printf("Querie 2\nQual a inicial pela qual quer procurar?\n");
	scanf(" %c", &letra);

	int n = 0, x = 30, max; char keys;
	PList *p = searchI(cat, letra);
	max = getQnt(p);

	while(1){
		printf("A mostrar resultados %d-%d de %d.\n", (n+1), (n+31), max);
		for(; n < x; n++)
			printf("%d. %s\n", (n+1), getCode(p, n));

		printf("S pra seguinte, A para anterior, Q para sair\n");
		scanf(" %c", &keys);
		if(keys=='Q' || keys=='q') break;
		else if(keys=='S' || keys=='s') x += 30;
		else if(keys=='A' || keys=='a'){
			if(n >= 60) {n-=60; x-=30;}
			else printf("error\n");
		}else printf("Escolha inválida\n");
	} 

	return 0;
}
