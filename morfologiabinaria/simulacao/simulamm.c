/*
 * SIMULADOR da operacoes de dilatacao e erosao binarias.
 * Implementado a partir da definicao.
 * Por Luiz Eduardo da Silva
 *
 */

#include <stdio.h>
#include <stdlib.h>

#define DEBUG(X)

#define IMGW 16
#define IMGH 16

int in[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

typedef struct
{
    int di, dj;
} pto;

/*
 * Elemento Estruturante:
 * . x .
 * x x x
 * . x .
 */
#define N1 5
pto B1[N1] = {
    {-1, 0},
    {0, -1},
    {0, 0},
    {0, 1},
    {1, 0}};

/*
 * Elemento Estruturante:
 * x x x
 * x . .
 * x . .
 */
#define N2 5
pto B2[N2] = {
    {-1, -1},
    {-1, 0},
    {-1, 1},
    {0, -1},
    {1, -1}};

/*
 * Elemento Estruturante:
 * . . .
 * x x x
 * . . .
 */
#define N3 3
pto B3[N3] = {
    {0, -1},
    {0, 0},
    {0, 1}};

/*
 * Elemento Estruturante:
 * x x x x x
 * x . . . x
 * x . x . x
 * x . . . x
 * x x x x x
 */
#define N4 17
pto B4[N4] = {
    {-2, -2},
    {-2, -1},
    {-2, 0},
    {-2, 1},
    {-2, 2},
    {-1, -2},
    {-1, 2},
    {0, -2},
    {0, 0},
    {0, 2},
    {1, -2},
    {1, 2},
    {2, -2},
    {2, -1},
    {2, 0},
    {2, 1},
    {2, 2}};

int estaContido(int *in, int i, int j, pto *B, int N)
{
    int k;
    for (k = 0; k < N; k++)
    {
        int lin = i + B[k].di;
        int col = j + B[k].dj;
        if (lin >= 0 && lin < IMGH && col >= 0 && col < IMGW)
        {
            int px = in[lin * IMGW + col];
            DEBUG(printf("px(%d,%d) = %d\n", lin, col, px);)
            if (!px)
                return 0;
        }
        else
            return 0;
    }
    return 1;
}

int intercepta(int *in, int i, int j, pto *B, int N)
{
    int k;
    for (k = 0; k < N; k++)
    {
        int lin = i - B[k].di;
        int col = j - B[k].dj;
        if (lin >= 0 && lin < IMGH && col >= 0 && col < IMGW)
        {
            int px = in[lin * IMGW + col];
            DEBUG(printf("px(%d,%d) = %d\n", lin, col, px);)
            if (px)
                return 1;
        }
        else
            return 0;
    }
    return 0;
}

void erosao(int *in, int *out, pto *B, int N)
{
    int i, j;
    for (i = 1; i < IMGH - 1; i++)
        for (j = 1; j < IMGW - 1; j++)
            if (estaContido(in, i, j, B, N))
                out[i * IMGW + j] = 1;
            else
                out[i * IMGW + j] = 0;
}

void dilatacao(int *in, int *out, pto *B, int N)
{
    int i, j;
    for (i = 1; i < IMGH - 1; i++)
        for (j = 1; j < IMGW - 1; j++)
            if (intercepta(in, i, j, B, N))
                out[i * IMGW + j] = 1;
            else
                out[i * IMGW + j] = 0;
}

void display(int *in, int *out)
{
    int i, j;
    for (i = 0; i < IMGH; i++)
    {
        for (j = 0; j < IMGW; j++)
        {
            int pos = i * IMGW + j;
            if (in[pos] && out[pos])
                printf("*");
            if (in[pos] && !out[pos])
                printf("-");
            if (!in[pos] && out[pos])
                printf("+");
            if (!in[pos] && !out[pos])
                printf(".");
        }
        printf("\n");
    }
    printf("\n");
}

int out1[IMGW * IMGH];
int out2[IMGW * IMGH];

int main(void)
{
    pto *B = B3;
    int N = N3;
    printf("Imagem\n");
    display(in, in);

    printf("Erosao\n");
    erosao(in, out1, B, N);
    display(in, out1);

    printf("Dilatacao\n");
    dilatacao(in, out1, B, N);
    display(in, out1);

    printf("Abertura\n");
    erosao(in, out1, B, N);
    dilatacao(out1, out2, B, N);
    display(in, out2);

    printf("Fechamento\n");
    dilatacao(in, out1, B, N);
    erosao(out1, out2, B, N);
    display(in, out2);

    return (0);
}
