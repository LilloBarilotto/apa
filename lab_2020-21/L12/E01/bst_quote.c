//
// Created by super on 15/01/2021.
//

#include "quote.h"
#include "bst_quote.h"
#include "date.h"

typedef struct bst_node* link;
struct bst_node {
    Quote item;
    link l, r, p;
    int N;
};
struct bst{
    link root; link z;
};

static link NEW(Quote item, link p, link l, link r, int N) {
    link x = malloc(sizeof *x);

    x->item= item;
    x->p = p;
    x->l = l;
    x->r = r;
    x->N = N;
    return x;
}
static link LINKsearchR(link h, Quote k, link z){
    int cmp= QUOTEcmp(k, h->item);

    if(cmp ==0)
        return h;
    if(cmp <0)
        return LINKsearchR(h->l, k, z);
    return LINKsearchR(h->r, k, z);
}
static void treeFree(link h, link z) {
    if (h == z )
        return;
    treeFree(h ->l, z );
    treeFree(h ->r, z );

    QUOTEfree(h->item);
    free(h);
}
static int countR(link h, link z){
    if (h ==z)
        return 0;
    return countR(h->l, z) + countR(h->r, z)+1;
}

static Quote searchR(link h, Data d, link z){
    int cmp;
    if (h == z)
        return QUOTEinit();
    cmp= DATAcmp(d, h->item.d);
    if(cmp ==0)
        return h->item;
    if(cmp < 0)
        return searchR(h->l, d, z);
    return searchR(h->r, d, z);
}
static link insertR(link h, Quote x, link z){
    if (h == z)
        return NEW(x,z, z, z, 1);

    if (QUOTEcmp(x, h->item)<0) {
        h->l = insertR(h->l, x, z);
        h->r->p= h;
    }
    else {
        h->r = insertR(h->r, x, z);
        h->r->p= h;
    }

    h->N++;
    return h;
}

Bst BSTinit() {
    Bst bst = malloc(sizeof *bst) ;
    bst->root= ( bst->z = NEW( QUOTEinit(),NULL, NULL, NULL, 0));
    return bst;
}
void    BSTfree(Bst bst){
    if (bst == NULL)
        return;
    treeFree(bst->root, bst->z);
    free(bst->z);
}
int     BSTcount(Bst bst){
    return countR(bst->root, bst->z);
}
Quote   BSTsearch(Bst bst, Data d){
    return searchR(bst->root, d, bst->z);
}
void    BSTinsert_leafR(Bst bst, Quote x){
    bst->root = insertR(bst->root, x, bst->z);
}
void    BSTadd(Bst bst, FILE *fin){
    Quote q=QUOTEinit();
    q=QUOTEread(q, fin);

    if(BSTsearch(bst, q.d).val==0)
        BSTinsert_leafR(bst, q);
    else {
        BSTupdate_link(bst, q);
        QUOTEfree(q);
    }
}
void    BSTupdate_link(Bst bst, Quote q){
    link x;

    x=LINKsearchR(bst->root, q, bst->z);
    x->item.val+=   q.val;
    x->item.num+=   q.num;
}

static void    searchR_quote(link h, Data d1, Data d2, link z, Quote * p_q, int cmp){
    if(h->l!=z)
        searchR_quote(h->l, d1, d2, z, p_q, cmp);

    if(DATAcmp(h->item.d, d1)>=0 && DATAcmp(h->item.d, d2)<=0){
        if(QUOTEseebest(h->item, *p_q, cmp))
            *p_q=h->item;
    }

    if(h->r!=z)
        searchR_quote(h->r, d1, d2, z, p_q, cmp);
}
void    BSTsearch_QUOTE(Bst bst, Data d1, Data d2,Quote * p_q, int cmp){
    if(bst->root != bst->z)
        searchR_quote(bst->root, d1, d2, bst->z, p_q, cmp);
}

static Data BSTsearch_minData(link x, link z){
    while (x->l != z)
         x= x->l;

    return x->item.d;
}
static Data BSTsearch_maxData(link x, link z){
    while (x->r != z)
        x= x->r;

    return x->item.d;
}

Data BSTsearch_Data(Bst bst, int cmp){
    if(cmp==0)
        return BSTsearch_minData(bst->root, bst->z);

    return BSTsearch_maxData(bst->root, bst->z);
}

static void BSTfind_n_camm_r(link x, int * p_min, int *p_max, int count, link z){
    if(x==z){
        if(count<*p_min) *p_min=count;
        if(count>*p_max) *p_max=count;
        return;
    }
    BSTfind_n_camm_r(x->l, p_min, p_max, count + 1, z);
    BSTfind_n_camm_r(x->r, p_min, p_max, count + 1, z);
}

void BSTfind_n_cammino(Bst bst, int * p_min, int *p_max){
    BSTfind_n_camm_r(bst->root, p_min, p_max, 0, bst->z);
}

static Quote selectR(link h, int r, link z) {
    int t;
    if (h == z)
        return QUOTEinit();
    t = h->l->N;
    if (t > r)
        return selectR(h->l, r, z);
    if (t < r)
        return selectR(h->r, r-t-1, z);
    return h->item;
}
Quote BSTselect(Bst bst, int r) {
    return selectR(bst->root, r, bst->z);
}

static link rotR(link h) {
    link x = h->l;
    h->l = x->r;
    x->r->p = h;
    x->r = h;
    x->p = h->p;
    h->p = x;
    x->N = h->N;
    h->N = 1;
    h->N += (h->l) ? h->l->N : 0;
    h->N += (h->r) ? h->r->N : 0;
    return x;
}
static link rotL(link h) {
    link x = h->r;
    h->r = x->l;
    x->l->p = h;
    x->l = h;
    x->p = h->p;
    h->p = x;
    x->N = h->N;
    h->N = 1;
    h->N += (h->l) ? h->l->N : 0;
    h->N += (h->r) ? h->r->N : 0;
    return x;
}
static link partR(link h, int r) {
    int t = h->l->N;
    if (t > r) {
        h->l = partR(h->l, r);
        h = rotR(h);
    }
    if (t < r) {
        h->r = partR(h->r, r-t-1);
        h = rotL(h);
    }
    return h;
}
static link balanceR(link h, link z) {
    int r;
    if (h == z)
        return z;
    r = (h->N+1)/2-1;
    h = partR(h, r);
    h->l = balanceR(h->l, z);
    h->r = balanceR(h->r, z);
    return h;
}
void BSTbalance(Bst bst) {
    bst->root = balanceR(bst->root, bst->z);
}