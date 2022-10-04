all: run

run: main.o add-delete.o list.o dbluof.o dbcat.o lista-iterador.o pilha.o teste.o
	gcc -o run main.o add-delete.o list.o dbluof.o dbcat.o lista-iterador.o pilha.o teste.o

main.o: main.c luof.h
	gcc -o main.o main.c -c -W -Wall -pedantic

add.o: add-delete.c luof.h
	gcc -o add-delete.o add-delete.c -c -W -Wall -pedantic

list.o: list.c luof.h
	gcc -o list.o list.c -c -W -Wall -pedantic

dbluof.o: dbluof.c luof.h
	gcc -o dbluof.o dbluof.c -c -W -Wall -pedantic

dbcat.o: dbcat.c luof.h
	gcc -o dbcat.o dbcat.c -c -W -Wall -pedantic

lista-iterador.o: lista-iterador.c lista-iterador.h
	gcc -o lista-iterador.o lista-iterador.c -c -W -Wall -pedantic

pilha.o: pilha.c pilha.h
	gcc -o pilha.o pilha.c -c -W -Wall -pedantic

teste.o: teste.c luof.h
	gcc -o teste.o teste.c -c -W -Wall -pedantic

clean:
	rm *.o run
