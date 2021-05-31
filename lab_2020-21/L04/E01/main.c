#include <stdio.h>

int gcd(int a, int b);

int main() {
    int a,b;
    printf("Inserisci il primo numero:\t");
    scanf("%d", &a);
    printf("Inserisci il secondo numero:");
    scanf("%d", &b);

    if(a==0 || b==0)
        printf("Errore  non si fanno queste cose..");
    else
        printf("\n\nIl massimo comun divisore e' : \t%d", gcd(a,b));

    return 0;
}

int gcd(int a, int b){
    int tmp;

    if(a<b){
        tmp=a;
        a=b;
        b=tmp;
    }

    if(a%b==0)
        return b;

    if(a%2 == 0  && b%2==0)
        return 2*gcd(a/2,b/2);

    if( a%2==1 && b%2==0)
        return gcd(a,b/2);

    if( a%2==1 && b%2==1)
        return gcd((a-b)/2, b);

    return 1;
}