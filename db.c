#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sqlite3.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "luof.h"

int fInicializaDB(sqlite3 **db, identificadores *id) {

	char *user, dir[100], cr, *errodbChar = 0;
	int errodbInt;
	struct stat st;

	user = getlogin();
	if (user == NULL) {
		printf("Erro: não foi possível reconhecer o usuário\n");
		return 1;
	}

	sprintf(dir, "/home/%s/.luof", user);
	if (stat(dir, &st) == -1) {
		printf("Criar novo Banco de Dados? [s/n]:  ");
		scanf("%c", &cr);
		if (cr != 's') {
			printf("Saindo...\n");
			return 1;
		}

		//cria diretório .luof
		if (mkdir(dir, S_IRWXU) == -1) {
			printf("Erro: não foi possível criar diretório na home do usuário\n");
			return 1;
		}

		//cria banco de dados
		sprintf(dir, "/home/%s/.luof/luof.db", user);
		errodbInt = sqlite3_open(dir, db);
		if (errodbInt) {
			printf("Erro: não foi possível criar banco de dados - %s\n", sqlite3_errmsg(*db));
			sqlite3_close(*db);
			return 1;
		}

		//cria tabelas
		char criaTabelas[] = "CREATE TABLE site ( "
			"id INT PRIMARY KEY UNIQUE NOT NULL, "
			"nome TEXT, "
			"categoria INT, "
			"link TEXT, "
			"comentario TEXT);"
			" "
			"CREATE TABLE categoria ( "
			"id INT PRIMARY KEY UNIQUE NOT NULL, "
			"categoria TEXT, "
			"catPai INT);";
		errodbInt = sqlite3_exec(*db, criaTabelas, 0, 0, &errodbChar);
		if (errodbInt != SQLITE_OK) {
			printf("Erro: não foi possível criar as tabelas para o banco de dados: %s\n", errodbChar);
			sqlite3_free(errodbChar);
			return 1;
		}

		//inicia identificadores
		id->siteIni = 0,
		id->siteFim = 0,
		id->catIni = 0,
		id->catFim = 0;
		sprintf(dir, "/home/%s/.luof/ids.txt", user);
		FILE *ids = fopen(dir, "w");
		if (ids == NULL) {
			printf("Erro: não foi possível criar ids.txt\n");
			return 1;
		}
		fprintf(ids, "0\n0\n0\n0");
		fclose(ids);
	}
	else {
		sprintf(dir, "/home/%s/.luof/luof.db", user);
		errodbInt = sqlite3_open(dir, db);
		if (errodbInt) {
			printf("Erro: erro ao acessar bd - %s\n", sqlite3_errmsg(*db));
			sqlite3_close(*db);
			return 1;
		}

		sprintf(dir, "/home/%s/.luof/ids.txt", user);
		FILE *ids = fopen(dir, "r");
		fscanf(ids, "%d", &id->siteIni);
		fscanf(ids, "%d", &id->siteFim);
		fscanf(ids, "%d", &id->catIni);
		fscanf(ids, "%d", &id->catFim);
		fclose(ids);
	}

	return 0;
}

void fFinalizaDB(sqlite3 *db, identificadores id) {
	char dir[100], *user = getlogin();
	sprintf(dir, "/home/%s/.luof/ids.txt", user);
	FILE *ids = fopen(dir, "w");
	if (ids == NULL)
		printf("Erro: não foi possível escrever em ids.txt\n");
	else {
		fprintf(ids, "%d\n%d\n%d\n%d", id.siteIni, id.siteFim, id.catIni, id.catFim);
		fclose(ids);
	}
	sqlite3_close(db);
}

int fVerificaExistenciaDB(sqlite3 *db, site s) {
}

void fIncluiDB(sqlite3 *db, site s) {
}
