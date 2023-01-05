#include "luof.h"

void percursoCategorias(sCat *cat, int hie) {
	for (int i = 0; i < hie; i++)
		printf("\t");
	printf(ANSI_BOLD_CYA "%s\n" ANSI_COLOR_WHT, cat->nome);
	for (int i = 0; i < hie; i++)
		printf("\t");
	printf("%s\n", cat->caminho);
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
	printf(ANSI_BOLD_YEL "Nome: %s\n" ANSI_COLOR_WHT, s.nome);
	printf("Categoria: %s\n", s.categoria);
	printf("Link: %s\n", s.link);
	printf("Texto: %s\n\n", s.texto);
}

void printaColorido() {
	printf("Teste de cores:\n");

	printf("\x1b[0;30m\\x1b[0;30m\n");
	printf("\x1b[0;31m\\x1b[0;31m\n");
	printf("\x1b[0;32m\\x1b[0;32m\n");
	printf("\x1b[0;33m\\x1b[0;33m\n");
	printf("\x1b[0;34m\\x1b[0;34m\n");
	printf("\x1b[0;35m\\x1b[0;35m\n");
	printf("\x1b[0;36m\\x1b[0;36m\n");
	printf("\x1b[0;37m\\x1b[0;37m\n\n");

	printf("\x1b[1;30m\\x1b[1;30m\n");
	printf("\x1b[1;31m\\x1b[1;31m\n");
	printf("\x1b[1;32m\\x1b[1;32m\n");
	printf("\x1b[1;33m\\x1b[1;33m\n");
	printf("\x1b[1;34m\\x1b[1;34m\n");
	printf("\x1b[1;35m\\x1b[1;35m\n");
	printf("\x1b[1;36m\\x1b[1;36m\n");
	printf("\x1b[1;37m\\x1b[1;37m\n\n");

	printf("\x1b[2;30m\\x1b[2;30m\n");
	printf("\x1b[2;31m\\x1b[2;31m\n");
	printf("\x1b[2;32m\\x1b[2;32m\n");
	printf("\x1b[2;33m\\x1b[2;33m\n");
	printf("\x1b[2;34m\\x1b[2;34m\n");
	printf("\x1b[2;35m\\x1b[2;35m\n");
	printf("\x1b[2;36m\\x1b[2;36m\n");
	printf("\x1b[2;37m\\x1b[2;37m\n\n");

	printf("\x1b[3;30m\\x1b[3;30m\n");
	printf("\x1b[3;31m\\x1b[3;31m\n");
	printf("\x1b[3;32m\\x1b[3;32m\n");
	printf("\x1b[3;33m\\x1b[3;33m\n");
	printf("\x1b[3;34m\\x1b[3;34m\n");
	printf("\x1b[3;35m\\x1b[3;35m\n");
	printf("\x1b[3;36m\\x1b[3;36m\n");
	printf("\x1b[3;37m\\x1b[3;37m\n\n");

	printf("\x1b[4;30m\\x1b[4;30m\n");
	printf("\x1b[4;31m\\x1b[4;31m\n");
	printf("\x1b[4;32m\\x1b[4;32m\n");
	printf("\x1b[4;33m\\x1b[4;33m\n");
	printf("\x1b[4;34m\\x1b[4;34m\n");
	printf("\x1b[4;35m\\x1b[4;35m\n");
	printf("\x1b[4;36m\\x1b[4;36m\n");
	printf("\x1b[4;37m\\x1b[4;37m\n\n");

	printf("\x1b[0;90m\\x1b[0;90m\n");
	printf("\x1b[0;91m\\x1b[0;91m\n");
	printf("\x1b[0;92m\\x1b[0;92m\n");
	printf("\x1b[0;93m\\x1b[0;93m\n");
	printf("\x1b[0;94m\\x1b[0;94m\n");
	printf("\x1b[0;95m\\x1b[0;95m\n");
	printf("\x1b[0;96m\\x1b[0;96m\n");
	printf("\x1b[0;97m\\x1b[0;97m\n\n");

	printf("\x1b[1;90m\\x1b[1;90m\n");
	printf("\x1b[1;91m\\x1b[1;91m\n");
	printf("\x1b[1;92m\\x1b[1;92m\n");
	printf("\x1b[1;93m\\x1b[1;93m\n");
	printf("\x1b[1;94m\\x1b[1;94m\n");
	printf("\x1b[1;95m\\x1b[1;95m\n");
	printf("\x1b[1;96m\\x1b[1;96m\n");
	printf("\x1b[1;97m\\x1b[1;97m\n\n");

}

void fTeste() {

	sBanco db;

	if (fInicializaDB(&db))
		return;

	percursoCategorias(db.listaCategorias, 0);

	fLiberaCats(db.listaCategorias);
	printf("\n");
	printaListaSites(db.raiz);

}
