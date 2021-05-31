#ifndef INC_3_GRAPH_H
#define INC_3_GRAPH_H
#include <stdio.h>
#include <string.h>

typedef struct edge { int v; int w; int wt; } Edge;
typedef struct graph * Graph;

Graph   GRAPHinit(int V);
void    GRAPHfree(Graph G);
Graph   GRAPHload(FILE * fin);
void    GRAPHdisplay(Graph G);
void    GRAPHstore();
void    GRAPHgetIndex();
void    GRAPHremoveE();
void    GRAPHedges();
void    GRAPHpath();
void    GRAPHpathH();
void    GRAPHbfs();
void    GRAPHdfs();

void    GRAPHsubgraph(Graph G);
void    GRAPHladj_from_madj(Graph G);


#endif //INC_3_GRAPH_H
