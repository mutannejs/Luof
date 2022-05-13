#include <stdio.h>
#include <stdlib.h>
#include "luof.h"

void fHelp(int argc, char *argv[]) {

	if (argc == 2) {
		fHelpAll();
	}
	else if (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0) {
		fHelpH();
        }
        else if (strcmp(argv[2], "-as") == 0 || strcmp(argv[2], "--add-site") == 0) {
                fHelpAS();
        }
        else if (strcmp(argv[2], "-ac") == 0 || strcmp(argv[2], "--add-category") == 0) {
                fHelpAC();
        }
        else if (strcmp(argv[2], "-ds") == 0 || strcmp(argv[2], "--delete-site") == 0) {
                fHelpDS();
        }
        else if (strcmp(argv[2], "-dc") == 0 || strcmp(argv[2], "--delete-category") == 0) {
                fHelpDC();
        }
        else if (strcmp(argv[2], "-la") == 0 || strcmp(argv[2], "--list-all") == 0) {
                fHelpLA();
        }
        else if (strcmp(argv[2], "-lc") == 0 || strcmp(argv[2], "--list-category") == 0) {
                fHelpLC();
        }
        else if (strcmp(argv[2], "-sa") == 0 || strcmp(argv[2], "--see-all") == 0) {
                fHelpSA();
        }
        else if (strcmp(argv[2], "-sc") == 0 || strcmp(argv[2], "--see-category") == 0) {
                fHelpSC();
        }
        else if (strcmp(argv[2], "-ss") == 0 || strcmp(argv[2], "--see-site") == 0) {
                fHelpSS();
        }
        else if (strcmp(argv[2], "-ms") == 0 || strcmp(argv[2], "--modify-site") == 0) {
                fHelpMS();
        }
        else if (strcmp(argv[2], "-mc") == 0 || strcmp(argv[2], "--modify-category") == 0) {
                fHelpMC();
        }
        else if (strcmp(argv[2], "--version") == 0) {
                fHelpVersion();
        }
        else if (strcmp(argv[2], "--backup") == 0) {
                fHelpBackup();
        }
        else if (strcmp(argv[2], "--export") == 0) {
                fHelpExport();
        }
        else if (strcmp(argv[2], "--html") == 0) {
                fHelpHtml();
        }
}

void fHelpAll() {
}

void fHelpH() {
}

void fHelpAC() {
}

void fHelpAS() {
}

void fHelpDC() {
}

void fHelpDS() {
}

void fHelpLA() {
}

void fHelpLC() {
}

void fHelpSA() {
}

void fHelpSC() {
}

void fHelpSS() {
}

void fHelpMC() {
}

void fHelpMS() {
}

void fHelpVersion() {
}

void fHelpBackup() {
}

void fHelpExport() {
}

void fHelpHtml() {
}
