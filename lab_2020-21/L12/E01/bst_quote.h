//
// Created by super on 15/01/2021.
//

#ifndef MAIN_C_BST_QUOTE_H
#define MAIN_C_BST_QUOTE_H

#include "quote.h"
#include "date.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct bst * Bst;


Bst     BSTinit();
void    BSTfree(Bst bst);
Quote   BSTsearch(Bst bst, Data d);
void    BSTadd(Bst bst, FILE *fin);
void    BSTupdate_link(Bst bst, Quote q);
void    BSTinsert_leafR(Bst bst, Quote x);
int     BSTcount(Bst bst);
void    BSTsearch_QUOTE(Bst bst, Data d1, Data d2,Quote * p_q, int cmp);
Data    BSTsearch_Data(Bst bst, int cmp);
void    BSTfind_n_cammino(Bst bst, int * p_min, int *p_max);


Quote   BSTmin(Bst bst);
Quote   BSTmax(Bst bst);
void BSTbalance(Bst bst);

#endif //MAIN_C_BST_QUOTE_H
