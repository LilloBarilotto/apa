//
// Created by super on 16/12/2020.
//

#include "equipArray.h"
#define N_SCELTE_UPDATE 2

struct equipArray_s{
    int inUso; //conta quanti equipaggiamenti usa
    int vettEq[EQUIP_SLOT]; //max 8 indici per i vari oggetti presenti in lista
};

equipArray_t equipArray_init(){
    equipArray_t equip=malloc(1 * sizeof(struct equipArray_s));
    equip->inUso=0;
    return equip;
}
void equipArray_free(equipArray_t equipArray){
    free(equipArray);
}

/* quanti equipaggiamenti sono in uso */
int equipArray_inUse(equipArray_t equipArray){
    return equipArray->inUso;
}

/* scrittura su file */
void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray){
    int i, inUso;

    inUso=equipArray_inUse(equipArray);
    
    printf("Equipaggiamento %d slot occupati su %d:\n", inUso, EQUIP_SLOT);

    for(i=0; i<inUso; i++)
        invArray_printByIndex(fp, invArray, equipArray->vettEq[i]);
}
/* modifica equipaggiamento scegliendo un oggetto da inventario */
void equipArray_update(equipArray_t equipArray, invArray_t invArray){

    int sel, i, index, inUso;
    char *scelte_equip[]={
            "Aggiungi Oggetto nell'equip",
            "Rimuovi Oggetto nell'equip",
    };
    char nome[LEN];

    printf("\nMenu Equip:\n");
    for(i=0;i<N_SCELTE_UPDATE;i++)
        printf("%2d > %s\n",i,scelte_equip[i]);
    scanf(" %d",&sel);

    inUso=equipArray_inUse(equipArray);

    switch(sel){
        case 0:
            if(inUso==8){
                printf("Impossibile inserire altri oggetti nell'equip");
                return;
            }

            invArray_print(stdout, invArray);
            printf("Inserisci nome dell'oggetto:\t");
            scanf("%s", nome);
            index=invArray_searchByName(invArray, nome);

            if(index!=-1) {

                equipArray->vettEq[inUso] = index;
                equipArray->inUso++;
                printf("Oggetto aggiunto in equip");
            }
            else
                printf("Oggetto NON aggiunto in equip");

            break;
        case 1:
            if(inUso==0){
                printf("\nImpossibile cancellazione, nessun oggetto presente in equip (e comprali due stivali o pozioni braccino corto)\n");
                return;
            }

            printf("\nInserisci nome dell'oggetto DA ELIMINARE DALL'EQUIP:\t");
            scanf("%s", nome);
            index=invArray_searchByName(invArray, nome);

            if(index==-1)
                printf("Non trovato nell'inventario l'oggetto...");
            else {
                for (i = 0; i < inUso; i++)
                    if (index == equipArray_getEquipByIndex(equipArray, i)) {

                        while (i < inUso) {
                            equipArray->vettEq[i] = equipArray->vettEq[i + 1];
                            i++;
                        }
                        equipArray->inUso--;
                        printf("Oggetto cancellato da equip");
                        return;
                    }

                printf("Oggetto non trovato in equip");
            }
            break;

        default:
            printf("\nErrore, inserito numero errato");
    }
}
/* torna indice (nel vettore inventario) dell'oggetto in posizione index (0..EQUIP_SLOT-1) di equipArray */
int equipArray_getEquipByIndex(equipArray_t equipArray, int index){
    return equipArray->vettEq[index];
}

stat_t equipArray_calculateDelta(equipArray_t equipArray, invArray_t invArray){
    stat_t delta, statinv;
    int i, inUso;

    delta=stat_init();
    inUso=equipArray_inUse(equipArray);

    for(i=0; i<inUso; i++){
        statinv=inv_getStat(invArray_getByIndex(invArray, equipArray_getEquipByIndex(equipArray, i)));
        stat_addStat(&delta, &statinv, 1);
    }

    return delta;
}