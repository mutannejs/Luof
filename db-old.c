#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "luof.h"

/* Lembrando que antes do programa estar em total funcionamento, tudo que será criado estará na pasta atual do programa, portanto, no final será necessário modificar todos os caminhos para o programa usar a home do usuario.
 * */
int fInicializaDB(FILE **aLuof) {

	char dir[50] = ".luof";//mudar para /home/$usuario/.luof
	char vBooleana;
	struct stat st;

	if (stat(dir, &st) == -1) {//se ainda não existe o DB
		printf("Criar novo Banco de Dados? [s/n]: ");
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
		*aLuof = fopen(dir, "w");
		if (*aLuof == NULL) {
			printf("Erro: não foi possível criar banco de dados\n");
			return 1;
		}
	}
	else {//se existe o DB
		strcpy(dir, ".luof/luof");
		*aLuof = fopen(dir, "r");
		if (*aLuof == NULL) {
			printf("Erro: Não foi possível acessar o banco de dados\n");
			return 1;
		}
	}

	return 0;
}

void fFinalizaDB(FILE **aLuof, FILE **aCat) {
	
	fclose(*aLuof);
	fclose(*aCat);

}

sLista preencheCat(FILE **aLuof) {

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
	while (fgets(linhaCat, 100, *aLuof) != NULL) {

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
	}

	freeStack(&p);

	return l;

}

char* fBuscaCat(sLista l, sSite s, sCat *c) {

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
	sIterador it = criaIt(l);

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
	else
		return NULL;

}

int fBuscaFavorito(FILE **aCat, sSite s, sCat c, char favorito) {

	//usado para armazenar os site da categoria
	sSite siteTemp;
	char nomeTemp[100], nomeArqCat[110], ignorarChar;
	int tamanho;

	//usado para indicar se o site foi encontrado
	int encontrou = 0;

	//abre o arquivo da categoria
	strcpy(nomeArqCat, ".luof/");
	strcpy(&nomeArqCat[6], c.nome);
	printf("%s\n", nomeArqCat);
	*aCat = fopen(nomeArqCat, "r");
	
	//documento não encontrado
	if (*aCat == NULL)
		return 0;

	//pega linha por linha do arquivo da categoria, faz a comparação usando a primeira linha de um site, e depois pega as outras três referentes ao mesmo site
        while (encontrou == 0 && fgets(nomeTemp, 100, *aCat) != NULL) {

                //guarda em siteTemp o nome
                tamanho = strlen(nomeTemp);
                nomeTemp[tamanho-1] = '\0';
		strcpy(siteTemp.nome, nomeTemp);

		//guarda em siteTemp a categoria
		fgets(siteTemp.categoria, 1000, *aCat);
		tamanho = strlen(siteTemp.categoria);
		siteTemp.categoria[tamanho-1] = '\0';

		//guarda em siteTemp o link
		fgets(siteTemp.link, 500, *aCat);
		tamanho = strlen(siteTemp.link);
		siteTemp.link[tamanho-1] = '\0';

		//guarda em siteTemp o texto
		fgets(siteTemp.texto, 5000, *aCat);
		tamanho = strlen(siteTemp.texto);
		siteTemp.texto[tamanho-1] = '\0';

		//guarda em siteTemp o ehCat e ignora o '\n'
		siteTemp.ehCat = fgetc(*aCat);
		ignorarChar = fgetc(*aCat);

		//o site já existe no arquivo
		if (strcmp(siteTemp.nome, s.nome) == 0 && strcmp(siteTemp.categoria, s.categoria) == 0 && siteTemp.ehCat == favorito) {
			encontrou = 1;
			return 1;
		}

	}

	//não encontrou o site
	return 2;

}
