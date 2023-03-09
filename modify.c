#include "luof.h"

void fModifyBookmark() {

	sSite s, sNew;
	sCat *cat, *cat2;
	sBanco db;
	int opcao;

	if (fInicializaDB(&db))
		return;

	if (fSetaSiteCategoria(&s)) {
		fFinalizaDB(&db);
		return;
	}

	if (fBuscaCat(&db, s.categoria, &cat)) {
		fFinalizaDB(&db);
		return;
	}
	fPreencheListaSite(&db, cat, 0);

	if (fSetaSiteNome(&s)) {
		fFinalizaDB(&db);
		return;
	}

	if (!fBuscaFavorito(&db, &s)) {
		printf(ERRO);
		printf("O favorito não existe para ser modificado.\n");
		fFinalizaDB(&db);
		return;
	}

	//printa os dados do site para o usuário saber o que modificar
	printf(ANSI_BOLD_WHT "\nDados antigos:\n" ANSI_BOLD_YEL);
	printf("Categoria : %s\n", s.categoria);
	printf("Nome      : %s\n", s.nome);
	printf("Link      : %s\n", s.link);
	printf("Descrição : %s\n", s.texto);

	//pergunta qual atributo será modificado
	printf(ANSI_BOLD_WHT "\nVocê deseja modificar? [1]categoria [2]nome [3]link [4]descrição [5]tudo [6]nada : " ANSI_COLOR_GRA);
	scanf(" %d", &opcao);

	//se o usuário não desejar modificar nada
	if (opcao < 1 || opcao > 5) {
		printf(ANSI_BOLD_WHT "\nSaindo...\n");
		fFinalizaDB(&db);
		return;
	}

	//guarda em sNew os dados antigos, para caso algum não seja alterado
	strcpy(sNew.categoria, s.categoria);
	strcpy(sNew.nome, s.nome);
	strcpy(sNew.link, s.link);
	strcpy(sNew.texto, s.texto);
	cat2 = cat;

	//Pede os novos dados do site
	printf(ANSI_BOLD_WHT "\nNovos dados:\n" ANSI_COLOR_GRA);
	//pede a categoria
	if (opcao == 1 || opcao == 5) {
		if (fSetaSiteCategoria(&sNew))
			return;
		if (fBuscaCat(&db, sNew.categoria, &cat2)) {
			fFinalizaDB(&db);
			return;
		}
		fPreencheListaSite(&db, cat2, 0);
	}

	//pede o nome
	if (opcao == 2 || opcao == 5) {
		if (fSetaSiteNome(&sNew))
			return;
	}

	//se o nome ou a categoria for modificados é necessário ver se já não existe um favorito igual
	if (strcmp(s.categoria, sNew.categoria) || strcmp(s.nome, sNew.nome)) {
		if (fBuscaFavorito(&db, &sNew)) {
			printf(ERRO);
			printf("Já existe um favorito chamado %s na categoria %s, logo a modificação não pode ser concluída.\n", sNew.nome, sNew.categoria);
			fFinalizaDB(&db);
			return;
		}
	}

	//pede o link
	if (opcao == 3 || opcao == 5) {
		if (fSetaSiteLink(&sNew))
			return;
	}

	//pede o texto
	if (opcao == 4 || opcao == 5) {
		if (fSetaSiteTexto(&sNew))
			return;
	}

	//se nada foi modificado
	if (!strcmp(s.nome, sNew.nome) && !strcmp(s.categoria, sNew.categoria) && !strcmp(s.link, sNew.link) && !strcmp(s.texto, sNew.texto)) {
		printf(ANSI_BOLD_WHT "\nNão foi feito modificações.\n");
		fFinalizaDB(&db);
		return;
	}

	//adiciona o favorito na categoria nova
	fInsereFavorito(&db, sNew);
	if (strcmp(cat->nome, cat2->nome) != 0) {//se o arquivo da categoria for diferente
		fEscreveArquivoCat(&db, cat2->nome);
		//seta db.listaFavs com os favoritos da antiga categoria
		fPreencheListaSite(&db, cat, 0);
	}
	//remove o favorito da categoria antiga
	fRemoveFavorito(&db, s);
	fEscreveArquivoCat(&db, cat->nome);

	fFinalizaDB(&db);

	printf(ANSI_BOLD_WHT "\nFavorito modificado com sucesso.\n");

}

