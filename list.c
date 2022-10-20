#include "luof.h"

void fListCategory(int opcao) {

	sSite s;
	sCat *categoria = malloc(sizeof(sCat));
	sBanco db;

	if (fInicializaDB(&db))
		return;

	printf("Categoria:\n");
	scanf(" %[^\n]", s.categoria);

	if (strcmp(s.categoria, "/") == 0) {
		strcpy(s.categoria, "luof");
		db.listaSites = db.raiz;
	}
	else {
		if (fBuscaCat(&db, s, &categoria))
			return;
		if (fPreencheListaSite(&db, categoria))
			return;
	}
	
	printf("\n");
	
	if (emptyList(db.listaSites)) {
		printf("Lista vazia.\n");
		return;
	}
	
	sSite *siteDoIterador;
	sIterador it = criaIt(db.listaSites);
	
	//printa favorito por favorito
	do {
		siteDoIterador = retornaItera(&it);
		if (strcmp(siteDoIterador->categoria, s.categoria) == 0) {
			if (siteDoIterador->ehCat == '1') {
				printf("* %s\n", siteDoIterador->nome);
			}
			else if (opcao == 0) {
				printf("%s\n", siteDoIterador->nome);
				printf("\tLink: %s\n", siteDoIterador->link);
				printf("\tTexto: %s\n", siteDoIterador->texto);
			}
			else {
				printf("%s\n", siteDoIterador->nome);
			}
		}
		iteraProximo(&it);
	} while(!inicioIt(&it));

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

sLista fListTree_preencheSites(sSite s) {
	
	sSite siteTemp;
	sLista listaFavoritos;
	FILE *arqCat;
	char categoria[TAMLINKARQ], nomeArqCat[TAMLINKARQ], nomeTemp[TAMNOMEFAV], ehCategoria[3];
	int tamanho;
	
	if (strcmp(s.categoria, "luof") == 0) {
		strcpy(categoria, s.nome);
	}
	else {
		strcpy(categoria, s.categoria);
		fIncrementaCamCat(categoria, s.nome);
	}
	
	fSetaCaminhoArquivo(nomeArqCat, s.nome);
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
	listaFavoritos = fListTree_preencheSites(s);

	//se a categoria estiver vazia
	if (emptyList(listaFavoritos)) {
		freeList(listaFavoritos);
		return;
	}

	//printa os favoritos da categoria
	it = criaIt(listaFavoritos);
	do {
		siteDoIterador = retornaItera(&it);
		
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

	sBanco db;
	sSite *siteDoIterador;
	sIterador it;
	char linhas[] = "10000000000";

	if (fInicializaDB(&db))
		return;

	//caso o banco esteja vazio
	if (emptyList(db.raiz)) {
		printf("Nada ainda foi inserido\n");
		return;
	}

	it = criaIt(db.raiz);
	siteDoIterador = retornaItera(&it);

	//printa as categorias da raiz
	if (siteDoIterador->ehCat == '1') {
		do {
			fListTree_printaFavorito(*siteDoIterador, linhas, 0);
			if (fimIt(&it))
				strcpy(linhas, "00000000000");
			fListTree_private(&db, linhas, *siteDoIterador, 0);
			iteraProximo(&it);
			siteDoIterador = retornaItera(&it);
		} while (!inicioIt(&it) && siteDoIterador->ehCat == '1');
	}
	
	strcpy(linhas, "10000000000");

	//printa os sites da raiz
	if (siteDoIterador->ehCat == '0') {
		do {
			fListTree_printaFavorito(*siteDoIterador, linhas, 0);
			iteraProximo(&it);
			siteDoIterador = retornaItera(&it);
		} while (!inicioIt(&it) && siteDoIterador->ehCat == '0');
	}

	fFinalizaDB(&db);

}
