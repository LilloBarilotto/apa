#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXE    5
#define MAXDIAG 3
#define DIFFICULT_EXTREME 8

typedef enum {transizione, indietro, avanti} tipologia;
typedef enum {spalle, fronte} direzione;
typedef enum {no_seguito, seguito} precendenza;
typedef enum {FALSE, TRUE} finale;

typedef struct {
    char * nome;
    tipologia tipo;
    direzione ingresso, uscita;
    precendenza prec;
    finale fin;
    float val;
    int difficolta;
    float utile; //appetibile
} element;
typedef struct {
    int indexElement[MAXE];
    int n;
    float val;
    int difficolta;
} diagonal;

int leggifile(FILE *fp, element ** p_elemento);
void elementRead(FILE *fp, element * elemento);
void elementFree(element *elemento);
void listaFree(element * elementi, int n);

void ord_element_appet(element * A, int n);
void merge_sort(element * A, element *B, int l, int r);
void merge(element * A, element * B, int l, int q, int r);
float appet(element * first, element * second);

void greedy_sol(element * elemento, int n, int DD, int DP);
void find_diag_greedy(diagonal * p_diag, element * elemento, int n, int DD, int DP_3,
                      int * p_check_seq2, int * p_check_avanti, int * p_check_indietro);

int compatible_diagonal(diagonal * p_diagTMP, int pos, element * elemento, int i, int DD, int DP_3);
int addAcrob(diagonal * p_diag,int pos, element * elemento, int n, int DD, int DP_3);
int addAvanti(diagonal * p_diag,int pos, element * elemento, int n, int DD, int DP_3);
int addIndietro(diagonal * p_diag,int pos, element * elemento, int n, int DD, int DP_3);
int addElement(diagonal * p_diag,int pos, element * elemento, int n, int DD, int DP_3);

void stampadiag(diagonal * diag, element  * elemento);
void stampa_elemento(element * elemento);

int main() {
    element * elemento;

    int i, n, DD, DP;
    char nomefile[100];
    FILE *fp;

    printf("GREEDY\n");
    printf("Inserisci nome del file:\t");
    scanf("%s", nomefile);

    if((fp=fopen(nomefile,"r"))==NULL)
        return 1;

    n=leggifile(fp, &elemento);
    fclose(fp);

    printf("Inserisci valore DD:\t");
    scanf("%d", &DD);

    printf("Inserisci valore DP:\t");
    scanf("%d", &DP);

    ord_element_appet(elemento, n);

    for(i=0; i<n; i++)
        stampa_elemento(&elemento[i]);

    greedy_sol(elemento, n, DD, DP);

    listaFree(elemento, n);

    return 0;
}

int leggifile(FILE * fp, element ** p_elemento){
    int i, n;
    element * elementTMP;

    fscanf(fp,"%d",&n);

    elementTMP=malloc(n * sizeof(element));

    for(i=0; i<n; i++)
        elementRead(fp, &elementTMP[i]);

    *p_elemento=elementTMP;
    return n;
}
void elementRead(FILE *fp, element * elemento){
    char stringaTMP[100];

    fscanf(fp, "%s ", stringaTMP);
    elemento->nome=strdup(stringaTMP);

    fscanf(fp, "%d %d %d %d %d %f %d", &elemento->tipo, &elemento->ingresso, &elemento->uscita, &elemento->prec, &elemento->fin, &elemento->val, &elemento->difficolta);

    elemento->utile=elemento->val/(float)elemento->difficolta;
    if(elemento->tipo!= transizione)
        elemento->utile+=10000;

    if(elemento->fin== FALSE)
        elemento->utile+=100000;
}
void elementFree(element *elemento){
    free(elemento->nome);
}
void listaFree(element * elementi, int n){
    int i;
    for(i=0; i<n; i++)
        elementFree(&elementi[i]);

    free(elementi);
}

void ord_element_appet(element * A, int n){
    element * B= malloc(n * sizeof(element));
    merge_sort(A ,B, 0, n-1);
    free(B);
}
void merge_sort(element * A, element *B, int l, int r){
    int q;

    if(r<=l)
        return;

    q=(l+r)/2;
    merge_sort(A,B,l,q);
    merge_sort(A,B,q+1,r);
    merge(A,B, l, q, r);
}
void merge(element * A, element * B, int l, int q, int r){
    int i,j,k;

    i=l;
    j=q+1;

    for(k=l; k<=r; k++)
        if(i>q)
            B[k]=A[j++];
        else if (j>r)
            B[k]=A[i++];
        else if (appet(&A[i],&A[j])>=0)
            B[k]=A[i++];
        else
            B[k]=A[j++];

    for(k=l; k<=r; k++)
        A[k]=B[k];
}
float appet(element * first, element * second){
    if(first->utile == second->utile) {
        if (first->fin == FALSE)
            return 1; //preferisco mettere prima un elemento non finale
        return -1;
    }
    return first->utile - second->utile;
}

