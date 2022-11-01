#include "luof.h"

void fModifySite() {

	sSite s, sNew;
	sCat *categoria;
	sCat *categoria2;
	sBanco db;
	int opcao;

	if (fInicializaDB(&db))
		return;

	if (fSetaSiteCategoria(&s))
		return;

	if (strcmp(s.categoria, "/") == 0) {
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
		printf("\nO favorito não existe para ser modificado.\n");
		fFinalizaDB(&db);
		return;
	}

	//printa os dados do site para o usuário saber o que modificar
	printf("\nDados antigos:\n");
	if (strcmp(s.categoria, "luof") == 0)
		printf("Categoria : /\n");
	else
		printf("Categoria : %s\n", s.categoria);
	printf("Nome      : %s\n", s.nome);
	printf("Link      : %s\n", s.link);
	printf("Texto     : %s\n", s.texto);

	//pergunta qual atributo será modificado
	printf("\nVocê deseja modificar? [1]categoria [2]nome [3]link [4]texto [5]tudo [6]nada : ");
	scanf(" %d", &opcao);

	//se os usuário não desejar modificar nada
	if (opcao < 1 || opcao > 5) {
		printf("\nSaindo...\n");
		fFinalizaDB(&db);
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
		if (fSetaSiteCategoria(&sNew))
			return;
		if (strcmp(sNew.categoria, "/") == 0) {
			strcpy(sNew.categoria, "luof");
			categoria2 = db.listaCategorias;
		}
		else if (fBuscaCat(&db, sNew, &categoria2)) {
			fFinalizaDB(&db);
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
		if (fSetaSiteNome(&sNew))
			return;
	}

	//se o nome ou a categoria for modificados é necessário ver se já não existe um favorito igual
	if (strcmp(s.categoria, sNew.categoria) || strcmp(s.nome, sNew.nome)) {
		if (fBuscaFavorito(&db, &sNew)) {
			printf("\nJá existe outro favorito com esse nome e categoria, logo a modificação não pode ser concluída.\n");
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
		printf("\nNão foi feito modificações.\n");
		fFinalizaDB(&db);
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
		catIt = (struct sCat*) retornaItera(&it);
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

	if (fSetaCatCategoria(&c))
		return;

	if (strcmp(c.categoria, "/") == 0) {
		if (emptyList(db.raiz)) {
			printf("\nCategoria pai vazia.\n");
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
			printf("\nCategoria pai vazia.\n");
			fFinalizaDB(&db);
			return;
		}
	}

	if (fSetaCatNome(&c))
		return;
	c.ehCat = '1';

	if (!fBuscaFavorito(&db, &c)) {
		printf("\nA categoria não existe para ser modificada.\n");
		fFinalizaDB(&db);
		return;
	}

	//printa os dados da categoria para o usuário saber o que modificar
	printf("\nDados antigos:\n");
	if (strcmp(c.categoria, "luof") == 0)
		printf("Categoria pai     : /\n");
	else
		printf("Categoria pai     : %s\n", c.categoria);
	printf("Nome da categoria : %s\n", c.nome);

	//pergunta qual atributo será modificado
	printf("\nVocê deseja modificar? [1]categoria pai [2]nome da categoria [3]tudo [4]nada : ");
	scanf(" %d", &opcao);

	//se o usuário não deseja modificar nada
	if (opcao < 1 || opcao > 3) {
		printf("\nSaindo...\n");
		fFinalizaDB(&db);
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
		if (fSetaCatCategoria(&cNew))
			return;
		if (strcmp(cNew.categoria, "/") == 0) {
			strcpy(cNew.categoria, "luof");
			categoria2 = db.listaCategorias;
		}
		else if (fBuscaCat(&db, cNew, &categoria2)) {
			fFinalizaDB(&db);
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
		if (fSetaCatNome(&cNew))
			return;
	}

	/* --- Quando a função fJoinCategory() estiver pronta ---
	if (strcmp(c.categoria, cNew.categoria) || strcmp(c.nome, cNew.nome)) {
		rBuscaFavorito = fBuscaFavorito(&db, &cNew);
		if (rBuscaFavorito) {
			printf("\nJá existe outro favorito com esse nome e categoria, você deseja junta-los em um só? [s/n]: \n");
			scanf(" %c", &vBooleana);
			if (vBooleana != 's') {
			printf("\nJá existe outro favorito com esse nome e categoria.\n");//tirar
				printf("Saindo...\n");
				fFinalizaDB(&db);
				return;
			}
			else {
				fJoinCategorys_private(&db, s, sNew);
				fFinalizaDB(&db);
				return;
			}
		}
	}*/
	if (strcmp(c.categoria, cNew.categoria) || strcmp(c.nome, cNew.nome)) {
		rBuscaFavorito = fBuscaFavorito(&db, &cNew);
		if (rBuscaFavorito) {
			if (vBooleana != 's') {
				printf("\nJá existe outro favorito com esse nome e categoria.\n");
				printf("Saindo...\n");
				fFinalizaDB(&db);
				return;
			}
		}
	}

	//se nada foi modificado
	if (!strcmp(c.nome, cNew.nome) && !strcmp(c.categoria, cNew.categoria) && !strcmp(c.link, cNew.link) && !strcmp(c.texto, cNew.texto)) {
		printf("\nNão foi feito modificações.\n");
		fFinalizaDB(&db);
		return;
	}

	//caminhoA é o caminho antigo dos favoritos pertencentes a categoria
	fSetaCaminhoCategoria(caminhoA, c);

	//caminhoN é o caminho novo dos favoritos pertencentes a categoria
	fSetaCaminhoCategoria(caminhoN, cNew);

	//seta categoria3 como a posição na árvore da categoria que está sendo modificada
	strcpy(catTemp, c.categoria);
	if (strcmp(c.categoria, "luof") == 0)
		strcpy(c.categoria, c.nome);
	else
		fIncrementaCamCat(c.categoria, c.nome);
	fBuscaCat(&db, c, &categoria3);
	strcpy(c.categoria, catTemp);

	if (strcmp(c.categoria, cNew.categoria) && !strcmp(c.nome, cNew.nome)) {//se só a categoria será modificada

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
	else if (!strcmp(c.categoria, cNew.categoria) && strcmp(c.nome, cNew.nome)) {//se só o nome será modificado

		//fAtualizaNomeCatListaSitesPai(&db, categoria2, c, cNew);
		fModificaFavorito(&db, c, cNew, categoria2);

		//muda nome na árvore
		strcpy(categoria3->nome, cNew.nome);
		fEscreveLuof(&db);

		//junta os favoritos das categorias
		if (fSeparaArquivoCategoria(&db, caminhoA, categoria3, c.nome)) {
			fFinalizaDB(&db);
			return;
		}

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
		if (fSeparaArquivoCategoria(&db, caminhoA, categoria3, c.nome)) {
			fFinalizaDB(&db);
			return;
		}

		//muda categoria em todos os favoritos filhos
		fMudaCaminhoCategoriaArvore(&db, categoria3, caminhoA, caminhoN);

	}

	fFinalizaDB(&db);

	printf("\nFavorito modificado com sucesso.\n");

}

/*void fJoinCategorys_juntaLista(sBanco *db, sCat *cate, sCat *cate2, sSite c1, sSite c2) {

	sCat *catIt, *cat1;
	sSite *siteDoIterador, *siteDoIterador2;
	sLista listaTemp, listaSitesA;
	sIterador it, it2;
	char caminhoA[TAMCAMINHO], nomeArqCat[TAMLINKARQ];
	int encontrou, inserir, qtdLista, continuarLoop;

	//caminhoA é o caminho antigo dos favoritos pertencentes a categoria
	fSetaCaminhoCategoria(caminhoA, c1);
printf("12\n");

	//cat passa a apontar para a categoria que foi juntada
	encontrou = 0;
	it = criaIt(cate->catFilhos);
	do {
		catIt = (struct sCat*) retornaItera(&it);
		if (strcmp(catIt->nome, c1.nome) == 0) {
			cat1 = catIt;
			encontrou = 1;
		}
		else {
			iteraProximo(&it);
		}
	} while (encontrou == 0 && !inicioIt(&it));
printf("13\n");

	//guarda em listaSitesA os favoritos do arquivo da cat
	listaTemp = db->listaSites;
	fPreencheListaSite(db, cat1);
	listaSitesA = db->listaSites;
printf("15\n");

	//guarda em db->listaSites os favoritos do arquivo da cat2
	fPreencheListaSite(db, cat2);
printf("16\n");

	//insere os sites da listaSitesA (removendo-os também) e insere na db->listaSites
	it = criaIt(listaSitesA);
	it2 = criaIt(db->listaSites);
	do {
		qtdLista = sizeList(listaSitesA);
		siteDoIterador = (struct sSite*) retornaItera(&it);
		if (strcmp(siteDoIterador->categoria, caminhoA) == 0) {
			//----------------------------
			encontrou = 0;
			inserir = 1;
			continuarLoop = 0;
			if (emptyList(db->listaSites)) {
				pushBackList(db->listaSites, siteDoIterador);
			}
			else if (siteDoIterador->ehCat == '0') {
				iteraFim(&it2);
				do {
					siteDoIterador2 = (struct sSite*) retornaItera(&it2);
					if (!strcmp(siteDoIterador2->nome, siteDoIterador->nome) && siteDoIterador2->ehCat == siteDoIterador->ehCat)
						inserir = 0;
					else if (strcmp(siteDoIterador2->nome, siteDoIterador->nome) < 0 || siteDoIterador2->ehCat == '1')
						encontrou = 1;
					else
						iteraAnterior(&it2);
				} while (encontrou == 0 && inserir == 1 && !fimIt(&it2));
				siteDoIterador2 = (struct sSite*) frontList(db->listaSites);
				if (siteDoIterador2->ehCat == '0' && encontrou == 0 && inserir == 1)
					pushFrontList(db->listaSites, siteDoIterador);
				else if (inserir == 1)
					insereProxIt(&it2, siteDoIterador);
			}
			else {
				iteraInicio(&it2);
				do {
					siteDoIterador2 = (struct sSite*) retornaItera(&it2);
					if (!strcmp(siteDoIterador2->nome, siteDoIterador->nome) && siteDoIterador2->ehCat == siteDoIterador->ehCat) {
						inserir = 0;
						continuarLoop = 1;
						//fJoinCategorys_juntaLista(db, cat1, cat2, *siteDoIterador, *siteDoIterador2);
					}
					else if (strcmp(siteDoIterador2->nome, siteDoIterador->nome) > 0 || siteDoIterador2->ehCat == '0') {
						encontrou = 1;
					}
					else {
						iteraProximo(&it2);
					}
				} while (encontrou == 0 && inserir == 1 && !inicioIt(&it2));
				siteDoIterador2 = (struct sSite*) backList(db->listaSites);
				if (siteDoIterador2->ehCat == '1' && encontrou == 0 && inserir == 1)
					pushBackList(db->listaSites, siteDoIterador);
				else if (inserir == 1)
					insereAntIt(&it2, siteDoIterador);
			}
			//----------------------------
			removeIt(&it);
		}
		else {
			iteraProximo(&it);
		}
	} while ((!emptyList(listaSitesA) && (!inicioIt(&it) || qtdLista != sizeList(listaSitesA))) || continuarLoop);
printf("16\n");

	//atualiza o arquivo da cat2
	fEscreveArquivoCat(db, c2.nome);
printf("17\n");

	//se a lista da cat1 ficou vazia remove o arquivo antigo, caso contrário só atualiza o arquivo
	freeList(db->listaSites);
	db->listaSites = listaSitesA;
	if (emptyList(listaSitesA)) {
		fSetaCaminhoArquivo(nomeArqCat, c1.nome);
		remove(nomeArqCat);
	}
	else {
		fEscreveArquivoCat(db, c1.nome);
	}
printf("18\n");

	freeList(db->listaSites);
	db->listaSites = listaTemp;
printf("19\n");

}

void fJoinCategorys_private(sBanco *db, sSite c1, sSite c2) {

	sCat *catPai, *cat1, *cat2, *catIt, *catTemp, *catTemp2;
	sSite *siteDoIterador, *siteDoIterador2;
	sLista listaSitesA;
	sIterador it, it2;
	char nomeArqCat[TAMLINKARQ], caminhoA[TAMCAMINHO], caminhoN[TAMCAMINHO];
	int qtdLista, encontrou, inserir, continuarLoop;

	//caminhoA é o caminho antigo dos favoritos pertencentes a categoria
	strcpy(caminhoA, c1.categoria);
	if (strcmp(c1.categoria, "luof") == 0)
		strcpy(caminhoA, c1.nome);
	else
		fIncrementaCamCat(caminhoA, c1.nome);
	fSetaCaminhoCategoria(caminhoA, c1);
printf("1\n");//CORRETO

	//caminhoN é o caminho novo dos favoritos pertencentes a categoria
	strcpy(caminhoN, c2.categoria);
	if (strcmp(c2.categoria, "luof") == 0)
		strcpy(caminhoN, c2.nome);
	else
		fIncrementaCamCat(caminhoN, c2.nome);
	fSetaCaminhoCategoria(caminhoN, c2);
printf("2\n");//CORRETO

	//remove a c1 da listaSites da sua categoriaPai
	if (strcmp(c1.categoria, "luof") == 0) {
		db->listaSites = db->raiz;
		catPai = db->listaCategorias;
	}
	else {
		if (fBuscaCat(db, c1, &catPai))
			return;
		if (fPreencheListaSite(db, catPai))
			return;
	}
	fRemoveFavorito(db, c1, catPai);
printf("3\n");//CORRETO

	//guarda em cat2 a categoria na árvore que representa c2
	if (strcmp(c2.categoria, "luof") == 0) {
		db->listaSites = db->raiz;
		cat2 = db->listaCategorias;
	}
	else {
		if (fBuscaCat(db, c2, &cat2))
			return;
		if (fPreencheListaSite(db, cat2))
			return;
	}
	encontrou = 0;
	it = criaIt(cat2->catFilhos);
	do {
		catIt = (struct sCat*) retornaItera(&it);
		if (strcmp(catIt->nome, c2.nome) == 0) {
			cat2 = catIt;
			encontrou = 1;
		}
		else {
			iteraProximo(&it);
		}
	} while (encontrou == 0 && !inicioIt(&it));
printf("4\n");//CORRETO

	//insere os filhos da cat1 no catFilhos da cat2 e remove cat1 da árvore
	encontrou = 0;
	it = criaIt(catPai->catFilhos);
	do {
		catIt = (struct sCat*) retornaItera(&it);
		if (strcmp(catIt->nome, c1.nome) == 0) {
			//guarda em listaSitesA os favoritos do arquivo da cat1
			fPreencheListaSite(db, catIt);
			listaSitesA = db->listaSites;
			//reposiciona as categorias na árvore
			while (!emptyList(catIt->catFilhos)) {
				inserir = 1;
				it2 = criaIt(cat2->catFilhos);
				catTemp = (struct sCat*) frontList(catIt->catFilhos);
				if (!emptyList(cat2->catFilhos)) {
					do {
						catTemp2 = (struct sCat*) retornaItera(&it2);
						if (strcmp(catTemp2->nome, catTemp->nome) == 0)
							inserir = 0;
						else
							iteraProximo(&it2);
					} while (inserir == 1 && !inicioIt(&it2));
				}
				if (inserir == 1)
					pushBackList(cat2->catFilhos, catTemp);
				popFrontList(catIt->catFilhos);
			}
			removeIt(&it);
			encontrou = 1;
		}
		else {
			iteraProximo(&it);
		}
	} while (encontrou == 0 && !inicioIt(&it));
	fEscreveLuof(db);
//percursoCategorias(db->listaCategorias, 0);
printf("5\n");//CORRETO

	//guarda em db.listaSites os favoritos do arquivo da categoria2
	fPreencheListaSite(db, cat2);
//printaListaSites(db->listaSites);
printf("6\n");//CORRETO

	//se a lista listaSitesA estiver vazia apenas exclui a a categoria
	if (!emptyList(listaSitesA)) {
		fSetaCaminhoArquivo(nomeArqCat, c1.nome);
		remove(nomeArqCat);
	}
printf("7\n");

	//insere os sites da listaSitesA (removendo-os também) e insere na db->listaSites
	it = criaIt(listaSitesA);
	it2 = criaIt(db->listaSites);
	do {
		qtdLista = sizeList(listaSitesA);
		siteDoIterador = (struct sSite*) retornaItera(&it);
		if (strcmp(siteDoIterador->categoria, caminhoA) == 0) {
			//----------------------------
			encontrou = 0;
			inserir = 1;
			continuarLoop = 0;
			if (emptyList(db->listaSites)) {
				pushBackList(db->listaSites, siteDoIterador);
			}
			else if (siteDoIterador->ehCat == '0') {
				iteraFim(&it2);
				do {
					siteDoIterador2 = (struct sSite*) retornaItera(&it2);
					if (!strcmp(siteDoIterador2->nome, siteDoIterador->nome) && siteDoIterador2->ehCat == siteDoIterador->ehCat)
						inserir = 0;
					else if (strcmp(siteDoIterador2->nome, siteDoIterador->nome) < 0 || siteDoIterador2->ehCat == '1')
						encontrou = 1;
					else
						iteraAnterior(&it2);
				} while (encontrou == 0 && inserir == 1 && !fimIt(&it2));
				siteDoIterador2 = (struct sSite*) frontList(db->listaSites);
				if (siteDoIterador2->ehCat == '0' && encontrou == 0 && inserir == 1)
					pushFrontList(db->listaSites, siteDoIterador);
				else if (inserir == 1)
					insereProxIt(&it2, siteDoIterador);
			}
			else {
				iteraInicio(&it2);
				do {
					siteDoIterador2 = (struct sSite*) retornaItera(&it2);
					if (!strcmp(siteDoIterador2->nome, siteDoIterador->nome) && siteDoIterador2->ehCat == siteDoIterador->ehCat) {
						inserir = 0;
						encontrou = 1;
						//continuarLoop = 1;
						//fJoinCategorys_juntaLista(db, catIt, cat2, *siteDoIterador, *siteDoIterador2);
					}
					else if (strcmp(siteDoIterador2->nome, siteDoIterador->nome) > 0 || siteDoIterador2->ehCat == '0') {
						encontrou = 1;
					}
					else {
						iteraProximo(&it2);
					}
				} while (encontrou == 0 && inserir == 1 && !inicioIt(&it2));
				siteDoIterador2 = (struct sSite*) backList(db->listaSites);
				if (siteDoIterador2->ehCat == '1' && encontrou == 0 && inserir == 1)
					pushBackList(db->listaSites, siteDoIterador);
				else if (inserir == 1)
					insereAntIt(&it2, siteDoIterador);
			}
			//----------------------------
			removeIt(&it);
		}
		else {
			iteraProximo(&it);
		}
	} while ((!emptyList(listaSitesA) && (!inicioIt(&it) || qtdLista != sizeList(listaSitesA))) || continuarLoop);
printf("8\n");

	//atualiza o arquivo da cat2
	fEscreveArquivoCat(db, c2.nome);
printf("9\n");

	//se a lista da cat1 ficou vazia remove o arquivo antigo, caso contrário só atualiza o arquivo
	freeList(db->listaSites);
	db->listaSites = listaSitesA;
	if (emptyList(listaSitesA)) {
		fSetaCaminhoArquivo(nomeArqCat, c1.nome);
		remove(nomeArqCat);
	}
	else {
		fEscreveArquivoCat(db, c1.nome);
	}
printf("10\n");

	//muda categoria em todos os favoritos filhos
	fMudaCaminhoCategoriaArvore(db, cat2, caminhoA, caminhoN);
printf("11\n");

}

void fJoinCategorys() {

	sSite c1, c2;
	sCat *categoria;
	sBanco db;

	if (fInicializaDB(&db))
		return;

	//recebe os dados da primeira categoria
	printf("Primeira categoria\n");

	printf("\tCategoria pai     : ");
	scanf(" %[^\n]", c1.categoria);

	if (strcmp(c1.categoria, "/") == 0) {
		strcpy(c1.categoria, "luof");
		db.listaSites = db.raiz;
		categoria = db.listaCategorias;
	}
	else {
		if (fBuscaCat(&db, c1, &categoria)) {
			fFinalizaDB(&db);
			return;
		}
		if (fPreencheListaSite(&db, categoria)) {
			fFinalizaDB(&db);
			return;
		}
	}

	printf("\tNome da categoria : ");
	scanf(" %[^\n]", c1.nome);
	c1.ehCat = '1';

	if (!fBuscaFavorito(&db, &c1)) {
		printf("\nA categoria não existe para ser unida a outra.\n");
		fFinalizaDB(&db);
		return;
	}

	//recebe os dados da segunda categoria
	printf("Segunda categoria\n");

	printf("\tCategoria pai     : ");
	scanf(" %[^\n]", c2.categoria);

	if (strcmp(c2.categoria, "/") == 0) {
		strcpy(c2.categoria, "luof");
		db.listaSites = db.raiz;
		categoria = db.listaCategorias;
	}
	else {
		if (fBuscaCat(&db, c2, &categoria)) {
			fFinalizaDB(&db);
			return;
		}
		if (fPreencheListaSite(&db, categoria)) {
			fFinalizaDB(&db);
			return;
		}
	}

	printf("\tNome da categoria : ");
	scanf(" %[^\n]", c2.nome);
	c2.ehCat = '1';

	if (!fBuscaFavorito(&db, &c2)) {
		printf("\nA categoria não existe para ser unida a outra.\n");
		fFinalizaDB(&db);
		return;
	}

	//junta as categorias
	fJoinCategorys_private(&db, c1, c2);

	fFinalizaDB(&db);

}*/
