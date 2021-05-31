#ifndef INC_3_ST_H
#define INC_3_ST_H



typedef struct symbtab * ST;

ST      STinit(int maxN);
int     STcount(ST st);
void    STfree(ST st);
void    STinsert(ST st, char * val);
int     STsearch(ST st, char * k);
char *  STsearchByIndex(ST st, int index);
void    STdelete(ST st, char * k);
void    STdisplay(ST st);


#endif //INC_3_ST_H