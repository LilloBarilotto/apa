//
// Created by super on 07/12/2020.
//
#include "pg.h"

int pg_read(FILE *fp, pg_t *pgp){
    int letti=0;
    letti+=fscanf(fp, "%s", pgp->cod);
    letti+=fscanf(fp, "%s", pgp->nome);
    letti+=fscanf(fp, "%s", pgp->classe);

    stat_read(fp, &pgp->b_stat);

    pgp->eq_stat=pgp->b_stat;

    return letti;
}
void pg_clean(pg_t *pgp){
    equipArray_free(pgp->equip);
}
void pg_print(FILE *fp, pg_t *pgp, invArray_t invArray){
    printf("\n\nCODICE: %s\t", pgp->cod);
    printf("NOME: %s \t", pgp->nome);
    printf("CLASSE: %s \n", pgp->classe);

    printf("STAT BASE:\t");
    stat_print(fp, &pgp->b_stat, 0);

    printf("STAT CON EQUIP:");
    stat_print(fp, &pgp->eq_stat,  1);

    equipArray_print(fp, pgp->equip, invArray);
}

void pg_updateEquip(pg_t *pgp, invArray_t invArray){
    pg_print(stdout, pgp, invArray);

    equipArray_update(pgp->equip, invArray);

    pg_calculateEquipStat( pgp, invArray);//Prima lo avevi messo in pg_print,
    // ma dico io ma perche? Almeno cosi lo ricalcoli solo nel caso di possibili update
}

void pg_calculateEquipStatVecchio(pg_t *pgp, invArray_t invArray){
    int i, inUso;
    stat_t statTMP;

    inUso=equipArray_inUse(pgp->equip);
    pgp->eq_stat=pgp->b_stat;

    for(i=0; i<inUso; i++) {
        statTMP=inv_getStat(invArray_getByIndex(invArray, equipArray_getEquipByIndex(pgp->equip, i)));
        stat_addStat(&pgp->eq_stat, &statTMP, 1);
    }
}

void pg_calculateEquipStat(pg_t *pgp, invArray_t invArray){
    stat_t delta;

    pgp->eq_stat=pgp->b_stat;
    delta=equipArray_calculateDelta(pgp->equip, invArray);

    stat_addStat(&pgp->eq_stat, &delta, 1);
}