#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define NOMEFILE "log.txt"
#define MAXS 30
#define MAXCORSE 20

typedef enum{r_date=0, r_partenza, r_arrivo, r_tratta, r_ricerca, r_fine, r_err, r_stampa} comando_e;

typedef struct corsette{
    char codice_tratta[MAXS+1];
    char partenza[MAXS+1];
    char destinazione[MAXS+1];
    char data[11];
    char ora_partenza[MAXS+1];
    char ora_arrivo[MAXS+1];
    int ritardo;
} corsa;

comando_e leggicomando();
void selezionaDati(corsa corse[], int neff, comando_e comando,  comando_e *ultimo_ordine);
void stampacorsa(corsa corsettatmp, FILE * fp);
void mergesort(corsa corse[], corsa corsetmp[], int l, int r, comando_e tipo_ordine);
int confronto(corsa corse[], int i, int j, comando_e tipo_ordine);
int ricerca_binaria(corsa corse[], char codice[],int l, int r, int size);
int ricerca_lineare(corsa corse[], char codice[],int l, int r, int size);

int main() {
    comando_e comando, ultimo_ordine=-1; //-1 se non ordinato, il resto in base al numero specificato in leggicomando
    corsa corse[MAXCORSE];

    int i,n;

    FILE *fp;

    if((fp=fopen(NOMEFILE, "r"))==NULL){
        return 1;
    }

    fscanf(fp,"%d", &n);

    for(i=0; i<n;i++){
        fscanf(fp,"%s %s %s %s %s %s %d",corse[i].codice_tratta, corse[i].partenza , corse[i].destinazione, corse[i].data,corse[i].ora_partenza, corse[i].ora_arrivo, &corse[i].ritardo);
    }
    fclose(fp);

    do{
        comando=leggicomando();

        if(comando!=ultimo_ordine)
            selezionaDati(corse, n, comando, &ultimo_ordine);
        else
            printf("Ho già fatto questo ordinamento l'ultima volta... su non farmi sprecare tempo di esecuzione..");
    }while(comando!=r_fine);

    return 0;
}

comando_e leggicomando(){
    char testo[MAXS+1];
    int ntesto;
    printf("\n\nInserisci uno dei seguenti comandi:\n");
    printf("1) 'DATE', ordinamento del vettore per data, e a parità di date per ora\n");
    printf("2) 'PARTENZA', ordinamento del vettore per stazione di partenza\n");
    printf("3) 'ARRIVO',  ordinamento del vettore per stazione di arrivo\n");
    printf("4) 'TRATTA', ordinamento del vettore per codice di tratta\n");
    printf("5) 'RICERCA', ricerca di una tratta per stazione di partenza, anche per parziale. Stampa tutte le tratte\n");
    printf("6) 'STAMPA', stampa su file scelto o a video il vettore\n");
    printf("7) 'FINE', per terminare il programma\n");
    printf("Inserisci qui:\t");

    scanf("%s",testo);
    ntesto=strlen(testo);
    for(int i=0; i<ntesto; i++)
        testo[i]=tolower(testo[i]);

    if(strcmp(testo,"date")==0) return r_date;
    else if(strcmp(testo,"partenza")==0) return r_partenza;
    else if(strcmp(testo,"arrivo")==0) return r_arrivo;
    else if(strcmp(testo,"tratta")==0) return r_tratta;
    else if(strcmp(testo,"ricerca")==0) return r_ricerca;
    else if(strcmp(testo,"stampa")==0) return r_stampa;
    else if(strcmp(testo,"fine")==0) return r_fine;

    return r_err;
}

