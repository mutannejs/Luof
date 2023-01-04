#include "luof.h"

int fInicializaDB(sBanco *db) {

	char dir[TAMLINKARQ];
	char vBooleana;
	struct stat st;

	//seta a variável caminhoDB com o caminho para o banco de dados
	strcpy(db->caminhoDB, ".luof/");
	db->tamCaminhoDB = strlen(db->caminhoDB);
	/* --- quando caminhoDB estiver com o caminho da diretório atual ---
	char *login;
	login = getlogin();
	sprintf(db->caminhoDB, "/home/%s/.luof/", login);
	------------------------------------------------------------------*/

	strcpy(dir, db->caminhoDB);

	if (stat(db->caminhoDB, &st) == -1) {//se ainda não existe o DB
		printf(ANSI_BOLD_WHT "Criar novo Banco de dados? [s/n]: " ANSI_COLOR_GRA);
		scanf(" %c", &vBooleana);
		if (vBooleana != 's') {
			printf(ANSI_BOLD_WHT "Saindo...\n");
			return 1;
		}

		//cria diretório .luof
		if (mkdir(db->caminhoDB, S_IRWXU) == -1) {
			printf(ANSI_COLOR_RED "Erro: não foi possível criar diretório na home do usuário.\n");
			return 1;
		}

		//cria banco de dados
		fSetaCaminhoArquivo(db, dir, "luof");
		db->aLuof = fopen(dir, "w");
		if (db->aLuof == NULL) {
			printf(ANSI_COLOR_RED "Erro: não foi possível criar banco de dados.\n");
			return 1;
		}

		printf(ANSI_BOLD_WHT "Banco de dados criado com sucesso.\n\n");
	}
	else {//se existe o DB
		fSetaCaminhoArquivo(db, dir, "luof");
		db->aLuof = fopen(dir, "r");

		if (db->aLuof == NULL) {
			printf(ANSI_COLOR_RED "Erro: Não foi possível acessar o banco de dados.\n");
			return 1;
		}
	}

	//preenche uma sLista com todas as categorias
	fPreencheListaCat(db);
	//preenche uma sLista com todos os favoritos da raiz
	fPreencheRaiz(db);
	//fecha o arquivo db->aLuof e aponta os ponteiros não usado para NULL
	fclose(db->aLuof);
	db->aLuof = NULL;
	db->listaSites = NULL;

	return 0;

}

void fFinalizaDB(sBanco *db) {

	//libera estruturas do banco
	if (db->listaCategorias)
		fLiberaCats(db->listaCategorias);
	if (db->listaSites && db->listaSites != db->raiz)
		freeList(db->listaSites);
	if (db->raiz)
		freeList(db->raiz);

}

void fLiberaCats(sCat *listaCategorias) {

	sCat *cat;
	sIterador it;

	if (!emptyList(listaCategorias->catFilhos)) {
		it = criaIt(listaCategorias->catFilhos);
		do {
			cat = (struct sCat*) retornaItera(&it);
			fLiberaCats(cat);
			//libera a lista na volta da recursão
			freeList(cat->catFilhos);
			//--------------
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

}

void fPreencheListaCat(sBanco *db) {

	sCat c;//a categoria lida terá seus dados guardados aqui
	sCat *cPaiAtual;//aponta para a categoria que possui a lista onde a categoria lida será inserida
	char linhaCat[TAMNOMEFAV+1];//usado para ler as linhas do arquivo

	//seta os dados de db->listaCategorias
	db->listaCategorias = malloc(sizeof(struct sCat));
	strcpy(db->listaCategorias->nome, "luof");
	db->listaCategorias->hie = -1;
	db->listaCategorias->catPai = NULL;
	db->listaCategorias->catFilhos = criaLista(struct sCat);

	//cPai passa a apontar para db->listaCategorias
	cPaiAtual = db->listaCategorias;

	//lê linha por linha e constroi a árvore de categorias
	if (fgets(linhaCat, 100, db->aLuof)) {
		while (strcmp(linhaCat, "##\n") != 0) {

			//seta os dados da caategoria lida
			strcpy(c.nome, &linhaCat[1]);
			c.nome[strlen(c.nome) - 1] = '\0';
			c.hie = linhaCat[0] - 48;
			c.catFilhos = criaLista(struct sCat);

			if (c.hie == cPaiAtual->hie + 1) {//se a cat é da mesma hierarquia da última cat lida
				c.catPai = cPaiAtual;
				pushBackList(cPaiAtual->catFilhos, &c);
			}
			else if (c.hie == cPaiAtual->hie + 2) {//se a cat é na verdade filha da última cat lida
				cPaiAtual = backList(cPaiAtual->catFilhos);
				c.catPai = cPaiAtual;
				pushBackList(cPaiAtual->catFilhos, &c);
			}
			else {//se a cat é de uma hierarquia menor da última cat lida
				for (int i = cPaiAtual->hie; i > c.hie; i--)
					cPaiAtual = cPaiAtual->catPai;
				cPaiAtual = cPaiAtual->catPai;
				c.catPai = cPaiAtual;
				pushBackList(cPaiAtual->catFilhos, &c);
			}

			//lê a próxima linha
			fgets(linhaCat, 100, db->aLuof);
		}
	}

}

void fPreencheRaiz(sBanco *db) {

	//usado para armazenar temporariamente os sites
	sSite siteTemp;
	char nomeTemp[TAMNOMEFAV];

	db->raiz = criaLista(struct sSite);

	//pega linha por linha do arquivo aLuof(logo após "##\n"), faz a comparação usando a primeira linha de um site, e depois pega as outras três referentes ao mesmo site
	while (fgets(nomeTemp, TAMNOMEFAV, db->aLuof) != NULL) {
		siteTemp = fRecuperaFavorito(db->aLuof, nomeTemp);
		pushBackList(db->raiz, &siteTemp);
	}

}

void fEscreveLuof_private(sBanco *db, sLista listaCategorias, int hierarquia) {

	sCat *cat;
	sIterador it;

	if (!emptyList(listaCategorias)) {
		it = criaIt(listaCategorias);
		do {
			cat = (struct sCat*) retornaItera(&it);
			//escreve no arquivo
			fprintf(db->aLuof, "%d%s\n", hierarquia, cat->nome);
			//------------------
			fEscreveLuof_private(db, cat->catFilhos, hierarquia+1);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

}

void fEscreveLuof(sBanco *db) {

	sSite *siteDoIterador;
	sIterador it;
	char nomeArqCat[TAMLINKARQ];

	//reabre o arquivo aLuof para sobreescreve-lo
	fSetaCaminhoArquivo(db, nomeArqCat, "luof");
	if (db->aLuof)
		db->aLuof = freopen(nomeArqCat, "w", db->aLuof);
	else
		db->aLuof = fopen(nomeArqCat, "w");

	//escreve a árvore de categorias no arquivo aLuof
	fEscreveLuof_private(db, db->listaCategorias->catFilhos, 0);

	//marca o fim da árvore de categorias
	fprintf(db->aLuof, "##\n");

	//escreve os favoritos da raiz
	it = criaIt(db->raiz);
	do {
		siteDoIterador = (struct sSite*) retornaItera(&it);
		fprintf(db->aLuof, "%s\n%s\n%s\n%s\n%c\n", siteDoIterador->nome, siteDoIterador->categoria, siteDoIterador->link, siteDoIterador->texto, siteDoIterador->ehCat);
		iteraProximo(&it);
	} while (!inicioIt(&it));

	fclose(db->aLuof);
	db->aLuof = NULL;

}
