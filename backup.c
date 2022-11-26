#include "luof.h"

void fBackup_escreveArvore(FILE *arqCat, sLista listaCategorias, int hierarquia) {

	sCat *cat;
	sIterador it;

	if (!emptyList(listaCategorias)) {
		it = criaIt(listaCategorias);
		do {
			cat = (struct sCat*) retornaItera(&it);
			//escreve no arquivo
			fprintf(arqCat, "%d%s\n", hierarquia, cat->nome);
			//------------------
			fBackup_escreveArvore(arqCat, cat->catFilhos, hierarquia+1);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

}

void fBackup_preencnheListaCats(sLista listaCats, sCat *cat) {

	sCat *catF;
	sIterador it;
	int encontrou = 0;

	//percorre a listaCats, se não houver nenhuma categoria com o mesmo nome de cat, adiciona ela na lista
	if (!emptyList(listaCats)) {
		it = criaIt(listaCats);
		do {
			catF = (struct sCat*) retornaItera(&it);
			if (strcmp(cat->nome, catF->nome) == 0)
				encontrou = 1;
			iteraProximo(&it);
		} while (encontrou == 0 && !inicioIt(&it));
	}
	if (encontrou == 0) {
		pushBackList(listaCats, cat);
	}

	//faz a recursão para as categorias filhas de cat
	if (!emptyList(cat->catFilhos)) {
		it = criaIt(cat->catFilhos);
		do {
			catF = (struct sCat*) retornaItera(&it);
			fBackup_preencnheListaCats(listaCats, catF);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

}

char* fBackup_criar(sBanco *db) {

	sSite *siteDoIterador;
	sCat *cat;
	sLista listaCats;
	sIterador it, it2;
	char nomeBackup[TAMNOMEFAV], *nomeB;
	FILE *arqBackup;

	//preenche a listaCats com todas as categorias sem repetir categorias com o mesmo nome
	listaCats = criaLista(struct sCat);
	if (!emptyList(db->listaCategorias->catFilhos)) {
		it = criaIt(db->listaCategorias->catFilhos);
		do {
			cat = (struct sCat*) retornaItera(&it);
			fBackup_preencnheListaCats(listaCats, cat);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

	//cria arquivo do backup
	strcpy(nomeBackup, "backup.luof");
	arqBackup = fopen(nomeBackup, "r");
	//faz um loop para não haver conflito entre backup's caso já exista algum no diretório atual
	for (int i = 1; arqBackup; i++) {
		sprintf(nomeBackup, "backup%d.luof", i);
		arqBackup = freopen(nomeBackup, "r", arqBackup);
	}
	arqBackup = fopen(nomeBackup, "w");
	if (!arqBackup)
		return NULL;

	//escreve a árvore de categorias no arquivo
	fBackup_escreveArvore(arqBackup, db->listaCategorias->catFilhos, 0);

	//marca o fim da árvore de categorias
	fprintf(arqBackup, "##\n");

	//escreve a raiz no arquivo
	fprintf(arqBackup, "luof/%ld\n", sizeList(db->raiz));
	if (!emptyList(db->raiz)) {
		it = criaIt(db->raiz);
		do {
			siteDoIterador = (struct sSite*) retornaItera(&it);
			fprintf(arqBackup, "%s\n%s\n%s\n%s\n%c\n", siteDoIterador->nome, siteDoIterador->categoria, siteDoIterador->link, siteDoIterador->texto, siteDoIterador->ehCat);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

	//escreve os demais arquivos
	if (!emptyList(listaCats)) {
		it = criaIt(listaCats);
		do {
			cat = (struct sCat*) retornaItera(&it);
			if (fPreencheListaSite(db, cat)) {
				fclose(arqBackup);
				remove(nomeBackup);
				return NULL;
			}
			//escreve os sites do arquivo
			fprintf(arqBackup, "%s/%ld\n", cat->nome, sizeList(db->listaSites));
			if (!emptyList(db->listaSites)) {
				it2 = criaIt(db->listaSites);
				do {
					siteDoIterador = (struct sSite*) retornaItera(&it2);
					fprintf(arqBackup, "%s\n%s\n%s\n%s\n%c\n", siteDoIterador->nome, siteDoIterador->categoria, siteDoIterador->link, siteDoIterador->texto, siteDoIterador->ehCat);
					iteraProximo(&it2);
				} while (!inicioIt(&it2));
			}
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

	//fecha o arquivo e retorna seu nome
	fclose(arqBackup);
	nomeB = malloc(sizeof(char)*strlen(nomeBackup));
	strcpy(nomeB, nomeBackup);
	return nomeB;

}

void fBackup_excluirArquivos(sBanco *db) {

	sCat *cat;
	sLista listaCats;
	sIterador it;
	char nomeArqCat[TAMLINKARQ];

	//preenche a listaCats com todas as categorias sem repetir categorias com o mesmo nome
	listaCats = criaLista(struct sCat);
	if (!emptyList(db->listaCategorias->catFilhos)) {
		it = criaIt(db->listaCategorias->catFilhos);
		do {
			cat = (struct sCat*) retornaItera(&it);
			fBackup_preencnheListaCats(listaCats, cat);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

	//remove todos os arquivos usando o nome das categorias
	it = criaIt(listaCats);
	while (!emptyList(listaCats)) {
		cat = (struct sCat*) retornaItera(&it);
		fSetaCaminhoArquivo(db, nomeArqCat, cat->nome);
		remove(nomeArqCat);
		removeIt(&it);
	}

}

int fBackup_separaNomeQuantidade(char *nomeQtdArq, char *nomeArq) {

	int i = 0;
	int qtdSites;

	while (nomeQtdArq[i] != '/')
		i++;

	strncpy(nomeArq, nomeQtdArq, i);
	nomeArq[i] = '\0';
	qtdSites = atoi(&nomeQtdArq[i+1]);

	return qtdSites;

}

void fBackup_adicionaSite(FILE *arq, sLista l) {

	sSite s;
	char ehCategoria[3];

	fgets(s.nome, TAMCAMINHO, arq);
	s.nome[strlen(s.nome)-1] = '\0';

	fgets(s.categoria, TAMCAMINHO, arq);
	s.categoria[strlen(s.categoria)-1] = '\0';

	fgets(s.link, TAMLINKARQ, arq);
	s.link[strlen(s.link)-1] = '\0';

	fgets(s.texto, TAMTEXTO, arq);
	s.texto[strlen(s.texto)-1] = '\0';

	fgets(ehCategoria, 3, arq);
	s.ehCat = ehCategoria[0];

	pushBackList(l, &s);

}

void fBackup_restaurar(sBanco *db, FILE *arqBackup) {

	char nomeQtdArq[TAMNOMEFAV+10], nomeArq[TAMNOMEFAV];
	int qtdSites;

	//recupera a árvore de categorias
	fclose(db->aLuof);
	fLiberaCats(db->listaCategorias);
	db->aLuof = arqBackup;
	fPreencheListaCat(db);
	arqBackup = db->aLuof;
	db->aLuof = NULL;

	//recupera a raiz
	freeList(db->raiz);
	db->raiz = criaLista(struct sSite);
	fgets(nomeQtdArq, 100, arqBackup);
	qtdSites = fBackup_separaNomeQuantidade(nomeQtdArq, nomeArq);

	for (int i = 0; i < qtdSites; i++)
		fBackup_adicionaSite(arqBackup, db->raiz);

	//escreve o arquivo luof
	fEscreveLuof(db);
	fclose(db->aLuof);
	db->aLuof = NULL;

	//escreve os demais arquivos
	while(fgets(nomeQtdArq, 100, arqBackup) != NULL) {

		if (db->listaSites)
			freeList(db->listaSites);

		db->listaSites = criaLista(struct sSite);
		qtdSites = fBackup_separaNomeQuantidade(nomeQtdArq, nomeArq);

		for (int i = 0; i < qtdSites; i++)
			fBackup_adicionaSite(arqBackup, db->listaSites);

		fEscreveArquivoCat(db, nomeArq);

		fclose(db->aCat);
		db->aCat = NULL;

	}

}

void fBackup() {

	char *nomeBackup, caminhoBackup[TAMLINKARQ];
	int opcao;
	sBanco db;
	FILE *arqBackup;

	if (fInicializaDB(&db))
		return;

	//pergunta ao usuário o que ele deseja fazer
	printf("Você deseja criar um backup novo ou restaurar um antigo? [1]criar [2]restaurar [3]sair : ");
	scanf(" %d", &opcao);

	//se o usuário não desejar fazer nada
	if (opcao < 1 || opcao > 2) {
		printf("\nSaindo...\n");
		fFinalizaDB(&db);
		return;
	}
	else if (opcao == 1) {

		//cria o arquivo do backup no diretório atual
		nomeBackup = fBackup_criar(&db);

		//retorna mensagem de sucesso ou erro
		if (!nomeBackup) {
			printf("\nErro ao tentar criar backup.\n");
			printf("Saindo...\n");
		}
		else {
			printf("\nArquivo %s adicionado no diretório atual.\n", nomeBackup);
			printf("Backup criado com sucesso.\n");
			free(nomeBackup);
		}

	}
	else {

		//pede ao usuário o caminho do backup
		printf("Informe o caminho do arquivo de backup: ");
		scanf(" %[^\n]", caminhoBackup);

		//abre o arquivo
		arqBackup = fopen(caminhoBackup, "r");

		//retorna mensagem de erro ou continua a restauração
		if (!arqBackup) {
			printf("\nErro ao tentar abrir o arquivo de backup.\n");
			printf("Saindo...\n");
		}
		else {
			//apaga os arquivos já existentes no banco
			fBackup_excluirArquivos(&db);
			//restaura o backup
			fBackup_restaurar(&db, arqBackup);

			printf("\nBackup restaurado com sucesso.\n");
		}

	}

	//fecha os arquivos abertos
	fFinalizaDB(&db);

}
