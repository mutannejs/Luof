install: compile create-db move

compile:
#	gcc -o luof main.c add-remove.c list.c modify.c dbluof.c dbcat.c modulos.c help.c backup.c import-export.c teste.c lista-iterador.c 
	gcc -o luof main.c add-remove.c list.c dbluof.c dbcat.c modulos.c teste.c lista-iterador.c 
	@ chmod 777 luof

create-db:
	@ mkdir ~/.luof
	@ chmod u+rxw ~/.luof
	@ touch ~/.luof/luof
	@ chmod u+rxw ~/.luof/luof

move:
	@ sudo mv luof /usr/bin/

desinstall:
	@ rm -r ~/.luof
	@ sudo rm /usr/bin/luof
