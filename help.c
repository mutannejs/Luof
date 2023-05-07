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
	" não informar nenhum comando para ver um menu das opções disponíveis. A maioria dos"
	" comandos permite receber um ou mais argumentos após seu nome na linha de comando, para"
	" saber quais argumentos podem ser passados para aquele comando deve-se verificar seu modo"
	" de uso com o comando --help.\n"
	ANSI_BOLD_WHT "\tExs: " ANSI_COLOR_BLU "luof -ab\n"
	"\t     luof --list-tree\n"
	"\t     luof -lc -s Filmes\n"
	ANSI_COLOR_WHT
	"\tUsar os caracteres < e > em nomes de favoritos ou em nomes de categorias pode gerar um"
	" mal funcionamento de algumas operações, por tanto, não é recomendado usar esses dois"
	" caracteres nos casos mencionados.\n"
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
	" o favorito, depois seu nome, depois o link da página, e, por fim, uma descrição sobre.\n\n"

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
	" basta informar o(s) novo(s) dado(s) da categoria.\n\n");

	printf(
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

	ANSI_BOLD_YEL "\t-lts\tou   --list-tree-short:\n"
	ANSI_COLOR_WHT
	"\tUse para ver todas as categorias que estão armazenadas. Os dados serão"
	" mostrados em forma de árvore, obedecendo à hierarquia das categorias.\n\n"

	ANSI_BOLD_YEL "\t--backup:\n"
	ANSI_COLOR_WHT
	"\tUse para criar ou restaurar um backup. Informe a opção desejada, se for [1]criar,"
	" o arquivo de backup (o arquivo possui o nome luof.bkp) será adicionado no diretório"
	" atual. Caso a opção seja [2]restaurar será necessário informar o caminho do arquivo de"
	" backup que se deseja restaurar.\n\n"

	ANSI_BOLD_YEL "\t--export:\n"
	ANSI_COLOR_WHT
	"\tUse para exportar uma categoria do luof para ser importada em um browser.\n\n"

	ANSI_BOLD_YEL "\t--import:\n"
	ANSI_COLOR_WHT
	"\tUse para importar favoritos exportados de outro browser.\n\n"

	ANSI_BOLD_YEL "\t--free:\n"
	ANSI_COLOR_WHT
	"\tUse para apagar todas categorias e favoritos do banco.\n\n"
	);
}

void fHelp_h() {
	printf(
	ANSI_BOLD_WHT "luof:" ANSI_BOLD_YEL "\t-h\tou   --help:\n\n"

	ANSI_COLOR_WHT
	"\tUse para ver um menu contendo uma breve descrição do programa Luof e os comandos que"
	" este possui, e uma breve descrição de como usá-los. Para uma descrição mais detalhada"
	" pode-se usar o comando --help (ou -h) seguido do comando que se deseja saber mais.\n\n"

	"\tO comando help tem saídas em diferentes cores.\n"
	"\tSaídas em:\n"
	ANSI_BOLD_YEL  "\tamarelo    " ANSI_COLOR_WHT ": o comando em sua forma por extenso e simplificada.\n"
	ANSI_BOLD_CYA  "\tazul claro " ANSI_COLOR_WHT ": saídas no terminal feitas pelo luof.\n"
	ANSI_COLOR_BLU "\tazul escuro" ANSI_COLOR_WHT ": entradas do usuário feitas dentro do luof.\n\n"
	ANSI_COLOR_WHT "\tNos exemplos há linhas com o caractere " ANSI_BOLD_WHT ">" ANSI_COLOR_WHT
	" seguido por um comando, essa linha representa a entrada feita no teminal pelo usuário"
	" para executar um comando do luof.\n\n"

	ANSI_BOLD_WHT "Exemplos:\n"
	ANSI_BOLD_WHT "> " ANSI_COLOR_WHT "luof -h\n"
	ANSI_BOLD_WHT "> " ANSI_COLOR_WHT "luof --help\n"
	ANSI_BOLD_WHT "> " ANSI_COLOR_WHT "luof -h -ab\n"
	ANSI_BOLD_WHT "> " ANSI_COLOR_WHT "luof -h --add-bookmark\n"
	ANSI_BOLD_WHT "> " ANSI_COLOR_WHT "luof --help -rc\n"
	ANSI_BOLD_WHT "> " ANSI_COLOR_WHT "luof --help --remove-category\n\n"
	);
}

