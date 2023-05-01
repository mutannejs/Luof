#include "luof.h"

sCom fSetaArgumentos(void (**funcao)(sCom com), int argc, char *argv[]) {

	sCom com;
	com.flag[0] = '\0';
	com.caminho[0] = '\0';

	if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		*funcao = fHelp;
		if (argc > 2)
			strcpy(com.flag, argv[2]);
	}
	else if (strcmp(argv[1], "-ab") == 0 || strcmp(argv[1], "--add-bookmark") == 0) {
		*funcao = fAddBookmark;
	}
	else if (strcmp(argv[1], "-ac") == 0 || strcmp(argv[1], "--add-category") == 0) {
		*funcao = fAddCategory;
	}
	else if (strcmp(argv[1], "-rb") == 0 || strcmp(argv[1], "--remove-bookmark") == 0) {
		*funcao = fRemoveBookmark;
	}
	else if (strcmp(argv[1], "-rc") == 0 || strcmp(argv[1], "--remove-category") == 0) {
		*funcao = fRemoveCategory;
	}
	else if (strcmp(argv[1], "-mb") == 0 || strcmp(argv[1], "--modify-bookmark") == 0) {
		*funcao = fModifyBookmark;
	}
	else if (strcmp(argv[1], "-mc") == 0 || strcmp(argv[1], "--modify-category") == 0) {
		*funcao = fModifyCategory;
	}
	else if (strcmp(argv[1], "-lc") == 0 || strcmp(argv[1], "--list-category") == 0) {
		*funcao = fListCategory;
		if (argc > 2 && argv[2][0] == '-') {//seta a flag
			if (strcmp(argv[2], "-s") == 0 || strcmp(argv[2], "--short") == 0)
				strcpy(com.flag, "-s");
			if (argc > 3)//seta o caminho
				fSetaCaminho(&com, 3, argc, argv);
		}
		else if (argc > 2)//seta o caminho
			fSetaCaminho(&com, 2, argc, argv);
	}
	else if (strcmp(argv[1], "-lcs") == 0 || strcmp(argv[1], "--list-category-short") == 0) {
		*funcao = fListCategory;
		strcpy(com.flag, "-s");
		if (argc > 2)
			fSetaCaminho(&com, 2, argc, argv);
	}
	else if (strcmp(argv[1], "-lt") == 0 || strcmp(argv[1], "--list-tree") == 0) {
		*funcao = fListTree;
		if (argc > 2 && (strcmp(argv[2], "-s") == 0 || strcmp(argv[2], "--short") == 0)) {
			strcpy(com.flag, "-s");
			if (argc > 3)//seta o caminho
				fSetaCaminho(&com, 3, argc, argv);
		}
		else if (argc > 2)//seta o caminho
			fSetaCaminho(&com, 2, argc, argv);
	}
	else if (strcmp(argv[1], "-lts") == 0 || strcmp(argv[1], "--list-tree-short") == 0) {
		*funcao = fListTree;
		strcpy(com.flag, "-s");
		if (argc > 2)
			fSetaCaminho(&com, 2, argc, argv);
	}
	else if (strcmp(argv[1], "--backup") == 0) {
		*funcao = fBackup;
	}
	else if (strcmp(argv[1], "--export") == 0) {
		*funcao = fExport;
	}
	else if (strcmp(argv[1], "--import") == 0) {
		*funcao = fImport;
	}
	else if (strcmp(argv[1], "--free") == 0) {
		*funcao = fFree;
	}
	else {
		printf(ANSI_BOLD_WHT "Função inválida\n");
	}

	return com;

}

void fSetaCaminho(sCom *com, int ini, int fim, char *argv[]) {

	//seta o argumento em s.categoria
	strcpy(com->caminho, argv[ini]);
	for (int i = ini+1; i < fim; i++) {
		strcat(com->caminho, " ");
		strcat(com->caminho, argv[i]);
	}
	//retira as / a mais no início e no fim
	while (com->caminho[0] == '/')
		strcpy(com->caminho, &com->caminho[1]);
	while (com->caminho[strlen(com->caminho)-1] == '/')
			com->caminho[strlen(com->caminho)-1] = '\0';

}

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

	printf(ANSI_BOLD_WHT "Descrição : " ANSI_COLOR_GRA);

	do {
		c = getchar();
	} while (isspace(c));

	do {
		if (cont == TAMTEXTO - 1) {
			printf(ERRO);
			printf("Descrições devem ter no máximo %d caracteres.\n", TAMTEXTO - 1);
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
