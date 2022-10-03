#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "luof.h"

/* Lembrando que antes do programa estar em total funcionamento, tudo que será criado estará na pasta atual do programa, portanto, no final será necessário modificar todos os caminhos para o programa usar a home do usuario.
 * */
int fInicializaDB(sBanco *db) {

	char dir[50] = ".luof";//mudar para /home/$usuario/.luof
	char vBooleana;
	struct stat st;

	if (stat(dir, &st) == -1) {//se ainda não existe o DB
		printf("Criar novo Banco de dados? [s/n]: ");
		scanf(" %c", &vBooleana);
		if (vBooleana != 's') {
			printf("Saindo...\n");
			return 1;
		}

		//cria diretório .luof
		if (mkdir(dir, S_IRWXU) == -1) {
			printf("Erro: não foi possível criar diretório na home do usuário\n");
			return 1;
		}

		//cria banco de dados
		strcpy(dir, ".luof/luof");
		db->aLuof = fopen(dir, "w");
		if (db->aLuof == NULL) {
			printf("Erro: não foi possível criar banco de dados\n");
			return 1;
		}

		printf("Banco de dados criado com sucesso\n\n");
	}
	else {//se existe o DB
		strcpy(dir, ".luof/luof");
		db->aLuof = fopen(dir, "r");
		if (db->aLuof == NULL) {
			printf("Erro: Não foi possível acessar o banco de dados\n");
			return 1;
		}
	}

	return 0;
}

void fFinalizaDB(sBanco *db) {
	
	if(db->aLuof)
		fclose(db->aLuof);
	if(db->aCat)
		fclose(db->aCat);

}

void fPreencheListaCat(sBanco *db) {

	//informa se hove uma mudança entre categoriaPai para categoriaFilho (mudança na hierarquia) - Hie de hierarquia, N de novo, A de antigo
	int nivelHieN = 0, nivelHieA = 0;
	char linhaCat[100];

	//será usado para inserir os elementos na lista
	sCat c, *swapFilho;

	//cria uma pilha para guardar a lista da categoriaPai, e verificar seus filhos
	sPilha p = criaPilha(struct sLista *);//FOI MUDADO PARA GUARDAR PONTEIRO DA LISTA

	//cria a lista para inserir as categorias
	sLista l = criaLista(struct sCat);

	sLista temp = l;//usado para quando a lista usada mudar

	//pega linha por linha do arquivo .luof/luof
	//while (fgets(linhaCat, 100, db->aLuof) != NULL) {	
	fgets(linhaCat, 100, db->aLuof);
	while (strcmp(linhaCat, "##\n") != 0) {

		//muda o \n no fim da linha para \0
		int tamLinha = strlen(linhaCat);
		linhaCat[tamLinha-1] = '\0';
		
		//pega o nivel da categoria da linha atual. Como pega apenas o primeiro caracter, implica que exista apenas 10 niveis de categorias
		nivelHieN = linhaCat[0] - 48;//transformando char em int

		if (nivelHieN == nivelHieA) {//continuou no mesmo nivel
			strcpy(c.nome, &linhaCat[1]);
			c.catFilhos = criaLista(struct sCat);

			pushBackList(temp, &c);
		}
		else if (nivelHieN > nivelHieA) {//subiu um nivel (agora é categoriaFilho)
			pushStack(&p, &temp);//guarda o endereço da lista atual,
			swapFilho = (struct sCat *) backList(temp);
			temp = swapFilho->catFilhos;//temp aponta para a lista da categoria atual

			strcpy(c.nome, &linhaCat[1]);
			c.catFilhos = criaLista(struct sCat);

			pushBackList(temp, &c);
		}
		else {//desceu um nivel
			//for usado para retirar da pilha as listas vazias (categorias sem subcategorias)
			for (int i = 1; i < nivelHieA - nivelHieN; i++)
				popStack(&p);
				
			temp = *((struct sLista**) topStack(&p));

			strcpy(c.nome, &linhaCat[1]);
			c.catFilhos = criaLista(struct sCat);

			pushBackList(temp, &c);
			popStack(&p);
		}

		nivelHieA = nivelHieN;
		
		fgets(linhaCat, 100, db->aLuof);
	
	}

	freeStack(&p);
	
	db->listaCategorias = l;
	
	fPreencheRaiz(db);

}

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
	char nomeArqCat[110];

	//usado para percorrer a lista e adicionar o site no lugar correto
	//sIterador it = criaIt(db->listaSites);
	//sSite *siteDoIterador;
	//int encontrouPos;

	//abre o arquivo da categoria
	strcpy(nomeArqCat, ".luof/");
	strcpy(&nomeArqCat[6], c->nome);
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

