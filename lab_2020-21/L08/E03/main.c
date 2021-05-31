#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COD 6
#define MAXS 50
#define MAXEQUIP 8

typedef enum {r_insList=0, r_insInv, r_insPg, r_delPg, r_addObjOnPg, r_rmvObjOnPg, r_err, r_stat, r_end} command_e;

typedef struct statistiche{
    int hp, mp, atk, def, mag, spr;
} stat_t;

typedef struct  inventari{
    char * nome;
    char * tipo;
    stat_t stat;
} inv_t;

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

typedef struct tabellatot_inventario{
    inv_t * vettInv;
    int nInv;
} tabInv_t;

typedef struct tabellaPersonaggi {
    nodoPg_t * headPg;
    nodoPg_t * tailPg;
    int nPg;
} tabPg_t;

pg_t readPg(FILE * fp);
pg_t setVoidPg();
int readPgMore(char *nomefile, tabPg_t * p_tabPg);
nodoPg_t * newNodePg(pg_t val, nodoPg_t * next);
void listWrapInsTailFast(tabPg_t * l,pg_t val);
pg_t searchPg(tabPg_t * tabPg, char * codice);
void liberaPg(nodoPg_t *x);
void delPg(tabPg_t * tabPg, char * codice);
void printPg();


void liberaObj(inv_t *x);
int readObjMore(char * nomefile, tabInv_t * tabInv);
inv_t * searchObj(tabInv_t * tabInv, char * nome);
int addObjOnPg(tabPg_t * tabPg, inv_t * puntObj, char * codice);
int delObjOnPg(tabPg_t * tabPg, inv_t * puntObj, char * codice);


command_e readcommand();
pg_t calculateStat(pg_t pgTMP);
void printStat(stat_t stat);

void readInv();
void printPgMore();
void printObj();
void printObjMore();

void deallocaTabPg(tabPg_t * tabPg);
void inizializzaTabPg(tabPg_t * tabPg);

void deallocaTabInv(tabInv_t * tabInv);
void inizializzaTabInv(tabInv_t * tabInv);

char strTMP[MAXS+1];
char strNameObj[MAXS+1];
int file_gia_letto=-1;

