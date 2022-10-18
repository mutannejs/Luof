#include "luof.h"

void fEscreveLuof_private(sBanco *db, sLista listaCategorias, int hierarquia) {
	
	if (!emptyList(listaCategorias)) {
		sIterador it = criaIt(listaCategorias);
		do {
			sCat *cat = (struct sCat*) retornaItera(&it);
			//escreve no arquivo
			fprintf(db->aLuof, "%d%s\n", hierarquia, cat->nome);
			//------------------
			fEscreveLuof_private(db, cat->catFilhos, hierarquia+1);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}
	
}

void fEscreveLuof(sBanco *db) {
	
	char nomeArqCat[TAMLINKARQ];
	sSite *siteDoIterador;
	sIterador it;
	
	//reabre o arquivo aLuof para sobreescreve-lo
	fSetaCaminhoArquivo(nomeArqCat, "luof");
	if (db->aLuof)
		db->aLuof = freopen(nomeArqCat, "w", db->aLuof);
	else
		db->aLuof = fopen(nomeArqCat, "w");

	//escreve a árvore de categorias no arquivo aLuof
	fEscreveLuof_private(db, db->listaCategorias, 0);
	
	//marca o fim da árvore de categorias
	fprintf(db->aLuof, "##\n");
	
	//escreve os favoritos da raiz
	it = criaIt(db->raiz);
	do {
		siteDoIterador = retornaItera(&it);
		fprintf(db->aLuof, "%s\n%s\n%s\n%s\n%c\n", siteDoIterador->nome, siteDoIterador->categoria, siteDoIterador->link, siteDoIterador->texto, siteDoIterador->ehCat);
		iteraProximo(&it);
	} while (!inicioIt(&it));
	
}

void fMudaNomeCategoriaArvore(sBanco *db, char *caminho, char *nome) {
	sCat *categoria;
	sSite s;
	strcpy(s.categoria, caminho);
	s.ehCat = '1';
	fBuscaCat(db, s, &categoria);
	strcpy(categoria->nome, nome);
}

void fMudaCaminhoCategoriaArvore_private(sBanco *db, sCat *cat, char *caminhoA, char *caminhoN) {
	
	sIterador it;
	sSite *siteDoIterador;
	sCat *catTemp;
	char caminhoAntigo[TAMCAMINHO], caminhoNovo[TAMCAMINHO];
	
	//preenche a lista dos favoritos da categoria
	fPreencheListaSite(db, cat);
	
	if (emptyList(db->listaSites))
		return;
	
	//corrige a categoria dos favoritos da categoria
	it = criaIt(db->listaSites);
	do {
		siteDoIterador = (struct sSite*) retornaItera(&it);
		if (strcmp(siteDoIterador->categoria, caminhoA) == 0) {
			strcpy(siteDoIterador->categoria, caminhoN);
		}
	} while (!inicioIt(&it));
	
	//reescreve seu arquivo
	fEscreveArquivoCat(db, cat->nome);
	
	if (emptyList(cat->catFilhos))
		return;

	//chama a mesma função para suas categorias
	it = criaIt(cat->catFilhos);
	do {
		catTemp = (struct sCat*) retornaItera(&it);
		
		strcpy(caminhoAntigo, caminhoA);
		fIncrementaCamCat(caminhoAntigo, catTemp->nome);
		strcpy(caminhoNovo, caminhoN);
		fIncrementaCamCat(caminhoNovo, catTemp->nome);
		fMudaCaminhoCategoriaArvore_private(db, catTemp, caminhoAntigo, caminhoNovo);
		
		iteraProximo(&it);
	} while (!inicioIt(&it));
	
}

void fMudaCaminhoCategoriaArvore(sBanco *db, char *caminho1, char *caminho2) {
	
	int encontrou = 0;
	char caminhoCatAtual[TAMCAMINHO];
	sIterador it;
	sCat *categoria, *catTemp, *categoriaPai;

	sSite s;
	strcpy(s.categoria, caminho1);
	s.ehCat = '1';
	
	fBuscaCat(db, s, &categoria);
	
	if (strcmp(caminho2, "luof")) {
		//adiciona a categoria no lugar correto
		pushBackList(db->listaCategorias, categoria);
		
		strcpy(caminhoCatAtual, categoria->nome);
	}
	else {
		
		strcpy(s.categoria, caminho2);
		
		fBuscaCat(db, s, &categoriaPai);
		//adiciona a categoria no lugar correto
		pushBackList(categoriaPai->catFilhos, categoria);
		
		strcpy(caminhoCatAtual, caminho2);
		fIncrementaCamCat(caminhoCatAtual, categoria->nome);
	}
	
	//remove a categoria do lugar antigo
	if (categoria->catPai)
		it = criaIt(categoria->catPai->catFilhos);
	else
		it = criaIt(db->listaCategorias);

	do {
		catTemp = (struct sCat*) retornaItera(&it);
		if (strcmp(catTemp->nome, categoria->nome) == 0) {
			encontrou = 1;
			removeIt(&it);
		}
		else {
			iteraProximo(&it);
		}
	} while (encontrou == 0 && !inicioIt(&it));
	
	//corrige os favoritos da categoria modificada
	fMudaCaminhoCategoriaArvore_private(db, categoria, caminho1, caminhoCatAtual);
	
}

void fEscreveArquivoCat(sBanco *db, char *nomeArq) {
	
	char nomeArqCat[TAMLINKARQ];
	sSite *siteDoIterador;
	sIterador it;

	//reabre o arquivo para sobreescreve-lo
	fSetaCaminhoArquivo(nomeArqCat, nomeArq);
	if (db->aCat)
		db->aCat = freopen(nomeArqCat, "w", db->aCat);
	else
		db->aCat = fopen(nomeArqCat, "w");

	if (!emptyList(db->listaSites)) {
		//escreve a lista no arquivo da categoria
		it = criaIt(db->listaSites);
		do {
			siteDoIterador = retornaItera(&it);
			fprintf(db->aCat, "%s\n%s\n%s\n%s\n%c\n", siteDoIterador->nome, siteDoIterador->categoria, siteDoIterador->link, siteDoIterador->texto, siteDoIterador->ehCat);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}
	
}

void fSetaCaminhoArquivo(char *arq, char *nome) {
	strcpy(arq, caminhoDB);
	strcpy(&arq[tamCaminhoDB], nome);
}

void fIncrementaCamCat(char *caminho, char *nome) {
	strcat(caminho, "/");
	strcat(caminho, nome);	
}
