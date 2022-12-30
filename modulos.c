#include "luof.h"

int fSetaSiteCategoria(sSite *s) {

	int cont = 0;
	char c;

	printf(ANSI_BOLD_WHT "Categoria : " ANSI_COLOR_GRA);

	do {
		c = getchar();
	} while (isspace(c));

	do {
		if (cont == 1 && s->categoria[0] == '/') {
			cont--;
		}
		else if (cont == TAMCAMINHO - 1) {
			printf(ERRO);
			printf("Categorias devem ter no máximo %d caracteres.\n", TAMCAMINHO - 1);
			return 1;
		}
		s->categoria[cont] = c;
		cont++;
		c = getchar();
	} while (c != '\n');
	s->categoria[cont] = '\0';

	return 0;

}

int fSetaSiteNome(sSite *s) {

	int cont = 0;
	char c;

	printf(ANSI_BOLD_WHT "Nome      : " ANSI_COLOR_GRA);

	do {
		c = getchar();
	} while (isspace(c));

	do {
		if (cont == TAMNOMEFAV - 1) {
			printf(ERRO);
			printf("Nomes de favoritos devem ter no máximo %d caracteres.\n", TAMNOMEFAV - 1);
			return 1;
		}
		s->nome[cont] = c;
		cont++;
		c = getchar();
	} while (c != '\n');
	s->nome[cont] = '\0';

	return 0;

}

int fSetaSiteLink(sSite *s) {

	int cont = 0;
	char c;

	printf(ANSI_BOLD_WHT "Link      : " ANSI_COLOR_GRA);

	do {
		c = getchar();
	} while (isspace(c));

	do {
		if (cont == TAMLINKARQ - 1) {
			printf(ERRO);
			printf("Links devem ter no máximo %d caracteres.\n", TAMLINKARQ - 1);
			return 1;
		}
		s->link[cont] = c;
		cont++;
		c = getchar();
	} while (c != '\n');
	s->link[cont] = '\0';

	return 0;

}

int fSetaSiteTexto(sSite *s) {

	int cont = 0;
	char c;

	printf(ANSI_BOLD_WHT "Texto     : " ANSI_COLOR_GRA);

	do {
		c = getchar();
	} while (isspace(c));

	do {
		if (cont == TAMTEXTO - 1) {
			printf(ERRO);
			printf("Textos devem ter no máximo %d caracteres.\n", TAMTEXTO - 1);
			return 1;
		}
		s->texto[cont] = c;
		cont++;
		c = getchar();
	} while (c != '\n');
	s->texto[cont] = '\0';

	return 0;

}

int fSetaCatCategoria(sSite *s) {

	int cont = 0;
	char c;

	printf(ANSI_BOLD_WHT "Categoria pai     : " ANSI_COLOR_GRA);

	do {
		c = getchar();
	} while (isspace(c));

	do {
		if (cont == 1 && s->categoria[0] == '/') {
			cont--;
		}
		else if (cont == TAMCAMINHO - 1) {
			printf(ERRO);
			printf("Categorias devem ter no máximo %d caracteres.\n", TAMCAMINHO - 1);
			return 1;
		}
		s->categoria[cont] = c;
		cont++;
		c = getchar();
	} while (c != '\n');
	s->categoria[cont] = '\0';

	return 0;

}

int fSetaCatNome(sSite *s) {

	int cont = 0;
	char c;

	printf(ANSI_BOLD_WHT "Nome da categoria : " ANSI_COLOR_GRA);

	do {
		c = getchar();
	} while (isspace(c));

	do {
		if (c == '/') {
			printf(ERRO);
			printf("Categorias não podem ter '/' no nome.\n");
			return 1;
		}
		else if (cont == TAMNOMEFAV - 1) {
			printf(ERRO);
			printf("Nomes de categorias devem ter no máximo %d caracteres.\n", TAMNOMEFAV - 1);
			return 1;
		}
		s->nome[cont] = c;
		cont++;
		c = getchar();
	} while (c != '\n');
	s->nome[cont] = '\0';

	if (strcmp(s->nome, "luof") == 0) {
		printf(ERRO);
		printf("luof é um nome reservado do sistema e não pode ser usado para nomes de categorias.\n");
		return 1;
	}

	return 0;

}

