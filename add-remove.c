#include "luof.h"

void fAddBookmark() {

	//variaveis
	sSite s;
	sCat *categoria;
	sBanco db;

	// inicializa o banco de dados (se existir guarda em aLuof o arquivo com as categorias, se não pergunta se quer cria-lo)
	if (fInicializaDB(&db))
		return;

	//prenche o campo ehCat
	s.ehCat = '0';

	//Pede a categoria
	if (fSetaSiteCategoria(&s))
		return;

	//Se a categoria é a raiz guarda no arquivo raiz
	if (strcmp(s.categoria, "/") == 0) {
		strcpy(s.categoria, "luof");
		db.listaSites = db.raiz;

		//categoria passa a apontar para a categoria raiz
		categoria = db.listaCategorias;
	}
	else {
		//verifica se a categoria existe e guarda em categoria sua posição na árvore
		if (fBuscaCat(&db, s, &categoria)) {
			fFinalizaDB(&db);
			return;
		}
		//cria uma lista de favoritos pertecentes à categoria
		if (fPreencheListaSite(&db, categoria)) {
			fFinalizaDB(&db);
			return;
		}
	}

	//pede nome
	if (fSetaSiteNome(&s))
		return;

	//verifica se já existe um favorito com esse nome na categoria e se o arquivo pode ser aberto
	if (fBuscaFavorito(&db, &s)) {
		printf("\nO favorito já existe.\n");
		fFinalizaDB(&db);
		return;
	}

	//Pede o link
	if (fSetaSiteLink(&s))
		return;

	//Pede o texto
	if (fSetaSiteTexto(&s))
		return;

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

	//pede a categoria pai
	if (fSetaCatCategoria(&c))
		return;

	if (strcmp(c.categoria, "/") == 0) {
		strcpy(c.categoria, "luof");
		db.listaSites = db.raiz;
		categoria = db.listaCategorias;
	}
	else {
		if (fBuscaCat(&db, c, &categoria)) {
			fFinalizaDB(&db);
			return;
		}
		if (fPreencheListaSite(&db, categoria)) {
			fFinalizaDB(&db);
			return;
		}
	}

	//pede o nome (da categoria)
	if (fSetaCatNome(&c))
		return;

	//preenche os outros campos
	strcpy(c.link, "@categoria@");
	strcpy(c.texto, "@categoria@");
	c.ehCat = '1';

	if (fBuscaFavorito(&db, &c)) {
		printf("\nA categoria já existe.\n");
		fFinalizaDB(&db);
		return;
	}

	//adiciona na árvore de categorias
	fAdicionaCatLuof(&db, c, categoria);

	fAdicionaFavorito(&db, c, categoria);
	fFinalizaDB(&db);

	printf("\nCategoria adicionada com sucesso.\n");

}

void fRemoveBookmark() {

	sSite s;
	sCat *categoria;
	sBanco db;

	if (fInicializaDB(&db))
		return;

	if (fSetaSiteCategoria(&s))
		return;
	
	if (strcmp(s.categoria, "/") == 0) {
		//se a lista está vazia não continua a função
		if (emptyList(db.raiz)) {
			printf("\nCategoria vazia.\n");
			fFinalizaDB(&db);
			return;
		}

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

		//se a lista está vazia não continua a função
		if (emptyList(db.listaSites)) {
			printf("\nCategoria vazia.\n");
			fFinalizaDB(&db);
			return;
		}
	}

	if (fSetaSiteNome(&s))
		return;
	s.ehCat = '0';

	if (!fBuscaFavorito(&db, &s)) {
		printf("\nO favorito não existe para ser excluido.\n");
		fFinalizaDB(&db);
		return;
	}

	//remove do banco de dados
	fRemoveFavorito(&db, s, categoria);

	fFinalizaDB(&db);

	printf("\nFavorito removido com sucesso.\n");

}

void fRemoveCategory() {

	sSite c;
	sCat *categoria;
	sBanco db;

	if (fInicializaDB(&db))
		return;

	if (fSetaCatCategoria(&c))
		return;

	if (strcmp(c.categoria, "/") == 0) {
		if (emptyList(db.raiz)) {
			printf("\nCategoria vazia.\n");
			fFinalizaDB(&db);
			return;
		}
		strcpy(c.categoria, "luof");
		db.listaSites = db.raiz;
		categoria = db.listaCategorias;
	}
	else {
		if (fBuscaCat(&db, c, &categoria)) {
			fFinalizaDB(&db);
			return;
		}
		if (fPreencheListaSite(&db, categoria)) {
			fFinalizaDB(&db);
			return;
		}
		if (emptyList(db.listaSites)) {
			printf("\nCategoria vazia.\n");
			fFinalizaDB(&db);
			return;
		}
	}

	if (fSetaCatNome(&c))
		return;
	c.ehCat = '1';
	
	if (!fBuscaFavorito(&db, &c)) {
		printf("\nA categoria não existe para ser excluida.\n");
		fFinalizaDB(&db);
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