void fModifyCategory_atualizaCaminho(sBanco *db, sCat *cat, char *caminhoA, char *nomeA) {

	sSite *favorito;
	sCat *catTemp;
	sLista listaNA;//listaNomeAntigo
	sIterador it;
	char caminhoAntigo[TAMCAMINHO], nomeN[TAMNOMEFAV], nomeArqCat[TAMLINKARQ];
	long qtdLista;

	if (strcmp(cat->nome, nomeA)) {//se o nome da categoria mudou

		//preenche a lista dos favoritos do favorito com o nome antigo e a guarda em listaNA
		strcpy(nomeN, cat->nome);
		strcpy(cat->nome, nomeA);
		fPreencheListaSite(db, cat, 0);
		listaNA = db->listaFavs;
		//preenche a lista dos favoritos do arquivo com o nome novo
		db->listaFavs = criaLista(struct sSite);
		strcpy(cat->nome, nomeN);
		fPreencheListaSite(db, cat, 0);

		//corrige a categoria dos favoritos da categoria
		if (!emptyList(listaNA)) {
			it = criaIt(listaNA);
			do {
				qtdLista = sizeList(listaNA);
				favorito = (struct sSite*) retornaItera(&it);
				//caso o favorito verificado seja da categoria modificada
				if (!strcmp(favorito->categoria, caminhoA)) {
					strcpy(favorito->categoria, cat->caminho);
					fInsereFavorito(db, *favorito);
					removeIt(&it);
				}
				else {
					iteraProximo(&it);
				}
			} while ((!emptyList(listaNA) && (!inicioIt(&it) || qtdLista != sizeList(listaNA))));
		}

		//escreve o arquivo com o nome novo da categoria se ele não está vazio
		if (!emptyList(db->listaFavs)) {
			fEscreveArquivoCat(db, nomeN);
		}

		//db->listaFavs passa a ser a lista do arquivo com nome antigo
		freeList(db->listaFavs);
		db->listaFavs = listaNA;
		listaNA = NULL;

		if (emptyList(db->listaFavs)) {//remove o arquivo com o nome antigo da categoria se ele ficou vazio
			fSetaCaminhoArquivo(db, nomeArqCat, nomeA);
			remove(nomeArqCat);
		}
		else {//escreve o arquivo com o nome antigo da categoria se não está vazio
			fEscreveArquivoCat(db, nomeA);
		}

	}
	else {//se o nome da categoria não mudou

		//preenche a lista dos favoritos da categoria
		fPreencheListaSite(db, cat, 0);

		if (!emptyList(db->listaFavs)) {
			//corrige a categoria dos favoritos da categoria
			it = criaIt(db->listaFavs);
			do {
				favorito = (struct sSite*) retornaItera(&it);
				if (!strcmp(favorito->categoria, caminhoA))
					strcpy(favorito->categoria, cat->caminho);
				iteraProximo(&it);
			} while (!inicioIt(&it));

			//escreve o arquivo
			fEscreveArquivoCat(db, cat->nome);
		}

	}

	//chama a mesma função para suas categorias
	if (emptyList(cat->catFilhos))
		return;
	it = criaIt(cat->catFilhos);
	do {
		catTemp = (struct sCat*) retornaItera(&it);
		strcpy(caminhoAntigo, caminhoA);
		strcat(caminhoAntigo, "/");
		strcat(caminhoAntigo, catTemp->nome);
		strcpy(catTemp->caminho, cat->caminho);
		strcat(catTemp->caminho, "/");
		strcat(catTemp->caminho, catTemp->nome);
		fModifyCategory_atualizaCaminho(db, catTemp, caminhoAntigo, catTemp->nome);
		iteraProximo(&it);
	} while (!inicioIt(&it));

}

