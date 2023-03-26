#include "luof.h"

int contaQtdCats(sCat *cat) {

	int qtd = 1;

	if (!emptyList(cat->catFilhos)) {
		sIterador it = criaIt(cat->catFilhos);
		do {
			sCat *catF = (struct sCat*) retornaItera(&it);
			qtd += contaQtdCats(catF);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

	return qtd;

}

int fBackup_preencnheListaCats(char **nomes, int *pos, sCat *cat) {

	sCat *catTemp;
	sIterador it;
	int encontrou = 0;

	//percorre a listaCats, se não houver nenhuma categoria com o mesmo nome de cat, adiciona seu nome na lista
	if (*pos == 0) {
		strcpy(nomes[0], cat->nome);
		*pos += 1;
	}
	else {
		for (int i = 0; i < *pos; i++) {
			if (strcmp(nomes[i], cat->nome) == 0)
				encontrou = 1;
		}
		if (!encontrou) {
			strcpy(nomes[*pos], cat->nome);
			*pos += 1;
		}
	}

	//faz a recursão para as categorias filhas de cat
	if (!emptyList(cat->catFilhos)) {
		it = criaIt(cat->catFilhos);
		do {
			catTemp = (struct sCat*) retornaItera(&it);
			fBackup_preencnheListaCats(nomes, pos, catTemp);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

	return *pos;

}

char* fBackup_criar(sBanco *db) {

	sSite *favorito;
	sCat catTemp;
	sIterador it;
	char nomeBackup[TAMLINKARQ], *nomeB;
	int qtdCats;
	FILE *arqBackup;

	//cria uma lista com os nomes de todas categorias, sem repetir o nome
	char **nomes;
	int qtdNomes = 0;
	qtdCats = contaQtdCats(db->arvoreCats);
	nomes = malloc(qtdCats * sizeof(char*));
	nomes[0] = malloc(qtdCats * TAMNOMEFAV * sizeof(char));
	for (int i = 1; i < qtdCats; i++)
		nomes[i] = nomes[0] + (i * TAMNOMEFAV);
	qtdNomes = fBackup_preencnheListaCats(nomes, &qtdNomes, db->arvoreCats);

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
	for (int i = 0; i < qtdNomes; i++) {
		strcpy(catTemp.nome, nomes[i]);
		fPreencheListaSite(db, &catTemp, 0);
		if (!emptyList(db->listaFavs)) {
			fprintf(arqBackup, "%s/%ld\n", catTemp.nome, sizeList(db->listaFavs));
			it = criaIt(db->listaFavs);
			do {
				favorito = (struct sSite*) retornaItera(&it);
				fprintf(arqBackup, "%s\n%s\n%s\n%s\n", favorito->nome, favorito->categoria, favorito->link, favorito->texto);
				iteraProximo(&it);
			} while (!inicioIt(&it));
		}
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
