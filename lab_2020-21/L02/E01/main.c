#include <stdio.h>
#define  N 50

typedef  struct  rettangolo{
    int imax,jmax;
    int b, h, area;
} rett;

rett ricercarett(char mat[][N], int i, int j, int nr, int nc);

int main() {
    char mat[N][N];
    int nr , nc , i , j;
    FILE * fp;
    rett bmax, hmax, areamax, tmp;
    bmax.b=0, hmax.h=0, areamax.area=0;

    if((fp=fopen("mappa.txt", "r"))==NULL){
        return 1;
    }

    fscanf(fp,"%d %d", &nr,&nc);
    
    for(i=0; i<nr; i++)
        for(j=0; j<nc; j++)
            fscanf(fp," %c", &mat[i][j]);
    fclose(fp);

    for(i=0; i<nr; i++) {
        for (j = 0; j < nc; j++)
            printf("%c ", mat[i][j]);
        printf("\n");
    }

    for(i=0; i<nr; i++)
        for(j=0; j<nc; j++)
            if(mat[i][j]=='1') {
                tmp = ricercarett(mat, i, j, nr, nc);

                if (tmp.b > bmax.b) bmax = tmp;
                if (tmp.h > hmax.h) hmax = tmp;
                if (tmp.area > areamax.area) areamax = tmp;
            }

    printf("Ecco le matrici:\n");
    printf("1)Max Base,    estr. sup SX=<%d,%d> b=%d, h=%d, Area=%d\n", bmax.imax, bmax.jmax, bmax.b, bmax.h, bmax.area);
    printf("2)Max Area,    estr. sup SX=<%d,%d> b=%d, h=%d, Area=%d\n", areamax.imax, areamax.jmax, areamax.b, areamax.h, areamax.area);
    printf("3)Max Altezza, estr. sup SX=<%d,%d> b=%d, h=%d, Area=%d\n", hmax.imax, hmax.jmax, hmax.b, hmax.h, hmax.area);
    
    return 0;
}

rett ricercarett(char mat[][N], int i, int j, int nr, int nc){
    rett tmp={i, j, 0,0, 0};
    int jtmp=j;

    do{
        mat[i][jtmp]='0'; //cosi da non doverla ricontrollare dopo in altre chiamate di funz.
        tmp.b++;
        jtmp++;
    }while(jtmp<nc && mat[i][jtmp]=='1');

    do{
        mat[i][j]='0';  //stesso commento di sopra
        tmp.h++;
        i++;
    }while(i<nr && mat[i][j]=='1');
    tmp.area=tmp.b*tmp.h;

    return tmp;
}