int fPreencheRaiz(sBanco *db) {
	
	//usado para armazenar temporariamente os sites
	sSite siteTemp;
	char nomeTemp[100], ehCategoria[3];
	int tamanho;

	db->raiz = criaLista(struct sSite);

	//pega linha por linha do arquivo aLuof(logo após "##\n"), faz a comparação usando a primeira linha de um site, e depois pega as outras três referentes ao mesmo site
        while (fgets(nomeTemp, 100, db->aLuof) != NULL) {

        //guarda em siteTemp o nome
        tamanho = strlen(nomeTemp);
        nomeTemp[tamanho-1] = '\0';
		strcpy(siteTemp.nome, nomeTemp);

		//guarda em siteTemp a categoria
		fgets(siteTemp.categoria, 1000, db->aLuof);
		tamanho = strlen(siteTemp.categoria);
		siteTemp.categoria[tamanho-1] = '\0';

		//guarda em siteTemp o link
		fgets(siteTemp.link, 500, db->aLuof);
		tamanho = strlen(siteTemp.link);
		siteTemp.link[tamanho-1] = '\0';

		//guarda em siteTemp o texto
		fgets(siteTemp.texto, 5000, db->aLuof);
		tamanho = strlen(siteTemp.texto);
		siteTemp.texto[tamanho-1] = '\0';

		//guarda em siteTemp o ehCat e ignora o '\n'
		fgets(ehCategoria, 3, db->aLuof);
		siteTemp.ehCat = ehCategoria[0];

		//adiciona o site na lista
		pushBackList(db->raiz, &siteTemp);

	}

	return 1;
	
}

int fBuscaFavorito(sBanco *db, sSite *s, char favorito) {

	//variaveis para percorrer a lista
	sIterador it = criaIt(db->listaSites);
	sSite *fav;

	//informa se encontrou o site
	int encontrou = 0;

	do {
		fav = retornaItera(&it);
		//encontrou o favorito
		if (strcmp(fav->nome, s->nome) == 0 && strcmp(fav->categoria, s->categoria) == 0 && s->ehCat == favorito) {
			//seta os outros valores de s
			strcpy(s->link, fav->link);
			strcpy(s->texto, fav->texto);
			//informa que o site foi encontrado
			return 1;
		}
		iteraProximo(&it);
	} while (encontrou == 0 && !inicioIt(&it));

	//não encontrou o site
	return 0;

}

void fAdicionaFavorito(sBanco *db, sSite s, sCat c) {

	char nomeArqCat[110];
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
		insereAntIt(&it, &s);
	}

	//se a categoria é a raiz
	if (strcmp(s.categoria, "luof") == 0) {
		//reabre o arquivo aLuof para sobreescreve-lo
		strcpy(nomeArqCat, ".luof/luof");
		db->aLuof = freopen(nomeArqCat, "w", db->aLuof);

		//escreve a árvore de categorias no arquivo aLuof
		fEscreveLuof(db, db->listaCategorias, 0);
		
		//marca o fim da árvore de categorias
		fprintf(db->aLuof, "##\n");
		
		//escreve os favoritos da raiz
		iteraInicio(&it);
		do {
			siteDoIterador = retornaItera(&it);
			fprintf(db->aLuof, "%s\n%s\n%s\n%s\n%c\n", siteDoIterador->nome, siteDoIterador->categoria, siteDoIterador->link, siteDoIterador->texto, siteDoIterador->ehCat);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}
	else {
		//reabre o arquivo para sobreescreve-lo
		strcpy(nomeArqCat, ".luof/");
		strcpy(&nomeArqCat[6], c.nome);
		db->aCat = freopen(nomeArqCat, "w", db->aCat);

		//escreve a lista no arquivo da categoria
		iteraInicio(&it);
		do {
			siteDoIterador = retornaItera(&it);
			fprintf(db->aCat, "%s\n%s\n%s\n%s\n%c\n", siteDoIterador->nome, siteDoIterador->categoria, siteDoIterador->link, siteDoIterador->texto, siteDoIterador->ehCat);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}

}

void fAdiocionaCatLuof(sBanco *db, sSite s, sCat c) {
}

void fEscreveLuof(sBanco *db, sLista listaCategorias, int hierarquia) {
	
	if (!emptyList(listaCategorias)) {
		sIterador it = criaIt(listaCategorias);
		do {
			sCat *cat = (struct sCat*) retornaItera(&it);
			//escreve no arquivo
			fprintf(db->aLuof, "%d%s\n", hierarquia, cat->nome);
			//------------------
			fEscreveLuof(db, cat->catFilhos, hierarquia+1);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}
}
