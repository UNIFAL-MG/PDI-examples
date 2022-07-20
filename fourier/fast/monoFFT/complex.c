// C program to show the working
// of complex.h library

#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    double real = 1.3, imag = 4.9;
    double complex z = CMPLX(real, imag);
    double complex w = 1.2 + 3.4 * I;
    printf("z = %.1f + %.1fi\n", creal(z), cimag(z));
    printf("w = %.1f + %.1fi\n", creal(w), cimag(w));
}