//
// Created by super on 07/12/2020.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inv.h"

#define MAXEQUIP 8
#define COD 6

typedef struct equipaggiamenti{
    int inUso; //conta quanti equipaggiamenti usa
    inv_t * vettEq[MAXEQUIP]; //max 8 indici per i vari oggetti presenti in lista
} tabEquip_t;

typedef struct personaggio_t{
    char codice[COD+1];
    char * nome;
    char * classe;
    stat_t stat;
    tabEquip_t * equip;
} pg_t;

typedef struct nodo_personaggiot nodoPg_t;
struct nodo_personaggiot{
    pg_t pg;
    nodoPg_t * next;
};

typedef struct tabellaPersonaggi {
    nodoPg_t * headPg;
    nodoPg_t * tailPg;
    int nPg;
} tabPg_t;

pg_t readPg(FILE * fp);
int readPgMore(char *nomefile, tabPg_t * p_tabPg);
nodoPg_t * newNodePg(pg_t val, nodoPg_t * next);
void listWrapInsTailFast(tabPg_t * l,pg_t val);
pg_t * searchPg(tabPg_t * tabPg, char * codice);
void liberaPg(nodoPg_t *x);
void delPg(tabPg_t * tabPg, char * codice);
void printPg(pg_t * p_pg, int see_stat);
void printPgEquip(pg_t * p_pg);
int readPgInUsoEquip(pg_t * p_pg);
stat_t calculateStatPg(pg_t * p_pg);
void printPgMore(tabPg_t * tabPg, int see_stat);


int addObjOnPg(tabPg_t * tabPg, inv_t * puntObj, char * codice);
int delObjOnPg(tabPg_t * tabPg, inv_t * puntObj, char * codice);

void deallocaTabPg(tabPg_t * tabPg);
void inizializzaTabPg(tabPg_t * tabPg);

