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
	printf("Texto     : %s\n", s.texto);

	//pergunta qual atributo será modificado
	printf(ANSI_BOLD_WHT "\nVocê deseja modificar? [1]categoria [2]nome [3]link [4]texto [5]tudo [6]nada : " ANSI_COLOR_GRA);
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

/*int fSeparaArquivoCategoria(sBanco *db, char categoria[], sCat *cat, char nomeA[]) {

	sSite siteTemp, *siteDoIterador, *siteDoIterador2;
	sLista listaSitesA, listaSitesN;
	sIterador it, it2;
	FILE *arq;
	char nomeTemp[TAMNOMEFAV], nomeArqCat[TAMLINKARQ];
	int qtdLista, encontrouPos;

	//preenche db->listaSites com os favoritos do arquivo com nome antigo
	listaSitesA = db->listaSites;
	strcpy(nomeTemp, cat->nome);
	strcpy(cat->nome, nomeA);
	fPreencheListaSite(db, cat);
	strcpy(cat->nome, nomeTemp);

	//verifica se já existe um arquivo com o novo nome da categoria, se sim preenche uma lista com seus favoritos
	listaSitesN = criaLista(struct sSite);
	fSetaCaminhoArquivo(db, nomeArqCat, cat->nome);
	arq = fopen(nomeArqCat, "r");
	if (arq) {
		while (fgets(nomeTemp, 100, arq) != NULL) {
			siteTemp = fRecuperaFavorito(arq, nomeTemp);
			pushBackList(listaSitesN, &siteTemp);
		}
		fclose(arq);
	}

	//remove os sites da categoria da db->listaSites (arquivo com nome antigo) e insere no listaSitesN (arquivo com nome novo)
	it = criaIt(db->listaSites);
	it2 = criaIt(listaSitesN);
	if (!emptyList(db->listaSites)) {
		do {
			qtdLista = sizeList(db->listaSites);
			siteDoIterador = (struct sSite*) retornaItera(&it);
			if (strcmp(siteDoIterador->categoria, categoria) == 0) {
				//----------------------------
				if (emptyList(listaSitesN)) {
					pushBackList(listaSitesN, siteDoIterador);
				}
				else if (siteDoIterador->ehCat == '0') {
					encontrouPos = 0;
					iteraFim(&it2);
					do {
						siteDoIterador2 = (struct sSite*) retornaItera(&it2);
						if (strcmp(siteDoIterador2->nome, siteDoIterador->nome) < 0 || siteDoIterador2->ehCat == '1')
							encontrouPos = 1;
						else
							iteraAnterior(&it2);
					} while (encontrouPos == 0 && !fimIt(&it2));
					siteDoIterador2 = (struct sSite*) frontList(listaSitesN);
					if (siteDoIterador2->ehCat == '0' && encontrouPos == 0)
						pushFrontList(listaSitesN, siteDoIterador);
					else
						insereProxIt(&it2, siteDoIterador);
				}
				else {
					encontrouPos = 0;
					iteraInicio(&it2);
					do {
						siteDoIterador2 = (struct sSite*) retornaItera(&it2);
						if (strcmp(siteDoIterador2->nome, siteDoIterador->nome) > 0 || siteDoIterador2->ehCat == '0')
							encontrouPos = 1;
						else
							iteraProximo(&it2);
					} while (encontrouPos == 0 && !inicioIt(&it2));
					siteDoIterador2 = (struct sSite*) backList(listaSitesN);
					if (siteDoIterador2->ehCat == '1' && encontrouPos == 0)
						pushBackList(listaSitesN, siteDoIterador);
					else
						insereAntIt(&it2, siteDoIterador);
				}
				//----------------------------
				removeIt(&it);
			}
			else {
				iteraProximo(&it);
			}
		} while (!emptyList(db->listaSites) && (!inicioIt(&it) || qtdLista != sizeList(db->listaSites)));
	}

	//se a lista da categoria (nome antigo) ficou vazia remove o arquivo antigo, caso contrário só atualiza o arquivo
	if (emptyList(db->listaSites)) {
		fSetaCaminhoArquivo(db, nomeArqCat, nomeA);
		remove(nomeArqCat);
	}
	else {
		 fEscreveArquivoCat(db, nomeA);
	}

	//escreve o arquivo novo da categoria
	if (!emptyList(listaSitesN)) {
		 db->listaSites = listaSitesN;
		 fEscreveArquivoCat(db, cat->nome);
	}

	freeList(db->listaSites);
	db->listaSites = listaSitesA;

	return 0;

}*/

void fModifyCategory_atualizaCaminho(sBanco *db, sCat *cat, char *caminhoA, int criarNA) {

	sSite *favorito;
	sCat *catTemp;
	sLista listaNA;
	sIterador it;
	char caminhoAntigo[TAMCAMINHO];
	int qtdLista;

	//preenche a lista dos favoritos da categoria
	fPreencheListaSite(db, cat, 0);
	listaNA = db->listaFavs;
	db->listaFavs = criaLista(struct sSite);

	//corrige a categoria dos favoritos da categoria
	if (!emptyList(listaNA)) {
		it = criaIt(listaNA);
		do {
			qtdLista = sizeList(listaNA);
			favorito = (struct sSite*) retornaItera(&it);
			if (strcmp(favorito->categoria, caminhoA) && criarNA) {
				fInsereFavorito(db, *favorito);
				removeIt(&it);
			}
			else {
				strcpy(favorito->categoria, cat->caminho);
			}
			iteraProximo(&it);
		} while ((!emptyList(listaNA) && (!inicioIt(&it) || qtdLista != sizeList(listaNA))));
	}

	//reescreve o(s) arquivo(s)
	if (criarNA) {
		fBuscaCat(db, caminhoA, &catTemp);
		if (!emptyList(db->listaFavs))
			fEscreveArquivoCat(db, catTemp->nome);
		freeList(db->listaFavs);
		db->listaFavs = listaNA;
		fEscreveArquivoCat(db, cat->nome);
	}
	else {
		freeList(db->listaFavs);
		db->listaFavs = listaNA;
		fEscreveArquivoCat(db, cat->nome);
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
		fModifyCategory_atualizaCaminho(db, catTemp, caminhoAntigo, 0);
		iteraProximo(&it);
	} while (!inicioIt(&it));

}

void fModifyCategory() {

	sCat *cat, *catNew, c;//a própria categoria que será modificada
	sCat *catPai;//a categoria pai antiga
	sCat *catPai2;//a categoria pai nova
	sBanco db;
	char catTemp[TAMCAMINHO], caminhoA[TAMCAMINHO], caminhoN[TAMCAMINHO];
	int opcao, rBuscaFavorito = 0;

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

	if (strcmp(c.nome, cat->nome) && !strcmp(c.caminho, catPai->caminho)) {//se só o nome da categoria foi modificado
		strcpy(cat->nome, c.nome);
		catNew = cat;
	}
	else {
		fRemoveCategoria(&db, cat);
		fInsereCategoria(&db, catPai2, c);
		catNew = fBuscaCatFilha(catPai2, c.nome);
	}
	if (!strcmp(c.caminho, catPai->caminho))
		fModifyCategory_atualizaCaminho(&db, catNew, cat->caminho, 0);
	else
		fModifyCategory_atualizaCaminho(&db, catNew, cat->caminho, 1);

	fEscreveLuof(&db);
	fFinalizaDB(&db);

	printf(ANSI_BOLD_WHT "\nCategoria modificada com sucesso.\n");

}
