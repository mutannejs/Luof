#include "luof.h"

void percursoCategorias(sCat *cat, int hie) {
	for (int i = 0; i < hie; i++)
		printf("\t");
	printf("%s\n", cat->nome);
	if (!emptyList(cat->catFilhos)) {
		sIterador it = criaIt(cat->catFilhos);
		do {
			sCat *catF = (struct sCat*) retornaItera(&it);
			percursoCategorias(catF, hie+1);
			iteraProximo(&it);
		} while (!inicioIt(&it));
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

void printaColorido() {
	printf(ANSI_CLEAR);
	printf("Teste de cores:\n");
	printf(ANSI_COLOR_RED "Esse texto está em vermelho?\n" ANSI_COLOR_RESET);
	printf(ANSI_COLOR_GRAY "Se sim, esse texto deve estar em cinza.\n" ANSI_COLOR_RESET);
	printf("Fim do teste.\n");
}
