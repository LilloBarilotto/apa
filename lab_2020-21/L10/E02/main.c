#include <stdio.h>
#include <stdlib.h>

#define NUMPIETRE 4

//typedef enum{z, r, t, s} enum_pietre; //zaffiro, rubini, topazio, smeraldo

// z o t ->z,r
// s o r ->s,t

//collana come: gemma attuale + max(collana[set tutte gemme -1 specifica tipo1], collana[set gemme -1spec tipo2]

/*mat  inizio [0,1,2,3][0,...,z][0,...,r][0,...,s][0,...,t]
 * fZ,fT->fZ,fR
 * fS,fR->fS,fT
*/

void free5D(int ***** mat, int z, int r, int t);
int ***** mat_init_inizialize(int z, int r, int t, int s);
int result_collana(int z, int r, int t, int s);
int max(int a, int b);
int fZ(int *****m, int z, int r, int t, int s);
int fT(int *****m, int z, int r, int t, int s);
int fS(int *****m, int z, int r, int t, int s);
int fR(int *****m, int z, int r, int t, int s);


int main() {
    char nome[100];
    FILE *fp;
    int *z, *r, *t, *s, n, i, ris;

    printf("Inserisci nome file:\t");
    scanf("%s", nome);

    if ((fp = fopen(nome, "r")) == NULL)
        return 1;

    fscanf(fp,"%d", &n);
    z = malloc(n * sizeof(int));
    r = malloc(n * sizeof(int));
    t = malloc(n * sizeof(int));
    s = malloc(n * sizeof(int));

    for (i = 0; i < n; i++)
        fscanf(fp,"%d %d %d %d", &z[i], &r[i], &t[i], &s[i]);
    fclose(fp);

    for (i = 0; i < n; i++) {
        ris = result_collana(z[i], r[i], t[i], s[i]);

        printf("TEST #%d\nla collana con %d zaffiri, %d rubini, %d topazi, %d smeraldi\n e' lunga al massimo %d\n",
               i+1,z[i],r[i],t[i],s[i], ris);
    }
    free(z);
    free(r);
    free(s);
    free(t);
}


int result_collana(int z, int r, int t, int s){
    int ***** mat=mat_init_inizialize(z+1,r+1,t+1,s+1);
    int ris=0, Z, R, S, T;

    Z=fZ(mat,z, r, t, s);
    if(ris<Z)
        ris=Z;

    R=fR(mat,z, r, t, s);
    if(ris<R)
        ris=R;


    T=fT(mat,z, r, t, s);
    if(ris<T)
        ris=T;

    S=fS(mat,z, r, t, s);
    if(ris<S)
        ris=S;

    free5D(mat, z+1, r+1, t+1);

    return ris;
}

int ***** mat_init_inizialize(int z, int r, int t, int s){
    int ***** mat;
    int x,i,j,k,l;

    mat= malloc( NUMPIETRE * sizeof(int *****));

    for(x=0; x<NUMPIETRE; x++) {
        mat[x] = malloc(z * sizeof(int ***));
        for (i = 0; i < z; i++) {

            mat[x][i] = malloc(r * sizeof(int **));
            for (j = 0; j < r; j++) {

                mat[x][i][j] = malloc(t * sizeof(int *));
                for (k = 0; k < t; k++) {

                    mat[x][i][j][k] = malloc(s * sizeof(int));
                    for (l = 0; l < s; l++)
                        mat[x][i][j][k][l] = -1;
                }
            }
        }
    }

    return mat;
}
void free5D(int ***** mat, int z, int r, int t){
    int x,i,j,k;

    for(x=0; x<NUMPIETRE; free(mat[x]), x++)
        for(i=0; i<z; free(mat[x][i]), i++)
            for(j=0; j<r; free(mat[x][i][j]), j++)
                for(k=0; k<t; free(mat[x][i][j][k]), k++);

    free(mat);
}

int fZ(int *****m, int z, int r, int t, int s){
    int maxcollanaprec;

    if(z<=0)
        return 0;

    if(m[0][z][r][t][s]!=-1)
        return  m[0][z][r][t][s];


    z--;
    maxcollanaprec= max(fZ(m,z, r, t, s), fR(m,z,r,t, s));
    z++;
    m[0][z][r][t][s]=1 + maxcollanaprec;

    return  m[0][z][r][t][s];
}

int fT(int *****m, int z, int r, int t, int s){
    int maxcollanaprec;
    if(t<=0)
        return 0;

    if(m[2][z][r][t][s]!=-1)
        return  m[2][z][r][t][s];

    t--;
    maxcollanaprec= max(fZ(m,z, r, t, s), fR(m,z,r,t, s));
    t++;
    m[2][z][r][t][s]=1 + maxcollanaprec;

    return  m[2][z][r][t][s];
}

int fS(int *****m, int z, int r, int t, int s){
    int maxcollanaprec;
    if(s<=0)
        return 0;

    if(m[3][z][r][t][s]!=-1)
        return  m[3][z][r][t][s];

    s--;
    maxcollanaprec= max(fT(m,z, r, t, s), fS(m,z,r,t, s));
    s++;
    m[3][z][r][t][s]=1 + maxcollanaprec;

    return  m[3][z][r][t][s];
}

int fR(int *****m, int z, int r, int t, int s){
    int maxcollanaprec;
    if(r<=0)
        return 0;

    if(m[1][z][r][t][s]!=-1)
        return  m[1][z][r][t][s];

    r--;
    maxcollanaprec=max(fT(m,z, r, t, s), fS(m,z,r,t, s));
    r++;
    m[1][z][r][t][s]=1 + maxcollanaprec;

    return  m[1][z][r][t][s];
}

int max(int a, int b){
    if(a>b)
        return a;
    return  b;
}