#include "luof.h"

void fHelp_private() {
	printf(
	ANSI_BOLD_WHT
	"\t* -------------- Luof -------------- *\n\n"
	ANSI_COLOR_WHT
	"\tEste projeto foi criado com a intenção de ser outro meio de guardar links de páginas web,"
	" assim como no Favoritos de um browser, pelo terminal linux de modo hierárquico, sendo"
	" fácil de adicionar e gerenciá-los.\n\n\n"
	
	ANSI_BOLD_WHT
	"\t* -- Alguns detalhes importantes --- *\n\n"
	ANSI_COLOR_WHT
	"\tEsse pequeno texto terá algumas informações em comum a todos ou a maioria dos comandos"
	" deste programa:\n"
	"\tPara usar o Luof, basta digitar na linha de comandos \"luof\" seguido do comando que se"
	" deseja utilizar, por extenso (com dois traços: -- ) ou abreviado (com um traço: - ), ou"
	" não informar nenhum comando para ver um menu das opções disponíveis.\n"
	ANSI_BOLD_WHT "\tExs: " ANSI_COLOR_BLU "luof -ab\n"
	"\t     luof --list-category\n"
	ANSI_COLOR_WHT
	"\tUma categoria é por exemplo como uma pasta no favoritos de um browser, e da mesma forma"
	" que pode existir pastas dentro de pastas, pode existir categorias dentro de categorias."
	" Quando um favorito não está dentro de nenhuma categoria, ou uma categoria não possui"
	" categoria pai, dizemos que ambos estão na raiz, representada por apenas uma barra (/).\n"
	"\tAlguns comandos possuem a saída \"Categoria\" ou \"Categoria pai\" no terminal, nestes"
	" casos é necessário informar a categoria que determinado favorito ou subcategoria pertence."
	" Para separar o nome de uma categoria de sua categoria pai, basta usar uma barra (/) entre"
	" seus nomes. E caso se deseje gerenciar algo que está na raiz, deve-se entrar com apenas"
	" uma barra (/) e teclar enter.\n"
	ANSI_BOLD_WHT "\tExs: " ANSI_BOLD_CYA "Categoria     : " ANSI_COLOR_BLU "Filmes/Terror\n"
							ANSI_BOLD_CYA "\t     Categoria pai : " ANSI_COLOR_BLU "/\n\n\n"

	ANSI_BOLD_WHT
	"\t* ------------ Comandos ------------ *\n\n"
	ANSI_BOLD_YEL "\t-h\tou   --help:\n"
	ANSI_COLOR_WHT
	"\tUse para ver este menu, contendo uma breve descrição do programa Luof e os comandos que"
	" este possui, e uma breve descrição de como usá-los. Para uma descrição mais detalhada"
	" pode-se usar o comando --help (ou -h) seguido do comando que se deseja saber mais.\n"
	ANSI_BOLD_WHT "\tEx: " ANSI_COLOR_BLU "luof -h -ab\n\n"

	ANSI_BOLD_YEL "\t-ab\tou   --add-bookmark:\n"
	ANSI_COLOR_WHT
	"\tUse para guardar um favorito. Primeiro informe a categoria na qual se deseja adicionar"
	" o favorito, depois seu nome, depois o link da página, e, por fim, um comentário sobre.\n\n"

	ANSI_BOLD_YEL "\t-ac\tou   --add-category:\n"
	ANSI_COLOR_WHT
	"\tUse para adicionar uma categoria. Primeiro informe a categoria pai da categoria que"
	" será adicionada, e depois o nome dela.\n\n"

	ANSI_BOLD_YEL "\t-rb\tou   --remove-bookmark:\n"
	ANSI_COLOR_WHT
	"\tUse para remover um favorito. Primeiro informe a categoria a qual o favorito pertence,"
	" e depois o nome dele.\n\n"

	ANSI_BOLD_YEL "\t-rc\tou   --remove-category:\n"
	ANSI_COLOR_WHT
	"\tUse para remover uma categoria. Primeiro informe a categoria pai da categoria que"
	" será removida, e depois o nome dela.\n\n"

	ANSI_BOLD_YEL "\t-mb\tou   --modify-bookmark:\n"
	ANSI_COLOR_WHT
	"\tUse para modificar um favorito. Primeiramente informe a categoria a qual o favorito"
	" pertence, depois o seu nome, qual dado dele deseja-se modificar, e, por fim, basta"
	" informar o(s) novo(s) dado(s) do favorito.\n\n"

	ANSI_BOLD_YEL "\t-mc\tou   --modify-category:\n"
	ANSI_COLOR_WHT
	"\tUse para modificar uma categoria. Primeiramente informe a categoria pai da categoria"
	" que será modificada, depois seu nome, qual dado dela deseja-se modificar, e por fim"
	" basta informar o(s) novo(s) dado(s) da categoria.\n\n"

	ANSI_BOLD_YEL "\t-lc\tou   --list-category:\n"
	ANSI_COLOR_WHT
	"\tUse para ver o nome das categorias e dados dos favoritos que pertencem a categoria"
	" informada.\n\n"

	ANSI_BOLD_YEL "\t-lcs\tou   --list-category-short:\n"
	ANSI_COLOR_WHT
	"\tUse para ver apenas os nomes das categorias e favoritos que pertencem a categoria"
	" informada.\n\n"

	ANSI_BOLD_YEL "\t-lt\tou   --list-tree:\n"
	ANSI_COLOR_WHT
	"\tUse para ver todas as categorias e favoritos que estão armazenados. Os dados serão"
	" mostrados em forma de árvore, obedecendo à hierarquia das categorias.\n\n"

	ANSI_BOLD_YEL "\t--backup:\n"
	ANSI_COLOR_WHT
	"\tUse para criar ou restaurar um backup. Informe a opção desejada, se for [1]criar,"
	" o arquivo de backup (o arquivo possui a extensão .luof) será adicionado no diretório"
	" atual. Caso a opção seja [2]restaurar será necessário informar o caminho do arquivo de"
	" backup que se deseja restaurar.\n\n"

	ANSI_BOLD_YEL "\t--export:\n"
	ANSI_COLOR_WHT
	"\tUse para exportar uma categoria do luof para ser importada em um browser.\n\n"
	);
}

