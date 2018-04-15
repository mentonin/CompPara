#include <assert.h>
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

void printMapa(char* nomeArquivo, unsigned char* mapa, unsigned int height, unsigned int width)
{
    FILE* arquivo = fopen(nomeArquivo, "w");
    fprintf(arquivo, "P5 %d %d 255\n", width, height);
    for (unsigned int h = height; h > 0; h--) {
        for (unsigned int w = 0; w < width; w++) {
            fwrite((mapa + (h - 1) * width + w), sizeof(char), 1, arquivo);
        }
        // fprintf(arquivo, "\n");
    }
    fclose(arquivo);
}

void locToPos(unsigned int loc, unsigned int w, unsigned int* pos)
{
    pos[0] = loc % w;
    pos[1] = loc / w;
}
void posToLoc(unsigned int* pos, unsigned int w, unsigned int* loc)
{
    if (pos[0] >= w) {
        printf("ERR: fora da fronteira");
        exit(-1);
    }
    *loc = pos[0] + w * pos[1];
}