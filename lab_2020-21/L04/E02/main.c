#include <stdio.h>
#define N 300
#define MAX 10000
int majority( int *a, int n);
int majrecurse(int *a, int l, int r);

int main() {
    int maj,n;
    int v[N];

    int i, cnt = 0;
    char riga[MAX], *s;

    printf("Inserisci la stringa contenente tutti i numeri utili (su un unica rigaaaa):\n");
    fgets(riga,MAX,stdin);
    s=riga;

    while (sscanf(s, "%d%n", &v[cnt], &i)>0) {
        s = s+i; // oppure s = &s[i];
        cnt++;
    }

    n=cnt;
    maj=majority( v, n);

    if(maj==-1)
        printf("Non esiste nessun maggioritario nel vettore");
    else
        printf("Il maggioritario è uguale a %d", maj);

    return 0;
}

int majority( int *a, int n){
    return  majrecurse(a,0, n-1);
}

int majrecurse(int *a, int l, int r){
    int sx,dx, countsx, countdx, m, i,meta;

    if(l==r)
        return a[l];

    countsx=0;
    countdx=0;
    m=(l+r)/2;

    sx=majrecurse(a, l, m);
    dx=majrecurse(a, m+1, r);

    for(i=l; i<=r; i++) {
        if (a[i] == sx)
            countsx++;
        else if (a[i] == dx)
            countdx++;
    }

    meta=(r+1-l)/2;
    if(countsx>meta)
        return sx;

    if(countdx>meta)
        return dx;

    return -1;
}
