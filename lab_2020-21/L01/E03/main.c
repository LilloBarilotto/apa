#include <stdio.h>
#include <string.h>

#define N 30

void spostariga(int mat[][N], int i, int dir, int nc, int pos);

void spostacolonna(int mat[][N], int j, int dir, int nr, int pos);

void stampamatrix(int mat[][N], int nr, int nc);


int main() {

    int mat[N][N];
    int riprova, nr, nc, i, j, indice, esci = 0, errore, pos, dir;
    FILE *fp;
    char nomefile[20 + 2]; // il +2 vale per i due caratteri '\0' e '\n'
    char testo[100 + 1];
    char selettore[10];
    char direzione[10];

    //APERTURA FILE
    riprova = 1;
    while (riprova == 1) {

        printf("Inserisci il nome del file da tastiera (max 20 caratteri):\n");
        if (fgets(nomefile, 20 + 2, stdin) == NULL)    //POSSIBILE NOME NON A NORMA
            printf("Errore lettura del nome da tastiera non conforme, riprova \n\n");
        else {
            nomefile[strcspn(nomefile, "\n")] = '\0';

            if ((fp = fopen(nomefile, "r")) == NULL)      //FILE NON ESISTENTE
                printf("File inesistente inserisci nuovamente il nome grazie...\n\n");
            else
                riprova = 0; //FILE TROVATO ED APERTO
        }
    }

    fscanf(fp, "%d %d", &nr, &nc);

    //SCANSIONE MATRICE
    for (i = 0; i < nr; i++)
        for (j = 0; j < nc; j++)
            fscanf(fp, "%d", &mat[i][j]);
    fclose(fp);

    stampamatrix(mat, nr, nc);

    printf("\nInserisci in ordine:\n'selettore' 'indice' 'direzione' 'pos'\n\n");

    while (esci == 0 && fgets(testo, 101, stdin) != NULL) {

        strlwr(testo);
        sscanf(testo, "%s %d %s %d", selettore, &indice, direzione, &pos);

        if (strcmp(selettore, "esci") == 0)
            esci = 1;
        else {
            errore = 0;
            if (strcmp(selettore, "riga") == 0) {

                if (strcmp(direzione, "sinistra") == 0) dir = 1;
                else if (strcmp(direzione, "destra") == 0) dir = -1;
                else errore = 1;

                if (errore == 0)
                    spostariga(mat, indice, dir, nc, pos);
            } else {
                if (strcmp(selettore, "colonna") == 0) {
                    if (strcmp(direzione, "su") == 0) dir = 1;
                    else if (strcmp(direzione, "giu") == 0) dir = -1;
                    else errore = 1;

                    if (errore == 0)
                        spostacolonna(mat, indice, dir, nr, pos);
                } else errore = 1;
            }

            if (errore == 1)
                printf("Guarda che hai sbagliato qualcosa nell'inserimento dati");
            else
                stampamatrix(mat, nr, nc);

            printf("\nInserisci in ordine:\n'selettore' 'indice' 'direzione' 'pos'\n\n");
        }
    }

    printf("\n\nArrivederciiii");

    return 0;
}


void stampamatrix(int mat[][N], int nr, int nc) {
    int i, j;
    for (i = 0; i < nr; i++) {
        for (j = 0; j < nc; j++)
            printf("%d\t", mat[i][j]);
        printf("\n");
    }
}

void spostariga(int mat[][N], int i, int dir, int nc, int pos) {
    int count, tmp, j;

    if (dir == 1)
        for (count = 0; count < pos; count++) {
            tmp = mat[i][0];

            for (j = 0; j < nc - 1; j++)
                mat[i][j] = mat[i][j + 1];

            mat[i][j] = tmp;
        }
    else {
        for (count = 0; count < pos; count++) {
            tmp = mat[i][nc - 1];

            for (j = nc - 1; j > 0; j--)
                mat[i][j] = mat[i][j - 1];
            mat[i][j] = tmp;
        }
    }
}

void spostacolonna(int mat[][N], int j, int dir, int nr, int pos) {
    int count, tmp, i;

    if (dir == 1)
        for (count = 0; count < pos; count++) {
            tmp = mat[0][j];

            for (i = 0; i < nr - 1; i++)
                mat[i][j] = mat[i + 1][j];

            mat[i][j] = tmp;
        }
    else {
        for (count = 0; count < pos; count++) {
            tmp = mat[nr - 1][j];

            for (i = nr - 1; i > 0; i--)
                mat[i][j] = mat[i - 1][j];
            mat[i][j] = tmp;
        }
    }
}