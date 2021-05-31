//
// Created by super on 14/01/2021.
//
#include <stdlib.h>
#include "date.h"

struct data{
    int year, month, day;
    int hh, mm;
};

Data    DATAinit(){
    Data d= malloc(sizeof(*d));
    return d;
}
void    DATAread(Data d, FILE *fin){
    fscanf(fin,"%d/%d/%d", &d->year, &d->month, &d->day);
    fscanf(fin,"%d:%d", &d->hh, &d->mm);
}
void    DATAfree(Data d){
    free(d);
}
void    DATAprint(Data d, FILE * fout){
    fprintf(fout, "%4d/%2d/%2d", d->year, d->month, d->day);
}
int     DATAcmp(Data d1, Data d2){
    if(d1->year==d2->year) {
        if (d1->month == d2->month)
            return d1->day - d2->day;
        return d1->month - d2->month;
    }
    return d1->year - d2->year;
}