void selezionaDati(corsa corse[], int neff, comando_e comando,  comando_e *ultimo_ordine){
    corsa corsettetmp[neff];
    char partenza_cercata[MAXS+1];
    char nomefile[MAXS+1];
    int i, size, selezionafile=0;
    FILE * fp;

    switch (comando) {
        case r_date:
        case r_partenza:
        case r_arrivo:
        case r_tratta:
            *ultimo_ordine=comando;
            mergesort(corse,corsettetmp, 0, neff-1, comando);

            break;
        case r_ricerca:
            printf("\nInserisci la stazione di partenza (anche parziale):");
            scanf("%s",partenza_cercata);
            size=strlen(partenza_cercata);

            if(*ultimo_ordine==r_partenza) {
                printf("\nRICERCA BINARIA IN CORSO\n");
                if(ricerca_binaria(corse, partenza_cercata, 0, neff - 1, size)==0)
                    printf("\nNessuna corrispondenza trovata mi spiace...\n");
            }
            else {
                printf("\nRICERCA LINEARE IN CORSO\n");
                if(ricerca_lineare(corse, partenza_cercata, 0, neff - 1, size)==0)
                    printf("\nNessuna corrispondenza trovata mi spiace...\n");
            }
            break;

        case r_stampa:
            printf("Inserisci il nome del file dove vuoi stampare (oppure 'VIDEO' tutto maiuscolo per stampare a video):\n");
            scanf("%s",nomefile);

            if(strcmp(nomefile,"VIDEO")==0)
                fp=stdout;
            else {
                fp = fopen(nomefile, "w");
                selezionafile=1;
            }
            for(i=0; i<neff; i++)
                stampacorsa(corse[i],fp);

            if(selezionafile==1)
                fclose(fp);
            break;

        case r_err:
            printf("\n\nErrore inserisci dato valido\n\n");
            break;
        case r_fine:
            printf("\n\nADDEOOO...\n");
    }
}

void stampacorsa(corsa corsettatmp, FILE * fp){
    fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t%d\n",corsettatmp.codice_tratta, corsettatmp.partenza , corsettatmp.destinazione, corsettatmp.data,corsettatmp.ora_partenza, corsettatmp.ora_arrivo, corsettatmp.ritardo);
}

void mergesort(corsa corse[], corsa corsetmp[], int l, int r, comando_e tipo_ordine){
    int m, i, j, k;

    if(l==r)
        return;

    m=(l+r)/2;

    mergesort(corse,corsetmp, l, m, tipo_ordine);
    mergesort(corse,corsetmp, m+1, r, tipo_ordine);

    i = l;
    j = m+1;
    for (k = l; k <= r; k++)
        if (i > m)
            corsetmp[k] = corse[j++];
        else if (j > r)
            corsetmp[k] = corse[i++];
        else if (confronto(corse,i,j, tipo_ordine)<=0)
            corsetmp[k] = corse[i++];
        else
            corsetmp[k] = corse[j++];
    for ( k = l; k <= r; k++ )
        corse[k] = corsetmp[k];

}

int confronto(corsa corse[], int i, int j, comando_e tipo_ordine){
    switch (tipo_ordine) {
        case r_date:
            if(strcmp(corse[i].data,corse[j].data)==0)
                return strcmp(corse[i].ora_partenza,corse[j].ora_partenza);
            return strcmp(corse[i].data,corse[j].data);

        case r_partenza:
            return strcmp(corse[i].partenza,corse[j].partenza);

        case r_arrivo:
            return strcmp(corse[i].destinazione,corse[j].destinazione);

        case r_tratta:
            return strcmp(corse[i].codice_tratta,corse[j].codice_tratta);
    }
    return 0; //messo solo per i warning
}
int ricerca_lineare(corsa corse[], char codice[],int l, int r, int size){
    int j=0;

    for(int i=l; i<=r; i++)
        if (strncmp(corse[i].partenza, codice, size)==0) {
            if (j == 0) {
                j++;
                printf("\n\nAbbiamo trovato le tratte:\n");
            }
            stampacorsa(corse[i],stdout);
        }

    return j;
}

int ricerca_binaria(corsa corse[], char codice[],int l, int r, int size){
    int m,confronto;

    if (l>r)
        return 0;

    m=(l+ r)/2;

    confronto= strncmp(codice, corse[m].partenza, size);

    if (confronto==0){
        //stampa in 2 modi, prima partendo dal primo che trovi dopo l fino ad m, e poi una ricerca che procede finche il cmp da zero, poi esce, diversamente dalla lineare che continua a cercare
        while(strncmp(corse[l].partenza, codice, size)<0)
            l++;

        while(l<=m) {
            stampacorsa(corse[l],stdout);
            l++;
        }

        while(l<=r && strncmp(codice, corse[l].partenza, size)==0){
            stampacorsa(corse[l],stdout);
            l++;
        }

        return 1;
    }
    else if (confronto<0)
        return ricerca_binaria(corse, codice, l, m - 1, size);

    return ricerca_binaria(corse,codice,m+1, r, size);
}