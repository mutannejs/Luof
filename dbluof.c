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
