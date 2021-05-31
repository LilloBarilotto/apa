#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXS 50
#define MAXCODE 5

typedef enum{ins_tastiera=0, ins_file, ricerca, del_cod, del_date,r_fine, r_err, r_stampa} comando_e;

typedef struct date {
    int gg;
    int mm;
    int aaaa;
} Data;

typedef struct anagrafica{
    char codice[MAXCODE+1];
    char cognome[MAXS+1];
    char nome[MAXS+1];
    char via[MAXS+1];
    char citta[MAXS+1];
    int cap;
    Data data;
} Item;

typedef struct node Node;
struct node {
    Item val;
    Node *next;
};

int confronto(Data data1, Data data2);
int leggidafile(char * nomefile, Node **head);
comando_e leggicomando();
void selezionaDati(Node ** header, comando_e comando);
void stampa(Item tmp, FILE * fp);
Item ricerca_code(Node *hp, char * stringa);
Item ricerca_date(Node *hp, Data *data1, Data *data2);
Node * SortListDelDate(Node * h, Data *data1, Data *data2);
Node * SortListDelCode(Node * h, char * string);
Item ITEMsetvoid();
void dealloca_tutto(Node *x);

int main() {
    comando_e comando;
    Node * h=NULL;

    do{
        comando=leggicomando();
        selezionaDati(&h, comando);
    }while(comando!=r_fine);

    dealloca_tutto(h);

    return 0;
}

int confronto(Data data1, Data data2){

    if(data1.aaaa==data2.aaaa) {
        if (data1.mm == data2.mm)
            return data1.gg - data2.gg;
        return data1.mm - data2.mm;
    }
    return data1.aaaa-data2.aaaa;
}

Node * newNode(Item val, Node * next) {
    Node * x =(Node *) malloc(sizeof (Node));
    if (x==NULL)
        return NULL;

    x->val = val;
    x->next = next;

    return x;
}

Node * SortListIns(Node * h, Item val) {
    Node * x, *p;

    if (h==NULL || confronto(val.data,h->val.data)>0)
        return newNode(val, h);

    for (x=h->next, p=h;    x!=NULL && confronto(val.data, x->val.data)<0;   p=x, x=x->next);

    p->next = newNode(val, x);

    return h;
}

int leggidafile(char * nomefile, Node **head){
    FILE *fp;
    Item tmp;

    if((fp=fopen( nomefile, "r"))== NULL)
        return -1;

    while(fscanf(fp,"%s %s %s %d/%d/%d %s %s %d", tmp.codice, tmp.nome, tmp.cognome, &tmp.data.gg, &tmp.data.mm,&tmp.data.aaaa, tmp.via, tmp.citta, &tmp.cap)!=EOF)
        (*head)=SortListIns(*head, tmp);

    fclose(fp);

    return 0;
}

comando_e leggicomando(){
    char testo[MAXS+1];
    int ntesto;
    printf("\n\nInserisci uno dei seguenti comandi:\n");

    printf("1) 'TASTIERA', carica nuovo elemento da tastiera\n");
    printf("2) 'FILE', carica nuovi elementi da file\n");
    printf("3) 'STAMPA', stampa su file o video di tutti gli elementi\n");
    printf("4) 'RICERCA', ricerca, per codice, di un elemento\n");
    printf("5) 'DEL_CODE', cancellazione con estrazione di un elemento dalla lista, previa ricerca per codice\n");
    printf("6) 'DEL_DATE', cancellazione con estrazione di TUTTI gli elementi con data compresa tra 2 inserite\n");
    printf("7) 'FINE', per terminare il programma\n");
    printf("Inserisci qui:\t");

    scanf("%s",testo);
    ntesto=strlen(testo);
    for(int i=0; i<ntesto; i++)
        testo[i]=tolower(testo[i]);

    if(strcmp(testo,"tastiera")==0) return ins_tastiera; //FATTO
    else if(strcmp(testo,"stampa")==0) return r_stampa; //FATTO
    else if(strcmp(testo,"fine")==0) return r_fine;     //FATTO
    else if(strcmp(testo,"file")==0) return ins_file;   // FATTO
    else if(strcmp(testo,"del_code")==0) return del_cod;// IN CORSO
    else if(strcmp(testo,"del_date")==0) return del_date;//FATTO
    else if(strcmp(testo,"ricerca")==0) return ricerca; // FATTO

    return r_err;
}

