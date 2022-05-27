#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "Corda/corda.h"
#include "luof.h"

/* Lembrando que antes do programa estar em total funcionamento, tudo que será criado estará na pasta atual do programa, portanto, no final será necessário modificar todos os caminhos para o programa usar a home do usuario.
 * */
int fInicializaDB(FILE **aLuof) {

	char dir[20] = ".luof";//mudar para /home/$usuario/.luof
	char vBooleana, *errodbChar = 0;
	int errodbInt;
	struct stat st;

	if (stat(dir, &st) == -1) {//se ainda não existe o DB
		printf("Criar novo Banco de Dados? [s/n]: ");
		scanf(" %c", &vBooleana);
		limpaBuffer();
		if (vBooleana != 's') {
			printf("Saindo...\n");
			return 1;
		}

		//cria diretório .luof
		if (mkdir(dir, S_IRWXU) == -1) {
			printf("Erro: não foi possível criar diretório na home do usuário\n");
			return 1;
		}

		//cria banco de dados
		strcpy(dir, ".luof/luof");
		*aLuof = fopen(dir, "w");
		if (*aLuof == NULL) {
			printf("Erro: não foi possível criar banco de dados\n");
			return 1;
		}
	}
	else {//se existe o DB
		strcpy(dir, ".luof/luof");
		*aLuof = fopen(dir, "w");
		if (*aLuof == NULL) {
			printf("Erro: Não foi possível acessar o banco de dados\n");
			return 1;
		}
	}

	return 0;
}

void fFinalizaDB(FILE **aLuof, FILE **aCat) {
	
	fclose(*aLuof);
	fclose(*aCat);

}
