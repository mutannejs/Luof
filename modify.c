#include "luof.h"

void fModifySite() {
	
	sSite s, sNew;
	sCat *categoria;
	sCat *categoria2;
	sBanco db;
	int opcao;

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

	//fecha os arquivos abertos e reabre eles
	fFinalizaDB(&db);
	if (fInicializaDB(&db))
		return;

	//guarda em sNew os dados antigos, para caso algum não seja alterado
	strcpy(sNew.categoria, s.categoria);
	strcpy(sNew.nome, s.nome);
	strcpy(sNew.link, s.link);
	strcpy(sNew.texto, s.texto);
	sNew.ehCat = s.ehCat;
	//seta a categoria antiga (se ainda não foi setada)
	if (strcmp(s.categoria, "luof") != 0)
		fBuscaCat(&db, s, &categoria);
	else
		categoria = db.listaCategorias;

	//Pede os novos dados do site
	printf("\nNovos dados:\n");

	//pede a categoria
	if (opcao == 1 || opcao == 5) {
		printf("Categoria: ");
		scanf(" %[^\n]", sNew.categoria);
		if (strcmp(sNew.categoria, "/") == 0) {
			strcpy(sNew.categoria, "luof");
			categoria2 = db.listaCategorias;
		}
		else if (fBuscaCat(&db, sNew, &categoria2)) {
			return;
		}
	}
	else {
		categoria2 = categoria;
	}

	//seta db.listaSites
	if (strcmp(sNew.categoria, "luof") == 0)
		db.listaSites = db.raiz;
	else
		fPreencheListaSite(&db, categoria2);

	//pede o nome
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

	//pede o link
	if (opcao == 3 || opcao == 5) {
		printf("Link: ");
		scanf(" %[^\n]", sNew.link);
	}

	//pede o texto
	if (opcao == 4 || opcao == 5) {
		printf("Texto: ");
		scanf(" %[^\n]", sNew.texto);
	}

	//se nada foi modificado
	if (!strcmp(s.nome, sNew.nome) && !strcmp(s.categoria, sNew.categoria) && !strcmp(s.link, sNew.link) && !strcmp(s.texto, sNew.texto)) {
		printf("\nNão foi feito modificações.\n");
		return;
	}

	if (strcmp(s.categoria, sNew.categoria) == 0 || strcmp(categoria->nome, categoria2->nome) == 0) {//se as categorias forem iguais ou o arquivo for o mesmo

		//modifica o favorito na listaSites da categoria pai
		fModificaFavorito(&db, s, sNew, categoria2);

	}
	else {//as categorias são diferentes

		//adiciona o favorito na categoria nova
		fAdicionaFavorito(&db, sNew, categoria2);

		//seta db.listaSites com a os favoritos da antiga categoria
		if (strcmp(s.categoria, "luof") == 0)
			db.listaSites = db.raiz;
		else
			fPreencheListaSite(&db, categoria);

		//remove o favorito da categoria antiga
		fRemoveFavorito(&db, s, categoria);

	}

	//freeList(listaTemp1);
	fFinalizaDB(&db);
	
	printf("\nFavorito modificado com sucesso.\n");

}

void fReposicionaCatArvore(sBanco *db, sCat *categoria, sCat *categoria2, sCat **categoria3, char *nome) {

	sCat *catIt;
	sIterador it;
	int encontrou;
	
	encontrou = 0;
	it = criaIt(categoria->catFilhos);
	do {
		catIt = retornaItera(&it);
		if (catIt == *categoria3) {
			pushBackList(categoria2->catFilhos, catIt);
			removeIt(&it);
			encontrou = 1;
		}
		iteraProximo(&it);
	} while (encontrou == 0 && !inicioIt(&it));
	*categoria3 = backList(categoria2->catFilhos);
	if (nome)
		strcpy((*categoria3)->nome, nome);
	fEscreveLuof(db);

}