void fHelp_h() {
	printf(
	ANSI_BOLD_WHT "luof:" ANSI_BOLD_YEL "\t-h\tou   --help:\n\n"
	ANSI_COLOR_WHT
	"\tUse para ver um menu contendo uma breve descrição do programa Luof e os comandos que"
	" este possui, e uma breve descrição de como usá-los. Para uma descrição mais detalhada"
	" pode-se usar o comando --help (ou -h) seguido do comando que se deseja saber mais.\n\n"
	ANSI_BOLD_WHT "Exs:" ANSI_COLOR_BLU "\tluof -h\n"
	"\tluof --help\n"
	"\tluof -h -ab\n"
	"\tluof -h --add-bookmark\n"
	"\tluof --help -rc\n"
	"\tluof --help --remove-category\n\n"
	);
}

void fHelp_ab() {
	printf(
	ANSI_BOLD_WHT "luof:" ANSI_BOLD_YEL "\t-ab\tou   --add-bookmark:\n\n"
	ANSI_COLOR_WHT
	"\tUse para guardar um favorito. Primeiro informe a categoria na qual se deseja adicionar"
	" o favorito, depois seu nome, depois o link da página, e, por fim, um comentário sobre.\n"
	"\tÉ necessário preencher todos os campos requeridos. Caso tecle enter sem nada informado,"
	" terá uma quebra de linha na saída do terminal, mas a entrada daquele dado ainda será"
	" esperada.\n"
	"\tNo geral, todos os campos aceitam um número grande de caracteres, com exceção do nome"
	" que aceita no máximo 99.\n\n"
	ANSI_BOLD_WHT "Ex:\n"
	ANSI_BOLD_CYA "Categoria : " ANSI_COLOR_BLU "Jogos/steam\n"
	ANSI_BOLD_CYA "Nome      : " ANSI_COLOR_BLU "Dark Souls\n"
	ANSI_BOLD_CYA "Link      : " ANSI_COLOR_BLU "https://store.steampowered.com/app/570940/DARK_SOULS_REMASTERED/\n"
	ANSI_BOLD_CYA "Texto     : " ANSI_COLOR_BLU "Página da steam referente ao jogo Dark Souls: Remastered\n\n"
	);
}

