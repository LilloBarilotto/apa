#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXS 50

#include "inv.h"
#include "pg.h"

typedef enum {r_insList=0, r_insInv, r_insPg, r_delPg, r_addObjOnPg, r_rmvObjOnPg, r_err, r_stat, r_end} command_e;

command_e readcommand();

char strTMP[MAXS+1];
char strNameObj[MAXS+1];
int file_gia_letto=-1;

int main() {
    tabPg_t * tabPg;
    tabInv_t * tabInv;
    command_e command;

    pg_t * p_pg;
    inv_t * puntObjTMP;

    tabPg  = malloc(1 * sizeof(tabPg_t));
    tabInv = malloc(1 * sizeof(tabInv_t));

    inizializzaTabPg(tabPg);
    inizializzaTabInv(tabInv);

    do{
        command=readcommand();

        switch(command){
            case r_insList:
                printf("Inserisci nome file:\t");
                scanf("%s", strTMP);

                if(readPgMore(strTMP, tabPg)==-1)
                    printf("Impossibile aprire il file\n");
                else
                    printf("Lettura completata\n");
                break;

            case r_insInv:
                if(file_gia_letto!=0) {
                    printf("Inserisci nome file:\t");
                    scanf("%s", strTMP);

                    if (readObjMore(strTMP, tabInv) == -1)
                        printf("Impossibile aprire il file\n");
                    else {
                        file_gia_letto = 0;
                        printf("Lettura completata\n");
                    }
                }
                else
                    printf("Per adesso posso gestire solo l'inserimento di un file Inventario, mi spiace\n");
                break;

            case r_insPg:
                printf("Inserisci il personaggio:\n");
                listWrapInsTailFast(tabPg, readPg(stdin));
                break;

            case r_delPg:
                printPgMore(tabPg, 0);
                printf("Inserisci codice:\t");
                scanf("%s", strTMP);

                p_pg=searchPg(tabPg, strTMP);
                if(p_pg==NULL)
                    printf("Nessun personaggio trovato con questo codice da cancellare\n");
                else{
                    delPg(tabPg, strTMP);
                    printf("Eliminazione personaggio completata\n");
                }
                break;

            case r_addObjOnPg:
                printPgMore(tabPg, 0);
                printf("Inserisci codice del personaggio:\t");
                scanf("%s", strTMP);

                p_pg=searchPg(tabPg, strTMP);

                if(p_pg==NULL)
                    printf("Nessun personaggio trovato con questo codice, non posso inserire l'oggetto\n");
                else if(readPgInUsoEquip(p_pg)==MAXEQUIP)
                    printf("Impossibile inserire altri oggetti\n");
                else{
                    printObjMore(tabInv);

                    printf("\nInserisci nome oggetto:\t");
                    scanf("%s", strNameObj);
                    puntObjTMP=searchObj(tabInv, strNameObj);

                    if(puntObjTMP==NULL)
                        printf("Nessun oggetto trovato....\n");
                    else{
                        if(addObjOnPg(tabPg, puntObjTMP, strTMP)==-1)
                            printf("Impossibile inserire altri oggetti\n"); //In realtà basta readPgInUsoEquip, ma cosi do senso al return di addObjOnPg, magari mi torna utile per altri casi
                        printf("Aggiunto oggetto all'equipaggiamento\n");
                    }
                }
                break;

            case r_rmvObjOnPg:
                printPgMore(tabPg, 0);
                printf("Inserisci codice del personaggio:\t");
                scanf("%s", strTMP);

                p_pg=searchPg(tabPg, strTMP);

                if(p_pg==NULL)
                    printf("Nessun personaggio trovato con questo codice, non posso inserire l'oggetto\n");
                else if(readPgInUsoEquip(p_pg)==0)
                    printf("Nessun oggetto in equip, non puoi eliminare\n");
                else{
                    printf("\nEcco gli oggetti del personaggio\n");
                    printPgEquip(p_pg);
                    printf("\n\n");
                    printf("Inserisci nome oggetto da cancellare:\t");
                    scanf("%s", strNameObj);
                    puntObjTMP=searchObj(tabInv, strNameObj);

                    if(puntObjTMP==NULL)
                        printf("Nessun oggetto trovato....\n");
                    else{
                        if(delObjOnPg(tabPg, puntObjTMP, strTMP)==-1)
                            printf("Impossibile cancellare, oggetto non presente in equip\n");
                        else
                            printf("Oggetto eliminato dall'equipaggiamento\n");
                    }
                }
                break;

            case r_stat:
                printPgMore(tabPg, 0);
                printf("Inserisci codice del personaggio:\t");
                scanf("%s", strTMP);

                p_pg=searchPg(tabPg, strTMP);

                if(p_pg==NULL)
                    printf("Nessun personaggio trovato con questo codice, non posso inserire l'oggetto\n");
                else {
                    printPg(p_pg, 1);
                    if(readPgInUsoEquip(p_pg)!=0) {
                        printf("EQUIP:\n");
                        printPgEquip(p_pg);
                        printf("STATISTICHE CON EQUIP INCLUSO:\n");
                        printStat(calculateStatPg(p_pg));
                    }
                    else
                        printf("NO EQUIP PRESENTE\n");
                }
                break;

            case r_err:
                printf("Riprovare, comando errato\n");
                break;
        }
    }while(command!= r_end);

    deallocaTabPg(tabPg);
    deallocaTabInv(tabInv);
    printf("Bye have a great time!\n");
    return 0;
}

command_e readcommand(){
    printf("Inserisci uno dei seguenti comandi:\n");
    printf("'INS_LIST', per inserire i personaggi da file\n");
    printf("'INS_PG'  , per inserire un personaggio da tastiera\n");
    if(file_gia_letto!=0)
        printf("'INS_INV' , per inserire gli oggetti da file\n");
    printf("'ADD_OBJ' , aggiungi oggetto dell'inventario nell'equipaggiamento un personaggio\n");
    printf("'RMV_OBJ' , togli oggetto dall'equipaggiamento di un personaggio\n");
    printf("'STAT'    , mostra le stat CON e SENZA equip di un personaggio\n");
    printf("'DEL_PG'  , per eliminare un personaggio dalla lista\n");
    printf("'FINE'    , chiudi il programma\n\n");

    scanf("%s", strTMP);
    strlwr(strTMP);

    if(strcmp(strTMP, "ins_list")==0)   return r_insList;
    if(strcmp(strTMP, "ins_pg")==0)     return r_insPg;
    if(strcmp(strTMP, "ins_inv")==0)    return r_insInv;
    if(strcmp(strTMP, "add_obj")==0)    return r_addObjOnPg;
    if(strcmp(strTMP, "rmv_obj")==0)    return r_rmvObjOnPg;
    if(strcmp(strTMP, "del_pg")==0)     return r_delPg;
    if(strcmp(strTMP, "stat")==0)       return r_stat;
    if(strcmp(strTMP, "fine")==0)       return r_end;

    return r_err;
}