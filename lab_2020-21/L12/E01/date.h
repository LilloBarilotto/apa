//
// Created by super on 14/01/2021.
//
#include <stdlib.h>
#include <stdio.h>

#ifndef INC_1_DATE_H
#define INC_1_DATE_H

typedef struct data * Data;

Data    DATAinit();
void    DATAread(Data d, FILE *fin);
void    DATAfree(Data d);
void    DATAprint(Data d, FILE * fout);
int     DATAcmp(Data d1, Data d2);
#endif //INC_1_DATE_H