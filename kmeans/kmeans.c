/*------------------------------------------------
 * K-means em Processamento de Imagens
 * Para segmentação e redução da quantização
 *
 * Por Luiz Eduardo da Silva
 *------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/imagelib.h"

/*------------------------------------------------
 * Coordenadas dos pixels de cada grupo (cluster)
 *------------------------------------------------*/
typedef struct no *ptno;
typedef struct no
{
    int i, j;
    ptno prox;
} no;

/*------------------------------------------------
 * Insere as coordenadas em cada grupo
 *------------------------------------------------*/
void insere(ptno *grupo, int i, int j)
{
    ptno new = (ptno)malloc(sizeof(no));
    new->i = i;
    new->j = j;
    new->prox = *grupo;
    *grupo = new;
}

/*------------------------------------------------
 * Mostrar os grupos (para depuração)
 *------------------------------------------------*/
void mostraGrupos(ptno *grupos, int n)
{
    int i;
    ptno p;
    for (i = 0; i < n; i++)
    {
        printf("%2d: ", i);
        p = grupos[i];
        while (p)
        {
            printf("(%d,%d) ", p->i, p->j);
            p = p->prox;
        }
        printf("\n");
    }
}

/*------------------------------------------------
 * Destroi os grupos para reconstrui-los
 * a cada iteracao
 *------------------------------------------------*/
void destroiGrupos(ptno *grupos, int n)
{
    int i;
    ptno p, q;
    for (i = 0; i < n; i++)
    {
        p = grupos[i];
        while (p)
        {
            q = p;
            p = p->prox;
            free(q);
        }
    }
}

/*------------------------------------------------
 * Inicia o centro de cada cluster do kmeans
 * As intensidades dos grupos é dividida
 * em n partes da escala de cinza (255)
 *------------------------------------------------*/
void iniciaCentros(int *centros, int n, int mn)
{
    int i;
    for (i = 0; i < n; i++)
        centros[i] = (int)((i + 1) * mn / n);
}

/*------------------------------------------------
 * Mostra os centros dos cluster a cada iteração
 *------------------------------------------------*/
void mostraCentros(int *centros, int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("centro[%2d] = %d\n", i, centros[i]);
    printf(".\n");
}

/*------------------------------------------------
 * Calcula o centro mais próximo de cada pixel
 *  (com diferenca de intensidade menor)
 *------------------------------------------------*/
int centroProximo(int pixel, int *centros, int n)
{
    int i, menor = 256, centro = 0;
    for (i = 0; i < n; i++)
    {
        int dif = abs(pixel - centros[i]);
        if (dif < menor)
        {
            menor = dif;
            centro = i;
        }
    }
    return centro;
}

/*------------------------------------------------
 * Reposiciona os centros de cada cluster
 *------------------------------------------------*/
void calculaCentros(image In, ptno *grupos, int *centros, int n)
{
    int i;
    ptno p;
    for (i = 0; i < n; i++)
    {
        if (grupos[i])
        {
            int soma = 0, conta = 0;
            p = grupos[i];
            while (p)
            {
                soma += In->px[p->i * In->nc + p->j];
                conta++;
                p = p->prox;
            }
            centros[i] = (int)(soma / conta);
        }
        else
            centros[i] = 0;
    }
}

/*------------------------------------------------
 * Verifica se os centros foram modificados
 *------------------------------------------------*/
int iguais(int *centros, int *novos, int n)
{
    int i;
    for (i = 0; i < n; i++)
        if (centros[i] != novos[i])
            return 0;
    return 1;
}

/*-------------------------------------------------------
 * Algoritmo Kmeans:
 * 1) Escolha um número fixo de grupos (clusters)
 * 2) Escolha os centros dos grupos
 * 3) Associe cada um dos pixels ao centro mais próximo
 * 4) Quando todos os pixels foram assinalados, recalcule
 *    a posição dos centros
 * 5) Repita os passos 3) e 4) até que os centros não
 *    se movam mais.
 *-------------------------------------------------------*/

/*-------------------------------------------------------
 * Implementacao do K-means para processamento de imagens
 * Parâmetros:
 *   - In: imagem de entrada
 *   - Out: imagem de saída
 *   - nl: número de linhas
 *   - nc: número de colunas
 *   - mn: máximo tom de cinza
 *   - n: número de grupos
 *-------------------------------------------------------*/
image kmeans(image In, int n)
{
    int i, j, terminou = 0;
    int centros[n];      // tons de cinza "centrais" de cada grupo
    int novosCentros[n]; // novos tons calculados para cada centro
    ptno grupos[n];      // vetor de listas de pixels
    image Out = img_clone(In);

    iniciaCentros(centros, n, In->ml);
    mostraCentros(centros, n);
    while (!terminou)
    {
        for (i = 0; i < n; i++)
            grupos[i] = NULL;
        for (i = 0; i < In->nr; i++)
            for (j = 0; j < In->nc; j++)
            {
                int centro = centroProximo(In->px[i * In->nc + j], centros, n);
                insere(grupos + centro, i, j);
            }
        // mostraGrupos(grupos, n);
        calculaCentros(In, grupos, novosCentros, n);
        if (!iguais(centros, novosCentros, n))
        {
            for (i = 0; i < n; i++)
                centros[i] = novosCentros[i];
            mostraCentros(centros, n);
            destroiGrupos(grupos, n);
        }
        else
            terminou = 1;
    }
    for (i = 0; i < n; i++)
    {
        ptno p = grupos[i];
        while (p)
        {
            Out->px[p->i * Out->nc + p->j] = centros[i];
            p = p->prox;
        }
    }
    destroiGrupos(grupos, n);
    return Out;
}

void msg(char *s)
{
    printf("\nKmeans");
    printf("\n-------------------------------");
    printf("\nUso:  %s  nome-imagem[.pgm] [número-clusters]\n\n", s);
    printf("    nome-imagem[.pgm] é o nome do arquivo da imagem \n");
    printf("    [numero-cluster] o valor default é 3 \n");
    exit(1);
}

/*------------------------------------------------
 * Main
 *------------------------------------------------*/
int main(int argc, char *argv[])
{

    char nameIn[100], nameOut[100], cmd[110];
    image In, Out;
    int n = 3;
    if (argc < 2)
        msg(argv[0]);
    if (argc == 3)
        n = atoi(argv[2]);

    img_name(argv[1], nameIn, nameOut, GRAY, GRAY);

    //-- read image
    In = img_get(nameIn, GRAY);
    //-- transformation
    Out = kmeans(In, n);
    //-- save image
    img_put(Out, nameOut, GRAY);

    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}