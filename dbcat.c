#include "luof.h"

int fBuscaCat(sBanco *db, sSite s, sCat **c) {

	char categorias[10][TAMNOMEFAV];//vetor com as categorias
	int qtdCats = 0;//quantidade de categorias totais
	int tamCat = strlen(s.categoria);
	int ind = 0;//indica o indice do caracter no nome da categoria

	//cria um vetor com a hierarquia de categorias (em categorias[][])
	for (int i = 0; i <= tamCat; i++) {
		if (s.categoria[i] == '/') {//chegou no fim do nome de alguma categoria
			//só pode haver 10 categorias no máximo, logo a categoria atual não pode ser adicionada
			if (qtdCats == 10) {
				printf(ERRO);
				printf("No máximo pode existir 9 subcategorias.\n");
				return 1;
			}
			//marca o fim do nome da categoria
			categorias[qtdCats][ind] = '\0';
			//atualiza os valores dos contadores
			ind = 0;
			qtdCats++;
		}
		else if (s.categoria[i] == '\0') {
			categorias[qtdCats][ind] = s.categoria[i];
			qtdCats++;
		}
		else{
			categorias[qtdCats][ind] = s.categoria[i];
			ind++;
		}
	}

	//se ainda não tiver sido criado nenhuma categoria
	if (emptyList(db->listaCategorias->catFilhos)) {
		printf(ERRO);
		printf("Categoria \"%s\" não encontrada.\n", categorias[0]);
		return 1;
	}

	//variaveis usadas para percorrer a lista de categorias
	sIterador it = criaIt(db->listaCategorias->catFilhos);
	sCat *cat;
	
	//busca a categoria na árvore de categorias e c passa a ser igual a ela
	for (int i = 0; i < qtdCats; i++) {
		int encontrou = 0;
		//se chegou em uma categoria vazia
		if (emptyList(it.lista)) {
			printf(ERRO);
			printf("Categoria \"%s\" não encontrada.\n", categorias[i]);
			return 1;
		}
		do {
			cat = (struct sCat*) retornaItera(&it);
			//se o nome bater procura a subcategoria e guarda em c a categoria atual
			if (strcmp(cat->nome, categorias[i]) == 0) {
				*c = cat;
				it = criaIt(cat->catFilhos);
				encontrou = 1;
			}
			else {
				iteraProximo(&it);
			}
		} while (encontrou == 0 && !inicioIt(&it));
		//se não encontrou a categoria[i] na árvore de categoria
		if (encontrou == 0) {
			printf(ERRO);
			printf("Categoria \"%s\" não encontrada.\n", categorias[i]);
			return 1;
		}
	}

	return 0;

}

int fPreencheListaSite(sBanco *db, sCat *c) {

	//usado para armazenar temporariamente os sites da categorias
	sSite siteTemp;
	FILE *arqCat;
	char nomeTemp[TAMNOMEFAV], nomeArqCat[TAMLINKARQ];

	//abre o arquivo da categoria
	fSetaCaminhoArquivo(db, nomeArqCat, c->nome);
	arqCat = fopen(nomeArqCat, "r");

	//documento não encontrado
	if (arqCat == NULL) {
		printf(ANSI_COLOR_RED "\nErro: O arquivo %s não pode ser aberto.\n", nomeArqCat);
		return 1;
	}

	db->listaSites = criaLista(struct sSite);

	//pega linha por linha do arquivo da categoria, faz a comparação usando a primeira linha de um site, e depois pega as outras três referentes ao mesmo site
	while (fgets(nomeTemp, 100, arqCat) != NULL) {
		siteTemp = fRecuperaFavorito(arqCat, nomeTemp);
		pushBackList(db->listaSites, &siteTemp);
	}

	fclose(arqCat);

	return 0;

}

sLista fPreencheListaSiteCmp(sBanco *db, sSite s) {

	sSite siteTemp;
	sLista listaFavoritos;
	FILE *arqCat;
	char categoria[TAMCAMINHO], nomeArqCat[TAMLINKARQ], nomeTemp[TAMNOMEFAV];

	if (strcmp(s.categoria, "luof") == 0) {
		strcpy(categoria, s.nome);
	}
	else {
		strcpy(categoria, s.categoria);
		fIncrementaCamCat(categoria, s.nome);
	}

	fSetaCaminhoArquivo(db, nomeArqCat, s.nome);
	arqCat = fopen(nomeArqCat, "r");
	listaFavoritos = criaLista(struct sSite);

	while (fgets(nomeTemp, 100, arqCat) != NULL) {
		siteTemp = fRecuperaFavorito(arqCat, nomeTemp);
		//adiciona na lista apenas se pertencer a categoria correta
		if (strcmp(siteTemp.categoria, categoria) == 0)
			pushBackList(listaFavoritos, &siteTemp);
	}

	fclose(arqCat);
	
	return listaFavoritos;

}

