#include "luof.h"

void fHelp_private() {
	printf(
	"\t* -------------- Luof -------------- *\n\n"
	"\tEste projeto foi criado com a intenção de ser outro meio de guardar links de páginas web,"
	" assim como no Favoritos de um browser, pelo terminal linux de modo hierárquico, sendo"
	" fácil de adicionar e gerenciá-los.\n\n\n"
	
	"\t* -- Alguns detalhes importantes --- *\n\n"
	"\tEsse pequeno texto terá algumas informações em comum a todos ou a maioria dos comandos"
	" deste programa:\n"
	"\tPara usar o Luof, basta digitar na linha de comandos \"luof\" seguido do comando que se"
	" deseja utilizar, por extenso (com dois traços: -- ) ou abreviado (com um traço: - ), ou"
	" não informar nenhum comando para ver um menu das opções disponíveis.\n"
	"\tExs: luof -ab\n"
	"\t     luof --list-category\n"
	"\tUma categoria é por exemplo como uma pasta no favoritos de um browser, e da mesma forma"
	" que pode existir pastas dentro de pastas, pode existir categorias dentro de categorias."
	" Quando um favorito não está dentro de nenhuma categoria, ou uma categoria não possui"
	" categoria pai, dizemos que ambos estão na raiz, representada por apenas uma barra (/).\n"
	"\tAlguns comandos possuem a saída \"Categoria\" ou \"Categoria pai\" no terminal, nestes"
	" casos é necessário informar a categoria que determinado favorito ou subcategoria pertence."
	" Para separar o nome de uma categoria de sua categoria pai, basta usar uma barra (/) entre"
	" seus nomes. E caso se deseje gerenciar algo que está na raiz, deve-se entrar com apenas"
	" uma barra (/) e teclar enter.\n"
	"\tExs: Categoria     : Filmes/Terror\n"
	"\t     Categoria pai : /\n\n\n"

	"\t* ------------ Comandos ------------ *\n\n"
	"\t-h\tou   --help:\n"
	"\tUse para ver este menu, contendo uma breve descrição do programa Luof e os comandos que"
	" este possui, e uma breve descrição de como usá-los. Para uma descrição mais detalhada"
	" pode-se usar o comando --help (ou -h) seguido do comando que se deseja saber mais.\n\n"

	"\t-ab\tou   --add-bookmark:\n"
	"\tUse para guardar um favorito. Primeiro informe a categoria na qual se deseja adicionar"
	" o favorito, depois seu nome, depois o link da página, e, por fim, um comentário sobre.\n\n"

	"\t-ac\tou   --add-category:\n"
	"\tUse para adicionar uma categoria. Primeiro informe a categoria pai da categoria que"
	" será adicionada, e depois o nome dela.\n\n"

	"\t-rb\tou   --remove-bookmark:\n"
	"\tUse para remover um favorito. Primeiro informe a categoria a qual o favorito pertence,"
	" e depois o nome dele.\n\n"

	"\t-rc\tou   --remove-category:\n"
	"\tUse para remover uma categoria. Primeiro informe a categoria pai da categoria que"
	" será removida, e depois o nome dela.\n\n"

	"\t-mb\tou   --modify-bookmark:\n"
	"\tUse para modificar um favorito. Primeiramente informe a categoria a qual o favorito"
	" pertence, depois o seu nome, qual dado dele deseja-se modificar, e, por fim, basta"
	" informar o(s) novo(s) dado(s) do favorito.\n\n"

	"\t-mc\tou   --modify-category:\n"
	"\tUse para modificar uma categoria. Primeiramente informe a categoria pai da categoria"
	" que será modificada, depois seu nome, qual dado dela deseja-se modificar, e por fim"
	" basta informar o(s) novo(s) dado(s) da categoria.\n\n"

	"\t-lc\tou   --list-category:\n"
	"\tUse para ver o nome das categorias e dados dos favoritos que pertencem a categoria"
	" informada.\n\n"

	"\t-lcs\tou   --list-category-short:\n"
	"\tUse para ver apenas os nomes das categorias e favoritos que pertencem a categoria"
	" informada.\n\n"

	"\t-lt\tou   --list-tree:\n"
	"\tUse para ver todas as categorias e favoritos que estão armazenados. Os dados serão"
	" mostrados em forma de árvore, obedecendo à hierarquia das categorias.\n\n"

	"\t--backup:\n"
	"\tUse para criar ou restaurar um backup. Informe a opção desejada, se for [1]criar,"
	" o arquivo de backup (o arquivo possui a extensão .luof) será adicionado no diretório"
	" atual. Caso a opção seja [2]restaurar será necessário informar o caminho do arquivo de"
	" backup que se deseja restaurar.\n\n"
	);
}

