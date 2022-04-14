/* ----------------------- Links Uteis ou Favoritos (luof) ---------------------
 *
 * Versão:
 * 	1.0.
 *
 * Sobre:
 * 	Luof é um gerencioador de links, foi feito para armazenar informações
 *	de uma página como no "Favoritos" de um browser, sendo facíl de
 * 	gerencia-las e modificalas.
 *
 * Criador:
 * 	Murillo J. S.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "luof.h"

int main(int argc, char *argv[]) {

	/*if (argc == 1) {
		fMenu();
	}
	else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		fHelp();
	}
	else*/ if (strcmp(argv[1], "-as") == 0 || strcmp(argv[1], "--add-site") == 0) {
		fAddSite();
	}
	/*else if (strcmp(argv[1], "-ac") == 0 || strcmp(argv[1], "--add-category") == 0) {
		fAddCategory(argc, argv);
	}
	else if (strcmp(argv[1], "-ds") == 0 || strcmp(argv[1], "--delete-site") == 0) {
		fDeleteSite();
	}
	else if (strcmp(argv[1], "-dc") == 0 || strcmp(argv[1], "--delete-category") == 0) {
		fDeleteCategory();
	}
	else if (strcmp(argv[1], "-la") == 0 || strcmp(argv[1], "--list-all") == 0) {
		fListAll();
	}
	else if (strcmp(argv[1], "-lc") == 0 || strcmp(argv[1], "--list-category") == 0) {
		fListCategory();
	}
	else if (strcmp(argv[1], "-sa") == 0 || strcmp(argv[1], "--see-all") == 0) {
		fSeeAll();
	}
	else if (strcmp(argv[1], "-sc") == 0 || strcmp(argv[1], "--see-category") == 0) {
		fSeeCategory();
	}
	else if (strcmp(argv[1], "-ss") == 0 || strcmp(argv[1], "--see-site") == 0) {
		fSeeSite();
	}
	else if (strcmp(argv[1], "-ms") == 0 || strcmp(argv[1], "--modify-site") == 0) {
		fModifySite();
	}
	else if (strcmp(argv[1], "-mc") == 0 || strcmp(argv[1], "--modify-category") == 0) {
		fModifyCategory();
	}
	else if (strcmp(argv[1], "--version") == 0) {
		fVersion();
	}
	else if (strcmp(argv[1], "--backup") == 0) {
		fBackup();
	}
	else if (strcmp(argv[1], "--export") == 0) {
		fExport();
	}	
	else if (strcmp(argv[1], "--html") == 0) {
		fHtml();
	}*/

	return 0;
}
