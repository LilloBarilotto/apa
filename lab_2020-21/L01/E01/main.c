#include <stdio.h>
#define MAX 20
int main() {

    int mat[MAX][MAX];
    FILE *fp;
    int n,m, i,j, sommamax, imax;
    int squadra[MAX]={0};


    if((fp=fopen("mat.txt","r"))==NULL)
        return -1;

    fscanf(fp,"%d %d", &n,&m);  //do per scontato che nel file n ed m siano minori o uguale a max, altrimenti if prima di continuare e in caso contrario  return -2;
    for(i=0; i<n; i++)
        for(j=0; j<m; j++)
            fscanf(fp,"%d",&mat[i][j]);

    fclose(fp);

    for(j=0;j<m;j++) {
        sommamax=0;

        //VERSIONE CHE STAMPA TUTTE LE SQUADRE PRIME CON LO STESSO PUNTEGGIO
        /*
         for (i = 0; i < n; i++) {
            squadra[i] += mat[i][j];
            if(squadra[i]>sommamax)
                sommamax=squadra[i];
        }
        printf("\n\n\n%d giornata\n", j+1);
        printf("Ecco le squadre prime in classifica:\n");

        for (i = 0; i< n; i++)
            if(sommamax==squadra[i])
                printf("Squadra %d\n", i+1);
         */


        //VERSIONE CHE STAMPA SOLO UNA SQUADRA CAPOLISTA, criterio ->la prima che appare nella matrice con il massimo viene salvata, le altre uguali scartate
        for (i = 0; i < n; i++) {
            squadra[i] += mat[i][j];
            if(squadra[i]>sommamax) {
                sommamax = squadra[i];
                imax=i;
            }
        }
        printf("\n\n\n%d giornata\n", j+1);
        printf("Ecco le capolista di questa giornata:\n");
        printf("Squadra %d\n", imax+1);
    }

    return 0;
}
