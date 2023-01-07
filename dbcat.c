#include "luof.h"

int fPreencheListaSite(sBanco *db, sCat *c, int cmp) {

	sSite siteTemp;
	char nomeTemp[TAMNOMEFAV], nomeArqCat[TAMLINKARQ];
	FILE *arqCat;

	//abre o arquivo da categoria
	fSetaCaminhoArquivo(db, nomeArqCat, c->nome);
	arqCat = fopen(nomeArqCat, "r");

	//documento não encontrado
	if (arqCat == NULL) {
		printf(ANSI_COLOR_RED "\nErro: O arquivo %s não pode ser aberto.\n", nomeArqCat);
		return 1;
	}

	db->listaFavs = criaLista(struct sSite);

	//pega linha por linha do arquivo da categoria, faz a comparação usando a primeira linha de um site, e depois pega as outras três referentes ao mesmo site
	while (fgets(nomeTemp, 100, arqCat) != NULL) {
		siteTemp = fRecuperaFavorito(arqCat, nomeTemp);
		//adiciona na lista apenas se pertencer a categoria correta
		if (cmp == 0 || strcmp(siteTemp.categoria, c->caminho) == 0)
			pushBackList(db->listaFavs, &siteTemp);
	}

	fclose(arqCat);

	return 0;

}

sSite fRecuperaFavorito(FILE *arq, char *nomeT) {

	sSite s;

	//verifica se foi passado o nome do site, ou se ele ainda não foi lido
	if (nomeT) {
		strncpy(s.nome, nomeT, strlen(nomeT));
		s.nome[strlen(nomeT)-1] = '\0';
	}
	else {
		fgets(s.nome, TAMCAMINHO, arq);
		s.nome[strlen(s.nome)-1] = '\0';
	}

	fgets(s.categoria, TAMCAMINHO, arq);
	s.categoria[strlen(s.categoria)-1] = '\0';

	fgets(s.link, TAMLINKARQ, arq);
	s.link[strlen(s.link)-1] = '\0';

	fgets(s.texto, TAMTEXTO, arq);
	if (s.texto[strlen(s.texto)-1] == '\n')
	s.texto[strlen(s.texto)-1] = '\0';

	return s;

}

int fBuscaFavorito(sBanco *db, sSite *s) {

	if (emptyList(db->listaFavs))
		return 0;

	//variaveis para percorrer a lista
	sIterador it = criaIt(db->listaFavs);
	sSite *favorito;

	do {
		favorito = (struct sSite*) retornaItera(&it);
		//encontrou o favorito
		if ((strcmp(favorito->nome, s->nome) == 0) && (strcmp(favorito->categoria, s->categoria) == 0)) {
			//seta os outros valores de s
			strcpy(s->link, favorito->link);
			strcpy(s->texto, favorito->texto);
			//informa que o site foi encontrado
			return 1;
		}
		iteraProximo(&it);
	} while (!inicioIt(&it));

	//não encontrou o site
	return 0;

}

void fInsereFavorito(sBanco *db, sSite s) {

	sSite *favorito;
	sIterador it = criaIt(db->listaFavs);
	int encontrouPos = 0;

	if (emptyList(db->listaFavs)) {//se a lista está vazia apenas insere o favorito no fim da lista
		pushBackList(db->listaFavs, &s);
	}
	else {//se a lista não estiver vazia, insere em ordem alfabética
		//procura a posição que o favorito deve ser inserido (antes do primeiro favorito com o nome maior no quesito lexográfico)
		do {
			favorito = (struct sSite*) retornaItera(&it);
			if (strcmp(favorito->nome, s.nome) > 0)
				encontrouPos = 1;
			else
				iteraProximo(&it);
		} while (encontrouPos == 0 && !inicioIt(&it));

		if (encontrouPos == 0)
			pushBackList(db->listaFavs, &s);
		else
			insereAntIt(&it, &s);
	}

}

void fRemoveFavorito(sBanco *db, sSite s) {

	sSite *favorito;
	sIterador it = criaIt(db->listaFavs);
	int encontrouPos = 0;

	do {
		favorito = (struct sSite*) retornaItera(&it);
		if ((strcmp(favorito->nome, s.nome) == 0) && (strcmp(favorito->categoria, s.categoria) == 0)) {//encontrou o favorito
			removeIt(&it);//remove ele da lista
			encontrouPos = 1;
		}
		else {
			iteraProximo(&it);
		}
	} while (encontrouPos == 0 && !inicioIt(&it));

}

void fModificaFavorito(sBanco *db, sSite s, sSite sNew) {

	fRemoveFavorito(db, s);
	fInsereFavorito(db, sNew);

}
