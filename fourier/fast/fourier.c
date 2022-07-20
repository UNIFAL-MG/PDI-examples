/*-------------------------------------------------------------------------
 * Fast Fourier Transform
 * Por Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <math.h>
#include "../../utils/imagelib.h"

typedef double complex cpx;

cpx *vetComplex(int n)
{
    return malloc(n * sizeof(cpx));
}

unsigned int bitReverso(unsigned int x, int log2n)
{
    int n = 0;
    for (int i = 0; i < log2n; i++)
    {
        n <<= 1;
        n |= (x & 1);
        x >>= 1;
    }
    return n;
}

void fft(cpx *f, cpx *F, int n, int dir)
{
    // int log2n = log(n) / log(2);
    int log2n = 0;
    for (int i = n; i != 1; log2n++, i /= 2)
        ;
    for (unsigned int i = 0; i < n; ++i)
    {
        int rev = bitReverso(i, log2n);
        F[i] = f[rev];
    }

    // J é igual a raiz de menos 1 (sqrt(-1))
    const cpx J = CMPLX(0, 1);
    for (int s = 1; s <= log2n; ++s)
    {
        int m = 1 << s;  // Equivale a 2 elevado a s
        int m2 = m >> 1; // Equivale a m2 = m / 2
        cpx w = CMPLX(1, 0);

        // Cálculo do fator Wm
        cpx wm = cexp(dir * J * (M_PI / m2));
        for (int j = 0; j < m2; ++j)
        {
            for (int k = j; k < n; k += m)
            {
                // t = próximo termo
                cpx u = F[k];
                cpx t = w * F[k + m2];
                F[k] = u + t;
                F[k + m2] = u - t;
            }
            w *= wm;
        }
    }
    if (dir == 1)
        for (int i = 0; i < n; i++)
            F[i] = 1.0 / n * F[i];
}

int testPowerof2(int n)
{
    uint m, m2;
    for (m = n, m2 = 1; m != 1; m >>= 1, m2 <<= 1)
        ;
    if (m2 != n)
    {
        printf("A dimensão %d não é potência de 2!\n", n);
        exit(10);
    }
}

void fft2d(cpx *Img, int nl, int nc, int dir)
{
    /* Transforma as linhas */
    cpx *f = vetComplex(nl);
    cpx *F = vetComplex(nl);
    testPowerof2(nl);
    for (int j = 0; j < nc; j++)
    {
        for (int i = 0; i < nl; i++)
            f[i] = Img[i * nc + j];
        fft(f, F, nl, dir);
        for (int i = 0; i < nl; i++)
            Img[i * nc + j] = F[i];
    }
    free(f);
    free(F);

    /* Transforma as colunas */
    testPowerof2(nc);
    f = vetComplex(nc);
    F = vetComplex(nc);
    for (int i = 0; i < nl; i++)
    {
        for (int j = 0; j < nc; j++)
            f[j] = Img[i * nc + j];
        fft(f, F, nc, dir);
        for (int j = 0; j < nl; j++)
            Img[i * nc + j] = F[j];
    }
    free(f);
    free(F);
}

void trocaQuadrante(cpx *Img, int nl, int nc)
{
    cpx *Swap = vetComplex(nl * nc);
    for (int i = 0; i < nl * nc; i++)
        Swap[i] = Img[i];

    for (int i = 0; i < nl; i++)
        for (int j = 0; j < nc; j++)
            Img[((i + nl / 2) % nl) * nc + (j + nc / 2) % nc] = Swap[i * nc + j];
    free(Swap);
}

void filter(char *imgFilter, cpx *Img, int nr, int nc)
{
    image fil = img_get(imgFilter, GRAY);
    if (fil->nr != nr || fil->nc != nc)
    {
        printf("ERRO: dimensões do filtro\n");
        exit(10);
    }
    for (int i = 0; i < nr * nc; i++)
        if (fil->px[i] == 0)
            Img[i] = 0;
    img_free(fil);
}

image fourier(image In, char *imgFilter)
{
    int nr = In->nr, nc = In->nc, mn = In->ml;
    int hasFilter = imgFilter[0];

    image Out = img_clone(In);

    cpx *Img = vetComplex(nr * nc);
    for (int i = 0; i < nr * nc; i++)
        Img[i] = CMPLX(In->px[i], 0);

    fft2d(Img, nr, nc, -1);
    trocaQuadrante(Img, nr, nc);

    if (hasFilter)
    {
        filter(imgFilter, Img, nr, nc);
        fft2d(Img, nr, nc, 1);
    }

    double max = -1.E12;
    for (int i = 0; i < nr * nc; i++)
    {
        // cabs = magnitude do número complexo
        double magnitude = cabs(Img[i]);
        if (magnitude > max)
            max = magnitude;
    }

    if (hasFilter) // A saída é a imagem filtrada
        for (int i = 0; i < nr * nc; i++)
            Out->px[i] = (cabs(Img[i]) / max) * mn;
    else // A saída é o espectro com transf. log para melhorar visualização
        for (int i = 0; i < nr * nc; i++)
            Out->px[i] = mn * (log(cabs(Img[i]) + 1) / log(max + 1));

    free(Img);
    return Out;
}

void msg(char *s)
{
    printf("\nFast Fourier");
    printf("\n-------------------------------");
    printf("\nUso:  %s  nome-imagem[.pgm] [filtro.pgm]\n", s);
    printf("    nome-imagem[.pgm] é o nome do arquivo da imagem \n");
    printf("    filtro.pgm - nome do arquivo da imagem de filtragem\n");
    printf("                 Se não especificado, apresenta o Espectro\n\n");
    exit(1);
}

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    char nameIn[100], nameOut[100], cmd[110], filName[100] = "";
    image In, Out;
    if (argc < 2)
        msg(argv[0]);
    if (argc == 3)
        strcpy(filName, argv[2]);

    img_name(argv[1], nameIn, nameOut, GRAY, GRAY);

    //-- read image
    In = img_get(nameIn, GRAY);
    //-- transformation
    Out = fourier(In, filName);
    //-- save image
    img_put(Out, nameOut, GRAY);

    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}
