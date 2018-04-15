#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <complex.h>
#include <omp.h>
#include <time.h>

#include "../include/Functions.c"
#include "../include/pcg-c-basic-0.9/pcg_basic.h"

void xposToX(unsigned int* pos, double* x,
    double x0, double size, unsigned int Res)
{
    *x = (((double)(*pos) / (double)Res)) * size + x0;
}

void posToXY(unsigned int* pos, double* xy,
    double x0, double y0, double size, unsigned int Res)
{
    xposToX(pos, xy, x0, size, Res);
    xposToX((pos + 1), (xy + 1), y0, size, Res);
}

void posToZ(unsigned int* pos, double complex* Z,
    double x0, double y0, double size, unsigned int Res)
{
    double xy[2];
    posToXY(pos, xy, x0, y0, size, Res);
    *Z = *xy + *(xy + 1) * I;
}

/* 
a.out <double x0> <double y0> <double size> <int res> <int zTimeout> <int totalThreads>

x0 = fronteira da esquerda (|x0| <= 2)
y0 = fronteira inferior (|y0| <= 2)
size = dimensão do intervalo
res = resolução de x e y
zTimeout = numero maximo de iteracoes antes de assumir que pertence ao conjunto
totalThreads = numero de threads do time
 */
int main(int argc, char** argv)
{
    // Recebe os argumentos da linha de comando
    if (argc != 7) {
        printf("a.out <double x0> <double y0> <double size> <int res> <int zTimeout> <int totalThreads>\n");
        exit(-1);
    }
    double x0 = atof(argv[1]);
    double y0 = atof(argv[2]);
    double size = atof(argv[3]);
    unsigned int res = atoi(argv[4]);
    unsigned int zTimeout = atoi(argv[5]);
    unsigned int totalThreads = atoi(argv[6]);
    unsigned int bound = res * res;

    // Inicializa o mapa
    unsigned char* Mapa;
    Mapa = calloc(bound, sizeof(char));

// Inicia a parte paralela do codigo
#pragma omp parallel num_threads(totalThreads)
    {
        int tnum = omp_get_thread_num();
        printf("thread #%2d started\n", tnum);
        // Estas variaveis sao privadas a cada threads
        unsigned int local, iter;
        // Testa todos os pontos distribuido entre as threads do time
#pragma omp for
        for (local = 0; local < bound; local++) {
            unsigned int posicao[2];
            double complex number;
            double complex z = 0;
            locToPos(local, res, posicao);
            posToZ(posicao, &number, x0, y0, size, res);
            for (iter = zTimeout; iter > 0; iter--) {
                z = z * z + number; // Calcula novo valor
                if (cabs(z) > 2) { // Testa a condicao de fronteira
                    // Atualiza o mapa
                    Mapa[local] = ((double)iter * 255.0 / (double)zTimeout);
                    // Sai do loop
                    break;
                }
            }
        }
        printf("#%2d ended\n", tnum);
    }

    // Registra o mapa final
    printMapa("Mandelbrot.out.pgm", Mapa, res, res);
}