// CMakeProject1.cpp: define o ponto de entrada para o aplicativo.
//




#include "pch.h"

#include <stdio.h>
#include <stdlib.h> 
#include <chrono>

using namespace std;

int *lista_free_man;


class Info_entrada
{
public:
	Info_entrada(int _n, int *_preferencias_homens, int *_preferencias_mulheres)
	{
		n = _n;
		preferencias_homens = _preferencias_homens;
		preferencias_mulheres = _preferencias_mulheres;
	}

	~Info_entrada()
	{
		free(preferencias_homens);
		free(preferencias_mulheres);
	}


	int n;
	int *preferencias_homens;
	int *preferencias_mulheres;

private:


};


class No_lista
{
public:
	No_lista(int _conteudo, No_lista *_prox)
	{
		conteudo = _conteudo;
		proximo = _prox;
	}

	int get_conteudo()
	{
		return conteudo;
	}

	No_lista *get_prox()
	{
		return proximo;
	}

private:
	int conteudo;
	No_lista *proximo;
};


class Lista
{
public:

	Lista()
	{
		primeiro_no = NULL;
	}

	No_lista * get_primeiro_no()
	{
		return primeiro_no;
	}

	void insere_no(int conteudo)
	{
		No_lista *no = new No_lista(conteudo, primeiro_no);
		primeiro_no = no;
		n++;
	}

	int retira_primeiro_no()
	{
		int resposta = primeiro_no->get_conteudo();
		No_lista *novo_primeiro = primeiro_no->get_prox();
		delete primeiro_no;
		primeiro_no = novo_primeiro;
		n--;
		return resposta;
	}

	int get_n()
	{
		return n;
	}


private:
	No_lista *primeiro_no;
	int n;


};


Info_entrada *le_entrada(char *_arquivo)
{
	FILE *arquivo = fopen(_arquivo, "r");
	char lixo[100];
	int n;
	int *preferencias_homem;
	int *preferencias_mulher;
	Info_entrada *info_entrada;

	fgets(lixo, 100, arquivo);

	fscanf(arquivo, "%s", lixo);

	fscanf(arquivo, "%d", &n);

	fscanf(arquivo, "\n");

	preferencias_homem = (int *)malloc(sizeof(int) * n*n);
	preferencias_mulher = (int *)malloc(sizeof(int) * n*n);

	fgets(lixo, 100, arquivo);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			fscanf(arquivo, "%d", preferencias_homem + (i*n + j));
		}
	}
	fscanf(arquivo, "\n");

	fgets(lixo, 100, arquivo);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			fscanf(arquivo, "%d", preferencias_mulher + (i*n + j));
		}
	}

	info_entrada = new Info_entrada(n, preferencias_homem, preferencias_mulher);

	return info_entrada;

}


int *emparelha(Info_entrada *info_entrada, int *_passos)
{


	int n = info_entrada->n;
	int *preferencia_homens = info_entrada->preferencias_homens;
	int *preferencia_mulheres = info_entrada->preferencias_mulheres;
	int homem_corrente;
	int mulher_candidata;
	int parceiro_pe_na_bunda;

	//Estruturas 
	Lista *lista_solteiros = new Lista();
	int *prox_mulher = (int *)malloc(sizeof(int) * n);
	int *parceiro_atual = (int *)malloc(sizeof(int) * n);
	int *ranking_dos_homens = (int *)malloc(sizeof(int) * n*n);

	//Inicialização
	for (int i = n-1; i >= 0; i--)
	{
		lista_solteiros->insere_no(i);
		prox_mulher[i] = 0;
		parceiro_atual[i] = -1;
	}

	//Inicialização do ranking dos homens
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			ranking_dos_homens[i * n + preferencia_mulheres[i * n + j]] = j;
		}
	}

	int passos = 0;

	//enquanto tem solteiro
	while (lista_solteiros->get_primeiro_no())
	{
		//printf("%d\n", lista_solteiros->get_n());
		//pega esse solteiro
		homem_corrente = lista_solteiros->get_primeiro_no()->get_conteudo();
		//pega a mulher preferida do solteiro em quem ele não chegou junto
		mulher_candidata = preferencia_homens[homem_corrente * n + prox_mulher[homem_corrente]];
		//se está solteira
		if (parceiro_atual[mulher_candidata] == -1)
		{
			//casa
			parceiro_atual[mulher_candidata] = lista_solteiros->retira_primeiro_no();
		}
		else
		{
			//se ela prefere este solteiro ao atual parceiro dela
			if (ranking_dos_homens[mulher_candidata * n + homem_corrente] < ranking_dos_homens[mulher_candidata * n + parceiro_atual[mulher_candidata]])
			{
				//dá um pé na bunda do parceiro e casa com o solteiro
				parceiro_pe_na_bunda = parceiro_atual[mulher_candidata];
				parceiro_atual[mulher_candidata] = lista_solteiros->retira_primeiro_no();
				lista_solteiros->insere_no(parceiro_pe_na_bunda);

			}
			else
			{
				//marca que já chegou na mulher
				prox_mulher[homem_corrente]++;
			}

		}
		passos++;
	}

	printf("%d\n", passos);

	delete lista_solteiros;
	free(prox_mulher);
	free(ranking_dos_homens);
	_passos[0] = passos;
	return parceiro_atual;

}


int roda_um(char *arquivo, FILE *log, int rodada)
{
	Info_entrada *info_entrada;
	int *parceiros;
	int n;
	printf("lendo %s \n", arquivo);
	info_entrada = le_entrada(arquivo);
	n = info_entrada->n;
	printf("vai rodar %s \n", arquivo);
	auto start = std::chrono::system_clock::now();
	int passos;
	parceiros = emparelha(info_entrada, &passos);
	auto end = std::chrono::system_clock::now();
	printf("rodou %s \n", arquivo);
	std::chrono::duration<double> elapsed_seconds = end - start;
	double segundos = elapsed_seconds.count();
	fprintf(log, "%d,%f,%d", n, segundos, passos);
	/*
	for (int i = 0; i < n; i++)
	{
		fprintf(log, ",%d", parceiros[i]);
	}
	*/
	fprintf(log, "\n");

	delete info_entrada;
	free(parceiros);

	printf("%d elementos, %f segundos, %d passos\n", n, segundos, passos);
	printf("RODADA %d", rodada);

	return passos;
}



int main()
{

	FILE *arquivo = fopen("D:\\doutorado\\APA\\matches\\resultado\\rodada_best_worse_27_a_30.csv", "w");

	

	
	char arquivo_corrente[500]; 
	
	for (int i = 0; i < 3; i++)
	{

		FILE *arquivo_rodadas = fopen("c:\\APA\\arquivos_ale.csv", "r");

		int retorno = fscanf(arquivo_rodadas, "%s\n", arquivo_corrente);

		while (retorno != EOF)
		{
			roda_um(arquivo_corrente, arquivo,i);
			retorno = fscanf(arquivo_rodadas, "%s\n", arquivo_corrente);

		}
		printf("RODADA %d\n", i);

		fclose(arquivo_rodadas);

	}

	return 0;
}

