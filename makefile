install: compile create-db move
	@ echo "luof instalado."

compile:
	@ gcc -o luof main.c add-remove.c list.c modify.c dbluof.c dbcat.c modulos.c help.c backup.c import-export.c teste.c lista-iterador.c -std=c99 -Wall -Wextra -Wno-unused-result -Wpedantic -O0
	@ chmod 777 luof

create-db:
	@ mkdir ~/.luof
	@ chmod u+rxw ~/.luof
	@ touch ~/.luof/luof
	@ chmod u+rxw ~/.luof/luof
	@ touch ~/.luof/raiz.luof
	@ chmod u+rxw ~/.luof/raiz.luof

move:
	@ sudo mv luof /usr/bin/

desinstall:
	@ rm -r ~/.luof
	@ sudo rm /usr/bin/luof
