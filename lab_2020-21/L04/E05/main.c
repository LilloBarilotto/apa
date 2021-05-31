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
comando_e leggitag(comando_e special);
void selezionaDati(corsa * corse[][MAXCORSE], int neff, comando_e comando);
void stampacorsa(corsa * corsettatmp, FILE * fp);
void mergesort(corsa * corse[], corsa * corsetmp[], int l, int r, comando_e tipo_ordine);
int confronto(corsa * corsa1, corsa * corsa2, comando_e tipo_ordine);
int ricerca_binaria(corsa * corse[], corsa * corsatmp ,int l, int r, int size, comando_e tag);
int confronto_size(corsa * corsa1, corsa * corsa2, comando_e tipo_ordine, int size);

int main() {
    comando_e comando;
    corsa corse[MAXCORSE];
    corsa * puntord[4][MAXCORSE];// nello stesso ordine del typedef enum comando_e , i valori da 0 a 3
    corsa * punttmp[MAXCORSE];
    int i,n;

    FILE *fp;

    if((fp=fopen(NOMEFILE, "r"))==NULL){
        return 1;
    }

    fscanf(fp,"%d", &n);

    for(i=0; i<n;i++){
        fscanf(fp,"%s %s %s %s %s %s %d",corse[i].codice_tratta, corse[i].partenza , corse[i].destinazione, corse[i].data,corse[i].ora_partenza, corse[i].ora_arrivo, &corse[i].ritardo);
        puntord[0][i]=&corse[i];
        puntord[1][i]=&corse[i];
        puntord[2][i]=&corse[i];
        puntord[3][i]=&corse[i];
    }
    fclose(fp);

    for(i=0; i<4; i++)
        mergesort(puntord[i], punttmp, 0, n-1, (comando_e) i);

    do{
        comando=leggicomando();
        selezionaDati(puntord, n, comando);

    }while(comando!=r_fine);

    return 0;
}

comando_e leggicomando(){
    char testo[MAXS+1];
    int ntesto;
    printf("\n\nInserisci uno dei seguenti comandi:\n");
    printf("1) 'RICERCA', ricerca attraverso i tag delle chiavi secondarie, anche per parziale. Stampa tutte le tratte inerenti\n");
    printf("2) 'STAMPA', stampa su file scelto o a video il vettore\n");
    printf("3) 'FINE', per terminare il programma\n");
    printf("Inserisci qui:\t");

    scanf("%s",testo);
    ntesto=strlen(testo);
    for(int i=0; i<ntesto; i++)
        testo[i]=tolower(testo[i]);

    if(strcmp(testo,"ricerca")==0) return r_ricerca;
    else if(strcmp(testo,"stampa")==0) return r_stampa;
    else if(strcmp(testo,"fine")==0) return r_fine;

    return r_err;
}

comando_e leggitag(comando_e special){
    char testo[MAXS+1];
    int ntesto;
    printf("\nInserisci uno dei seguenti TAG:\n");
    printf("1) 'CODICE', attraverso il codice-tratta\n");
    printf("2) 'PARTENZA', sulla stazione di partenza\n");
    printf("3) 'ARRIVO', sulla stazione di arrivo\n");
    if(special==r_date)
        printf("4) 'DATE', sulle date\n");
    printf("Inserisci qui:\t");

    scanf("%s",testo);
    ntesto=strlen(testo);
    for(int i=0; i<ntesto; i++)
        testo[i]=tolower(testo[i]);

    if(strcmp(testo,"codice")==0) return r_tratta;
    else if(strcmp(testo,"partenza")==0) return r_partenza  ;
    else if(strcmp(testo,"arrivo")==0) return r_arrivo;
    else if(special==r_date && strcmp(testo,"date")==0) return r_date;

    return r_err;
}

