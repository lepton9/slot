#include "../include/Slot.h"
#include <assert.h>
#include <string.h>


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
  for (int i = 0; i < h; i++) {
    s->slotGrid[i] = (char*)malloc(w * sizeof(char)); 
    for (int j = 0; j < w; j++) {
      s->slotGrid[i][j] = '#';
    }
  }

  char sym[5] = {'B', 'W', '#', '&', '*'};
  double p[5] = {0.1, 0.2, 0.3, 0.25, 0.4};
  memcpy(s->symbols, sym, sizeof(sym));
  memcpy(s->prob, p, sizeof(p));

  s->alias = initialize(s->prob, sizeof(s->prob)/sizeof(s->prob[0]));
}


int main() {
  Slot s;
  initSlot(&s, 10,10);
  renderGrid(stdout, s.slotGrid, s.w, s.h);
  checkProb(&s);
  spin(&s);
  printf("\n");
  renderGrid(stdout, s.slotGrid, s.w, s.h);

  return 0;
}

