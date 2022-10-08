#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "luof.h"
#include "pilha.h"

void fModifySite() {
	
	//variaveis
	sSite s;
	sCat categoria;
	sBanco db;
	int rBuscaFavorito;

	// inicializa o banco de dados (se existir guarda em aLuof o arquivo com as categorias, se não pergunta se quer cria-lo)
	if (fInicializaDB(&db))
		return;

	//preenche uma sLista com todas as categorias
	fPreencheListaCat(&db);
	
	//preenche uma sLista com todos os favoritos da raiz
	fPreencheRaiz(&db);

	//Pede o nome e a categoria
	printf("Categoria:\n");
	scanf(" %[^\n]", s.categoria);
	printf("Nome:\n");
	scanf(" %[^\n]", s.nome);

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
	
	//verifica se já existe um favorito com esse nome na categoria e se o arquivo pode ser aberto
	rBuscaFavorito = fBuscaFavorito(&db, &s, '0');
	if (!rBuscaFavorito) {
		printf("O favorito não existe para ser modificado.\n");
		return;
	}
	
	//printa os dados do site para o usuário saber o que modificar
	printf("\nDados antigos:\n");
	printf("Categoria: %s\n", s.categoria);
	printf("Nome: %s\n", s.nome);
	printf("Link: %s\n", s.link);
	printf("Texto: %s\n", s.texto);
	
	//remove do banco de dados
	fRemoveFavorito(&db, s, categoria);

	//fecha os arquivos abertos e reabre eles
	fFinalizaDB(&db);
	if (fInicializaDB(&db))
		return;
	fPreencheListaCat(&db);
	fPreencheRaiz(&db);
	
	//Pede os novos dados do site - identico a fAddSite()
	printf("\nNovos dados:\n");
	printf("Categoria:\n");
	scanf(" %[^\n]", s.categoria);
	printf("Nome:\n");
	scanf(" %[^\n]", s.nome);
	if (strcmp(s.categoria, "/") == 0) {
		strcpy(s.categoria, "luof");
		db.listaSites = db.raiz;
		db.aCat = NULL;
	}
	else {
		if (fBuscaCat(&db, s, &categoria))
			return;
		if (fPreencheListaSite(&db, &categoria))
			return;
	}
	rBuscaFavorito = fBuscaFavorito(&db, &s, '0');
	if (rBuscaFavorito) {
		printf("O favorito já existe.\n");
		return;
	}
	printf("Link:\n");
	scanf(" %[^\n]", s.link);
	printf("Texto:\n");
	scanf(" %[^\n]", s.texto);
	fAdicionaFavorito(&db, s, categoria);

	//fecha os arquivos abertos
	fFinalizaDB(&db);
	
	printf("\nFavorito modificado com sucesso.\n");

}
