/* Lista duplamente encadeada com iterador
 * Última modificação:	03/07/22
 * 
 * */

#include <stdlib.h>
#include <string.h>
#include "lista-iterador.h"

sLista allocList(long size) {
	//aloca lista
	sLista l = malloc(sizeof(struct sLista));
	//seta valores da lista
	l->qtd = 0;
	l->size = size;
	l->sentinela = malloc(sizeof(sNode));
	//encadeia o sentinela
	l->sentinela->ant = l->sentinela->prox = l->sentinela;
	//retorna o endereço da lista
	return l;
}

void pushBackList(sLista l, void *e) {
	//aloca um node
	sNode *no = (sNode*) malloc(sizeof(sNode));
	//seta os valores do node
	no->elem = malloc(l->size);
	memmove(no->elem, e, l->size);
	no->ant = l->sentinela->ant;
	no->prox = l->sentinela;
	//atualiza a lista
	l->sentinela->ant->prox = no;
	l->sentinela->ant = no;
	l->qtd++;
}

void pushFrontList(sLista l, void *e) {
	//aloca um node
	sNode *no = (sNode*) malloc(sizeof(sNode));
	//seta os valores do node
	no->elem = malloc(l->size);
	memmove(no->elem, e, l->size);
	no->ant = l->sentinela;
	no->prox = l->sentinela->prox;
	//atualiza a lista
	l->sentinela->prox->ant = no;
	l->sentinela->prox = no;
	l->qtd++;
}

int popBackList(sLista l) {
	//se não houver nodes
	if (emptyList(l))
		return 0;
	//aloca um node
	sNode *no = l->sentinela->ant;
	//atualiza o encadeamento da lista
	l->sentinela->ant = no->ant;
	l->sentinela->ant->prox = no->prox;
	l->sentinela->prox->ant = no->ant;
	//libera o node e atualiza a qtd de nós da lista
	free(no->elem);
	free(no);
	l->qtd--;
	return 1;
}

int popFrontList(sLista l) {
	//se não houver nodes
	if (emptyList(l))
		return 0;
	//aloca um node
	sNode *no = l->sentinela->prox;
	//atualiza o encadeamento da lista
	l->sentinela->prox = no->prox;
	l->sentinela->ant->prox = no->prox;
	l->sentinela->prox->ant = no->ant;
	//libera o node e atualiza a qtd de nós da lista
	free(no->elem);
	free(no);
	l->qtd--;
	return 1;
}

void* backList(sLista l) {
	if (emptyList(l))
		return 0;
	return l->sentinela->ant->elem;
}

void* frontList(sLista l) {
	if (emptyList(l))
		return 0;
	return l->sentinela->prox->elem;
}

int emptyList(sLista l) {
	if (l->qtd == 0)
		return 1;
	else
		return 0;
}

long sizeList(sLista l) {
	return l->qtd;
}

void freeList(sLista l) {
	while (popBackList(l));
	free(l->sentinela);
}


// Iterador

sIterador criaIt(sLista l) {
	sIterador i;
	if (emptyList(l))
		i.it = NULL;
	else
		i.it = l->sentinela->prox;
	i.lista = l;
	return i;
}

void iteraAnterior(sIterador *i) {
	if (emptyList(i->lista))
		i->it = NULL;
	else if (i->it->ant == i->lista->sentinela)
		i->it = i->it->ant->ant;
	else
		i->it = i->it->ant;
}

void iteraProximo(sIterador *i) {
	if (emptyList(i->lista))
		i->it = NULL;
	else if (i->it->prox == i->lista->sentinela)
		i->it = i->it->prox->prox;
	else
		i->it = i->it->prox;
}

void iteraInicio(sIterador *i) {
	if (emptyList(i->lista))
		i->it = NULL;
	else
		i->it = i->lista->sentinela->prox;
}

void iteraFim(sIterador *i) {
	if (emptyList(i->lista))
		i->it = NULL;
	else
		i->it = i->lista->sentinela->ant;
}

void insereAntIt(sIterador *i, void *e) {
	if (!emptyList(i->lista)) {
		sNode *no = (sNode*) malloc(sizeof(sNode));
		no->elem = malloc(i->lista->size);
		memmove(no->elem, e, i->lista->size);
		no->ant = i->it->ant;
		no->prox = i->it;
		i->it->ant->prox = no;
		i->it->ant = no;
		i->lista->qtd++;
	}
	else {
		pushFrontList(i->lista, e);
		iteraInicio(i);
	}
}

void insereProxIt(sIterador *i, void *e) {
	if (!emptyList(i->lista)) {
		sNode *no = (sNode*) malloc(sizeof(sNode));
		no->elem = malloc(i->lista->size);
		memmove(no->elem, e, i->lista->size);
		no->ant = i->it;
		no->prox = i->it->prox;
		i->it->prox->ant = no;
		i->it->prox = no;
		i->lista->qtd++;
	}
	else {
		pushBackList(i->lista, e);
		iteraInicio(i);
	}
}

int removeIt(sIterador *i) {
	if (emptyList(i->lista))
		return 0;
	sNode *no = i->it;
	i->it->ant->prox = i->it->prox;
	i->it->prox->ant = i->it->ant;
	if (sizeList(i->lista) == 1)
		i->it = NULL;
	else
		iteraProximo(i);
	free(no->elem);
	free(no);
	i->lista->qtd--;
	return 1;
}

int removeAntIt(sIterador *i) {
	if (emptyList(i->lista))
		return 0;
	
	sNode *no;
	if (sizeList(i->lista) == 1) {
		removeIt(i);
	}
	else if (i->it->ant == i->lista->sentinela) {
		no = i->it->ant->ant;
		i->it->ant->ant->ant->prox = i->it->ant;
		i->it->ant->ant = i->it->ant->ant->ant;
	}
	else {
		no = i->it->ant;
		i->it->ant->ant->prox = i->it;
		i->it->ant = i->it->ant->ant;
	}
	free(no->elem);
	free(no);
	i->lista->qtd--;
	return 1;
}

int removeProxIt(sIterador *i) {
	if (emptyList(i->lista))
		return 0;
		
	sNode *no;
	if (sizeList(i->lista) == 1) {
		removeIt(i);
	}
	else if (i->it->prox == i->lista->sentinela) {
		no = i->it->prox->prox;
		i->it->prox->prox->prox->ant = i->it->prox;
		i->it->prox->prox = i->it->prox->prox->prox;
	}
	else {
		no = i->it->prox;
		i->it->prox->prox->ant = i->it;
		i->it->prox = i->it->prox->prox;
	}
	free(no->elem);
	free(no);
	i->lista->qtd--;
	return 1;
}

void* retornaItera(sIterador *i) {
	if (emptyList(i->lista))
		return NULL;
	else
		return i->it->elem;
}

int inicioIt(sIterador *i) {
	if (i->it == i->lista->sentinela->prox || emptyList(i->lista))
		return 1;
	else
		return 0;
}

int fimIt(sIterador *i) {
	if (i->it == i->lista->sentinela->ant || emptyList(i->lista))
		return 1;
	else
		return 0;
}
