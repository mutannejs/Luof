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
#define TAMFAVSIMP 10000//tamanho favoritos importados

#define ANSI_COLOR_RED "\x1b[0;31m"
#define ANSI_COLOR_GRA "\x1b[0;37m"
#define ANSI_COLOR_BLU "\x1b[0;94m"
#define ANSI_COLOR_WHT "\x1b[0;97m"
#define ANSI_BOLD_YEL "\x1b[1;93m"
#define ANSI_BOLD_CYA "\x1b[1;96m"
#define ANSI_BOLD_WHT "\x1b[1;97m"

#define ERRO "\n\x1b[0;95mERRO\x1b[0;97m: "
#define ERRO2 "\x1b[0;95mERRO\x1b[0;97m: "

/* fNome é usado para funções
 * rNome é usado para retorno de função
 * sNome é usado para struct's
 * nome é usado para outras variáveis
 * */

// --- Structs ---
typedef struct sSite {
	char nome[TAMNOMEFAV];
	char categoria[TAMCAMINHO];
	char link[TAMLINKARQ];
	char texto[TAMTEXTO];
} sSite;

typedef struct sCat {
	char nome[TAMNOMEFAV];
	char caminho[TAMCAMINHO];
	int hie;
	struct sCat *catPai;
	sLista catFilhos;
} sCat;

typedef struct sComando {
	char flag[25];
	char caminho[TAMCAMINHO + TAMNOMEFAV];
} sCom;

typedef struct sBanco {
	char caminhoDB[TAMLINKARQ];
	sCat *arvoreCats;
	sLista listaFavs;
} sBanco;

// --- Protótipo das funções ---
//modulos
sCom fSetaArgumentos(void (**funcao)(sCom com), int argc, char *argv[]);
int fSetaCaminho(sCom *com, int ini, int fim, char *argv[]);
int fAnalisaCaminhoSite(sSite *s, sCom com);
int fAnalisaCaminhoCat(sCat *cat, sCom com);
void fSetaCaminhoArquivo(sBanco *db, char *arq, char *nome);
void fSetaCaminhoCategoria(char caminho[], char nome[]);
int fSetaSiteCategoria(sSite *s);
int fSetaSiteNome(sSite *s);
int fSetaSiteLink(sSite *s);
int fSetaSiteTexto(sSite *s);
int fSetaCatCategoria(sCat *cat);
int fSetaCatNome(sCat *cat);

//dbluof
int fInicializaDB(sBanco *db);
void fFinalizaDB_private(sCat *listaCategorias);
void fFinalizaDB(sBanco *db);
void fPreencheArvoreCats(sBanco *db, FILE *aLuof);
void fEscreveLuof_private(FILE *aLuof, sLista listaCategorias, int hierarquia);
void fEscreveLuof(sBanco *db);
int fBuscaCat(sBanco *db, char caminho[], sCat **c);
sCat* fBuscaCatFilha(sCat *catPai, char nome[]);
int fInsereCategoria(sCat *cPai, sCat c);
void fRemoveCategoria(sCat *cat);
void fEscreveArquivoCat(sBanco *db, char *nomeArq);
void fRemoveArqCat(sBanco *db, sCat *cat);
void fApagarBanco_private(sBanco *db, sCat *cat);
void fApagarBanco(sBanco *db);

//dbcat
void fPreencheListaSite(sBanco *db, sCat *c, int cmp);
sSite fRecuperaFavorito(FILE *arq, char *nomeT);
int fBuscaFavorito(sBanco *db, sSite *s);
void fInsereFavorito(sBanco *db, sSite s);
void fRemoveFavorito(sBanco *db, sSite s);
void fModificaFavorito(sBanco *db, sSite s, sSite sNew);

//teste
void percursoCategorias(sCat *cat, int hie);
void printaListaSites(sLista l);
void printaSite(sSite s);
void printaColorido();
void fTeste();

//main
void fMenu();
void fFree(sCom com);

//help
void fHelp_private();
void fHelp_h();
void fHelp_ab();
void fHelp_ac();
void fHelp_rb();
void fHelp_rc();
void fHelp_lc();
void fHelp_lcs();
void fHelp_lt();
void fHelp_lts();
void fHelp_mb();
void fHelp_mc();
void fHelp_backup();
void fHelp_export();
void fHelp_import();
void fHelp_free();
void fHelp(sCom com);

//add-remove
void fAddBookmark(sCom com);
void fAddCategory(sCom com);
void fRemoveBookmark(sCom com);
void fRemoveCategory(sCom com);


//modify
void fModifyBookmark(sCom com);
void fModifyCategory_atualizaCaminho(sBanco *db, sCat *cat, char *caminhoA, char *nomeA);
void fModifyCategory(sCom com);

//list
void fListCategory(sCom com);
void fListTree_printaLinhas(char linhas[], int hierarquia);
void fListTree_private(sBanco *db, char linhas[], sCat *cat, int hierarquia, int opcao);
void fListTree(sCom com);

//backup
int contaQtdCats(sCat *cat);
int fBackup_preencnheListaCats(char **nomes, int *pos, sCat *cat);
char* fBackup_criar(sBanco *db);
int fBackup_separaNomeQuantidade(char *nomeQtdArq, char *nomeArq);
void fBackup_restaurar(sBanco *db, FILE *arqBackup);
void fBackup(sCom com);

//import-export
void fExport_private(sBanco *db, FILE *arqExport, sCat *cat, int hierarquia);
void fExport(sCom com);
sSite fImport_setaFavorito(char *linha, char *categoria);
void fImport_fav(sBanco *db, FILE *arqImport, sCat *cat);
void fImport_cat(sBanco *db, FILE *arqImport, sCat *cat, char *linha);
void fImport(sCom com);

#endif
