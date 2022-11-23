#include "luof.h"

void fListCategory(int opcao) {

	sSite s, *siteDoIterador;
	sCat *categoria;
	sBanco db;
	sIterador it;
	int cont = 0;

	if (fInicializaDB(&db))
		return;

	//caso o banco esteja vazio
	if (emptyList(db.raiz)) {
		printf("Nada ainda foi inserido.\n");
		fFinalizaDB(&db);
		return;
	}

	if (fSetaSiteCategoria(&s))
		return;

	if (strcmp(s.categoria, "/") == 0) {
		strcpy(s.categoria, "luof");
		db.listaSites = db.raiz;
		categoria = db.listaCategorias;
	}
	else {
		if (fBuscaCat(&db, s, &categoria)) {
			fFinalizaDB(&db);
			return;
		}
		if (fPreencheListaSite(&db, categoria)) {
			fFinalizaDB(&db);
			return;
		}
	}

	if (emptyList(db.listaSites)) {
		printf("\nLista vazia.\n");
		fFinalizaDB(&db);
		return;
	}

	//printa favorito por favorito
	it = criaIt(db.listaSites);
	do {
		siteDoIterador = (struct sSite*) retornaItera(&it);
		if (strcmp(siteDoIterador->categoria, s.categoria) == 0) {
			if ((siteDoIterador->ehCat == '0' && opcao == 0) || (cont == 0 && siteDoIterador->ehCat == '1'))
				printf("\n");
			if (siteDoIterador->ehCat == '1') {
				printf("* %s\n", siteDoIterador->nome);
			}
			else if (opcao == 0) {
				printf("Nome  : %s\n", siteDoIterador->nome);
				printf("Link  : %s\n", siteDoIterador->link);
				printf("Texto : %s\n", siteDoIterador->texto);
			}
			else {
				printf("%s\n", siteDoIterador->nome);
			}
			cont++;
		}
		iteraProximo(&it);
	} while (!inicioIt(&it));

	//se db.listaSites não está vazio, mas não possui favoritos dessa categoria
	if (cont == 0) {
		printf("\nLista vazia.");
	}

	printf("\n");

	fFinalizaDB(&db);

}

void fListTree_printaFavorito(sSite s, char linhas[], int hierarquia) {
	if (linhas[0] == '1')
		printf("|");
	else
		printf(" ");

	for (int i = 1; i < hierarquia+1; i++) {
		if (linhas[i] == '1')
			printf("    |");
		else
			printf("     ");
	}

	if (s.ehCat == '1')
		printf("_ * %s\n", s.nome);
	else
		printf("_ %s\n", s.nome);
}

sLista fListTree_preencheSites(sBanco *db, sSite s) {

	sSite siteTemp;
	sLista listaFavoritos;
	FILE *arqCat;
	char categoria[TAMCAMINHO], nomeArqCat[TAMLINKARQ], nomeTemp[TAMNOMEFAV], ehCategoria[3];
	int tamanho;

	if (strcmp(s.categoria, "luof") == 0) {
		strcpy(categoria, s.nome);
	}
	else {
		strcpy(categoria, s.categoria);
		fIncrementaCamCat(categoria, s.nome);
	}

	fSetaCaminhoArquivo(db, nomeArqCat, s.nome);
	arqCat = fopen(nomeArqCat, "r");
	listaFavoritos = criaLista(struct sSite);

	while (fgets(nomeTemp, 100, arqCat) != NULL) {

		tamanho = strlen(nomeTemp);
		nomeTemp[tamanho-1] = '\0';
		strcpy(siteTemp.nome, nomeTemp);

		fgets(siteTemp.categoria, TAMCAMINHO, arqCat);
		tamanho = strlen(siteTemp.categoria);
		siteTemp.categoria[tamanho-1] = '\0';

		fgets(siteTemp.link, TAMLINKARQ, arqCat);
		tamanho = strlen(siteTemp.link);
		siteTemp.link[tamanho-1] = '\0';

		fgets(siteTemp.texto, TAMTEXTO, arqCat);
		tamanho = strlen(siteTemp.texto);
		siteTemp.texto[tamanho-1] = '\0';

		fgets(ehCategoria, 3, arqCat);
		siteTemp.ehCat = ehCategoria[0];

		//adiciona na lista apenas se pertencer a categoria correta
		if (strcmp(siteTemp.categoria, categoria) == 0)
			pushBackList(listaFavoritos, &siteTemp);
	}

	fclose(arqCat);
	
	return listaFavoritos;

}

void fListTree_private(sBanco *db, char linhas[], sSite s, int hierarquia) {

	sSite *siteDoIterador;
	sLista listaFavoritos = NULL;
	sIterador it;

	hierarquia = hierarquia + 1;
	linhas[hierarquia] = '1';

	//preenche a lista de favoritos e guarda em listaFavoritos
	listaFavoritos = fListTree_preencheSites(db, s);

	//se a categoria estiver vazia
	if (emptyList(listaFavoritos)) {
		freeList(listaFavoritos);
		return;
	}

	//printa os favoritos da categoria
	it = criaIt(listaFavoritos);
	do {
		siteDoIterador = (struct sSite*) retornaItera(&it);

		//printa a categoria, obedecendo a hierarquia
		fListTree_printaFavorito(*siteDoIterador, linhas, hierarquia);

		if (fimIt(&it))
			linhas[hierarquia] = '0';
		if (siteDoIterador->ehCat == '1')
			fListTree_private(db, linhas, *siteDoIterador, hierarquia);

		iteraProximo(&it);
	} while (!inicioIt(&it));

	linhas[hierarquia] = '0';
	freeList(listaFavoritos);

}

void fListTree() {

	sSite *siteDoIterador;
	sBanco db;
	sIterador it;
	char linhas[] = "10000000000";

	if (fInicializaDB(&db))
		return;

	if (emptyList(db.raiz)) {
		printf("Nada ainda foi inserido.\n");
		fFinalizaDB(&db);
		return;
	}

	it = criaIt(db.raiz);
	siteDoIterador = (struct sSite*) retornaItera(&it);

	//printa as categorias da raiz
	if (siteDoIterador->ehCat == '1') {
		do {
			fListTree_printaFavorito(*siteDoIterador, linhas, 0);
			if (fimIt(&it))
				strcpy(linhas, "00000000000");
			fListTree_private(&db, linhas, *siteDoIterador, 0);
			iteraProximo(&it);
			siteDoIterador = (struct sSite*) retornaItera(&it);
		} while (!inicioIt(&it) && siteDoIterador->ehCat == '1');
	}

	strcpy(linhas, "10000000000");

	//printa os sites da raiz
	if (siteDoIterador->ehCat == '0') {
		do {
			fListTree_printaFavorito(*siteDoIterador, linhas, 0);
			iteraProximo(&it);
			siteDoIterador = (struct sSite*) retornaItera(&it);
		} while (!inicioIt(&it) && siteDoIterador->ehCat == '0');
	}

	printf("\n");

	fFinalizaDB(&db);

}
