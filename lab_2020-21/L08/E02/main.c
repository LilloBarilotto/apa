#include <stdio.h>
#include <stdlib.h>

#define MAXS 30
#define NUMPIETRE 4
#define NUMPIETREPROSSIME 2
typedef enum{z, r, t, s} enum_pietre; //zaffiro, rubini, topazio, smeraldo

typedef struct tipologia{
    enum_pietre  ok[NUMPIETREPROSSIME];
}Tipi;

void dealloca_tutto( int ** pietra_number, int * max_rip,  int num_test, int ** pietra_val);
int leggifile(char * nomefile, int *num_test, int *** p_pietra_numer, int ** p_max_rip, int *** p_pietra_val);
void wrapper (int * pietre_attuale, int * pietra_val,  int max_rip, Tipi * tipo);
void set_pietre(Tipi tipo[NUMPIETRE]);
void powerset_r(int * count_type_stone, int max_rip, Tipi * tipo, enum_pietre actual, int * pietra_val,
                int *sol, int val, int count,  int *b_sol, int *b_val, int *b_count, int rip_attuali);
int check_z_s(int *sol,int n);

int main() {
    int i;
    char nomefile[MAXS+1];
    int n_test;
    int ** pietra_number;
    int ** pietra_val;
    int * max_rip;

    Tipi tipo[NUMPIETRE];

    printf("VERSIONE 2.0 \n\n");

    printf("Inserisci il nome del file:\t");
    scanf("%s", nomefile);

    if(leggifile(nomefile, &n_test, &pietra_number, &max_rip, &pietra_val)==-1)
        return -1;

    set_pietre(tipo);

    for(i=0; i<n_test; i++)
        wrapper(pietra_number[i], pietra_val[i], max_rip[i], tipo);

    dealloca_tutto(pietra_number, max_rip, n_test, pietra_val);
    return 0;
}

void set_pietre(Tipi tipo[NUMPIETRE]){
    int i;

    for(i=0; i<NUMPIETRE; i++) {
        if (i == z || i == t) {
            tipo[i].ok[0] = z;
            tipo[i].ok[1] = r;
        }
        else {
            tipo[i].ok[0]  = s;
            tipo[i].ok[1]  = t;
        }
    }
}

int leggifile(char * nomefile, int *num_test, int *** p_pietra_numer, int ** p_max_rip, int *** p_pietra_val){
    int i,j;
    FILE * fp;
    int ** pietra_number_TMP;
    int * max_rip_TMP;
    int ** pietra_val_TMP;

    printf("VERSIONE 2.0\n\n");

    if((fp=fopen(nomefile, "r"))==NULL)
        return -1;

    fscanf(fp,"%d ", num_test);

    pietra_number_TMP=  (int **)malloc( (*num_test) * sizeof(int *));
    max_rip_TMP=        (int *)calloc( *num_test, sizeof(int ));
    pietra_val_TMP=     (int **)malloc( (*num_test) * sizeof(int *));


    for(i=0; i<*num_test; i++) {
        pietra_number_TMP[i]=   (int *)malloc( NUMPIETRE * sizeof(int ));
        pietra_val_TMP[i]=      (int *)malloc( NUMPIETRE * sizeof(int ));

        for (j = 0; j < NUMPIETRE; j++)
            fscanf(fp, "%d", &pietra_number_TMP[i][j]);

        for (j = 0; j < NUMPIETRE; j++)
            fscanf(fp, "%d", &pietra_val_TMP[i][j]);

        fscanf(fp, "%d", &max_rip_TMP[i]);
    }

    fclose(fp);

    *p_pietra_numer=pietra_number_TMP;
    *p_max_rip= max_rip_TMP;
    *p_pietra_val=pietra_val_TMP;

    return 0;
}

void dealloca_tutto( int ** pietra_number, int * max_rip,  int num_test, int ** pietra_val){
    int i;

    for(i=0; i<num_test; i++) {
        free(pietra_number[i]);
        free(pietra_val[i]);
    }

    free(pietra_number);
    free(max_rip);
    free(pietra_val);
}

