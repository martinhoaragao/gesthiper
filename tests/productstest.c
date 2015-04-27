#include "../src/products.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	char filename[100], prod[6], key[6], letra, keys;
	int qnt = 0, index = 0, validPCodes = 0, i = 0;
	int n = 0, x = 30, max, garbage;
	ProductsCat* cat = initProductsCat();
	PList *xs, *p;
	FILE *fr;



	printf("Qual o nome do ficheiro a ler?\n");
	garbage = scanf("%s", filename);

	fr = fopen(filename, "r");
	if (fr == NULL){
		printf("O ficheiro não existe!\n");
		return 1;
	}

	while(fgets(key, 10, fr)){
		validPCodes += insert_product(cat, key);
		if(i<194000) search2(cat, key);
		i++;
	}
	printf("O ficheiro %s foi lido.\n", filename);
	printf("Foram lidas %d linhas.\n", i);
	printf("Foram validadas %d linhas.\n", validPCodes);


	printf("Qual o produto que quer procurar?\n");
	garbage = scanf("%s", prod);
	printf("%d\n", search(cat, prod));


	printf("Remove code\n");
	garbage = scanf("%s", prod);
	if(search(cat, prod)){
		cat = removeProduct(cat, prod);
	}else printf("error\n");


	xs = notboughtProducts(cat);
	qnt = getQnt(xs);
	printf("%d\n", qnt);
	for(index=0; index < qnt; index++){
		printf("%s\n", getCode(xs, index));
	}


	/* Querie 2 */
	printf("Querie 2\nQual a inicial pela qual quer procurar?\n");
	garbage = scanf(" %c", &letra);

	p = searchI(cat, letra);
	max = getQnt(p);

	i=0;
	while(1){
		if(i==1)printf("A mostrar resultados %d-%d de %d.\n", (n+1), (n+31), max);
		for(; n < x; n++)
			printf("%d. %s\n", (n+1), getCode(p, n));

		printf("S pra seguinte, A para anterior, Q para sair\n");
		garbage = scanf(" %c", &keys);
		if(keys=='Q' || keys=='q') break;
		else if(keys=='S' || keys=='s'){
			i = 1;
			if((x+30) > max) x = max;
			else x += 30;
		}
		else if(keys=='A' || keys=='a'){
			i = 1;
			if(n >= 60) {n-=60; x-=30;}
			else printf("error\n");
		}else printf("Escolha inválida\n");
	}
	garbage = 0;
	return garbage;
}
