#include "readData.h"
#include <fstream>
#include <iostream>

#include <vector>
#include <map>
#include <algorithm>
#include <time.h>
#include <cmath>
#include <limits>
#include <chrono>

struct Subsequence;

#define EPSILON 1e-6

void printData();

double **M; // matriz de adjacencia
int N;      // quantidade total de vertices

double calculaCustoAcumulado(std::vector<int> &s);
double calculaCustoSubsequencia(std::vector<int> &s, int i, int j);

void printSolution(std::vector<int> &s);

void reinsertion(std::vector<int> &solucao, int i, int tamanho_subsequencia, int j);
void swap(std::vector<int> &solucao, int i, int j);
void twoOpt(std::vector<int> &solucao, int i, int j);

void buscaVizinhanca2Opt(std::vector<int> &s, std::vector<std::vector<std::vector<double>>> &reOpt, double &valor_obj);
void buscaVizinhancaReinsertion(std::vector<int> &s, std::vector<std::vector<std::vector<double>>> &reOpt, double &valor_obj, int t);
void buscaVizinhancaSwap(std::vector<int> &s, std::vector<std::vector<std::vector<double>>> &reOpt, double &valor_obj);

void perturb(std::vector<int> &s);

std::vector<int> construction(double alpha);
void reOptPreProcessing(std::vector<int> &s, std::vector<std::vector<std::vector<double>>> &reOpt);
std::vector<std::vector<Subsequence>> subseq_matrix;

void RVND(std::vector<int> &solucao, std::vector<std::vector<std::vector<double>>> &reOpt, double &valor_obj);
std::vector<int> GILS_RVND();


/******** Estruturas Auxiliares MLP *********/

struct Subsequence{

	double t, c;
	int w;
	int first, last;

	inline static Subsequence Concatenate(Subsequence &sigma1, Subsequence &sigma2){

		Subsequence sigma;

		double temp = M[sigma1.last][sigma2.first];
		sigma.w = sigma1.w + sigma2.w;
		sigma.t= sigma1.t + temp + sigma2.t;
		sigma.c = sigma1.c + sigma2.w * (sigma1.t + temp) + sigma2.c;
		sigma.first = sigma1.first;
		sigma.last = sigma2.last;

		return sigma;
	}


};

void UpdateAllSubseq(std::vector<int> sequence, std::vector<std::vector<Subsequence>> &subseq_matrix){

  int n = sequence.size();

  for (int i = 0; i < n; i++){

    int v = sequence[i];

    subseq_matrix[i][i].w = (i > 0);
    subseq_matrix[i][i].c = 0;
    subseq_matrix[i][i].t = 0;
    subseq_matrix[i][i].first = sequence[i];
    subseq_matrix[i][i].last = sequence[i];

  }

  for (int i = 0; i < n; i++){
    for (int j = i + 1; j < n; j++){
      subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j-1], subseq_matrix[j][j]);
    }
  }

  for (int i = n - 1; i >= 0; i--){
    for (int j = i - 1; j >= 0; j--){
      subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j+1], subseq_matrix[j][j]);
    }
  }

}

/************* Fim Estruturas Auxiliares MLP *************/


int main(int argc, char **argv)
{
  std::vector<std::pair<std::pair<int, int>, double>> custo_insercao;

  int x = time(NULL);
  srand(x);
  std::cout << x << "\n";

  readData(argc, argv, &N, &M);

  // std::vector<int> s = {1, 36, 29, 13, 70, 31, 69, 59, 63, 66, 22, 38, 23, 16, 47, 37, 58, 50, 10, 52, 60, 12, 34, 21, 17, 43, 41, 6, 42, 18, 4, 2, 7, 32, 3, 8, 28, 14, 20, 30, 44, 68, 9, 27, 46, 40, 61, 45, 25, 39, 62, 54, 33, 67, 48, 11, 56, 65, 51, 5, 53, 57, 15, 24, 19, 26, 49, 55, 35, 64, 1};
  std::vector<int> s;
  double valor_obj;

  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  s = GILS_RVND();
  end = std::chrono::system_clock::now();

  // std::cout << s.size() << "\n";
  // printSolution(s);
  // std::cout << N << "\n";
  std::cout << calculaCustoAcumulado(s) << "\n";

  int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  std::cout << "Tempo total (s): " << elapsed_seconds / 1000.0 << "\n\n";

  // std::vector<int> s = construction(0.1);
  // double valor_obj = calculaCustoAcumulado(s);
  // std::vector<std::vector<std::vector<double>>> reOpt(3, std::vector<std::vector<double>>(N + 1, std::vector<double>(N + 1, 0)));

  // reOptPreProcessing(s, reOpt);

  // RVND(s, reOpt, valor_obj);

  return 0;
}