void fSetaCaminhoArquivo(sBanco *db, char *arq, char *nome) {
	strcpy(arq, db->caminhoDB);
	strcpy(&arq[db->tamCaminhoDB], nome);
}

void fIncrementaCamCat(char *caminho, char *nome) {
	strcat(caminho, "/");
	strcat(caminho, nome);
}

void fSetaCaminhoCategoria(char caminho[], sSite s) {
	strcpy(caminho, s.categoria);
	if (strcmp(s.categoria, "luof") == 0)
		strcpy(caminho, s.nome);
	else
		fIncrementaCamCat(caminho, s.nome);
}

sSite fRecuperaFavorito(FILE *arq, char *nomeT) {

	sSite s;
	char ehCategoria[3];

	if (nomeT) {
		strncpy(s.nome, nomeT, strlen(nomeT));
		s.nome[strlen(nomeT)-1] = '\0';
	}
	else {
		fgets(s.nome, TAMCAMINHO, arq);
		s.nome[strlen(s.nome)-1] = '\0';
	}

	fgets(s.categoria, TAMCAMINHO, arq);
	s.categoria[strlen(s.categoria)-1] = '\0';

	fgets(s.link, TAMLINKARQ, arq);
	s.link[strlen(s.link)-1] = '\0';

	fgets(s.texto, TAMTEXTO, arq);
	s.texto[strlen(s.texto)-1] = '\0';

	fgets(ehCategoria, 3, arq);
	s.ehCat = ehCategoria[0];

	return s;

}

