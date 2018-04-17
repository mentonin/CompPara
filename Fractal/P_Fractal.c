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
    // Checa os argumentos passados pela linha de comando
    if (argc < 4 || argc > 5) {
        printf("P_Fractal hPixels wPixels iteracoes totalThreads");
        exit(-1);
    }
    unsigned int hPixels = (unsigned int)atoi(argv[1]);
    unsigned int wPixels = (unsigned int)atoi(argv[2]);
    unsigned int iter = (unsigned int)atoi(argv[3]);
    unsigned int totalThreads;
    if (argc == 5) {
        totalThreads = (unsigned int)atoi(argv[4]);
    } else {
        totalThreads = omp_get_max_threads();
    }
    unsigned int bound = (hPixels * wPixels);

    // Inicializa o mapa
    unsigned char* Mapa;
    Mapa = calloc(bound, sizeof(char));
    // Sorteia uma seed inicial
    pcg32_srandom(
        time(NULL) ^ (intptr_t)&printf,
        (intptr_t)&argc);
    // Mapa[pcg32_boundedrand(bound)] = 1;
    Mapa[bound/2] = 255;

    // Imprime o mapa inicial
    // printMapa(Mapa, hPixels, wPixels);

// Inicia o loop da fractal dividido
#pragma omp parallel num_threads(totalThreads)
    {
        int tnum = omp_get_thread_num();
        printf("Thread #%d started\n", tnum);
        clock_t begin = clock();
        unsigned int local, direcao, novolocal, encontro, i;
        pcg32_random_t localRNG;
        pcg32_srandom_r(&localRNG,
            time(NULL) ^ ((intptr_t)&argc + tnum),
            (intptr_t)&tnum);

#pragma omp for
        for (i = 0; i < iter; i++) {
            // Sorteia um local inicial
            do {
                local = pcg32_boundedrand_r(&localRNG, bound);
            } while (Mapa[local] != 0); // Garante que é um local desocupado

            encontro = 0;
            while (!encontro) {
                // Sorteia uma direcao e calcula o novo local
                novolocal = -1;
                while (novolocal >= bound) {
                    direcao = pcg32_boundedrand_r(&localRNG, 4); // 0:E, 1:N, 2:W, 3:S
                    switch (direcao) {
                    case 0:
                        if ((local + 1) % wPixels != 0) {
                            novolocal = local + 1;
                        }
                        break;
                    case 1:
                        novolocal = local - wPixels;
                        break;
                    case 2:
                        if (local % wPixels != 0) {
                            novolocal = local - 1;
                        }
                        break;
                    case 3:
                        novolocal = local + wPixels;
                        break;
                    default:
                        printf("Erro no sorteio de direcao");
                        exit(-1);
                    }
                }

                // Checa o novo local
                if (Mapa[novolocal]) {
                    // Se ocupada, sai do loop
                    break;
                } else {
                    // Se vazia, muda a posicao e volta para o sorteio de direcao}
                    local = novolocal;
                }
            }
#pragma omp atomic
            Mapa[local]--; // overflow de 0x00 para 0xFF
        }
        clock_t end = clock();
        printf("Thread #%d ended (%lf s)\n", tnum, (double)(end - begin) / CLOCKS_PER_SEC);
    }
    free(Mapa);
    printMapa("P_Fractal.out.pgm", Mapa, hPixels, wPixels);
    return 0;
}