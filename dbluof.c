#include "luof.h"

int fInicializaDB(sBanco *db) {

	char caminho[TAMLINKARQ];
	char vBooleana;
	struct stat st;
	FILE *arquivo;

	//seta a variável caminhoDB com o caminho para o banco de dados
	/* --- quando caminhoDB estiver com o caminho da home do usuário ---
	char *login;
	login = getlogin();
	sprintf(db->caminhoDB, "/home/%s/.luof/", login);
	------------------------------------------------------------------*/
	strcpy(db->caminhoDB, ".luof/");

	strcpy(caminho, db->caminhoDB);

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

		//cria arquivo luof
		fSetaCaminhoArquivo(db, caminho, NULL);
		arquivo = fopen(caminho, "w");
		if (arquivo == NULL) {
			printf(ANSI_COLOR_RED "Erro: não foi possível criar banco de dados louf.\n");
			return 1;
		}

		//cria arquivo raiz
		fSetaCaminhoArquivo(db, caminho, "raiz");
		arquivo = freopen(caminho, "w", arquivo);
		if (arquivo == NULL) {
			printf(ANSI_COLOR_RED "Erro: não foi possível criar banco de dados raiz.\n");
			return 1;
		}

		printf(ANSI_BOLD_WHT "Banco de dados criado com sucesso.\n\n");
	}
	else {//se existe o DB
		//verifica a existencia do arquivo luof
		fSetaCaminhoArquivo(db, caminho, NULL);
		arquivo = fopen(caminho, "r");
		if (arquivo == NULL) {
			printf(ANSI_COLOR_RED "Erro: Não foi possível acessar o banco de dados luof.\n");
			return 1;
		}

		//verifica a existencia do arquivo raiz
		fSetaCaminhoArquivo(db, caminho, "raiz");
		arquivo = freopen(caminho, "r", arquivo);
		if (arquivo == NULL) {
			printf(ANSI_COLOR_RED "Erro: Não foi possível acessar o banco de dados raiz.\n");
			return 1;
		}
	}

	//preenche uma árvore com todas as categorias
	fPreencheArvoreCats(db);
	//fecha o arquivo e seta listaFavs como NULL
	fclose(arquivo);
	db->listaFavs = NULL;

	return 0;

}

