#include <stdio.h>
#include <stdlib.h>

typedef struct attivita{
    int s;
    int f;
}att;

int leggifile(char *nome, att ** p_vett, int  * p_n);
void attSel(int N, att *v);
void powerset_r(att *v, int k, att *sol, int count, int val, att *b_sol, int * b_count, int * b_val);
int check(att actual, att * sol, int count);

int main() {
    att * vett;
    int n;
    char strTMP[50];

    printf("Inserisci nome file:");
    scanf("%s", strTMP);

    if(leggifile(strTMP, &vett, &n)==-1)
        return -1;

    attSel(n, vett);

    free(vett);
    return 0;
}

int leggifile(char *nome, att ** p_vett, int  * p_n){
    FILE * fp;
    int i;

    if((fp=fopen(nome,"r"))==NULL)
        return -1;

    fscanf(fp, "%d", p_n);

    *p_vett=malloc( (*p_n) * sizeof(att));

    for(i=0; i<(*p_n); i++)
        fscanf(fp, "%d%d", &((*p_vett)[i].s), &((*p_vett)[i].f) );

    return 0;
}

void attSel(int N, att *v){
    att *sol, *b_sol;
    int val, b_val, b_count, i;

    sol=    malloc(N * sizeof(att));
    b_sol=  malloc(N* sizeof(att));

    b_val=b_count=val=0;

    for(i=0; i<N; i++){
        sol[0]=v[i];
        val+=v[i].f - v[i].s;
        powerset_r(v, N, sol, 1, val, b_sol, &b_count, &b_val);
        val-=v[i].f - v[i].s;
    }

    printf("Ecco la combinazione migliore di attivita' che puoi fare.\n");
    printf("Num attivita':\t%d\n", b_count);
    printf("Num ore totali':\t%d\n", b_val);
    printf("Attivita:");
    for(i=0; i<b_count; i++)
        printf("\t(%d , %d)", b_sol[i].s, b_sol[i].f);
    printf("\n");

    free(sol);
    free(b_sol);
}

void powerset_r(att *v, int k, att *sol, int count, int val, att *b_sol, int * b_count, int * b_val){
    int i;

    for(i=count; i<k; i++)
        if(check(v[i], sol, count)){
            sol[count]=v[i];
            val+=v[i].f - v[i].s;
            powerset_r(v, k, sol, count+1, val, b_sol, b_count, b_val);
            val-=v[i].f - v[i].s;
        }

    if(val> *b_val){
        for(i=0; i<count; i++)
            b_sol[i]=sol[i];

        *b_count=count;
        *b_val=val;
    }
}

int check(att actual, att * sol, int count){
    for(int i=0; i<count; i++)
        if( (actual.s < sol[i].f) && (sol[i].s < actual.f) )
            return 0;
    return 1;
}