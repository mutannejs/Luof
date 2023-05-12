#include "luof.h"

int main(int argc, char *argv[]) {

	sCom com;
	void (*funcao)(sCom com) = NULL;

	if (argc == 1) {
		fMenu();
	}
	else {
		com = fSetaArgumentos(&funcao, argc, argv);
		if (funcao)
			funcao(com);
	}

	return 0;

}

void fMenu() {

	sCom com;
	sBanco db;
	int opcao;

	com.flag[0] = '\0';
	com.caminho[0] = '\0';

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
		case 1 :	fAddBookmark(com);
					break;
		case 2 :	fRemoveBookmark(com);
					break;
		case 3 :	fModifyBookmark(com);
					break;
		case 4 :	fAddCategory(com);
					break;
		case 5 :	fRemoveCategory(com);
					break;
		case 6 :	fModifyCategory(com);
					break;
		case 7 :	fListCategory(com);
					break;
		case 8 :	fListCategory(com);
					break;
		case 9 :	fListTree(com);
					break;
		case 10:	fListTree(com);
					break;
		case 11:	fBackup(com);
					break;
		case 12:	fExport(com);
					break;
		case 13:	fImport(com);
					break;
		case 14:	fHelp(com);
					break;
		default :	printf(ANSI_BOLD_WHT "Saindo...\n");
					break;
	};

}

void fFree(sCom com) {

	sBanco db;
	char vBooleana;

	if (strcmp(com.flag, "-y") != 0) {
		printf(ANSI_BOLD_WHT  "Você tem certeza que deseja apagar todas categorias e favoritos? [s/n]: " ANSI_COLOR_GRA);
		scanf(" %c", &vBooleana);
		if (vBooleana != 's') {
			printf(ANSI_BOLD_WHT "Saindo...\n");
			return;
		}
	}

	if (fInicializaDB(&db))
		return;
	fApagarBanco(&db);

	printf(ANSI_BOLD_WHT "Categorias e favoritos apagados.\n");

}
