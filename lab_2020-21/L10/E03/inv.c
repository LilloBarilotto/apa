//
// Created by super on 07/12/2020.
//
#include "inv.h"
#include <stdio.h>

void stat_read(FILE *fp, stat_t *statp){
    fscanf(fp, "%d %d %d %d %d %d", &statp->hp, &statp->mp, &statp->atk, &statp->def, &statp->mag, &statp->spr);
}
void stat_print(FILE *fp, stat_t *statp, int soglia){
    //    int hp, mp, atk, def, mag, spr;
    //    devi aggiungere la soglia per quanto riguarda il personaggio e la stampa dei suoi stat
    if(soglia==0) {
        fprintf(fp, "\thp: %d", statp->hp);
        fprintf(fp, "\tmp: %d", statp->mp);
        fprintf(fp, "\tatk: %d", statp->atk);
        fprintf(fp, "\tdef: %d", statp->def);
        fprintf(fp, "\tmag: %d", statp->mag);
        fprintf(fp, "\tspr: %d\n", statp->spr);
    }
    else{
        if(statp->hp>0) fprintf(fp, "\thp: %d", statp->hp);
        else            fprintf(fp, "\thp: 0");

        if(statp->mp>0) fprintf(fp, "\tmp: %d", statp->mp);
        else            fprintf(fp, "\tmp: 0");

        if(statp->atk>0) fprintf(fp, "\tatk: %d", statp->atk);
        else             fprintf(fp, "\tatk: 0");

        if(statp->def>0) fprintf(fp, "\tdef: %d", statp->def);
        else             fprintf(fp, "\tdef: 0");

        if(statp->mag>0) fprintf(fp, "\tmag: %d", statp->mag);
        else             fprintf(fp, "\tmag: 0");

        if(statp->spr>0) fprintf(fp, "\tspr: %d\n", statp->spr);
        else             fprintf(fp, "\tspr: 0\n");
    }
}


void inv_read(FILE *fp, inv_t *invp){
    fscanf(fp, "%s", invp->nome);

    fscanf(fp, "%s", invp->tipo);

    stat_read(fp, &invp->stat);
}
void inv_print(FILE *fp, inv_t *invp){
    printf("NOME: %s", invp->nome);
    printf("\tTIPO: %s", invp->tipo);
    printf("\tSTAT:");
    stat_print(fp, &invp->stat, 0);
}

stat_t inv_getStat(inv_t *invp){
    return invp->stat;
}

void stat_addStat(stat_t * statpEquip, stat_t * statpObj, int add){
    //per adesso uso solo add!=1, lascio entrambe le versioni nel caso facessi il calcolo statEquip...
    //... ad ogni aggiunta/rimozione di un solo oggetto piuttosto del ricalcolo completo delle eq_stat ecc...
    if(add==-1){
        statpEquip->hp-= statpObj->hp;
        statpEquip->mp-= statpObj->mp;
        statpEquip->atk-= statpObj->atk;
        statpEquip->def-= statpObj->def;
        statpEquip->mag-= statpObj->mag;
        statpEquip->spr-= statpObj->spr;
    }
    else{
        statpEquip->hp+= statpObj->hp;
        statpEquip->mp+= statpObj->mp;
        statpEquip->atk+= statpObj->atk;
        statpEquip->def+= statpObj->def;
        statpEquip->mag+= statpObj->mag;
        statpEquip->spr+= statpObj->spr;
    }
}

stat_t stat_init(){
    stat_t stat;

    stat.atk=0;
    stat.def=0;
    stat.hp=0;
    stat.mag=0;
    stat.mp=0;
    stat.spr=0;

    return stat;
}