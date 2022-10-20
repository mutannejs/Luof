#ifndef LUOF_H
#define LUOF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "lista-iterador.h"
#include "pilha.h"

#define TAMNOMEFAV 100//tamanho de nome de favoritos
#define TAMLINKARQ 500//tamanho de links e de arquivos
#define TAMCAMINHO 1000//tamanho caminho da categoria
#define TAMTEXTO 5000//tamanho texto

/* fNome é usado para funções
 * rNome é usado para retorno de função
 * aNome é usado para arquivos
 * sNome é usado para struct's
 * nome é usado para outras variáveis
 * */

// --- Structs ---
typedef struct sSite {
	char nome[TAMNOMEFAV];
	char categoria[TAMCAMINHO];
	char link[TAMLINKARQ];
	char texto[TAMTEXTO];
	char ehCat;
} sSite;

typedef struct sCat {
	char nome[TAMNOMEFAV];
	struct sCat *catPai;
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
char caminhoDB[TAMLINKARQ];
int tamCaminhoDB;

// --- Protótipo das funções ---
//modulos
void fEscreveLuof_private(sBanco *db, sLista listaCategorias, int hierarquia);
void fEscreveLuof(sBanco *db);
void fMudaNomeCategoriaArvore(sBanco *db, char *caminho, char *nome);
void fMudaCaminhoCategoriaArvore_private(sBanco *db, sCat *cat, char *caminhoA, char *caminhoN);
void fMudaCaminhoCategoriaArvore(sBanco *db, char *caminho1, char *caminho2);
void fEscreveArquivoCat(sBanco *db, char *nomeArq);
void fSetaCaminhoArquivo(char *arq, char *nome);
void fIncrementaCamCat(char *caminho, char *nome);

//dbluof
int fInicializaDB(sBanco *db);
void fFinalizaDB(sBanco *db);
void fLiberaCats(sLista listaCategorias);
char* fPreencheListaCat_private(sBanco *db, sCat *cPai, char linhaCat[]);
void fPreencheListaCat(sBanco *db);
void fPreencheRaiz(sBanco *db);

//dbcat
int fBuscaCat(sBanco *db, sSite s, sCat **c);
int fPreencheListaSite(sBanco *db, sCat *c);
int fBuscaFavorito(sBanco *db, sSite *s);
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
void fListTree_printaFavorito(sSite s, char linhas[], int hierarquia);
sLista fListTree_preencheSites(sSite s);
void fListTree_private(sBanco *db, char linhas[], sSite s, int hierarquia);
void fListTree();

/*void fSeeAll();
void fSeeCategory();
void fSeeSite();*/

//modify
void fModifySite();
void fModifyCategory();

/*void fVersion();
void fBackup();
void fExport();
void fHtml();
*/

#endif