std::vector<int> GILS_RVND()
{
  double melhor_valor_obj = std::numeric_limits<double>::infinity(), valor_obj, valor_obj_b;
  std::vector<int> melhor_s, s, s_b, s_copia;
  double I_max = 10, I_ils;

  std::vector<double> R(26);
  for (int i = 0; i < 26; i++)
  {
    R[i] = i / 100.0;
  }

  if (N + 1 > 100)
  {
    I_ils = 100;
  }
  else
  {
    I_ils = N + 1;
  }

  std::vector<std::vector<std::vector<double>>> reOpt(3, std::vector<std::vector<double>>(N + 1, std::vector<double>(N + 1, 0)));
  std::vector<std::vector<std::vector<double>>> reOpt_b(3, std::vector<std::vector<double>>(N + 1, std::vector<double>(N + 1, 0)));

  for (int i = 0; i < I_max; i++)
  {
    double alpha = R[rand() % 26];
    s = construction(alpha);
    reOptPreProcessing(s, reOpt);

		subseq_matrix = std::vector<std::vector<Subsequence>>(s.size(), std::vector<Subsequence>(s.size()));
		UpdateAllSubseq(s, subseq_matrix);

		// for (int i = 0; i < reOpt[0].size(); i++)
		// {
		// 	for (int j = 0; j < reOpt[0][i].size(); j++)
		// 	{
		// 		if (subseq_matrix[i][j].t != reOpt[0][i][j])
		// 		{
		// 			exit(0);
		// 		}
		// 	}
		// }

    valor_obj = reOpt[1][0][N] + reOpt[0][0][N];

    s_b = s;
    valor_obj_b = valor_obj;

    int iterIls = 0;

    while (iterIls < I_ils)
    {
      RVND(s, reOpt, valor_obj);

      if (valor_obj < valor_obj_b)
      {
        s_b = s;
        valor_obj_b = valor_obj;
        iterIls = 0;
      }

      s_copia = s_b;
      perturb(s_copia);
      s = s_copia;
      reOptPreProcessing(s, reOpt);
			UpdateAllSubseq(s, subseq_matrix);

      valor_obj = reOpt[1][0][N] + reOpt[0][0][N];

      iterIls++;
    }

    if (valor_obj_b < melhor_valor_obj)
    {
      melhor_s = s_b;
      melhor_valor_obj = valor_obj_b;
    }
  }

  return melhor_s;
}

void RVND(std::vector<int> &solucao, std::vector<std::vector<std::vector<double>>> &reOpt, double &valor_obj)
{
  std::vector<int> NL = {1, 2, 3, 4, 5};
  // std::vector<int> NL = {2};
  int n;

  std::vector<int> nova_solucao = solucao;
  double novo_valor_obj = valor_obj;

  int i = 0;
  while (NL.empty() == false)
	{
		n = rand() % NL.size();
		switch (NL[n])
		{
			case 1:
				buscaVizinhancaSwap(nova_solucao, reOpt, novo_valor_obj);
				break;
			case 2:
				buscaVizinhanca2Opt(nova_solucao, reOpt, novo_valor_obj);
				break;
			case 3:
				buscaVizinhancaReinsertion(nova_solucao, reOpt, novo_valor_obj, 1);
				break;
			case 4:
				buscaVizinhancaReinsertion(nova_solucao, reOpt, novo_valor_obj, 2);
				break;
			case 5:
				buscaVizinhancaReinsertion(nova_solucao, reOpt, novo_valor_obj, 3);
				break;
		}

		if (novo_valor_obj < valor_obj)
		{
			NL = {1, 2, 3, 4, 5};
			// NL = {2};
			valor_obj = novo_valor_obj;
			solucao = nova_solucao;
		}
		else
		{
			NL.erase(NL.begin() + n);
			nova_solucao = solucao;
			novo_valor_obj = valor_obj;
		}
	}
}

