# Luof

## Conceito do projeto

Este projeto foi criado com a intenção de ser outro meio de guardar links de páginas web,
 assim como no Favoritos de um browser, pelo terminal linux de modo hierárquico, sendo
 fácil de adicionar e gerenciá-los. Possibilitando armazenar links de páginas visitadas
 em diferentes navegadores em um lugar só, independente da quantidade, deixando apenas os
 mais importantes e recorrentes no Favoritos do browser usado.

## Pré-requisitos e recursos utilizados

Em geral foi utilizado a Linguagem C para desenvolver este projeto, possuindo também um
 arquivo makefile para ajudar na compilação do programa. Para compila-lo usando o makefile
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
7. Implementado a função help, a função import e feito ajustes gerais no código.

## Instalação

Primeiramente deve-se clonar o repositório do luof e entrar em seu diretório, isso pode ser feito
 com os comandos a seguir:
```
git clone https://github.com/mutannejs/Luof.git
cd Luof
```

### Usando make
Para instalar o programa usando o makefile, dentro da pasta do projeto, basta rodar o comando:
```
make install
```
Ao utilizar o comando acima será requisitado a senha de seu usuário. Este comando compila o
 programa e move o arquivo binário para a basta _/usr/bin_, para que possa executá-lo em
 qualquer diretório dentro do sistema sem a necessidade de informar seu caminho completo,
 além de criar o banco de dados na _home_ do usuário. Caso não queira usar o luof em outros
 diretórios, deve-se rodar o comando `make compile` em conjunto com o esquema descrito nas
 linhas 14 a 19 no arquivo _dbluof.c_ (se ele não for executado, ocorrerá erro na execução
 do programa, o esquema deve ser feito antes de usar o comando make). Desse modo não será
 criado nenhum arquivo em outros diretórios, nem será requisitado a senha do usuário.

### Sem usar make
Para compilar o programa sem o makefile, só com o gcc, basta entrar na pasta do projeto
 e rodar os comandos:
```
gcc -o luof main.c add-remove.c list.c modify.c dbluof.c dbcat.c modulos.c help.c backup.c import-export.c lista-iterador.c teste.c
```
Para poder executá-lo em qualquer diretório dentro do sistema sem a necessidade de informar
 seu caminho completo, após rodar o comando acima será necessário usar o comando:
```
sudo mv luof /usr/bin
```
Caso não queira usar o luof em outros diretórios, antes de compilar o programa deve ser feito
 o esquema descrito nas linhas 14 a 19 no arquivo dbluof.c (se ele não for executado,
 ocorrerá erro na execução do programa). Desse modo não será criado nenhum arquivo em outros
 diretórios, nem será requisitado a senha do usuário.
 
### Se não foi usado make install
Na primeira execução do programa será necessário criar um novo banco de dados,  basta teclar
 's' seguido de enter para responder que o novo banco deve ser criado. A seguir há uma imagem
 que descreve este cenário, usando o modo de instalação sem o make, será usado as funções
 `--import` e `-lt` apenas para melhorar o exemplo:
![imagem](https://github.com/mutannejs/Luof/blob/master/imagens/img07.png)

## Desinstalando

Caso tenha sido usado o comando `make install` para instalar o luof, para desinstalá-lo basta usar o comando:
```
make desinstall
```
Este comando remove o arquivo binário da pasta _/usr/bin_ e exclui o banco de dados da _home_ do usuário
 que executou o comando.

## Execução

Neste tópico será mostrado algumas das principais funções que o programa nos permite utilizar. Primeiramente
 será apresentado uma imagem contendo um texto explicando o básico para utilizar o programa, ele poderá ser
 visto utilizando o comando `luof --help` após o luof ter sido instalado em seu sistema:
![imagem](https://github.com/mutannejs/Luof/blob/master/imagens/img00.png)
Além desse texto explicativo, a função `help` mostra todas as opções possíveis de usar no luof, incluindo
 funções não apresentadas neste tópico.

Após instalado o programa, será adicionado um primeiro favorito nele. O favorito será o repositório deste
 projeto, e será inserido na raiz (até então, não há nenhuma categoria inserida, logo, todos favoritos só
 poderão ser inseridos na raiz). Para fazer isso, é necessário usar o comando `--add-bookmark` ou sua
 abreviação `-ab`, e informar o nome, link, categoria e um texto sobre o favorito quando requisitados:
![imagem](https://github.com/mutannejs/Luof/blob/master/imagens/img01.png)

Agora será inserido a primeira categoria (na raiz), chamada **linux**. Logo depois, dentro da categoria criada
 será inserida outra categoria, chamada **debian**. Para fazer isso, é necessário usar o comando
 `--add-category` ou sua abreviação `-ac`, e informar a categoria a qual ela pertence e seu nome:
![imagem](https://github.com/mutannejs/Luof/blob/master/imagens/img02.png)

A próxima imagem mostra como adicionar favoritos às categorias existentes no programa, será inserido então
 o favorito **news** na categoria **linux/debian**, e o favorito **foca linux** na categoria **linux**.
 Dessa vez será usado o comando abreviado `-ab`, que possui o mesmo efeito do comando `--add-bookmark`
 usado anteriormente:
![imagem](https://github.com/mutannejs/Luof/blob/master/imagens/img03.png)

Os comando anteriores não seriam úteis se não houvesse uma maneira de rever as informações guardadas. Para
 isso existe duas opções. A função `--list-tree` ou sua versão abreviada `-lt`, que mostra tudo que
 está armazenado em forma de árvore, obedecendo a hierarquia das categorias. E a função `--list-category`
 ou sua versão abreviada `-lc`, que mostra os favoritos e subcategorias de uma categoria específica, a
 qual deve ser informada quando requisitado, neste exemplo será visualizado a categoria **linux**:
![imagem](https://github.com/mutannejs/Luof/blob/master/imagens/img04.png)

Neste passo, o favorito **luof** adicionado no início desse tópico será removido, para isso é usado a função
 `--remove-bookmark` ( poderia ser usado sua abreviação `-rb`) e informado o nome e categoria do favorito
 quando requisitado. São usados as duas funções descritas no passo anterior para mostrar o resultado da
 remoção desse favorito, ambas agora na versão abreviada:
![imagem](https://github.com/mutannejs/Luof/blob/master/imagens/img05.png)

Por fim, vemos para que serve e como usar a função `--help`, fazemos isso usando a própria função `--help` e
 passando a mesma como argumento, mas nesse exemplo no argumento ela está em sua forma abreviada `-h`:
![imagem](https://github.com/mutannejs/Luof/blob/master/imagens/img06.png)

## Bugs/problemas conhecidos

Este projeto foi feito pensando no sistema de diretórios do linux, portanto, não deve
funcionar em outros sistemas operacionais.

## Autores

Murillo Justino dos Santos
