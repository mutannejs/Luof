#include "luof.h"

int main(int argc, char *argv[]) {

	if (argc == 1) {
		fMenu();
	}
	else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		if (argc > 2)
			fHelp(argv[2]);
		else
			fHelp(NULL);
	}
	else if (strcmp(argv[1], "-ab") == 0 || strcmp(argv[1], "--add-bookmark") == 0) {
		fAddBookmark();
	}
	else if (strcmp(argv[1], "-ac") == 0 || strcmp(argv[1], "--add-category") == 0) {
		fAddCategory();
	}
	else if (strcmp(argv[1], "-rb") == 0 || strcmp(argv[1], "--remove-bookmark") == 0) {
		fRemoveBookmark();
	}
	else if (strcmp(argv[1], "-rc") == 0 || strcmp(argv[1], "--remove-category") == 0) {
		fRemoveCategory();
	}
	else if (strcmp(argv[1], "-mb") == 0 || strcmp(argv[1], "--modify-bookmark") == 0) {
		fModifyBookmark();
	}
	else if (strcmp(argv[1], "-mc") == 0 || strcmp(argv[1], "--modify-category") == 0) {
		fModifyCategory();
	}
	else if (strcmp(argv[1], "-lc") == 0 || strcmp(argv[1], "--list-category") == 0) {
		fListCategory(0, argc, argv);
	}
	else if (strcmp(argv[1], "-lcs") == 0 || strcmp(argv[1], "--list-category-short") == 0) {
		fListCategory(1, argc, argv);
	}
	else if (strcmp(argv[1], "-lt") == 0 || strcmp(argv[1], "--list-tree") == 0) {
		fListTree(0);
	}
	else if (strcmp(argv[1], "-lts") == 0 || strcmp(argv[1], "--list-tree-short") == 0) {
		fListTree(1);
	}
	else if (strcmp(argv[1], "--backup") == 0) {
		fBackup();
	}
	else if (strcmp(argv[1], "--export") == 0) {
		fExport();
	}
	else if (strcmp(argv[1], "--import") == 0) {
		fImport();
	}
	else if (strcmp(argv[1], "--free") == 0) {
		fFree();
	}
	else {
		printf(ANSI_BOLD_WHT "Função inválida\n");
	}

	return 0;

}

void fMenu() {

	sBanco db;
	int opcao;

	if (fInicializaDB(&db))
		return;
	fFinalizaDB(&db);

	printf(
	ANSI_BOLD_WHT "O que você deseja fazer?\n"
				  "1." ANSI_COLOR_WHT "  Adicionar um favorito\n"
	ANSI_BOLD_WHT "2." ANSI_COLOR_WHT "  Remover um favorito\n"
	ANSI_BOLD_WHT "3." ANSI_COLOR_WHT "  Modificar um favorito\n"
	ANSI_BOLD_WHT "4." ANSI_COLOR_WHT "  Adicionar uma categoria\n"
	ANSI_BOLD_WHT "5." ANSI_COLOR_WHT "  Remover uma categoria\n"
	ANSI_BOLD_WHT "6." ANSI_COLOR_WHT "  Modificar uma categoria\n"
	ANSI_BOLD_WHT "7." ANSI_COLOR_WHT "  Listar uma categoria\n"
	ANSI_BOLD_WHT "8." ANSI_COLOR_WHT "  Listar uma categoria de modo resumido\n"
	ANSI_BOLD_WHT "9." ANSI_COLOR_WHT "  Ver a árvore de categorias e favoritos\n"
	ANSI_BOLD_WHT "10." ANSI_COLOR_WHT " Ver a árvore de categorias\n"
	ANSI_BOLD_WHT "11." ANSI_COLOR_WHT " Criar ou restaurar um backup\n"
	ANSI_BOLD_WHT "12." ANSI_COLOR_WHT " Exportar uma categoria\n"
	ANSI_BOLD_WHT "13." ANSI_COLOR_WHT " Importar favoritos\n"
	ANSI_BOLD_WHT "14." ANSI_COLOR_WHT " Ver um manual de como usar o Luof\n"
	ANSI_BOLD_WHT "15." ANSI_COLOR_WHT " Sair\n\n"
	ANSI_BOLD_WHT "Opção: " ANSI_COLOR_GRA
	);

	do {
		scanf(" %d", &opcao);
		if (opcao < 1 || opcao > 15)
			printf(ANSI_BOLD_WHT "\nDigite apenas valores válidos.\nOpção: " ANSI_COLOR_GRA);
	} while (opcao < 1 || opcao > 15);
	printf("\n");

	switch (opcao) {
		case 1 :	fAddBookmark();
					break;
		case 2 :	fRemoveBookmark();
					break;
		case 3 :	fModifyBookmark();
					break;
		case 4 :	fAddCategory();
					break;
		case 5 :	fRemoveCategory();
					break;
		case 6 :	fModifyCategory();
					break;
		case 7 :	fListCategory(0, 2, NULL);
					break;
		case 8 :	fListCategory(1, 2, NULL);
					break;
		case 9 :	fListTree(0);
					break;
		case 10:	fListTree(1);
					break;
		case 11:	fBackup();
					break;
		case 12:	fExport();
					break;
		case 13:	fImport();
					break;
		case 14:	fHelp(NULL);
					break;
		default :	printf(ANSI_BOLD_WHT "Saindo...\n");
					break;
	};

}

void fFree() {

	sBanco db;
	char vBooleana;

	printf(ANSI_BOLD_WHT  "Você tem certeza que deseja apagar todas categorias e favoritos? [s/n]: " ANSI_COLOR_GRA);
	scanf(" %c", &vBooleana);
	if (vBooleana != 's') {
		printf(ANSI_BOLD_WHT "Saindo...\n");
		return;
	}

	if (fInicializaDB(&db))
		return;
	fApagarBanco(&db);

	printf(ANSI_BOLD_WHT "Categorias e favoritos apagados.\n");

}