void greedy_sol(element * elemento, int n, int DD, int DP){
    diagonal diag[MAXDIAG];
    int i, check_seq2, check_avanti, check_indietro;
    float val;

    check_seq2=check_avanti=check_indietro=0;
    val=0;

    for(i=0; i<MAXDIAG; i++) {
        find_diag_greedy(&diag[i], elemento, n, DD, DP / 3, &check_seq2, &check_avanti, &check_indietro);
        val+=diag[i].val;
    }

    printf("\n\nSTAMPA PROGRAMMA CON VALORE:\t%.3f\n", val);

    for(i=0; i<MAXDIAG; i++)
        stampadiag(&diag[i], elemento);

    printf("\n\n\nCiao <3\n");
}
void find_diag_greedy(diagonal * p_diag, element * elemento, int n, int DD, int DP_3,
                      int * p_check_seq2, int * p_check_avanti, int * p_check_indietro){
    int check_acrob=0, i, resAcrob;
    p_diag->difficolta=0;

    for(i=0; i<MAXE && p_diag->difficolta<DD && p_diag->difficolta<DP_3; i++){
        if(i>0) {
            if (elemento[p_diag->indexElement[i - 1]].fin == TRUE)
                break;
            if(*p_check_seq2==0 && elemento[p_diag->indexElement[i - 1]].tipo!=transizione && ( resAcrob= addAcrob(p_diag, i, elemento, n, DD, DP_3)) ){
                *p_check_seq2=1;
                if(resAcrob== avanti)
                    *p_check_avanti=1;
                else
                    *p_check_indietro=1;
                continue;
            }
        }
        if (check_acrob == 0 && ( resAcrob = addAcrob(p_diag, i, elemento, n, DD, DP_3))){
            check_acrob = 1;
            if(resAcrob == avanti)
                *p_check_avanti=1;
            else
                *p_check_indietro=1;
            continue;
        }

        if (*p_check_avanti == 0 && addAvanti(p_diag, i, elemento, n, DD, DP_3)){
            *p_check_avanti = 1;
            continue;
        }

        if(*p_check_indietro == 0 && addIndietro(p_diag, i, elemento, n, DD, DP_3)){
            *p_check_indietro = 1;
            continue;
        }

        if(addElement(p_diag, i, elemento, n, DD, DP_3)==0) //significa che non posso piu' aggiungere  nulla
            break;
    }

    p_diag->n=i;
}

int addAcrob(diagonal * p_diag,int pos, element * elemento, int n, int DD, int DP_3){
    int i;

    for(i=0; i<n; i++) {
        if (elemento[i].tipo!= transizione  && compatible_diagonal(p_diag, pos, elemento, i, DD, DP_3)){
            p_diag->indexElement[pos]=i;
            p_diag->difficolta+=elemento[i].difficolta;
            p_diag->val+=elemento[i].val;
            return elemento[i].tipo;
        }
    }
    return 0;
}
int addAvanti(diagonal * p_diag,int pos, element * elemento, int n, int DD, int DP_3){
    int i;

    for(i=0; i<n; i++) {
        if (elemento[i].tipo== avanti  && compatible_diagonal(p_diag, pos, elemento, i, DD, DP_3)){
            p_diag->indexElement[pos]=i;
            p_diag->difficolta+=elemento[i].difficolta;
            p_diag->val+=elemento[i].val;

            return 1;
        }
    }
    return 0;
}
int addIndietro(diagonal * p_diag,int pos, element * elemento, int n, int DD, int DP_3){
    int i;

    for(i=0; i<n; i++) {
        if (elemento[i].tipo== indietro  && compatible_diagonal(p_diag, pos, elemento, i, DD, DP_3)){
            p_diag->indexElement[pos]=i;
            p_diag->difficolta+=elemento[i].difficolta;
            p_diag->val+=elemento[i].val;

            return 1;
        }
    }
    return 0;
}
int addElement(diagonal * p_diag,int pos, element * elemento, int n, int DD, int DP_3){
    int i;

    for(i=0; i<n; i++) {
        if (compatible_diagonal(p_diag, pos, elemento, i, DD, DP_3)){
            p_diag->indexElement[pos]=i;
            p_diag->difficolta+=elemento[i].difficolta;
            p_diag->val+=elemento[i].val;

            return 1;
        }
    }
    return 0;
}
int compatible_diagonal(diagonal * p_diagTMP, int pos, element * elemento, int i, int DD, int DP_3){
    if(pos==0) {
        if (elemento[i].prec == no_seguito &&
            elemento[i].ingresso == fronte &&
            elemento[i].difficolta <= DD &&
            (elemento[i].difficolta + p_diagTMP->difficolta) <=DP_3)
            return 1;
        return 0;
    }

    if( elemento[ p_diagTMP->indexElement[pos-1] ].fin==FALSE &&
        elemento[ p_diagTMP->indexElement[pos-1] ].uscita == elemento[i].ingresso  &&
        (elemento[i].difficolta + p_diagTMP->difficolta) <=DD &&
        (elemento[i].difficolta + p_diagTMP->difficolta) <=DP_3)
        return 1;

    return 0;
}

void stampadiag(diagonal * diag, element  * elemento){
    int i;
    for(i=0; i< diag->n; i++)
        stampa_elemento(&elemento[diag->indexElement[i]]);
    printf("\n");
}
void stampa_elemento(element * elemento){
    printf("%s %.3f\t\n",elemento->nome, elemento->val);
}