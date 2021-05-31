#include <stdio.h>
#include <stdlib.h>
#define NOMEFILE "grafo.txt"

typedef  struct archetti{
  int v;
  int u;
}Arco;

int leggidafile(char * nomefile, Arco ** arco, int *n, int *e);
int powerset(int* val, int k, int* sol, int e, Arco * arco);
int powerset_r(int* val, int k, int* sol, int j, int pos, int start, int e, Arco * arco);
int check_vector(int *sol, int e, int n,  Arco * arco);

int main() {
    int i, controllofile,n,e, count;
    Arco *arco;
    int *val, *sol;

    controllofile = leggidafile(NOMEFILE, &arco, &n, &e);

    if (controllofile == -1){
        printf("Errore non e' stato trovato il file di apertura... mi spiace chiudo tutto");
        return -1;
    }

    val=malloc(n * sizeof(int));
    sol=malloc(n * sizeof(int));

    for(i=0; i<n; i++)
        val[i]=i;

    count=powerset(val,n, sol, e, arco);

    if(count>0)
        printf("Come vedi sono stati trovati %d vertex cover\n\n", count);
    else
        printf("Non sono stati trovati vertex cover...\n\n");

    free(val);
    free(sol);
    free(arco);

    return 0;
}

int leggidafile(char * nomefile, Arco ** arco, int *n, int *e){
    FILE *fp;
    int i;
    Arco * arcoTMP;

    if((fp=fopen(nomefile, "r"))==NULL)
        return -1;

    fscanf(fp,"%d%d", n,e);

    arcoTMP=malloc( (*e) * sizeof(arco));

    for(i=0; i<*e; i++)
        fscanf(fp,"%d%d", &arcoTMP[i].u, &arcoTMP[i].v);

    fclose(fp);
    *arco=arcoTMP;

    return 0;
}

int powerset(int* val, int k, int* sol, int e, Arco * arco){
    int count = 0, j;

    for(j = 0; j <= k; j++)
        count += powerset_r(val, k, sol, j, 0, 0, e, arco);

    return count;
}

int powerset_r(int* val, int k, int* sol, int j, int pos, int start, int e, Arco * arco){
    int count = 0, i;

    if (pos == j){
        if(check_vector(sol, e, j, arco)==1) {
            printf("{ ");

            for (i = 0; i < j; i++)
                printf("%d ", sol[i]);

            printf("}\n");

            return 1;
        }
        return 0;
    }

    for (i = start; i < k; i++){
        sol[pos] = val[i];
        count += powerset_r(val, k, sol, j, pos+1, i+1,e, arco);
    }

    return count;
}

int check_vector(int *sol, int e, int n, Arco * arco){
    int i, jj, ok=1;

    for(i=0; ok==1 && i<e; i++){
        ok=0; //arco di default non ha una corrispondenza di estremi con nessun valore di sol

        for(jj=0; jj<n && ok==0; jj++)
            if(sol[jj]==arco[i].u || sol[jj]==arco[i].v)
                ok=1;
    }

    return ok;
}