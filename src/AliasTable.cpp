#include "../include/AliasTable.h"

// p represents the probabilities
AliasTable* initialize(double* p, size_t n) {
  if (n <= 0) return NULL;
  srand (time(NULL));

  AliasTable* t = (AliasTable*)malloc(sizeof(AliasTable));
  t->n = n;

  int* large = (int*)malloc(n * sizeof(int));
  size_t l = 0;
  int* small = (int*)malloc(n * sizeof(int));
  size_t s = 0;

  for (int j = 0; j < n; j++) {
    if (p[j] > (double)1/n) {
      large[l++] = j;
    }
    else {
      small[s++] = j;
    }
  }
  large = (int*)realloc(large, l * sizeof(int));
  small = (int*)realloc(small, s * sizeof(int));

  t->prob = (double*)malloc(n * sizeof(double));
  t->alias = (int*)malloc(n * sizeof(int));

  int j, k;
  while (s > 0 && l > 0) {
    s--; l--;
    j = small[s];
    k = large[l];

    t->prob[j] = n * p[j];
    t->alias[j] = k;

    p[k] = p[k] + (p[j] - (double)1/n);

    if (p[k] > (double)1/n) {
      large[l++] = k;
    } else {
      small[s++] = k;
    }
  }

  while(s > 0) {
    t->prob[small[--s]] = 1;
  }
  while(l > 0) {
    t->prob[large[--l]] = 1;
  }

  free(large);
  free(small);

  return t;
}

double drand (double s, double e) {
    return ((double)rand() * (e - s)) / (double)RAND_MAX + s;
}

int randAlias(AliasTable* at) {
  if (at->n <= 0) return -1;

  double u = drand(0, at->n);
  int j = floor(u);

  if (u - j <= at->prob[j]) return j;
  return at->alias[j];
}


