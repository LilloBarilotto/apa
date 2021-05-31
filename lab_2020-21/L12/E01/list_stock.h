//
// Created by super on 15/01/2021.
//

#ifndef MAIN_C_LIST_STOCK_H
#define MAIN_C_LIST_STOCK_H

#include <stdlib.h>
#include <stdio.h>
#include "stock.h"

typedef struct list * List;
typedef struct node * link;

void LISTinsert(List l, Stock s);
void LISTadd(List l, FILE *fin);
void LISTfree(List l);
link LISTsearch(List l, char * titolo);
List LISTinit();
link LISTgetz(List l);

Stock LINKgetitem(link x);

#endif //MAIN_C_LIST_STOCK_H
