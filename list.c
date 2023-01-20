#include "luof.h"

void fListCategory(int opcao) {

	sSite *favorito, s;
	sCat *cat, *catTemp;
	sBanco db;
	sIterador it;

	if (fInicializaDB(&db))
		return;

	if (fSetaSiteCategoria(&s)) {
		fFinalizaDB(&db);
		return;
	}
	if (fBuscaCat(&db, s.categoria, &cat)) {
		fFinalizaDB(&db);
		return;
	}
	fPreencheListaSite(&db, cat, 1);

	//se a categoria não possui subcategorias nem favoritos
	if (emptyList(cat->catFilhos) && emptyList(db.listaFavs)) {
		printf(ERRO);
		printf("Lista vazia.\n");
		fFinalizaDB(&db);
		return;
	}

	//printa categoria por categoria
	if (!emptyList(cat->catFilhos)) {
		printf("\n");
		it = criaIt(cat->catFilhos);
		do {
			catTemp = (struct sCat*) retornaItera(&it);
			printf(ANSI_BOLD_WHT "* %s\n", catTemp->nome);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

	//printa favorito por favorito
	if (!emptyList(db.listaFavs)) {

		if (opcao == 1 && !emptyList(cat->catFilhos))
			printf("\n");

		it = criaIt(db.listaFavs);
		do {
			favorito = (struct sSite*) retornaItera(&it);
			if (opcao == 0) {
				printf(ANSI_BOLD_WHT "\nNome  : " ANSI_COLOR_GRA "%s\n", favorito->nome);
				printf(ANSI_BOLD_WHT "Link  : " ANSI_COLOR_GRA "%s\n", favorito->link);
				printf(ANSI_BOLD_WHT "Texto : " ANSI_COLOR_GRA "%s\n", favorito->texto);
			}
			else {
				printf(ANSI_COLOR_GRA "%s\n", favorito->nome);
			}
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

	fFinalizaDB(&db);

}

void fListTree_printaLinhas(char linhas[], int hierarquia) {

	if (linhas[0] == '1')
		printf(ANSI_COLOR_GRA "|");
	else
		printf(" ");

	for (int i = 1; i < hierarquia+1; i++) {
		if (linhas[i] == '1')
			printf(ANSI_COLOR_GRA "    |");
		else
			printf("     ");
	}

}

void fListTree_private(sBanco *db, char linhas[], sCat *cat, int hierarquia, int opcao) {

	sSite *favorito;
	sCat *catTemp;
	sLista listaTemp;
	sIterador it;

	hierarquia = hierarquia + 1;
	linhas[hierarquia] = '1';
	fPreencheListaSite(db, cat, 1);
	listaTemp = db->listaFavs;
	db->listaFavs = NULL;

	//printa todas as subcategorias
	if (!emptyList(cat->catFilhos)) {
		it = criaIt(cat->catFilhos);
		do {
			catTemp = (struct sCat*) retornaItera(&it);

			//printa a categoria
			fListTree_printaLinhas(linhas, hierarquia);
			printf(ANSI_COLOR_GRA "_ " ANSI_BOLD_WHT "* %s\n", catTemp->nome);
			//printa suas subcategorias e favoritos
			if (fimIt(&it) && (emptyList(listaTemp) || opcao == 1))
				linhas[hierarquia] = '0';
			fListTree_private(db, linhas, catTemp, hierarquia, opcao);

			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

	//printa favorito por favorito da categoria
	linhas[hierarquia] = '1';
	db->listaFavs = listaTemp;
	if (opcao == 0 && !emptyList(db->listaFavs)) {
		it = criaIt(db->listaFavs);
		do {
			favorito = (struct sSite*) retornaItera(&it);

			//printa o favorito
			fListTree_printaLinhas(linhas, hierarquia);
			printf(ANSI_COLOR_GRA "_ " ANSI_COLOR_WHT "%s\n", favorito->nome);

			iteraProximo(&it);
		} while (!inicioIt(&it));
	}
	freeList(db->listaFavs);
	linhas[hierarquia] = '0';

}

void fListTree(int opcao) {

	sSite *favorito;
	sCat *catTemp;
	sBanco db;
	sLista listaTemp;
	sIterador it;
	char linhas[] = "10000000000";

	if (fInicializaDB(&db))
		return;

	fPreencheListaSite(&db, db.arvoreCats, 0);
	if (emptyList(db.listaFavs) && emptyList(db.arvoreCats->catFilhos)) {
		printf(ERRO2);
		printf("Lista vazia.\n");
		fFinalizaDB(&db);
		return;
	}

	fPreencheListaSite(&db, db.arvoreCats, 1);
	listaTemp = db.listaFavs;
	db.listaFavs = NULL;

	//printa todas as categorias e seus favoritos
	if (!emptyList(db.arvoreCats->catFilhos)) {
		it = criaIt(db.arvoreCats->catFilhos);
		do {
			catTemp = (struct sCat*) retornaItera(&it);

			//printa a categoria
			fListTree_printaLinhas(linhas, 0);
			printf(ANSI_COLOR_GRA "_ " ANSI_BOLD_WHT "* %s\n", catTemp->nome);
			//printa suas subcategorias e favoritos
			if (fimIt(&it) && (emptyList(listaTemp) || opcao == 1))
				strcpy(linhas, "00000000000");
			fListTree_private(&db, linhas, catTemp, 0, opcao);

			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

	//printa os favoritos da raiz
	db.listaFavs = listaTemp;
	if (opcao == 0 && !emptyList(db.listaFavs)) {
		it = criaIt(db.listaFavs);
		do {
			favorito = (struct sSite*) retornaItera(&it);

			//printa o favorito
			fListTree_printaLinhas(linhas, 0);
			printf(ANSI_COLOR_GRA "_ " ANSI_COLOR_WHT "%s\n", favorito->nome);

			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

	fFinalizaDB(&db);

}
