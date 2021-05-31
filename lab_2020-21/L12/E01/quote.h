//
// Created by super on 14/01/2021.
//

#ifndef MAIN_C_QUOTE_H
#define MAIN_C_QUOTE_H

#include <stdlib.h>
#include <stdio.h>
#include "date.h"



typedef struct quote Quote;

struct quote{
    Data d;
    int val, num;
};

Quote   QUOTEinit();
Quote   QUOTEread(Quote q, FILE *fin);
void    QUOTEfree(Quote q);
int     QUOTEcmp(Quote q1, Quote q2);
void    QUOTEprint(Quote q1, FILE *fout);
int     QUOTEseebest(Quote q1, Quote q2, int cmp);
float   QUOTEqi(Quote q1);



#endif //MAIN_C_QUOTE_H
