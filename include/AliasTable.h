#ifndef ALIASTABLE_H
#define ALIASTABLE_H

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct {
  size_t n;
  double* prob;
  int* alias;
} AliasTable;

AliasTable* initialize(double* p, size_t n);
int randAlias(AliasTable* at);
double drand (double s, double e);

#endif
