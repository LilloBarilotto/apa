//
// Created by super on 18/01/2021.
//

#ifndef L13_GRAPH_H
#define L13_GRAPH_H
#include <stdio.h>
#include "ST.h"


typedef struct graph *Graph;
typedef struct edge Edge;

Graph GRAPHinit(int V);
void GRAPHfree(Graph G);
Graph GRAPHload(FILE * fin);
void GRAPHstore(Graph G, FILE *fout);
void    GRAPHdisplay(Graph G);
int GRAPHgetIndex(Graph G, char *label);
void GRAPHinsertE(Graph G, int id1, int id2, int wt);
void GRAPHremoveE(Graph G, int id1, int id2);
void GRAPHedges(Graph G, Edge *a);
int GRAPHpath(Graph G, int id1, int id2);
void GRAPHpathH(Graph G, int id1, int id2);
void GRAPHbfs(Graph G, int id);
int GRAPHdfs(Graph G, int id);
int GRAPHcc(Graph G);
int GRAPHscc(Graph G);

void   GRAPHfind_subset_edge(Graph G);
void   DAGssmp(Graph D);

#endif //L13_GRAPH_H