void fHelp_ab() {
	printf(
	ANSI_BOLD_WHT "luof:" ANSI_BOLD_YEL "\t-ab\tou   --add-bookmark:\n\n"

	ANSI_COLOR_WHT
	"\tUse para guardar um favorito. Primeiro informe a categoria na qual se deseja adicionar"
	" o favorito, depois seu nome, depois o link da página, e, por fim, uma descrição sobre.\n"
	"\tÉ necessário preencher todos os campos requeridos. Caso tecle enter sem nada informado,"
	" terá uma quebra de linha na saída do terminal, mas a entrada daquele dado ainda será"
	" esperada. No geral, todos os campos aceitam um número grande de caracteres, com exceção"
	" do nome que aceita no máximo 99.\n\n"

	ANSI_BOLD_WHT "Args :" ANSI_COLOR_WHT "\tEssa função permite passar na linha de comando o"
	" caminho do favorito a ser adicionado, ou seja, a categoria junto com seu nome.\n\n"

	ANSI_BOLD_WHT "Exemplos :\n"
	"> " ANSI_COLOR_WHT "luof --add-bookmark\n"
	ANSI_BOLD_CYA "Categoria : " ANSI_COLOR_BLU "Jogos/steam\n"
	ANSI_BOLD_CYA "Nome      : " ANSI_COLOR_BLU "Dark Souls\n"
	ANSI_BOLD_CYA "Link      : " ANSI_COLOR_BLU "https://store.steampowered.com/app/570940/DARK_SOULS_REMASTERED/\n"
	ANSI_BOLD_CYA "Descrição : " ANSI_COLOR_BLU "Página da steam referente ao jogo Dark Souls: Remastered\n\n"
	ANSI_BOLD_CYA "Favorito adicionado com sucesso.\n\n"

	ANSI_BOLD_WHT "> " ANSI_COLOR_WHT "luof -ab Jogos/steam/Dark Souls\n"
	ANSI_BOLD_CYA "Link      : " ANSI_COLOR_BLU "https://store.steampowered.com/app/570940/DARK_SOULS_REMASTERED/\n"
	ANSI_BOLD_CYA "Descrição : " ANSI_COLOR_BLU "Página da steam referente ao jogo Dark Souls: Remastered\n\n"
	ANSI_BOLD_CYA "Favorito adicionado com sucesso.\n\n"
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
	" esperada. O nome da categoria aceita no máximo 99 caracteres e não pode conter o caractere"
	" \"/\". Uma categoria pode no máximo alcançar 10 níveis de subcategoria (considerando que"
	" a raiz está na 0).\n\n"

	ANSI_BOLD_WHT "Args :" ANSI_COLOR_WHT "\tEssa função permite passar na linha de comando o"
	" caminho da categoria a ser adicionada, ou seja, a categoria pai junto com seu nome.\n\n"

	ANSI_BOLD_WHT "Exemplos :\n"
	"> " ANSI_COLOR_WHT "luof --add-category\n"
	ANSI_BOLD_CYA "Categoria pai     : " ANSI_COLOR_BLU "/\n"
	ANSI_BOLD_CYA "Nome da categoria : " ANSI_COLOR_BLU "Jogos\n\n"
	ANSI_BOLD_CYA "Categoria adicionada com sucesso.\n\n"

	ANSI_BOLD_WHT "> " ANSI_COLOR_WHT "luof -ac Jogos\n"
	ANSI_BOLD_CYA "\nCategoria adicionada com sucesso.\n\n"
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

	ANSI_BOLD_WHT "Args :" ANSI_COLOR_WHT "\tEssa função permite passar na linha de comando o"
	" caminho do favorito a ser removido, ou seja, a categoria junto com seu nome.\n\n"

	ANSI_BOLD_WHT "Exemplos:\n"
	"> " ANSI_COLOR_WHT "luof --remove-bookmark\n"
	ANSI_BOLD_CYA "Categoria : " ANSI_COLOR_BLU "Jogos/steam\n"
	ANSI_BOLD_CYA "Nome      : " ANSI_COLOR_BLU "Dark Souls\n\n"
	ANSI_BOLD_CYA "Favorito removido com sucesso.\n\n"

	ANSI_BOLD_WHT "> " ANSI_COLOR_WHT "luof -rb Jogos/steam/Dark Souls\n"
	ANSI_BOLD_CYA "\nFavorito removido com sucesso.\n\n"
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

	ANSI_BOLD_WHT "Args :" ANSI_COLOR_WHT "\tEssa função permite passar na linha de comando o"
	" caminho da categoria a ser removida, ou seja, a categoria pai junto com seu nome.\n\n"

	ANSI_BOLD_WHT "Exemplos:\n"
	"> " ANSI_COLOR_WHT "luof --remove-category\n"
	ANSI_BOLD_CYA "Categoria pai     : " ANSI_COLOR_BLU "Jogos\n"
	ANSI_BOLD_CYA "Nome da categoria : " ANSI_COLOR_BLU "steam\n\n"
	ANSI_BOLD_CYA "Categoria removida com sucesso.\n\n"

	ANSI_BOLD_WHT "> " ANSI_COLOR_WHT "luof -rc Jogos/steam\n"
	ANSI_BOLD_CYA "\nCategoria removida com sucesso.\n\n"
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

	ANSI_BOLD_WHT "Args :" ANSI_COLOR_WHT "\tEssa função permite passar na linha de comando o"
	" caminho do favorito a ser modificado, ou seja, a categoria junto com seu nome.\n\n"

	ANSI_BOLD_WHT "Exemplos:\n"
	ANSI_COLOR_WHT "\tCaso quisesse-se modificar o nome do favorito \"Dark Souls\""
	" pertencente a categoria \"Jogos/steam\":\n\n"
	ANSI_BOLD_WHT "> " ANSI_COLOR_WHT "luof --modify-bookmark\n"
	ANSI_BOLD_CYA "Categoria : " ANSI_COLOR_BLU "Jogos/steam\n"
	ANSI_BOLD_CYA "Nome      : " ANSI_COLOR_BLU "Dark Souls\n\n"
	ANSI_BOLD_CYA "Dados antigos:\n"
	"Categoria : Jogos/steam\n"
	"Nome      : Dark Souls\n"
	"Link      : https://store.steampowered.com/app/570940/DARK_SOULS_REMASTERED/\n"
	"Descrição : Página da steam referente ao jogo Dark Souls: Remastered\n\n"
	"Você deseja modificar? [1]categoria [2]nome [3]link [4]descrição [5]tudo [6]nada : " ANSI_COLOR_BLU "2\n\n"
	ANSI_BOLD_CYA "Novos dados:\n"
	ANSI_BOLD_CYA "Nome      : " ANSI_COLOR_BLU "Dark Souls II\n\n"
	ANSI_BOLD_CYA "Favorito modificado com sucesso.\n\n"


	ANSI_COLOR_WHT "\tCaso quisesse-se modificar o nome, o link e o texto do favorito \"Dark Souls\""
	" pertencente a categoria \"Jogos/steam\", poderia-se escolher a opção tudo (teclando 5"
	" quando requisitado a entrada), fazer as modificações necessárias e copiar os dados"
	" que não deseja-se modificar:\n\n"
	ANSI_BOLD_WHT "> " ANSI_COLOR_WHT "luof -mb Jogos/steam/Dark Souls\n"
	ANSI_BOLD_CYA "Dados antigos:\n"
	"Categoria : Jogos/steam\n"
	"Nome      : Dark Souls\n"
	"Link      : https://store.steampowered.com/app/570940/DARK_SOULS_REMASTERED/\n"
	"Descrição : Página da steam referente ao jogo Dark Souls: Remastered\n\n"
	"Você deseja modificar? [1]categoria [2]nome [3]link [4]descrição [5]tudo [6]nada : " ANSI_COLOR_BLU "5\n\n"
	ANSI_BOLD_CYA "Novos dados:\n"
	ANSI_BOLD_CYA "Categoria : " ANSI_COLOR_BLU "Jogos/steam\n"
	ANSI_BOLD_CYA "Nome      : " ANSI_COLOR_BLU "Dark Souls II\n"
	ANSI_BOLD_CYA "Link      : " ANSI_COLOR_BLU "https://store.steampowered.com/app/335300/DARK_SOULS_II_Scholar_of_the_First_Sin/\n"
	ANSI_BOLD_CYA "Descrição : " ANSI_COLOR_BLU "Página da steam referente ao jogo Dark souls II: Scholar Of The First Sin\n\n"
	ANSI_BOLD_CYA "Favorito modificado com sucesso.\n\n"
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

	ANSI_BOLD_WHT "Args :" ANSI_COLOR_WHT "\tEssa função permite passar na linha de comando o"
	" caminho da categoria a ser modificada, ou seja, a categoria pai junto com seu nome.\n\n"

	ANSI_BOLD_WHT "Exemplos:\n"
	ANSI_COLOR_WHT "\tCaso quisesse-se modificar a categoria pai da categoria \"Jogos/steam\" para a"
	" categoria raiz:\n\n"
	ANSI_BOLD_WHT "> " ANSI_COLOR_WHT "luof --modify-category\n"
	ANSI_BOLD_CYA "Categoria pai     : " ANSI_COLOR_BLU "Jogos\n"
	ANSI_BOLD_CYA "Nome da categoria : " ANSI_COLOR_BLU "steam\n\n"
	ANSI_BOLD_CYA "Você deseja modificar? [1]categoria pai [2]nome da categoria [3]tudo [4]nada : " ANSI_COLOR_BLU "1\n\n"
	ANSI_BOLD_CYA "Novos dados:\n"
	ANSI_BOLD_CYA "Categoria pai     : " ANSI_COLOR_BLU "/\n\n"
	ANSI_BOLD_CYA "Categoria modificada com sucesso.\n\n"

	ANSI_COLOR_WHT "\tCaso quisesse-se modificar a categoria pai da categoria \"Jogos/steam\" para a"
	" categoria raiz e seu nome para \"Steam\":\n\n"
	ANSI_BOLD_WHT "> " ANSI_COLOR_WHT "luof -mc Jogos/steam\n"
	ANSI_BOLD_CYA "Você deseja modificar? [1]categoria pai [2]nome da categoria [3]tudo [4]nada : " ANSI_COLOR_BLU "3\n\n"
	ANSI_BOLD_CYA "Novos dados:\n"
	ANSI_BOLD_CYA "Categoria pai     : " ANSI_COLOR_BLU "/\n"
	ANSI_BOLD_CYA "Nome da categoria : " ANSI_COLOR_BLU "Steam\n\n"
	ANSI_BOLD_CYA "Categoria modificada com sucesso.\n\n"
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

		ANSI_BOLD_WHT "Args :"
		ANSI_COLOR_WHT "\tEssa função permite passar na linha de comando:\n"
		"\t-s ou --short para não exibir os dados dos favoritos, apenas seus nomes.\n"
		"\tO caminho da categoria a ser visualizada: ou seja, a categoria pai junto com seu nome.\n"
		"\t(Caso passado os dois argumentos, o caminho da categoria deve ser passada por"
		" último)\n\n"

		ANSI_BOLD_WHT "Exemplos:\n"
		"> " ANSI_COLOR_WHT "luof --list-category\n"
		ANSI_BOLD_CYA "Categoria : " ANSI_COLOR_BLU "/\n\n" ANSI_BOLD_CYA
		"* Jogos\n\n"
		"Nome      : ilovepdf\n"
		"Link      : https://www.ilovepdf.com/pt\n"
		"Descrição : Site com diversas ferramentas para manipular pdf's\n\n"
		"Nome      : randoma11y\n"
		"Link      : https://randoma11y.com/\n"
		"Texto     : Site que combina cores aleatoriamente para uso em páginas web\n\n"

		ANSI_BOLD_WHT "> " ANSI_COLOR_WHT "luof -lc\n"
		ANSI_BOLD_CYA "Categoria : " ANSI_COLOR_BLU "Jogos\n\n" ANSI_BOLD_CYA
		"* steam\n\n"

		ANSI_BOLD_WHT "> " ANSI_COLOR_WHT "luof -lc -s /\n\n"
		ANSI_BOLD_CYA
		"* Jogos\n"
		"ilovepdf\n"
		"randoma11y\n\n"
		);
	}
	else {
		printf(
		ANSI_BOLD_WHT "luof:" ANSI_BOLD_YEL "\t-lcs\tou   --list-category-short:\n\n"

		ANSI_COLOR_WHT
		"\tUse para ver o nome das categorias e nome dos favoritos que pertencem a categoria"
		" informada. Onde as categorias serão mostradas com um apóstrofo (*) antes de seu"
		" nome, e os favoritos terão apenas seu nome mostrado."
		" O nome da categoria também pode ser passado na linha de comandos.\n\n"

		ANSI_BOLD_WHT "Args :" ANSI_COLOR_WHT "\tEssa função permite passar na linha de comando o"
		" caminho da categoria a ser visualizada, ou seja, a categoria pai junto com seu nome.\n\n"

		ANSI_BOLD_WHT "Exemplos:\n"
		"> " ANSI_COLOR_WHT "luof -lcs\n"
		ANSI_BOLD_CYA "\nCategoria : " ANSI_COLOR_BLU "/\n\n" ANSI_BOLD_CYA
		"* Jogos\n\n"
		"ilovepdf\n"
		"randoma11y\n\n"

		ANSI_BOLD_WHT "> " ANSI_COLOR_WHT "luof -lcs /\n\n"
		ANSI_BOLD_CYA "* Jogos\n\n"
		"ilovepdf\n"
		"randoma11y\n\n"
		);
	}
}

