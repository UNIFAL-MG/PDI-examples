#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef int *image;

int in[] =
    {0, 0, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 1, 0,
     0, 1, 0, 0, 1, 0, 0,
     0, 0, 1, 0, 1, 0, 0,
     0, 1, 0, 0, 1, 0, 0,
     0, 1, 1, 1, 1, 0, 0,
     0, 0, 0, 0, 0, 0, 0};

typedef struct
{
    int i, j;
} point;

typedef struct no *ptno;

typedef struct no
{
    point p;
    ptno prox;
} no;

ptno insere(ptno L, point p)
{
    ptno q = (ptno)malloc(sizeof(no));
    q->p = p;
    q->prox = L;
    return q;
}

void mostraContorno(ptno L)
{
    printf("Contorno = [");
    while (L)
    {
        printf("(%d,%d)", L->p.i, L->p.j);
        L = L->prox;
        if (L)
            printf(",");
    }
    printf("]\n");
}

ptno contorno(image img, int h, int w)
{
    point b0, b1, b;
    ptno L = NULL;
    /*    -1   0   1
     * -1: N1  N2  N3
     *  0: N0  x   N4
     *  1: N7  N6  N5 */
    point N[8] =
        {0, -1, -1, -1, -1, 0, -1, 1, 0, 1, 1, 1, 1, 0, 1, -1};
    /* ... b.. cb. .cb ..c ... ... ...
     * bx. cx. .x. .x. .xb .xc .x. .x.
     * c.. ... ... ... ... ..b .bc bc.
     *
     * b=0 b=1 b=2 b=3 b=4 b=5 b=6 b=7
     * c=6 c=6 c=0 c=0 c=2 c=2 c=4 c=4 (em relação a b)*/
    int anterior[8] = {6, 6, 0, 0, 2, 2, 4, 4};
    int i, j, k;
    bool fim;
    for (i = 0; i < h * w && !img[i]; i++)
        ;
    if (img[i])
    {
        b0.i = i / w;
        b0.j = i % w;
    }
    else
        return NULL;
    L = insere(L, b0);
    k = 0;
    do
    {
        i = b0.i + N[k].i;
        j = b0.j + N[k].j;
        k++;
    } while (img[i * w + j] != 1 && k < 8);
    if (img[i * w + j] == 1)
    {
        b1.i = i;
        b1.j = j;
        k = anterior[k];
        fim = false;
        do
        {
            b.i = i;
            b.j = j;
            do
            {
                k = (k + 1) % 8;
                i = b.i + N[k].i;
                j = b.j + N[k].j;
            } while (img[i * w + j] != 1);
            k = anterior[k];
            L = insere(L, b);
            if (i == b0.i && j == b0.j)
            {
                int i1, j1, k1;
                k1 = k;
                do
                {
                    k1 = (k1 + 1) % 8;
                    i1 = b0.i + N[k1].i;
                    j1 = b0.j + N[k1].j;
                } while (img[i1 * w + j1] != 1);
                if (i1 == b1.i && j1 == b1.j)
                    fim = true;
            }
        } while (!fim);
    }
    return L;
}

int main(int argc, char *argv[])
{
    ptno L = contorno(in, 7, 7);
    mostraContorno(L);
}
