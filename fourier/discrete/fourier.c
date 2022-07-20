/*-------------------------------------------------------------------------
 * Tranformada de Fourier Discreta
 * Por Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../utils/imagelib.h"

double *imgDouble(int nr, int nc)
{
    return malloc(nr * nc * sizeof(double));
}

/*-----------------
 dir = -1 direta
 dir = 1 inversa
 ------------------*/
image fourier(image In, int dir)
{
    int M = In->nr;
    int N = In->nc;
    int *f = In->px;
    int mn = In->ml;
    image Out = img_clone(In);
    double max = -1E12;
    double *Re = imgDouble(M, N);
    double *Im = imgDouble(M, N);
    double *F = imgDouble(M, N);
    for (int u = 0; u < M; u++)
    {
        printf("linha = %d\n", u);
        for (int v = 0; v < N; v++)
        {
            Re[u * N + v] = Im[u * N + v] = 0.0;
            for (int x = 0; x < M; x++)
                for (int y = 0; y < N; y++)
                {
                    double theta = 2 * M_PI * ((double)u * x / M + (double)v * y / N);
                    Re[u * N + v] += (double)f[x * N + y] * cos(theta);
                    Im[u * N + v] += dir * (double)f[x * N + y] * sin(theta);
                }
            F[u * N + v] = pow(pow(Re[u * N + v], 2) + pow(Im[u * N + v], 2), .5);
            if (F[u * N + v] > max)
                max = F[u * N + v];
        }
    }

    if (dir == 1)
        for (int i = 0; i < M * N; i++)
            F[i] = 1.0 / (M * N) * F[i];

    printf("Max = %.2lf\n", max);
    for (int i = 0; i < M * N; i++)
        Out->px[i] = mn * (log(F[i] + 1) / log(max + 1));

    //--- Troca quadrante
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
        {
            // printf("Im[i = %d, j = %d] <- F[i = %d, j = %d]\n", (i + M / 2) % M, (j + N / 2) % N, i, j);
            Out->px[((i + M / 2) % M) * N + (j + N / 2) % N] = mn * (log(F[i * N + j] + 1) / log(max + 1));
        }
    free(F);
    free(Re);
    free(Im);
    return Out;
}

void msg(char *s)
{
    printf("\nFourier");
    printf("\n-------------------------------");
    printf("\nUso:  %s  nome-imagem[.pgm] \n\n", s);
    printf("    nome-imagem[.pgm] é o nome do arquivo da imagem \n");
    exit(1);
}

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    char nameIn[100], nameOut[100], cmd[110];
    image In, Out;
    if (argc < 2)
        msg(argv[0]);

    img_name(argv[1], nameIn, nameOut, GRAY, GRAY);

    //-- read image
    In = img_get(nameIn, GRAY);
    //-- transformation
    Out = fourier(In, -1);
    //-- save image
    img_put(Out, nameOut, GRAY);

    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}