void perturb(std::vector<int> &s)
{
  int n_min = 2, n_max = std::ceil(s.size() / 10.0);
  n_max = n_max >= n_min ? n_max : n_min;

  int t1 = n_min == n_max ? n_min : rand() % (n_max - n_min) + n_min;
  int i = rand() % (s.size() - 1 - t1) + 1;

  int j, t2;

  if (i < 1 + n_min)
  {
    j = rand() % ((s.size() - n_min - 1) - (i + t1) + 1) + (i + t1);
    t2 = rand() % (std::min((int)(s.size() - j - 1), n_max) - n_min + 1) + n_min;
  }
  else if (i + t1 >= s.size() - n_min)
  {
    j = rand() % (i - n_min) + 1;
    t2 = rand() % (std::min((i - j), n_max) - n_min + 1) + n_min;
  }
  else
  {
    if (rand() % 2 == 1)
    {
      j = rand() % ((s.size() - n_min - 1) - (i + t1) + 1) + (i + t1);
      t2 = rand() % (std::min((int)(s.size() - j - 1), n_max) - n_min + 1) + n_min;
    }
    else
    {
      j = rand() % (i - n_min) + 1;
      t2 = rand() % (std::min((i - j), n_max) - n_min + 1) + n_min;
    }
  }

  std::vector<int> subsequencia_i(s.begin() + i, s.begin() + i + t1);
  std::vector<int> subsequencia_j(s.begin() + j, s.begin() + j + t2);

  if (i < j)
  {
    s.erase(s.begin() + j, s.begin() + j + t2);
    s.insert(s.begin() + j, subsequencia_i.begin(), subsequencia_i.end());
    s.erase(s.begin() + i, s.begin() + i + t1);
    s.insert(s.begin() + i, subsequencia_j.begin(), subsequencia_j.end());
  }
  else
  {
    s.erase(s.begin() + i, s.begin() + i + t1);
    s.insert(s.begin() + i, subsequencia_j.begin(), subsequencia_j.end());
    s.erase(s.begin() + j, s.begin() + j + t2);
    s.insert(s.begin() + j, subsequencia_i.begin(), subsequencia_i.end());
  }
}

std::vector<int> construction(double alpha)
{
  int r = 1;
  std::vector<int> s(N + 1), CL(N - 1);
  std::vector<double> distances(N - 1);

  s[0] = s[s.size() - 1] = r;

  for (int i = 2; i <= N; i++)
  {
    CL[i - 2] = i;
  }

  int c = 1;
  while (!CL.empty())
  {
    for (int i = 0; i < CL.size(); i++)
    {
      distances[i] = M[r][CL[i]];
    }
    std::sort(distances.begin(), distances.end());

    int a = rand() % (int)std::floor(alpha * (CL.size() - 1) + 1);
    r = CL[a];
    s[c] = r;
    c++;

    CL.erase(CL.begin() + a);
    distances.erase(distances.begin());
  }

  return s;
}

void reOptPreProcessing(std::vector<int> &s, std::vector<std::vector<std::vector<double>>> &reOpt)
{
  // T = 0, C = 1, W = 2
  for (int i = 0; i < N; i++)
  {
    reOpt[0][i][i] = 0;
    reOpt[1][i][i] = 0;
    if (s[i] == 1)
    {
      reOpt[2][i][i] = 0;
    }
    else
    {
      reOpt[2][i][i] = 1;
    }
  }

  for (int t = 2; t <= N + 1; t++)
  {
    for (int i = 0, j; i < N - t + 2; i++)
    {
      j = i + t - 1;
      reOpt[2][i][j] = reOpt[2][i][j - 1] + reOpt[2][j][j];
      reOpt[0][i][j] = reOpt[0][i][j - 1] + M[s[j - 1]][s[j]];
      reOpt[1][i][j] = reOpt[1][i][j - 1] + reOpt[2][j][j] * (reOpt[0][i][j - 1] + M[s[j - 1]][s[j]]) + reOpt[1][j][j];

      reOpt[2][j][i] = reOpt[2][i][j];
      reOpt[0][j][i] = reOpt[0][i][j];

      reOpt[1][j][i] = reOpt[1][j - 1][i] + reOpt[2][j - 1][i] * (reOpt[0][j][j] + M[s[j]][s[j - 1]]) + reOpt[1][j][j];
    }
  }
}

