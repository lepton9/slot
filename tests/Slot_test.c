#include "../include/Slot.h"
#include "testLib.h"
#include <string.h>


void test_columnDown() {
  int w = 5; int h = 5;
  Slot* s;
  initSlot(s, w, h);
  //char* r = {'#', '#', '#', '#', '#'};
  //memcpy(s->slotGrid[0], (char){'#', '#', '#', '#', '#'}, w);

  freeSlot(s);
}

void test_getRandomSymbol() {

}

void test_insertToStr() {
  char* str1 = malloc(6 * sizeof(char));
  char* str2 = malloc(3 * sizeof(char));
  strcpy(str1, "abcde");
  strcpy(str2, "ff");

  insertToStr(str1, str2, 2);
  assertf(strcmp(str1, "abffcde") != 0, "Compared strings differ");
  assertf(strlen(str1) == 7, "Wrong string lenght");

  free(str1);
  free(str2);
}

void run_tests() {
  test_columnDown();
  test_getRandomSymbol();
  test_insertToStr();

}

int main(int argc, char** argv) {
  makeTests(argv[0]);
  run_tests();
  printLogs();
  return 0;
}
