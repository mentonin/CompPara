#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <omp.h>
#include <time.h>

#include "../include/Functions.c"
#include "../include/pcg-c-basic-0.9/pcg_basic.h"

int main(int argc, char** argv)
{
    if (argc != 3) {
        printf("Sintaxe: S_Gauss fatores iteracoes");
        exit(-1);
    }

    // Seed do RNG
    pcg32_random_t rng;
    pcg32_srandom_r(
        &rng,
        time(NULL) ^ (intptr_t)&printf,
        (intptr_t)&argc);

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

    // Loop executado por um processador
    for (int i = 0; i < repeticoes; i++) {
        int soma = 0;
        for (int c = 0; c < termos; c++) {
            soma += weigthRand(&rng, distribuicao, distrSize);
        }
        gauss[soma]++;
    }

    // Imprime o vetor de frequencias em S_dados.csv
    freqPrint("S_dados.out.csv", termos * n + 1, gauss);
    return 0;
}