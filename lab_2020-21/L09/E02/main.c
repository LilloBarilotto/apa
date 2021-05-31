#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char color;
    int val;
} tipo;

typedef struct {
    tipo type[2]; //0 per colonna, 1 per riga, quando useremo rotazione saranno praticamente invertiti
    int block;
}tessera;

typedef struct {
    tessera * x;
    int indicetessera; //non serve per trovare la soluzione, ma per come avevo fatto il programma non potevo stamparla nel formato board
    // mi salvo l'indice SOLO per la stampa, questo dato non viene usato per calcoli vari
    int rotazione;
    int block;
}casella;

void recursive(casella * cas, tessera * tes, int T, int R, int C, int count, casella *b_sol, int *b_val );
void find_sol(casella * cas, tessera * tes, int T, int R, int C);
int checksol(casella * cas, int R, int C);
void printSol(casella * cas, int val, int R, int C);

int main() {
    FILE *fptiles, *fpboard;
    int T,i,j, R, C, pos, rot;

    tessera * tes;
    casella * cas;

    if((fptiles=fopen("tiles.txt","r"))==NULL)
        return -1;

    fscanf(fptiles,"%d", &T );
    tes=malloc(T * sizeof(tessera));

    for(i=0; i<T; i++) {
        fscanf(fptiles, "\n%c %d %c %d", &(tes[i].type[0].color), &(tes[i].type[0].val), &(tes[i].type[1].color),
               &(tes[i].type[1].val));
        tes[i].block=0;
    }
    fclose(fptiles);

    if((fpboard=fopen("board.txt","r"))==NULL)
        return -1;

    fscanf(fpboard  ,"%d%d", &R, &C);
    cas=malloc((R*C) * sizeof(casella));

    for(i=0; i<R; i++)
        for(j=0; j<C; j++){
            cas[i*C+j].block=0;

            fscanf(fpboard, "%d/%d", &pos,&rot);
            if(pos!=-1 && rot!=-1){
                cas[i*C+j].x=&tes[pos];
                cas[i*C+j].indicetessera=pos;

                cas[i*C+j].rotazione=rot;
                cas[i*C+j].block=1;
                tes[pos].block=1;
            }
        }
    fclose(fpboard);

    find_sol(cas, tes, T, R, C);

    free(cas);
    free(tes);
    return 0;
}

void find_sol(casella * cas, tessera * tes, int T, int R, int C){
    casella *b_sol;
    int b_val=0, i, j;

    b_sol=malloc((R*C) * sizeof(casella));

    for(i=0; cas[i].block==1; i++);

    for(j=0; j<T; j++){
        if(tes[j].block==0){
            cas[i].block=1;
            tes[j].block=1;
            cas[i].x=&(tes[j]);
            cas[i].indicetessera=j;
            recursive(cas, tes, T, R, C, i+1, b_sol, &b_val);

            cas[i].rotazione=1;
            recursive(cas, tes, T, R, C, i+1, b_sol, &b_val);
            cas[i].rotazione=0;

            tes[j].block=0;
            cas[i].block=0;
        }
    }
    printSol(b_sol, b_val, R, C);

    free(b_sol);
}

void recursive(casella * cas, tessera * tes, int T, int R, int C, int count, casella *b_sol, int *b_val ){
    int val,i,j;

    //SE MI TROVO IN UN BLOCCO DOVE E' GIA STATO BLOCCATO DALLA BOARD.TXT VADO AVANTI
    while(count< R*C && cas[count].block==1)
        count++;

    //SONO ALLA FINE
    if(count==R*C) {
        val=checksol(cas, R, C);
        if (val>*b_val){
            for(i=0; i<T; i++)
                b_sol[i]=cas[i];
            *b_val=val;
        }
        return;
    }

    //HO ANCORA CASELLE VUOTE (Indipendentemente da quante tessere ho rimaste)
    for(j=0; j<T; j++){
        if(tes[j].block==0){
            cas[count].block=1;
            tes[j].block=1;
            cas[count].x=&(tes[j]);
            cas[count].indicetessera=j;
            recursive(cas, tes, T, R, C, count+1, b_sol, b_val);

            cas[count].rotazione=1;
            recursive(cas, tes, T, R, C, count+1, b_sol, b_val);
            cas[count].rotazione=0;

            tes[j].block=0;
            cas[count].block=0;
        }
    }
}

int checksol(casella * cas, int R, int C){
    int sum_tot=0, sum_c_tmp=0, sum_r_tmp=0, i, j, pos;
    char colore_tmp;

    //CALCOLO SOMMA PARZIALE DI OGNI RIGA
    for(i=0; i<R; sum_r_tmp=0, i++) {
        pos=i *C;
        sum_r_tmp += cas[pos].x->type[cas[pos].rotazione].val;
        colore_tmp=  cas[pos].x->type[cas[pos].rotazione].color;

        for (j = 1; j < C;
             sum_r_tmp+=  cas[pos + j].x->type[cas[pos + j].rotazione].val,
                     j++)
            if(colore_tmp != cas[pos + j].x->type[cas[pos + j].rotazione].color){
                sum_r_tmp=0;
                break;
            }
        sum_tot+= sum_r_tmp;
    }

    //CALCOLO SOMMA PARZIALE DI OGNI COLONNA
    for(j=0; j<C; sum_c_tmp=0, j++){
        pos=j;
        sum_c_tmp+= cas[pos].x->type[1-cas[pos].rotazione].val;
        colore_tmp= cas[pos].x->type[1-cas[pos].rotazione].color;

        for (i=1, pos=i*C+j; i<R;
             sum_c_tmp+=cas[pos].x->type[1-cas[pos].rotazione].val,
                     i++,
             pos=i*C+j)
            if(colore_tmp!= cas[pos].x->type[1-cas[pos].rotazione].color){
                sum_c_tmp=0;
                break;
            }
        sum_tot+= sum_c_tmp;
    }

    return sum_tot;
}

void printSol(casella * cas, int val, int R, int C){
    int i,j;
    printf("\n\nCONFIGURAZIONE BEST SOL-VAL: %d\n\n", val);
    printf("%d %d\n", R, C);

    for(i=0; i<R; i++, printf("\n"))
        for(j=0; j<C; j++)
            printf("%d/%d ", cas[i*C+j].indicetessera, cas[i*C+j].rotazione);
}