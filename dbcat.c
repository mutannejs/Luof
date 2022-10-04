#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "luof.h"

char* fBuscaCat(sBanco *db, sSite s, sCat *c) {

	char categorias[10][100];//vetor com as categorias
	int qtdCats = 0;//quantidade de categorias totais
	int tamCat = strlen(s.categoria);
	int ini = 0;//indica o começo do nome da categoria

	//cria uma lista com as categorias e subcategorias
	for (int i = 0; i <= tamCat; i++) {
	//chegou no fim de alguma categoria
		if (s.categoria[i] == '/' || i == tamCat) {
		//adiciona em categorias
		strncpy(categorias[qtdCats], &s.categoria[ini], i-ini);
		categorias[qtdCats][i-ini+1] = '\0';

		//ini passa a ser o próximo caracter depois de /
		ini = i + 1;
		//qtdCats incrementa
		qtdCats++;
		}
	}

	//se ainda não tiver sido criado nenhuma categoria
	if (emptyList(db->listaCategorias)) {
		int tamNome = strlen(categorias[0]);
		char *nomeCatNaoEncontrada = (char *) malloc(tamNome);
		strncpy(nomeCatNaoEncontrada, categorias[0], tamNome);
		return nomeCatNaoEncontrada;
	}

	//variaveis usadas para percorrer a lista de categorias
	sIterador it = criaIt(db->listaCategorias);

	//informa quantas categorias já foram encontradas, deve ser igual a i ou maior em 1
	int encontrou = 0;

	//busca a categoria na estrutura sCat c
	for (int i = 0; i < qtdCats && encontrou == i; i++) {
		do {
			sCat *cat1 = (struct sCat*) retornaItera(&it);
			//printf("%s :: %s\n", cat1->nome, categorias[i]);RETIRAR

			//se o nome bater procura a subcategoria e guarda em c a categoria atual
			if (strcmp(cat1->nome, categorias[i]) == 0) {
				*c = *cat1;
				it = criaIt(cat1->catFilhos);
				encontrou++;
			}
			else {
				iteraProximo(&it);
			}

		} while (encontrou == i && !inicioIt(&it));
	}

	//se não encontrou alguma categoria retorna o nome dela, caso contrário retorna NULL
	if (encontrou < qtdCats) {
		int tamNome = strlen(categorias[encontrou]);
		char *nomeCatNaoEncontrada = (char *) malloc(tamNome);
		strncpy(nomeCatNaoEncontrada, categorias[encontrou], tamNome);
		return nomeCatNaoEncontrada;
	}
	else {
		//cria uma lista de favoritos pertecentes à categoria
		if (fPreencheListaSite(db, c) == 0) {
			printf("Erro no arquivo: ");
			return "";
		}
		return NULL;
	}

}

int fPreencheListaSite(sBanco *db, sCat *c) {

	//usado para armazenar temporariamente os sites da categorias
	sSite siteTemp;
	char nomeTemp[100], ehCategoria[3];
	int tamanho;

	//nome do arquivo da categoria
	char nomeArqCat[400];

	//abre o arquivo da categoria
	fSetaCaminhoArquivo(nomeArqCat, c->nome);
	db->aCat = fopen(nomeArqCat, "r");
	
	//documento não encontrado
	if (db->aCat == NULL)
		return 0;

	db->listaSites = criaLista(struct sSite);

	//pega linha por linha do arquivo da categoria, faz a comparação usando a primeira linha de um site, e depois pega as outras três referentes ao mesmo site
	while (fgets(nomeTemp, 100, db->aCat) != NULL) {

        //guarda em siteTemp o nome
        tamanho = strlen(nomeTemp);
        nomeTemp[tamanho-1] = '\0';
		strcpy(siteTemp.nome, nomeTemp);

		//guarda em siteTemp a categoria
		fgets(siteTemp.categoria, 1000, db->aCat);
		tamanho = strlen(siteTemp.categoria);
		siteTemp.categoria[tamanho-1] = '\0';

		//guarda em siteTemp o link
		fgets(siteTemp.link, 500, db->aCat);
		tamanho = strlen(siteTemp.link);
		siteTemp.link[tamanho-1] = '\0';

		//guarda em siteTemp o texto
		fgets(siteTemp.texto, 5000, db->aCat);
		tamanho = strlen(siteTemp.texto);
		siteTemp.texto[tamanho-1] = '\0';

		//guarda em siteTemp o ehCat e ignora o '\n'
		fgets(ehCategoria, 3, db->aCat);
		siteTemp.ehCat = ehCategoria[0];

		//adiciona o site na lista
		pushBackList(db->listaSites, &siteTemp);

	}

	return 1;

}

