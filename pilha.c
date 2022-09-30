#include <stdlib.h>
#include <string.h>
#include "pilha.h"

sPilha allocStack(long size) {
	sPilha p;
	p.topo = NULL;
	p.size = size;
	p.altura = 0;
	return p;
}

void pushStack(sPilha *p, void *e) {
	sItem *it = (sItem*) malloc(sizeof(sItem));
	it->elem = malloc(p->size);
	memmove(it->elem, e, p->size);
	if (!emptyStack(p))
		it->ant = p->topo;
	p->topo = it;
	p->altura = p->altura + 1;
}

int popStack(sPilha *p) {
	sItem *temp = p->topo;
	if (!emptyStack(p)) {
		p->topo = p->topo->ant;
		free(temp->elem);
		free(temp);
		p->altura = p->altura - 1;
		return 1;
	}
	return 0;
}

void* topStack(sPilha *p) {
	return p->topo->elem;
}

int emptyStack(sPilha *p) {
	if (p->altura == 0)
		return 1;
	else
		return 0;
}

long sizeStack(sPilha *p) {
	return p->altura;
}

void freeStack(sPilha *p) {
	while (popStack(p)) {};
}
