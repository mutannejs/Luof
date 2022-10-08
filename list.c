#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "luof.h"
#include "pilha.h"

void fListCategory(int opcao) {

	//variaveis
	sSite s;
	sCat categoria;
	sBanco db;

	// inicializa o banco de dados (se existir guarda em aLuof o arquivo com as categorias, se não pergunta se quer cria-lo)
	if (fInicializaDB(&db))
		return;

	//preenche uma sLista com todas as categorias
	fPreencheListaCat(&db);
	
	//preenche uma sLista com todos os favoritos da raiz
	fPreencheRaiz(&db);

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
		//verifica se a categoria existe e guarda em categoria sua posição na árvore
		if (fBuscaCat(&db, s, &categoria))
			return;
		//cria uma lista de favoritos pertecentes à categoria
		if (fPreencheListaSite(&db, &categoria))
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

void fListTree() {
	
	sBanco db;
	sIterador it;
	sSite *siteDoIterador;
	char hierarquia[10] = "1111111111";

	// inicializa o banco de dados (se existir guarda em aLuof o arquivo com as categorias, se não pergunta se quer cria-lo)
	if (fInicializaDB(&db))
		return;

	//preenche uma sLista com todas as categorias
	fPreencheListaCat(&db);
	
	//preenche uma sLista com todos os favoritos da raiz
	fPreencheRaiz(&db);
	
	//printa o nome (por extenso) de todas categorias
	if (!emptyList(db.listaCategorias)) {
		//FUNCAO QUE ORDENA LISTA - FAZER
		it = criaIt(db.listaCategorias);
		do {
			sCat *cat = (struct sCat*) retornaItera(&it);
			fNomesCats(&db, cat, hierarquia, 0);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}
	
	//printa os sites da raiz
	if (!emptyList(db.raiz)) {
		it = criaIt(db.raiz);
		do {
			siteDoIterador = retornaItera(&it);
			if (siteDoIterador->ehCat == '0')
				printf("%s\n", siteDoIterador->nome);
			iteraProximo(&it);
		} while(!inicioIt(&it));
	}
	else
		printf("Vazia.\n");

	//fecha os arquivos abertos
	fFinalizaDB(&db);
}