int fBuscaFavorito(sBanco *db, sSite *s, char favorito) {

	if (emptyList(db->listaSites))
		return 0;

	//variaveis para percorrer a lista
	sIterador it = criaIt(db->listaSites);
	sSite *fav;

	//informa se encontrou o site
	int encontrou = 0;

	do {
		fav = retornaItera(&it);
		//encontrou o favorito
		if ((strcmp(fav->nome, s->nome) == 0) && (strcmp(fav->categoria, s->categoria) == 0) && (fav->ehCat == favorito)) {
			//seta os outros valores de s
			strcpy(s->link, fav->link);
			strcpy(s->texto, fav->texto);
			s->ehCat = favorito;
			//informa que o site foi encontrado
			return 1;
		}
		iteraProximo(&it);
	} while (encontrou == 0 && !inicioIt(&it));

	//não encontrou o site
	return 0;

}

void fAdicionaFavorito(sBanco *db, sSite s, sCat c) {

	char nomeArqCat[400];
	int encontrouPos;
	sIterador it = criaIt(db->listaSites);
	sSite *siteDoIterador;

	/* Adiciona o site na lista
	 * Ela estará em ordem alfabética, sendo os primeiros favoritos categorias*/
	if (emptyList(db->listaSites)) {
		pushBackList(db->listaSites, &s);
	}
	else if (s.ehCat == '0') {
		encontrouPos = 0;
		iteraFim(&it);
		do {
			siteDoIterador = retornaItera(&it);
			if (strcmp(siteDoIterador->nome, s.nome) < 0 || siteDoIterador->ehCat == '1')
				encontrouPos = 1;
			else
				iteraAnterior(&it);
		} while (encontrouPos == 0 && !fimIt(&it));
		//se entrar no if significa que não há subcategorias nessa categoria, apenas sites
		//e o site a ser inserido é o menor site (logo deu a volta na lista e nunca encontrou a posição)
		siteDoIterador = frontList(db->listaSites);
		if (siteDoIterador->ehCat == '0' && encontrouPos == 0)
			pushFrontList(db->listaSites, &s);
		else
			insereProxIt(&it, &s);
	}
	else {
		encontrouPos = 0;
		iteraInicio(&it);
		do {
			siteDoIterador = retornaItera(&it);
			if (strcmp(siteDoIterador->nome, s.nome) > 0 || siteDoIterador->ehCat == '0')
				encontrouPos = 1;
			else
				iteraProximo(&it);
		} while (encontrouPos == 0 && !inicioIt(&it));
		//se entrar no if significa que não há sites nessa categoria, apenas subcategorias
		//e a categoria a ser inserida é a maior categoria
		siteDoIterador = backList(db->listaSites);
		if (siteDoIterador->ehCat == '1' && encontrouPos == 0)
			pushBackList(db->listaSites, &s);
		else
			insereAntIt(&it, &s);
	}

	//se a categoria é a raiz
	if (strcmp(s.categoria, "luof") == 0) {
		//reabre o arquivo aLuof para sobreescreve-lo
		fSetaCaminhoArquivo(nomeArqCat, "luof");
		db->aLuof = freopen(nomeArqCat, "w", db->aLuof);

		//escreve a árvore de categorias no arquivo aLuof
		fEscreveLuof(db, db->listaCategorias, 0);
		
		//marca o fim da árvore de categorias
		fprintf(db->aLuof, "##\n");
		
		//escreve os favoritos da raiz
		it = criaIt(db->listaSites);
		do {
			siteDoIterador = retornaItera(&it);
			fprintf(db->aLuof, "%s\n%s\n%s\n%s\n%c\n", siteDoIterador->nome, siteDoIterador->categoria, siteDoIterador->link, siteDoIterador->texto, siteDoIterador->ehCat);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}
	else {
		//reabre o arquivo para sobreescreve-lo
		fSetaCaminhoArquivo(nomeArqCat, c.nome);
		db->aCat = freopen(nomeArqCat, "w", db->aCat);

		//escreve a lista no arquivo da categoria
		it = criaIt(db->listaSites);
		do {
			siteDoIterador = retornaItera(&it);
			fprintf(db->aCat, "%s\n%s\n%s\n%s\n%c\n", siteDoIterador->nome, siteDoIterador->categoria, siteDoIterador->link, siteDoIterador->texto, siteDoIterador->ehCat);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

}

void fAdicionaCatLuof(sBanco *db, sSite s, sCat c) {
	
	sCat cNova;//para a categoria ser inserida na árvore de categorias
	char nomeArqCat[400];
	sIterador it = criaIt(db->raiz);//para atualizar o arquivo aLuof
	sSite *siteDoIterador;
	FILE *nCat;//usado para criar um novo arquivo caso necessário
	
	//seta os valores da categoria para ser inserida na árvore
	strcpy(cNova.nome, s.nome);
	cNova.catFilhos = criaLista(struct sCat);
	
	//adiciona a categoria nova na árvore de categorias
	pushBackList(c.catFilhos, &cNova);
	
	//se a categoria pai não for a raiz atualiza o arquivo aLuof (se for, o arquivo será atualizado ao usar fAdicionaFavorito())
	if (strcmp(s.categoria, "luof") != 0) {
		
		//reabre o arquivo aLuof para sobreescreve-lo
		fSetaCaminhoArquivo(nomeArqCat, "luof");
		db->aLuof = freopen(nomeArqCat, "w", db->aLuof);

		//escreve a árvore de categorias no arquivo aLuof
		fEscreveLuof(db, db->listaCategorias, 0);
		
		//marca o fim da árvore de categorias
		fprintf(db->aLuof, "##\n");
		
		//escreve os favoritos da raiz
		it = criaIt(db->raiz);
		do {
			siteDoIterador = retornaItera(&it);
			fprintf(db->aLuof, "%s\n%s\n%s\n%s\n%c\n", siteDoIterador->nome, siteDoIterador->categoria, siteDoIterador->link, siteDoIterador->texto, siteDoIterador->ehCat);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}
	
	//cria o arquivo referente a categoria caso ainda não exista nenhum arquivo com esse nome
	//se não entrar no if significa que a categoria pai possui o mesmo nome da nova categoria
	if (strcmp(c.nome, cNova.nome) != 0) {

		fSetaCaminhoArquivo(nomeArqCat, s.nome);
		
		nCat = fopen(nomeArqCat, "r");
		//se nCat == NULL significa que o arquivo não existe, então ele é criado e em ambos os casos fechado
		if (!nCat)
			nCat = fopen(nomeArqCat, "w");
			
		fclose(nCat);
	}
	
}

void fRemoveFavorito(sBanco *db, sSite s, sCat c) {

	char nomeArqCat[400];
	sIterador it = criaIt(db->listaSites);
	sSite *siteDoIterador;

	/* Adiciona o site na lista
	 * Ela estará em ordem alfabética, sendo os primeiros favoritos categorias*/
	if (s.ehCat == '0') {
		iteraFim(&it);
		do {
			siteDoIterador = retornaItera(&it);
			if ((strcmp(siteDoIterador->nome, s.nome) == 0) && (strcmp(siteDoIterador->categoria, s.categoria) == 0) && (s.ehCat == siteDoIterador->ehCat)) {
				printf("encontrou o favorito a ser excluido.\n");
				removeIt(&it);
			}
			else
				iteraAnterior(&it);
		} while (!fimIt(&it));
	}
	else {
		iteraInicio(&it);
		do {
			siteDoIterador = retornaItera(&it);
			if ((strcmp(siteDoIterador->nome, s.nome) == 0) && (strcmp(siteDoIterador->categoria, s.categoria) == 0) && (s.ehCat == siteDoIterador->ehCat)) {
				printf("encontrou o favorito a ser excluido.\n");
				removeIt(&it);
			}
			else
				iteraProximo(&it);
		} while (!inicioIt(&it));
	}

	//se a categoria é a raiz
	if (strcmp(s.categoria, "luof") == 0) {
		//reabre o arquivo aLuof para sobreescreve-lo
		fSetaCaminhoArquivo(nomeArqCat, "luof");
		db->aLuof = freopen(nomeArqCat, "w", db->aLuof);

		//escreve a árvore de categorias no arquivo aLuof
		fEscreveLuof(db, db->listaCategorias, 0);
		
		//marca o fim da árvore de categorias
		fprintf(db->aLuof, "##\n");
		
		//escreve os favoritos da raiz
		it = criaIt(db->listaSites);
		do {
			siteDoIterador = retornaItera(&it);
			fprintf(db->aLuof, "%s\n%s\n%s\n%s\n%c\n", siteDoIterador->nome, siteDoIterador->categoria, siteDoIterador->link, siteDoIterador->texto, siteDoIterador->ehCat);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}
	else {
		//reabre o arquivo para sobreescreve-lo
		fSetaCaminhoArquivo(nomeArqCat, c.nome);
		db->aCat = freopen(nomeArqCat, "w", db->aCat);

		//escreve a lista no arquivo da categoria
		it = criaIt(db->listaSites);
		do {
			siteDoIterador = retornaItera(&it);
			fprintf(db->aCat, "%s\n%s\n%s\n%s\n%c\n", siteDoIterador->nome, siteDoIterador->categoria, siteDoIterador->link, siteDoIterador->texto, siteDoIterador->ehCat);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

}
