//
// Created by super on 14/01/2021.
//
#include "stock.h"
#include "quote.h"
#include "bst_quote.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAXC 20

struct stock{
    char * titolo;
    int n_trans;
    Bst bst;
};

Stock   STOCKz(char * titolo){
    Stock s= malloc(sizeof(*s));
    s->titolo=strdup(titolo);
    s->n_trans=0;
    s->bst=BSTinit();

    return s;
}
Stock   STOCKinit(FILE *fin){
    Stock s= malloc(sizeof(*s));
    char titolo[MAXC];

    fscanf(fin,"%s %d", titolo, &s->n_trans);
    s->titolo=strdup(titolo);
    s->bst=BSTinit();

    return s;
}
void    STOCKfree(Stock s){
    free(s->titolo);
    BSTfree(s->bst);
    free(s);
}
void    STOCKprint(Stock s, FILE *fout){
    fprintf(fout, "%s", s->titolo);
}
void    STOCKupload(Stock s, int trans, FILE *fin){
    for(int i=0; i<trans; i++)
        BSTadd(s->bst, fin);

    s->n_trans+=trans;
}
int     STOCKkeycmp(Stock s1, Stock s2){
    return strcmp(s1->titolo, s2->titolo);
}
char *  STOCKgetkey(Stock s){
    return s->titolo;
}
int     STOCKgettrans(Stock s){
    return s->n_trans;
}
void STOCKmenu(Stock s, int scelta){
    Data d1, d2 , d3tmp;
    char scritta[]="Inserisci la data nel seguente formato 'yyyy/mm/gg hh:mm':\t";
    Quote q1, q2;
    q1.val=q2.val=0;
    int min=INT_MAX,max=INT_MIN, S;

    switch(scelta) {
        case 2:
            d1=DATAinit();
            printf("DATA-1\n");
            printf(scritta);
            DATAread(d1, stdin);

            q1 = BSTsearch(s->bst, d1);
            if (q1.val == 0)
                printf("Nessuna quotazione trovata in quel giorno");
            else {
                printf("Quotazione trovata:\n");
                QUOTEprint(q1, stdout);
            }
            DATAfree(d1);
            break;

        case 3:
            d1=DATAinit();
            d2=DATAinit();

            printf("DATA-1\n");
            printf(scritta);
            DATAread(d1, stdin);

            printf("DATA-2\n");
            printf(scritta);
            DATAread(d1, stdin);

            if(DATAcmp(d1, d2)>0){
                d3tmp=d1;
                d1=d2;
                d2=d3tmp;
            }
            BSTsearch_QUOTE(s->bst, d1, d2, &q1, 0);// 0 indica min
            BSTsearch_QUOTE(s->bst, d1, d2, &q2, 1);// 1 indica max

            if(q1.val!=0){
                printf("QUOTAZIONE MINIMA:\n");
                QUOTEprint(q1, stdout);
                printf("QUOTAZIONE MASSIMA:\n");
                QUOTEprint(q2, stdout);
            }

            DATAfree(d1);
            DATAfree(d2);
            break;

        case 4:
            d1=BSTsearch_Data(s->bst, 0);
            d2=BSTsearch_Data(s->bst, 1);

            BSTsearch_QUOTE(s->bst, d1, d2, &q1, 0);// 0 indica min
            BSTsearch_QUOTE(s->bst, d1, d2, &q2, 1);// 1 indica max

            if(q1.val!=0){
                printf("QUOTAZIONE MINIMA:\n");
                QUOTEprint(q1, stdout);
                printf("QUOTAZIONE MASSIMA:\n");
                QUOTEprint(q2, stdout);
            }
            break;
        case 5:
            printf("Inserisci soglia S:");
            scanf("%d", &S);
            BSTfind_n_cammino(s->bst, &min, &max);

            if(S < (max/min)){
                printf("La soglia viene superata partiziono tutto\n");
            }
            else
                printf("La soglia non viene superata lascio invariato\n");
    }
}