void fHelp_lt(int opcao) {
	if (opcao == 0) {
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
	else {
		printf(
		ANSI_BOLD_WHT "luof:" ANSI_BOLD_YEL "\t-lts\tou   --list-tree-short:\n\n"
		ANSI_COLOR_WHT
		"\tUse para ver todas as categorias que estão armazenadas. Os dados serão"
		" mostrados em forma de árvore, obedecendo à hierarquia das categorias.\n"
		"\tAlém de mostrar uma visão de todas as categoria do banco, serve principalmente para saber"
		" o caminho de algo específico.\n\n"
		ANSI_BOLD_WHT "Ex:"
		ANSI_COLOR_WHT " Caso o comando \"luof -lts\" gerasse a saída:\n"
		ANSI_BOLD_CYA "|_ * Jogos\n"
		"|    |_ * steam\n"
		"|         |_ * Dark Souls\n"
		"|_ * séries\n"
		"     |_ * Netflix\n"
		"     |_ * TV\n"
		"          |_ * Comédia\n"
		"          |_ * Romance\n\n"
		ANSI_COLOR_WHT "\tCaso quisesse-se listar uma categoria, poderia ser usado como base o resultado dessa"
		" pesquisa, que diferentemente de -lt mostraria apenas o que de fato interessa, as categorias. Se"
		" por exemplo, quisesse-se saber os favoritos da categoria \"Dark Souls\", basta entrar com:"
		" \"Jogos/steam/Dark Souls\" quando requisitado a categoria.\n\n"
		);
	}
}

void fHelp_backup() {
	printf(
	ANSI_BOLD_WHT "luof:" ANSI_BOLD_YEL "\t--backup:\n\n"
	ANSI_COLOR_WHT
	"\tUse para criar ou restaurar um backup. Informe a opção desejada; se a opção for"
	" [1]criar, o arquivo de backup (o arquivo possui o nome luof.bkp) será adicionado"
	" no diretório atual, caso já exista um arquivo de backup no diretório ele não será"
	" perdido. Caso a opção seja [2]restaurar, será necessário informar o caminho do arquivo"
	" de backup que se deseja restaurar, lembrando que ao restaurar um backup todos os"
	" dados atuais do banco serão perdidos.\n\n"
	ANSI_BOLD_WHT "Ex:"
	ANSI_COLOR_WHT " Caso quisesse-se criar um backup, porém na pasta atual já exista o arquivo luof.bkp:\n"
	ANSI_BOLD_CYA "Você deseja criar um backup novo ou restaurar um antigo? [1]criar [2]restaurar [3]sair : " ANSI_COLOR_BLU "1\n\n"
	ANSI_BOLD_CYA "Arquivo luof-1.bkp adicionado no diretório atual.\n"
	"Backup criado com sucesso.\n\n"
	ANSI_BOLD_WHT "Ex:"
	ANSI_COLOR_WHT " Caso quisesse-se restaurar um backup com o caminho \"/home/usuario/Downloads/luof-5.bkp\":\n"
	ANSI_BOLD_CYA "Você deseja criar um backup novo ou restaurar um antigo? [1]criar [2]restaurar [3]sair : " ANSI_COLOR_BLU "2\n\n"
	ANSI_BOLD_CYA "Informe o caminho do arquivo de backup: " ANSI_COLOR_BLU "/home/usuario/Downloads/luof-5.bkp\n"
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
	ANSI_BOLD_WHT "Ex:"
	ANSI_COLOR_WHT " Caso quisesse-se exportar a categoria \"Jogos\" e não exista nenhum arquivo exportado no diretório atual:\n"
	ANSI_BOLD_CYA "Categoria : " ANSI_COLOR_BLU "Jogos\n\n"
	ANSI_BOLD_CYA "Arquivo bookmarks-luof.html adicionado no diretório atual.\n"
	"Export criado com sucesso.\n\n"
	);
}

void fHelp_import() {
	printf(
	ANSI_BOLD_WHT "luof:" ANSI_BOLD_YEL "\t--import:\n\n"
	ANSI_COLOR_WHT
	"\tUse para importar favoritos exportados de outro browser para dentro de uma"
	" categoria. É necessário informar o caminho do arquivo gerado pelo browser, e"
	" a categoria onde os favoritos serão inseridos.\n\n"
	ANSI_BOLD_WHT "Ex:"
	ANSI_COLOR_WHT " Caso quisesse-se importar o arquivo \"bookmarks.html\" localizado no"
	" diretório atual para dentro da categoria \"Jogos\":\n"
	ANSI_BOLD_CYA "Informe o caminho do arquivo de import: " ANSI_COLOR_BLU "bookmarks.html\n"
	ANSI_BOLD_CYA "Categoria pai: " ANSI_COLOR_BLU "Jogos\n\n"
	ANSI_BOLD_CYA "Import feito com sucesso.\n\n"
	);
}

void fHelp_free() {
	printf(
	ANSI_BOLD_WHT "luof:" ANSI_BOLD_YEL "\t--free:\n\n"
	ANSI_COLOR_WHT
	"\tUse para apagar todas categorias e favoritos armazenados. Não haverá como voltar"
	" atrás e recuperar tudo que foi excluído, por isso é fortemente recomendado realizar"
	" um backup antes de continuar com a execução da função.\n"
	"\tAo usar esse comando será perguntado se há certeza em continuar, se sim, devesse"
	" teclar 's' e depois enter.\n\n"
	);
}

void fHelp(sCom com) {

	if (com.flag[0] == '\0') {
		fHelp_private();
	}
	else if (strcmp(com.flag, "-h") == 0 || strcmp(com.flag, "--help") == 0) {
		fHelp_h();
	}
	else if (strcmp(com.flag, "-ab") == 0 || strcmp(com.flag, "--add-bookmark") == 0) {
		fHelp_ab();
	}
	else if (strcmp(com.flag, "-ac") == 0 || strcmp(com.flag, "--add-category") == 0) {
		fHelp_ac();
	}
	else if (strcmp(com.flag, "-rb") == 0 || strcmp(com.flag, "--remove-bookmark") == 0) {
		fHelp_rb();
	}
	else if (strcmp(com.flag, "-rc") == 0 || strcmp(com.flag, "--remove-category") == 0) {
		fHelp_rc();
	}
	else if (strcmp(com.flag, "-mb") == 0 || strcmp(com.flag, "--modify-bookmark") == 0) {
		fHelp_mb();
	}
	else if (strcmp(com.flag, "-mc") == 0 || strcmp(com.flag, "--modify-category") == 0) {
		fHelp_mc();
	}
	else if (strcmp(com.flag, "-lc") == 0 || strcmp(com.flag, "--list-category") == 0) {
		fHelp_lc(0);
	}
	else if (strcmp(com.flag, "-lcs") == 0 || strcmp(com.flag, "--list-category-short") == 0) {
		fHelp_lc(1);
	}
	else if (strcmp(com.flag, "-lt") == 0 || strcmp(com.flag, "--list-tree") == 0) {
		fHelp_lt(0);
	}
	else if (strcmp(com.flag, "-lts") == 0 || strcmp(com.flag, "--list-tree-short") == 0) {
		fHelp_lt(1);
	}
	else if (strcmp(com.flag, "--backup") == 0) {
		fHelp_backup();
	}
	else if (strcmp(com.flag, "--export") == 0) {
		fHelp_export();
	}
	else if (strcmp(com.flag, "--import") == 0) {
		fHelp_import();
	}
	else if (strcmp(com.flag, "--free") == 0) {
		fHelp_free();
	}
	else {
		printf(ERRO2);
		printf("%s não é um comando de luof. Use \"luof --help\" para ver comandos válidos.\n", com.flag);
	}

}
