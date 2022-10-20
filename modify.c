#include "luof.h"

void fModifySite() {
	
	sSite s, sNew, *fav;
	sCat *categoria = malloc(sizeof(sCat));
	sCat *categoria2 = malloc(sizeof(sCat));
	sBanco db;
	sLista listaTemp1, listaTemp2;
	sIterador it;
	int opcao, encontrou;

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
	if (strcmp(s.categoria, "luof") == 0)
		printf("Categoria: /\n");
	else
		printf("Categoria: %s\n", s.categoria);
	printf("Nome: %s\n", s.nome);
	printf("Link: %s\n", s.link);
	printf("Texto: %s\n", s.texto);
	
	//pergunta qual atributo será modificado
	printf("\nVocê deseja modificar? [1]categoria [2]nome [3]link [4]texto [5]tudo [6]nada\n");
	scanf(" %d", &opcao);
	
	//se os usuário não desejar modificar nada
	if (opcao < 1 || opcao > 5) {
		printf("Saindo...\n");
		return;
	}
	
	//seta alguns valores antes de receber novos
	listaTemp1 = db.listaSites;
	if (db.listaSites != db.raiz)
		db.raiz = NULL;
	db.listaSites = NULL;
	strcpy(sNew.categoria, s.categoria);
	strcpy(sNew.nome, s.nome);
	strcpy(sNew.link, s.link);
	strcpy(sNew.texto, s.texto);
	sNew.ehCat = s.ehCat;

	//fecha os arquivos abertos e reabre eles
	fFinalizaDB(&db);
	if (fInicializaDB(&db))
		return;
		
	db.listaSites = listaTemp1;
	if (strcmp(s.categoria, "luof") != 0) {
		fBuscaCat(&db, s, &categoria);
		fBuscaCat(&db, s, &categoria2);
	}
	
	//Pede os novos dados do site
	printf("\nNovos dados:\n");
	if (opcao == 1 || opcao == 5) {
		printf("Categoria: ");
		scanf(" %[^\n]", sNew.categoria);
		if (strcmp(sNew.categoria, "/") == 0) {
			strcpy(sNew.categoria, "luof");
			db.listaSites = db.raiz;
		}
		else {
			if (fBuscaCat(&db, sNew, &categoria2))
				return;
			if (fPreencheListaSite(&db, categoria2))
				return;
		}
	}	
	if (opcao == 2 || opcao == 5) {
		printf("Nome: ");
		scanf(" %[^\n]", sNew.nome);
	}
	
	if (strcmp(s.categoria, sNew.categoria) || strcmp(s.nome, sNew.nome)) {
		if (fBuscaFavorito(&db, &sNew)) {
			printf("\nJá existe outro favorito com esse nome e categoria, logo a modificação não pode ser concluída.\n");
			return;
		}
	}
	
	if (opcao == 3 || opcao == 5) {
		printf("Link: ");
		scanf(" %[^\n]", sNew.link);
	}
	if (opcao == 4 || opcao == 5) {
		printf("Texto: ");
		scanf(" %[^\n]", sNew.texto);
	}
	
	//muda apenas o nome no arquivo (se as categorias forem iguais)
	if (strcmp(s.categoria, sNew.categoria) == 0) {

		if (strcmp(sNew.categoria, "luof") == 0)
			db.listaSites = db.raiz;

		encontrou = 0;
		it = criaIt(db.listaSites);

		do {
			fav = retornaItera(&it);
			//encontrou o favorito
			if ((strcmp(fav->nome, s.nome) == 0) && (strcmp(fav->categoria, s.categoria) == 0) && (fav->ehCat == s.ehCat)) {
				//atualiza todos os valores
				strcpy(fav->nome, sNew.nome);
				strcpy(fav->link, sNew.link);
				strcpy(fav->texto, sNew.texto);
				encontrou = 1;
			}
			iteraProximo(&it);
		} while (encontrou == 0 && !inicioIt(&it));
		
		if (strcmp(sNew.categoria, "luof") == 0)
			fEscreveLuof(&db);
		else
			fEscreveArquivoCat(&db, categoria2->nome);
		
	}
	else {//as categorias são diferentes

		//remove o favorito da categoria antiga
		listaTemp2 = db.listaSites;
		if (strcmp(s.categoria, "luof") == 0)
			db.listaSites = db.raiz;
		else
			db.listaSites = listaTemp1;

		encontrou = 0;
		it = criaIt(db.listaSites);
		do {
			fav = retornaItera(&it);
			if ((strcmp(fav->nome, s.nome) == 0) && (strcmp(fav->categoria, s.categoria) == 0) && (fav->ehCat == s.ehCat)) {
				removeIt(&it);
				encontrou = 1;
			}
			iteraProximo(&it);
		} while (encontrou == 0 && !inicioIt(&it));
		
		//escreve no arquivo
		if (strcmp(s.categoria, "luof") == 0)
			fEscreveLuof(&db);
		else
			fEscreveArquivoCat(&db, categoria->nome);

		//adiciona o favorito na categoria nova
		db.listaSites = listaTemp2;
		encontrou = 0;
		it = criaIt(db.listaSites);
		iteraFim(&it);
		if (emptyList(db.listaSites)) {
			pushBackList(db.listaSites, &sNew);
		}
		else {
			do {
				fav = retornaItera(&it);
				if (strcmp(fav->nome, sNew.nome) < 0 || fav->ehCat == '1')
					encontrou = 1;
				else
					iteraAnterior(&it);
			} while (encontrou == 0 && !fimIt(&it));
			fav = frontList(db.listaSites);
			if (fav->ehCat == '0' && encontrou == 0)
				pushFrontList(db.listaSites, &sNew);
			else
				insereProxIt(&it, &sNew);
		}
		//escreve no arquivo
		if (strcmp(sNew.categoria, "luof") == 0)
			fEscreveLuof(&db);
		else
			fEscreveArquivoCat(&db, categoria2->nome);
	}

	//freeList(listaTemp1);
	fFinalizaDB(&db);
	
	printf("\nFavorito modificado com sucesso.\n");

}

void fModifyCategory() {

	sSite c;
	sCat *categoria = malloc(sizeof(sCat));
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
		strcpy(categoria->nome, "luof");
		categoria->catFilhos = db.listaCategorias;
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
		printf("\nA categoria não existe para ser modificada\n");
		return;
	}
	
	printf("\nDados antigos:\n");
	if (strcmp(c.categoria, "luof") == 0)
		printf("Categoria pai: /\n");
	else
		printf("Categoria pai: %s\n", c.categoria);
	printf("Categoria: %s\n", c.nome);

	printf("\nNovos dados:\n");
	printf("Categoria pai: ");
	scanf(" %[^\n]", c.categoria);
	if (strcmp(c.categoria, "/") == 0) {
		strcpy(c.categoria, "luof");
		db.listaSites = db.raiz;
	}
	else {
		if (fBuscaCat(&db, c, &categoria))
			return;
		if (fPreencheListaSite(&db, categoria))
			return;
	}
	printf("Categoria: ");
	scanf(" %[^\n]", c.nome);
	if (fBuscaFavorito(&db, &c)) {
		printf("\nO favorito já existe.\n");
		return;
	}

}