void fHelp_h() {
	printf(
	"luof:\t-h\tou   --help:\n\n"
	"\tUse para ver um menu contendo uma breve descrição do programa Luof e os comandos que"
	" este possui, e uma breve descrição de como usá-los. Para uma descrição mais detalhada"
	" pode-se usar o comando --help (ou -h) seguido do comando que se deseja saber mais.\n\n"
	"Exs:\tluof -h\n"
	"\tluof --help\n"
	"\tluof -h -ab\n"
	"\tluof -h --add-bookmark\n"
	"\tluof --help -rc\n"
	"\tluof --help --remove-category\n\n"
	);
}

void fHelp_ab() {
	printf(
	"luof:\t-ab\tou   --add-bookmark:\n\n"
	"\tUse para guardar um favorito. Primeiro informe a categoria na qual se deseja adicionar"
	" o favorito, depois seu nome, depois o link da página, e, por fim, um comentário sobre.\n"
	"\tÉ necessário preencher todos os campos requeridos. Caso tecle enter sem nada informado,"
	" terá uma quebra de linha na saída do terminal, mas a entrada daquele dado ainda será"
	" esperada.\n"
	"\tNo geral, todos os campos aceitam um número grande de caracteres, com exceção do nome"
	" que aceita no máximo 99.\n\n"
	"Ex:\n"
	"Categoria : Jogos/steam\n"
	"Nome      : Dark Souls\n"
	"Link      : https://store.steampowered.com/app/570940/DARK_SOULS_REMASTERED/\n"
	"Texto     : Página da steam referente ao jogo Dark Souls: Remastered\n\n"
	);
}

void fHelp_ac() {
	printf(
	"luof:\t-ac\tou   --add-category:\n\n"
	"\tUse para adicionar uma categoria. Primeiro informe a categoria pai da categoria que será"
	" adicionada, e depois o nome dela.\n"
	"\tÉ necessário preencher todos os campos requeridos. Caso tecle enter sem nada informado,"
	" terá uma quebra de linha na saída do terminal, mas a entrada daquele dado ainda será"
	" esperada.\n"
	"\tO nome da categoria aceita no máximo 99 caracteres, não pode conter o caractere \"/\", e"
	" também não pode ser igual à palavra \"luof\".\n"
	"\tUma categoria pode no máximo alcançar 9 níveis de subcategoria (considerando que a raiz"
	" está na ).\n"
	"Ex:\n"
	"Categoria pai     : /\n"
	"Nome da categoria : Jogos\n\n"
	);
}

void fHelp_rb() {
	printf(
	"luof:\t-rb\tou   --remove-bookmark:\n\n"
	"\tUse para remover um favorito. Primeiro informe a categoria a qual o favorito pertence,"
	" depois informe o nome dele.\n"
	"\tÉ necessário preencher todos os campos requeridos. Caso tecle enter sem nada informado,"
	" terá uma quebra de linha na saída do terminal, mas a entrada daquele dado ainda será"
	" esperada.\n\n"
	"Ex:\n"
	"Categoria : Jogos/steam\n"
	"Nome      : Dark Souls\n\n"
	);
}

void fHelp_rc() {
	printf(
	"luof:\t-rc\tou   --remove-category:\n\n"
	"\tUse para remover uma categoria. Primeiro informe a categoria pai da categoria que"
	" será removida, e depois o nome dela.\n"
	"\tUma categoria removida também terá suas subcategorias e favoritos removidos.\n"
	"\tÉ necessário preencher todos os campos requeridos. Caso tecle enter sem nada informado,"
	" terá uma quebra de linha na saída do terminal, mas a entrada daquele dado ainda será"
	" esperada.\n\n"
	"Ex:\n"
	"Categoria pai     : Jogos\n"
	"Nome da categoria : steam\n\n"
	);
}

