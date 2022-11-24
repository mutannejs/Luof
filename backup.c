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
	FILE *arqCat;

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
	arqCat = fopen(nomeBackup, "r");
	//faz um loop para não haver conflito entre backup's caso já exista algum no diretório atual
	for (int i = 1; arqCat; i++) {
		sprintf(nomeBackup, "backup%d.luof", i);
		arqCat = freopen(nomeBackup, "r", arqCat);
	}
	arqCat = fopen(nomeBackup, "w");
	if (!arqCat)
		return NULL;

	//escreve a árvore de categorias no arquivo
	fBackup_escreveArvore(arqCat, db->listaCategorias->catFilhos, 0);

	//marca o fim da árvore de categorias
	fprintf(arqCat, "##\n");

	//escreve a raiz no arquivo
	fprintf(arqCat, "luof/%ld\n", sizeList(db->raiz));
	if (!emptyList(db->raiz)) {
		it = criaIt(db->raiz);
		do {
			siteDoIterador = (struct sSite*) retornaItera(&it);
			fprintf(arqCat, "%s\n%s\n%s\n%s\n%c\n", siteDoIterador->nome, siteDoIterador->categoria, siteDoIterador->link, siteDoIterador->texto, siteDoIterador->ehCat);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

	//escreve os demais arquivos
	if (!emptyList(listaCats)) {
		it = criaIt(listaCats);
		do {
			cat = (struct sCat*) retornaItera(&it);
			if (fPreencheListaSite(db, cat)) {
				fclose(arqCat);
				remove(nomeBackup);
				return NULL;
			}
			//escreve os sites do arquivo
			fprintf(arqCat, "%s/%ld\n", cat->nome, sizeList(db->listaSites));
			if (!emptyList(db->listaSites)) {
				it2 = criaIt(db->listaSites);
				do {
					siteDoIterador = (struct sSite*) retornaItera(&it2);
					fprintf(arqCat, "%s\n%s\n%s\n%s\n%c\n", siteDoIterador->nome, siteDoIterador->categoria, siteDoIterador->link, siteDoIterador->texto, siteDoIterador->ehCat);
					iteraProximo(&it2);
				} while (!inicioIt(&it2));
			}
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

	//fecha o arquivo e retorna seu nome
	fclose(arqCat);
	nomeB = malloc(sizeof(char)*strlen(nomeBackup));
	strcpy(nomeB, nomeBackup);
	return nomeB;

}

void fBackup() {

	char *nomeBackup;
	int opcao;
	sBanco db;

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
	}

	//fecha os arquivos abertos
	fFinalizaDB(&db);

}
