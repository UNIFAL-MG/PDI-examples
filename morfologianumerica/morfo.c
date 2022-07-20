/*-------------------------------------------------------------------------
 * Morfologia Numérica
 * Por Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/imagelib.h"

image dilata(image In)
{
    int nr = In->nr;
    int nc = In->nc;
    image Out = img_clone(In);
    for (int i = 1; i < nr - 1; i++)
        for (int j = 1; j < nc - 1; j++)
        {
            int max = -1;
            for (int y = -1; y <= 1; y++)
                for (int x = -1; x <= 1; x++)
                {
                    int lin = i + y;
                    int col = j + x;
                    int px = In->px[lin * nc + col];
                    if (px > max)
                        max = px;
                }
            Out->px[i * nc + j] = max;
        }
    return Out;
}

image erode(image In)
{
    int nr = In->nr;
    int nc = In->nc;
    image Out = img_clone(In);
    for (int i = 1; i < nr - 1; i++)
        for (int j = 1; j < nc - 1; j++)
        {
            int min = In->ml + 1;
            for (int y = -1; y <= 1; y++)
                for (int x = -1; x <= 1; x++)
                {
                    int lin = i + y;
                    int col = j + x;
                    int px = In->px[lin * nc + col];
                    if (px < min)
                        min = px;
                }
            Out->px[i * nc + j] = min;
        }
    return Out;
}

void msg(char *s)
{
    printf("\nErosao/Dilatacao");
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
    Out = erode(In);
    //-- save image
    img_put(Out, nameOut, GRAY);

    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}
