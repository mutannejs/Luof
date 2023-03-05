# Luof

## Conceito do projeto

Este projeto foi criado com a intenção de ser outro meio de guardar links de páginas web,
 assim como no Favoritos de um browser, pelo terminal linux de modo hierárquico, sendo
 fácil de adicionar e gerenciá-los. Possibilitando armazenar links de páginas visitadas
 em diferentes navegadores em um lugar só, independente da quantidade, deixando apenas os
 mais importantes e recorrentes no Favoritos do browser usado.

## Pré-requisitos e recursos utilizados

Em geral foi utilizado a Linguagem C para desenvolver este projeto, possuindo também um
 arquivo makefile para ajudar na compilação do programa. Para compilá-lo usando o makefile
 é necessário ter o programa make e o compilador gcc instalados no sistema, caso contrário, 
 qualquer compilador C deve funcionar.

## Passo a passo

1. Definição de quais seriam as estruturas usadas no código, e os padrões de nomenclaturas.
2. Definido como o banco de dados seria organizado, e criado a função responsável por
 iniciá-lo. Foi definido então que o banco de dados seria implementado usando arquivos e
 as funções da biblioteca padrão C para manipulá-los.
3. Implementado funções para adicionar favoritos e categorias, e depois para remover
 favoritos e categorias.
4. Implementado funções para listar favoritos de uma categoria, e depois para listar todas
categorias e favoritos em árvore.
5. Implementado funções para modificar favoritos e categorias.
6. Implementado funções de criar e restaurar backup e de exportar favoritos.
7. Implementado a função help e feito ajustes gerais no código.

## Instalação

### Usando make
Para instalar o programa usando o makefile, basta entrar na pasta do projeto e rodar o comando:
```
make install
```
Ao utilizar o comando acima será requisitado a senha de seu usuário. Este comando compila o
 programa e move o arquivo binário para a basta _/usr/bin_, para que possa executa-lo em
 qualquer diretório dentro do sistema sem a necessidade de informar seu caminho completo.
 Caso queira apenas criar um arquivo binário no diretório atual, basta rodar o comando:
```
make compile
```

### Usando gcc
Para compilar o programa usando o gcc, basta entrar na pasta do projeto e rodar o comando:
```
gcc -o luof main.c add-remove.c list.c modify.c dbluof.c dbcat.c modulos.c help.c backup.c import-export.c lista-iterador.c teste.c
```
Para poder executa-lo em qualquer diretório dentro do sistema sem a necessidade de informar
 seu caminho completo, após rodar o comando acima será necessário usar o comando:
```
sudo mv luof /usr/bin
```

## Execução

## Bugs/problemas conhecidos

Este projeto foi feito pensando no sistema de diretórios do linux, portanto não deve
funcionar em outros sistemas operacionais.

## Autores

Murillo Justino dos Santos

## Imagens/screenshots
