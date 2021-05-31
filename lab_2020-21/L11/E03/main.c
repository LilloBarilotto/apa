#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

#define nomefile "grafo.txt"

int main() {
    FILE *fp;
    Graph G;

    if((fp=fopen(nomefile, "r"))==NULL)
        return -1;

    G=GRAPHload(fp);
    fclose(fp);

    GRAPHdisplay(G);

    GRAPHsubgraph(G);

    GRAPHladj_from_madj(G);
    GRAPHsubgraph(G);

    GRAPHfree(G);
    return 0;
}