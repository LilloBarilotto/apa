#include <stdio.h>


#include "list_stock.h"
#include "stock.h"
#include "date.h"

#define MAXSCELTE   7
#define MAXC        20

char SCELTE[MAXSCELTE][70]={
        "Acquisizione file", "ricerca titolo", "Ricerca quotazione in data",
        "ricerca quotazione massima tra 2 date", "ricerca quotazione massima in tutto il periodo",
        "bilanciamento", "Fine Programma"};


int main() {
    List l=LISTinit();
    Stock s;
    link x;

    int scelta, i, selected_stock=0, n_titoli;
    char nomefile[MAXC];
    char titolo[MAXC];

    FILE * fp;

    do{
        printf("PROGRAMMA TITOLI AZIONARI\n");
        for(i=0; i < MAXSCELTE; i++)
            printf("%d)\t%s\n", i, SCELTE[i]);

        printf("\nInserisci la scelta");
         scanf("%d", &scelta);

        if(scelta<0 || scelta >6){
            printf("Errore scelta non esistente\n");
            continue;
        }

        if(scelta>1 && selected_stock==0){
            printf("Errore nessun titolo selezionato prima..\n");
            continue;
        }

        switch(scelta){
            case 0:
                printf("Inserisci nome file d'acquisizione:\t");
                scanf("%s", nomefile);

                if((fp= fopen(nomefile,"r"))==NULL){
                    printf("Nome file non trovato\n");
                    continue;
                }

                fscanf(fp,"%d", &n_titoli);

                for(i=0; i<n_titoli; i++)
                    LISTadd(l, fp);

                fclose(fp);

                break;

            case 1:
                printf("Inserisci nome titolo:\t");
                scanf("%s", titolo);

                if((x = LISTsearch(l, titolo) )== LISTgetz(l))
                    printf("\nNessun titolo trovato\n");
                else {
                    s = LINKgetitem(x);
                    printf("\nTrovato l'azione in borsa:\t");
                    STOCKprint(s, stdout);
                    printf("\nTi ricordo che da adesso avrai accesso alle altre opzioni del menu\n");
                    selected_stock=1;
                }
                break;

            case 2:
            case 3:
            case 4:
            case 5:
                STOCKmenu(s, scelta);

                break;
            case 6:
                printf("\n\nArrivederci e grazie per il pesce, addeo addeo addeo... cit\n");
        }
    }while(scelta != 6);

    LISTfree(l);

    return 0;
}