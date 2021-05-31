//
// Created by super on 14/01/2021.
//
#include <stdlib.h>
#include <stdio.h>
#include "date.h"

#ifndef INC_1_STOCK_H
#define INC_1_STOCK_H

typedef struct stock * Stock;

Stock   STOCKinit(FILE *fin);
void    STOCKfree(Stock s);
void    STOCKprint(Stock s, FILE *fout);
void    STOCKupload(Stock s, int trans, FILE *fin);
void    STOCKsearch(char * nome);
char *  STOCKgetkey(Stock s);
int     STOCKkeycmp(Stock s1, Stock s2);
Stock   STOCKz(char * titolo);
int     STOCKgettrans(Stock s);
void STOCKmenu(Stock s, int scelta);
#endif //INC_1_STOCK_H