int fSeparaArquivoCategoria(sBanco *db, char categoria[], sCat *cat, char nomeA[]) {

	sSite siteTemp, *siteDoIterador, *siteDoIterador2;
	sLista listaSitesA, listaSitesN;
	sIterador it, it2;
	FILE *arq;
	char nomeTemp[TAMNOMEFAV], nomeArqCat[TAMLINKARQ];
	int qtdLista, encontrouPos;

	//preenche db->listaSites com os favoritos do arquivo com nome antigo
	listaSitesA = db->listaSites;
	strcpy(nomeTemp, cat->nome);
	strcpy(cat->nome, nomeA);
	fPreencheListaSite(db, cat);
	strcpy(cat->nome, nomeTemp);

	//verifica se já existe um arquivo com o novo nome da categoria, se sim preenche uma lista com seus favoritos
	listaSitesN = criaLista(struct sSite);
	fSetaCaminhoArquivo(db, nomeArqCat, cat->nome);
	arq = fopen(nomeArqCat, "r");
	if (arq) {
		while (fgets(nomeTemp, 100, arq) != NULL) {
			siteTemp = fRecuperaFavorito(arq, nomeTemp);
			pushBackList(listaSitesN, &siteTemp);
		}
		fclose(arq);
	}

	//remove os sites da categoria da db->listaSites (arquivo com nome antigo) e insere no listaSitesN (arquivo com nome novo)
	it = criaIt(db->listaSites);
	it2 = criaIt(listaSitesN);
	if (!emptyList(db->listaSites)) {
		do {
			qtdLista = sizeList(db->listaSites);
			siteDoIterador = (struct sSite*) retornaItera(&it);
			if (strcmp(siteDoIterador->categoria, categoria) == 0) {
				//----------------------------
				if (emptyList(listaSitesN)) {
					pushBackList(listaSitesN, siteDoIterador);
				}
				else if (siteDoIterador->ehCat == '0') {
					encontrouPos = 0;
					iteraFim(&it2);
					do {
						siteDoIterador2 = (struct sSite*) retornaItera(&it2);
						if (strcmp(siteDoIterador2->nome, siteDoIterador->nome) < 0 || siteDoIterador2->ehCat == '1')
							encontrouPos = 1;
						else
							iteraAnterior(&it2);
					} while (encontrouPos == 0 && !fimIt(&it2));
					siteDoIterador2 = (struct sSite*) frontList(listaSitesN);
					if (siteDoIterador2->ehCat == '0' && encontrouPos == 0)
						pushFrontList(listaSitesN, siteDoIterador);
					else
						insereProxIt(&it2, siteDoIterador);
				}
				else {
					encontrouPos = 0;
					iteraInicio(&it2);
					do {
						siteDoIterador2 = (struct sSite*) retornaItera(&it2);
						if (strcmp(siteDoIterador2->nome, siteDoIterador->nome) > 0 || siteDoIterador2->ehCat == '0')
							encontrouPos = 1;
						else
							iteraProximo(&it2);
					} while (encontrouPos == 0 && !inicioIt(&it2));
					siteDoIterador2 = (struct sSite*) backList(listaSitesN);
					if (siteDoIterador2->ehCat == '1' && encontrouPos == 0)
						pushBackList(listaSitesN, siteDoIterador);
					else
						insereAntIt(&it2, siteDoIterador);
				}
				//----------------------------
				removeIt(&it);
			}
			else {
				iteraProximo(&it);
			}
		} while (!emptyList(db->listaSites) && (!inicioIt(&it) || qtdLista != sizeList(db->listaSites)));
	}

	//se a lista da categoria (nome antigo) ficou vazia remove o arquivo antigo, caso contrário só atualiza o arquivo
	if (emptyList(db->listaSites)) {
		fSetaCaminhoArquivo(db, nomeArqCat, nomeA);
		remove(nomeArqCat);
	}
	else {
		 fEscreveArquivoCat(db, nomeA);
	}

	//escreve o arquivo novo da categoria
	if (!emptyList(listaSitesN)) {
		 db->listaSites = listaSitesN;
		 fEscreveArquivoCat(db, cat->nome);
	}

	freeList(db->listaSites);
	db->listaSites = listaSitesA;

	return 0;

}

void fMudaCaminhoCategoriaArvore(sBanco *db, sCat *cat, char *caminhoA, char *caminhoN) {

	sSite *siteDoIterador;
	sCat *catTemp;
	sIterador it;
	char caminhoAntigo[TAMCAMINHO], caminhoNovo[TAMCAMINHO];//serão usados dentro de um loop, por isso não altera caminhoA e caminhoN

	//preenche a lista dos favoritos da categoria
	fPreencheListaSite(db, cat);

	if (emptyList(db->listaSites))
		return;

	//corrige a categoria dos favoritos da categoria
	it = criaIt(db->listaSites);
	do {
		siteDoIterador = (struct sSite*) retornaItera(&it);
		if (strcmp(siteDoIterador->categoria, caminhoA) == 0)
			strcpy(siteDoIterador->categoria, caminhoN);
		iteraProximo(&it);
	} while (!inicioIt(&it));

	//reescreve seu arquivo
	fEscreveArquivoCat(db, cat->nome);

	if (emptyList(cat->catFilhos))
		return;

	//chama a mesma função para suas categorias
	it = criaIt(cat->catFilhos);
	do {
		catTemp = (struct sCat*) retornaItera(&it);

		strcpy(caminhoAntigo, caminhoA);
		fIncrementaCamCat(caminhoAntigo, catTemp->nome);
		strcpy(caminhoNovo, caminhoN);
		fIncrementaCamCat(caminhoNovo, catTemp->nome);
		fMudaCaminhoCategoriaArvore(db, catTemp, caminhoAntigo, caminhoNovo);

		iteraProximo(&it);
	} while (!inicioIt(&it));

}
