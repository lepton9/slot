#include "../include/Slot.h"
#include "../../testLibC/utestC.h"
#include <string.h>


void test_columnDown() {
  int w = 5; int h = 5;
  Slot* s = initSlot(w, h);
  //char* r = {'#', '#', '#', '#', '#'};
  //memcpy(s->slotGrid[0], (char){'#', '#', '#', '#', '#'}, w);

  freeSlot(s);
}

void test_getRandomSymbol() {

}

void test_symbolsFallDown() {
  Slot* s = initSlot(5, 5);
  char gr[5][5] = {
      {' ','#',' ','#','%'},
      {'#',' ',' ','%','#'},
      {'#',' ',' ','%','#'},
      {'#','#',' ','%','#'},
      {'#','#',' ',' ','%'}
  };
  char grGravity[5][5] = {
      {' ',' ',' ',' ','%'},
      {'#',' ',' ','#','#'},
      {'#','#',' ','%','#'},
      {'#','#',' ','%','#'},
      {'#','#',' ','%','%'}
  };

  for (int i = 0; i < s->h; i++) {
    for (int j = 0; j < s->w; j++) {
      s->slotGrid[i][j].st.c = gr[i][j];
    }
  }

  symbolsFallDown(s);

  for (int i = 0; i < s->h; i++) {
    for (int j = 0; j < s->h; j++) {
      assertEq_char(s->slotGrid[i][j].st.c, grGravity[i][j], "Should be same symbols");
    }
  }

  freeSlot(s);
}

void test_insertToStr() {
  char* str1 = malloc(6 * sizeof(char));
  char* str2 = malloc(3 * sizeof(char));
  strcpy(str1, "abcde");
  strcpy(str2, "ff");

  insertToStr(&str1, str2, 2);
  assertf(strcmp(str1, "abffcde") == 0, "Compared strings differ");
  assertf(strlen(str1) == 7, "Wrong string lenght");

  free(str1);
  free(str2);
}

int main(int argc, char** argv) {
  makeTests(argv[0]);
  addTestFunc(test_columnDown);
  addTestFunc(test_getRandomSymbol);
  addTestFunc(test_insertToStr);
  addTestFunc(test_symbolsFallDown);
  runTests();
  return 0;
}

