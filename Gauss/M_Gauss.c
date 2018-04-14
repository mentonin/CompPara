#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <omp.h>
#include <time.h>

#include "Functions.c"
#include "pcg_basic.h"

int main(int argc, char** argv)
{
    if (argc != 3) {
        printf("Sintaxe: S_Gauss fatores iteracoes");
        exit(-1);
    }

    // Vetor de probabilidades
    int distribuicao[] = { 1, 1, 1, 2, 2, 3, 3, 4, 5, 5, 5, 6, 7, 7, 7, 8, 9 };
    // Numero de elementos do vetor de probabilidades
    int distrSize = (int)sizeof(distribuicao) / sizeof(distribuicao[0]);
    // Maior elemento do vetor de probabilidades (assumindo crescente)
    int n = distribuicao[distrSize - 1];

    // Numero de termos da distribuicao a serem somados
    int termos = atoi(argv[1]);
    // Numero de repeticoes do loop
    int repeticoes = atoi(argv[2]);
    // Vetor dos elementos de soma, variando de 0 a termos*n
    int* gauss = (int*)calloc(termos * n + 1, sizeof(int));

// Loop executado por todos os processadores
#pragma omp parallel
    {
        // Seed do RNG individual para cada thread
        pcg32_random_t thread_rng;
        int tnum = omp_get_thread_num() + 1;
        pcg32_srandom_r(
            &thread_rng,
            time(NULL) ^ ((intptr_t)&printf + tnum),
            (intptr_t)&argc);

        for (int i = 0; i < repeticoes; i++) {
            int soma = 0;
            for (int c = 0; c < termos; c++) {
                soma += weigthRand(&thread_rng, distribuicao, distrSize);
            }
#pragma omp atomic
            gauss[soma]++;
        }
    }

    // Imprime o vetor de frequencias em M_dados.csv
    freqPrint("M_dados.csv", termos * n + 1, gauss);
    return 0;
}