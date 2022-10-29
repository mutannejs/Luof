#include "luof.h"

void fAddSite() {

	//variaveis
	sSite s;
	sCat *categoria;
	sBanco db;

	// inicializa o banco de dados (se existir guarda em aLuof o arquivo com as categorias, se não pergunta se quer cria-lo)
	if (fInicializaDB(&db))
		return;

	//prenche o campo ehCat
	s.ehCat = '0';

	//Pede o nome e a categoria
	printf("Categoria:\n");
	scanf(" %[^\n]", s.categoria);

	//Se a categoria é a raiz guarda no arquivo raiz
	if (strcmp(s.categoria, "/") == 0) {
		strcpy(s.categoria, "luof");
		db.listaSites = db.raiz;
	}
	else {
		//verifica se a categoria existe e guarda em categoria sua posição na árvore
		if (fBuscaCat(&db, s, &categoria))
			return;
		//cria uma lista de favoritos pertecentes à categoria
		if (fPreencheListaSite(&db, categoria))
			return;
	}

	printf("Nome:\n");
	scanf(" %[^\n]", s.nome);
	
	//verifica se já existe um favorito com esse nome na categoria e se o arquivo pode ser aberto
	if (fBuscaFavorito(&db, &s)) {
		printf("\nO favorito já existe.\n");
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
	
	printf("\nFavorito adicionado com sucesso.\n");
	
}

void fAddCategory() {

	sSite c;
	sCat *categoria;
	sBanco db;

	if (fInicializaDB(&db))
		return;

	printf("Categoria pai:\n");
	scanf(" %[^\n]", c.categoria);

	if (strcmp(c.categoria, "/") == 0) {
		strcpy(c.categoria, "luof");
		db.listaSites = db.raiz;
		
		//seta os dados da categoria pai
		categoria = db.listaCategorias;
	}
	else {
		if (fBuscaCat(&db, c, &categoria))
			return;
		if (fPreencheListaSite(&db, categoria))
			return;
	}

	printf("Categoria:\n");
	scanf(" %[^\n]", c.nome);

	//preenche os outros campos
	strcpy(c.link, "@categoria@");
	strcpy(c.texto, "@categoria@");
	c.ehCat = '1';
	
	if (fBuscaFavorito(&db, &c)) {
		printf("\nA categoria já existe.\n");
		return;
	}

	//adiciona na árvore de categorias
	fAdicionaCatLuof(&db, c, categoria);
	
	fAdicionaFavorito(&db, c, categoria);
	fFinalizaDB(&db);
	
	printf("\nCategoria adicionada com sucesso.\n");

}

void fDeleteSite() {

	sSite s;
	sCat *categoria;
	sBanco db;

	if (fInicializaDB(&db))
		return;

	printf("Categoria:\n");
	scanf(" %[^\n]", s.categoria);
	
	if (strcmp(s.categoria, "/") == 0) {
		//se a lista está vazia não continua a função
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
		printf("\nO favorito não existe para ser excluido.\n");
		return;
	}
	
	//remove do banco de dados
	fRemoveFavorito(&db, s, categoria);

	fFinalizaDB(&db);
	
	printf("\nFavorito removido com sucesso.\n");
	
}

void fDeleteCategory() {

	sSite c;
	sCat *categoria;
	sBanco db;

	if (fInicializaDB(&db))
		return;

	printf("Categoria pai:\n");
	scanf(" %[^\n]", c.categoria);

	if (strcmp(c.categoria, "/") == 0) {
		if (emptyList(db.raiz)) {
			printf("\nCategoria vazia\n");
			return;
		}
		strcpy(c.categoria, "luof");
		db.listaSites = db.raiz;
		categoria = db.listaCategorias;
	}
	else {
		if (fBuscaCat(&db, c, &categoria))
			return;
		if (fPreencheListaSite(&db, categoria))
			return;
	}

	printf("Categoria:\n");
	scanf(" %[^\n]", c.nome);
	c.ehCat = '1';
	
	if (!fBuscaFavorito(&db, &c)) {
		printf("\nA categoria não existe para ser excluida.\n");
		return;
	}
	
	fRemoveFavorito(&db, c, categoria);
	
	//remove todos os favoritos do arquivo da categoria, com exceção daqueles pertencentes à outra categoria
	fRemoveArqCat(&db, c);
	
	//remove da árvore de categorias
	fRemoveCatLuof(&db, c, categoria);

	fFinalizaDB(&db);
	
	printf("\nCategoria removida com sucesso.\n");
	
}
