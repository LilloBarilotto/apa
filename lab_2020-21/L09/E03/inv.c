//
// Created by super on 07/12/2020.
//
#include "inv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSinv 50

void liberaObj(inv_t *x){
    free(x->nome);
    free(x->tipo);
}
inv_t readObj(FILE * fp){
    inv_t val;
    char strTMP[MAXSinv];

    fscanf(fp, "%s", strTMP);
    val.nome=strdup(strTMP);

    fscanf(fp, "%s", strTMP);
    val.tipo=strdup(strTMP);

    fscanf(fp, "%d %d %d %d %d %d", &val.stat.hp, &val.stat.mp, &val.stat.atk, &val.stat.def, &val.stat.mag, &val.stat.spr);

    return val;
}
int readObjMore(char * nomefile, tabInv_t * tabInv){
    FILE *fp;
    int O, i;

    if((fp=fopen(nomefile, "r"))==NULL)
        return -1;

    fscanf(fp, "%d", &O);

    tabInv->vettInv=malloc( O *sizeof(inv_t));
    //quando potrai metti realloc (tabInv->vettInv, (tabInv->nInv+O)*sizefof(inv_t));

    for(i=tabInv->nInv, tabInv->nInv+=O; i<tabInv->nInv; i++)
        tabInv->vettInv[i]=readObj(fp);

    fclose(fp);
    return 0;
}
inv_t * searchObj(tabInv_t * tabInv, char * nome){
    int i;
    for(i=0; i<tabInv->nInv && strcmp(tabInv->vettInv[i].nome, nome)!=0; i++);

    if(i<tabInv->nInv)
        return &(tabInv->vettInv[i]);

    return NULL;
}

void inizializzaTabInv(tabInv_t * tabInv){
    tabInv->vettInv=NULL;
    tabInv->nInv=0;
}
void deallocaTabInv(tabInv_t * tabInv){
    for(int i=0; i<tabInv->nInv; i++)
        liberaObj( &(tabInv->vettInv[i]));
}

void printStat(stat_t stat){
    //    int hp, mp, atk, def, mag, spr;
    printf("\thp: %d", stat.hp);
    printf("\tmp: %d", stat.mp);
    printf("\tatk: %d", stat.atk);
    printf("\tdef: %d", stat.def);
    printf("\tmag: %d", stat.mag);
    printf("\tspr: %d\n", stat.spr);
}

void printObj(inv_t * obj){
    printf("NOME: %s", obj->nome);
    printf("\tTIPO: %s", obj->tipo);
    printf("\tSTAT:");
    printStat(obj->stat);
}
void printObjMore(tabInv_t * tabInv){
    int i;
    printf("Lista inventario TOTALE:\n");
    for(i=0; i<tabInv->nInv; i++)
        printObj( &(tabInv->vettInv[i]));
}
