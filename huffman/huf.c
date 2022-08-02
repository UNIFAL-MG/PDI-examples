/*-------------------------------------------------------------------------
 * Image Processing using C-Ansi
 *   Program: Huffman tree
 * By Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAXSYMBS 256
#define MAXNODES 511

/*--------------------------------------------------------------
 * Root= 3    0    1    2    3    4    5    6
 *          -----------------------------------
 *    freq    3    8   11    1    0    0    0
 *  father    1    2   -1    0    0    0    0
 *    left   -1   -1   -1   -1    0    0    0
 *   right   -1   -1   -1   -1    0    0    0
 * 
 * Lista = 0:[1|1]=> 1:[1|2]=> 2:[1|3]=> 3:[1|-1]=> 
 *--------------------------------------------------------------*/

typedef struct
{
    int freq;
    int father;
    int left;
    int right;
} nodetype;

nodetype node[MAXNODES];

int pqmindelete(int *rootnodes)
{
    int aux;
    aux = *rootnodes;
    *rootnodes = node[*rootnodes].father;
    return aux;
}

int pqinsert(int *rootnodes, int i)
{
    int k, ant, aux;
    aux = *rootnodes;
    ant = -1;
    while (aux != -1 && node[aux].freq <= node[i].freq)
    {
        ant = aux;
        aux = node[aux].father;
    }
    if (ant == -1)
    {
        node[i].father = *rootnodes;
        *rootnodes = i;
    }
    else
    {
        node[i].father = node[ant].father;
        node[ant].father = i;
    }
}

/*--------------------------------------------------------------
 * Mostra a estrutura de nos (Arvore + lista ordenada)
 *--------------------------------------------------------------*/
void shownodes(int root, int n)
{
    int i;
    printf("\n\nRoot=%2d", root);
    for (i = 0; i < 2 * n - 1; i++)
        printf("%5d", i);
    printf("\n       ");
    for (i = 0; i < 2 * n - 1; i++)
        printf("-----");
    printf("\n%7s", "freq");
    for (i = 0; i < 2 * n - 1; i++)
        printf("%5d", node[i].freq);
    printf("\n%7s", "father");
    for (i = 0; i < 2 * n - 1; i++)
        printf("%5d", node[i].father);
    printf("\n%7s", "left");
    for (i = 0; i < 2 * n - 1; i++)
        printf("%5d", node[i].left);
    printf("\n%7s", "right");
    for (i = 0; i < 2 * n - 1; i++)
        printf("%5d", node[i].right);
    printf("\n\nLista = ");
    while (root != -1)
    {
        printf("%d:[%d|%d]=> ", root, node[root].freq, node[root].father);
        root = node[root].father;
    }
    printf("\n");
}

/*--------------------------------------------------------------
 * buildTree
 * Funcao que constroi a arvore de huffman
 * Parametros:
 *    h  - vetor de histograma (frequencia de cada pixel [0..mn])
 *    mn - maximo nivel de cinza
 * Retorna a raiz da árvore construida (2 * mn)
 *--------------------------------------------------------------*/
int buildTree(int *h, int mn)
{
    int i, k, p, p1, p2, rootnodes = -1, root, father;
    for (i = 0; i < mn + 1; i++)
    {
        node[i].freq = h[i];
        node[i].left = -1;
        node[i].right = -1;
        pqinsert(&rootnodes, i);
    }
    shownodes(rootnodes, mn + 1);
    for (p = mn + 1; p < 2 * (mn + 1) - 1; p++)
    {
        p1 = pqmindelete(&rootnodes);
        p2 = pqmindelete(&rootnodes);
        node[p1].father = p;
        node[p2].father = p;
        node[p].freq = node[p1].freq + node[p2].freq;
        node[p].left = p1;
        node[p].right = p2;
        pqinsert(&rootnodes, p);
        shownodes(rootnodes, mn + 1);
    }
    puts("\n");
    return pqmindelete(&rootnodes);
}

void reverse(char *str)
{
    int i, len = strlen(str);
    for (i = 0; i < len / 2; i++)
    {
        char tmp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = tmp;
    }
}

