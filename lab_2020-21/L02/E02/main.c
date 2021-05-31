#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define NOMEFILE "log.txt"
#define MAXS 30
#define MAXCORSE 20


typedef enum{r_date=0, r_partenza, r_capolinea, r_ritardo, r_ritardo_tot, r_fine, r_err} comando_e;

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
void selezionaDati(corsa corse[], int neff, comando_e comando);
void stampacorsa(corsa corse[], int i);

int main() {
    comando_e comando;

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

    do{
        comando=leggicomando();
        selezionaDati(corse, n, comando);
    }while(comando!=r_fine);

    return 0;
}

comando_e leggicomando(){
    char testo[MAXS+1];
    int ntesto;
    printf("\n\nInserisci uno dei seguenti comandi:\n");
    printf("1) 'DATE', per poter trovare tutte le corse effettuate tra 2 date\n");
    printf("2) 'PARTENZA', per poter trovare tutte le corse che partono da una determinata fermata\n");
    printf("3) 'CAPOLINEA', per poter trovare tutte le corse che finiscono in una determinata fermata\n");
    printf("4) 'RITARDO', per elencare tutte le corse che hanno raggiunto la destinazione in ritardo in un certo intervallo di date\n");
    printf("5) 'RITARDO_TOT', per elencare il ritardo complessivo accumulato dalle corse identificate da un certo codice di tratta\n");
    printf("6) 'FINE', per terminare il programma\n");
    printf("Inserisci qui:\t");

    scanf("%s",testo);
    ntesto=strlen(testo);

    for(int i=0; i<ntesto; i++)
        testo[i]=tolower(testo[i]);

    if(strcmp(testo,"date")==0) return r_date;
    else if(strcmp(testo,"partenza")==0) return r_partenza;
    else if(strcmp(testo,"capolinea")==0) return r_capolinea;
    else if(strcmp(testo,"ritardo")==0) return r_ritardo;
    else if(strcmp(testo,"ritardo_tot")==0) return r_ritardo_tot;
    else if(strcmp(testo,"fine")==0) return r_fine;

    return r_err;
}

void selezionaDati(corsa corse[], int neff, comando_e comando){
char data1[11], data2[11], datatmp[11];
char fermata[MAXS+1];
char codice[MAXS+1];
int i, ritardotot=0;

    switch (comando) {
        case r_date:
            printf("Inserisci la prima data (format: 'YYYY/MM/GG'):");
            scanf("%s",data1);
            printf("Inserisci la seconda data (format: 'YYYY/MM/GG'):");
            scanf("%s",data2);

            if(strcmp(data1,data2)>0){
                strcpy(datatmp,data1);
                strcpy(data1,data2);
                strcpy(data2,datatmp);
            }

            for(i=0; i<neff; i++)
                if((strcmp(corse[i].data,data1)>=0) && (strcmp(corse[i].data,data2)<=0))
                    stampacorsa(corse,i);
            break;

        case r_partenza:
            printf("Inserisci la fermata di partenza:");
            scanf("%s",fermata);

            for(i=0; i<neff; i++)
                if((strcmp(corse[i].partenza,fermata)==0))
                    stampacorsa(corse,i);
            break;

        case r_capolinea:
            printf("Inserisci la fermata di destinazione:");
            scanf("%s",fermata);

            for(i=0; i<neff; i++)
                if((strcmp(corse[i].destinazione,fermata)==0))
                    stampacorsa(corse,i);
            break;

        case r_ritardo:
            printf("Inserisci la prima data (format: 'YYYY/MM/GG'):");
            scanf("%s",data1);
            printf("Inserisci la seconda data (format: 'YYYY/MM/GG'):");
            scanf("%s",data2);

            if(strcmp(data1,data2)>0){
                strcpy(datatmp,data1);
                strcpy(data1,data2);
                strcpy(data2,datatmp);
            }

            for(i=0; i<neff; i++)
                if(corse[i].ritardo>0 && (strcmp(corse[i].data,data1)>=0) && (strcmp(corse[i].data,data2)<=0))
                    stampacorsa(corse,i);
            break;

        case r_ritardo_tot:
            printf("\nInserisci il codice tratta:");
            scanf("%s",codice);

            for(i=0; i<neff; i++)
                if((strcmp(corse[i].codice_tratta,codice)==0))
                    ritardotot+=corse[i].ritardo;

                printf("Il ritardo totale accumulato nella tratta %s e': %d minuti",codice, ritardotot);
            break;

        case r_err:
            printf("\n\nErrore inserisci dato valido\n\n");
            break;
        case r_fine:
            printf("\n\nADDEOOO...\n");
    }

    return ;
}

void stampacorsa(corsa corse[], int i){
    printf("%s\t%s\t%s\t\t%s\t%s\t%s\t%d\n",corse[i].codice_tratta, corse[i].partenza , corse[i].destinazione, corse[i].data,corse[i].ora_partenza, corse[i].ora_arrivo, corse[i].ritardo);
    return ;
}