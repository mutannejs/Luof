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
	printf("\nVocê deseja modificar? [1]categoria [2]nome [3]link [4]texto [5]tudo [6]nada : ");
	scanf(" %d", &opcao);
	
	//se os usuário não desejar modificar nada
	if (opcao < 1 || opcao > 5) {
		printf("Saindo...\n");
		return;
	}

	//listaTemp1 passa a ser a listaSites da categoriaPai inicial
	listaTemp1 = db.listaSites;
	//db.listaSites passa a ser igual a null para a listaTemp1 não ser liberada também
	db.listaSites = NULL;
	//guarda em sNew os dados antigos, para caso algum não seja alterado
	strcpy(sNew.categoria, s.categoria);
	strcpy(sNew.nome, s.nome);
	strcpy(sNew.link, s.link);
	strcpy(sNew.texto, s.texto);
	sNew.ehCat = s.ehCat;

	//fecha os arquivos abertos e reabre eles
	fFinalizaDB(&db);
	if (fInicializaDB(&db))
		return;

	//db.listaSites passa a ser igual a listaTemp1 para o caso da categoria não ser alterada(não ocorre a função fPreencheListaSites)
	db.listaSites = listaTemp1;
	//faz a busca da categoria novamente pois a árvore de categorias foi refeita
	if (strcmp(s.categoria, "luof") != 0)
		fBuscaCat(&db, s, &categoria);
	else
		categoria = db.listaCategorias;
	categoria2 = categoria;
	
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
	
	//se o nome ou a categoria for modificados é necessário ver se já não existe um favorito igual
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

		//se a categoria é luof db.listaSites passa a apontar para a raiz, pois essa lista será escrita no aLuof
		if (strcmp(sNew.categoria, "luof") == 0)
			db.listaSites = db.raiz;

		encontrou = 0;
		it = criaIt(db.listaSites);
		do {
			fav = retornaItera(&it);
			if ((strcmp(fav->nome, s.nome) == 0) && (strcmp(fav->categoria, s.categoria) == 0) && (fav->ehCat == s.ehCat)) {
				//atualiza todos os valores do favorito na lista
				strcpy(fav->nome, sNew.nome);
				strcpy(fav->link, sNew.link);
				strcpy(fav->texto, sNew.texto);
				encontrou = 1;
			}
			iteraProximo(&it);
		} while (encontrou == 0 && !inicioIt(&it));
		//escreve no arquivo
		if (strcmp(sNew.categoria, "luof") == 0)
			fEscreveLuof(&db);
		else
			fEscreveArquivoCat(&db, categoria2->nome);
		
	}
	else {//as categorias são diferentes

		listaTemp2 = db.listaSites;
		if (strcmp(s.categoria, "luof") == 0)
			db.listaSites = db.raiz;
		else
			db.listaSites = listaTemp1;

		//remove o favorito da categoria antiga
		fRemoveFavorito(&db, s, categoria);

		if (strcmp(s.categoria, "luof") == 0)
			db.listaSites = db.raiz;
		else
			db.listaSites = listaTemp2;

		//adiciona o favorito na categoria nova
		fAdicionaFavorito(&db, sNew, categoria2);
	}

	//freeList(listaTemp1);
	fFinalizaDB(&db);
	
	printf("\nFavorito modificado com sucesso.\n");

}

