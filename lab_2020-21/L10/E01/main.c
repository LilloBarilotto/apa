#include <stdio.h>
#include <stdlib.h>

typedef struct attivita{
    int s;
    int f;
}att;

int leggifile(char *nome, att ** p_vett, int  * p_n);
void ATTsel(int N, att *v);
void ATTord(int N, att *v);
void stampaSol(att* v, int *P, int i);
void stampaSolNlogN(att* v, int *P, int i);

void ATTord2(int N, att *v);
void MergeSortR(att * A, att * B, int l, int r);
void Merge(att *A, att *B, int l, int q, int r);

int main() {

    att * vett;
    int n;
    char strTMP[50];

    printf("Inserisci nome file:");
    scanf("%s", strTMP);

    if(leggifile(strTMP, &vett, &n)==-1)
        return -1;

    ATTord2(n, vett);
    ATTsel(n, vett);

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

void ATTord(int N, att *v){
    int i,j;
    att tmp;

    for(i=0; i<N-1; i++)
        for(j=0; j<N-i-1; j++)
            if(v[j].f>v[j+1].f || (v[j].f==v[j+1].f && v[j].s> v[j+1].s) ) {
                tmp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = tmp;
            }
}

void ATTord2(int N, att *v){
    int l=0, r=N-1;
    att * tmp= malloc( N * sizeof(att));
    MergeSortR(v, tmp, l, r);
    free(tmp);
}

void MergeSortR(att * A, att * B, int l, int r){
    int q = (l + r)/2;
    if (r <= l)
        return;
    MergeSortR(A, B, l, q);
    MergeSortR(A, B, q+1, r);
    Merge(A, B, l, q, r);
}

void Merge(att *A, att *B, int l, int q, int r) {
    int i, j, k;
    i = l;
    j = q+1;

    for (k = l; k <= r; k++)
        if (i > q)
            B[k] = A[j++];
        else if (j > r)
            B[k] = A[i++];
        else if (A[j].f>A[i].f || (A[j].f==A[i].f && A[j].s> A[i].s) )
            B[k] = A[i++];
        else
            B[k] = A[j++];
    for ( k = l; k <= r; k++ )
        A[k] = B[k];
}

/*
void ATTsel_NlogN(int N, att *v){
    int * opt, * p;
    att *val;
    int i, b_val, last;

    val=    malloc((N+1)*sizeof(att));
    opt=    malloc((N+1)*sizeof(int));
    p=      malloc((N+1)*sizeof(int));

    for(i=0; i<N; i++)
        val[i+1]=v[i];

    val[0].s=0;
    val[0].f=0;
    opt[0]=0;
    p[0]=-1;

    opt[1]=val[1].f - val[1].s;
    p[1]=0;

    for (i=2; i<=N; i++) {
        while()

        if (opt[i-1] > opt[i-2]+val[i])
            opt[i] = opt[i-1];
        else
            opt[i] = opt[i-2] + val[i];
    }

    free(val);
    free(opt);
    free(p);
}
*/

void ATTsel(int N, att *v){
    int * opt, * p;
    int i,j, b_val, last;

    opt=    malloc( N * sizeof(int));
    p=      malloc( N * sizeof(int));

    b_val=opt[0]=v[0].f-v[0].s;
    last=0;
    p[0]=-1;

    for(i=1; i<N; i++) {
        opt[i] = v[i].f - v[i].s;
        p[i] = -1 ;

        for (j = i-1; j>=0; j--)
            if((v[i].s>=v[j].f) && (opt[i]<v[i].f - v[i].s +opt[j])){
                opt[i] = v[i].f - v[i].s +opt[j];
                p[i] = j;
            }

        if(b_val< opt[i]){
            b_val=opt[i];
            last=i;
        }
    }

    stampaSol(v, p, last);

    free(opt);
    free(p);
}

void stampaSol(att* v, int *P, int i){
    if(P[i] == -1){
        fprintf(stdout, "(%d, %d) ", v[i].s, v[i].f);
        return;
    }
    stampaSol(v, P, P[i]);
    fprintf(stdout, "(%d, %d) ", v[i].s, v[i].f);
}

void stampaSolNlogN(att* v, int *P, int i){
    if(P[i] == -1)
        return;
    stampaSolNlogN(v, P, P[i]);
    fprintf(stdout, "(%d, %d) ", v[i].s, v[i].f);
}


/*
int resolve_NlogN(att *v,  int * opt, int  * p, int k){
    if(k==0){
        return opt[k];
    }

    if(k==1){
    }

    opt[k]=v[k].f-v[k].s;

    if(opt[k-1]>opt[k-2])
        if()


    if(v[k].s > v[k-1].f){
        opt[k]+=
    }
}
 */