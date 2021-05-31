//
// Created by super on 07/12/2020.
//
#ifndef INV_H_DEFINED
#define INV_H_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct statistiche{
    int hp, mp, atk, def, mag, spr;
} stat_t;

typedef struct  inventari{
    char * nome;
    char * tipo;
    stat_t stat;
} inv_t;

typedef struct tabellatot_inventario{
    inv_t * vettInv;
    int nInv;
} tabInv_t;

inv_t * searchObj(tabInv_t * tabInv, char * nome);
int readObjMore(char * nomefile, tabInv_t * tabInv);
void liberaObj(inv_t *x);
void printObj(inv_t * obj);
void printObjMore(tabInv_t * tabInv);

void deallocaTabInv(tabInv_t * tabInv);
void inizializzaTabInv(tabInv_t * tabInv);

void printStat(stat_t stat);

#endif