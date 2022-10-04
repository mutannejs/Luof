#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "luof.h"

void fAddSite() {

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

	//prenche o campo ehCat
	s.ehCat = '0';

	//Pede o nome e a categoria
	printf("Nome:\n");
	scanf(" %[^\n]", s.nome);

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
	
	//verifica se já existe um favorito com esse nome na categoria e se o arquivo pode ser aberto
	int rBuscaFavorito = fBuscaFavorito(&db, &s, '0');
	if (rBuscaFavorito) {
		printf("O favorito já existe.\n");
		return;
	}

	//Pede o link e comentario
	printf("Link:\n");
	scanf(" %[^\n]", s.link);

	printf("Texto:\n");
	scanf(" %[^\n]", s.texto);

	//adiciona no banco de dados
	fAdicionaFavorito(&db, s, categoria);

	//fecha os arquivos abertos
	fFinalizaDB(&db);
	
	printf("Favorito adicionado com sucesso.\n");
	
}

void fAddCategory() {

	//variaveis
	sSite c;//será usado para guardar a categoria (como será armazenada)
	sCat categoria;
	sBanco db;

	// inicializa o banco de dados (se existir guarda em aLuof o arquivo com as categorias, se não pergunta se quer cria-lo)
	int rInicializaDB = fInicializaDB(&db);
	if (rInicializaDB)
		return;

	//preenche uma sLista com todas as categorias
	fPreencheListaCat(&db);

	//Pede a categoria
	printf("Categoria pai:\n");
	scanf(" %[^\n]", c.categoria);

	//Se a categoria é a raiz guarda no arquivo raiz
	if (strcmp(c.categoria, "/") == 0) {
		strcpy(c.categoria, "luof");
		db.listaSites = db.raiz;
		db.aCat = NULL;
		
		strcpy(categoria.nome, "luof");
		categoria.catFilhos = db.listaCategorias;
	}
	else {
		//verifica se a categoria pai existe
		char *rBuscaCat = fBuscaCat(&db, c, &categoria);
		if (rBuscaCat != NULL) {
			printf("Categoria \"%s\" não encontrada.\n", rBuscaCat);
			return;
		}
	}

	printf("Categoria:\n");
	scanf(" %[^\n]", c.nome);

	//preenche os outros campos
	strcpy(c.link, "@categoria@");
	strcpy(c.texto, "@categoria@");
	c.ehCat = '1';
	
	//verifica se já existe um favorito com esse nome na categoria e se o arquivo pode ser aberto
	int rBuscaFavorito = fBuscaFavorito(&db, &c, '1');
	if (rBuscaFavorito) {
		printf("A categoria já existe.\n");
		return;
	}
	
	printf("\nNome: %s\n", c.nome);
	printf("Categoria: %s\n", c.categoria);
	printf("Link: %s\n", c.link);
	printf("Texto: %s\n", c.texto);
	printf("ehCat: %c\n\n", c.ehCat);
	printf("cat: %s\n", categoria.nome);
	printf("a lista estah Vazia?: %d\n", emptyList(categoria.catFilhos));

	//adiciona no banco de dados
	fAdicionaCatLuof(&db, c, categoria);
	
	printf("......\n");
	
	fAdicionaFavorito(&db, c, categoria);
	
	printf("......\n");

	//fecha os arquivos abertos
	fFinalizaDB(&db);
	
	printf("Categoria adicionada com sucesso.\n");

}

void fDeleteSite() {
	
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

	//Pede o nome e a categoria
	printf("Nome:\n");
	scanf(" %[^\n]", s.nome);

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
	
	//verifica se já existe um favorito com esse nome na categoria e se o arquivo pode ser aberto
	int rBuscaFavorito = fBuscaFavorito(&db, &s, '0');
	if (!rBuscaFavorito) {
		printf("O favorito não existe para ser excluido.\n");
		return;
	}
	
	//remove do banco de dados
	fRemoveFavorito(&db, s, categoria);

	//fecha os arquivos abertos
	fFinalizaDB(&db);
	
	printf("Favorito removido com sucesso.\n");
	
}
