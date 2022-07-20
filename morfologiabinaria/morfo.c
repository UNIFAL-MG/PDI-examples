/*-------------------------------------------------------------------------
 * Morfologia Binária
 * Por Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/imagelib.h"

image dilata(image In)
{
    int ee[3][2] =
        {
            //{-1, 0},
            {0, -1},
            {0, 0},
            {0, 1}
            //{1, 0}
        };
    int nr = In->nr;
    int nc = In->nc;
    image Out = img_clone(In);
    for (int i = 0; i < nr; i++)
        for (int j = 0; j < nc; j++)
        {
            int intercepta = 0;
            for (int k = 0; k < 3; k++)
            {
                int lin = i + ee[k][0];
                int col = j + ee[k][1];
                if (lin >= 0 && lin < nr && col >= 0 && col < nc)
                {
                    if (In->px[lin * nc + col])
                        intercepta = 1;
                }
            }
            Out->px[i * nc + j] = intercepta;
        }
    return Out;
}

void msg(char *s)
{
    printf("\nErosao/Dilatacao");
    printf("\n-------------------------------");
    printf("\nUso:  %s  nome-imagem[.pbm] \n\n", s);
    printf("    nome-imagem[.pbm] é o nome do arquivo da imagem \n");
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

    img_name(argv[1], nameIn, nameOut, BW, BW);

    //-- read image
    In = img_get(nameIn, BW);
    //-- transformation
    Out = dilata(In);
    for (int i = 0; i < In->nr * In->nc; i++)
        Out->px[i] = Out->px[i] - In->px[i];

    //-- save image
    img_put(Out, nameOut, BW);

    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}