void fHelp_mb() {
	printf(
	"luof:\t-mb\tou   --modify-bookmark:\n\n"
	"\tUse para modificar um favorito. Primeiramente informe a categoria a qual o favorito"
	" pertence, depois o seu nome, qual dado dele deseja-se modificar (deve ser informado o"
	" número), e, por fim, basta informar o(s) novo(s) dado(s) do favorito.\n"
	"\tÉ necessário preencher todos os campos requeridos. Caso tecle enter sem nada informado,"
	" terá uma quebra de linha na saída do terminal, mas a entrada daquele dado ainda será"
	" esperada.\n\n"
	"Ex: Caso quisesse-se modificar o nome, o link e o texto do favorito \"Dark Souls\""
	" pertencente a categoria \"Jogos/steam\", poderia-se escolher a opção tudo (teclando 5"
	" quando requisitado a entrada), fazer as modificações necessárias e copiar os dados"
	" que não deseja-se modificar:\n\n"
	"Dados antigos:\n"
	"Categoria : Jogos/steam\n"
	"Nome      : Dark Souls\n"
	"Link      : https://store.steampowered.com/app/570940/DARK_SOULS_REMASTERED/\n"
	"Texto     : Página da steam referente ao jogo Dark Souls: Remastered\n\n"
	"Você deseja modificar? [1]categoria [2]nome [3]link [4]texto [5]tudo [6]nada : 5\n\n"
	"Novos dados:\n"
	"Categoria : Jogos/steam\n"
	"Nome      : Dark Souls II\n"
	"Link      : https://store.steampowered.com/app/335300/DARK_SOULS_II_Scholar_of_the_First_Sin/\n"
	"Texto     : Página da steam referente ao jogo Dark souls II: Scholar Of The First Sin\n\n"
	);
}

void fHelp_mc() {
	printf(
	"luof:\t-mc\tou   --modify-category:\n\n"
	"\tUse para modificar uma categoria. Primeiro informe a categoria pai da categoria que"
	" será modificada, depois o nome dela, qual dado seseja-se modificar (deve ser informado o"
	" número), e, por fim, basta informar o(s) novo(s) dado(s) da categoria.\n"
	"\tÉ necessário preencher todos os campos requeridos. Caso tecle enter sem nada informado,"
	" terá uma quebra de linha na saída do terminal, mas a entrada daquele dado ainda será"
	" esperada.\n\n"
	"Ex: Caso quisesse-se modificar a categoria pai da categoria \"Jogos/steam\" para a"
	" categoria raiz:\n\n"
	"Dados antigos:\n"
	"Categoria pai     : Jogos\n"
	"Nome da categoria : steam\n\n"
	"Você deseja modificar? [1]categoria pai [2]nome da categoria [3]tudo [4]nada : 1\n\n"
	"Novos dados:\n"
	"Categoria pai     : /\n\n"
	);
}

void fHelp_lc(int opcao) {
	if (opcao == 0) {
		printf(
		"luof:\t-lc\tou   --list-category:\n\n"
		"\tUse para ver o nome das categorias e dados dos favoritos que pertencem a categoria"
		" informada. As subcategorias serão mostradas com um apóstrofo (*) antes de seu"
		" nome, e os favoritos terão seu nome, link e texto mostrados.\n\n"
		);
	}
	else {
		printf(
		"luof:\t-lcs\tou   --list-category-short:\n\n"
		"\tUse para ver o nome das categorias e nome dos favoritos que pertencem a categoria"
		" informada. Onde as categorias serão mostradas com um apóstrofo (*) antes de seu"
		" nome, e os favoritos terão apenas seu nome mostrado.\n\n"
		);
	}
}

void fHelp_lt() {
	printf(
	"luof:\t-lt\tou   --list-tree:\n\n"
	"\tUse para ver todas as categorias e favoritos que estão armazenados. Os dados serão"
	" mostrados em forma de árvore, obedecendo à hierarquia das categorias, onde as categorias"
	" serão mostradas com um apóstrofo (*) antes de seu nome.\n"
	"\tAlém de mostrar uma visão de tudo que está armazenado, serve principalmente para saber"
	" o caminho (categoria e/ou nome) de algo específico.\n\n"
	"Ex: Caso o comando \"luof -lt\" gerasse a saída:\n"
	"|_ * Jogos\n"
	"|    |_ * steam\n"
	"|         |_ * Dark Souls\n"
	"|         |    |_ ds2 sotfs\n"
	"|         |    |_ dsr\n"
	"|_ ilovepdf\n"
	"|_ randoma11y\n\n"
	"\tPoderia-se concluir que o caminho do favorito \"dsr\" é \"Jogos/steam/Dark Souls/dsr\","
	" e caso quisesse-se removê-lo, os dados informados teriam que ser:\n"
	"Categoria : Jogos/steam/Dark Souls\n"
	"Nome      : dsr\n\n"
	"\tDa mesma forma, se se quisesse remover o favorito \"ilovepdf\" ou a categoria \"steam\","
	" os dados informados teriam que ser:\n"
	"\tPara o favorito \"ilovepdf\"\n"
	"Categoria : /\n"
	"Nome      : ilovepdf\n\n"
	"\tPara a categoria \"steam\"\n"
	"Categoria pai     : Jogos\n"
	"Nome da categoria : steam\n\n"
	);
}

