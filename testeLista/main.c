#include <stdio.h>
#include <string.h>
#include "lista-iterador.h"

typedef struct sPessoa {
	int idade;
	char nome[50];
} sPessoa;

int main() {
	
	sPessoa p, *pe;
	sLista l;
	sIterador it;

	l = criaLista(struct sPessoa);
	
	printf("Lista\n\tqtd: %ld\n\tsize: %ld\n", l->qtd, l->size);
	
	p.idade = 10;
	strcpy(p.nome, "Ana");
	pushBackList(l, &p);
	
	p.idade = 12;
	strcpy(p.nome, "Bruno");
	pushFrontList(l, &p);
	
	pe = (struct sPessoa*) backList(l);
	printf("Lista\n\tqtd: %ld\n\tsize: %ld\n", l->qtd, l->size);
	printf("p1: nome: %s, idade: %d\n", pe->nome, pe->idade);
	
	pe = (struct sPessoa*) frontList(l);
	printf("p2: nome: %s, idade: %d\n", pe->nome, pe->idade);
	
	popFrontList(l);
	pe = (struct sPessoa*) backList(l);
	printf("Lista\n\tqtd: %ld\n\tsize: %ld\n", l->qtd, l->size);
	printf("p1: nome: %s, idade: %d\n", pe->nome, pe->idade);
	
	printf("Qtd Lista: %ld\n", sizeList(l));

	p.idade = 14;
	strcpy(p.nome, "Carla");
	pushBackList(l, &p);

	p.idade = 16;
	strcpy(p.nome, "Diego");
	pushBackList(l, &p);

	p.idade = 20;
	strcpy(p.nome, "Fabio");
	pushBackList(l, &p);

	it = criaIt(l);

	do {
		pe = retornaItera(&it);
		iteraProximo(&it);
		printf("Nome: %s - idade: %d\n", pe->nome, pe->idade);
	} while (!inicioIt(&it));

	freeList(l);

	return 0;

}
