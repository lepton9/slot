#ifndef ALIASTABLE_H
#define ALIASTABLE_H

#include <stdlib.h>

typedef struct {
  size_t n;
  double* prob;
  int* alias;
} AliasTable;

AliasTable* initialize(double* p, size_t n);
int randAlias(AliasTable* at);
double drand (double s, double e);
void freeAlias(AliasTable* at);

#endif