void fHelp_backup() {
	printf(
	"luof:\t--backup:\n"
	"\tUse para criar ou restaurar um backup. Informe a opção desejada; se a opção for"
	" [1]criar, o arquivo de backup (o arquivo possui a extensão .luof) será adicionado"
	" no diretório atual, caso já exista um arquivo de backup no diretório ele não será"
	" perdido. Caso a opção seja [2]restaurar, será necessário informar o caminho do arquivo"
	" de backup que se deseja restaurar, lembrando que ao restaurar um backup todos os"
	" dados atuais do banco serão perdidos.\n\n"
	"Ex: Caso quisesse-se criar um backup, porém na pasta atual já exista o arquivo backup.luof:\n\n"
	"Você deseja criar um backup novo ou restaurar um antigo? [1]criar [2]restaurar [3]sair : 1\n"
	"Arquivo backup1.luof adicionado no diretório atual.\n"
	"Backup criado com sucesso.\n\n"
	"Ex: Caso quisesse-se restaurar um backup com o caminho \"/home/usuario/Downloads/backup5.luof\":\n\n"
	"Você deseja criar um backup novo ou restaurar um antigo? [1]criar [2]restaurar [3]sair : 2\n"
	"Informe o caminho do arquivo de backup: /home/usuario/Downloads/backup5.luof\n"
	"Backup restaurado com sucesso.\n\n"
	);
}

void fHelp(char *argv) {

	if (!argv) {
		fHelp_private();
	}
	else if (strcmp(argv, "-h") == 0 || strcmp(argv, "--help") == 0) {
		fHelp_h();
	}
	else if (strcmp(argv, "-ab") == 0 || strcmp(argv, "--add-bookmark") == 0) {
		fHelp_ab();
	}
	else if (strcmp(argv, "-ac") == 0 || strcmp(argv, "--add-category") == 0) {
		fHelp_ac();
	}
	else if (strcmp(argv, "-rb") == 0 || strcmp(argv, "--remove-bookmark") == 0) {
		fHelp_rb();
	}
	else if (strcmp(argv, "-rc") == 0 || strcmp(argv, "--remove-category") == 0) {
		fHelp_rc();
	}
	else if (strcmp(argv, "-mb") == 0 || strcmp(argv, "--modify-bookmark") == 0) {
		fHelp_mb();
	}
	else if (strcmp(argv, "-mc") == 0 || strcmp(argv, "--modify-category") == 0) {
		fHelp_mc();
	}
	else if (strcmp(argv, "-lc") == 0 || strcmp(argv, "--list-category") == 0) {
		fHelp_lc(0);
	}
	else if (strcmp(argv, "-lcs") == 0 || strcmp(argv, "--list-category-short") == 0) {
		fHelp_lc(1);
	}
	else if (strcmp(argv, "-lt") == 0 || strcmp(argv, "--list-tree") == 0) {
		fHelp_lt();
	}
	else if (strcmp(argv, "--backup") == 0) {
		fHelp_backup();
	}
	else {
		printf("%s não é um comando de luof. Use \"luof --help\" para ver comandos válidos.\n", argv);
	}
	/*else if (strcmp(argv, "-jc") == 0 || strcmp(argv, "--join-categorys") == 0) {
		//fHelp_jc();
		printf("Função ainda não implementada\n");
	}
	else if (strcmp(argv, "--import") == 0) {
		//fHelp_import();
		printf("Função ainda não implementada\n");
	}
	else if (strcmp(argv, "--export") == 0) {
		//fHelp_export();
		printf("Função ainda não implementada\n");
	}*/

}
