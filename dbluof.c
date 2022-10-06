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

	char dir[300];
	char vBooleana;
	struct stat st;
	
	//seta a variável caminhoDB com o caminho para o banco de dados
	/* --- quando caminhoDB estiver com o caminho da home do usuário ---
	char *login;
	login = getlogin();
	sprintf(caminhoDB, "/home/%s/.luof/", login);
	------------------------------------------------------------------*/
	strcpy(caminhoDB, ".luof/");
	tamCaminhoDB = strlen(caminhoDB);
	
	strcpy(dir, caminhoDB);

	if (stat(caminhoDB, &st) == -1) {//se ainda não existe o DB
		printf("Criar novo Banco de dados? [s/n]: ");
		scanf(" %c", &vBooleana);
		if (vBooleana != 's') {
			printf("Saindo...\n");
			return 1;
		}

		//cria diretório .luof
		if (mkdir(caminhoDB, S_IRWXU) == -1) {
			printf("Erro: não foi possível criar diretório na home do usuário\n");
			return 1;
		}

		//cria banco de dados
		fSetaCaminhoArquivo(dir, "luof");
		db->aLuof = fopen(dir, "w");
		if (db->aLuof == NULL) {
			printf("Erro: não foi possível criar banco de dados\n");
			return 1;
		}

		printf("Banco de dados criado com sucesso\n\n");
	}
	else {//se existe o DB
		fSetaCaminhoArquivo(dir, "luof");
		db->aLuof = fopen(dir, "r");
		if (db->aLuof == NULL) {
			printf("Erro: Não foi possível acessar o banco de dados\n");
			return 1;
		}
	}

	return 0;
}

void fFinalizaDB(sBanco *db) {
	
	//fecha arquivos
	if (db->aLuof)
		fclose(db->aLuof);
	if (db->aCat)
		fclose(db->aCat);

	//libera estruturas do banco
	if (db->listaCategorias) {
		fLiberaCats(db->listaCategorias);
		freeList(db->listaCategorias);
	}
	if (db->listaSites != db->raiz)
		freeList(db->listaSites);
	if (db->raiz)
		freeList(db->raiz);

}

void fSetaCaminhoArquivo(char *arq, char *nome) {
	strcpy(arq, caminhoDB);
	strcpy(&arq[tamCaminhoDB], nome);
}

void fLiberaCats(sLista listaCategorias) {
	
	if (!emptyList(listaCategorias)) {
		sIterador it = criaIt(listaCategorias);
		do {
			sCat *cat = (struct sCat*) retornaItera(&it);
			fLiberaCats(cat->catFilhos);
			//libera a lista na volta da recursão
			freeList(cat->catFilhos);
			//--------------
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}
}

void fPreencheListaCat(sBanco *db) {

	//informa se hove uma mudança entre categoriaPai para categoriaFilho (mudança na hierarquia) - Hie de hierarquia, N de novo, A de antigo
	int nivelHieN = 0, nivelHieA = 0;
	char linhaCat[100], *rFgets;

	//será usado para inserir os elementos na lista
	sCat c, *swapFilho;

	//cria uma pilha para guardar a lista da categoriaPai, e verificar seus filhos
	sPilha p = criaPilha(struct sLista *);//FOI MUDADO PARA GUARDAR PONTEIRO DA LISTA

	//cria a lista para inserir as categorias
	sLista l = criaLista(struct sCat);

	sLista temp = l;//usado para quando a lista usada mudar

	//pega linha por linha do arquivo .luof/luof
	//while (fgets(linhaCat, 100, db->aLuof) != NULL) {	
	rFgets = fgets(linhaCat, 100, db->aLuof);
	while (rFgets != NULL && strcmp(linhaCat, "##\n") != 0) {

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

void fPreencheRaiz(sBanco *db) {
	
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

void fNomesCats(sBanco *db, sCat *cat, char h[], int hierarquia) {
	
	//int tamCat = strlen(categoria);
	sIterador it;
	sSite *siteDoIterador;
	sCat *catF;
	int contEspaco;

	fPreencheListaSite(db, cat);
	contEspaco = sizeList(db->listaSites);

	//printa o nome da categoria
	if (hierarquia != 0) {
		for (int i = 0; i < hierarquia; i++) {
			if (h[i] == '1' || i+1 == hierarquia)
				printf("    |");
			else
				printf("     ");
		}
		printf("_ ");
	}
	printf("* %s\n", cat->nome);
	
	//pega as categorias filhas - continua a recursão
	if (!emptyList(cat->catFilhos)) {
		//FUNCAO QUE ORDENA LISTA - FAZER
		it = criaIt(cat->catFilhos);
		do {
			catF = retornaItera(&it);
			if (contEspaco <= 0)
				h[hierarquia] = '0';
			contEspaco--;
			fNomesCats(db, catF, h, hierarquia + 1);
			h[hierarquia] = '1';
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}
	
	//printa os sites da categoria
	fPreencheListaSite(db, cat);
	if (!emptyList(db->listaSites)) {
		it = criaIt(db->listaSites);
		do {
			siteDoIterador = retornaItera(&it);
			if (siteDoIterador->ehCat == '0') {
				for (int i = 0; i < hierarquia + 1; i++) {
					if (h[i] == '1')
						printf("    |");
					else
						printf("     ");
				}
				printf("_ %s\n", siteDoIterador->nome);
			}
			iteraProximo(&it);
		} while(!inicioIt(&it));
	}

}