void wrapper (int * pietre_attuale, int * pietra_val,  int max_rip, Tipi * tipo){
    int i, val=0, b_val=0, maxlen=0, b_count;

    int * b_sol;
    int * sol;

    for(i=0; i<NUMPIETRE; i++)
        maxlen+=pietre_attuale[i];

    b_sol=malloc( maxlen * sizeof(int));
    sol=malloc( maxlen * sizeof(int));

    for(i=0; i<NUMPIETRE; i++)
        if(pietre_attuale[i]>0) {
            pietre_attuale[i]--;
            val+=pietra_val[i];

            sol[0]=i;
            powerset_r(pietre_attuale, max_rip, tipo, i, pietra_val, sol, val, 1 , b_sol, &b_val, &b_count, 1);
            pietre_attuale[i]++;
            val-=pietra_val[i];
        }

    for (i = 0;  i< NUMPIETRE; i++)
        printf("%d ", pietre_attuale[i]);

    printf("PIETRE_TOT=%d\t MAX_RIP=%d\n", maxlen, max_rip);

    if(b_val==0)
        printf("Non sono state trovate collane per questo test...\n\n");
    else{
        printf("Collan best val=%d , lunghezza=%d\n", b_val , b_count);
        for(i=0; i<b_count; i++)
            printf("%d", b_sol[i]);

        printf("\n\n");
    }

    free(b_sol);
    free(sol);
}

void powerset_r(int * count_type_stone, int max_rip, Tipi * tipo, enum_pietre actual, int * pietra_val,
                int *sol, int val, int count,  int *b_sol, int *b_val, int *b_count, int rip_attuali){
    int i;

    for(i=0; i<NUMPIETREPROSSIME; i++)
        if(count_type_stone[tipo[actual].ok[i]]>0){
            if(actual==tipo[actual].ok[i])
                rip_attuali++;
            else
                rip_attuali=1;

            if(rip_attuali<=max_rip){
                count_type_stone[tipo[actual].ok[i]]--;
                val+=pietra_val[tipo[actual].ok[i]];

                sol[count] = tipo[actual].ok[i];
                //chiamatarecursive passi ok1 come nuovo ultimo valore preso
                powerset_r(count_type_stone, max_rip, tipo, tipo[actual].ok[i], pietra_val, sol, val, count+1 , b_sol, b_val, b_count, rip_attuali);

                count_type_stone[tipo[actual].ok[i]]++;
                val-=pietra_val[tipo[actual].ok[i]];
            }
        }

    if(val > *b_val && check_z_s(sol,count)<=0){
        *b_val=val;
        *b_count=count;

        for(i=0; i< count; i++)
            b_sol[i]=sol[i];
    }
}

int check_z_s(int *sol,int n){
    int i, count_z, count_s;
    for(i=0, count_s=0, count_z=0; i<n; i++)
        if(sol[i]==z)
            count_z++;
        else if(sol[i]==s)
            count_s++;

    return (count_z - count_s);
}

/*DA OTTIMIZZARE
 * 1- Il codice riga 164-181 e 183-200 è praticamente lo stesso, non so se mettere tutto in un altra funzione per adesso lascio cosi
 * 2- la funzione check_z_s magari potrebbere essere sostituita da 2 valori count_z e count_s che controllano come un promising/pruning
 *      per anticipare se ok1 oppure ok2 sono z ed s, di conseguenza verificare se uno dei 2 darà problemi alla possibile soluzione
 * 3- Oh fratm non so che scrivere ma mettere solo 2 cose da ottimizzare mi pareva troppo da sborone AHAHAHAHAHAH
 * 4- Se non cancello questi commenti è già tanto se arrivo a 18 all'esame.....
 * 5- L'if finale nella ricorsione penso stia bene li, non penso sia utile metterlo all'inizio perchè sarebbe vero inizialmente sempre
 *    e quindi troverei sempre la sovrascrittura del mio b_sol inizialmente, invece teoricamente parlando così dovrebbe sovrascrivere b_sol
 *    già inizialmente con un valore bello alto (e di conseguenza gli if di ricorsioni più "in alto" non verranno "eseguiti", -operazioni da fare)
 */