void selezionaDati(corsa * corse[][MAXCORSE], int neff, comando_e comando){
    char stringa_cercata[MAXS+1];
    char nomefile[MAXS+1];
    int i, size, selezionafile=0;

    corsa tmp;
    comando_e tag;
    FILE * fp;

    switch (comando) {
        case r_ricerca:
            printf("\nAdesso specifica il tag attraverso il quale vorrai effettuare la ricerca:");
            tag=leggitag(r_err);

            if(tag==r_err)
                printf("Errore nella scelta del campo/tag dove cercare...");
            else {
                printf("\nInserisci la stringa da cercare per il tag richiesto:");
                scanf("%s", stringa_cercata);
                size = strlen(stringa_cercata);

                if(tag==r_tratta)
                    strcpy(tmp.codice_tratta,stringa_cercata);
                else if(tag==r_partenza)
                    strcpy(tmp.partenza,stringa_cercata);
                else
                    strcpy(tmp.destinazione,stringa_cercata);

                printf("\nRICERCA BINARIA IN CORSO\n");
                if (ricerca_binaria(corse[tag], &tmp, 0, neff - 1, size, tag) == 0)
                    printf("\nNessuna corrispondenza trovata mi spiace...\n");
            }
            break;

        case r_stampa:
            printf("\nAdesso specifica il tag per stampare attraverso un determinato ordinamento");
            tag=leggitag(r_date);

            if(tag==r_err)
                printf("Errore nella scelta del campo/tag da stampare...");
            else {
                printf("Inserisci il nome del file dove vuoi stampare (oppure 'VIDEO' tutto maiuscolo per stampare a video):\n");
                scanf("%s", nomefile);

                if (strcmp(nomefile, "VIDEO") == 0)
                    fp = stdout;
                else {
                    fp = fopen(nomefile, "w");
                    selezionafile = 1;
                }
                for (i = 0; i < neff; i++)
                    stampacorsa(corse[(int)tag][i], fp);

                if (selezionafile == 1)
                    fclose(fp);
            }
            break;

        case r_err:
            printf("\n\nErrore inserisci dato valido\n\n");
            break;
        case r_fine:
            printf("\n\nADDEOOO...\n");
    }
}

void stampacorsa(corsa * corsettatmp, FILE * fp){
    fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t%d\n",corsettatmp->codice_tratta, corsettatmp->partenza , corsettatmp->destinazione, corsettatmp->data,corsettatmp->ora_partenza, corsettatmp->ora_arrivo, corsettatmp->ritardo);
}

void mergesort(corsa * corse[], corsa * corsetmp[], int l, int r, comando_e tipo_ordine){
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
        else if (confronto(corse[i],corse[j], tipo_ordine)<=0)
            corsetmp[k] = corse[i++];
        else
            corsetmp[k] = corse[j++];
    for ( k = l; k <= r; k++ )
        corse[k] = corsetmp[k];
}

int confronto(corsa * corsa1, corsa * corsa2, comando_e tipo_ordine){
    switch (tipo_ordine) {
        case r_date:
            if(strcmp(corsa1->data,corsa2->data)==0)
                return strcmp(corsa1->ora_partenza,corsa2->ora_partenza);
            return strcmp(corsa1->data,corsa2->data);

        case r_partenza:
            return strcmp(corsa1->partenza,corsa2->partenza);

        case r_arrivo:
            return strcmp(corsa1->destinazione,corsa2->destinazione);

        case r_tratta:
            return strcmp(corsa1->codice_tratta,corsa2->codice_tratta);
    }
    return 0;
}

int ricerca_binaria(corsa * corse[], corsa * corsatmp ,int l, int r, int size, comando_e tag){
    int m,resultconfronto;

    if (l>r)
        return 0;

    m=(l+ r)/2;

    resultconfronto=confronto_size(corsatmp, corse[m],tag, size);

    if (resultconfronto == 0){
        //stampa in 2 modi, prima partendo dal primo che trovi dopo l fino ad m, e poi una ricerca che procede finche il cmp da zero, poi esce, diversamente dalla lineare che continua a cercare
        while(confronto_size(corse[l], corsatmp, tag, size)<0)
            l++;

        while(l<=m) {
            stampacorsa(corse[l],stdout);
            l++;
        }

        while(l<=r && confronto_size(corsatmp, corse[l], tag, size)==0){
            stampacorsa(corse[l],stdout);
            l++;
        }

        return 1;
    }
    else if (resultconfronto < 0)
        return ricerca_binaria(corse, corsatmp, l, m - 1, size, tag);

    return ricerca_binaria(corse,corsatmp,m+1, r, size, tag);
}

int confronto_size(corsa * corsa1, corsa * corsa2, comando_e tipo_ordine, int size){
    switch (tipo_ordine) {
        case r_partenza:
            return strncmp(corsa1->partenza,corsa2->partenza, size);

        case r_arrivo:
            return strncmp(corsa1->destinazione,corsa2->destinazione, size);

        case r_tratta:
            return strncmp(corsa1->codice_tratta,corsa2->codice_tratta, size);
    }
    return 0;
}