/*-------------------------------------------------------------
 * Huffman code
 * n = numero de simbolos (pixels)
 * p = símbolos (pixel)
 * str = string do código binário de p
 *-------------------------------------------------------------*/
void code(int n, int p, char *str)
{
    int root = 2 * n - 2;
    int i = 0;
    while (p != root)
    {
        if (p == node[node[p].father].left)
            str[i++] = '0';
        else
            str[i++] = '1';
        p = node[p].father;
    }
    str[i] = 0;
    reverse(str);
}

/*--------------------------------------------------------------
 * drawLinks, createDot
 * Rotinas que geram uma visualização da árvore de huffman
 * Para produzir uma imagem, deve-se ter graphViz instalado
 * e digitar em linha de comando:
 *    dot -Tpng tree.dot -o tree.png
 *--------------------------------------------------------------*/
void drawLinks(FILE *dot, int root)
{
    if (root != -1)
    {
        int hasLeft = node[root].left != -1;
        int hasRight = node[root].right != -1;
        if (!hasLeft && !hasRight) // folha
            // fprintf(dot, "\tn%d [shape = record, label=\"px=%d|fq=%d\", color=RED]\n", root, root, node[root].freq);
            fprintf(dot, "\tn%d [shape = record, label=\"%c|%d\", color=RED]\n", root, root + 'a', node[root].freq);
        else
            fprintf(dot, "\tn%d [shape = circle, label=\"%d\"]\n", root, node[root].freq);
        drawLinks(dot, node[root].left);
        if (hasLeft)
            fprintf(dot, "\tn%d -> n%d [label=0]\n", root, node[root].left);
        drawLinks(dot, node[root].right);
        if (hasRight)
            fprintf(dot, "\tn%d -> n%d [label=1]\n", root, node[root].right);
    }
}

void createDot(int root)
{
    int i;
    FILE *dot;
    dot = fopen("tree.dot", "wt");
    fprintf(dot, "digraph {\n");
    fprintf(dot, "\tnode [fontename=\"Arial\"];\n");
    drawLinks(dot, root);
    fprintf(dot, "}\n");
    fclose(dot);
}

void displayCalcs(int *h, int n)
{
    char str[110];
    int i, soma, freqtotal, bytes, bits;

    printf("Pixel Freq. Huf.Code\n");
    printf("--------------------\n");
    for (i = 0; i < n; i++)
    {
        code(n, i, str);
        printf("%5d %5d %s\n", i, h[i], str);
    }
    printf("\nCálculo do tamanho (em bits) da saída (usando Huf.code)\n");
    soma = 0;
    freqtotal = 0;
    for (i = 0; i < n; i++)
    {
        int len, total;
        code(n, i, str);
        len = strlen(str);
        total = len * h[i];
        printf("%3d pixel(%d) * %2d bits = %3d bits\n", h[i], i, len, total);
        soma += len * h[i];
        freqtotal += h[i];
    }
    bytes = soma / 8 + ((soma % 8) != 0);
    printf("\nTotal de bits (cod.Huf.) = %d. Aprox. %d bytes", soma, bytes);
    bits = (int)(floor(log(n) / log(2))) + 1;
    soma = freqtotal * bits;
    bytes = soma / 8 + ((soma % 8) != 0);
    printf("\nTotal de bits (original) = %d. Aprox. %d bytes\n\n", soma, bytes);
}

void huf_tree(int *h, int mn, int dot)
{
    char str[110];
    int root = buildTree(h, mn - 1);
    displayCalcs(h, mn);
    if (dot)
    {
        createDot(root);
        system("dot -Tsvg tree.dot -o tree.svg");
        sprintf(str, "%s tree.svg &", "eog");
        system(str);
    }
}

/*--------------------------------------------------------------
 * main
 *--------------------------------------------------------------*/

int main(void)
{
    int i, n, *h;

    // Le dados
    scanf("%d", &n);
    h = (int *)malloc(n * sizeof(int));
    for (i = 0; i < n; i++)
        scanf("%d", h + i);

    // Calcula a arvore de huffman
    huf_tree(h, n, 1);
    return 0;
}