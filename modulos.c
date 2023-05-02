#include "luof.h"

sCom fSetaArgumentos(void (**funcao)(sCom com), int argc, char *argv[]) {

	sCom com;
	int args[2] = {0,0};//args[0] se foi passado flag, args[1] se foi passado caminho

	com.flag[0] = '\0';
	com.caminho[0] = '\0';

	if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		*funcao = fHelp;
		if (argc > 2)//foi passada uma flag
			args[0] = 1;
	}
	else if (strcmp(argv[1], "-ab") == 0 || strcmp(argv[1], "--add-bookmark") == 0) {
		*funcao = fAddBookmark;
		if (argc > 2)//foi passado um caminho
			args[1] = 1;
	}
	else if (strcmp(argv[1], "-ac") == 0 || strcmp(argv[1], "--add-category") == 0) {
		*funcao = fAddCategory;
		if (argc > 2)//foi passado um caminho
			args[1] = 1;
	}
	else if (strcmp(argv[1], "-rb") == 0 || strcmp(argv[1], "--remove-bookmark") == 0) {
		*funcao = fRemoveBookmark;
		if (argc > 2)//foi passado um caminho
			args[1] = 1;
	}
	else if (strcmp(argv[1], "-rc") == 0 || strcmp(argv[1], "--remove-category") == 0) {
		*funcao = fRemoveCategory;
		if (argc > 2)//foi passado um caminho
			args[1] = 1;
	}
	else if (strcmp(argv[1], "-mb") == 0 || strcmp(argv[1], "--modify-bookmark") == 0) {
		*funcao = fModifyBookmark;
		if (argc > 2)//foi passado um caminho
			args[1] = 1;
	}
	else if (strcmp(argv[1], "-mc") == 0 || strcmp(argv[1], "--modify-category") == 0) {
		*funcao = fModifyCategory;
		if (argc > 2)//foi passado um caminho
			args[1] = 1;
	}
	else if (strcmp(argv[1], "-lc") == 0 || strcmp(argv[1], "--list-category") == 0) {
		*funcao = fListCategory;
		if (argc > 2 && argv[2][0] == '-')//foi passado uma flag
			args[0] = 1;
		if ((argc > 2 && argv[2][0] != '-') || argc > 3)//foi passado um caminho
			args[1] = 1;
	}
	else if (strcmp(argv[1], "-lcs") == 0 || strcmp(argv[1], "--list-category-short") == 0) {
		*funcao = fListCategory;
		strcpy(com.flag, "-s");//seta a flag
		if (argc > 2)//foi passado um caminho
			args[1] = 1;
	}
	else if (strcmp(argv[1], "-lt") == 0 || strcmp(argv[1], "--list-tree") == 0) {
		*funcao = fListTree;
		if (argc > 2 && argv[2][0] == '-')//foi passado uma flag
			args[0] = 1;
		if ((argc > 2 && argv[2][0] != '-') || argc > 3)//foi passado um caminho
			args[1] = 1;
	}
	else if (strcmp(argv[1], "-lts") == 0 || strcmp(argv[1], "--list-tree-short") == 0) {
		*funcao = fListTree;
		strcpy(com.flag, "-s");//seta a flag
		if (argc > 2)//foi passado um caminho
			args[1] = 1;
	}
	else if (strcmp(argv[1], "--backup") == 0) {
		*funcao = fBackup;
		if (argc > 2 && argv[2][0] == '-')//foi passado uma flag
			args[0] = 1;
		if ((argc > 2 && argv[2][0] != '-') || argc > 3)//foi passado um caminho
			args[1] = 1;
	}
	else if (strcmp(argv[1], "--export") == 0) {
		*funcao = fExport;
		if (argc > 2 && argv[2][0] == '-')//foi passado uma flag
			args[0] = 1;
		if ((argc > 2 && argv[2][0] != '-') || argc > 3)//foi passado um caminho
			args[1] = 1;
	}
	else if (strcmp(argv[1], "--import") == 0) {
		*funcao = fImport;
		if (argc > 2 && argv[2][0] == '-')//foi passado uma flag
			args[0] = 1;
		if ((argc > 2 && argv[2][0] != '-') || argc > 3)//foi passado um caminho
			args[1] = 1;
	}
	else if (strcmp(argv[1], "--free") == 0) {
		*funcao = fFree;
		if (argc > 2)//foi passado uma flag
			args[0] = 1;
	}
	else {
		printf(ANSI_BOLD_WHT "Função inválida\n");
	}

	//seta qual a flag passada e qual o caminho (se foram passados)
	if (args[0] && args[1]) {
		strcpy(com.flag, argv[2]);
		if (fSetaCaminho(&com, 3, argc, argv))
			*funcao = NULL;
	}
	else if (args[0] && !args[1]) {
		strcpy(com.flag, argv[2]);
	}
	else if (!args[0] && args[1]) {
		if (fSetaCaminho(&com, 2, argc, argv))
			*funcao = NULL;
	}

	return com;

}

int fSetaCaminho(sCom *com, int ini, int fim, char *argv[]) {

	int cont = 0;

	//verifica se o caminho passado não é maior que o máximo aceito
	for (int i = ini, j = 0; i < fim; i++) {
		j += strlen(argv[i]);
		if (j >= (TAMCAMINHO + TAMNOMEFAV)) {
			printf(ERRO);
			printf("O caminho tem mais caracteres do que o máximo permitido.\n");
			return 1;
		}
	}

	//tira as / no início caso ela não seja o único caractere
	if (strcmp(argv[ini], "/") != 0) {
		while (argv[ini][cont] == '/')
			cont++;
	}
	strcpy(com->caminho, &argv[ini][cont]);

	//seta o argumento em s.categoria
	for (int i = ini+1; i < fim; i++) {
		strcat(com->caminho, " ");
		strcat(com->caminho, argv[i]);
	}

	//retira as / a mais no início e no fim caso ela não seja o único caractere
	if (strcmp(com->caminho, "/") != 0 )
	while (com->caminho[strlen(com->caminho)-1] == '/')
		com->caminho[strlen(com->caminho)-1] = '\0';

	return 0;

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
