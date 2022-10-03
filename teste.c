#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "luof.h"

void percursoCategorias(sLista l) {
	if (!emptyList(l)) {
		printf("inicio lista\n");
		sIterador it = criaIt(l);
		do {
			sCat *cat = (struct sCat*) retornaItera(&it);
			printf("\t%s\n", cat->nome);
			percursoCategorias(cat->catFilhos);
			iteraProximo(&it);
		} while (!inicioIt(&it));
		printf("fim lista\n");
	}
	else {
		printf("lista vazia\n");
	}
}

void printaSite(sLista l) {
	
	sIterador it = criaIt(l);
	sSite *fav;

	do {
		fav = retornaItera(&it);
		printf("Nome: %s\n", fav->nome);
		printf("Categoria: %s\n", fav->categoria);
		printf("Link: %s\n", fav->link);
		printf("Texto: %s\n", fav->texto);
		printf("ehCat: %c\n\n", fav->ehCat);
		iteraProximo(&it);
	} while (!inicioIt(&it));
}
