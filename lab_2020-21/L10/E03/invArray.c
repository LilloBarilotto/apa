//
// Created by super on 16/12/2020.
//
#include "invArray.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct invArray_s{
    inv_t * vettInv;
    int nInv;
};

invArray_t invArray_init(){
    invArray_t invArray=malloc( 1 * sizeof(struct invArray_s));
    invArray->vettInv=NULL;
    invArray->nInv=0;
    return invArray;
}
void invArray_free(invArray_t invArray){
    free(invArray->vettInv);
    free(invArray);
}


void invArray_read(FILE *fp, invArray_t invArray){
    int O, i;

    fscanf(fp, "%d", &O);

    invArray->vettInv=malloc( O *sizeof(inv_t));
    //quando potrai metti realloc(invArray->vettInv, (invArray->nInv+O)*sizeof(inv_t));
    //spoiler: Beh quello prima era un commento dei vecchi lab, a quanto pare non puoi. Sad Lillo Barilotto
    for(i=invArray->nInv, invArray->nInv+=O; i<invArray->nInv; i++)
        inv_read(fp, &invArray->vettInv[i]);

}
void invArray_print(FILE *fp, invArray_t invArray){
    int i;
    printf("Lista inventario TOTALE:\n");
    for(i=0; i<invArray->nInv; i++)
        inv_print(fp, &(invArray->vettInv[i]));
}

//PRIMA SI CHIAMAVA searchObj la invArray_searchByName, controlla di aver cambiato i nomi ovunque
int invArray_searchByName(invArray_t invArray, char *name){
    int i;
    for(i=0; i<invArray->nInv && strcmp(invArray->vettInv[i].nome, name)!=0; i++);

    if(i<invArray->nInv)
        return i;

    return -1;
}

void invArray_printByIndex(FILE *fp, invArray_t invArray, int index){
    inv_print(fp, invArray_getByIndex(invArray, index));
}

inv_t *invArray_getByIndex(invArray_t invArray, int index){
    return &(invArray->vettInv[index]);
}