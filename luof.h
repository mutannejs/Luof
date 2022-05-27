#ifndef LUOF_H
#define LUOF_H

/* fNome é usado para funções
 * aNome é usado para arquivos
 * rNome é usado para retorno de função
 * nome é usado para outras variáveis
 * */

// --- Structs ---
typedef struct site {
	char *nome;
	char *categoria;
	char *link;
	char *texto;
} site;

typedef struct categoria {
	char *categoria;
	char *catRaiz;
} categoria;

// --- Protótipo das funções ---
int fInicializaDB(FILE **aLuof);
void fFinalizaDB(FILE **aLuof, FILE **aCat);

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
