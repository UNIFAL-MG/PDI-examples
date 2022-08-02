/*-------------------------------------------------------------------------
 * Tranformação de intensidade
 * Por Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/imagelib.h"

image ponto(image In)
{
    int mask[3][3] =
        {-1, -1, -1,
         -1, 8, -1,
         -1, -1, -1};
    image Out = img_clone(In);
    for (int i = 1; i < In->nr - 1; i++)
        for (int j = 1; j < In->nc - 1; j++)
        {
            int y, x, peso, soma = 0;
            for (y = -1; y <= 1; y++)     // -1 0 1
                for (x = -1; x <= 1; x++) // -1 0 1
                {
                    int peso = mask[y + 1][x + 1];
                    soma += peso * In->px[(i + y) * In->nc + j + x];
                }
            if (abs(soma) > 1000)
                Out->px[i * In->nc + j] = 255;
            else
                Out->px[i * In->nc + j] = 0;
        }
    return Out;
}

void msg(char *s)
{
    printf("\nPontos isolados");
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
    Out = ponto(In);
    //-- save image
    img_put(Out, nameOut, GRAY);

    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}
