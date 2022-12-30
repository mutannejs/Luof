#include "luof.h"

void fExport_private(sBanco *db, FILE *arqExport, sSite s, int hierarquia) {

	sSite *siteDoIterador;
	sLista listaFavoritos = NULL;
	sIterador it;

	hierarquia = hierarquia + 1;

	//escreve os dados da categoria
	for (int i = 0; i < hierarquia; i++)
		fprintf(arqExport, "\t");
	fprintf(arqExport, "<DT><H3>%s</H3>\n", s.nome);
	for (int i = 0; i < hierarquia; i++)
		fprintf(arqExport, "\t");
	fprintf(arqExport, "<DL><p>\n");

	listaFavoritos = fPreencheListaSiteCmp(db, s);

	if (emptyList(listaFavoritos)) {
		freeList(listaFavoritos);
		return;
	}

	it = criaIt(listaFavoritos);
	do {
		siteDoIterador = (struct sSite*) retornaItera(&it);

		//escreve o favorito no arqivo do export se ele for um favorito, e faz a recursão caso ele seja uma categoria
		if (siteDoIterador->ehCat == '1')
			fExport_private(db, arqExport, *siteDoIterador, hierarquia);
		else {
			//fExport_printaFavorito(*siteDoIterador, hierarquia);
			for (int i = 0; i <= hierarquia; i++)
				fprintf(arqExport, "\t");
			fprintf(arqExport, "<DT><A HREF=\"%s\">%s</A>\n", siteDoIterador->link, siteDoIterador->nome);
		}

		iteraProximo(&it);
	} while (!inicioIt(&it));

	freeList(listaFavoritos);

	//escreve o final da categoria
	for (int i = 0; i < hierarquia; i++)
		fprintf(arqExport, "\t");
	fprintf(arqExport, "</DL><p>\n");

}

void fExport() {

	sSite c;
	sCat *categoria;
	sBanco db;
	sSite *siteDoIterador;
	sIterador it;
	char nomeExport[TAMLINKARQ];
	FILE *arqExport;

	if (fInicializaDB(&db))
		return;

	if (emptyList(db.raiz)) {
		printf(ERRO);
		printf("Nada ainda foi inserido.\n");
		fFinalizaDB(&db);
		return;
	}

	if (fSetaSiteCategoria(&c))
		return;
	c.ehCat = '1';

	if (strcmp(c.categoria, "/") == 0) {
		if (emptyList(db.raiz)) {
			printf(ERRO);
			printf("Categoria vazia.\n");
			fFinalizaDB(&db);
			return;
		}
		strcpy(c.nome, "luof");
		db.listaSites = db.raiz;
		categoria = db.listaCategorias;
	}
	else {
		//verifica se a categoria existe e se ela não está vazia
		if (fBuscaCat(&db, c, &categoria)) {
			fFinalizaDB(&db);
			return;
		}
		if (fPreencheListaSite(&db, categoria)) {
			fFinalizaDB(&db);
			return;
		}
		if (emptyList(db.listaSites)) {
			printf(ERRO);
			printf("Categoria vazia.\n");
			fFinalizaDB(&db);
			return;
		}
		//separa a categoria pai e o nome da categoria
		for (int i = strlen(c.categoria); i >= 0; i--) {
			if (c.categoria[i] == '/' || i == 0) {
				strcpy(c.nome, &c.categoria[i]);
				if (i == 0) {
					strcpy(c.categoria, "luof");
					categoria = db.listaCategorias;
					db.listaSites = db.raiz;
				}
				else {
					c.categoria[i] = '\0';
					if (fBuscaCat(&db, c, &categoria)) {
						fFinalizaDB(&db);
						return;
					}
					if (fPreencheListaSite(&db, categoria)) {
						fFinalizaDB(&db);
						return;
					}
				}
				i = 0;
			}
		}
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
	if (strcmp(c.nome, "luof") == 0) {

		//escreve os dados da categoria
		fprintf(arqExport, "\t<DT><H3>%s</H3>\n\t<DL><p>\n", c.nome);

		if (emptyList(db.listaSites)) {
			freeList(db.listaSites);
			return;
		}

		it = criaIt(db.listaSites);
		do {
			siteDoIterador = (struct sSite*) retornaItera(&it);
			//escreve o favorito no arqivo do export se ele for um favorito, e faz a recursão caso ele seja uma categoria
			if (siteDoIterador->ehCat == '1') {
				fExport_private(&db, arqExport, *siteDoIterador, 1);
			}
			else {
				//fExport_printaFavorito(*siteDoIterador, hierarquia);
				fprintf(arqExport, "\t<DT><A HREF=\"%s\">%s</A>\n", siteDoIterador->link, siteDoIterador->nome);
			}
			iteraProximo(&it);
		} while (!inicioIt(&it));

		//escreve o final da categoria
		fprintf(arqExport, "\t</DL><p>\n");

	}
	else {
		fExport_private(&db, arqExport, c, 0);
	}

	//escreve a última linha
	fprintf(arqExport, "</DL>");

	//fecha os arquivos abertos
	fFinalizaDB(&db);

	printf(ANSI_BOLD_WHT "\nExport criado com sucesso.\n");

}