double calculaCustoSubsequencia(std::vector<int> &s, int i, int j)
{
  double custo = 0;
  for (int u = i; u < j; u++)
  {
    custo += M[s[u]][s[u + 1]];
  }
  return custo;
}

double calculaCustoAcumulado(std::vector<int> &s)
{
  double custo = 0;
  for (int u = 0; u < N; u++)
  {
    custo += (N - u) * M[s[u]][s[u + 1]];
  }

  return custo;
}

void printSolution(std::vector<int> &s)
{
  for (int i = 0; i < s.size(); i++)
  {
    std::cout << s[i] << " ";
  }
  std::cout << "\n";
}

void twoOpt(std::vector<int> &solucao, int i, int j)
{
  std::reverse(solucao.begin() + i, solucao.begin() + j + 1);
}

void reinsertion(std::vector<int> &solucao, int i, int tamanho_subsequencia, int j)
{
  int vertex = solucao[i];

  std::vector<int> subsequencia(solucao.begin() + i, solucao.begin() + i + tamanho_subsequencia);

  solucao.erase(solucao.begin() + i, solucao.begin() + i + tamanho_subsequencia);
  solucao.insert(solucao.begin() + j, subsequencia.begin(), subsequencia.end());
}

void swap(std::vector<int> &solucao, int i, int j)
{
  std::swap(solucao[i], solucao[j]);
}

