#include "luof.h"

void fModifySite() {
	
	sSite s;
	sCat *categoria = malloc(sizeof(sCat));
	sBanco db;

	if (fInicializaDB(&db))
		return;

	printf("Categoria:\n");
	scanf(" %[^\n]", s.categoria);

	if (strcmp(s.categoria, "/") == 0) {
		if (emptyList(db.raiz)) {
			printf("\nCategoria vazia\n");
			return;
		}
		strcpy(s.categoria, "luof");
		db.listaSites = db.raiz;
	}
	else {
		if (fBuscaCat(&db, s, &categoria))
			return;
		if (fPreencheListaSite(&db, categoria))
			return;
	}
	
	printf("Nome:\n");
	scanf(" %[^\n]", s.nome);
	s.ehCat = '0';
	
	if (!fBuscaFavorito(&db, &s)) {
		printf("\nO favorito não existe para ser modificado.\n");
		return;
	}
	
	//printa os dados do site para o usuário saber o que modificar
	printf("\nDados antigos:\n");
	printf("Categoria: %s\n", s.categoria);
	printf("Nome: %s\n", s.nome);
	printf("Link: %s\n", s.link);
	printf("Texto: %s\n", s.texto);
	
	//remove do banco de dados
	fRemoveFavorito(&db, s, *categoria);

	//fecha os arquivos abertos e reabre eles
	fFinalizaDB(&db);
	if (fInicializaDB(&db))
		return;
	
	//Pede os novos dados do site
	printf("\nNovos dados:\n");
	printf("Categoria: ");
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
	printf("Nome: ");
	scanf(" %[^\n]", s.nome);
	if (fBuscaFavorito(&db, &s)) {
		printf("\nO favorito já existe.\n");
		return;
	}
	printf("Link: ");
	scanf(" %[^\n]", s.link);
	printf("Texto: ");
	scanf(" %[^\n]", s.texto);
	fAdicionaFavorito(&db, s, *categoria);

	fFinalizaDB(&db);
	
	printf("\nFavorito modificado com sucesso.\n");

}
