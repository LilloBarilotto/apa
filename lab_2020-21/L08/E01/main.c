#include <stdio.h>
#include <stdlib.h>

#define MAXS 30
#define NUMPIETRE 4

typedef enum{z, r, t, s} enum_pietre; //zaffiro, rubini, topazio, smeraldo

typedef struct tipologia{
    enum_pietre  ok1;
    enum_pietre  ok2;
}Tipi;


//RISCRIVILO PRENDENDO IN ESEMPIO L'ES DOPO

void dealloca_tutto( int ** pietra_number, int * max_len,  int num_test);
int leggifile(char * nomefile, int *num_test, int *** p_pietra_numer, int ** p_max_len);
void wrapper (int * pietre_attuale, int * maxlen_attuale, Tipi * tipo);
void set_pietre(Tipi tipo[NUMPIETRE]);
void powerset_r(int * count_type_stone, int count, Tipi * tipo, enum_pietre actual,int *b_val, int *b_sol, int *sol, int * maxlen_attuale);


int main() {
    int i;
    char nomefile[MAXS+1];
    int n_test;
    int ** pietra_number;
    int * max_len;

    Tipi tipo[NUMPIETRE];

    printf("Inserisci il nome del file:\t");
    scanf("%s", nomefile);

    if(leggifile(nomefile, &n_test, &pietra_number, &max_len)==-1)
        return -1;

    set_pietre(tipo);

    for(i=0; i<n_test; i++)
        wrapper(pietra_number[i], &max_len[i], tipo);

    dealloca_tutto(pietra_number, max_len, n_test);
    return 0;
}

int leggifile(char * nomefile, int *num_test, int *** p_pietra_numer, int ** p_max_len){
    int i,j;
    FILE * fp;
    int ** pietra_number_TMP;
    int * max_len_TMP;


    if((fp=fopen(nomefile, "r"))==NULL)
        return -1;

    fscanf(fp,"%d ", num_test);

    pietra_number_TMP=  (int **)malloc( (*num_test) * sizeof(int *));
    max_len_TMP=        (int *)calloc( *num_test, sizeof(int ));

    for(i=0; i<*num_test; i++) {
        pietra_number_TMP[i]=   (int *)malloc( NUMPIETRE * sizeof(int ));

        for (j = 0; j < NUMPIETRE; j++)
            fscanf(fp, "%d", &pietra_number_TMP[i][j]);
    }
    fclose(fp);

    *p_pietra_numer=pietra_number_TMP;
    *p_max_len= max_len_TMP;

    return 0;
}

void dealloca_tutto( int ** pietra_number, int * max_len,  int num_test){
    int i;

    for(i=0; i<num_test; i++)
        free(pietra_number[i]);

    free(pietra_number);
    free(max_len);
}

void wrapper (int * pietre_attuale, int * maxlen_attuale, Tipi * tipo){
    int i, b_val=0;

    int * b_sol;
    int * sol;

    for(i=0; i<NUMPIETRE; i++)
        *maxlen_attuale+=pietre_attuale[i];

    b_sol=malloc( (*maxlen_attuale) * sizeof(int));
    sol=malloc( (*maxlen_attuale) * sizeof(int));

    for(i=0; i<NUMPIETRE && b_val< *maxlen_attuale; i++)
        if(pietre_attuale[i]>0) {
            pietre_attuale[i]--;

            sol[0]=i;
            powerset_r(pietre_attuale, 1, tipo, i, &b_val, b_sol, sol, maxlen_attuale);

            pietre_attuale[i]++;
        }

    for (i = 0;  i< NUMPIETRE; i++)
        printf("%d ", pietre_attuale[i]);

    printf("TOT=%d pietre\n", *maxlen_attuale);

    if(b_val==0)
        printf("Non sono state trovate collane per questo test...\n\n");
    else{
        printf("Abbiamo trovato questa collana di lunghezza %d\n", b_val);
        for(i=0; i<b_val; i++)
            printf("%d", b_sol[i]);

        printf("\n\n");
    }

    free(b_sol);
    free(sol);
}

void powerset_r(int * count_type_stone, int count, Tipi * tipo, enum_pietre actual,int *b_val, int *b_sol, int *sol, int * maxlen_attuale) {
    int i;

    if(*maxlen_attuale==*b_val)
        return;

    if (count > *b_val) {
        *b_val=count;

        for(i=0; i< *b_val; i++)
            b_sol[i]=sol[i];
    }

    if(count_type_stone[tipo[actual].ok1]>0){
        //OTTIMIZZAZIONE PER ADESSO solo aggiungendo questi if (actual==tipo[actual].ok1)
        //sia in questo caso che quello sotto, altrimenti si lascia solo il codice principale che è quello degli else
        if(actual==tipo[actual].ok1){

            for(i=0; count_type_stone[tipo[actual].ok1]>0; i++, count_type_stone[tipo[actual].ok1]--)
                sol[count+i] = actual;

            powerset_r(count_type_stone, count + i, tipo, tipo[actual].ok1, b_val, b_sol, sol, maxlen_attuale);

            count_type_stone[tipo[actual].ok1]+=i;
        }
        else {
            count_type_stone[tipo[actual].ok1]--;

            sol[count] = tipo[actual].ok1;
            //chiamatarecursive passi ok1 come nuovo ultimo valore preso
            powerset_r(count_type_stone, count + 1, tipo, tipo[actual].ok1, b_val, b_sol, sol, maxlen_attuale);

            count_type_stone[tipo[actual].ok1]++;
        }
    }

    if(*maxlen_attuale==*b_val)
        return;

    if(count_type_stone[tipo[actual].ok2]>0) {
        if(actual==tipo[actual].ok2){

            for(i=0; count_type_stone[tipo[actual].ok2]>0; i++, count_type_stone[tipo[actual].ok2]--)
                sol[count+i] = actual;

            powerset_r(count_type_stone, count + i, tipo, tipo[actual].ok2, b_val, b_sol, sol, maxlen_attuale);

            count_type_stone[tipo[actual].ok2]+=i;
        }
        else {
        count_type_stone[tipo[actual].ok2]--;

        sol[count] = tipo[actual].ok2;
        //chiamatarecursive passi ok2 come ultima pietra inserita
        powerset_r(count_type_stone, count + 1, tipo, tipo[actual].ok2, b_val, b_sol, sol, maxlen_attuale);

        count_type_stone[tipo[actual].ok2]++;
        }
    }
}

void set_pietre(Tipi tipo[NUMPIETRE]){
    int i;

    for(i=0; i<NUMPIETRE; i++) {
        if (i == z || i == t) {
            tipo[i].ok1 = z;
            tipo[i].ok2 = r;
        }
        else {
            tipo[i].ok1 = s;
            tipo[i].ok2 = t;
        }
    }
}