void fModifyCategory() {

	sCat *cat, *catNew, c;//a própria categoria que será modificada
	sCat *catPai;//a categoria pai antiga
	sCat *catPai2;//a categoria pai nova
	sBanco db;
	char caminhoA[TAMCAMINHO], nomeA[TAMNOMEFAV];
	int opcao;

	if (fInicializaDB(&db))
		return;

	if (fSetaCatCategoria(&c)) {
		fFinalizaDB(&db);
		return;
	}

	if (fBuscaCat(&db, c.caminho, &catPai)) {
		fFinalizaDB(&db);
		return;
	}

	if (fSetaCatNome(&c)) {
		fFinalizaDB(&db);
		return;
	}

	cat = fBuscaCatFilha(catPai, c.nome);
	if (!cat) {
		printf(ERRO);
		printf("A categoria não existe para ser modificada.\n");
		fFinalizaDB(&db);
		return;
	}

	//pergunta qual atributo será modificado
	printf(ANSI_BOLD_WHT "\nVocê deseja modificar? [1]categoria pai [2]nome da categoria [3]tudo [4]nada : " ANSI_COLOR_GRA);
	scanf(" %d", &opcao);

	//se o usuário não deseja modificar nada
	if (opcao < 1 || opcao > 3) {
		printf(ANSI_BOLD_WHT "\nSaindo...\n");
		fFinalizaDB(&db);
		return;
	}

	//Pede os novos dados da categoria
	printf(ANSI_BOLD_WHT "\nNovos dados:\n");

	//pede a categoria pai
	if (opcao == 1 || opcao == 3) {
		if (fSetaCatCategoria(&c)) {
			fFinalizaDB(&db);
			return;
		}
		if (fBuscaCat(&db, c.caminho, &catPai2)) {
			fFinalizaDB(&db);
			return;
		}
	}
	else {
		catPai2 = catPai;
	}

	//pede o nome da categoria
	if (opcao == 2 || opcao == 3) {
		if (fSetaCatNome(&c))
			return;
	}
	catNew = fBuscaCatFilha(catPai2, c.nome);
	if (catNew) {
		printf(ERRO);
		printf("Já existe uma categoria com o nome %s em %s, logo a modificação não pode ser concluída.\n", c.nome, c.caminho);
		fFinalizaDB(&db);
		return;
	}

	//verifica se a nova categoria pai não é ou pertence a uma subcategoria da categoria que será modificada
	if (strlen(cat->caminho) <= strlen(c.caminho) && strncmp(c.caminho, cat->caminho, strlen(c.caminho)) == 0) {
		printf(ERRO);
		printf("A nova categoria pai não pode ser ou pertencer a uma subcategoria da categoria que será modificada.\n");
		fFinalizaDB(&db);
		return;
	}

	//se nada foi modificado
	if (!strcmp(c.nome, cat->nome) && !strcmp(c.caminho, catPai->caminho)) {
		printf(ANSI_BOLD_WHT "\nNão foi feito modificações.\n");
		fFinalizaDB(&db);
		return;
	}

	//seta o caminho e nome antigo da categoria
	strcpy(nomeA, cat->nome);
	strcpy(caminhoA, cat->caminho);

	//coloca a categoria na posição correta da árvore, ou apenas modifica seu nome
	if (strcmp(c.nome, cat->nome) && !strcmp(c.caminho, catPai->caminho)) {//se só o nome da categoria foi modificado
		strcpy(cat->nome, c.nome);
		strcpy(cat->caminho, cat->catPai->caminho);
		fSetaCaminhoCategoria(cat->caminho, c.nome);
		catNew = cat;
	}
	else {//se o caminho mudou
		fInsereCategoria(catPai2, c);
		catNew = fBuscaCatFilha(catPai2, c.nome);
		freeList(catNew->catFilhos);
		catNew->catFilhos = cat->catFilhos;
		cat->catFilhos = NULL;
		fRemoveCategoria(cat);
	}

	//atualiza o caminho de suas subcategorias e favoritos
	fModifyCategory_atualizaCaminho(&db, catNew, caminhoA, nomeA);

	fEscreveLuof(&db);
	fFinalizaDB(&db);

	printf(ANSI_BOLD_WHT "\nCategoria modificada com sucesso.\n");

}