void fHelp_ac() {
	printf(
	ANSI_BOLD_WHT "luof: " ANSI_BOLD_YEL "\t-ac\tou   --add-category:\n\n"
	ANSI_COLOR_WHT
	"\tUse para adicionar uma categoria. Primeiro informe a categoria pai da categoria que será"
	" adicionada, e depois o nome dela.\n"
	"\tÉ necessário preencher todos os campos requeridos. Caso tecle enter sem nada informado,"
	" terá uma quebra de linha na saída do terminal, mas a entrada daquele dado ainda será"
	" esperada.\n"
	"\tO nome da categoria aceita no máximo 99 caracteres, não pode conter o caractere \"/\", e"
	" também não pode ser igual à palavra \"luof\".\n"
	"\tUma categoria pode no máximo alcançar 9 níveis de subcategoria (considerando que a raiz"
	" está na ).\n\n"
	ANSI_BOLD_WHT "Ex:\n"
	ANSI_BOLD_CYA "Categoria pai     : " ANSI_COLOR_BLU "/\n"
	ANSI_BOLD_CYA "Nome da categoria : " ANSI_COLOR_BLU "Jogos\n\n"
	);
}

void fHelp_rb() {
	printf(
	ANSI_BOLD_WHT "luof:" ANSI_BOLD_YEL "\t-rb\tou   --remove-bookmark:\n\n"
	ANSI_COLOR_WHT
	"\tUse para remover um favorito. Primeiro informe a categoria a qual o favorito pertence,"
	" depois informe o nome dele.\n"
	"\tÉ necessário preencher todos os campos requeridos. Caso tecle enter sem nada informado,"
	" terá uma quebra de linha na saída do terminal, mas a entrada daquele dado ainda será"
	" esperada.\n\n"
	ANSI_BOLD_WHT "Ex:\n"
	ANSI_BOLD_CYA "Categoria : " ANSI_COLOR_BLU "Jogos/steam\n"
	ANSI_BOLD_CYA "Nome      : " ANSI_COLOR_BLU "Dark Souls\n\n"
	);
}

void fHelp_rc() {
	printf(
	ANSI_BOLD_WHT "luof:" ANSI_BOLD_YEL "\t-rc\tou   --remove-category:\n\n"
	ANSI_COLOR_WHT
	"\tUse para remover uma categoria. Primeiro informe a categoria pai da categoria que"
	" será removida, e depois o nome dela.\n"
	"\tUma categoria removida também terá suas subcategorias e favoritos removidos.\n"
	"\tÉ necessário preencher todos os campos requeridos. Caso tecle enter sem nada informado,"
	" terá uma quebra de linha na saída do terminal, mas a entrada daquele dado ainda será"
	" esperada.\n\n"
	ANSI_BOLD_WHT "Ex:\n"
	ANSI_BOLD_CYA "Categoria pai     : " ANSI_COLOR_BLU "Jogos\n"
	ANSI_BOLD_CYA "Nome da categoria : " ANSI_COLOR_BLU "steam\n\n"
	);
}

void fHelp_mb() {
	printf(
	ANSI_BOLD_WHT "luof:" ANSI_BOLD_YEL "\t-mb\tou   --modify-bookmark:\n\n"
	ANSI_COLOR_WHT
	"\tUse para modificar um favorito. Primeiramente informe a categoria a qual o favorito"
	" pertence, depois o seu nome, qual dado dele deseja-se modificar (deve ser informado o"
	" número), e, por fim, basta informar o(s) novo(s) dado(s) do favorito.\n"
	"\tÉ necessário preencher todos os campos requeridos. Caso tecle enter sem nada informado,"
	" terá uma quebra de linha na saída do terminal, mas a entrada daquele dado ainda será"
	" esperada.\n\n"
	ANSI_BOLD_WHT "Ex:"
	ANSI_COLOR_WHT " Caso quisesse-se modificar o nome, o link e o texto do favorito \"Dark Souls\""
	" pertencente a categoria \"Jogos/steam\", poderia-se escolher a opção tudo (teclando 5"
	" quando requisitado a entrada), fazer as modificações necessárias e copiar os dados"
	" que não deseja-se modificar:\n\n"
	ANSI_BOLD_WHT "Ex:\n"
	ANSI_BOLD_CYA "Categoria : " ANSI_COLOR_BLU "Jogos/steam\n"
	ANSI_BOLD_CYA "Nome      : " ANSI_COLOR_BLU "Dark Souls\n\n"
	ANSI_BOLD_CYA "Dados antigos:\n"
	"Categoria : Jogos/steam\n"
	"Nome      : Dark Souls\n"
	"Link      : https://store.steampowered.com/app/570940/DARK_SOULS_REMASTERED/\n"
	"Texto     : Página da steam referente ao jogo Dark Souls: Remastered\n\n"
	"Você deseja modificar? [1]categoria [2]nome [3]link [4]texto [5]tudo [6]nada : " ANSI_COLOR_BLU "5\n\n"
	ANSI_BOLD_CYA "Novos dados:\n"
	ANSI_BOLD_CYA "Categoria : " ANSI_COLOR_BLU "Jogos/steam\n"
	ANSI_BOLD_CYA "Nome      : " ANSI_COLOR_BLU "Dark Souls II\n"
	ANSI_BOLD_CYA "Link      : " ANSI_COLOR_BLU "https://store.steampowered.com/app/335300/DARK_SOULS_II_Scholar_of_the_First_Sin/\n"
	ANSI_BOLD_CYA "Texto     : " ANSI_COLOR_BLU "Página da steam referente ao jogo Dark souls II: Scholar Of The First Sin\n\n"
	);
}

