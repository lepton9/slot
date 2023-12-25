#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/**
#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define log_error(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define assertf(A, M, ...) if(!(A)) {log_error(M, ##__VA_ARGS__); assert(A); }
**/

typedef struct {
  char msg[64];
  char errn;
  void* expected;
  void* value;
} log;

typedef struct {
  log* logs[64];
  size_t n;
} logs;

typedef struct {
  char testFile[64];
  logs* failed;
  logs* passed;
} tests;

tests* makeTests(char file[32]);
void freeTests(tests* tests);
void addLog(tests* tests, char* msg, char passed);
void assertf(char expr, char* errMsg);
void printLogs();