int fBuscaFavorito(sBanco *db, sSite *s) {

	if (emptyList(db->listaSites))
		return 0;

	//variaveis para percorrer a lista
	sIterador it = criaIt(db->listaSites);
	sSite *fav;

	do {
		fav = (struct sSite*) retornaItera(&it);
		//encontrou o favorito
		if ((strcmp(fav->nome, s->nome) == 0) && (strcmp(fav->categoria, s->categoria) == 0) && (fav->ehCat == s->ehCat)) {
			//seta os outros valores de s
			strcpy(s->link, fav->link);
			strcpy(s->texto, fav->texto);
			//informa que o site foi encontrado
			return 1;
		}
		iteraProximo(&it);
	} while (!inicioIt(&it));

	//não encontrou o site
	return 0;

}

void fEscreveArquivoCat(sBanco *db, char *nomeArq) {

	sSite *siteDoIterador;
	sIterador it;
	FILE *arqCat;
	char nomeArqCat[TAMLINKARQ];

	//abre o arquivo da categoria para escrita
	fSetaCaminhoArquivo(db, nomeArqCat, nomeArq);
	arqCat = fopen(nomeArqCat, "w");

	if (!emptyList(db->listaSites)) {
		//escreve a lista no arquivo da categoria
		it = criaIt(db->listaSites);
		do {
			siteDoIterador = (struct sSite*) retornaItera(&it);
			fprintf(arqCat, "%s\n%s\n%s\n%s\n%c\n", siteDoIterador->nome, siteDoIterador->categoria, siteDoIterador->link, siteDoIterador->texto, siteDoIterador->ehCat);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

	fclose(arqCat);

}

void fAdicionaFavorito(sBanco *db, sSite s, sCat *c) {

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
			siteDoIterador = (struct sSite*) retornaItera(&it);
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
			siteDoIterador = (struct sSite*) retornaItera(&it);
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
	if (strcmp(s.categoria, "luof") == 0)
		fEscreveLuof(db);
	else
		fEscreveArquivoCat(db, c->nome);

}

void fRemoveFavorito(sBanco *db, sSite s, sCat *c) {

	sIterador it = criaIt(db->listaSites);
	sSite *siteDoIterador;
	int encontrou = 0;

	/* Remove o site da lista
	 * Ela estará em ordem alfabética, sendo os primeiros favoritos categorias*/
	if (s.ehCat == '0') {
		iteraFim(&it);
		do {
			siteDoIterador = (struct sSite*) retornaItera(&it);
			if ((strcmp(siteDoIterador->nome, s.nome) == 0) && (strcmp(siteDoIterador->categoria, s.categoria) == 0) && (s.ehCat == siteDoIterador->ehCat)) {
				removeIt(&it);
				encontrou = 1;
			}
			else {
				iteraAnterior(&it);
			}
		} while (encontrou == 0 && !fimIt(&it));
	}
	else {
		iteraInicio(&it);
		do {
			siteDoIterador = (struct sSite*) retornaItera(&it);
			if ((strcmp(siteDoIterador->nome, s.nome) == 0) && (strcmp(siteDoIterador->categoria, s.categoria) == 0) && (s.ehCat == siteDoIterador->ehCat)) {
				removeIt(&it);
				encontrou = 1;
			}
			else {
				iteraProximo(&it);
			}
		} while (encontrou == 0 && !inicioIt(&it));
	}

	//se a categoria é a raiz
	if (strcmp(s.categoria, "luof") == 0)
		fEscreveLuof(db);
	else
		fEscreveArquivoCat(db, c->nome);

}

void fModificaFavorito(sBanco *db, sSite s, sSite sNew, sCat *c) {

	sIterador it = criaIt(db->listaSites);
	sSite *siteDoIterador;
	int encontrou = 0;

	/* Modifica o site na lista
	 * Ela estará em ordem alfabética, sendo os primeiros favoritos categorias*/
	if (s.ehCat == '0') {
		iteraFim(&it);
		do {
			siteDoIterador = (struct sSite*) retornaItera(&it);
			if ((strcmp(siteDoIterador->nome, s.nome) == 0) && (strcmp(siteDoIterador->categoria, s.categoria) == 0) && (s.ehCat == siteDoIterador->ehCat)) {
				strcpy(siteDoIterador->nome, sNew.nome);
				strcpy(siteDoIterador->categoria, sNew.categoria);
				strcpy(siteDoIterador->link, sNew.link);
				strcpy(siteDoIterador->texto, sNew.texto);
				siteDoIterador->ehCat = sNew.ehCat;
				encontrou = 1;
			}
			else {
				iteraAnterior(&it);
			}
		} while (encontrou == 0 && !fimIt(&it));
	}
	else {
		iteraInicio(&it);
		do {
			siteDoIterador = (struct sSite*) retornaItera(&it);
			if ((strcmp(siteDoIterador->nome, s.nome) == 0) && (strcmp(siteDoIterador->categoria, s.categoria) == 0) && (s.ehCat == siteDoIterador->ehCat)) {
				strcpy(siteDoIterador->nome, sNew.nome);
				strcpy(siteDoIterador->categoria, sNew.categoria);
				strcpy(siteDoIterador->link, sNew.link);
				strcpy(siteDoIterador->texto, sNew.texto);
				siteDoIterador->ehCat = sNew.ehCat;
				encontrou = 1;
			}
			else {
				iteraProximo(&it);
			}
		} while (encontrou == 0 && !inicioIt(&it));
	}

	//se a categoria é a raiz
	if (strcmp(s.categoria, "luof") == 0)
		fEscreveLuof(db);
	else
		fEscreveArquivoCat(db, c->nome);

}

void fAdicionaCatLuof(sBanco *db, sSite s, sCat *c) {

	char nomeArqCat[TAMLINKARQ];
	sCat cNova;//para a categoria ser inserida na árvore de categorias
	FILE *nCat;//usado para criar um novo arquivo caso necessário

	//seta os valores da categoria para ser inserida na árvore
	strcpy(cNova.nome, s.nome);
	cNova.catFilhos = criaLista(struct sCat);

	//adiciona a categoria nova na árvore de categorias
	pushBackList(c->catFilhos, &cNova);

	//se a categoria pai não for a raiz atualiza o arquivo aLuof (se for, o arquivo será atualizado ao usar fAdicionaFavorito())
	if (strcmp(s.categoria, "luof") != 0)
		fEscreveLuof(db);

	//cria o arquivo referente a categoria caso ainda não exista nenhum arquivo com esse nome
	//se não entrar no if significa que a categoria pai possui o mesmo nome da nova categoria
	if (strcmp(c->nome, cNova.nome) != 0) {

		fSetaCaminhoArquivo(db, nomeArqCat, s.nome);
		nCat = fopen(nomeArqCat, "r");

		//se nCat == NULL significa que o arquivo não existe, então ele é criado e em ambos os casos fechado
		if (!nCat)
			nCat = fopen(nomeArqCat, "w");

		fclose(nCat);
	}

}

void fRemoveCatLuof(sBanco *db, sSite s, sCat *c) {

	int encontrou = 0;//diz se encontrou a categoria
	sSite *siteDoIterador;
	sIterador it = criaIt(c->catFilhos);//para atualizar o arquivo aLuof

	//remove a categoria da árvore de categorias
	do {
		siteDoIterador = (struct sSite*) retornaItera(&it);
		if (strcmp(siteDoIterador->nome, s.nome) == 0) {
			encontrou = 1;
			removeIt(&it);
		}
		else {
			iteraProximo(&it);
		}
	} while (encontrou == 0 && !inicioIt(&it));

	fEscreveLuof(db);

}

void fRemoveArqCat(sBanco *db, sSite s) {

	//para a categoria que terá seus sites removidos
	sSite *siteDoIterador;
	sCat cNova;
	sBanco dbNew;
	sIterador it;
	int qtdLista;
	char nomeArqCat[TAMLINKARQ], categoria[TAMCAMINHO];

	//seta os valores da categoria para ser criada a lista com seus sites
	strcpy(cNova.nome, s.nome);
	cNova.catFilhos = criaLista(struct sCat);

	//cria uma lista de favoritos pertecentes à categoria
	if (fPreencheListaSite(db, &cNova)) {
		printf(ANSI_COLOR_RED "Erro: Não foi possível abrir o arquivo da categoria excluida.\n");
		return;
	}

	//se a lista estiver vazia exclui o arquivo
	if (emptyList(db->listaSites)) {
		fSetaCaminhoArquivo(db, nomeArqCat, s.nome);
		remove(nomeArqCat);
		return;
	}

	//seta em categoria o caminho da categoria excluida
	if (strcmp(s.categoria, "luof") == 0) {
		strcpy(categoria, s.nome);
	}
	else {
		strcpy(categoria, s.categoria);
		fIncrementaCamCat(categoria, s.nome);
	}

	//remove os sites da categoria na listaSites
	it = criaIt(db->listaSites);//para atualizar o arquivo aCat
	do {
		qtdLista = sizeList(db->listaSites);
		siteDoIterador = (struct sSite*) retornaItera(&it);
		if (strcmp(siteDoIterador->categoria, categoria) == 0) {

			//se o favorito for uma categoria, ela também é removida
			if (siteDoIterador->ehCat == '1') {
				dbNew = *db;
				fRemoveArqCat(&dbNew, *siteDoIterador);
			}

			removeIt(&it);
		}
		else {
			iteraProximo(&it);
		}
	} while (!emptyList(db->listaSites) && (!inicioIt(&it) || qtdLista != sizeList(db->listaSites)));

	//se a lista da categoria ficou vazia remove o arquivo, caso contrário só atualiza o arquivo
	if (emptyList(db->listaSites)) {
		fSetaCaminhoArquivo(db, nomeArqCat, s.nome);
		remove(nomeArqCat);
	}
	else {
		fEscreveArquivoCat(db, s.nome);
	}

}