void buscaVizinhanca2Opt(std::vector<int> &s, std::vector<std::vector<std::vector<double>>> &reOpt, double &valor_obj)
{
  double T = 0, C, W, melhor_valor_obj = valor_obj, temp_obj;
  double T1, C1, W1, T2, C2, W2, T3, C3, W3;
  double fs;
  int melhor_i, melhor_j;
  bool improved = false;
	Subsequence sigma1, sigma2;

  for (int i = 1; i < s.size() - 2; i++)
  {
    for (int j = i + 1; j < s.size() - 1; j++)
    {
      W1 = reOpt[2][j][i];
      W2 = reOpt[2][j + 1][N];

      T1 = reOpt[0][0][i - 1];
      T2 = T1 + M[s[i - 1]][s[j]] + reOpt[0][j][i];

      C = reOpt[1][0][i - 1] + W1 * (T1 + M[s[i - 1]][s[j]]) + reOpt[1][j][i] + W2 * (T2 + M[s[i]][s[j + 1]]) + reOpt[1][j + 1][N];
      T = T2 + M[s[i]][s[j + 1]] + reOpt[0][j + 1][N];

      fs = C + T;

			sigma1 = Subsequence::Concatenate(subseq_matrix[0][i-1], subseq_matrix[j][i]);
			sigma2 = Subsequence::Concatenate(sigma1, subseq_matrix[j+1][N]);

			// std::cout << reOpt[2][j][i] << " " << subseq_matrix[j][i].w << "\n";

      // twoOpt(s, i, j);

      // if (fs != calculaCustoAcumulado(s))
      // {
      //   std::cout << i << " " << j << "A" << "\n";
      //   std::cout << fs << "\n";
      //   std::cout << calculaCustoAcumulado(s) << "\n\n";
      // }
      // else
      // {
      //   std::cout << i << " " << j << "\n";
      //   std::cout << fs << "\n";
      //   std::cout << calculaCustoAcumulado(s) << "\n\n";
      // }

      // twoOpt(s, i, j);

      // if (fs + EPSILON < melhor_valor_obj)
      if (sigma2.c + EPSILON < melhor_valor_obj)
      {
        improved = true;
        // melhor_valor_obj = fs;
        melhor_valor_obj = sigma2.c;
        melhor_i = i;
        melhor_j = j;
      }
    }
  }

  if (improved == true)
  {
    valor_obj = melhor_valor_obj;
    twoOpt(s, melhor_i, melhor_j);

		UpdateAllSubseq(s, subseq_matrix);

    for (int j = melhor_i; j <= melhor_j; j++)
    {
      for (int i = j - 1; i >= 0; i--)
      {
        reOpt[2][i][j] = reOpt[2][i][j - 1] + reOpt[2][j][j];
        reOpt[0][i][j] = reOpt[0][i][j - 1] + M[s[j - 1]][s[j]];
        reOpt[1][i][j] = reOpt[1][i][j - 1] + reOpt[2][j][j] * (reOpt[0][i][j - 1] + M[s[j - 1]][s[j]]) + reOpt[1][j][j];

        reOpt[2][j][i] = reOpt[2][i][j];
        reOpt[0][j][i] = reOpt[0][i][j];

        reOpt[1][j][i] = reOpt[1][j - 1][i] + reOpt[2][j - 1][i] * (reOpt[0][j][j] + M[s[j]][s[j - 1]]) + reOpt[1][j][j];
      }
    }

    for (int i = melhor_i; i <= melhor_j; i++)
    {
      for (int j = melhor_j + 1; j <= N; j++)
      {
        reOpt[2][i][j] = reOpt[2][i][j - 1] + reOpt[2][j][j];
        reOpt[0][i][j] = reOpt[0][i][j - 1] + M[s[j - 1]][s[j]];
        reOpt[1][i][j] = reOpt[1][i][j - 1] + reOpt[2][j][j] * (reOpt[0][i][j - 1] + M[s[j - 1]][s[j]]) + reOpt[1][j][j];

        reOpt[2][j][i] = reOpt[2][i][j];
        reOpt[0][j][i] = reOpt[0][i][j];

        reOpt[1][j][i] = reOpt[1][j - 1][i] + reOpt[2][j - 1][i] * (reOpt[0][j][j] + M[s[j]][s[j - 1]]) + reOpt[1][j][j];
      }
    }

    for (int i = melhor_i - 1, k = melhor_i, l; i >= 0; i--)
    {
      for (int j = melhor_j + 1; j <= N; j++)
      {

        reOpt[2][i][j] = reOpt[2][i][k] + reOpt[2][k + 1][j];
        reOpt[0][i][j] = reOpt[0][i][k] + M[s[k]][s[k + 1]] + reOpt[0][k + 1][j];
        reOpt[1][i][j] = reOpt[1][i][k] + reOpt[2][k + 1][j] * (reOpt[0][i][k] + M[s[k]][s[k + 1]]) + reOpt[1][k + 1][j];

        reOpt[2][j][i] = reOpt[2][i][j];
        reOpt[0][j][i] = reOpt[0][i][j];

        reOpt[1][j][i] = reOpt[1][k][i] + reOpt[2][k][i] * (reOpt[0][j][k + 1] + M[s[k + 1]][s[k]]) + reOpt[1][j][k + 1];
      }
    }

  }
}

