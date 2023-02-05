#include "luof.h"

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

int fSetaSiteCategoria(sSite *s) {

	int cont = 0;
	char c;

	printf(ANSI_BOLD_WHT "Categoria : " ANSI_COLOR_GRA);

	do {
		c = getchar();
	} while (isspace(c));

	do {
		if (cont == 1 && s->categoria[0] == '/' && !isspace(c)) {
			cont--;
		}
		else if (cont == TAMCAMINHO - 1) {
			printf(ERRO);
			printf("Categorias devem ter no máximo %d caracteres.\n", TAMCAMINHO - 1);
			return 1;
		}
		s->categoria[cont] = c;
		cont++;
		c = getchar();
	} while (c != '\n');

	while (isspace(s->categoria[cont-1]))
		cont--;
	while (cont > 1 && s->categoria[cont-1] == '/')
		cont--;

	s->categoria[cont] = '\0';

	return 0;

}

int fSetaSiteNome(sSite *s) {

	int cont = 0;
	char c;

	printf(ANSI_BOLD_WHT "Nome      : " ANSI_COLOR_GRA);

	do {
		c = getchar();
	} while (isspace(c));

	do {
		if (cont == TAMNOMEFAV - 1) {
			printf(ERRO);
			printf("Nomes de favoritos devem ter no máximo %d caracteres.\n", TAMNOMEFAV - 1);
			return 1;
		}
		s->nome[cont] = c;
		cont++;
		c = getchar();
	} while (c != '\n');

	while (isspace(s->nome[cont-1]))
		cont--;

	s->nome[cont] = '\0';

	return 0;

}

int fSetaSiteLink(sSite *s) {

	int cont = 0;
	char c;

	printf(ANSI_BOLD_WHT "Link      : " ANSI_COLOR_GRA);

	do {
		c = getchar();
	} while (isspace(c));

	do {
		if (cont == TAMLINKARQ - 1) {
			printf(ERRO);
			printf("Links devem ter no máximo %d caracteres.\n", TAMLINKARQ - 1);
			return 1;
		}
		s->link[cont] = c;
		cont++;
		c = getchar();
	} while (c != '\n');

	while (isspace(s->link[cont-1]))
		cont--;

	s->link[cont] = '\0';

	return 0;

}

int fSetaSiteTexto(sSite *s) {

	int cont = 0;
	char c;

	printf(ANSI_BOLD_WHT "Texto     : " ANSI_COLOR_GRA);

	do {
		c = getchar();
	} while (isspace(c));

	do {
		if (cont == TAMTEXTO - 1) {
			printf(ERRO);
			printf("Textos devem ter no máximo %d caracteres.\n", TAMTEXTO - 1);
			return 1;
		}
		s->texto[cont] = c;
		cont++;
		c = getchar();
	} while (c != '\n');

	while (isspace(s->texto[cont-1]))
		cont--;

	s->texto[cont] = '\0';

	return 0;

}

int fSetaCatCategoria(sCat *cat) {

	int cont = 0;
	char c;

	printf(ANSI_BOLD_WHT "Categoria pai     : " ANSI_COLOR_GRA);

	do {
		c = getchar();
	} while (isspace(c));

	do {
		if (cont == 1 && cat->caminho[0] == '/' && !isspace(c)) {
			cont--;
		}
		else if (cont == TAMCAMINHO - 1) {
			printf(ERRO);
			printf("Categorias devem ter no máximo %d caracteres.\n", TAMCAMINHO - 1);
			return 1;
		}
		cat->caminho[cont] = c;
		cont++;
		c = getchar();
	} while (c != '\n');

	while (isspace(cat->caminho[cont-1]))
		cont--;
	while (cont > 1 && cat->caminho[cont-1] == '/')
		cont--;

	cat->caminho[cont] = '\0';

	return 0;

}

int fSetaCatNome(sCat *cat) {

	int cont = 0;
	char c;

	printf(ANSI_BOLD_WHT "Nome da categoria : " ANSI_COLOR_GRA);

	do {
		c = getchar();
	} while (isspace(c));

	do {
		if (c == '/') {
			printf(ERRO);
			printf("Categorias não podem ter '/' no nome.\n");
			return 1;
		}
		else if (cont == TAMNOMEFAV - 1) {
			printf(ERRO);
			printf("Nomes de categorias devem ter no máximo %d caracteres.\n", TAMNOMEFAV - 1);
			return 1;
		}
		cat->nome[cont] = c;
		cont++;
		c = getchar();
	} while (c != '\n');

	while (isspace(cat->nome[cont-1]))
		cont--;

	cat->nome[cont] = '\0';

	return 0;

}
