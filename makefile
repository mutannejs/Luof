all: run

run: main.o add.o db.o lista-iterador.o pilha.o
	gcc -o run main.o add.o db.o lista-iterador.o pilha.o

main.o: main.c luof.h
	gcc -o main.o main.c -c -W -Wall -pedantic

add.o: add.c luof.h
	gcc -o add.o add.c -c -W -Wall -pedantic

db.o: db.c luof.h
	gcc -o db.o db.c -c -W -Wall -pedantic

lista-iterador.o: lista-iterador.c lista-iterador.h
	gcc -o lista-iterador.o lista-iterador.c -c -W -Wall -pedantic

pilha.o: pilha.c pilha.h
	gcc -o pilha.o pilha.c -c -W -Wall -pedantic

clean:
	rm *.o run
