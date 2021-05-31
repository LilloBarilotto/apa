#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "ST.h"

#define MAXC 30

 typedef struct node * link;

 struct node{
    int v;
    int wt;
    link next;
};

 struct graph {
    int V;
    int E;
    ST tab;
    int ** madj;
    link * ladj;
    link z;
};

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
static Edge EDGEcreate(int v, int w, int wt){
    Edge e;

    e.v=v;
    e.w=w;
    e.wt=wt;

    return e;
}
static void insertE(Graph G, Edge e){
    int v=e.v, w=e.w, wt=e.wt;

    if(G->madj[v][w] ==0) { //non posso sovrascrivere i valori di strade già decise
        G->E++;

        G->madj[v][w] = wt;
        G->madj[w][v] = wt;
    }
}

static link NEW(int v, int wt, link next){
    link x = malloc(sizeof(*x));

    x->v= v;
    x->wt=wt;
    x->next=next;

    return x;
}
static void ladjinit(Graph G){
    G->z= NEW(-1, -1, NULL);

    G->ladj= malloc( G->V * sizeof(link));
    for(int i=0; i<G->V; i++)
        G->ladj[i]=G->z;
}

static int     GRAPHsubgraph_madj(Graph G, int id1, int id2, int id3){
    return (G->madj[id1][id2]!=0 && G->madj[id1][id3]!=0 && G->madj[id2][id3]!=0);
}
static int     GRAPHsubgraph_ladj(Graph G, int id1, int id2, int id3){
    link x;
    int count;

    for(count=0, x= G->ladj[id1]; x != G->z; x= x->next)
        if(x->v==id2 || x->v==id3)
            count++;

    if(count!=2)//basta !=2 perchè non è multigrafo
        return 0;

    for(x= G->ladj[id2]; x != G->z; x= x->next)
        if(x->v==id3)
            return 1;

    return 0;
}
static void    GRAPHinsertE(Graph G, int id1, int id2, int wt){
    insertE(G, EDGEcreate(id1, id2, wt));
}

Graph   GRAPHinit(int V){
    Graph G= malloc( sizeof(*G) );

    G->V= V;
    G->E= 0;

    G->madj= MATRIXint(V, V, 0);
    G->tab=  STinit(V);
    G->ladj= NULL;

    return G;
}
Graph   GRAPHload(FILE * fin){
    int id1, id2;
    Graph G;
    char id_elab1[MAXC];
    char id_rete1[MAXC];
    char id_elab2[MAXC];
    char id_rete2[MAXC];
    int flusso;
    ST tab;

    tab=STinit(2);

    while(fscanf(fin,"%s %s %s %s %*d", id_elab1, id_rete1, id_elab2, id_rete2)!=EOF) {
        STinsert(tab, id_elab1);
        STinsert(tab, id_elab2);
    } //order alfabetico nella STinsert

    G= GRAPHinit(STcount(tab));  // V esattamente il valore nella tabella creata PRIMA di Graph
    rewind(fin);

    STfree(G->tab);
    G->tab=tab;

    while(fscanf(fin,"%s %s %s %s %d", id_elab1, id_rete1, id_elab2, id_rete2, &flusso)!=EOF){
        id1 = STsearch(G->tab,id_elab1);
        id2 = STsearch(G->tab,id_elab2);

        if(id1 >=0 && id2 >=0 && id1!=id2) //non possono esistere cappi
            GRAPHinsertE(G, id1, id2, flusso);
    }

    return G;
}
void    GRAPHfree(Graph G){
    int i;
    link t, next;

    //FREE LADJ se creata
    if(G->ladj!=NULL){
        for(i=0; i<G->V; i++)
            for(t=G->ladj[i]; t != G->z; t= next) {
                next = t->next;
                free(t);
            }
        free(G->z);
    }
    //FREE MADJ
    for(i=0; i<G->V; i++)
        free(G->madj[i]);

    free(G->madj);
    STfree(G->tab);
    free(G);
}
void    GRAPHdisplay(Graph G){
    int i, j, count;

    for(i=0; i<G->V; i++){
        printf("%d) %s\n", i+1, STsearchByIndex(G->tab, i));

        for(count=0, j=0; j<G->V; j++)
            if(G->madj[i][j]!=0){
                printf("\t<--->%s\t, Peso %d\n", STsearchByIndex(G->tab, j), G->madj[i][j]);
                count++;
            }

        if(count==0)
            printf("Nessun arco trovato\n\n");
        else
            printf("\n\n");
    }
}

void    GRAPHsubgraph(Graph G){
    int id[3];
    int response, i, result;
    char nome[MAXC];

    if(G->V<3){
        printf("\n\nImpossibile vedere sottografo di 3, non ci sono almeno 3 nodi\n\n");
        return;
    }

    for(i=0; i<3; i++){
        do{
            printf("Inserisci nome nodo %d:\t", i+1);
            scanf("%s", nome);

            if( (response= STsearch(G->tab, nome) ) == -1)
                printf("Nodo non presente riprova con altro nome \n\n");
        }
        while(response == -1);

        id[i]=response;
    }

    if(G->ladj!=NULL){
        printf("\n\nRisposta attraverso la lista adiacenza...\n");
        result=GRAPHsubgraph_ladj(G, id[0], id[1], id[2]);
    }
    else{
        printf("\n\nRisposta attraverso la matrice adiacenza...\n");
        result=GRAPHsubgraph_madj(G, id[0], id[1], id[2]);
    }

    if(result==0)
        printf("\nI tre nodi non formano un sottografo\n\n");
    else
        printf("\nI tre nodi formano un sottografo\n\n");
}
void    GRAPHladj_from_madj(Graph G){
    ladjinit(G);
    int i,j;

    for(i=0; i<G->V; i++){
        for(j=i+1; j<G->V; j++)
            if(G->madj[i][j]!=0){
                G->ladj[i]=NEW(j, G->madj[i][j], G->ladj[i]);
                G->ladj[j]=NEW(i, G->madj[i][j], G->ladj[j]);
            }
    }
}