void fFinalizaDB_private(sCat *listaCategorias) {

	sCat *cat;
	sIterador it;

	if (!emptyList(listaCategorias->catFilhos)) {
		it = criaIt(listaCategorias->catFilhos);
		do {
			cat = (struct sCat*) retornaItera(&it);
			fFinalizaDB_private(cat);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}
	//libera sua lista de categorias filhas
	freeList(listaCategorias->catFilhos);

}

void fFinalizaDB(sBanco *db) {

	//libera estruturas do banco
	if (db->arvoreCats)
		fFinalizaDB_private(db->arvoreCats);
	if (db->arvoreCats)
		free(db->arvoreCats);
	if (db->listaFavs)
		freeList(db->listaFavs);

}

void fPreencheArvoreCats(sBanco *db) {

	sCat c;//a categoria lida terá seus dados guardados aqui
	sCat *cPaiAtual;//aponta para a categoria que possui a lista onde a categoria lida será inserida
	char linhaCat[TAMNOMEFAV+1];//é somado 1 pois a linha lida contém o caracter '\n'
	char camLuof[TAMLINKARQ];//caminho do arquivo luof
	FILE *aLuof;

	//abre o arquivo onde está armazenada a árvore de categorias
	fSetaCaminhoArquivo(db, camLuof, NULL);
	aLuof = fopen(camLuof, "r");

	//seta os dados de db->listaCategorias (a categoria raiz)
	db->arvoreCats = malloc(sizeof(struct sCat));
	strcpy(db->arvoreCats->nome, "raiz");
	strcpy(db->arvoreCats->caminho, "/");
	db->arvoreCats->hie = -1;
	db->arvoreCats->catPai = NULL;
	db->arvoreCats->catFilhos = criaLista(struct sCat);

	//cPai passa a apontar para db->listaCategorias
	cPaiAtual = db->arvoreCats;

	//lê linha por linha e constroi a árvore de categorias
	while (fgets(linhaCat, 100, aLuof)) {

		//seta os dados da categoria lida (com excecão de catPai e caminho (este depende do anterior))
		strcpy(c.nome, &linhaCat[1]);
		c.nome[strlen(c.nome) - 1] = '\0';
		c.hie = linhaCat[0] - 48;
		c.catFilhos = criaLista(struct sCat);

		if (c.hie == cPaiAtual->hie + 1) {//se a cat é da mesma hierarquia da última cat lida
			c.catPai = cPaiAtual;
			strcpy(c.caminho, c.catPai->caminho);
			fSetaCaminhoCategoria(c.caminho, c.nome);
			pushBackList(cPaiAtual->catFilhos, &c);
		}
		else if (c.hie == cPaiAtual->hie + 2) {//se a cat é na verdade filha da última cat lida
			cPaiAtual = backList(cPaiAtual->catFilhos);
			c.catPai = cPaiAtual;
			strcpy(c.caminho, c.catPai->caminho);
			fSetaCaminhoCategoria(c.caminho, c.nome);
			pushBackList(cPaiAtual->catFilhos, &c);
		}
		else {//se a cat é de uma hierarquia menor da última cat lida
			for (int i = cPaiAtual->hie; i > c.hie; i--)
				cPaiAtual = cPaiAtual->catPai;
			cPaiAtual = cPaiAtual->catPai;
			c.catPai = cPaiAtual;
			strcpy(c.caminho, c.catPai->caminho);
			fSetaCaminhoCategoria(c.caminho, c.nome);
			pushBackList(cPaiAtual->catFilhos, &c);
		}

	}

	fclose(aLuof);

}

void fEscreveLuof_private(FILE *aLuof, sLista listaCategorias, int hierarquia) {

	sCat *cat;
	sIterador it;

	if (!emptyList(listaCategorias)) {
		it = criaIt(listaCategorias);
		do {
			cat = (struct sCat*) retornaItera(&it);
			//escreve no arquivo
			fprintf(aLuof, "%d%s\n", hierarquia, cat->nome);
			//------------------
			fEscreveLuof_private(aLuof, cat->catFilhos, hierarquia+1);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

}

void fEscreveLuof(sBanco *db) {

	char camLuof[TAMLINKARQ];
	FILE *aLuof;

	//Abre o arquivo luof para sobreescreve-lo
	fSetaCaminhoArquivo(db, camLuof, NULL);
	aLuof = fopen(camLuof, "w");

	//escreve a árvore de categorias no arquivo luof
	fEscreveLuof_private(aLuof, db->arvoreCats->catFilhos, 0);

	fclose(aLuof);

}





//dbcat

int fPreencheListaSite(sBanco *db, sCat *c, int cmp) {

	sSite siteTemp;
	char nomeTemp[TAMNOMEFAV], nomeArqCat[TAMLINKARQ];
	FILE *arqCat;

	//abre o arquivo da categoria
	fSetaCaminhoArquivo(db, nomeArqCat, c->nome);
	arqCat = fopen(nomeArqCat, "r");

	//documento não encontrado
	if (arqCat == NULL) {
		printf(ANSI_COLOR_RED "\nErro: O arquivo %s não pode ser aberto.\n", nomeArqCat);
		return 1;
	}

	db->listaFavs = criaLista(struct sSite);

	//pega linha por linha do arquivo da categoria, faz a comparação usando a primeira linha de um site, e depois pega as outras três referentes ao mesmo site
	while (fgets(nomeTemp, 100, arqCat) != NULL) {
		siteTemp = fRecuperaFavorito(arqCat, nomeTemp);
		//adiciona na lista apenas se pertencer a categoria correta
		if (cmp == 1 && strcmp(siteTemp.categoria, c->caminho) == 0)
			pushBackList(db->listaFavs, &siteTemp);
	}

	fclose(arqCat);

	return 0;

}

sSite fRecuperaFavorito(FILE *arq, char *nomeT) {

	sSite s;

	//verifica se foi passado o nome do site, ou se ele ainda não foi lido
	if (nomeT) {
		strncpy(s.nome, nomeT, strlen(nomeT));
		s.nome[strlen(nomeT)-1] = '\0';
	}
	else {
		fgets(s.nome, TAMCAMINHO, arq);
		s.nome[strlen(s.nome)-1] = '\0';
	}

	fgets(s.categoria, TAMCAMINHO, arq);
	s.categoria[strlen(s.categoria)-1] = '\0';

	fgets(s.link, TAMLINKARQ, arq);
	s.link[strlen(s.link)-1] = '\0';

	fgets(s.texto, TAMTEXTO, arq);
	if (s.texto[strlen(s.texto)-1] == '\n')
	s.texto[strlen(s.texto)-1] = '\0';

	return s;

}

int fBuscaCat(sBanco *db, char caminho[], sCat **c) {

	//se a categoria é a raiz
	if (strcmp(caminho, "/") == 0) {
		*c = db->arvoreCats;
		return 1;
	}

	//se ainda não tiver sido criado nenhuma categoria
	if (emptyList(db->arvoreCats->catFilhos)) {
		printf(ERRO);
		printf("Categoria \"%s\" não encontrada.\n", caminho);
		return 1;
	}

	//variaveis usadas para percorrer a lista de categorias
	sIterador it = criaIt(db->arvoreCats->catFilhos);
	sCat *cat;

	//busca a categoria na árvore de categorias e c passa a ser igual a ela
	do {
		cat = (struct sCat*) retornaItera(&it);
		if (strcmp(caminho, cat->caminho) == 0) {//encontrou a categoria
			*c = cat;
			return 0;
		}
		else if (strncmp(caminho, cat->caminho, strlen(cat->caminho)) == 0) {//encontrou uma categoria na hierarquia de cats da cat buscada
			if (emptyList(cat->catFilhos)) {//a lista de categorias filhas está vazia
				printf(ERRO);
				printf("Categoria \"%s\" não encontrada.\n", caminho);
				return 1;
			}
			it = criaIt(cat->catFilhos);
		}
		else {
			iteraProximo(&it);
			if (inicioIt(&it)) {//se voltou para o inicio da lista significa que a cat não foi encontrada
				printf(ERRO);
				printf("Categoria \"%s\" não encontrada.\n", caminho);
				return 1;
			}
		}
	} while (1);

}

void fEscreveArquivoCat(sBanco *db, char *nomeCat) {

	sSite *siteDoIterador;
	sIterador it;
	char nomeArqCat[TAMLINKARQ];
	FILE *arqCat;

	//abre o arquivo da categoria para escrita
	fSetaCaminhoArquivo(db, nomeArqCat, nomeCat);
	arqCat = fopen(nomeArqCat, "w");

	if (!emptyList(db->listaFavs)) {
		//escreve a lista no arquivo da categoria
		it = criaIt(db->listaFavs);
		do {
			siteDoIterador = (struct sSite*) retornaItera(&it);
			fprintf(arqCat, "%s\n%s\n%s\n%s\n", siteDoIterador->nome, siteDoIterador->categoria, siteDoIterador->link, siteDoIterador->texto);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

	fclose(arqCat);

}

int fBuscaFavorito(sBanco *db, sSite *s) {

	if (emptyList(db->listaFavs))
		return 0;

	//variaveis para percorrer a lista
	sIterador it = criaIt(db->listaFavs);
	sSite *favorito;

	do {
		favorito = (struct sSite*) retornaItera(&it);
		//encontrou o favorito
		if ((strcmp(favorito->nome, s->nome) == 0) && (strcmp(favorito->categoria, s->categoria) == 0)) {
			//seta os outros valores de s
			strcpy(s->link, favorito->link);
			strcpy(s->texto, favorito->texto);
			//informa que o site foi encontrado
			return 1;
		}
		iteraProximo(&it);
	} while (!inicioIt(&it));

	//não encontrou o site
	return 0;

}

void fAdicionaFavorito(sBanco *db, sSite s) {
}




//modulos

void fSetaCaminhoArquivo(sBanco *db, char *arq, char *nome) {
	strcpy(arq, db->caminhoDB);
	if (nome) {
		strcat(arq, nome);
		strcat(arq, ".luof");
	}
	else {
		strcat(arq, "luof");
	}
}

void fSetaCaminhoCategoria(char caminho[], char nome[]) {
	if (strcmp(caminho, "/") == 0) {
		strcpy(caminho, nome);
	}
	else {
		strcat(caminho, "/");
		strcat(caminho, nome);
	}
}
