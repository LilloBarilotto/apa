#include <stdio.h>
#include <ctype.h>

#define MAXPAROLA 25
#define MAXSEQ 20
#define LENGHTSEQ 5
#define SEQUENZE "sequenze.txt"
#define TESTO "testo.txt"
#define MAXRIGA 200
#define MAXOCCORRENZE 10

void trovaoccorrenze2(FILE *fp, char sequenza[]);

int main() {
    FILE * fpseq;
    FILE * fptesto;
    char seq[MAXSEQ][LENGHTSEQ+1];
    int i,n;

    if((fpseq=fopen(SEQUENZE, "r")) == NULL)
        return -1;

    if((fptesto=fopen(TESTO, "r")) == NULL)
        return -2;

    fscanf(fpseq,"%d",&n);
    for(i=0; i<n; i++)
        fscanf(fpseq,"%s", seq[i]);
    fclose(fpseq);

    for(i=0; i<n; i++){
        trovaoccorrenze2(fptesto,seq[i]);
        rewind(fptesto);
    }

    fclose(fptesto);

    return 0;
}

void trovaoccorrenze2(FILE *fp, char sequenza[]) {
    char riga[MAXRIGA + 1];
    char stringatmp[MAXPAROLA + 1];
    int posizione , occorrenze , i, k, j, jj, lunghezzaseq;

    for (i = 0; sequenza[i] != '\0'; i++);
    lunghezzaseq = i;

    posizione=0;    //conta il numero di parole
    occorrenze=0;   //conta il numero di volte che trovo la sequenza in questo scansione del file

    while (occorrenze<MAXOCCORRENZE && fgets(riga, MAXRIGA + 1, fp) != NULL) {

        i = 0;
        while (riga[i] != '\0' && occorrenze<MAXOCCORRENZE) {

            while (!isalpha(riga[i]) && riga[i] != '\0')
                i++;

            k = 0;
            while (isalpha(riga[i+k])) {
                stringatmp[k] = riga[i+k];
                k++;
            }
            stringatmp[k] = '\0';

            if (k > 0)posizione++;

            for (j = 0, jj = 0; j <= k - lunghezzaseq; j++, jj = 0) {
                while (jj < lunghezzaseq && (tolower(sequenza[jj]) == tolower(stringatmp[j + jj])))
                    jj++;

                if (jj == lunghezzaseq) {
                    if (occorrenze == 0)
                        printf("\n\nEcco la tua sequenza %s nella parola:\n",sequenza);

                    printf("- %s - in posizione %d\n", stringatmp, posizione);
                    occorrenze++;
                    break;
                }
            }

            i = i + k;
        }
    }

    if (occorrenze == 0)
        printf("\n\nNon e' stata trovata la sequenza %s \n\n", sequenza);

}