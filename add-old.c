#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "luof.h"

void fAddSite() {

	//variaveis
	sSite s;
	sLista listaCategoria;
	sCat categoria;
	char nomeArqCat[110];//guardará o conteudo do arquivo da categoria para fazer a cópia e inserção do novo site
	FILE *aLuof;//arquivo com as categorias
	FILE *aCat;//arquivo da categoria especifica

	// inicializa o banco de dados (se existir guarda em aLuof o arquivo com as categorias, se não pergunta se quer cria-lo)
	int rInicializaDB = fInicializaDB(&aLuof);
	if (rInicializaDB)
		return;

	//preenche uma sLista com todas as categorias
	listaCategoria = preencheCat(&aLuof);

	//Pede o nome e a categoria
	printf("Nome:\n");
	scanf(" %[^\n]", s.nome);

	printf("Categoria:\n");
	scanf(" %[^\n]", s.categoria);

	//verifica se a categoria existe
	char *rBuscaCat = fBuscaCat(listaCategoria, s, &categoria);
	if (rBuscaCat != NULL) {
		printf("Categoria \"%s\" não encontrada.\n", rBuscaCat);
		return;
	}
	//verifica se já existe um favorito com esse nome na categoria e se o arquivo pode ser aberto
	int rBuscaFavorito = fBuscaFavorito(&aCat, s, categoria, '0');
	switch (rBuscaFavorito) {
		case 0:	printf("Arquivo referente à categoria não encontrado.\n");
			return;
		case 1:	printf("O site já existe.\n");
			return;
	}

	//Pede o link e comentario
	printf("Link:\n");
	scanf(" %[^\n]", s.link);

	printf("Texto:\n");
	scanf(" %[^\n]", s.texto);

	//prenche o campo ehCat
	s.ehCat = '0';

	//adiciona no banco de dados
	strcpy(nomeArqCat, ".luof/");
	strcpy(&nomeArqCat[6], categoria.nome);
	aCat = freopen(nomeArqCat, "a", aCat);//reabre o arquivo para escrever no fim dele
	fprintf(aCat, "%s\n%s\n%s\n%s\n%c\n", s.nome, s.categoria, s.link, s.texto, s.ehCat);

	//fecha os arquivos abertos
	fFinalizaDB(&aLuof, &aCat);
}

/*void percurso(sLista l) {
	if (!emptyList(l)) {
		printf("inicio lista\n");
		sIterador it = criaIt(l);
		do {
			sCat *cat = (struct sCat*) retornaItera(&it);
			printf("\t%s\n", cat->nome);
			percurso(cat->catFilhos);
			//
			//printf("\t%ld\n", sizeList(cat->catFilhos));
			//
			iteraProximo(&it);
		} while (!inicioIt(&it));
		printf("fim lista\n");
	}
	else {
		printf("lista vazia\n");
	}
}*/

void fAddCategory() {

	//variaveis
	sSite c;//será usado para guardar a categoria (como será armazenada)
	sLista listaCategoria;
	sCat categoria;
	char nomeArqCat[110];//guardará o conteudo do arquivo da categoria para fazer a cópia e inserção do novo site
	FILE *aLuof;//arquivo com as categorias
	FILE *aCat;//arquivo da categoria especifica

	// inicializa o banco de dados (se existir guarda em aLuof o arquivo com as categorias, se não pergunta se quer cria-lo)
	int rInicializaDB = fInicializaDB(&aLuof);
	if (rInicializaDB)
		return;

	//preenche uma sLista com todas as categorias
	listaCategoria = preencheCat(&aLuof);

	//Pede a categoria
	printf("Categoria pai:\n");
	scanf(" %[^\n]", c.categoria);

	printf("Categoria:\n");
	scanf(" %[^\n]", c.nome);

	//preenche os outros campos
	strcpy(c.link, "@categoria@");
	strcpy(c.texto, "@categoria@");
	c.ehCat = '1';

	//verifica se a categoria pai existe
	char *rBuscaCat = fBuscaCat(listaCategoria, c, &categoria);
	if (rBuscaCat != NULL) {
		printf("Categoria \"%s\" não encontrada.\n", rBuscaCat);
		return;
	}
	//verifica se já existe a categoria e se o arquivo pode ser aberto
	int rBuscaFavorito = fBuscaFavorito(&aCat, c, categoria, '1');
	switch (rBuscaFavorito) {
		case 0:	printf("Arquivo referente à categoria não encontrado.\n");
			return;
		case 1:	printf("A categoria já existe.\n");
			return;
	}

	fFinalizaDB(&aLuof, &aCat);

}
