#include "ST.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct symbtab{
    char ** a; //array di stringhe
    int maxN;
    int size;
};

ST STinit(int maxN){
    ST st;
    int i;

    st = malloc(sizeof( *st));
    st->a = malloc(maxN * sizeof(char *));

    for (i = 0; i < maxN; i++)
        st->a[i]=NULL;
    st->maxN = maxN;
    st->size= 0;

    return st;
}
int STcount(ST st) {
    return st->size;
}
void STfree(ST st) {
    for(int i=0; i< st->size; i++)
        free(st->a[i]);
    free(st->a);
    free(st);
}
void STinsert(ST st, char * val) {
    int i;
    void * punt;

    if(STsearch(st, val)!=-1)
        return;

    i = st->size++;
    if (st->size > st->maxN) {
        punt=realloc(st->a,(2*st->maxN)*sizeof( char *));

        if (punt == NULL)
            return;

        st->a=punt;
        st->maxN = 2*st->maxN;
    }

    while( i>0 && strcmp(val, st->a[i-1])<0){
        st->a[i] = st->a[i-1];
        i--;
    }

    st->a[i] = strdup(val);
}
int STsearch(ST st, char * k) {
    int i;
    if (st->size == 0)
        return -1;
    for (i = 0; i < st->size; i++)
        if (strcmp(k, st->a[i])==0)
            return i;
    return -1;
}
char * STsearchByIndex(ST st, int index) {
    if (st->size == 0)
        return NULL;
    return st->a[index];
}
void STdelete(ST st, char * k) {
    int i, j=0;

    while (strcmp(st->a[j], k)!=0)
        j++;

    if(j< st->size-1)
        free(st->a[j]);

    for (i = j; i < st->size-1; i++)
        st->a[i] = st->a[i+1];
    st->size--;
}
void STdisplay(ST st){
    int i;
    for (i = 0; i < st->size; i++)
        printf("%s",st->a[i]);
}

