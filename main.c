#include "luof.h"

int main(int argc, char *argv[]) {

	if (argc == 1) {
		//fMenu();
		printf("Função ainda não implementada\n");
	}
	else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		if (argc > 2)
			fHelp(argv[2]);
		else
			fHelp(NULL);
		//printf("Função ainda não implementada\n");
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
	else if (strcmp(argv[1], "-lt") == 0 || strcmp(argv[1], "--list-tree") == 0) {
		fListTree();
	}
	else if (strcmp(argv[1], "-lc") == 0 || strcmp(argv[1], "--list-category") == 0) {
		fListCategory(0);
	}
	else if (strcmp(argv[1], "-lcs") == 0 || strcmp(argv[1], "--list-category-short") == 0) {
		fListCategory(1);
	}
	else if (strcmp(argv[1], "-mb") == 0 || strcmp(argv[1], "--modify-bookmark") == 0) {
		fModifyBookmark();
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
