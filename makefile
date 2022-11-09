all: luof

luof: main.o add-remove.o list.o modify.o dbluof.o dbcat.o modulos.o help.o lista-iterador.o pilha.o teste.o
	gcc -o luof main.o add-remove.o list.o modify.o dbluof.o dbcat.o modulos.o help.o lista-iterador.o pilha.o teste.o

main.o: main.c luof.h
	gcc -o main.o main.c -c -W -Wall -pedantic

add-remove.o: add-remove.c luof.h
	gcc -o add-remove.o add-remove.c -c -W -Wall -pedantic

list.o: list.c luof.h
	gcc -o list.o list.c -c -W -Wall -pedantic

modify.o: modify.c luof.h
	gcc -o modify.o modify.c -c -W -Wall -pedantic

dbluof.o: dbluof.c luof.h
	gcc -o dbluof.o dbluof.c -c -W -Wall -pedantic

dbcat.o: dbcat.c luof.h
	gcc -o dbcat.o dbcat.c -c -W -Wall -pedantic
	
modulos.o: modulos.c luof.h
	gcc -o modulos.o modulos.c -c -W -Wall -pedantic

help.o: help.c luof.h
	gcc -o help.o help.c -c -W -Wall -pedantic

lista-iterador.o: lista-iterador.c lista-iterador.h
	gcc -o lista-iterador.o lista-iterador.c -c -W -Wall -pedantic

pilha.o: pilha.c pilha.h
	gcc -o pilha.o pilha.c -c -W -Wall -pedantic

teste.o: teste.c luof.h
	gcc -o teste.o teste.c -c -W -Wall -pedantic


clean:
	rm *.o
