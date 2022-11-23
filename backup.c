#include "luof.h"

char* fBackup_criar(sBanco *db) {

	char nomeBackup[TAMNOMEFAV], *nomeB;
	FILE *arqCat;

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

	//escreve .luof no arquivo
	//TODO


	//escreve demais arquivos

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
