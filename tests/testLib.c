#include "testLib.h"
#include <stdlib.h>
#include <string.h>

static tests* allTests;

tests* makeTests(char file[32]) {
  tests* t = (tests*)malloc(sizeof(tests));

  t->testFunctions = (testFunctions*)malloc(sizeof(testFunctions));
  t->testFunctions->functions = malloc(INITIAL_SIZE * sizeof(testFunction));
  t->testFunctions->size = INITIAL_SIZE;
  t->testFunctions->n = 0;

  t->failed = (logs*)malloc(sizeof(logs));
  t->failed->logs = malloc(INITIAL_SIZE * sizeof(log));
  t->passed = (logs*)malloc(sizeof(logs));
  t->passed->logs = malloc(INITIAL_SIZE * sizeof(log));

  t->failed->size = INITIAL_SIZE;
  t->failed->n = 0;
  t->passed->size = INITIAL_SIZE;
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
  // for (int i = 0; i < tests->passed->n; i++) {
  //   freeLog(tests->passed->logs[i]);
  // }
  // for (int i = 0; i < tests->failed->n; i++) {
  //   freeLog(tests->failed->logs[i]);
  // }
  free(tests->passed);
  free(tests->failed);
  free(tests->testFunctions);
  free(tests);
}

void addLogTo(logs* logs, log* l) {
  if (logs->n == logs->size) {
    logs->size *= 2;
    logs->logs = realloc(logs->logs, logs->size * sizeof(log));
    if (logs->logs == NULL) {
      perror("Realloc for logs failed");
      exit(EXIT_FAILURE);
    }
  }
  logs->logs[logs->n++] = *l;
}

void addLog(tests* tests, char* msg, char passed) {
  // log* l = (log*)malloc(sizeof(log));
  log l;
  strcpy(l.msg, msg);
  l.errn = errno;
  if (passed) addLogTo(tests->passed, &l);
  else addLogTo(tests->failed, &l);
}

void assertf(char expr, char* errMsg) {
  addLog(allTests, errMsg, expr);
  //if (expr) addLog(allTests, errMsg, 1);
  //else addLog(allTests, errMsg, 0);
}

void printLogs() {
  printf("\n");
  printf("Testing %s\n", allTests->testFile);
  if (allTests->passed->n > 0) printf(" \033[42;30mPASSED\033[0m: %d ", (int)allTests->passed->n);
  if (allTests->failed->n > 0) printf(" \033[41;30mFAILED\033[0m: %d ", (int)allTests->failed->n);
  printf("\n");

  //printf("Tests \033[42;30mPASSED\033[0m: %d \033[41;30mFAILED\033[0m: %d\n", (int)allTests->passed->n, (int)allTests->failed->n);

  for (int i = 0; i < allTests->failed->n; ++i) {
    // Print the function where the error happened
    printf("  \033[41;30m[ERROR]\033[0m %s\n", allTests->failed->logs[i].msg);
  }
  //if (allTests->failed->n == 0) printf("All tests passed!\n");
}

void addTestFunc(testFunction tf) {
  if (allTests->testFunctions->n == allTests->testFunctions->size) {
    allTests->testFunctions->size *= 2;
    allTests->testFunctions->functions = realloc(allTests->testFunctions->functions, allTests->testFunctions->size * sizeof(testFunction));
    if (allTests->testFunctions->functions == NULL) {
      perror("Realloc for testFunctions failed");
      exit(EXIT_FAILURE);
    }
  }
  allTests->testFunctions->functions[allTests->testFunctions->n++] = tf;
}

void runTests() {
  for (int i = 0; i < allTests->testFunctions->n; i++) {
    // Print the function name being tested at the moment
    allTests->testFunctions->functions[i] ();
  }
  printLogs();
  freeTests(allTests);
}


