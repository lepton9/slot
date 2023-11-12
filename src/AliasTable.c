#include "../include/AliasTable.h"
#include <assert.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

// p represents the probabilities of n elements
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

void freeAlias(AliasTable* at) {
  free(at->prob);
  free(at->alias);
  free(at);
}

double drand (double s, double e) {
    return ((double)rand() * (e - s)) / (double)RAND_MAX + s;
}

int randAlias(AliasTable* at) {
  if (at == NULL || at->n <= 0) return -1;

  double u = drand(0, at->n);
  int j = floor(u);

  if (u - j <= at->prob[j]) return j;
  return at->alias[j];
}

/**
int main() {
  size_t size = 10000;
  double p[3] = {0.50, 0.15, 0.35};
  AliasTable* at;
  at = initialize(p, sizeof(p)/sizeof(double));
  int a = 0; 
  int b = 0; 
  int c = 0;
  
  int ind;
  for (int i = 0; i < size; i++) {
    ind = randAlias(at);
    switch (ind) {
      case 0:
        a++;
        break;
      case 1:
        b++;
        break;
      case 2:
        c++;
        break;
      default:
        assert(ind > 0);
        break;
    }
  }

  printf("a: %d, b: %d, c: %d\n", a,b,c);
  freeAlias(at);

  return 0;
}
**/
