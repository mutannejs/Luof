/* ----------------------- Links Uteis ou Favoritos (luof) ---------------------
 *
 *	Sobre:
 *	Luof é um gerenciador de links, foi feito para armazenar informações
 *	de uma página como no "Favoritos" de um browser, tudo pelo terminal,
 *	sendo facíl de gerencia-las e modifica-las.
 *
 */

#include "luof.h"

int main(int argc, char *argv[]) {

	if (argc == 1) {
		//fMenu();
		printf("Função ainda não implementada\n");
	}
	else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		//fHelp();
		printf("Função ainda não implementada\n");
	}
	else if (strcmp(argv[1], "-as") == 0 || strcmp(argv[1], "--add-site") == 0) {
		fAddSite();
	}
	else if (strcmp(argv[1], "-ac") == 0 || strcmp(argv[1], "--add-category") == 0) {
		fAddCategory();
	}
	else if (strcmp(argv[1], "-ds") == 0 || strcmp(argv[1], "--delete-site") == 0) {
		fDeleteSite();
	}
	else if (strcmp(argv[1], "-dc") == 0 || strcmp(argv[1], "--delete-category") == 0) {
		fDeleteCategory();
	}
	else if (strcmp(argv[1], "-lt") == 0 || strcmp(argv[1], "--list-tree") == 0) {
		fListTree();
	}
	else if (strcmp(argv[1], "-lc") == 0 || strcmp(argv[1], "--list-category") == 0) {
		fListCategory(0);
	}
	else if (strcmp(argv[1], "-lcs") == 0 || strcmp(argv[1], "--list-category-short") == 0) {
		fListCategory(1);
	}
	else if (strcmp(argv[1], "-ms") == 0 || strcmp(argv[1], "--modify-site") == 0) {
		fModifySite();
	}
	else if (strcmp(argv[1], "-mc") == 0 || strcmp(argv[1], "--modify-category") == 0) {
		fModifyCategory();
	}
	else if (strcmp(argv[1], "-jc") == 0 || strcmp(argv[1], "--join-categorys") == 0) {
		//fJoinCategorys();
		printf("Função ainda não implementada\n");
	}
	else if (strcmp(argv[1], "--backup") == 0) {
		//fBackup();
		printf("Função ainda não implementada\n");
	}
	else if (strcmp(argv[1], "--export") == 0) {
		//fExport();
		printf("Função ainda não implementada\n");
	}	
	else if (strcmp(argv[1], "--html") == 0) {
		//fHtml();
		printf("Função ainda não implementada\n");
	}
	else {
		printf("Função inválida\n");
	}

	return 0;

}
