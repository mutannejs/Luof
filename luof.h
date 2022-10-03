#ifndef LUOF_H
#define LUOF_H

#include "lista-iterador.h"
#include "pilha.h"

/* fNome é usado para funções
 * rNome é usado para retorno de função
 * aNome é usado para arquivos
 * sNome é usado para struct's
 * nome é usado para outras variáveis
 * */

// --- Structs ---
typedef struct sSite {
	char nome[100];
	char categoria[1000];
	char link[500];
	char texto[5000];
	char ehCat;
} sSite;

typedef struct sCat {
	char nome[100];
	sLista catFilhos;
} sCat;

typedef struct sBanco {
	FILE *aLuof;
	FILE *aCat;
	sLista raiz;
	sLista listaCategorias;
	sLista listaSites;
} sBanco;

// --- Protótipo das funções ---
int fInicializaDB(sBanco *db);
void fFinalizaDB(sBanco *db);
void fPreencheListaCat(sBanco *db);
int fPreencheListaSite(sBanco *db, sCat *c);
int fPreencheRaiz(sBanco *db);
int fBuscaFavorito(sBanco *db, sSite *s, char favorito);
char* fBuscaCat(sBanco *db, sSite s, sCat *c);
void fEscreveLuof(sBanco *db, sLista listaCategorias, int hierarquia);

void percursoCategorias(sLista l);
void printaSite(sLista l);

//void fMenu();
//void fHelp();

void fAddSite();
//void fAddCategory();
void fAdicionaFavorito(sBanco *db, sSite s, sCat c);
void fAdicionaCatLuof(sBanco *db, sSite s, sCat c);
/*
*/
//void fCriaCategoria(char *categoria);

/*void fDeleteSite();
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
