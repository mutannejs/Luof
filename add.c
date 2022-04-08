#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "luof.h"
#include "Corda/corda.h"

void fAddSite(int argc, char *argv[]) {

	site s;
	sqlite3 *db;
	identificadores id;
	int nome = 0, cat = 0, link = 0, text = 0;
	char *insertDB;

	// inicializa o banco de dados
	int rInicializaDB = fInicializaDB(&db, &id);
	if (rInicializaDB)
		return;

	//verifica se foi passado algum argumento errado, se foi passado mais de um argumento para o mesmo objeto, e guarda os argumentos passados corretamente
	for (int i = 2; i < argc; i++) {
		if (strncmp(argv[i], "n=", 2) == 0) {
			nome++;
			copiaString(&s.nome, argv[i], 2, 0);
		}
		else if (strncmp(argv[i], "c=", 2) == 0) {
			cat++;
			copiaString(&s.categoria, argv[i], 2, 0);
		}
		else if (strncmp(argv[i], "l=", 2) == 0) {
			link++;
			copiaString(&s.link, argv[i], 2, 0);
		}
		else if (strncmp(argv[i], "t=", 2) == 0) {
			text++;
			copiaString(&s.texto, argv[i], 2, 0);
		}
		else {
			printf("argumento desconhecido: %s\n", argv[i]);
			return;
		}
	}
	if (nome > 1 || cat > 1 || link > 1 || text > 1) {
		printf("Argumentos repetidos\n");
		return;
	}

	//Pede o nome e acategoria caso não tenham sido passados com argumento
	if (nome == 0) {
		printf("Nome:\n");
		leString(&s.nome);
	}
	if (cat == 0) {
		printf("Categoria:\n");
		leString(&s.categoria);
	}

	//verifica se já existe na categoria um favorito com esse nome e se a categoria existe

	//Pede o link e o comentario caso não tenham sido passados com argumento
	if (link == 0) {
		printf("Link:\n");
		leString(&s.link);
	}
	if (text == 0) {
		printf("Texto:\n");
		leString(&s.texto);
	}

	//adiciona no bd
	concatenaString(9, &insertDB, "INSERT INTO site VALUES (\"", s.nome, "\", \"", s.categoria, "\", \"", s.link, "\", \"", s.texto, "\");");
	printf("\n%s\n", insertDB);

	fFinalizaDB(db, id);
}

/*void fAddCategory() {

	char *categoria, *subcategoria;
	sqlite3 *db;

	int rInicializaDB = fInicializaDB(&db);
	if (rInicializaDB)
		return;

	printf("Categoria onde será adicionada a nova categoria:\n");
	leString(&categoria);
	printf("Nova categoria:\n");
	leString(&subcategoria);

	//verifica se já existe na categoria uma subcategoria com esse nome

	//adiciona no bd

	fFinalizaDB(db);
}*/
