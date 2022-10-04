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

//variaveis globais
char caminhoDB[300];
int tamCaminhoDB;

// --- Protótipo das funções ---
//dbluof
int fInicializaDB(sBanco *db);
void fFinalizaDB(sBanco *db);
void fSetaCaminhoArquivo(char *arq, char *nome);
void fLiberaCats(sLista listaCategorias);
void fPreencheListaCat(sBanco *db);
void fPreencheRaiz(sBanco *db);
void fEscreveLuof(sBanco *db, sLista listaCategorias, int hierarquia);

//dbcat
char* fBuscaCat(sBanco *db, sSite s, sCat *c);
int fPreencheListaSite(sBanco *db, sCat *c);
int fBuscaFavorito(sBanco *db, sSite *s, char favorito);
void fAdicionaFavorito(sBanco *db, sSite s, sCat c);
void fRemoveFavorito(sBanco *db, sSite s, sCat c);
void fAdicionaCatLuof(sBanco *db, sSite s, sCat c);
void fRemoveCatLuof(sBanco *db, sSite s, sCat c);
void fRemoveArqCat(sBanco *db, sSite s);

//teste
void percursoCategorias(sLista l);
void printaListaSites(sLista l);
void printaSite(sSite s);

//void fMenu();
//void fHelp();

//add-delete
void fAddSite();
void fAddCategory();
void fDeleteSite();
void fDeleteCategory();

//list
void fListCategory(int opcao);
/*
void fListAll();
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
