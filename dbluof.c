#include "luof.h"

int fInicializaDB(sBanco *db) {

	char caminho[TAMLINKARQ], camLuof[TAMLINKARQ], vBooleana;
	struct stat st;
	FILE *aLuof, *arquivo;

	/* --- seta a variável caminhoDB com o caminho para o banco de dados --- */
	char *login;
	login = getlogin();
	sprintf(db->caminhoDB, "/home/%s/.luof/", login);

	/* --- comentar as três linhas a cima (10 a 12) e descomentar a de baixo (19),
	 * para usar o banco de dados no diretório do executável. Pode ser feito esse esquema
	 * ao usar o comando 'make compile' para gerar o executável e o banco de dados na
	 * própria pasta do projeto, assim, não será criado nenhum arquivo em outras pastas.
	 * --- */
	//strcpy(db->caminhoDB, ".luof/");

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
	fSetaCaminhoArquivo(db, camLuof, NULL);
	aLuof = fopen(camLuof, "r");
	fPreencheArvoreCats(db, aLuof);
	fclose(aLuof);
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

void fPreencheArvoreCats(sBanco *db, FILE *aLuof) {

	sCat c;//a categoria lida terá seus dados guardados aqui
	sCat *cPaiAtual;//aponta para a categoria que possui a lista onde a categoria lida será inserida
	char linhaCat[TAMNOMEFAV+1];//é somado 1 pois a linha lida contém o caracter '\n'

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
	while (fgets(linhaCat, 100, aLuof) && strcmp(linhaCat, "##\n") != 0) {

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

int fBuscaCat(sBanco *db, char caminho[], sCat **c) {

	//se a categoria é a raiz
	if (strcmp(caminho, "/") == 0) {
		*c = db->arvoreCats;
		return 0;
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

sCat* fBuscaCatFilha(sCat *catPai, char nome[]) {

	if (emptyList(catPai->catFilhos))
		return NULL;

	sCat *catTemp;
	sIterador it =criaIt(catPai->catFilhos);

	do {
		catTemp = (struct sCat*) retornaItera(&it);
		if (strcmp(catTemp->nome, nome) == 0)//encontrou a categoria
			return catTemp;
		else
			iteraProximo(&it);
	} while (!inicioIt(&it));

	//não encontrou a categoria
	return NULL;

}

int fInsereCategoria(sBanco *db, sCat *cPai, sCat c) {

	sCat *catTemp;
	sIterador it = criaIt(cPai->catFilhos);
	int encontrouPos = 0;
	char nomeArqCat[TAMLINKARQ];
	FILE *arqCat;//usado para criar um novo arquivo caso necessário

	//seta os novos dados da categoria
	strcpy(c.caminho, cPai->caminho);
	fSetaCaminhoCategoria(c.caminho, c.nome);
	c.hie = cPai->hie + 1;
	c.catPai = cPai;
	c.catFilhos = criaLista(struct sCat);

	//adiciona a categoria nova na árvore de categorias
	if (emptyList(cPai->catFilhos)) {//se a lista está vazia apenas insere a categoria no fim da lista
		pushBackList(cPai->catFilhos, &c);
	}
	else {//se a lista não estiver vazia, insere em ordem alfabética
		//procura a posição que a categoria deve ser inserida (antes da primeira categoria com o nome maior no quesito lexográfico)
		do {
			catTemp = (struct sCat*) retornaItera(&it);
			if (strcmp(catTemp->nome, c.nome) == 0)//se a cat já existe
				return 1;
			else if (strcmp(catTemp->nome, c.nome) > 0)//se encontrou a posicao a ser inserida
				encontrouPos = 1;
			else
				iteraProximo(&it);
		} while (encontrouPos == 0 && !inicioIt(&it));

		if (encontrouPos == 0)
			pushBackList(cPai->catFilhos, &c);
		else
			insereAntIt(&it, &c);
	}

	//a categoria foi inserida com sucesso
	return 0;

}

void fRemoveCategoria(sBanco *db, sCat *cat) {

	sCat *catTemp;
	sIterador it;

	//remove a categoria da árvore de categorias
	it = criaIt(cat->catPai->catFilhos);
	do {
		catTemp = (struct sCat*) retornaItera(&it);
		if (strcmp(catTemp->nome, cat->nome) == 0) {
			//remove suas subcategorias da árvore de categorias
			fFinalizaDB_private(cat);
			//remove a categoria da catFilhos da catPai
			removeIt(&it);
			return;
		}
		else {
			iteraProximo(&it);
		}
	} while (!inicioIt(&it));

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

void fRemoveArqCat(sBanco *db, sCat *cat) {

	//para a categoria que terá seus sites removidos
	sSite *favorito;
	sCat *catTemp;
	sIterador it;
	int qtdLista;
	char nomeArqCat[TAMLINKARQ];
	FILE *arqCat;

	//chama a mesma função para suas subcategorias
	if (!emptyList(cat->catFilhos)) {
		it = criaIt(cat->catFilhos);
		do {
			catTemp = (struct sCat*) retornaItera(&it);
			fRemoveArqCat(db, catTemp);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

	//verifica se o arquivo existe (se não existir apenas sai)
	fSetaCaminhoArquivo(db, nomeArqCat, cat->nome);
	arqCat = fopen(nomeArqCat, "r");
	if (!arqCat)
		return;

	//cria uma lista com todos os favoritos no arquivo da categoria
	fPreencheListaSite(db, cat, 0);

	//se a lista estiver vazia exclui o arquivo (com exceção da raiz)
	if (emptyList(db->listaFavs) && strcmp(cat->nome, "raiz") != 0) {
		fSetaCaminhoArquivo(db, nomeArqCat, cat->nome);
		remove(nomeArqCat);
		return;
	}

	//remove os sites da categoria na listaFavs
	it = criaIt(db->listaFavs);
	do {
		qtdLista = sizeList(db->listaFavs);
		favorito = (struct sSite*) retornaItera(&it);
		if (strcmp(favorito->categoria, cat->caminho) == 0)
			removeIt(&it);
		else
			iteraProximo(&it);
	} while (!emptyList(db->listaFavs) && (!inicioIt(&it) || qtdLista != sizeList(db->listaFavs)));

	//se a lista da categoria ficou vazia remove o arquivo (com exceção da raiz), caso contrário só atualiza o arquivo
	if (emptyList(db->listaFavs) && strcmp(cat->nome, "raiz") != 0) {
		fSetaCaminhoArquivo(db, nomeArqCat, cat->nome);
		remove(nomeArqCat);
	}
	else {
		fEscreveArquivoCat(db, cat->nome);
	}

}

void fApagarBanco_private(sBanco *db, sCat *cat) {

	sCat *catTemp;
	sIterador it;
	char nomeArqCat[TAMLINKARQ];
	FILE *arqCat;

	//remove o arquivo
	if (strcmp(cat->nome, "raiz") != 0) {
		fSetaCaminhoArquivo(db, nomeArqCat, cat->nome);
		arqCat = fopen(nomeArqCat, "r");
		if (arqCat)
			fclose(arqCat);
		remove(nomeArqCat);
	}

	//chama a mesma função para suas subcategorias
	if (!emptyList(cat->catFilhos)) {
		it = criaIt(cat->catFilhos);
		do {
			catTemp = (struct sCat*) retornaItera(&it);
			fApagarBanco_private(db, catTemp);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

}

void fApagarBanco(sBanco *db) {

	sCat *cat;
	sIterador it;
	char nomeArqCat[TAMLINKARQ];
	FILE *aLuof, *aRaiz;

	//remove todos os arquivos de categorias
	fApagarBanco_private(db, db->arvoreCats);

	//limpa o arquivo raiz e luof
	fSetaCaminhoArquivo(db, nomeArqCat, "raiz");
	aRaiz = fopen(nomeArqCat, "w");
	fclose(aRaiz);
	fSetaCaminhoArquivo(db, nomeArqCat, NULL);
	aLuof = fopen(nomeArqCat, "w");
	fclose(aLuof);

}
