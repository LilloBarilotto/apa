//
// Created by super on 16/12/2020.
//
#include "pgList.h"

typedef struct nodo_personaggiot nodoPg_t;
struct nodo_personaggiot{
    pg_t pg;
    nodoPg_t * next;
};
struct pgList_s {
    nodoPg_t * headPg;
    nodoPg_t * tailPg;
    int nPg;
};

void pgList_freeNodePg(nodoPg_t *x);
nodoPg_t * newNodePg(pg_t val, nodoPg_t * next){
    nodoPg_t * x=(nodoPg_t *)malloc(sizeof(nodoPg_t));

    if(x==NULL)
        return NULL;

    x->pg=val;
    x->next=next;
    x->pg.equip=equipArray_init();

    return x;
}
void pgList_freeNodePg(nodoPg_t * x){
    pg_clean(&x->pg);
    free(x);
}


pgList_t pgList_init(){
    pgList_t pgList= malloc( 1 * sizeof(struct pgList_s));
    pgList->headPg=NULL;
    pgList->tailPg=NULL;
    pgList->nPg=0;

    return pgList;
}
void pgList_free(pgList_t pgList){
    nodoPg_t * x;
    while(pgList->headPg!=NULL){
        x=pgList->headPg;
        pgList->headPg=pgList->headPg->next;
        pgList_freeNodePg(x);
    }
}


void pgList_read(FILE *fp, pgList_t pgList){
    pg_t pg;

    while(pg_read(fp, &pg)==3)
        pgList_insert(pgList, pg);
}
void pgList_print(FILE *fp, pgList_t pgList, invArray_t invArray){
    nodoPg_t *x;
    printf("LISTA PERSONAGGI\n");

    for(x=pgList->headPg; x!=NULL; x=x->next)
        pg_print(fp, &x->pg, invArray);
}


void pgList_insert(pgList_t pgList, pg_t pg){
    if (pgList->headPg==NULL)
        pgList->headPg = pgList->tailPg = newNodePg(pg, NULL);
    else {
        pgList->tailPg->next = newNodePg(pg, NULL);
        pgList->tailPg = pgList->tailPg->next;
    }
    pgList->nPg++;
}
void pgList_remove(pgList_t pgList, char* cod){
    nodoPg_t * x, *p;

    for(x=pgList->headPg, p=NULL; x!=NULL && strcmp(cod, x->pg.cod)!=0; p=x,  x=x->next);

    if(x!=NULL){
        if(p!=NULL)
            p->next=x->next;
        else
            pgList->headPg=x->next;

        pgList_freeNodePg(x);
    }
    else
        printf("\nCodice inserito non valido, non trovato e non cancellato dal roster di personaggi\n");
}
pg_t *pgList_searchByCode(pgList_t pgList, char* cod){
    nodoPg_t * x;
    for(x=pgList->headPg; x!=NULL && strcmp(cod, x->pg.cod)!=0; x=x->next);

    if(x!=NULL)
        return &(x->pg);

    return NULL;
}