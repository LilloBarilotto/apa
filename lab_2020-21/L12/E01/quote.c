//
// Created by super on 14/01/2021.
//

#include "quote.h"
#include "date.h"

Quote   QUOTEinit(){
    Quote q;
    q.val=0;
    q.num=0;
    q.d=DATAinit();
    return q;
}
Quote   QUOTEread(Quote q, FILE *fin){
    DATAread(q.d, fin);
    fscanf(fin,"%d %d",&q.val, &q.num);

    q.val= q.val * q.num;

    return q;
}
void    QUOTEfree(Quote q){
    DATAfree(q.d);
}
int     QUOTEcmp(Quote q1, Quote q2){
    return DATAcmp(q1.d, q2.d);
}
Data    QUOTEkey(Quote q){
    return  q.d;
}
void    QUOTEprint(Quote q1, FILE *fout){
    DATAprint(q1.d, fout);
    fprintf(fout,"Quotazione del giorno :%f\n", QUOTEqi(q1));
}

int QUOTEseebest(Quote q1, Quote q2, int cmp){
    if(q2.val==0)
        return 1;
    if(cmp==0 && QUOTEqi(q1)< QUOTEqi(q2) )
        return 1;
    if(cmp==1 && QUOTEqi(q1)> QUOTEqi(q2))
        return 1;
    return 0;
}

float QUOTEqi(Quote q){
    return ((float)q.val)/(float)q.num;
}