#ifndef LUOF_H
#define LUOF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "lista-iterador.h"

#define TAMNOMEFAV 100//tamanho de nome de favoritos
#define TAMCAMINHO 1000//tamanho caminho da categoria
#define TAMLINKARQ 2100//tamanho de links e de arquivos
#define TAMTEXTO 3000//tamanho texto

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
	int hie;
	struct sCat *catPai;
	sLista catFilhos;
} sCat;

typedef struct sBanco {
	FILE *aLuof;
	FILE *aCat;
	char caminhoDB[TAMLINKARQ];
	int tamCaminhoDB;
	sCat *listaCategorias;
	sLista raiz;
	sLista listaSites;
} sBanco;

// --- Protótipo das funções ---
//modulos
int fSetaSiteCategoria(sSite *s);
int fSetaSiteNome(sSite *s);
int fSetaSiteLink(sSite *s);
int fSetaSiteTexto(sSite *s);
int fSetaCatCategoria(sSite *s);
int fSetaCatNome(sSite *s);
void fSetaCaminhoArquivo(sBanco *db, char *arq, char *nome);
void fIncrementaCamCat(char *caminho, char *nome);
void fSetaCaminhoCategoria(char caminho[], sSite s);
void fEscreveLuof_private(sBanco *db, sLista listaCategorias, int hierarquia);
void fEscreveLuof(sBanco *db);
void fEscreveArquivoCat(sBanco *db, char *nomeArq);
int fSeparaArquivoCategoria(sBanco *db, char categoria[], sCat *cat, char nomeA[]);
void fMudaCaminhoCategoriaArvore(sBanco *db, sCat *cat, char *caminhoA, char *caminhoN);

//dbluof
int fInicializaDB(sBanco *db);
void fFinalizaDB(sBanco *db);
void fLiberaCats(sCat *listaCategorias);
void fPreencheListaCat(sBanco *db);
void fPreencheRaiz(sBanco *db);

//dbcat
int fBuscaCat(sBanco *db, sSite s, sCat **c);
int fPreencheListaSite(sBanco *db, sCat *c);
int fBuscaFavorito(sBanco *db, sSite *s);
void fAdicionaFavorito(sBanco *db, sSite s, sCat *c);
void fRemoveFavorito(sBanco *db, sSite s, sCat *c);
void fModificaFavorito(sBanco *db, sSite s, sSite sNew, sCat *c);
void fAdicionaCatLuof(sBanco *db, sSite s, sCat *c);
void fRemoveCatLuof(sBanco *db, sSite s, sCat *c);
void fRemoveArqCat(sBanco *db, sSite s);

//teste
void percursoCategorias(sCat *cat, int hie);
void printaListaSites(sLista l);
void printaSite(sSite s);

//menu
//void fMenu();

//help
void fHelp_private();
void fHelp_h();
void fHelp_ab();
void fHelp_ac();
void fHelp_rb();
void fHelp_rc();
void fHelp_lt();
void fHelp_lc(int opcao);
void fHelp_mb();
void fHelp_mc();
void fHelp_backup();
void fHelp(char *argv);

//add-remove
void fAddBookmark();
void fAddCategory();
void fRemoveBookmark();
void fRemoveCategory();

//modify
void fModifyBookmark();
void fReposicionaCatArvore(sBanco *db, sCat *categoria, sCat *categoria2, sCat **categoria3, char *nome);
void fModifyCategory();
//void fJoinCategorys_juntaCatsArvore(sBanco *db, sCat *catIt, sCat *cat2);
//void fJoinCategorys_juntaLista(sBanco *db, sCat *cate, sCat *cate2, sSite c1, sSite c2);
//void fJoinCategorys_private(sBanco *db, sSite c1, sSite c2);
//void fJoinCategorys();

//list
void fListCategory(int opcao);
void fListTree_printaFavorito(sSite s, char linhas[], int hierarquia);
sLista fListTree_preencheSites(sBanco *db, sSite s);
void fListTree_private(sBanco *db, char linhas[], sSite s, int hierarquia);
void fListTree();

//backup
void fBackup_escreveArvore(FILE *arqCat, sLista listaCategorias, int hierarquia);
void fBackup_preencnheListaCats(sLista listaCats, sCat *cat);
char* fBackup_criar(sBanco *db);
void fBackup_excluirArquivos(sBanco *db);
int fBackup_separaNomeQuantidade(char *nomeQtdArq, char *nomeArq);
void fBackup_adicionaSite(FILE *arq, sLista l);
void fBackup_restaurar(sBanco *db, FILE *arqBackup);
void fBackup();

//import-export
void fImport();
sLista fExport_preencheSites(sBanco *db, sSite s);
void fExport_private(sBanco *db, FILE *arqExport, sSite s, int hierarquia);
void fExport();

//void fHtml();

#endif
