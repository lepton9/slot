#include "../../testLibC/utestC.h"
#include "../include/AliasTable.h"

void test_initialize() {
  AliasTable *at;
  at = initialize((double[]){1, 2, 3}, 3);
  assertf(at != NULL, "Expected table not to be NULL");
  assertEq_int(at->n, 3, "Expected size");
  assertf(at->prob != NULL, "Expected prob table not to be NULL");
  assertf(at->alias != NULL, "Expected prob table not to be NULL");
  freeAlias(at);
}

void test_distribution() {
  size_t size = 100000;
  double variance = size * 0.005;
  double p[3] = {0.50, 0.15, 0.35};
  int dist[3] = {0, 0, 0};
  double p1[7] = {0.30, 0.13, 0.22, 0.15, 0.05, 0.10, 0.05};
  int dist1[7] = {0, 0, 0, 0, 0, 0, 0};
  AliasTable *at = initialize(p, sizeof(p) / sizeof(double));
  AliasTable *at1 = initialize(p1, sizeof(p1) / sizeof(double));

  for (int i = 0; i < size; i++) {
    dist[randAlias(at)]++;
    dist1[randAlias(at1)]++;
  }

  int sum = 0;
  for (int i = 0; i < 3; i++) {
    sum += dist[i];
    assertf(abs(dist[i] - (int)(size * p[i])) < variance, "Distribution");
  }
  assertEq_int(sum, size, "Sum of distributed elements");

  int sum1 = 0;
  for (int i = 0; i < 7; i++) {
    sum1 += dist1[i];
    assertf(abs(dist1[i] - (int)(size * p1[i])) < variance, "Distribution");
  }
  assertEq_int(sum1, size, "Sum of distributed elements");

  // printf("a: %d, b: %d, c: %d\n", dist[0], dist[1], dist[2]);
  freeAlias(at);
  freeAlias(at1);
}

int main(int argc, char **argv) {
  makeTests(argv[0]);
  addTestFunc(test_initialize);
  addTestFunc(test_distribution);
  runTests();
  return 0;
}
