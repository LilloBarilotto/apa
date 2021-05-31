//
// Created by super on 18/01/2021.
//
#include "graph.h"
#include <stdlib.h>
#include <stdio.h>

#define MAXC 30
#define NO_PATH -1

struct edge {
    int v, w;
    int wt;
};
struct graph {
    int V, E;
    ST tab;
    int **madj;
};

void reverse(int * A, int n){
    int l, h, temp;
    for ( l = 0, h = n - 1; l < h; l++, h--){
        temp = A[l];
        A[l] = A[h];
        A[h] = temp;
    }
}


static int ** MATRIXint(int r, int c, int val){
    int i, j;
    int ** mat= malloc(r * sizeof(int *));

    for(i=0; i<r; i++)
        mat[i]=malloc(c * sizeof(int));

    for(i=0; i<r; i++)
        for(j=0; j<c; j++)
            mat[i][j]=val;

    return mat;
}
static void insertE(Graph G, Edge e) {
    int v = e.v, w = e.w, wt = e.wt;

    G->madj[v][w] = wt;
    G->E++;
}
static Edge EDGEcreate(int v, int w, int wt) {
    Edge e;
    e.v = v; e.w = w; e.wt = wt;
    return e;
}
static int EDGEsum_wt(Edge * Edges, int E){
    int sum=0;
    for(int i=0; i<E; i++)
        sum+=Edges[i].wt;
    return sum;
}

Graph   GRAPHinit(int V){
    Graph G= malloc( sizeof(*G) );

    G->V= V;
    G->E= 0;

    G->tab=  STinit(V);
    G->madj= MATRIXint(V, V, -1);

    return G;
}
Graph   GRAPHload(FILE * fin){
    int id1, id2;
    char id_1[MAXC];
    char id_2[MAXC];
    int peso_arco, N, i ;
    Graph G;

    fscanf(fin, "%d", &N);

    G= GRAPHinit(N);

    for(i=0; i< N; i++){
        fscanf(fin,"%s", id_1);
        STinsert(G->tab, id_1);
    }

    while(fscanf(fin,"%s %s %d", id_1, id_2, &peso_arco)!=EOF){
        id1 = STsearch(G->tab,id_1);
        id2 = STsearch(G->tab,id_2);

        if(id1 >=0 && id2 >=0)
            GRAPHinsertE(G, id1, id2, peso_arco);
    }
    return G;
}
void GRAPHinsertE(Graph G, int id1, int id2, int wt) {
    insertE(G, EDGEcreate(id1, id2, wt));
}
void    GRAPHdisplay(Graph G){
    int i, j, count;

    for(i=0; i<G->V; i++){
        printf("%d) %s\n", i+1, STsearchByIndex(G->tab, i));

        for(count=0, j=0; j<G->V; j++)
            if(G->madj[i][j]>=0){
                printf("\t--->%s\t, Peso %d\n", STsearchByIndex(G->tab, j), G->madj[i][j]);
                count++;
            }

        if(count==0)
            printf("Nessun arco trovato\n\n");
        else
            printf("\n\n");
    }
}
void    GRAPHfree(Graph G){
    int i;

    //FREE MADj
    for(i=0; i<G->V; i++)
        free(G->madj[i]);

    STfree(G->tab);
    free(G);
}

