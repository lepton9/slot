#include "testLib.h"
#include <stdlib.h>
#include <string.h>

tests* allTests;


tests* makeTests(char file[32]) {
  tests* t = (tests*)malloc(sizeof(tests));
  t->failed = (logs*)malloc(sizeof(logs));
  t->passed = (logs*)malloc(sizeof(logs));
  t->failed->n = 0;
  t->passed->n = 0;
  strcpy(t->testFile, file); 
  allTests = t;
  return t;
}

void freeLog(log* l) {
  //free(l->expected);
  //free(l->value);
  free(l);
}

void freeTests(tests* tests) {
  for (int i = 0; i < tests->passed->n; i++) {
    freeLog(tests->passed->logs[i]);
  }
  for (int i = 0; i < tests->failed->n; i++) {
    freeLog(tests->failed->logs[i]);
  }
  free(tests->failed);
  free(tests->passed);
  free(tests);
}

void addLog(tests* tests, char* msg, char passed) {
  log* l = (log*)malloc(sizeof(log));
  strcpy(l->msg, msg);
  l->errn = errno;
  if (passed) tests->passed->logs[tests->passed->n++] = l;
  else tests->failed->logs[tests->failed->n++] = l;
}

void assertf(char expr, char* errMsg) {
  if (expr) addLog(allTests, errMsg, 1);
  else addLog(allTests, errMsg, 0);
}

void printLogs() {
  printf("\n");
  printf("Testing %s\n", allTests->testFile);
  if (allTests->passed->n > 0) printf(" \033[42;30mPASSED\033[0m: %d ", (int)allTests->passed->n);
  if (allTests->failed->n > 0) printf(" \033[41;30mFAILED\033[0m: %d ", (int)allTests->failed->n);
  printf("\n");

  //printf("Tests \033[42;30mPASSED\033[0m: %d \033[41;30mFAILED\033[0m: %d\n", (int)allTests->passed->n, (int)allTests->failed->n);

  for (int i = 0; i < allTests->failed->n; ++i) {
    printf("  \033[41;30m[ERROR]\033[0m %s\n", allTests->failed->logs[i]->msg);
  }
  //if (allTests->failed->n == 0) printf("All tests passed!\n");
  freeTests(allTests);
}


