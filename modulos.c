#include "luof.h"

void fEscreveLuof_private(sBanco *db, sLista listaCategorias, int hierarquia) {
	
	if (!emptyList(listaCategorias)) {
		sIterador it = criaIt(listaCategorias);
		do {
			sCat *cat = (struct sCat*) retornaItera(&it);
			//escreve no arquivo
			fprintf(db->aLuof, "%d%s\n", hierarquia, cat->nome);
			//------------------
			fEscreveLuof_private(db, cat->catFilhos, hierarquia+1);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}
	
}

void fEscreveLuof(sBanco *db) {
	
	char nomeArqCat[TAMLINKARQ];
	sSite *siteDoIterador;
	sIterador it;
	
	//reabre o arquivo aLuof para sobreescreve-lo
	fSetaCaminhoArquivo(nomeArqCat, "luof");
	if (db->aLuof)
		db->aLuof = freopen(nomeArqCat, "w", db->aLuof);
	else
		db->aLuof = fopen(nomeArqCat, "w");

	//escreve a árvore de categorias no arquivo aLuof
	fEscreveLuof_private(db, db->listaCategorias->catFilhos, 0);
	
	//marca o fim da árvore de categorias
	fprintf(db->aLuof, "##\n");
	
	//escreve os favoritos da raiz
	it = criaIt(db->raiz);
	do {
		siteDoIterador = retornaItera(&it);
		fprintf(db->aLuof, "%s\n%s\n%s\n%s\n%c\n", siteDoIterador->nome, siteDoIterador->categoria, siteDoIterador->link, siteDoIterador->texto, siteDoIterador->ehCat);
		iteraProximo(&it);
	} while (!inicioIt(&it));

}

int fSeparaArquivoCategoria(sBanco *db, char categoria[], sCat *cat, char nomeA[]) {

	sSite siteTemp, *siteDoIterador, *siteDoIterador2;
	sLista listaSitesA, listaSitesN;
	sIterador it, it2;
	FILE *arq;
	char nomeTemp[TAMNOMEFAV], ehCategoria[3], nomeArqCat[TAMLINKARQ];
	int qtdLista, encontrouPos;

	//preenche db->listaSites com os favoritos do arquivo com nome antigo
	listaSitesA = db->listaSites;
	strcpy(nomeTemp, cat->nome);
	strcpy(cat->nome, nomeA);
	fPreencheListaSite(db, cat);
	strcpy(cat->nome, nomeTemp);

	//verifica se já existe um arquivo com o novo nome da categoria, se sim preenche uma lista com seus favoritos
	listaSitesN = criaLista(struct sSite);
	fSetaCaminhoArquivo(nomeArqCat, cat->nome);
	arq = fopen(nomeArqCat, "r");
	if (arq) {
		while (fgets(nomeTemp, 100, arq) != NULL) {
			nomeTemp[strlen(nomeTemp)-1] = '\0';
			strcpy(siteTemp.nome, nomeTemp);

			fgets(siteTemp.categoria, TAMCAMINHO, arq);
			siteTemp.categoria[strlen(siteTemp.categoria)-1] = '\0';

			fgets(siteTemp.link, TAMLINKARQ, arq);
			siteTemp.link[strlen(siteTemp.link)-1] = '\0';

			fgets(siteTemp.texto, TAMTEXTO, arq);
			siteTemp.texto[strlen(siteTemp.texto)-1] = '\0';

			fgets(ehCategoria, 3, arq);
			siteTemp.ehCat = ehCategoria[0];

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
			siteDoIterador = retornaItera(&it);
			if (strcmp(siteDoIterador->categoria, categoria) == 0) {
				//----------------------------
				if (emptyList(listaSitesN)) {
					pushBackList(listaSitesN, siteDoIterador);
				}
				else if (siteDoIterador->ehCat == '0') {
					encontrouPos = 0;
					iteraFim(&it2);
					do {
						siteDoIterador2 = retornaItera(&it2);
						if (strcmp(siteDoIterador2->nome, siteDoIterador->nome) < 0 || siteDoIterador2->ehCat == '1')
							encontrouPos = 1;
						else
							iteraAnterior(&it2);
					} while (encontrouPos == 0 && !fimIt(&it2));
					siteDoIterador2 = frontList(listaSitesN);
					if (siteDoIterador2->ehCat == '0' && encontrouPos == 0)
						pushFrontList(listaSitesN, siteDoIterador);
					else
						insereProxIt(&it2, siteDoIterador);
				}
				else {
					encontrouPos = 0;
					iteraInicio(&it2);
					do {
						siteDoIterador2 = retornaItera(&it2);
						if (strcmp(siteDoIterador2->nome, siteDoIterador->nome) > 0 || siteDoIterador2->ehCat == '0')
							encontrouPos = 1;
						else
							iteraProximo(&it2);
					} while (encontrouPos == 0 && !inicioIt(&it2));
					siteDoIterador2 = backList(listaSitesN);
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
		fSetaCaminhoArquivo(nomeArqCat, nomeA);
		remove(nomeArqCat);
	}
	else {
		fSetaCaminhoArquivo(nomeArqCat, nomeA);
		arq = fopen(nomeArqCat, "w");
		if (!arq) {
			printf("\nO arquivo %s não pode ser aberto\n", nomeArqCat);
			return 1;
		}
		it = criaIt(db->listaSites);
		do {
			siteDoIterador = retornaItera(&it);
			fprintf(arq, "%s\n%s\n%s\n%s\n%c\n", siteDoIterador->nome, siteDoIterador->categoria, siteDoIterador->link, siteDoIterador->texto, siteDoIterador->ehCat);
			iteraProximo(&it);
		} while (!inicioIt(&it));
		fclose(arq);
	}
	
	//escreve o arquivo novo da categoria
	if (!emptyList(listaSitesN)) {
		fSetaCaminhoArquivo(nomeArqCat, cat->nome);
		arq = fopen(nomeArqCat, "w");
		if (!arq) {
			printf("\nO arquivo %s não pode ser aberto\n", nomeArqCat);
			return 1;
		}
		arq = fopen(nomeArqCat, "w");
		it = criaIt(listaSitesN);
		do {
			siteDoIterador = retornaItera(&it);
			fprintf(arq, "%s\n%s\n%s\n%s\n%c\n", siteDoIterador->nome, siteDoIterador->categoria, siteDoIterador->link, siteDoIterador->texto, siteDoIterador->ehCat);
			iteraProximo(&it);
		} while (!inicioIt(&it));
		fclose(arq);
	}
	
	freeList(db->listaSites);
	db->listaSites = listaSitesA;

	return 0;

}

void fMudaCaminhoCategoriaArvore_private(sBanco *db, sCat *cat, char *caminhoA, char *caminhoN) {
	
	sIterador it;
	sSite *siteDoIterador;
	sCat *catTemp;
	char caminhoAntigo[TAMCAMINHO], caminhoNovo[TAMCAMINHO];//serão usados dentro de um loop, por isso não altera caminhoA e caminhoN
	
	//preenche a lista dos favoritos da categoria
	fPreencheListaSite(db, cat);
	
	if (emptyList(db->listaSites))
		return;

	//corrige a categoria dos favoritos da categoria
	it = criaIt(db->listaSites);
	do {
		siteDoIterador = (struct sSite*) retornaItera(&it);
		if (strcmp(siteDoIterador->categoria, caminhoA) == 0) {
			strcpy(siteDoIterador->categoria, caminhoN);
		}
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
		fMudaCaminhoCategoriaArvore_private(db, catTemp, caminhoAntigo, caminhoNovo);
		
		iteraProximo(&it);
	} while (!inicioIt(&it));
	
}

void fMudaCaminhoCategoriaArvore(sBanco *db, char *caminho1, char *caminho2) {
	
	int encontrou = 0;
	char caminhoCatAtual[TAMCAMINHO];
	sIterador it;
	sCat *categoria, *catTemp, *categoriaPai;

	sSite s;
	strcpy(s.categoria, caminho1);
	s.ehCat = '1';
	
	fBuscaCat(db, s, &categoria);
	
	if (strcmp(caminho2, "luof")) {
		//adiciona a categoria no lugar correto
		pushBackList(db->listaCategorias->catFilhos, categoria);
		
		strcpy(caminhoCatAtual, categoria->nome);
	}
	else {
		
		strcpy(s.categoria, caminho2);
		
		fBuscaCat(db, s, &categoriaPai);
		//adiciona a categoria no lugar correto
		pushBackList(categoriaPai->catFilhos, categoria);
		
		strcpy(caminhoCatAtual, caminho2);
		fIncrementaCamCat(caminhoCatAtual, categoria->nome);
	}
	
	//remove a categoria do lugar antigo
	if (categoria->catPai)
		it = criaIt(categoria->catPai->catFilhos);
	else
		it = criaIt(db->listaCategorias->catFilhos);

	do {
		catTemp = (struct sCat*) retornaItera(&it);
		if (strcmp(catTemp->nome, categoria->nome) == 0) {
			encontrou = 1;
			removeIt(&it);
		}
		else {
			iteraProximo(&it);
		}
	} while (encontrou == 0 && !inicioIt(&it));
	
	//corrige os favoritos da categoria modificada
	fMudaCaminhoCategoriaArvore_private(db, categoria, caminho1, caminhoCatAtual);
	
}

void fEscreveArquivoCat(sBanco *db, char *nomeArq) {
	
	char nomeArqCat[TAMLINKARQ];
	sSite *siteDoIterador;
	sIterador it;

	//reabre o arquivo para sobreescreve-lo
	fSetaCaminhoArquivo(nomeArqCat, nomeArq);
	if (db->aCat)
		db->aCat = freopen(nomeArqCat, "w", db->aCat);
	else
		db->aCat = fopen(nomeArqCat, "w");

	if (!emptyList(db->listaSites)) {
		//escreve a lista no arquivo da categoria
		it = criaIt(db->listaSites);
		do {
			siteDoIterador = retornaItera(&it);
			fprintf(db->aCat, "%s\n%s\n%s\n%s\n%c\n", siteDoIterador->nome, siteDoIterador->categoria, siteDoIterador->link, siteDoIterador->texto, siteDoIterador->ehCat);
			iteraProximo(&it);
		} while (!inicioIt(&it));
	}
	
}

void fSetaCaminhoArquivo(char *arq, char *nome) {
	strcpy(arq, caminhoDB);
	strcpy(&arq[tamCaminhoDB], nome);
}

void fIncrementaCamCat(char *caminho, char *nome) {
	strcat(caminho, "/");
	strcat(caminho, nome);	
}
