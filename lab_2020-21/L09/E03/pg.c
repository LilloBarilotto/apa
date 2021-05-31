//
// Created by super on 07/12/2020.
//
#include "pg.h"
#define MAXSpg 50

nodoPg_t * newNodePg(pg_t val, nodoPg_t * next){
    nodoPg_t * x=(nodoPg_t *)malloc(sizeof(nodoPg_t));

    if(x==NULL)
        return NULL;

    x->pg=val;
    x->next=next;
    x->pg.equip=malloc(sizeof(tabEquip_t));
    x->pg.equip->inUso=0;

    return x;
}
void listWrapInsTailFast(tabPg_t * l,pg_t val) {
    if (l->headPg==NULL)
        l->headPg = l->tailPg = newNodePg(val, NULL);
    else {
        l->tailPg->next = newNodePg(val, NULL);
        l->tailPg = l->tailPg->next;
    }
    l->nPg++;
}

pg_t readPg(FILE * fp){
    pg_t valTMP;
    char strTMP[MAXSpg+1];

    fscanf(fp, "%s", valTMP.codice);

    fscanf(fp, "%s", strTMP);
    valTMP.nome=strdup(strTMP);

    fscanf(fp, "%s", strTMP);
    valTMP.classe=strdup(strTMP);

    fscanf(fp, "%d %d %d %d %d %d", &valTMP.stat.hp, &valTMP.stat.mp, &valTMP.stat.atk, &valTMP.stat.def, &valTMP.stat.mag, &valTMP.stat.spr);

    return valTMP;
}
int readPgMore(char * nomefile, tabPg_t * tabPg){
    FILE *fp;
    pg_t val;

    if((fp=fopen(nomefile, "r"))==NULL)
        return -1;

    while(!feof(fp)) {
        val=readPg(fp);
        listWrapInsTailFast(tabPg, val);
    }

    fclose(fp);
    return 0;
}
pg_t * searchPg(tabPg_t * tabPg, char * codice){
    nodoPg_t * x;
    for(x=tabPg->headPg; x!=NULL && strcmp(codice, x->pg.codice)!=0; x=x->next);

    if(x!=NULL)
        return &(x->pg);

    return NULL;
}
void delPg(tabPg_t * tabPg, char * codice){
    nodoPg_t * x, *p;

    for(x=tabPg->headPg, p=NULL; x!=NULL && strcmp(codice, x->pg.codice)!=0; p=x,  x=x->next);

    if(x!=NULL){
        p->next=x->next;
        liberaPg(x);
    }
}
void liberaPg(nodoPg_t *x){
    free(x->pg.nome);
    free(x->pg.classe);
    free(x->pg.equip);
    free(x);
}
void printPg(pg_t * p_pg, int see_stat){
    printf("CODICE: %s\t", p_pg->codice);
    printf("NOME: %s \t", p_pg->nome);
    printf("CLASSE: %s \n", p_pg->classe);

    if(see_stat)
        printStat(p_pg->stat);
}
void printPgMore(tabPg_t * tabPg, int see_stat){
     nodoPg_t *x;
     printf("LISTA PERSONAGGI\n");

     for(x=tabPg->headPg; x!=NULL; x=x->next)
         printPg(&(x->pg), see_stat);
}

int addObjOnPg(tabPg_t * tabPg, inv_t * puntObj, char * codice){
    nodoPg_t * x;

    for(x=tabPg->headPg; x!=NULL && strcmp(codice, x->pg.codice)!=0; x=x->next);

    if(x->pg.equip->inUso<MAXEQUIP){
        x->pg.equip->vettEq[x->pg.equip->inUso]=puntObj;
        x->pg.equip->inUso++;
        return 0;
    }

    return -1;
}
int delObjOnPg(tabPg_t * tabPg, inv_t * puntObj, char * codice){
    nodoPg_t * x;
    int i;

    for(x=tabPg->headPg; x!=NULL && strcmp(codice, x->pg.codice)!=0; x=x->next);

    for(i=0; i<x->pg.equip->inUso && x->pg.equip->vettEq[i]!=puntObj; i++);

    if(i <  x->pg.equip->inUso){
        while(i < x->pg.equip->inUso) {
            x->pg.equip->vettEq[i] = x->pg.equip->vettEq[i + 1];
            i++;
        }
        x->pg.equip->inUso--;

        return 0;
    }
    return -1;
}


void deallocaTabPg(tabPg_t * tabPg){
    nodoPg_t * x;
    while(tabPg->headPg!=NULL){
        x=tabPg->headPg;
        tabPg->headPg=tabPg->headPg->next;
        liberaPg(x);
    }
}
void inizializzaTabPg(tabPg_t * tabPg){
    tabPg->headPg=NULL;
    tabPg->tailPg=NULL;
    tabPg->nPg=0;
}

stat_t calculateStatPg(pg_t * p_pg){
    int i;
    stat_t statTMP=p_pg->stat;

    for(i=0; i<p_pg->equip->inUso; i++) {
        statTMP.hp+= p_pg->equip->vettEq[i]->stat.hp;
        statTMP.mp+= p_pg->equip->vettEq[i]->stat.mp;
        statTMP.atk+= p_pg->equip->vettEq[i]->stat.atk;
        statTMP.def+= p_pg->equip->vettEq[i]->stat.def;
        statTMP.mag+= p_pg->equip->vettEq[i]->stat.mag;
        statTMP.spr+= p_pg->equip->vettEq[i]->stat.spr;
    }

    if(statTMP.hp<0)
        statTMP.hp=0;
    if(statTMP.mp<0)
        statTMP.mp=0;
    if(statTMP.atk<0)
        statTMP.atk=0;
    if(statTMP.def<0)
        statTMP.def=0;
    if(statTMP.mag<0)
        statTMP.mag=0;
    if(statTMP.spr<0)
        statTMP.spr=0;

    return statTMP;
}
void printPgEquip(pg_t * p_pg){
    int i;
    for(i=0; i<p_pg->equip->inUso; i++)
        printObj(p_pg->equip->vettEq[i]);
}
int readPgInUsoEquip(pg_t * p_pg){
    return p_pg->equip->inUso;
}