void buscaVizinhancaReinsertion(std::vector<int> &s, std::vector<std::vector<std::vector<double>>> &reOpt, double &valor_obj, int t)
{
  double T = 0, C, W, melhor_valor_obj = valor_obj, temp_obj;
  double T1, C1, W1, T2, C2, W2, T3, C3, W3;
  double fs;
  int melhor_i, melhor_j;
  bool improved = false;
  int i1, i2, j1, j2;

  std::vector<int> COPIA;

	Subsequence sigma1, sigma2;
  for (int i = 1; i < N + 1 - t; i++)
  {
    for (int j = 1; j < N + 1 - t; j++)
    {
      if (i == j)
        continue;
      if (i < j)
      {
        i1 = 0, j1 = i - 1;
        i2 = i + t, j2 = j + t - 1;

        W1 = reOpt[2][i1][j1] + reOpt[2][i2][j2];
        T1 = reOpt[0][i1][j1] + M[s[j1]][s[i2]] + reOpt[0][i2][j2];
        C1 = reOpt[1][i1][j1] + reOpt[2][i2][j2] * (reOpt[0][i1][j1] + M[s[j1]][s[i2]]) + reOpt[1][i2][j2];

        i1 = i, j1 = i + t - 1;
        i2 = j + t, j2 = N;

        W2 = reOpt[2][i1][j1] + reOpt[2][i2][j2];
        T2 = reOpt[0][i1][j1] + M[s[j1]][s[i2]] + reOpt[0][i2][j2];
        C2 = reOpt[1][i1][j1] + reOpt[2][i2][j2] * (reOpt[0][i1][j1] + M[s[j1]][s[i2]]) + reOpt[1][i2][j2];

        C = C1 + W2 * (T1 + M[s[j + t - 1]][s[i]]) + C2;
        T = T1 + M[s[j + t - 1]][s[i]] + T2;

        // COPIA = s;
        // reinsertion(COPIA, i, t, j);
        // if (T != calculaCustoSubsequencia(COPIA, 0, N))
        // {
        //   std::cout << T << "\n";
        //   std::cout << calculaCustoSubsequencia(COPIA, 0, N) << "\n\n";
        // }
        // else
        // {
        //   std::cout << T << "\n";
        //   std::cout << calculaCustoSubsequencia(COPIA, 0, N) << "\n\n";
        // }

        fs = C + T;
      }
      else
      {
        i1 = 0, j1 = j - 1;
        i2 = i, j2 = i + t - 1;

        W1 = reOpt[2][i1][j1] + reOpt[2][i2][j2];
        T1 = reOpt[0][i1][j1] + M[s[j1]][s[i2]] + reOpt[0][i2][j2];
        C1 = reOpt[1][i1][j1] + reOpt[2][i2][j2] * (reOpt[0][i1][j1] + M[s[j1]][s[i2]]) + reOpt[1][i2][j2];

        i1 = j, j1 = i - 1;
        i2 = i + t, j2 = N;

        W2 = reOpt[2][i1][j1] + reOpt[2][i2][j2];
        T2 = reOpt[0][i1][j1] + M[s[j1]][s[i2]] + reOpt[0][i2][j2];
        C2 = reOpt[1][i1][j1] + reOpt[2][i2][j2] * (reOpt[0][i1][j1] + M[s[j1]][s[i2]]) + reOpt[1][i2][j2];

        C = C1 + W2 * (T1 + M[s[i + t - 1]][s[j]]) + C2;
        T = T1 + M[s[i + t - 1]][s[j]] + T2;

        // COPIA = s;
        // reinsertion(COPIA, i, t, j);
        // if (T != calculaCustoSubsequencia(COPIA, 0, N))
        // {
        //   std::cout << T << "\n";
        //   std::cout << calculaCustoSubsequencia(COPIA, 0, N) << "\n\n";
        // }
        // else
        // {
        //   std::cout << T << "\n";
        //   std::cout << calculaCustoSubsequencia(COPIA, 0, N) << "\n\n";
        // }

        fs = C + T;
      }
      if (fs + EPSILON < melhor_valor_obj)
      {
        improved = true;
        melhor_valor_obj = fs;
        melhor_i = i;
        melhor_j = j;
      }
    }
  }
  if (improved == true)
  {
    valor_obj = melhor_valor_obj;

    int i1, i2, i3, i4;
    int j1, j2, j3, j4;

    if (melhor_i < melhor_j)
    {
      i1 = 0, j1 = melhor_i - 1;
      i2 = melhor_i + t, j2 = melhor_j + t - 1;
      i3 = melhor_i, j3 = melhor_i + t - 1;
      i4 = melhor_j + t, j4 = N;
    }
    else
    {
      i1 = 0, j1 = melhor_j - 1;
      i2 = melhor_i, j2 = melhor_i + t - 1;
      i3 = melhor_j, j3 = melhor_i - 1;
      i4 = melhor_i + t, j4 = N;
    }

    reinsertion(s, melhor_i, t, melhor_j);

    for (int j = j1 + 1; j <= i4 - 1; j++)
    {
      for (int i = j - 1; i >= 0; i--)
      {
        reOpt[2][i][j] = reOpt[2][i][j - 1] + reOpt[2][j][j];
        reOpt[0][i][j] = reOpt[0][i][j - 1] + M[s[j - 1]][s[j]];
        reOpt[1][i][j] = reOpt[1][i][j - 1] + reOpt[2][j][j] * (reOpt[0][i][j - 1] + M[s[j - 1]][s[j]]) + reOpt[1][j][j];

        reOpt[2][j][i] = reOpt[2][i][j];
        reOpt[0][j][i] = reOpt[0][i][j];

        reOpt[1][j][i] = reOpt[1][j - 1][i] + reOpt[2][j - 1][i] * (reOpt[0][j][j] + M[s[j]][s[j - 1]]) + reOpt[1][j][j];
      }
    }

    for (int i = j1 + 1; i <= i4 - 1; i++)
    {
      for (int j = i4 - 1 + 1; j <= N; j++)
      {
        reOpt[2][i][j] = reOpt[2][i][j - 1] + reOpt[2][j][j];
        reOpt[0][i][j] = reOpt[0][i][j - 1] + M[s[j - 1]][s[j]];
        reOpt[1][i][j] = reOpt[1][i][j - 1] + reOpt[2][j][j] * (reOpt[0][i][j - 1] + M[s[j - 1]][s[j]]) + reOpt[1][j][j];

        reOpt[2][j][i] = reOpt[2][i][j];
        reOpt[0][j][i] = reOpt[0][i][j];

        reOpt[1][j][i] = reOpt[1][j - 1][i] + reOpt[2][j - 1][i] * (reOpt[0][j][j] + M[s[j]][s[j - 1]]) + reOpt[1][j][j];
      }
    }

    for (int i = j1 + 1 - 1, k = j1 + 1, l; i >= 0; i--)
    {
      for (int j = i4 - 1 + 1; j <= N; j++)
      {

        reOpt[2][i][j] = reOpt[2][i][k] + reOpt[2][k + 1][j];
        reOpt[0][i][j] = reOpt[0][i][k] + M[s[k]][s[k + 1]] + reOpt[0][k + 1][j];
        reOpt[1][i][j] = reOpt[1][i][k] + reOpt[2][k + 1][j] * (reOpt[0][i][k] + M[s[k]][s[k + 1]]) + reOpt[1][k + 1][j];

        reOpt[2][j][i] = reOpt[2][i][j];
        reOpt[0][j][i] = reOpt[0][i][j];

        reOpt[1][j][i] = reOpt[1][k][i] + reOpt[2][k][i] * (reOpt[0][j][k + 1] + M[s[k + 1]][s[k]]) + reOpt[1][j][k + 1];
      }
    }

		UpdateAllSubseq(s, subseq_matrix);
  }
}

