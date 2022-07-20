// FFT interativo

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>

#define DEBUG(x) x
typedef double complex cpx;

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

// Função para calcular os bits reversos de um indice
unsigned int bitReverso(unsigned int x, int log2n)
{
    int n = 0;
    for (int i = 0; i < log2n; i++)
    {
        n <<= 1;      // Equivale a n = n * 2;
        n |= (x & 1); // Equivale a (o último bit de x é 1, então soma 1 em n)
        x >>= 1;      // Equivale a x = x / 2; (próximo bit de x)
    }
    return n;
}

// Implementação do FFT baseado no esquema Butterfly
// f: função de entrada
// F: função transformada de Fourier
// n: número de valores de f
// dir: -1 = transformada direta, 1 = transformada inversa
void fft(cpx *f, cpx *F, int n, int dir)
{
    // int log2n = log(n) / log(2);
    int log2n = 0;
    for (int i = n; i != 1; log2n++, i /= 2)
        ;
    // Reordenação dos elementos, de acordo com os bits reversos
    for (unsigned int i = 0; i < n; ++i)
    {
        int rev = bitReverso(i, log2n);
        F[i] = f[rev];
        DEBUG(
            printf("F[%d] = f[%d] = %.1f + %.1fj\n", rev, i, creal(f[i]), cimag(f[i])););
    }
    DEBUG(printf("\n"););

    // J é igual a raiz de menos 1 (sqrt(-1))
    const cpx J = CMPLX(0, 1);
    for (int s = 1; s <= log2n; ++s)
    {
        int m = 1 << s;  // Equivale a 2 elevado a s
        int m2 = m >> 1; // Equivale a m2 = m / 2
        int expo = 0;
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
                DEBUG(
                    printf("F[%d] = F[%d] + F[%d]*W^%d_%d\n", k, k, k + m2, expo, 2 * m2);
                    printf("F[%d] = F[%d] - F[%d]*W^%d_%d\n", k + m2, k, k + m2, expo, 2 * m2););
            }
            DEBUG(
                printf("\nParciais:\n");
                for (int i = 0; i < n; i++)
                    printf("F[%d] = %.1f + %.1fj\n", i, creal(F[i]), cimag(F[i]));
                printf("\n"););
            w *= wm;
            expo++;
        }
    }
    if (dir == 1)
    {
        for (int i = 0; i < n; i++)
            F[i] = 1.0 / n * F[i];
    }
}

int main()
{
    int n;
    cpx *f;
    cpx *F;
    printf("n? ");
    scanf("%d", &n);
    testPowerof2(n);
    f = malloc(sizeof(cpx) * n);
    F = malloc(sizeof(cpx) * n);
    for (int i = 0; i < n; i++)
    {
        int value;
        printf("f[%d]? ", i);
        scanf("%d", &value);
        f[i] = CMPLX(value, 0);
    }
    fft(f, F, n, -1);
    puts("Transformada Direta:");
    puts("--------------------");
    for (int i = 0; i < n; ++i)
        printf("F[%d] = %.1f + %.1fj\n", i, creal(F[i]), cimag(F[i]));
    puts("");
    fft(F, f, n, 1);
    puts("Transformada Inversa");
    puts("--------------------");
    for (int i = 0; i < n; ++i)
        printf("f[%d] = %.1f\n", i, creal(f[i]));
}
