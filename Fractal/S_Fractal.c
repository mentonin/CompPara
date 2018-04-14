#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <omp.h>
#include <time.h>

#include "..\include\Functions.c"
#include "..\include\pcg_basic.h"

int main(int argc, char** argv)
{
    // Checa os argumentos passados pela linha de comando
    if (argc != 4) {
        printf("S_Fractal hPixels wPixels iteracoes");
        exit(-1);
    }
    int hPixels = atoi(argv[1]);
    int wPixels = atoi(argv[2]);
    int iter = atoi(argv[3]);
    int bound = (hPixels * wPixels);

    // Inicializa o mapa
    unsigned char* Mapa;
    Mapa = calloc(hPixels * wPixels, sizeof(char));
    // Sorteia uma seed inicial
    pcg32_srandom(
        time(NULL) ^ (intptr_t)&printf,
        (intptr_t)&argc);
    Mapa[pcg32_boundedrand(bound)] = 1;

    // Imprime o mapa inicial
    // printMapa(Mapa, hPixels, wPixels);

    // Inicia o loop da fractal
    int local, direcao, novolocal, encontro;
    for (int i = 0; i < iter; i++) {

        // Sorteia um local inicial
        do {
            local = pcg32_boundedrand(bound);
        } while (Mapa[local] != 0); // Garante que é um local desocupado

        encontro = 0;
        while (!encontro) {
            // Sorteia uma direcao e calcula o novo local
            novolocal = -1;
            while (novolocal < 0 || novolocal > bound - 1) {
                direcao = pcg32_boundedrand(4); // 0:E, 1:N, 2:W, 3:S
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
                    if (local + wPixels < bound) {
                        novolocal = local + wPixels;
                    }
                    break;
                default:
                    printf("Erro no sorteio de direcao");
                    exit(-1);
                }
            }

            // Checa o novo local
            if (Mapa[novolocal]) {
                // Se ocupada, muda o estado da posicao e volta para o local inicial
                Mapa[local]--;
                encontro++;
            } else {
                // Se vazia, muda a posicao e volta para o sorteio de direcao}
                local = novolocal;
            }
        }
    }
    printMapa("S_Fractal.pgm", Mapa, hPixels, wPixels);
    return 0;
}