#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "luof.h"

void fListCategory(int opcao) {

	//variaveis
	sSite s;
	sCat categoria;
	sBanco db;

	// inicializa o banco de dados (se existir guarda em aLuof o arquivo com as categorias, se não pergunta se quer cria-lo)
	int rInicializaDB = fInicializaDB(&db);
	if (rInicializaDB)
		return;

	//preenche uma sLista com todas as categorias
	fPreencheListaCat(&db);

	//Pede o nome da categoria
	printf("Categoria:\n");
	scanf(" %[^\n]", s.categoria);

	//Se a categoria é a raiz guarda no arquivo raiz
	if (strcmp(s.categoria, "/") == 0) {
		strcpy(s.categoria, "luof");
		db.listaSites = db.raiz;
		db.aCat = NULL;
	}
	else {
		//verifica se a categoria existe
		char *rBuscaCat = fBuscaCat(&db, s, &categoria);
		if (rBuscaCat != NULL) {
			printf("Categoria \"%s\" não encontrada.\n", rBuscaCat);
			return;
		}
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
		if (siteDoIterador->ehCat == '1') {
			printf("* %s\n", siteDoIterador->nome);
		}
		else if (opcao == 0) {
			printf("%s\n", siteDoIterador->nome);
			printf("\t%s\n", siteDoIterador->link);
			printf("\t%s\n", siteDoIterador->texto);
		}
		else {
			printf("%s\n", siteDoIterador->nome);
		}
		iteraProximo(&it);
	} while(!inicioIt(&it));

	//fecha os arquivos abertos
	fFinalizaDB(&db);
	
}
