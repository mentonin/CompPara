#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <omp.h>
#include <time.h>

#include "pcg-c-basic-0.9/pcg_basic.h"

// retorna um valor de um vetor de probabilidades inteiras
int weigthRand(pcg32_random_t* rng, int* weights, int arraySize)
{
    int num = pcg32_boundedrand_r(rng, arraySize);
    return *(weights + num);
}

void freqPrint(const char* filename, int vecSize, int* vec)
{
    FILE* dados = fopen(filename, "w");
    // Imprime os dados no arquivo de arquivo
    for (int i = 0; i < vecSize; i++) {
        fprintf(dados, "%d,%d\n", i, vec[i]);
    }
    fclose(dados);
}

void printMapa(char* nomeArquivo, unsigned char* mapa, int height, int width)
{
    FILE* arquivo = fopen(nomeArquivo, "w");
    fprintf(arquivo, "P5 %d %d 255\n", width, height);

    fwrite(mapa, 1, height * width, arquivo);

    fclose(arquivo);
}