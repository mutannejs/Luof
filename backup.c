#include "luof.h"

void fBackup_preencnheListaCats(sLista listaCats, sCat *cat) {

	sCat *catTemp;
	sIterador it;
	int encontrou = 0;

	//percorre a listaCats, se não houver nenhuma categoria com o mesmo nome de cat, adiciona ela na lista
	if (emptyList(listaCats)) {
		pushBackList(listaCats, cat);
	}
	else {
		it = criaIt(listaCats);
		do {
			catTemp = (struct sCat*) retornaItera(&it);
			if (strcmp(catTemp->nome, cat->nome) == 0)//se a cat já existe
				encontrou = -1;
			else if (strcmp(catTemp->nome, cat->nome) > 0)//se encontrou a posicao a ser inserida
				encontrou = 1;
			else
				iteraProximo(&it);
		} while (encontrou == 0 && !inicioIt(&it));

		if (encontrou == 0)
			pushBackList(listaCats, cat);
		else if (encontrou > 0)
			insereAntIt(&it, cat);
	}

	//faz a recursão para as categorias filhas de cat
	if (!emptyList(cat->catFilhos)) {
		it = criaIt(cat->catFilhos);
		do {
			catTemp = (struct sCat*) retornaItera(&it);
			fBackup_preencnheListaCats(listaCats, catTemp);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

}

char* fBackup_criar(sBanco *db) {

	sSite *favorito;
	sCat *catTemp;
	sLista listaCats;
	sIterador it, it2;
	char nomeBackup[TAMLINKARQ], *nomeB;
	FILE *arqBackup;

	//cria arquivo do backup
	strcpy(nomeBackup, "luof.bkp");
	arqBackup = fopen(nomeBackup, "r");
	//faz um loop para não haver conflito entre backup's caso já exista algum no diretório atual
	for (int i = 1; arqBackup; i++) {
		sprintf(nomeBackup, "luof-%d.bkp", i);
		arqBackup = freopen(nomeBackup, "r", arqBackup);
	}
	arqBackup = fopen(nomeBackup, "w");
	if (!arqBackup)
		return NULL;

	//escreve a árvore de categorias no arquivo
	fEscreveLuof_private(arqBackup, db->arvoreCats->catFilhos, 0);

	//marca o fim da árvore de categorias
	fprintf(arqBackup, "##\n");

	//escreve todos os favoritos no arquivo
	listaCats = criaLista(struct sCat*);
	fBackup_preencnheListaCats(listaCats, db->arvoreCats);
	if (!emptyList(listaCats)) {
		it = criaIt(listaCats);
		do {
			catTemp = (struct sCat*) retornaItera(&it);
			fPreencheListaSite(db, catTemp, 0);
			if (!emptyList(db->listaFavs)) {
				fprintf(arqBackup, "%s/%ld\n", catTemp->nome, sizeList(db->listaFavs));
				it2 = criaIt(db->listaFavs);
				do {
					favorito = (struct sSite*) retornaItera(&it2);
					fprintf(arqBackup, "%s\n%s\n%s\n%s\n", favorito->nome, favorito->categoria, favorito->link, favorito->texto);
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

void fBackup_restaurar(sBanco *db, FILE *arqBackup) {

	sSite s;
	char nomeQtdArq[TAMNOMEFAV+10], nomeArq[TAMNOMEFAV];
	int qtdSites;

	//recupera a árvore de categorias e escreve o arquivo luof
	fFinalizaDB(db);
	fPreencheArvoreCats(db, arqBackup);
	fEscreveLuof(db);

	//escreve os demais arquivos
	while(fgets(nomeQtdArq, 100, arqBackup)) {

		if (db->listaFavs)
			freeList(db->listaFavs);

		db->listaFavs = criaLista(struct sSite);
		qtdSites = fBackup_separaNomeQuantidade(nomeQtdArq, nomeArq);

		for (int i = 0; i < qtdSites; i++) {
			s = fRecuperaFavorito(arqBackup, NULL);
			pushBackList(db->listaFavs, &s);
		}

		fEscreveArquivoCat(db, nomeArq);

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
	printf(ANSI_BOLD_WHT "Você deseja criar ou restaurar um backup? [1]criar [2]restaurar [3]sair : " ANSI_COLOR_GRA);
	scanf(" %d", &opcao);

	//se o usuário não desejar fazer nada
	if (opcao < 1 || opcao > 2) {
		printf(ANSI_BOLD_WHT "\nSaindo...\n");
		fFinalizaDB(&db);
		return;
	}
	else if (opcao == 1) {

		//cria o arquivo do backup no diretório atual
		nomeBackup = fBackup_criar(&db);

		//retorna mensagem de sucesso ou erro
		if (!nomeBackup) {
			printf(ERRO);
			printf("Erro ao tentar criar backup.\n");
			printf("Saindo...\n");
		}
		else {
			printf(ANSI_BOLD_WHT "\nArquivo %s adicionado no diretório atual.\n", nomeBackup);
			printf("Backup criado com sucesso.\n");
			free(nomeBackup);
		}

	}
	else {

		//pede ao usuário o caminho do backup
		printf(ANSI_BOLD_WHT "Informe o caminho do arquivo de backup: " ANSI_COLOR_GRA);
		scanf(" %[^\n]", caminhoBackup);

		//abre o arquivo
		arqBackup = fopen(caminhoBackup, "r");

		//retorna mensagem de erro ou continua a restauração
		if (!arqBackup) {
			printf(ERRO);
			printf("Erro ao tentar abrir o arquivo de backup.\n");
			printf("Saindo...\n");
		}
		else {
			//apaga os arquivos já existentes no banco
			fApagarBanco(&db);
			//restaura o backup
			fBackup_restaurar(&db, arqBackup);

			printf(ANSI_BOLD_WHT "\nBackup restaurado com sucesso.\n");
		}

	}

	//fecha os arquivos abertos
	fFinalizaDB(&db);

}
