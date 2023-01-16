#include "luof.h"

void fExport_private(sBanco *db, FILE *arqExport, sCat *cat, int hierarquia) {

	sSite *favorito;
	sCat *catTemp;
	sIterador it;

	hierarquia = hierarquia + 1;

	//escreve todas as subcategorias
	if (!emptyList(cat->catFilhos)) {
		it = criaIt(cat->catFilhos);
		do {
			catTemp = (struct sCat*) retornaItera(&it);
			//escreve os dados da categoria
			for (int i = 0; i <= hierarquia; i++)
				fprintf(arqExport, "\t");
			fprintf(arqExport, "<DT><H3>%s</H3>\n", catTemp->nome);
			for (int i = 0; i <= hierarquia; i++)
				fprintf(arqExport, "\t");
			fprintf(arqExport, "<DL><p>\n");
			fExport_private(db, arqExport, catTemp, hierarquia);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

	//printa favorito por favorito da categoria
	fPreencheListaSite(db, cat, 1);
	if (!emptyList(db->listaFavs)) {
		it = criaIt(db->listaFavs);
		do {
			favorito = (struct sSite*) retornaItera(&it);
			for (int i = 0; i <= hierarquia; i++)
				fprintf(arqExport, "\t");
			fprintf(arqExport, "<DT><A HREF=\"%s\">%s</A>\n", favorito->link, favorito->nome);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

	//escreve o final da categoria
	for (int i = 0; i < hierarquia; i++)
		fprintf(arqExport, "\t");
	fprintf(arqExport, "</DL><p>\n");

}

void fExport() {

	sSite s;
	sCat *cat;
	sBanco db;
	char nomeExport[TAMLINKARQ];
	FILE *arqExport;

	if (fInicializaDB(&db))
		return;

	if (fSetaSiteCategoria(&s)) {
		fFinalizaDB(&db);
		return;
	}
	if (fBuscaCat(&db, s.categoria, &cat)) {
		fFinalizaDB(&db);
		return;
	}
	fPreencheListaSite(&db, cat, 1);

	//se a categoria não possui subcategorias nem favoritos
	if (emptyList(cat->catFilhos) && emptyList(db.listaFavs)) {
		printf(ERRO);
		printf("Lista vazia.\n");
		fFinalizaDB(&db);
		return;
	}

	//cria arquivo do export
	strcpy(nomeExport, "bookmarks-luof.html");
	arqExport = fopen(nomeExport, "r");
	//faz um loop para não haver conflito entre export's caso já exista algum no diretório atual
	for (int i = 1; arqExport; i++) {
		sprintf(nomeExport, "bookmarks-luof-%d.html", i);
		arqExport = freopen(nomeExport, "r", arqExport);
	}
	arqExport = fopen(nomeExport, "w");
	if (!arqExport) {
		fFinalizaDB(&db);
		return;
	}

	//escreve as primeiras linhas no arquivo do export
	fprintf(arqExport,
	"<!DOCTYPE NETSCAPE-Bookmark-file-1>\n"
	"<!-- This is an automatically generated file.\n"
	"\tIt will be read and overwritten.\n"
	"\tDO NOT EDIT! -->\n"
	"<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=UTF-8\">\n"
	"<TITLE>Bookmarks Luof</TITLE>\n"
	"<H1>Menu de favoritos gerado pelo Luof</H1>\n"
	"\n"
	"<DL><p>\n"
	);

	//escreve as demais linhas (categoria e seu conteúdo)
	fExport_private(&db, arqExport, cat, -1);

	//escreve a última linha
	fprintf(arqExport, "</DL>");

	//finaliza as estruturas criadas
	fFinalizaDB(&db);

	printf(ANSI_BOLD_WHT "\nArquivo %s adicionado no diretório atual.", nomeExport);
	printf("\nExport criado com sucesso.\n");

}