void fModifyCategory() {

	sSite c, cNew;
	sCat *categoria;//a categoria pai antiga
	sCat *categoria2;//a categoria pai nova
	sCat *categoria3;//a própria categoria que será modificada
	sBanco db;
	char vBooleana = 'n', catTemp[TAMCAMINHO], caminhoA[TAMCAMINHO], caminhoN[TAMCAMINHO];
	int opcao, rBuscaFavorito = 0;

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
		printf("\nA categoria não existe para ser modificada\n");
		return;
	}

	//printa os dados da categoria para o usuário saber o que modificar
	printf("\nDados antigos:\n");
	if (strcmp(c.categoria, "luof") == 0)
		printf("Categoria pai: /\n");
	else
		printf("Categoria pai: %s\n", c.categoria);
	printf("Categoria: %s\n", c.nome);

	//pergunta qual atributo será modificado
	printf("\nVocê deseja modificar? [1]categoria pai [2]categoria [3]tudo [4]nada : ");
	scanf(" %d", &opcao);

	//se o usuário não deseja modificar nada
	if (opcao < 1 || opcao > 3) {
		printf("Saindo...\n");
		return;
	}

	//fecha os arquivos abertos e reabre eles
	fFinalizaDB(&db);
	if (fInicializaDB(&db))
		return;

	//guarda em sNew os dados antigos, para caso algum não seja alterado
	strcpy(cNew.categoria, c.categoria);
	strcpy(cNew.nome, c.nome);
	strcpy(cNew.link, c.link);
	strcpy(cNew.texto, c.texto);
	cNew.ehCat = c.ehCat;
	//seta a categoria antiga (se ainda não foi setada)
	if (strcmp(c.categoria, "luof") != 0)
		fBuscaCat(&db, c, &categoria);
	else
		categoria = db.listaCategorias;

	//Pede os novos dados da categoria
	printf("\nNovos dados:\n");

	//pede a categoria pai
	if (opcao == 1 || opcao == 3) {
		printf("Categoria pai: ");
		scanf(" %[^\n]", cNew.categoria);
		if (strcmp(cNew.categoria, "/") == 0) {
			strcpy(cNew.categoria, "luof");
			categoria2 = db.listaCategorias;
		}
		else if (fBuscaCat(&db, cNew, &categoria2)) {
			return;
		}
	}
	else {
		categoria2 = categoria;
	}

	//seta db.listaSites
	if (strcmp(cNew.categoria, "luof") == 0)
		db.listaSites = db.raiz;
	else
		fPreencheListaSite(&db, categoria2);

	//pede a categoria
	if (opcao == 2 || opcao == 3) {
		printf("Categoria: ");
		scanf(" %[^\n]", cNew.nome);
	}

	if (strcmp(c.categoria, cNew.categoria) || strcmp(c.nome, cNew.nome)) {
		rBuscaFavorito = fBuscaFavorito(&db, &cNew);
		if (rBuscaFavorito) {
	//		printf("\nJá existe outro favorito com esse nome e categoria, você deseja junta-los em um só? [s/n]: \n");CHAMAR OUTRA FUNÇÃO PARA ISSO
	//		scanf(" %c", &vBooleana);
	//		if (vBooleana != 's') {
			printf("\nJá existe outro favorito com esse nome e categoria\n");
				printf("Saindo...\n");
				return;
	//		}
		}
	}

	//se nada foi modificado
	if (!strcmp(c.nome, cNew.nome) && !strcmp(c.categoria, cNew.categoria) && !strcmp(c.link, cNew.link) && !strcmp(c.texto, cNew.texto)) {
		printf("\nNão foi feito modificações.\n");
		return;
	}

	//caminhoA é o caminho antigo dos favoritos pertencentes a categoria
	strcpy(caminhoA, c.categoria);
	if (strcmp(c.categoria, "luof") == 0)
		strcpy(caminhoA, c.nome);
	else
		fIncrementaCamCat(caminhoA, c.nome);

	//caminhoN é o caminho novo dos favoritos pertencentes a categoria
	strcpy(caminhoN, cNew.categoria);
	if (strcmp(cNew.categoria, "luof") == 0)
		strcpy(caminhoN, cNew.nome);
	else
		fIncrementaCamCat(caminhoN, cNew.nome);

	//seta categoria3 como a posição na árvore da categoria que está sendo modificada
	strcpy(catTemp, c.categoria);
	if (strcmp(c.categoria, "luof") == 0)
		strcpy(c.categoria, c.nome);
	else
		fIncrementaCamCat(c.categoria, c.nome);
	fBuscaCat(&db, c, &categoria3);
	strcpy(c.categoria, catTemp);

	if (opcao == 1) {//se só a categoria será modificada

		//se o arquivo das categorias pais (antiga e nova) for o mesmo
		if (strcmp(categoria->nome, categoria2->nome) == 0) {
			//modifica o favorito na listaSites da categoria pai
			fModificaFavorito(&db, c, cNew, categoria2);
		}
		else {
			//adiciona na listaSites da categoria pai nova
			fAdicionaFavorito(&db, cNew, categoria2);

			//seta db.listaSites com a os favoritos da nova antiga
			if (strcmp(c.categoria, "luof") == 0)
				db.listaSites = db.raiz;
			else
				fPreencheListaSite(&db, categoria);

			//remove da listaSites da categoria pai antiga
			fRemoveFavorito(&db, c, categoria);
		}
		
		//muda categoria em todos os favoritos filhos
		fMudaCaminhoCategoriaArvore(&db, categoria3, caminhoA, caminhoN);

		//coloca a categoria na posição correta na árvore
		fReposicionaCatArvore(&db, categoria, categoria2, &categoria3, NULL);

	}
	else if (opcao == 2) {//se só o nome será modificado

		//fAtualizaNomeCatListaSitesPai(&db, categoria2, c, cNew);
		fModificaFavorito(&db, c, cNew, categoria2);

		//muda nome na árvore
		strcpy(categoria3->nome, cNew.nome);
		fEscreveLuof(&db);

		//junta os favoritos das categorias
		if (fSeparaArquivoCategoria(&db, caminhoA, categoria3, c.nome))
			return;

		//muda categoria em todos os favoritos filhos
		fMudaCaminhoCategoriaArvore(&db, categoria3, caminhoA, caminhoN);

	}
	else {//se tudo é modificado

		//coloca a categoria na posição correta na árvore e atualiza seu nome
		fReposicionaCatArvore(&db, categoria, categoria2, &categoria3, cNew.nome);

		//se o arquivo das categorias pais (antiga e nova) for o mesmo
		if (strcmp(categoria->nome, categoria2->nome) == 0) {
			//modifica o favorito na listaSites da categoria pai
			fModificaFavorito(&db, c, cNew, categoria2);
		}
		else {
			//adiciona na listaSites da categoria pai nova
			fAdicionaFavorito(&db, cNew, categoria2);

			//seta db.listaSites com a os favoritos da nova antiga
			if (strcmp(c.categoria, "luof") == 0)
				db.listaSites = db.raiz;
			else
				fPreencheListaSite(&db, categoria);

			//remove da listaSites da categoria pai antiga
			fRemoveFavorito(&db, c, categoria);
		}

		//cria um novo arquivo com o nome correto
		if (fSeparaArquivoCategoria(&db, caminhoA, categoria3, c.nome))
			return;

		//muda categoria em todos os favoritos filhos
		fMudaCaminhoCategoriaArvore(&db, categoria3, caminhoA, caminhoN);

	}

	//freeList(listaTemp1);
	fFinalizaDB(&db);

	printf("\nFavorito modificado com sucesso.\n");

}
