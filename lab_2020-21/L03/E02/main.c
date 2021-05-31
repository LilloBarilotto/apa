#include <stdio.h>

//IL PROGRAMMA RICEVE UN FLOAT E STAMPA LA CODIFICA IN BINARIO, separando segno esponente e mantissa
void stampaCodifica (void *p, int size, unsigned int bigEndian);
int b_padding(unsigned int bigEndian);

int main() {
    float af;
    double ad;
    long double ald;

    unsigned int bigEndian=0x00000001; //big se: 0x00, little se :0x01

    if( *((unsigned char *)&bigEndian)==0x01) {
        bigEndian = 0;
        printf("Il calcolatore e' little endian uff...\n\n");
    }
    else printf("BIGENDIAAAN YEAAAH\n\n");

    printf("Inserisci un float:\t");
    scanf("%Lf", &ald);

    af=(float)ald;
    ad=(double)ald;

    /* SE DA PROBLEMI OVVIAMENTE SOSTITUIRE CON IL SCANF FLOAT E POI DA LI IL CAST, ANCHE SE NON SI AVRA LA MASSIMA PRECISIONE IN GENERALE
     scanf("%f", &af);
     ad=(double)af;
     ald=(long double)af;
     */

    stampaCodifica((void *)&af,sizeof(af),bigEndian);
    stampaCodifica((void *)&ad,sizeof(ad),bigEndian);
    stampaCodifica((void *)&ald,sizeof(ald),bigEndian);

    return 0;
}


void stampaCodifica (void *p, int size, unsigned int bigEndian){
    unsigned char * bit= (unsigned char *)p;
    unsigned char byte, vet[8];
    int  i, count, contavett, direzione, dimesponente, padding;
    if(bigEndian==0){
            //LITTLE-ENDIAN
            // float per esempio  0x00000000 00000000 00000000 00000000-> mantissa23  esponente8 sign1
            //                  24-31bit=(1segno + 7bit exp)  16-23=(1bit exp +7bit mantissa) 8-15=(prossimi 8 bit mantissa) 0-7=(ultimi 8bitmantissa)
        bit+= size-1; //dovrei avere il byte con bit 24-31, sempre considerando il float
        direzione=-1;
    }
    else
        direzione=1; // non devo cambiare bit, alla fine sta già

    if(size==sizeof(float)){
        printf("\n\nCODIFICA FLOAT, size byte=%d, size bit=%d\n",size,size*8);
        dimesponente=9;
    }
    else if(size==sizeof(double)){
        printf("\n\nCODIFICA DOUBLE, size byte=%d, size bit=%d\n",size,size*8);
        dimesponente=12;
    }
    else{
        printf("\n\nCODIFICA LONG DOUBLE, size byte=%d, size bit=%d\n",size,size*8);
        dimesponente=16;
        padding=b_padding(bigEndian);
        bit=bit+(direzione*padding);
        size-=padding;
        printf("size byte padding=%d, size in bit padding=%d, size utile in byte del long double=%d\n",padding, padding*8,size);
    }

    for(count=0,i=0, byte=*bit; i<size; i++, bit+=direzione, byte= *bit){
        contavett=0;

        while (byte>0){
                vet[contavett]=byte%2+'0';
                contavett++;
                byte=byte/2;
        }

        while(contavett<8){
            vet[contavett]='0';
            contavett++;
        }
        contavett--;

        while(contavett>=0){
            if(count==1 || count==dimesponente)
                printf("\t");
            printf("%c",vet[contavett]);
            contavett--;
            count++;
        }
    }
}

int b_padding(unsigned int bigEndian){ //valido solo per little_endian AL MOMENTO, si ringrazia Vito per avermi aiutato con questo code
    int nbytereali= 0;
    long double x, y;
    unsigned char *p1 = (unsigned  char *)&x, *p2 = (unsigned  char *)&y;
    x = 1;
    y = -x;

    if(bigEndian==0)//LITTLE-ENDIAN, devo trovare il padding trovando prima i byte totali reali, e poi sottraendoli
        while(*p1 == *p2){
        p1++;
        p2++;
        nbytereali++;
        }
    else{//BIG-ENDIAN, quindi parto dalla fine per arrivare al byte reale + significativo
        p1+=sizeof(long double)-1;
        p2+=sizeof(long double)-1;
        while(*p1 == *p2){
            p1--;
            p2--;
            nbytereali++;
        }
    }

    return sizeof(long double) - nbytereali - 1;
}