void selezionaDati(Node ** header, comando_e comando){
    char stringa_cercata[MAXCODE+1];
    char nomefile[MAXS+1];
    int selezionafile=0, count=0;
    Data data1,data2, datatmp;
    Item tmp;

    Node * headtmp=*header;
    FILE * fp;

    switch (comando) {
        case del_cod:
        case ricerca:
                printf("\nInserisci la stringa da cercare per il codice richiesto:");
                scanf("%s", stringa_cercata);

                printf("\nRICERCA LINEARE IN CORSO\n");

                tmp=ricerca_code(headtmp, stringa_cercata);

                if(tmp.data.gg!=0){
                    printf("Trovato l'oggetto\n");
                    stampa(tmp,stdout);

                    if(comando==del_cod)
                        SortListDelCode(*header,stringa_cercata);
                }
                else
                    printf("Non e' stato trovato nessun valore corrispondete al codice cercato");

            break;

        case del_date:
            printf("\nInserisci la data 1:\t");
            scanf("%d/%d/%d", &data1.gg, &data1.mm, &data1.aaaa);

            printf("\nInserisci la data 2:\t");
            scanf("%d/%d/%d", &data2.gg, &data2.mm, &data2.aaaa);

            if(confronto(data1, data2)>0){
                datatmp=data1;
                data1=data2;
                data2=datatmp;
            }

            printf("\nRICERCA LINEARE IN CORSO\n");
            tmp=ricerca_date(headtmp, &data1, &data2);

            while(tmp.data.gg!=0) {
                count++;
                printf("Trovato ed eliminato l'oggetto\n");
                stampa(tmp, stdout);
                SortListDelDate(*header,&data1,&data2);
                tmp=ricerca_date(headtmp, &data1, &data2);
            }

            if(count==0)
                printf("Niente da fare non abbiamo trovato nulla...0..\n");

            break;

        case r_stampa:
                printf("Inserisci il nome del file dove vuoi stampare (oppure 'VIDEO' tutto maiuscolo per stampare a video):\n");
                scanf("%s", nomefile);

                if (strcmp(nomefile, "VIDEO") == 0)
                    fp = stdout;
                else {
                    fp = fopen(nomefile, "w");
                    selezionafile = 1;
                }

                while(headtmp!=NULL){
                    stampa(headtmp->val, fp);
                    headtmp=headtmp->next;
                }

                if (selezionafile == 1)
                    fclose(fp);

            break;

        case ins_file:
            printf("Inserisci il nome del file da dove estrarremo i dati:\n");
            scanf("%s", nomefile);

            leggidafile(nomefile, header);

            break;

        case ins_tastiera:
            printf("Inserisci in ordine CODICE NOME COGNOME gg/mm/aaaa VIA CITTA' CAP:\n");
            scanf("%s %s %s %d/%d/%d %s %s %d", tmp.codice, tmp.nome, tmp.cognome, &tmp.data.gg, &tmp.data.mm,&tmp.data.aaaa, tmp.via, tmp.citta, &tmp.cap);
            (*header)=SortListIns(*header, tmp);

            break;

        case r_err:
            printf("\n\nErrore inserisci dato valido\n\n");
            break;
        case r_fine:
            printf("\n\nADDEOOO...\n");
    }
}

void stampa(Item tmp, FILE * fp){
    fprintf(fp,"%s %s %s %d/%d/%d %s %s %d\n", tmp.codice, tmp.nome, tmp.cognome, tmp.data.gg, tmp.data.mm,tmp.data.aaaa, tmp.via, tmp.citta, tmp.cap);
}

Item ricerca_code(Node *hp, char * stringa){
    Node  *x;

    for (x=hp; x!=NULL; x=x->next)
        if (strcmp(stringa, x->val.codice)==0)
            return x->val;

    return ITEMsetvoid();
}


Item ricerca_date(Node *hp, Data *data1, Data *data2){
    Node  *x;

    for (x=hp; x!=NULL; x=x->next)
        if (confronto(x->val.data, *data1)>=0 && confronto(x->val.data, *data2)<=0)
            return x->val;

    return ITEMsetvoid();
}

Item ITEMsetvoid(){
    Item i;
    i.data.gg=0;

    return i;
}

Node * SortListDelCode(Node * h, char * string) {
    Node * x, *p;
    if (h == NULL) return NULL;
    for (x=h, p=NULL;   x!=NULL && strcmp(x->val.codice, string)!=0;  p=x, x=x->next);

    if (strcmp(x->val.codice, string)==0){
        if (x==h)
            h = x->next;
        else
            p->next = x->next;

        free(x);
    }

    return h;
}

Node * SortListDelDate(Node * h, Data *data1, Data *data2) {
    Node * x, *p;
    if (h == NULL) return NULL;
    for (x=h, p=NULL;   x!=NULL && (confronto(x->val.data, *data1)<0 || confronto(x->val.data, *data2)>0);  p=x, x=x->next);

    if (confronto(x->val.data, *data1)>=0 && confronto(x->val.data, *data2)<=0){
        if (x==h)
            h = x->next;
        else
            p->next = x->next;

        free(x);
    }

    return h;
}

void dealloca_tutto(Node *x){
    if(x==NULL)
        return;
    dealloca_tutto(x->next);
    free(x);
}