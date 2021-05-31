#include <stdio.h>
#include <stdlib.h>
#define NOMEFILE "mat.txt"

int ** malloc2dR(FILE *fp, int * nr, int *nc);
void malloc2dP(FILE *fp, int*** punt, int *nr, int *nc);
void separa(int **mat, int nr, int nc, int **bianchi_real , int **neri_real, int * neffbianchi, int * neffneri);

int main(){

    int ** punt;
    int ** punt2;

    FILE * fp;
    int nr, nc, i, j;
    int nr2,nc2;

    int * bianchi;
    int * neri;
    int nneri, nbianchi;

    if((fp=fopen(NOMEFILE, "r"))==NULL)
        return 1;    

    punt=malloc2dR(fp, &nr, &nc);   //malloc matrice con puntatore restituito
    
    rewind(fp);
    malloc2dP(fp, &punt2, &nr2, &nc2); 

    fclose(fp);

    separa(punt, nr, nc, &bianchi, &neri, &nbianchi, &nneri);

    printf("\n\n");
    for(i=0; i<nneri; i++)
        printf("%d\t",neri[i]);

    printf("\n\n");
    for(i=0; i<nbianchi; i++)
        printf("%d\t",bianchi[i]);

    
    for(i=0; i<nr; i++){
        free(punt[i]);
        free(punt2[i]);
    }

    free(punt);
    free(punt2);
    free(bianchi);
    free(neri);

    return 0;
}

int ** malloc2dR(FILE *fp, int * nr, int *nc){
    int i,j;
    int ** punt;

    fscanf(fp,"%d%d", nr, nc);

    punt=(int **)malloc( (*nr) * sizeof(int *) ); //creo il vettore per specificare che conterrà puntatori

    for(i=0; i<*nr; i++)
        punt[i]=(int *)malloc( (*nc) * sizeof(int)); //ad ogni contenitore di puntatore associo un vettore int di 'nc' elementi

    for(i=0; i<*nr; i++)
        for(j=0; j<*nc; j++)
            fscanf(fp,"%d", &(punt[i][j]) );
            
    return punt;
}

void malloc2dP(FILE *fp, int*** punt, int *nr, int *nc){
    int i,j;

    fscanf(fp,"%d%d", nr, nc);

    (*punt)=(int **)malloc( (*nr) * sizeof(int *) ); //creo il vettore per specificare che conterrà puntatori

    for(i=0; i<*nr; i++)
        (*punt)[i]=(int *)malloc( (*nc) * sizeof(int)); //ad ogni contenitore di puntatore associo un vettore int di 'nc' elementi

    for(i=0; i<*nr; i++)
        for(j=0; j<*nc; j++)
            fscanf(fp,"%d", &((*punt)[i][j]) );
            
    return punt;
}

void separa(int **mat, int nr, int nc, int **bianchi_real , int **neri_real, int * neffbianchi, int * neffneri){

    int *bianchi;
    int *neri;
    int nbianchi, nneri;
    int ineri,ibianchi, i,j;

    nbianchi=(nr*nc)/2;
    nneri=(nr*nc)-nbianchi;
    ineri=0;
    ibianchi=0;
    
    neri=(int *)malloc( nneri * sizeof(int));
    bianchi=(int *)malloc( nbianchi * sizeof(int));
    
    for(i=0; i<nr; i++)
        for(j=0; j<nc; j++)
            if( (i+j)%2 == 0 )
                neri[ineri++]=mat[i][j];
            else
                bianchi[ibianchi++]=mat[i][j];

    *bianchi_real=bianchi;
    *neri_real=neri;
    *neffbianchi=ibianchi;
    *neffneri=ineri;
}