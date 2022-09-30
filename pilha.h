#ifndef PILHA_H
#define PILHA_H

/* Cria uma pilha
 * Deve ser passado como argumento o tipo de dado que deseja guardar na pilha
 * Retorna a pilha já criada
 * Ex:	sPilha p = criaPilha(struct node);
 * */
#define criaPilha(type) allocStack(sizeof(type))

typedef struct sItem {
	void *elem;
	struct sItem *ant;
} sItem;

typedef struct sPilha {
	struct sItem *topo;
	long altura;
	long size;
} sPilha;

sPilha allocStack(long size);

/* Insere um elemento na pilha
 * Deve ser passado como primeiro argumento o endereço da pilha usada, e segundo argumento o endereço do elemento que será guardado
 * Não tem retorno
 * Ex:	pushStack(&p, &e);
 * */
void pushStack(sPilha *p, void *e);

/* Remove o topo da pilha
 * Deve ser passado como argumento o endereço da pilha usada
 * Retorna 1 caso tenha sido removido o elemento, e 0 caso a pilha esteja vazia
 * Ex:	popStack(&p);
 * */
int popStack(sPilha *p);

/* Retorna um ponteiro para o topo da pilha
 * Deve ser passado como argumento o endereço da pilha usada
 * Retorna um ponteiro para o conteúdo no topo da pilha (deve ser usado type casting), caso a pilha esteja vazia retorna NULL
 * Ex:	struct node *n = (struct node*) topStack(&p);
 * */
void* topStack(sPilha *p);

/* Informa se a pilha está vazia
 * Deve ser passado como argumento o endereço da pilha usada
 * Retorna 1 caso a pilha esteja vazia, e 0 caso tenha ao menos um elemento
 * Ex:	if (emptyStack(&p)) {}
 * */
int emptyStack(sPilha *p);

/* Informa o tamanho da pilha
 * Deve ser passado como argumento o endereço da pilha usada
 * Retorna quantos elementos a pilha possui
 * Ex:	long tamPilha = sizeStack(&p);
 * */
long sizeStack(sPilha *p);

/* Libera a pilha
 * Deve ser passado como argumento o endereço da pilha que deve ser liberada
 * Não tem retorno
 * Ex:	freeStack(&p);
 * */
void freeStack(sPilha *p);

#endif