//RICORDA:DEVI SISTEMARE UN PO' LA STAMPA, PENSA ANCHE SE POTRESTI FAR VISUALIZZARE IN MODO MIGLIORE LE COSE
int main() {
    tabPg_t * tabPg;
    tabInv_t * tabInv;
    command_e command;

    pg_t pgTMP;
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
                break;

            case r_insInv:
                if(file_gia_letto!=0) {
                    printf("Inserisci nome file:\t");
                    scanf("%s", strTMP);

                    if (readObjMore(strTMP, tabInv) == -1)
                        printf("Impossibile aprire il file\n");
                    else
                        file_gia_letto = 0;
                }
                else
                    printf("Per adesso posso gestire solo l'inserimento di un file Inventario, mi spiace\n");
                    //per risolvere questo problema dovrei cambiare la struttura dell'equip, metterci solo indici e non puntatori
                    //cosi se faccio realloc dell'inventario totale non avrò problemi con gli indici
                    //qualcosa mi dice che lo vedremo nei prossimi lab (spero)
                    //qualcosa mi dice che mi dimenticherò di cancellare questi commenti, evvabb
                break;

            case r_insPg:
                    printf("Inserisci il personaggio:\n");
                    pgTMP=readPg(stdin);
                    listWrapInsTailFast(tabPg, pgTMP);
                break;

            case r_delPg:
                printf("Inserisci codice:\t");
                scanf("%s", strTMP);

                pgTMP=searchPg(tabPg, strTMP);
                if(pgTMP.stat.atk==-1)
                    printf("Nessun personaggio trovato con questo codice da cancellare\n");
                else{
                    printf("Elimineremo il personaggio xxx\n");
                    delPg(tabPg, strTMP);
                }
                break;

            case r_addObjOnPg:
                printf("Inserisci codice del personaggio:\t");
                scanf("%s", strTMP);

                pgTMP=searchPg(tabPg, strTMP);

                if(pgTMP.stat.atk==-1)
                    printf("Nessun personaggio trovato con questo codice, non posso inserire l'oggetto\n");
                else{
                    printf("Inserisci nome oggetto:\t");
                    scanf("%s", strNameObj);
                    puntObjTMP=searchObj(tabInv, strNameObj);

                    if(puntObjTMP==NULL)
                        printf("Nessun oggetto trovato....\n");
                    else{
                        if(addObjOnPg(tabPg, puntObjTMP, strTMP)==-1)
                            printf("Impossibile inserire altri oggetti\n");
                        else
                            printf("Aggiunto oggetto all'equipaggiamento\n");
                    }
                }

                break;

            case r_rmvObjOnPg:
                printf("Inserisci codice del personaggio:\t");
                scanf("%s", strTMP);

                pgTMP=searchPg(tabPg, strTMP);

                if(pgTMP.stat.atk==-1)
                    printf("Nessun personaggio trovato con questo codice, non posso inserire l'oggetto\n");
                else if(pgTMP.equip->inUso==0)
                    printf("Nessun oggetto in equip, non puoi eliminare\n");
                else{
                    printf("\nEcco gli oggetti del personaggio\n");
                    //stampa oggetti
                    printf("Inserisci nome oggetto da cancellare:\t");
                    scanf("%s", strNameObj);
                    puntObjTMP=searchObj(tabInv, strNameObj);

                    if(puntObjTMP==NULL)
                        printf("Nessun oggetto trovato....\n");
                    else{
                        if(delObjOnPg(tabPg, puntObjTMP, strTMP)==-1)
                            printf("Impossibile cancellare, oggetto non presente in equip\n");
                        else
                            printf("Oggetto elimanto dall'equipaggiamento\n");
                    }
                }

                break;

            case r_stat:
                printf("Inserisci codice del personaggio:\t");
                scanf("%s", strTMP);

                pgTMP=searchPg(tabPg, strTMP);

                if(pgTMP.stat.atk==-1)
                    printf("Nessun personaggio trovato con questo codice, non posso inserire l'oggetto\n");
                else {
                    printf("Personaggio SENZA equip:\n");
                    printPg(pgTMP);

                    if(pgTMP.equip->inUso!=0) {
                        printf("Personaggio CON equip:\n");
                        pgTMP = calculateStat(pgTMP);
                        printPg(pgTMP);
                    }
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

nodoPg_t * newNodePg(pg_t val, nodoPg_t * next){
    nodoPg_t * x=(nodoPg_t *)malloc(sizeof(nodoPg_t));

    if(x==NULL)
        return NULL;

    x->pg=val;
    x->next=next;
    x->pg.equip=malloc(sizeof(tabEquip_t));
    x->pg.equip->inUso=0;

    return x;
};

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

pg_t setVoidPg(){
    pg_t val;
    val.stat.atk=-1;
    return val;
}

pg_t searchPg(tabPg_t * tabPg, char * codice){
    nodoPg_t * x;
    for(x=tabPg->headPg; x!=NULL && strcmp(codice, x->pg.codice)!=0; x=x->next);

    if(x!=NULL)
            return x->pg;

    return setVoidPg();
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

void printPg(pg_t pg){
    printf("CODICE: %s\n", pg.codice);
    printf("NOME: %s \n", pg.nome);
    printf("CLASSE: %s \n", pg.classe);
    printf("STATISTICHE:\t");
    printStat(pg.stat);
}

void liberaObj(inv_t *x){
    free(x->nome);
    free(x->tipo);
}

inv_t readObj(FILE * fp){
    inv_t val;

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
            x->pg.equip->vettEq[i] = x->pg.equip->vettEq[i+1];
            i++;
        }
        x->pg.equip->inUso--;

        return 0;
    }

    return -1;
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

pg_t calculateStat(pg_t pgTMP){
    int i;

    for(i=0; i<pgTMP.equip->inUso; i++) {
        pgTMP.stat.hp+= pgTMP.equip->vettEq[i]->stat.hp;
        pgTMP.stat.mp+= pgTMP.equip->vettEq[i]->stat.mp;
        pgTMP.stat.atk+= pgTMP.equip->vettEq[i]->stat.atk;
        pgTMP.stat.def+= pgTMP.equip->vettEq[i]->stat.def;
        pgTMP.stat.mag+= pgTMP.equip->vettEq[i]->stat.mag;
        pgTMP.stat.spr+= pgTMP.equip->vettEq[i]->stat.spr;
    }

    if(pgTMP.stat.hp<0)
        pgTMP.stat.hp=0;
    if(pgTMP.stat.mp<0)
        pgTMP.stat.mp=0;
    if(pgTMP.stat.atk<0)
        pgTMP.stat.atk=0;
    if(pgTMP.stat.def<0)
        pgTMP.stat.def=0;
    if(pgTMP.stat.mag<0)
        pgTMP.stat.mag=0;
    if(pgTMP.stat.spr<0)
        pgTMP.stat.spr=0;

    return pgTMP;
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
    if(strcmp(strTMP, "ins_pg")==0)   return r_insPg;
    if(strcmp(strTMP, "ins_inv")==0)    return r_insInv;
    if(strcmp(strTMP, "add_obj")==0)    return r_addObjOnPg;
    if(strcmp(strTMP, "rmv_obj")==0)    return r_rmvObjOnPg;
    if(strcmp(strTMP, "del_pg")==0)     return r_delPg;
    if(strcmp(strTMP, "stat")==0)       return r_stat;
    if(strcmp(strTMP, "fine")==0)       return r_end;

    return r_err;
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

void inizializzaTabInv(tabInv_t * tabInv){
    tabInv->vettInv=NULL;
    tabInv->nInv=0;
}

void deallocaTabInv(tabInv_t * tabInv){
    for(int i=0; i<tabInv->nInv; i++)
        liberaObj( &(tabInv->vettInv[i]));
}