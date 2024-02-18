# arquivos com extensão .c
C_SOURCE=$(wildcard src/*.c)
# arquivos com extensão .h
H_SOURCE=$(wildcard src/*.h)

install: compile move
	@ echo "luof instalado."

uninstall:
	@ if [ -d ~/.luof/ ]; then rm -r ~/.luof/ && echo "diretório ~/.luof removido"; fi
	@ if [ -e /usr/local/bin/luof ]; then sudo rm /usr/local/bin/luof && echo "binário luof removido"; fi

compile: $(C_SOURCE) $(H_SOURCE)
	@ gcc -o luof $(C_SOURCE) $(H_SOURCE) -std=c99 -Wall -Wextra -Wno-unused-result -Wpedantic -O0

#create-db:
#	@ mkdir ~/.luof
#	@ chmod u+rxw ~/.luof
#	@ touch ~/.luof/luof
#	@ chmod u+rxw ~/.luof/luof
#	@ touch ~/.luof/raiz.luof
#	@ chmod u+rxw ~/.luof/raiz.luof

move:
	@ sudo mv luof /usr/local/bin/
	@ sudo chmod a+x /usr/local/bin/luof
