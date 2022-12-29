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
		fListCategory(0);
	}
	else if (strcmp(argv[1], "-lcs") == 0 || strcmp(argv[1], "--list-category-short") == 0) {
		fListCategory(1);
	}
	else if (strcmp(argv[1], "-lt") == 0 || strcmp(argv[1], "--list-tree") == 0) {
		fListTree();
	}
	else if (strcmp(argv[1], "--backup") == 0) {
		fBackup();
	}
	else if (strcmp(argv[1], "--export") == 0) {
		fExport();
	}
	else {
		printf("Função inválida\n");
	}

	return 0;

}

void fMenu() {

	int opcao;

	printf(
	"O que você deseja fazer?\n"
	"0.  Adicionar um favorito\n"
	"1.  Remover um favorito\n"
	"2.  Modificar um favorito\n"
	"3.  Adicionar uma categoria\n"
	"4.  Remover uma categoria\n"
	"5.  Modificar uma categoria\n"
	"6.  Listar uma categoria\n"
	"7.  Listar uma categoria de modo resumido\n"
	"8.  Ver a árvore de categorias e favoritos\n"
	"9.  Criar ou restaurar um backup\n"
	"10. Exportar uma categoria\n"
	"11. Ver um manual de como usar o Luof\n"
	"12. Sair\n\n"
	"Opção: "
	);

	do {
		scanf(" %d", &opcao);
		if (opcao < 0 || opcao > 12)
			printf("Digite apenas valores válidos.\nOpção: ");
	} while (opcao < 0 || opcao > 12);
	printf("\n");

	switch (opcao) {
		case 0 :	fAddBookmark();
					break;
		case 1 :	fRemoveBookmark();
					break;
		case 2 :	fModifyBookmark();
					break;
		case 3 :	fAddCategory();
					break;
		case 4 :	fRemoveCategory();
					break;
		case 5 :	fModifyCategory();
					break;
		case 6 :	fListCategory(0);
					break;
		case 7 :	fListCategory(1);
					break;
		case 8 :	fListTree();
					break;
		case 9 :	fBackup();
					break;
		case 10:	fExport();
					break;
		case 11:	fHelp(NULL);
					break;
		default :	printf("Saindo...\n");
					break;
	};

}
