#include <stdio.h>
#define MAXS 30
#define MAXRIGA 200

typedef struct {
    int ricodifica;     //Int occupa 4 byte sempre, in testo invece occuperà minimo 2char per $ ->(opzionali in realtà se si vede poi la soluzione sotto)
                        // e 1 char minimo se solo un numero, personalmente preferisco avere sempre i 4byte dell'intero
    char originale[MAXRIGA]; //non metto MAXRIGA+1 perchè tanto non prende \n usando fscanf e quindi avrei al max 199 caratteri +1 che è \0, metto 200 per il caso limite di una parola che prende tutta la riga
} dictionary;

void codifica(FILE * fpricod, dictionary diz[], char riga[], int n);

int main() {
    int neff, i;    //neff-> numero effettivo record del dizionario
    dictionary dizionario[MAXS];
    char riga[MAXRIGA+1];

    FILE *fpdizionario, *fpsorg, *fpricodificato;

    //APERTURA FILE
    if((fpdizionario=fopen("dizionario.txt", "r")) == NULL)
        return 1;

    //SCANSIONE DEL DIZIONARE DA FILE, le opzioni son due, o metto ' fscanf(fpdizionario, "\n$%d$ %s%",ecc..)'
    // mettendo \n all'inizio della scanf interna al ciclo e togliendo i due %*c, oppure si lascia questo..
    fscanf(fpdizionario, "%d%*c", &neff);
    for( i=0; i<neff; i++)
        fscanf(fpdizionario, "$%d$ %s%*c",&dizionario[i].ricodifica, dizionario[i].originale);
    fclose(fpdizionario);
    //Chiudo subito inutile portare il file aperto fino alla fine..


    //ALTRE APERTURE DEI 2 FILE
    if((fpsorg=fopen("sorgente.txt", "r")) == NULL)
        return 2;
    if((fpricodificato=fopen("ricodificato.txt", "w"))== NULL)
        return 3;

    //PASSO UNA RIGA PER VOLTA, FINO A CHE NON FINISCE IL FILE SORGENTE
    while(fgets(riga, MAXRIGA, fpsorg)!=NULL){
        codifica(fpricodificato, dizionario, riga, neff);
    }

    //PER LA STAMPA ANDATE A CONTROLLARE IL FILE DIRETTAMENTE

    fclose(fpsorg);
    fclose(fpricodificato);

    return 0;
}


void codifica(FILE * fpricod, dictionary diz[], char riga[], int n){
    int i=0, trovato, j,k;

    while(riga[i]!='\n') {
        trovato=0;

        for(j=0; j<n && trovato==0; j++) {
            k=0;

            //Praticamente una strncmp...
            while(diz[j].originale[k]==riga[i+k])
                k++;
            if(diz[j].originale[k]=='\0')
                trovato=1;
        }

        if(trovato==1) { //STAMPO la codifica e sposto i fino alla fine della parola sostituita nella riga
            fprintf(fpricod, "$%d$", diz[j - 1].ricodifica);
            i += k;
        }
        else{ //se non l'ho trovato passo avanti e ricopio la lettera di questo indice
            fputc(riga[i],fpricod);
            i++;
        }
    }
    fputc('\n', fpricod);
}

/*CONSIDERAZIONI FINALI
Ho fatto in questo modo il programma anche pensando "eh se la riga del nuovo file superasse i 200 caratteri?"
Questo dubbio viene guardando l'ottavo record nel file di esempio "sorgente.txt"
il quale contiene "$7$ ll", a 2 char di input ne corrispondo 3 in uscita
so che è un caso ridicolo, ma un file input fatto di sole 60 volte in una riga di sole "ll", da 120 caratteri diventerebbero in uscita 180
Perchè tutta sta chiacchiera? 1- Mi andava. 2- Un'altra soluzione quindi potrebbe essere modificare la riga e stamparla subito senza problemi anzichè stampare per parola o char
MA IN QUESTO ULTIMO CASO LA STRINGA IN OUTPUT PUO' SUPERARE I 200 CARATTERI
*/

//Proprietà di Sebastiano Guzzone, detto Lillo Barilotto, matricola s270855
//Per altre info su telegram ----> https://t.me/lillobarilotto22

/*RIGA 56
FINCHÈ NON HO FINITO LA RIGA, potrei ottimizzare il numero di cicli cercando la lunghezza + piccola nel dizionario
e poi modificando la condizione cosi: //while(riga[i]!='\n' && i<MAXS-lunghezzaminimaparoladizionario)
Onestamente penso che andare a fare una funzione per la lunghezzaminima di una parola del dizionario
e poi andare a cercare la lunghezza effettiva della riga possa alla lunga aiutare ma non cambiare la complessità generale, in caso implemento dopo
*/