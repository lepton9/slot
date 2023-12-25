#include "../include/AliasTable.h"
#include "testLib.h"

void test_initialize() {
  AliasTable* at;
  at = initialize((double[]){1,2,3}, 3);
  assertf(at != NULL, "Expected table not to be NULL");
  assertf(at->n == 3, "Expected size differs");
  assertf(at->prob != NULL, "Expected prob table not to be NULL");
  assertf(at->alias != NULL, "Expected prob table not to be NULL");
}

void test_distribution() {
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

  //printf("a: %d, b: %d, c: %d\n", a,b,c);
  freeAlias(at);
}

void run_tests() {
  test_initialize();
  test_distribution();
}

int main(int argc, char** argv) {
  makeTests(argv[0]);
  run_tests();
  printLogs();
  return 0;
}