void fModifyCategory() {

	sSite c, cNew, *fav;
	sCat *categoria;
	sCat *categoria2;
	sCat *categoria3;
	sBanco db;
	sLista listaTemp1, listaTemp2;
	sIterador it;
	char vBooleana, catTemp[TAMLINKARQ];
	int opcao, encontrou, rBuscaFavorito = 0;

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
		categoria = malloc(sizeof(sCat));
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
	
	printf("\nVocê deseja modificar? [1]categoria pai [2]categoria [3]tudo [4]nada : ");
	scanf(" %d", &opcao);
	
	if (opcao < 1 || opcao > 3) {
		printf("Saindo...\n");
		return;
	}

	listaTemp1 = db.listaSites;
	db.listaSites = NULL;
	strcpy(cNew.categoria, c.categoria);
	strcpy(cNew.nome, c.nome);
	strcpy(cNew.link, c.link);
	strcpy(cNew.texto, c.texto);
	cNew.ehCat = c.ehCat;

	fFinalizaDB(&db);
	if (fInicializaDB(&db))
		return;

	db.listaSites = listaTemp1;
	if (strcmp(c.categoria, "luof") != 0)
		fBuscaCat(&db, c, &categoria);
	else
		categoria = db.listaCategorias;
	categoria2 = categoria;

	printf("\nNovos dados:\n");
	if (opcao == 1 || opcao == 3) {
		printf("Categoria pai: ");
		scanf(" %[^\n]", cNew.categoria);
		if (strcmp(cNew.categoria, "/") == 0) {
			strcpy(cNew.categoria, "luof");
			db.listaSites = db.raiz;
		}
		else {
			categoria2 = malloc(sizeof(sCat));
			if (fBuscaCat(&db, cNew, &categoria2))
				return;
			if (fPreencheListaSite(&db, categoria2))
				return;
		}
	}	
	if (opcao == 2 || opcao == 3) {
		printf("Categoria: ");
		scanf(" %[^\n]", cNew.nome);
	}
	
	if (strcmp(c.categoria, cNew.categoria) || strcmp(c.nome, cNew.nome)) {
		rBuscaFavorito = fBuscaFavorito(&db, &cNew);
		if (rBuscaFavorito) {
			//printf("\nJá existe outro favorito com esse nome e categoria, você deseja junta-los em um só? [s/n]: \n");
			//scanf(" %c", &vBooleana);
			//if (vBooleana != 's') {
			//	printf("Saindo...\n");
				return;
			//}
		}
	}

	if (strcmp(c.categoria, cNew.categoria) == 0) {//se só o nome será modificado

		//seta categoria3 como a posição na árvore da categoria que está sendo modificada (não sua categoria pai)
		strcpy(catTemp, c.categoria);
		if (strcmp(cNew.categoria, "luof") == 0)
			strcpy(c.categoria, c.nome);
		else
			fIncrementaCamCat(c.categoria, c.nome);
		fBuscaCat(&db, c, &categoria3);
		strcpy(c.categoria, catTemp);
		
		//catTemp passa a ser o caminho do arquivo da categoria3
		strcpy(catTemp, c.categoria);
		if (strcmp(cNew.categoria, "luof") == 0)
			strcpy(catTemp, c.nome);
		else
			fIncrementaCamCat(catTemp, c.nome);

		//muda nome na árvore
		strcpy(categoria3->nome, cNew.nome);
		fEscreveLuof(&db);

		//muda nome na listaSites da categoria pai
		if (strcmp(cNew.categoria, "luof") == 0)
			db.listaSites = db.raiz;
		encontrou = 0;
		it = criaIt(db.listaSites);
		do {
			fav = retornaItera(&it);
			if ((strcmp(fav->nome, c.nome) == 0) && (strcmp(fav->categoria, c.categoria) == 0) && (fav->ehCat == c.ehCat)) {
				strcpy(fav->nome, cNew.nome);
				encontrou = 1;
			}
			iteraProximo(&it);
		} while (encontrou == 0 && !inicioIt(&it));
		if (strcmp(cNew.categoria, "luof") == 0)
			fEscreveLuof(&db);
		else
			fEscreveArquivoCat(&db, categoria2->nome);

		//cria um novo arquivo com o nome correto
		if (fSeparaArquivoCategoria(&db, catTemp, categoria3, c.nome))
			return;

		//muda categoria em todos os favoritos filhos
		char caminhoN[TAMCAMINHO];
		strcpy(caminhoN, cNew.categoria);
		if (strcmp(cNew.categoria, "luof") == 0)
			strcpy(caminhoN, cNew.nome);
		else
			fIncrementaCamCat(caminhoN, cNew.nome);
		fMudaCaminhoCategoriaArvore_private(&db, categoria3, catTemp, caminhoN);

	}

	//freeList(listaTemp1);
	fFinalizaDB(&db);
	
	printf("\nFavorito modificado com sucesso.\n");

}
