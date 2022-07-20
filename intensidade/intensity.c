/*-------------------------------------------------------------------------
 * Tranformação de intensidade
 * Por Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/imagelib.h"

image intensity(image In)
{
    float T[In->ml + 1];
    float expo = 2;
    image Out = img_clone(In);
    for (int i = 0; i < In->ml + 1; i++)
    {
        //--- Transformacao Potência/Raiz
        T[i] = pow(i, expo) / pow(In->ml, expo) * In->ml;
        //--- Transformacao Logaritmica
        // T[i] = log(i + 1) / log(In->ml + 1) * In->ml;
        // printf("T[%d] = %.0f\n", i, T[i]);
    }
    for (int i = 0; i < In->nr * In->nc; i++)
        Out->px[i] = T[In->px[i]];
    return Out;
}

void msg(char *s)
{
    printf("\nIntensidade");
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
    Out = intensity(In);
    //-- save image
    img_put(Out, nameOut, GRAY);

    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}
