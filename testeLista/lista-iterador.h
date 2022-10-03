/* Lista duplamente encadeada com iterador
 * Última modificação:	25/08/22
 * linha 25: adicionado *
 * */

#ifndef LISTAIT_H
#define LISTAIT_H

/* Cria uma lista
 * Deve ser passado como argumento o tipo de dado que deseja guardar na lista
 * Retorna a lista já criada
 * Ex:	sLista l = criaLista(double);
 * */
#define criaLista(type) allocList(sizeof(type))

typedef struct sNode {
	void *elem;
	struct sNode *ant, *prox;
} sNode;

typedef struct sLista {
	long qtd;
	size_t size;
	struct sNode *sentinela;
} *sLista;

typedef struct sIterador {
	struct sNode *it;
	struct sLista *lista;
} sIterador;

sLista allocList(long size);

/* Insere um elemento no fim da lista
 * Deve ser passado como primeiro argumento o endereço da lista usada, e segundo argumento o endereço do elemento que será guardado
 * Não tem retorno
 * Ex:	pushBackList(&l, &e);
 * */
void pushBackList(sLista l, void *e);

/* Insere um elemento no início da lista
 * Deve ser passado como primeiro argumento o endereço da lista usada, e segundo argumento o endereço do elemento que será guardado
 * Não tem retorno
 * Ex:	pushFrontList(&l, &e);
 * */
void pushFrontList(sLista l, void *e);

/* Remove o elemento no fim da lista
 * Deve ser passado como argumento o endereço da lista usada
 * Retorna 1 caso tenha sido removido o elemento, e 0 caso a lista esteja vazia
 * Ex:	popBackList(&l);
 * */
int popBackList(sLista l);

/* Remove o elemento no início da lista
 * Deve ser passado como argumento o endereço da lista usada
 * Retorna 1 caso tenha sido removido o elemento, e 0 caso a lista esteja vazia
 * Ex:	popFrontList(&l);
 * */
int popFrontList(sLista l);

/* Retorna um ponteiro para o fim da lista
 * Deve ser passado como argumento o endereço da lista usada
 * Retorna um ponteiro para o conteúdo no fim da lista (deve ser usado type casting), caso a lista esteja vazia retorna NULL
 * Ex:	double *d = (double*) backList(&l);
 * */
void* backList(sLista l);

/* Retorna um ponteiro para o início da lista
 * Deve ser passado como argumento o endereço da lista usada
 * Retorna um ponteiro para o conteúdo no início da lista (deve ser usado type casting), caso a lista esteja vazia retorna NULL
 * Ex:	double *d = (double*) frontList(&l);
 * */
void* frontList(sLista l);

/* Informa se a lista está vazia
 * Deve ser passado como argumento o endereço da lista usada
 * Retorna 1 caso a lista esteja vazia, e 0 caso tenha ao menos um elemento
 * Ex:	if (emptyList(&l)) {}
 * */
int emptyList(sLista l);

/* Informa o tamanho da lista
 * Deve ser passado como argumento o endereço da lista usada
 * Retorna quantos elementos a lista possui
 * Ex:	long tamLista = sizeList(&l);
 * */
long sizeList(sLista l);

/* Libera a lista
 * Deve ser passado como argumento o endereço da lista que deve ser liberada
 * Não tem retorno
 * Ex:	freeList(&l);
 * */
void freeList(sLista l);


/* Iterador*/

/* Cria um iterador para uma lista
 * Deve ser passado como argumento o endereço da lista usada
 * Retorna um iterador para a lista, se referindo para o início dela
 * Ex:	sIterador i = criaIt(&l);
 * */
sIterador criaIt(sLista l);

/* Itera para o elemento anterior na lista
 * Deve ser passado como argumento o endereço do iterador usado
 * Não tem retorno
 * Ex:	iteraAnterior(&i);
 * */
void iteraAnterior(sIterador *i);

/* Itera para o próximo elemento na lista
 * Deve ser passado como argumento o endereço do iterador usado
 * Não tem retorno
 * Ex:	iteraProximo(&i);
 * */
void iteraProximo(sIterador *i);

/* Itera para o primeiro elemento na lista
 * Deve ser passado como argumento o endereço do iterador usado
 * Não tem retorno
 * Ex:	iteraInicio(&i);
 * */
void iteraInicio(sIterador *i);

/* Itera para o último elemento na lista
 * Deve ser passado como argumento o endereço do iterador usado
 * Não tem retorno
 * Ex:	iteraFim(&i);
 * */
void iteraFim(sIterador *i);

/* Insere um elemento na lista antes da posição atual do iterador
 * Deve ser passado como primeiro argumento o endereço do iterador usado, e segundo argumento o endereço do elemento que será guardado
 * Não tem retorno
 * Ex:	insereAntIt(&i, &e);
 * */
void insereAntIt(sIterador *i, void *e);

/* Insere um elemento na lista depois da posição atual do iterador
 * Deve ser passado como primeiro argumento o endereço do iterador usado, e segundo argumento o endereço do elemento que será guardado
 * Não tem retorno
 * Ex:	insereProxIt(&i, &e);
 * */
void insereProxIt(sIterador *i, void *e);

/* Remove o elemento da lista que está sendo referenciado pelo iterador
 * Deve ser passado como argumento o endereço do iterador usado (o iterador passa a apontar para o elemento seguinte)
 * Retorna 1 caso tenha sido removido o elemento, e 0 caso a lista esteja vazia
 * Ex:	removeIt(&i, &e);
 * */
int removeIt(sIterador *i);

/* Remove o elemento que está na lista antes da posição atual do iterador
 * Deve ser passado como argumento o endereço do iterador usado
 * Retorna 1 caso tenha sido removido o elemento, e 0 caso a lista esteja vazia
 * Ex:	removeAntIt(&i, &e);
 * */
int removeAntIt(sIterador *i);

/* Remove o elemento que está na lista depois da posição atual do iterador
 * Deve ser passado como argumento o endereço do iterador usado
 * Retorna 1 caso tenha sido removido o elemento, e 0 caso a lista esteja vazia
 * Ex:	removeProxIt(&i, &e);
 * */
int removeProxIt(sIterador *i);

/* Retorna um ponteiro para o elemento referenciado pelo iterador
 * Deve ser passado como argumento o endereço do iterador usado
 * Retorna um ponteiro para o conteúdo na lista referenciado pelo iterador (deve ser usado type casting), caso a lista esteja vazia retorna NULL
 * Ex:	double *d = (double*) retornaItera(&i);
 * */
void* retornaItera(sIterador *i);

/* Informa se o iterador está referenciando para o início da lista
 * Deve ser passado como argumento o endereço do iterador usado
 * Retorna 1 caso o iterador esteja referenciando para o início da lista, e 0 caso não esteja
 * Ex:	if (inicioIt(&i)) {}
 * */
int inicioIt(sIterador *i);

/* Informa se o iterador está referenciando para o fim da lista
 * Deve ser passado como argumento o endereço do iterador usado
 * Retorna 1 caso o iterador esteja referenciando para o fim da lista, e 0 caso não esteja
 * Ex:	if (fimIt(&i)) {}
 * */
int fimIt(sIterador *i);

// Exemplo de loop usando iterador, do início ao fim da lista
/* 
 * sIterador it = criaIt(&l);
 * do {
 * 	//processo
 * 	iteraProximo(&it);
 * } while (inicioIt(&it));
 * 
 * */

#endif
