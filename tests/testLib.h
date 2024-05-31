#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/**
#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define log_error(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define assertf(A, M, ...) if(!(A)) {log_error(M, ##__VA_ARGS__); assert(A); }
**/

#define INITIAL_SIZE 64

typedef void (*testFunction)();

typedef struct {
  char msg[64];
  char errn;
  void* expected;
  void* value;
} log;

typedef struct {
  log* logs;
  size_t n;
  size_t size;
} logs;

typedef struct {
  testFunction* functions;
  size_t n;
  size_t size;
} testFunctions;

typedef struct {
  char testFile[64];
  testFunctions* testFunctions;
  logs* failed;
  logs* passed;
} tests;

tests* makeTests(char file[64]);
void freeTests(tests* tests);
void addLogTo(logs* logs, log* log);
void addLog(tests* tests, char* msg, char passed);
void assertf(char expr, char* errMsg);
void printLogs();
void addTestFunc(testFunction);
void runTests();

