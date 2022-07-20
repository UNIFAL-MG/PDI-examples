/*-------------------------------------------------------------------------
 * Tranformação de intensidade
 * Por Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "imagelib.h"

int find(int parent[], int i)
{
    while (parent[i] != i)
        i = parent[i];
    return i;
}

void Union(int parent[], int i, int j)
{
    int x = find(parent, i);
    int y = find(parent, j);
    parent[y] = x;
}

void label(image In)
{
    int nr = In->nr;
    int nc = In->nc;
    int *px = In->px;
    int numLabel = 0;
    int parent[1000];
    for (int i = 0; i < 1000; i++)
        parent[i] = i;
    for (int i = 1; i < nr; i++)
        for (int j = 1; j < nc; j++)
        {
            int p = px[i * nc + j];
            int r = px[(i - 1) * nc + j];
            int t = px[i * nc + j - 1];
            if (p != 0)
            {
                if (r == 0 && t == 0)
                    px[i * nc + j] = ++numLabel;
                if (r != 0 && t == 0)
                    px[i * nc + j] = r;
                if (r == 0 && t != 0)
                    px[i * nc + j] = t;
                if (r != 0 && t != 0 && t == r)
                    px[i * nc + j] = r;
                if (r != 0 && t != 0 && t != r)
                {
                    px[i * nc + j] = t;
                    Union(parent, r, t);
                }
            }
        }
    for (int i = 0; i < nr * nc; i++)
        In->px[i] = find(parent, In->px[i]);
    In->ml = numLabel;
}

void msg(char *s)
{
    printf("\nLabelling");
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
    image In;
    if (argc < 2)
        msg(argv[0]);

    img_name(argv[1], nameIn, nameOut, BW, GRAY);

    //-- read image
    In = img_get(nameIn, BW);
    //-- transformation
    label(In);
    //-- save image
    img_put(In, nameOut, GRAY);

    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    return 0;
}