void dfsR(Graph G, Edge e, int *time,
          int *pre, int *post, int *st, int *p_check){
    int  w = e.w, i;

    st[e.w] = e.v;
    pre[w] = (*time)++;

    for (i=0; i<G->V && *p_check==0; i++)
        if(G->madj[w][i]>=0) {
            if (pre[i] == -1)
                dfsR(G, EDGEcreate(w,i, G->madj[w][i]) , time, pre, post, st, p_check);
            else if (post[i] == -1)
                    *p_check=1;//HO TROVATO UN Back NON VA BENE
        }
    post[w] = (*time)++;
}
int GRAPHdfs(Graph G, int id) {
    int v, time=0, *pre, *post, *st, check=0;

    pre= malloc(G->V * sizeof(int));
    post=malloc(G->V * sizeof(int));
    st = malloc(G->V * sizeof(int));

    for (v=0;v<G->V;v++) {
        pre[v]=-1;
        post[v]=-1;
        st[v]=-1;
    }

    dfsR(G, EDGEcreate(id,id, G->madj[id][id]), &time, pre, post, st, &check);
    for (v=0; v < G->V && check==0; v++)
        if (pre[v]==-1)
            dfsR(G,EDGEcreate(v,v, G->madj[v][v]),&time,pre,post,st, &check);

    return check;
}
int powerset_r(Edge * val, int k, Edge * sol, int j, int pos, int start, Edge * b_sol, int *p_sommabest, Graph G){
    int count=0, i, somma_attuale;

    if(pos==j){
        if(GRAPHdfs(G, 0)==0){
            printf("\nTROVATO SET PROBLEMATICO\n");

            for(i=0; i<j; i++)
                    printf("\t%s--->%s\t, Peso %d\n",STsearchByIndex(G->tab, sol[i].v), STsearchByIndex(G->tab, sol[i].w), sol[i].wt);

            somma_attuale=EDGEsum_wt(sol, j);
            if(*p_sommabest<somma_attuale){
                for(i=0; i<j; i++)
                    b_sol[i]=sol[i];
                *p_sommabest=somma_attuale;
            }
            return 1;
        }
        return 0;
    }

    for(i=start; i<k; i++){
        sol[pos]=val[i];
        G->madj[sol[pos].v][sol[pos].w]= -G->madj[sol[pos].v][sol[pos].w];

        count += powerset_r(val, k, sol, j, pos+1, i+1, b_sol, p_sommabest, G);

        G->madj[sol[pos].v][sol[pos].w]= -G->madj[sol[pos].v][sol[pos].w];
    }
    return count;
}
void GRAPHfind_subset_edge(Graph G){
    int j,  i, count, countarchi;
    Edge * archi;
    Edge * archi_problematici;
    Edge * b_archi_probl;
    int b_somma_archi=-1;


    if(GRAPHdfs(G,0) == 0) {
        printf("Il grafo è gia un DAG");
        return;
    }

    archi=malloc(G->E * sizeof(Edge));
    countarchi=0;

    //COSTRUZIONE DI TUTTI GLI ARCHI
    for(i=0; i<G->V && countarchi<G->E; i++)
        for(j=0; j< G->V && countarchi<G->E; j++)
            if(G->madj[i][j]>=0) {
                archi[countarchi] = EDGEcreate(i, j, G->madj[i][j]);
                countarchi++;
            }

    for(j=1; j< G->E-1; j++){
        archi_problematici=malloc(j * sizeof(Edge));
        b_archi_probl=malloc(j * sizeof(Edge));

        count=powerset_r(archi, G->E, archi_problematici,j, 0, 0, b_archi_probl, &b_somma_archi, G);

        free(archi_problematici);
        if(count==0)
            free(b_archi_probl);
        else {
            printf("HO TROVATO %d insiemi problematici, ELIMINO L'INSIEME:\n", count);
            for(i=0; i<j; i++)
                printf("\t%s--->%s\t, Peso %d\n",STsearchByIndex(G->tab, b_archi_probl[i].v), STsearchByIndex(G->tab, b_archi_probl[i].w), b_archi_probl[i].wt);
            break;
        }
    }
    free(archi);

    for(i=0; i<j; i++)
        G->madj[b_archi_probl[i].v][b_archi_probl[i].w]=-1;

    free(b_archi_probl);
}


void TSdfsR(Graph D, int v, int *ts, int *pre,  int *time){
    int w;

    pre[v]=0;
    for (w=0; w<D->V; w++)
        if(D->madj[w][v]>=0) {
            if (pre[w] == -1)
                TSdfsR(D, w, ts, pre, time);
        }

    ts[(*time)++]=v;
}
int * DAGrts(Graph D){
    int v, time = 0;
    int *pre, *ts;

    pre=malloc(D->V * sizeof(int));
    ts= malloc(D->V * sizeof(int));

    for(v=0; v<D->V; v++)
        pre[v]=ts[v]=-1;

    for(v=0; v< D->V; v++)
        if(pre[v]==-1)
            TSdfsR(D, v, ts, pre, &time);
    free(pre);

    return ts;
}

void DAGssmp(Graph D) {//Single short maximum path
    int * ts, * wt;
    int i,j,k;

    ts=DAGrts(D);
    reverse(ts, D->V); //cosi da avere l'ordinamento topologico

    wt=malloc(D->V * sizeof(int));


    printf("ORDINE TOPOLOGICO:\t");
    for(i=0; i<D->V; i++)
        printf("%d ", ts[i]);
    printf("\n");

    printf("MATRICE ADIACENZA:\n");
    for(i=0; i<D->V; i++, printf("\n"))
        for(j=0; j<D->V; j++)
            printf("%d ", D->madj[i][j]);


    for(k=0; k<D->V; k++) {

        for (j = 0; j< D->V; j++){
            wt[j]=NO_PATH;
        }
        wt[ts[k]]=0;

        for(i=k; i>=0; i--)
            if( wt[ts[i]]!=NO_PATH)
                for (j = 0; j < D->V; j++)
                    if (D->madj[ts[i]][j] >= 0)
                        if ( wt[j]==NO_PATH || wt[j] < wt[ts[i]] + D->madj[ts[i]][j])
                            wt[j] = wt[ts[i]] + D->madj[ts[i]][j];

        wt[ts[k]]=NO_PATH;

        printf("\n\nMAX DISTANCE from node:\t%s\n", STsearchByIndex(D->tab, ts[k]));
        for(i=0; i<D->V; i++)
            if(wt[i]>=0)
                printf("\t--->%s\t  = %d\n", STsearchByIndex(D->tab, i), wt[i]);
    }

    free(wt);
    free(ts);
}