void fHelp_mc() {
	printf(
	ANSI_BOLD_WHT "luof:" ANSI_BOLD_YEL "\t-mc\tou   --modify-category:\n\n"
	ANSI_COLOR_WHT
	"\tUse para modificar uma categoria. Primeiro informe a categoria pai da categoria que"
	" será modificada, depois o nome dela, qual dado seseja-se modificar (deve ser informado o"
	" número), e, por fim, basta informar o(s) novo(s) dado(s) da categoria.\n"
	"\tÉ necessário preencher todos os campos requeridos. Caso tecle enter sem nada informado,"
	" terá uma quebra de linha na saída do terminal, mas a entrada daquele dado ainda será"
	" esperada.\n\n"
	ANSI_BOLD_WHT "Ex:"
	ANSI_COLOR_WHT " Caso quisesse-se modificar a categoria pai da categoria \"Jogos/steam\" para a"
	" categoria raiz:\n\n"
	ANSI_BOLD_CYA "Categoria pai     : " ANSI_COLOR_BLU "Jogos\n"
	ANSI_BOLD_CYA "Nome da categoria : " ANSI_COLOR_BLU "steam\n\n"
	ANSI_BOLD_CYA "Você deseja modificar? [1]categoria pai [2]nome da categoria [3]tudo [4]nada : " ANSI_COLOR_BLU "1\n\n"
	ANSI_BOLD_CYA "Novos dados:\n"
	ANSI_BOLD_CYA "Categoria pai     : " ANSI_COLOR_BLU "/\n\n"
	);
}

void fHelp_lc(int opcao) {
	if (opcao == 0) {
		printf(
		ANSI_BOLD_WHT "luof:" ANSI_BOLD_YEL "\t-lc\tou   --list-category:\n\n"
		ANSI_COLOR_WHT
		"\tUse para ver o nome das categorias e dados dos favoritos que pertencem a categoria"
		" informada. As subcategorias serão mostradas com um apóstrofo (*) antes de seu"
		" nome, e os favoritos terão seu nome, link e texto mostrados.\n\n"
		);
	}
	else {
		printf(
		ANSI_BOLD_WHT "luof:" ANSI_BOLD_YEL "\t-lcs\tou   --list-category-short:\n\n"
		ANSI_COLOR_WHT
		"\tUse para ver o nome das categorias e nome dos favoritos que pertencem a categoria"
		" informada. Onde as categorias serão mostradas com um apóstrofo (*) antes de seu"
		" nome, e os favoritos terão apenas seu nome mostrado.\n\n"
		);
	}
}

