#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NOMEFILE "brani.txt"
#define MAXSTRING 256 //255+1 per /0
#define OUTPUT  "output.txt"

void stamparecursive(char **punt_string_sol, int *punt_cont_Ci, char *** punt_string_A, int l, int r, FILE * fpout);

int main(){
    int i,j,A,combtot=0;
    FILE * fp;
    FILE * fpout;
    int *punt_cont_Ci;

    char *** punt_string_A;
    char punt_string_tmp[MAXSTRING];
    char ** punt_string_sol;

    if((fp=fopen(NOMEFILE,"r"))==NULL)
        return 1;

    fscanf(fp,"%d",&A);

    punt_cont_Ci =(int *)calloc(A , sizeof(int) );      //num 'A' interi per contare le Ci di ognuno
    punt_string_A =(char ***)malloc( A * sizeof(char **) );  //num 'A' vettori di puntatori, per ognuno di questi ci saranno Ci canzoni
    
    for(i=0; i<A; i++){
        fscanf(fp,"%d", &punt_cont_Ci[i]);
        
        punt_string_A[i]=(char **)malloc( punt_cont_Ci[i] * sizeof(char *));

        for(j=0; j<punt_cont_Ci[i] ; j++) {
            fscanf(fp, "%s", punt_string_tmp);
            punt_string_A[i][j]=strdup(punt_string_tmp);
        }
    }
    fclose(fp);


    for(i=0; i<A; i++){
        printf("\n\nAmico %d numero canzoni %d\n", i, punt_cont_Ci[i] );

        for(j=0; j<punt_cont_Ci[i]; j++)
            printf("%s\n", punt_string_A[i][j]);
    }

    combtot=*(punt_cont_Ci);

    for(i=1;i<A; i++)
        combtot= combtot * punt_cont_Ci[i];

    printf("Le combinazioni totali sono %d", combtot);

    if((fpout=fopen(OUTPUT,"w"))==NULL)
        return 1;


    //altra versione sicuramente migliore da 1000 punti di vista, ma mi piaceva esercitarmi cosi per adesso
    //mi bastava salvare A puntatori che puntano all'inizio di ogni stringa
    //senza fare i strcpy, oppure fare direttamente i strdup
    punt_string_sol =(char **)malloc( A * sizeof(char *)); // num 'A' puntatori per le soluzioni

    for(i=0; i<A; i++)
        punt_string_sol[i] =(char *)malloc( MAXSTRING * sizeof(char));

    stamparecursive(punt_string_sol, punt_cont_Ci, punt_string_A, 0, A-1, fpout);

    fclose(fpout);

    for(i=0; i<A; i++){
        free(punt_string_sol[i]);

        for(j=0; j<punt_cont_Ci[i]; j++)
            free(punt_string_A[i][j]);

        free(punt_string_A[i]);
    }

    free(punt_cont_Ci);
    free(punt_string_A);
    free(punt_string_sol);

    return 0;
}

void stamparecursive(char **punt_string_sol, int *punt_cont_Ci, char *** punt_string_A, int l, int r, FILE * fpout){
    int i,j;

    if(l>r){
        for(j=0; j<=r; j++)
            fprintf(fpout,"Amico %d song: %s\n", j, punt_string_sol[j]);
        fprintf(fpout,"\n\n");

        return;
    }

    for(i=0;  i<punt_cont_Ci[l]; i++){
        strcpy(punt_string_sol[l], punt_string_A[l][i] );
        stamparecursive(punt_string_sol, punt_cont_Ci, punt_string_A, l+1, r, fpout);
    }
}