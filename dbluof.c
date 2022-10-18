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
			printf("Erro: não foi possível criar diretório na home do usuário\n");
			return 1;
		}

		//cria banco de dados
		fSetaCaminhoArquivo(dir, "luof");
		db->aLuof = fopen(dir, "w");
		if (db->aLuof == NULL) {
			printf("Erro: não foi possível criar banco de dados\n");
			return 1;
		}

		printf("Banco de dados criado com sucesso\n\n");
	}
	else {//se existe o DB
		fSetaCaminhoArquivo(dir, "luof");
		db->aLuof = fopen(dir, "r");
		
		if (db->aLuof == NULL) {
			printf("Erro: Não foi possível acessar o banco de dados\n");
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
		freeList(db->listaCategorias);
	}
	if (db->listaSites && db->listaSites != db->raiz)
		freeList(db->listaSites);
	if (db->raiz)
		freeList(db->raiz);

}

void fLiberaCats(sLista listaCategorias) {
	
	if (!emptyList(listaCategorias)) {
		sIterador it = criaIt(listaCategorias);
		do {
			sCat *cat = (struct sCat*) retornaItera(&it);
			fLiberaCats(cat->catFilhos);
			//libera a lista na volta da recursão
			freeList(cat->catFilhos);
			//--------------
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}
}

char* fPreencheListaCat_private(sBanco *db, sCat *cPai, char linhaCat[]) {

	//variaveis usadas
	sCat c, *cIt;
	char *rPreencheListaCat_private;
	int hierarquia = linhaCat[0] - 48;

	//seta os dados da categoria que será adicionada
	strcpy(c.nome, &linhaCat[1]);
	c.catPai = cPai;
	c.catFilhos = criaLista(struct sCat);
	
	//insere ela na lista de catFilhos
	pushBackList(cPai->catFilhos, &c);
	
	fgets(linhaCat, 100, db->aLuof);
	while (strcmp(linhaCat, "##\n") != 0) {
				
		//muda o \n no fim da linha para \0
		linhaCat[strlen(linhaCat)-1] = '\0';
		
		if (linhaCat[0] - 48 == hierarquia) {//adiciona a categoria em cPai->catFilhos
			//seta os dados da categoria que será adicionada
			strcpy(c.nome, &linhaCat[1]);
			c.catPai = cPai;
			c.catFilhos = criaLista(struct sCat);
			
			//insere ela na lista de catFilhos
			pushBackList(cPai->catFilhos, &c);
		}
		else if (linhaCat[0] - 48 < hierarquia) {
			return linhaCat;
		}
		else {//adiciona a categoria em catFilhos da última categoria adicionada
			cIt = (struct sCat*) backList(cPai->catFilhos);
			rPreencheListaCat_private = fPreencheListaCat_private(db, cIt, linhaCat);

			if (strcmp(rPreencheListaCat_private, "##\n") == 0) {//se a última linhaCat lida for o fim (##\n)
				return rPreencheListaCat_private;
			}
			else if (rPreencheListaCat_private[0] - 48 < hierarquia+1)//se a última linha lida for de uma hierarquia inferior a cPai
				return rPreencheListaCat_private;
		}
		
		//pega a próxima linha
		fgets(linhaCat, 100, db->aLuof);
		
	}
	
	//a árvore de categorias está completa
	return linhaCat;

}

void fPreencheListaCat(sBanco *db) {

	sCat c, *cIt;
	char linhaCat[100], *rFgets;
	char *rPreencheListaCat_private;

	//cria a lista para inserir as categorias
	db->listaCategorias = criaLista(struct sCat);

	//pega linha por linha do arquivo .luof/luof
	rFgets = fgets(linhaCat, 100, db->aLuof);
	while (rFgets != NULL && strcmp(linhaCat, "##\n") != 0) {

		//muda o \n no fim da linha para \0
		linhaCat[strlen(linhaCat)-1] = '\0';
		
		if (linhaCat[0] == '0') {//adiciona a categoria em db->listaCategorias
			strcpy(c.nome, &linhaCat[1]);
			c.catPai = NULL;
			c.catFilhos = criaLista(struct sCat);
			pushBackList(db->listaCategorias, &c);
		}
		else {//adiciona a categoria em catFilhos da última categoria adicionada
			cIt = (struct sCat*) backList(db->listaCategorias);
			rPreencheListaCat_private = fPreencheListaCat_private(db, cIt, linhaCat);
			if (strcmp(rPreencheListaCat_private, "##\n") == 0) {
				return;
			}
			else {
				strcpy(c.nome, &rPreencheListaCat_private[1]);
				c.catPai = NULL;
				c.catFilhos = criaLista(struct sCat);
				pushBackList(db->listaCategorias, &c);
			}
		}
		
		fgets(linhaCat, 100, db->aLuof);
	
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
