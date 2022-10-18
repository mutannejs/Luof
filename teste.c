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

void printaListaSites(sLista l) {
	if (!emptyList(l)) {
		sIterador it = criaIt(l);
		sSite *fav;

		do {
			fav = retornaItera(&it);
			printaSite(*fav);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}
	else {
		printf("lista vazia\n");
	}
}

void printaSite(sSite s) {
	
	printf("Nome: %s\n", s.nome);
	printf("Categoria: %s\n", s.categoria);
	printf("Link: %s\n", s.link);
	printf("Texto: %s\n", s.texto);
	printf("ehCat: %c\n\n", s.ehCat);

}
