#include "../include/Slot.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define BONUS_MIN 3

void spin(Slot* s) {
  for (int i = 0; i < s->w; i++) {
    spinColumn(s, i);
  }
}

void spinColumn(Slot* s, int column) {
  for (int i = 0; i < s->h; i++) {
    s->slotGrid[i][column] = getRandomSymbol(s);
  }
}

void animateSpin(Slot* s) {
  s->bonusSymbols = 0;
  for (int i = 0; i < s->w; i++) {
    animateColumn(s, i);
    s->bonusSymbols += countSymbolsCol(s->slotGrid, s->h, i, B);
  }

  if (s->bonusSymbols == BONUS_MIN) s->bonus = true;
}

void animateColumn(Slot* s, int column) {
  int am = (s->bonusSymbols == BONUS_MIN - 1) ? 100 : 20;
  spinColumn(s, column);
  for (int i = 0; i < am; i++) {
    columnDown(s, column);
    cls();
    renderGrid(stdout, s->slotGrid, s->w, s->h);
    printf("Bonus: %d\n", s->bonusSymbols);
    usleep(10000);
  }
}

void cls() {
  printf("\033[2J\033[%d;%dH", 1, 1);
}

void columnDown(Slot* s, int column) {
  for (int i = s->h-1; i > 0; i--) {
    s->slotGrid[i][column] = s->slotGrid[i-1][column];
  }
  s->slotGrid[0][column] = getRandomSymbol(s);
}

char getRandomSymbol(Slot* s) {
  int i = randAlias(s->alias);
  assert(i >= 0);
  return s->symbols[i];
}

void checkProb(Slot* s) {
  assert(sizeof(s->prob)/sizeof(s->prob[0]) == sizeof(s->symbols)/sizeof(s->symbols[0]));

  double sum = 0;
  for (int i = 0; i < sizeof(s->prob)/sizeof(s->prob[0]); i++) {
    sum += s->prob[i];
  }
  assert(sum > 0);  
}

void initSlot(Slot* s, int w, int h) {
  s->slotGrid = (char**)malloc(h * sizeof(char*)); 
  s->w = w;
  s->h = h;
  s->bonusSymbols = 0;
  s->bonus = false;
  for (int i = 0; i < h; i++) {
    s->slotGrid[i] = (char*)malloc(w * sizeof(char)); 
    for (int j = 0; j < w; j++) {
      s->slotGrid[i][j] = '#';
    }
  }

  char sym[5] = {'B', 'W', '#', '&', '*'};
  double p[5] = {0.01, 0.2, 0.3, 0.25, 0.4};
  memcpy(s->symbols, sym, sizeof(sym));
  memcpy(s->prob, p, sizeof(p));

  s->alias = initialize(s->prob, sizeof(s->prob)/sizeof(s->prob[0]));
}

void input() {
  size_t s = 64;
  char buffer[s];

}

void update() {

}


int main() {
  Slot s;
  initSlot(&s, 10,10);
  renderGrid(stdout, s.slotGrid, s.w, s.h);
  checkProb(&s);
  //sleep(2);
  animateSpin(&s);
  printf("Grid:\n");
  renderGrid(stdout, s.slotGrid, s.w, s.h);

  return 0;
}

