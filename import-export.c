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

sSite fImport_setaFavorito(char *linha, char *categoria) {

	sSite s;
	int fase = 0;

	//seta a categoria e o texto do site
	strcpy(s.categoria, categoria);
	strcpy(s.texto, "Favorito importado.");

	//encontra e guarda o nome do favorito
	for (int i = 0, j = 0; fase < 5; i++) {
		if (fase == 2) {
			if (linha[i] == '\"') {
				s.link[j] = '\0';
				j = 0;
				fase++;
			}
			else {
				s.link[j] = linha[i];
				j++;
			}
		}
		else if (fase == 4) {
			if (linha[i] == '<') {
				s.nome[j] = '\0';
				j = 0;
				fase++;
			}
			else {
				s.nome[j] = linha[i];
				j++;
			}
		}
		else if (linha[i] == '>') {
			fase++;
		}
		else if (strncmp(&linha[i], "HREF=\"", 6) == 0) {
			fase++;
			i += 5;
		}
	}

	return s;

}

void fImport_fav(sBanco *db, FILE *arqImport, sCat *cat) {

	sSite s, *favorito;
	sLista listaTemp;
	char linhaArq[TAMFAVSIMP], *linha;

	//cria uma lista para inserir os favoritos temporiariamente
	listaTemp = criaLista(struct sSite);

	do {

		//lê uma linha e tira seus espaços iniciais
		fgets(linhaArq, TAMFAVSIMP, arqImport);
		linha = linhaArq;
		while (isspace(linha[0]))
			linha++;

		if (strncmp(linha, "<DT><H", 6) == 0) {//a linha é uma subpasta
			fImport_cat(db, arqImport, cat, linha);
		}
		else if (strncmp(linha, "<DT><A", 6) == 0) {//a linha é um favorito
			s = fImport_setaFavorito(linha, cat->caminho);
			pushFrontList(listaTemp, &s);
		}

	} while (strncmp(linha, "</DL>", 5) != 0);

	//escreve os favoritos da listaTemp na categoria
	fPreencheListaSite(db, cat, 0);
	while (!emptyList(listaTemp)) {
		favorito = backList(listaTemp);
		if (!fBuscaFavorito(db, favorito))
			fInsereFavorito(db, *favorito);
		popBackList(listaTemp);
	}
	fEscreveArquivoCat(db, cat->nome);
	freeList(listaTemp);

}

void fImport_cat(sBanco *db, FILE *arqImport, sCat *cat, char *linha) {

	sCat c, *catFilha;

	//encontra e guarda o nome da categoria
	// i conta os caracteres da linha
	// j conta a quantidade de '>'
	// k conta os caracteres de c.nome
	for (int i = 0, j = 0, k = 0; j < 3; i++) {
		if (linha[i] == '>') {
			j++;
		}
		else if (j == 2) {
			if (linha[i] == '<') {
				c.nome[k] = '\0';
				j++;
			}
			else {
				c.nome[k] = linha[i];
				k++;
			}
		}
	}
	//lê o pŕoximo <DL><p>
	fgets(linha, TAMFAVSIMP, arqImport);

	//cria uma categoria, ou seta uma já criada com o mesmo nome. Com base em cat e c.nome
	catFilha = fBuscaCatFilha(cat, c.nome);
	if (!catFilha) {
		fInsereCategoria(cat, c);
		catFilha = fBuscaCatFilha(cat, c.nome);
	}

	//guarda seus favoritos e subcategorias
	fImport_fav(db, arqImport, catFilha);

}

void fImport() {

	sCat *cat, c;
	sBanco db;
	char caminhoImport[TAMLINKARQ], linhaArq[TAMFAVSIMP];
	FILE *arqImport;

	if (fInicializaDB(&db))
		return;

	//pede ao usuário o caminho do backup
	printf(ANSI_BOLD_WHT "Informe o caminho do arquivo de import: " ANSI_COLOR_GRA);
	scanf(" %[^\n]", caminhoImport);

	//abre o arquivo
	arqImport = fopen(caminhoImport, "r");

	//retorna mensagem de erro e encerra
	if (!arqImport) {
		printf(ERRO);
		printf("Erro ao tentar abrir o arquivo de import.\n");
		printf("Saindo...\n");
		fFinalizaDB(&db);
		return;
	}

	if (fSetaCatCategoria(&c)) {
		fFinalizaDB(&db);
		return;
	}
	if (fBuscaCat(&db, c.caminho, &cat)) {
		fFinalizaDB(&db);
		return;
	}

	//lê o arquivo até <DL><p>
	while (fgets(linhaArq, 9, arqImport) != NULL && strncmp(linhaArq, "<DL><p>", 7) != 0) {}

	//chama a recursão para os favoritos do import
	if (strncmp(linhaArq, "</DL>", 5) != 0) {
		fImport_fav(&db, arqImport, cat);
	}

	//fecha os arquivo e retorna sucesso
	fclose(arqImport);
	fEscreveLuof(&db);
	fFinalizaDB(&db);
	printf(ANSI_BOLD_WHT "\nImport feito com sucesso.\n");

}
