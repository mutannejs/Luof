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

	sSite s;
	sCat *cat;
	sBanco db;

	if (fInicializaDB(&db))
		return;

	percursoCategorias(db.arvoreCats, 0);
	fEscreveLuof(&db);
	printf("\n");
	if (fBuscaCat(&db, "ufscar", &cat))
		printf("Categoria ufscar não encontrada.\n");
	else
		printf("Cat %s encontrada.\n", cat->nome);
	if (fBuscaCat(&db, "ufscar/maritacas gamedev", &cat))
		printf("Categoria ufscar/maritacas gamedev não encontrada.\n");
	else
		printf("Cat %s encontrada.\n", cat->nome);
	if (fBuscaCat(&db, "ufscar/hackoon space", &cat))
		printf("Categoria ufscar/hackoon space não encontrada.\n");
	else
		printf("Cat %s encontrada.\n", cat->nome);
	if (fBuscaCat(&db, "receitas", &cat))
		printf("Categoria receitas não encontrada.\n");
	else
		printf("Cat %s encontrada.\n", cat->nome);
	if (fBuscaCat(&db, "receitas/dona dirce", &cat))
		printf("Categoria receitas/dona dirce não encontrada.\n");
	else
		printf("Cat %s encontrada.\n", cat->nome);
	if (fBuscaCat(&db, "ferramentas", &cat))
		printf("Categoria ferramentas não encontrada.\n");
	else
		printf("Cat %s encontrada.\n", cat->nome);
	if (fBuscaCat(&db, "ferramentas/programação", &cat))
		printf("Categoria ferramentas/programação não encontrada.\n");
	else
		printf("Cat %s encontrada.\n", cat->nome);
	if (fBuscaCat(&db, "ferramentas/programação/teste", &cat))
		printf("Categoria ferramentas/programação/teste não encontrada.\n");
	else
		printf("Cat %s encontrada.\n", cat->nome);
	if (fBuscaCat(&db, "windows 7", &cat))
		printf("Categoria windows 7 não encontrada.\n");
	else
		printf("Cat %s encontrada.\n", cat->nome);
	if (fBuscaCat(&db, "linux", &cat))
		printf("Categoria linux não encontrada.\n");
	else
		printf("Cat %s encontrada.\n", cat->nome);
	if (fBuscaCat(&db, "linux/debian", &cat))
		printf("Categoria linux/debian não encontrada.\n");
	else
		printf("Cat %s encontrada.\n", cat->nome);
	if (fBuscaCat(&db, "programação", &cat))
		printf("Categoria programação não encontrada.\n");
	else
		printf("Cat %s encontrada.\n", cat->nome);
	if (fBuscaCat(&db, "programação/C", &cat))
		printf("Categoria programação/C não encontrada.\n");
	else
		printf("Cat %s encontrada.\n", cat->nome);
	if (fBuscaCat(&db, "programa", &cat))
		printf("Categoria programa não encontrada.\n");
	else
		printf("Cat %s encontrada.\n", cat->nome);
	if (fBuscaCat(&db, "programação/B", &cat))
		printf("Categoria programação/B não encontrada.\n");
	else
		printf("Cat %s encontrada.\n", cat->nome);
	if (fBuscaCat(&db, "windows 7/teste", &cat))
		printf("Categoria windows 7/teste não encontrada.\n");
	else
		printf("Cat %s encontrada.\n", cat->nome);
	printf("\n");

	//preenche uma sLista com todos os favoritos da raiz
	fPreencheListaSite(&db, db.arvoreCats, 1);
	printaListaSites(db.listaFavs);
	strcpy(s.nome, "ava");
	strcpy(s.categoria, "/");
	if (!fBuscaFavorito(&db, &s))
		printf("Favorito com nome %s não encontrado.\n", s.nome);
	else
		printf("Favorito com nome %s encontrado.\n", s.nome);
	strcpy(s.nome, "bso");
	strcpy(s.categoria, "/");
	if (!fBuscaFavorito(&db, &s))
		printf("Favorito com nome %s não encontrado.\n", s.nome);
	else
		printf("Favorito com nome %s encontrado.\n", s.nome);
	strcpy(s.nome, "sagui");
	strcpy(s.categoria, "/");
	if (!fBuscaFavorito(&db, &s))
		printf("Favorito com nome %s não encontrado.\n", s.nome);
	else
		printf("Favorito com nome %s encontrado.\n", s.nome);
	strcpy(s.nome, "siga");
	strcpy(s.categoria, "/");
	if (!fBuscaFavorito(&db, &s))
		printf("Favorito com nome %s não encontrado.\n", s.nome);
	else
		printf("Favorito com nome %s encontrado.\n", s.nome);
	strcpy(s.nome, "sig");
	strcpy(s.categoria, "/");
	if (!fBuscaFavorito(&db, &s))
		printf("Favorito com nome %s não encontrado.\n", s.nome);
	else
		printf("Favorito com nome %s encontrado.\n", s.nome);
	strcpy(s.nome, "sigao");
	strcpy(s.categoria, "/");
	if (!fBuscaFavorito(&db, &s))
		printf("Favorito com nome %s não encontrado.\n", s.nome);
	else
		printf("Favorito com nome %s encontrado.\n", s.nome);
	strcpy(s.nome, "siga");
	strcpy(s.categoria, "teste");
	if (!fBuscaFavorito(&db, &s))
		printf("Favorito com nome %s não encontrado.\n", s.nome);
	else
		printf("Favorito com nome %s encontrado.\n", s.nome);
	fEscreveArquivoCat(&db, "raiz");

	fFinalizaDB(&db);

}
