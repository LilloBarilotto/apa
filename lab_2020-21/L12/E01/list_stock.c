//
// Created by super on 15/01/2021.
//

#include "list_stock.h"
#include <string.h>

struct node {
    Stock s;
    link next;
};
struct list{
    link head;
    link z;
    int size;
};

static link NEW(Stock s, link next){
    link x = malloc (sizeof *x);
    if (x == NULL)
        return NULL;
    x->s = s;
    x->next = next;

    return x;
}
void   NODEfree(link x){
    STOCKfree(x->s);
    free(x);
}

Stock LINKgetitem(link x){
    return x->s;
}//da usare al posto di passare x->s alla chiamata di  STOCKupload o nel main
link LISTgetz(List l){
    return l->z;
}

List LISTinit(){
    List l=malloc(sizeof(*l));

    l->size=0;
    l->head= ( l->z = NEW( STOCKz("") , NULL));

    return l;
}
void LISTinsert(List l, Stock s){
    link x, p;
    if ((l->head == l->z) || STOCKkeycmp(s, l->head->s)< 0)
            l->head = NEW(s, l->head);
    else {
            for (x = l->head->next, p = l->head;
                 x != l->z && STOCKkeycmp(s, x->s)> 0;
                 p = x, x = x->next);
            p->next = NEW(s, x);
        }
    l->size++;
}
void LISTadd(List l, FILE *fin){
    link x;
    Stock s= STOCKinit(fin);

    x= LISTsearch(l, STOCKgetkey(s));

    if(l->z == x) {
        LISTinsert(l, s);
        x= LISTsearch(l, STOCKgetkey(s));
        STOCKupload(x->s, STOCKgettrans(s), fin);
    }
    else{
        STOCKupload(x->s, STOCKgettrans(s), fin);
        STOCKfree(s);
    }
}
link LISTsearch(List l, char * titolo){
    link x;

    for(x=l->head; x!=l->z && strcmp(titolo, STOCKgetkey(x->s))!=0; x=x->next);

    return x;
}
void LISTfree(List l){
    link x, toBeDeleted;

    for (x = l->head; x != l->z; toBeDeleted = x, x = x->next, NODEfree(toBeDeleted));

    free(l);
}
