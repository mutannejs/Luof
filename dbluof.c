#include "luof.h"

int fInicializaDB(sBanco *db) {

	char dir[TAMLINKARQ];
	char vBooleana;
	struct stat st;

	//seta a variável caminhoDB com o caminho para o banco de dados
	/* --- quando caminhoDB estiver com o caminho da home do usuário ---
	char *login;
	login = getlogin();
	sprintf(caminhoDB, "/home/%s/.luof/", login);
	------------------------------------------------------------------*/
	strcpy(caminhoDB, ".luof/");
	tamCaminhoDB = strlen(caminhoDB);

	strcpy(dir, caminhoDB);

	if (stat(caminhoDB, &st) == -1) {//se ainda não existe o DB
		printf("Criar novo Banco de dados? [s/n]: ");
		scanf(" %c", &vBooleana);
		if (vBooleana != 's') {
			printf("Saindo...\n");
			return 1;
		}

		//cria diretório .luof
		if (mkdir(caminhoDB, S_IRWXU) == -1) {
			printf("Erro: não foi possível criar diretório na home do usuário.\n");
			return 1;
		}

		//cria banco de dados
		fSetaCaminhoArquivo(dir, "luof");
		db->aLuof = fopen(dir, "w");
		if (db->aLuof == NULL) {
			printf("Erro: não foi possível criar banco de dados.\n");
			return 1;
		}

		printf("Banco de dados criado com sucesso.\n\n");
	}
	else {//se existe o DB
		fSetaCaminhoArquivo(dir, "luof");
		db->aLuof = fopen(dir, "r");

		if (db->aLuof == NULL) {
			printf("Erro: Não foi possível acessar o banco de dados.\n");
			return 1;
		}
	}

	//preenche uma sLista com todas as categorias
	fPreencheListaCat(db);
	//preenche uma sLista com todos os favoritos da raiz
	fPreencheRaiz(db);
	db->aCat = NULL;
	db->listaSites = NULL;

	return 0;

}

void fFinalizaDB(sBanco *db) {

	//fecha arquivos
	if (db->aLuof)
		fclose(db->aLuof);
	if (db->aCat)
		fclose(db->aCat);

	//libera estruturas do banco
	if (db->listaCategorias) {
		fLiberaCats(db->listaCategorias);
	}
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
	char nomeTemp[TAMNOMEFAV], ehCategoria[3];
	int tamanho;

	db->raiz = criaLista(struct sSite);

	//pega linha por linha do arquivo aLuof(logo após "##\n"), faz a comparação usando a primeira linha de um site, e depois pega as outras três referentes ao mesmo site
	while (fgets(nomeTemp, TAMNOMEFAV, db->aLuof) != NULL) {

		//guarda em siteTemp o nome
		tamanho = strlen(nomeTemp);
		nomeTemp[tamanho-1] = '\0';
		strcpy(siteTemp.nome, nomeTemp);

		//guarda em siteTemp a categoria
		fgets(siteTemp.categoria, TAMCAMINHO, db->aLuof);
		tamanho = strlen(siteTemp.categoria);
		siteTemp.categoria[tamanho-1] = '\0';

		//guarda em siteTemp o link
		fgets(siteTemp.link, TAMLINKARQ, db->aLuof);
		tamanho = strlen(siteTemp.link);
		siteTemp.link[tamanho-1] = '\0';

		//guarda em siteTemp o texto
		fgets(siteTemp.texto, TAMTEXTO, db->aLuof);
		tamanho = strlen(siteTemp.texto);
		siteTemp.texto[tamanho-1] = '\0';

		//guarda em siteTemp o ehCat e ignora o '\n'
		fgets(ehCategoria, 3, db->aLuof);
		siteTemp.ehCat = ehCategoria[0];

		//adiciona o site na lista
		pushBackList(db->raiz, &siteTemp);

	}

}
