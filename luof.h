#ifndef LUOF_H
#define LUOF_H

#include <sqlite3.h>

// --- Structs ---
typedef struct site {
	char *nome;
	char *categoria;
	int idcat;
	char *link;
	char *texto;
} site;

typedef struct categoria {
	char *categoria;
	char *catPai;
} categoria;

typedef struct identificadores {//para falar qual será o id do próximo elemento (focando em sempre preencher os primeiros)
	int siteIni;
	int siteFim;
	int catIni;
	int catFim;
} identificadores;

// --- Protótipo das funções ---
int fInicializaDB(sqlite3 **db, identificadores *id);
void fFinalizaDB(sqlite3 *db, identificadores id);

//void fMenu();
//void fHelp();

void fAddSite(int argc, char *argv[]);
/*void fAddCategory();

void fDeleteSite();
void fDeleteCategory();

void fListAll();
void fListCategory();
void fSeeAll();
void fSeeCategory();
void fSeeSite();

void fModifySite();
void fModifyCategory();

void fVersion();
void fBackup();
void fExport();
void fHtml();
*/

#endif
