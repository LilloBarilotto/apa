#include <stdio.h>
#define  MAXR 50

typedef  struct  rettangolo{
    int imax,jmax;
    int b, h, area;
} rett;

int leggiMatrice(char mat[][MAXR], int maxr, int * nr, int * nc);
int riconosciRegione(char mat[][MAXR], int nr, int nc, int i, int j, int *b, int *h );

int main() {
    char M[MAXR][MAXR]; //metto char giusto per risparmiare byte ma anche perchè alla fine sono tutti 0ed1, se variavano oltre il 9  o minore di 0 mettevo int
    int nr , nc , i , j;
    rett bmax, hmax, areamax, tmp;
    bmax.b=0, hmax.h=0, areamax.area=0;

    if(leggiMatrice(M,MAXR,&nr,&nc))
        return -1;

    for(i=0; i<nr; i++) {
        for (j = 0; j < nc; j++)
            printf("%c ", M[i][j]);
        printf("\n");
    }

    for(i=0; i<nr; i++)
        for(j=0; j<nc; j++) {
            if (riconosciRegione(M, nr,nc,i,j, &tmp.b, &tmp.h)) {
                tmp.area=tmp.b * tmp.h;
                tmp.imax=i;
                tmp.jmax=j;

                printf("Trovato rettangolo in posizione estr. sup SX=<%d,%d> b=%d, h=%d\n", tmp.imax,tmp.jmax,tmp.b,tmp.h);

                if (tmp.b > bmax.b) bmax = tmp;
                if (tmp.h > hmax.h) hmax = tmp;
                if (tmp.area > areamax.area) areamax = tmp;
            }
        }

    printf("\n\nEcco i rettangoli speciali <3:\n");
    printf("1)Max Base,    estr. sup SX=<%d,%d> b=%d, h=%d, Area=%d\n", bmax.imax, bmax.jmax, bmax.b, bmax.h, bmax.area);
    printf("2)Max Area,    estr. sup SX=<%d,%d> b=%d, h=%d, Area=%d\n", areamax.imax, areamax.jmax, areamax.b, areamax.h, areamax.area);
    printf("3)Max Altezza, estr. sup SX=<%d,%d> b=%d, h=%d, Area=%d\n", hmax.imax, hmax.jmax, hmax.b, hmax.h, hmax.area);

    return 0;
}

int leggiMatrice(char mat[][MAXR], int maxr, int * nr, int * nc){
    FILE * fp;
    int i,j;

    if((fp=fopen("mappa.txt", "r"))==NULL)
        return 1;

    fscanf(fp,"%d %d", nr,nc);

    for(i=0; i< *nr; i++)
        for(j=0; j< *nc; j++)
            fscanf(fp," %c", &mat[i][j]);
    fclose(fp);

    return 0;
}

int riconosciRegione(char mat[][MAXR], int nr, int nc, int i, int j, int *b, int *h ){
    int jtmp=j, jj;
    *b=0;
    *h=0;

    if(mat[i][j]=='0')
        return 0;

    //controllo la base iniziale, poi mi servira per capire in ogni riga quanti  char riportare a '0', si assume il rettangolo giusto
    do{
        *b= *b + 1;
        jtmp++;
    }while(jtmp<nc && mat[i][jtmp]=='1');

    do{
        jj=j;
        do{
            mat[i][jj]='0'; //cosi da non dover ricontrollare dopo questa cella in altre chiamate di funz.
            jj++;
        }while(jj<jtmp);

        *h=*h + 1;
        i++;
    }while(i<nr && mat[i][j]=='1');

    return 1;
}