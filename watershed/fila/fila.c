#include <stdio.h>
#include <stdlib.h>

/*------------------------------------------------
 * Fila de prioridade
 *------------------------------------------------*/
typedef struct no *ptno;
typedef struct no
{
    int i, j;
    ptno next;
} no;

/*-----------------------------------------------
 *  init   *Q      new     init         *Q
 *  [a:]->[b:]-+   [c:]  ->[a:]->[b:]->[c:]-+
 *   ^---------+             ^--------------+
 *----------------------------------------------*/
void insQ(ptno *Q, int i, int j)
{
    ptno new = malloc(sizeof(no));
    new->i = i;
    new->j = j;
    if (!(*Q))
        new->next = new;
    else
    {
        new->next = (*Q)->next;
        (*Q)->next = new;
    }
    *Q = new;
}

/*-----------------------------------------------
 *  init         *Q                    *Q
 *  [a:]->[b:]->[c:]-+    [a:]  [b:]->[c:]-+
 *    ^--------------+            ^--------+
 *----------------------------------------------*/
void remQ(ptno *Q, int *i, int *j)
{
    ptno init = (*Q)->next;
    *i = init->i;
    *j = init->j;
    if (*Q == init)
        *Q = NULL;
    else
        (*Q)->next = init->next;
    free(init);
}

int isEmpty(ptno *Q)
{
    return *Q == NULL;
}

void initQPrior(ptno *QPrior, int mn)
{
    int i;
    for (i = 0; i < mn; i++)
        QPrior[i] = NULL;
}

void insert(ptno *QPrior, int i, int j, int p)
{
    insQ(QPrior + p, i, j); // &Qprior[p]
}

int pop(ptno *QPrior, int *i, int *j, int *maxPrior, int mn)
{
    while (*maxPrior < mn && isEmpty(QPrior + *maxPrior))
        (*maxPrior)++;
    if (*maxPrior == mn)
        return 1;
    remQ(QPrior + *maxPrior, i, j);
    return 0;
}

void show(ptno *Q, int m)
{
    int i;
    ptno p;
    for (i = 0; i < m; i++)
    {
        printf("Fila[%d] = {", i);
        p = (Q[i]) ? Q[i]->next : NULL;
        while (p && p != Q[i])
        {
            printf("(%d,%d) ", p->i, p->j);
            p = p->next;
        }
        if (p)
            printf("(%d,%d)}\n", p->i, p->j);
        else
            printf("}\n");
    }
}

int main(int argc, char const *argv[])
{
    ptno Filas[5];
    int m;
    initQPrior(Filas, 5);
    insert(Filas, 1, 4, 2);
    insert(Filas, 4, 5, 1);
    insert(Filas, 2, 2, 1);
    insert(Filas, 10, 4, 3);
    insert(Filas, 5, 7, 0);
    show(Filas, 5);
    return 0;
}
