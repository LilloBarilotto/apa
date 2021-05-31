#include <stdio.h>
#include "graph.h"

#define nomefile "grafo4.txt"

int main() {
    FILE *fp;
    Graph G;

    if((fp=fopen(nomefile, "r"))==NULL) {
        printf("File non trovato");
        return -1;
    }

    G=GRAPHload(fp);
    fclose(fp);

    GRAPHdisplay(G);

    GRAPHfind_subset_edge(G);

    printf("\n\nECCO IL DAG:\n");
    GRAPHdisplay(G);


    DAGssmp(G);

    GRAPHfree(G);
    return 0;
}
