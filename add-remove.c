#include "luof.h"

int fAnalisaCaminhoSite(sSite *s, sCom com) {
	if (com.caminho[0] != '\0') {//se o caminho foi passado seta a categoria e o favorito
		for (int i = strlen(com.caminho); i > -1; i--) {
			if (com.caminho[i] == '/') {
				strncpy(s->categoria, com.caminho, i);
				strcpy(s->nome, &com.caminho[i+1]);
				i = -1;
			}
			else if (i == 0) {
				strcpy(s->categoria, "/");
				strcpy(s->nome, com.caminho);
			}
		}
		if (strlen(s->nome) >= TAMNOMEFAV) {
			printf(ERRO);
			printf("Nomes de favoritos devem ter no máximo %d caracteres.\n", TAMNOMEFAV - 1);
			return 1;
		}
	}
	return 0;
}

int fAnalisaCaminhoCat(sCat *cat, sCom com) {
	if (com.caminho[0] != '\0') {//se o caminho foi passado seta a categoria pai e a nova categoria
		for (int i = strlen(com.caminho); i > -1; i--) {
			if (com.caminho[i] == '/') {
				strncpy(cat->caminho, com.caminho, i);
				strcpy(cat->nome, &com.caminho[i+1]);
				i = -1;
			}
			else if (i == 0) {
				strcpy(cat->caminho, "/");
				strcpy(cat->nome, com.caminho);
			}
		}
		if (strlen(cat->nome) >= TAMNOMEFAV) {
			printf(ERRO);
			printf("Categorias devem ter no máximo %d caracteres.\n", TAMNOMEFAV - 1);
			return 1;
		}
	}
	return 0;
}

void fAddBookmark(sCom com) {

	//variaveis
	sSite s;
	sCat *cat;
	sBanco db;

	//se o caminho foi passado seta a categoria e o favorito
	if (fAnalisaCaminhoSite(&s, com))
		return;

	// inicializa o banco de dados (se existir guarda em aLuof o arquivo com as categorias, se não pergunta se quer cria-lo)
	if (fInicializaDB(&db))
		return;

	//Pede a categoria (se não foi passado)
	if (com.caminho[0] == '\0' && fSetaSiteCategoria(&s)) {
		fFinalizaDB(&db);
		return;
	}

	//verifica se a categoria existe e guarda em categoria sua posição na árvore
	if (fBuscaCat(&db, s.categoria, &cat)) {
		fFinalizaDB(&db);
		return;
	}
	//cria uma lista de favoritos pertecentes à categoria
	fPreencheListaSite(&db, cat, 0);

	//pede nome (se não foi passado)
	if (com.caminho[0] == '\0' && fSetaSiteNome(&s)) {
		fFinalizaDB(&db);
		return;
	}

	//verifica se já existe um favorito com esse nome na categoria
	if (fBuscaFavorito(&db, &s)) {
		printf(ERRO);
		printf("O favorito já existe.\n");
		fFinalizaDB(&db);
		return;
	}

	//Pede o link
	if (fSetaSiteLink(&s)) {
		fFinalizaDB(&db);
		return;
	}

	//Pede o texto
	if (fSetaSiteTexto(&s)) {
		fFinalizaDB(&db);
		return;
	}

	//adiciona o favorito na db->listaFavs
	fInsereFavorito(&db, s);
	//atualiza o arquivo
	fEscreveArquivoCat(&db, cat->nome);

	//fecha os arquivos abertos
	fFinalizaDB(&db);

	printf(ANSI_BOLD_WHT "\nFavorito adicionado com sucesso.\n");

}

void fAddCategory(sCom com) {

	sCat *catPai, cat;
	sBanco db;

	//se o caminho foi passado seta a categoria pai e a nova categoria
	if (fAnalisaCaminhoCat(&cat, com))
		return;

	if (fInicializaDB(&db)) {
		fFinalizaDB(&db);
		return;
	}

	//pede o caminho da categoria pai (se não foi passado)
	if (com.caminho[0] == '\0' && fSetaCatCategoria(&cat)) {
		fFinalizaDB(&db);
		return;
	}

	if (fBuscaCat(&db, cat.caminho, &catPai)) {
		fFinalizaDB(&db);
		return;
	}

	//pede o nome da categoria (se não foi passado)
	if (com.caminho[0] == '\0' && fSetaCatNome(&cat)) {
		fFinalizaDB(&db);
		return;
	}

	//insere na árvore de categorias
	if (fInsereCategoria(catPai, cat)) {
		printf(ERRO);
		printf("A categoria já existe.\n");
		fFinalizaDB(&db);
		return;
	}
	//atualiza o arquivo luof
	fEscreveLuof(&db);

	fFinalizaDB(&db);

	printf(ANSI_BOLD_WHT "\nCategoria adicionada com sucesso.\n");

}

void fRemoveBookmark(sCom com) {

	sSite s;
	sCat *cat;
	sBanco db;

	if (fAnalisaCaminhoSite(&s, com))
		return;

	if (fInicializaDB(&db)) {
		fFinalizaDB(&db);
		return;
	}

	if (com.caminho[0] == '\0' && fSetaSiteCategoria(&s)) {
		fFinalizaDB(&db);
		return;
	}

	if (fBuscaCat(&db, s.categoria, &cat)) {
		fFinalizaDB(&db);
		return;
	}
	fPreencheListaSite(&db, cat, 0);

	if (com.caminho[0] == '\0' && fSetaSiteNome(&s)) {
		fFinalizaDB(&db);
		return;
	}

	if (!fBuscaFavorito(&db, &s)) {
		printf(ERRO);
		printf("O favorito não existe para ser excluido.\n");
		fFinalizaDB(&db);
		return;
	}

	//remove de db.listaFavs
	fRemoveFavorito(&db, s);
	fEscreveArquivoCat(&db, cat->nome);

	fFinalizaDB(&db);

	printf(ANSI_BOLD_WHT "\nFavorito removido com sucesso.\n");

}

void fRemoveCategory(sCom com) {

	sCat *cat, c;
	sBanco db;

	if (fAnalisaCaminhoCat(&c, com))
		return;

	if (fInicializaDB(&db))
		return;

	if (com.caminho[0] == '\0' && fSetaCatCategoria(&c)) {
		fFinalizaDB(&db);
		return;
	}

	if (fBuscaCat(&db, c.caminho, &cat)) {
		fFinalizaDB(&db);
		return;
	}

	if (com.caminho[0] == '\0' && fSetaCatNome(&c)) {
		fFinalizaDB(&db);
		return;
	}

	cat = fBuscaCatFilha(cat, c.nome);
	if (!cat) {
		printf(ERRO);
		printf("A categoria não existe para ser excluida.\n");
		fFinalizaDB(&db);
		return;
	}

	//remove todos os favoritos do arquivo da categoria, com exceção daqueles pertencentes à outra categoria
	fRemoveArqCat(&db, cat);
	//remove a categoria da árvore de categorias
	fRemoveCategoria(cat);
	fEscreveLuof(&db);

	fFinalizaDB(&db);

	printf(ANSI_BOLD_WHT "\nCategoria removida com sucesso.\n");

}