void fHelp_lt() {
	printf(
	ANSI_BOLD_WHT "luof:" ANSI_BOLD_YEL "\t-lt\tou   --list-tree:\n\n"
	ANSI_COLOR_WHT
	"\tUse para ver todas as categorias e favoritos que estão armazenados. Os dados serão"
	" mostrados em forma de árvore, obedecendo à hierarquia das categorias, onde as categorias"
	" serão mostradas com um apóstrofo (*) antes de seu nome.\n"
	"\tAlém de mostrar uma visão de tudo que está armazenado, serve principalmente para saber"
	" o caminho (categoria e/ou nome) de algo específico.\n\n"
	ANSI_BOLD_WHT "Ex:"
	ANSI_COLOR_WHT " Caso o comando \"luof -lt\" gerasse a saída:\n"
	ANSI_BOLD_CYA "|_ * Jogos\n"
	"|    |_ * steam\n"
	"|         |_ * Dark Souls\n"
	"|         |    |_ ds2 sotfs\n"
	"|         |    |_ dsr\n"
	"|_ ilovepdf\n"
	"|_ randoma11y\n\n"
	ANSI_COLOR_WHT "\tPoderia-se concluir que o caminho do favorito \"dsr\" é \"Jogos/steam/Dark Souls/dsr\","
	" e caso quisesse-se removê-lo, os dados informados teriam que ser:\n"
	ANSI_BOLD_CYA "Categoria : " ANSI_COLOR_BLU "Jogos/steam/Dark Souls\n"
	ANSI_BOLD_CYA "Nome      : " ANSI_COLOR_BLU "dsr\n\n"
	ANSI_COLOR_WHT "\tDa mesma forma, se se quisesse remover o favorito \"ilovepdf\" ou a categoria \"steam\","
	" os dados informados teriam que ser:\n"
	"\tPara o favorito \"ilovepdf\"\n"
	ANSI_BOLD_CYA "Categoria : " ANSI_COLOR_BLU "/\n"
	ANSI_BOLD_CYA "Nome      : " ANSI_COLOR_BLU "ilovepdf\n\n"
	ANSI_COLOR_WHT "\tPara a categoria \"steam\"\n"
	ANSI_BOLD_CYA "Categoria pai     : " ANSI_COLOR_BLU "Jogos\n"
	ANSI_BOLD_CYA "Nome da categoria : " ANSI_COLOR_BLU "steam\n\n"
	);
}

void fHelp_backup() {
	printf(
	ANSI_BOLD_WHT "luof:" ANSI_BOLD_YEL "\t--backup:\n\n"
	ANSI_COLOR_WHT
	"\tUse para criar ou restaurar um backup. Informe a opção desejada; se a opção for"
	" [1]criar, o arquivo de backup (o arquivo possui a extensão .luof) será adicionado"
	" no diretório atual, caso já exista um arquivo de backup no diretório ele não será"
	" perdido. Caso a opção seja [2]restaurar, será necessário informar o caminho do arquivo"
	" de backup que se deseja restaurar, lembrando que ao restaurar um backup todos os"
	" dados atuais do banco serão perdidos.\n\n"
	ANSI_BOLD_WHT "Ex:"
	ANSI_COLOR_WHT " Caso quisesse-se criar um backup, porém na pasta atual já exista o arquivo backup.luof:\n"
	ANSI_BOLD_CYA "Você deseja criar um backup novo ou restaurar um antigo? [1]criar [2]restaurar [3]sair : " ANSI_COLOR_BLU "1\n"
	ANSI_BOLD_CYA "Arquivo backup-1.luof adicionado no diretório atual.\n\n"
	"Backup criado com sucesso.\n\n"
	ANSI_BOLD_WHT "Ex:"
	ANSI_COLOR_WHT " Caso quisesse-se restaurar um backup com o caminho \"/home/usuario/Downloads/backup5.luof\":\n"
	ANSI_BOLD_CYA "Você deseja criar um backup novo ou restaurar um antigo? [1]criar [2]restaurar [3]sair : " ANSI_COLOR_BLU "2\n"
	ANSI_BOLD_CYA "Informe o caminho do arquivo de backup: " ANSI_COLOR_BLU "/home/usuario/Downloads/backup5.luof\n\n"
	ANSI_BOLD_CYA "Backup restaurado com sucesso.\n\n"
	);
}

void fHelp_export() {
	printf(
	ANSI_BOLD_WHT "luof:" ANSI_BOLD_YEL "\t--export:\n\n"
	ANSI_COLOR_WHT
	"\tUse para exportar uma categoria do luof para ser importada em um browser."
	" É necessário apenas informar o caminho da categoria desejada. o arquivo de"
	" export possui a extensão .html e nome \"bookmarks-luof\", podendo ter um número"
	" após caso já exista um arquivo de export (este não será perdido) no diretório atual,"
	" o mesmo no qual será armazenado o novo arquivo de export.\n\n"
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
	else if (strcmp(argv, "--export") == 0) {
		fHelp_export();
	}
	else {
		printf(ERRO2);
		printf("%s não é um comando de luof. Use \"luof --help\" para ver comandos válidos.\n", argv);
	}

}