void buscaVizinhancaSwap(std::vector<int> &s, std::vector<std::vector<std::vector<double>>> &reOpt, double &valor_obj)
{
  double T = 0, C, W, melhor_valor_obj = valor_obj, temp_obj;
  double T1, C1, W1, T2, C2, W2, T3, C3, W3;
  double fs;
  int melhor_i, melhor_j;
  bool improved = false;

	Subsequence sigma1, sigma2, sigma3, sigma4;

  for (int i = 1; i < s.size() - 2; i++)
  {
    for (int j = i + 2; j < s.size() - 1; j++)
    {
      W1 = reOpt[2][0][i - 1] + reOpt[2][j][j];
      W3 = reOpt[2][i][i] + reOpt[2][j + 1][N];

      T1 = reOpt[0][0][i - 1] + M[s[i - 1]][s[j]] + reOpt[0][j][j];
      T3 = reOpt[0][i][i] + M[s[i]][s[j + 1]] + reOpt[0][j + 1][N];

      C1 = reOpt[1][0][i - 1] + reOpt[2][j][j] * (reOpt[0][0][i - 1] + M[s[i - 1]][s[j]]) + reOpt[1][j][j];
      C3 = reOpt[1][i][i] + reOpt[2][j + 1][N] * (reOpt[0][i][i] + M[s[i]][s[j + 1]]) + reOpt[1][j + 1][N];

      W2 = reOpt[2][i + 1][j - 1];
      T2 = reOpt[0][i + 1][j - 1];
      C2 = reOpt[1][i + 1][j - 1];

      C = (C1 + W2 * (T1 + M[s[j]][s[i + 1]]) + C2) + W3 * (T1 + M[s[j]][s[i + 1]] + T2 + M[s[j - 1]][s[i]]) + C3;
      T = T1 + M[s[j]][s[i + 1]] + T2 + M[s[j - 1]][s[i]] + T3;

      fs = C + T;

			sigma1 = Subsequence::Concatenate(subseq_matrix[0][i-1], subseq_matrix[j][j]);
			sigma2 = Subsequence::Concatenate(sigma1, subseq_matrix[i+1][j-1]);
			sigma3 = Subsequence::Concatenate(sigma2, subseq_matrix[i][i]);
			sigma4 = Subsequence::Concatenate(sigma3, subseq_matrix[j+1][N]);

      // swap(s, i, j);
      // if (fs != calculaCustoAcumulado(s))
      // {
      // std::cout << "A\n";
      // }
      // else
      // {
      //   std::cout << "E\n";
      // }
      // swap(s, i, j);

      // if (fs + EPSILON < melhor_valor_obj)
      if (sigma4.c + EPSILON < melhor_valor_obj)
      {
        improved = true;
        // melhor_valor_obj = fs;
        melhor_valor_obj = sigma4.c;
        melhor_i = i;
        melhor_j = j;
      }
    }
  }

  if (improved == true)
  {
    valor_obj = melhor_valor_obj;
    swap(s, melhor_i, melhor_j);

    for (int j = melhor_i; j <= melhor_j; j++)
    {
      for (int i = j - 1; i >= 0; i--)
      {
        reOpt[2][i][j] = reOpt[2][i][j - 1] + reOpt[2][j][j];
        reOpt[0][i][j] = reOpt[0][i][j - 1] + M[s[j - 1]][s[j]];
        reOpt[1][i][j] = reOpt[1][i][j - 1] + reOpt[2][j][j] * (reOpt[0][i][j - 1] + M[s[j - 1]][s[j]]) + reOpt[1][j][j];

        reOpt[2][j][i] = reOpt[2][i][j];
        reOpt[0][j][i] = reOpt[0][i][j];

        reOpt[1][j][i] = reOpt[1][j - 1][i] + reOpt[2][j - 1][i] * (reOpt[0][j][j] + M[s[j]][s[j - 1]]) + reOpt[1][j][j];
      }
    }

    for (int i = melhor_i; i <= melhor_j; i++)
    {
      for (int j = melhor_j + 1; j <= N; j++)
      {
        reOpt[2][i][j] = reOpt[2][i][j - 1] + reOpt[2][j][j];
        reOpt[0][i][j] = reOpt[0][i][j - 1] + M[s[j - 1]][s[j]];
        reOpt[1][i][j] = reOpt[1][i][j - 1] + reOpt[2][j][j] * (reOpt[0][i][j - 1] + M[s[j - 1]][s[j]]) + reOpt[1][j][j];

        reOpt[2][j][i] = reOpt[2][i][j];
        reOpt[0][j][i] = reOpt[0][i][j];

        reOpt[1][j][i] = reOpt[1][j - 1][i] + reOpt[2][j - 1][i] * (reOpt[0][j][j] + M[s[j]][s[j - 1]]) + reOpt[1][j][j];
      }
    }

    for (int i = melhor_i - 1, k = melhor_i, l; i >= 0; i--)
    {
      for (int j = melhor_j + 1; j <= N; j++)
      {

        reOpt[2][i][j] = reOpt[2][i][k] + reOpt[2][k + 1][j];
        reOpt[0][i][j] = reOpt[0][i][k] + M[s[k]][s[k + 1]] + reOpt[0][k + 1][j];
        reOpt[1][i][j] = reOpt[1][i][k] + reOpt[2][k + 1][j] * (reOpt[0][i][k] + M[s[k]][s[k + 1]]) + reOpt[1][k + 1][j];

        reOpt[2][j][i] = reOpt[2][i][j];
        reOpt[0][j][i] = reOpt[0][i][j];

        reOpt[1][j][i] = reOpt[1][k][i] + reOpt[2][k][i] * (reOpt[0][j][k + 1] + M[s[k + 1]][s[k]]) + reOpt[1][j][k + 1];
      }
    }
    // reOptPreProcessing(s, reOpt);

		UpdateAllSubseq(s, subseq_matrix);
  }

  // std::cout << calculaCustoAcumulado(s) << " " << reOpt[1][0][N] + reOpt[0][0][N] << "\n";
}

void printData()
{
  std::cout << "dimension: " << N << std::endl;
  for (size_t i = 1; i <= N; i++)
  {
    for (size_t j = 1; j <= N; j++)
    {
      std::cout << M[i][j] << " ";
    }
    std::cout << std::endl;
  }
}
