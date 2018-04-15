#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <omp.h>
#include <time.h>

#include "../include/Functions.c"
#include "../include/pcg-c-basic-0.9/pcg_basic.h"

/* 
a.out <int h> <int w> <float perc> <int totalThreads>

h = altura da grid
w = largura da grid
perc = porcentual da grid que será ocupada
totalThreads = numero de threads no time
 */
int main(int argc, char** argv)
{
    // Checa os argumentos passados pela linha de comando
    if (argc < 4 || argc > 5) {
        printf("P_Fractal hPixels wPixels iteracoes totalThreads");
        exit(-1);
    }
    unsigned int hPixels = (unsigned int)atoi(argv[1]);
    unsigned int wPixels = (unsigned int)atoi(argv[2]);
    double perc = atof(argv[3]);
    unsigned int totalThreads;
    if (argc == 5) {
        totalThreads = (unsigned int)atoi(argv[4]);
    } else {
        totalThreads = omp_get_max_threads();
    }
    unsigned int bound = (hPixels * wPixels);
    unsigned int iter = bound * perc;

    // Inicializa o mapa
    unsigned char* Mapa;
    Mapa = calloc(bound, sizeof(char));
    // Sorteia uma seed inicial
    pcg32_srandom(
        time(NULL) ^ (intptr_t)&printf,
        (intptr_t)&argc);
    Mapa[pcg32_boundedrand(bound)] = 1;

// Inicia a secao paralela do codigo com um time de totalThreads threads
#pragma omp parallel num_threads(totalThreads)
    {
        // Estas variaveis sao privadas a cada thread
        int tnum = omp_get_thread_num(); // numero da thread
        unsigned int local, direcao, novolocal, i;

        // Cria e inicia a seed do RNG de cada thread
        pcg32_random_t localRNG;
        pcg32_srandom_r(&localRNG,
            time(NULL) ^ ((intptr_t)&argc + tnum),
            (intptr_t)&tnum);

// Divide o for por todas as threads do time
#pragma omp for
        for (i = 0; i < iter; i++) {
            // Sorteia um local inicial
            do {
                local = pcg32_boundedrand_r(&localRNG, bound);
            } while (Mapa[local] != 0); // Garante que é um local desocupado

            while (true) {
                // Sorteia uma direcao e calcula o novo local
                novolocal = -1; // overflow unsigned
                while (novolocal >= bound) { // garante que o novo local está dentro das fronteiras
                    direcao = pcg32_boundedrand_r(&localRNG, 4); // 0:E, 1:N, 2:W, 3:S
                    switch (direcao) {
                    case 0: // Leste
                        if ((local + 1) % wPixels != 0) {
                            novolocal = local + 1;
                        }
                        break;
                    case 1: // Norte
                        novolocal = local - wPixels;
                        break;
                    case 2: // Oeste
                        if (local % wPixels != 0) {
                            novolocal = local - 1;
                        }
                        break;
                    case 3: //Sul
                        novolocal = local + wPixels;
                        break;
                    default:
                        printf("Erro no sorteio de direcao");
                        exit(-1);
                    }
                }

                // Checa o novo local
                if (Mapa[novolocal]) { // Se ocupado, sai do loop
                    break;
                } else {
                    // Se vazio, atualiza a posicao e continua no loop
                    local = novolocal;
                }
            }
#pragma omp atomic // Evita a atualização simultanea da variavel global
            Mapa[local]--; // overflow de 0x00 para 0xFF
        }
    }
    // Grava o mapa final em um arquivo
    printMapa("P_Fractal.out.pgm", Mapa, hPixels, wPixels);
    return 0;
}