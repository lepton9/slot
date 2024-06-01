#include "../include/Slot.h"


int main(int argc, char** argv) {
  Slot* s = initSlot(10,10);
  renderGrid(stdout, s->slotGrid, s->w, s->h);
  checkProb(s);
  //sleep(2);
  //animateSpin(&s);
  //printf("Grid:\n");
  //renderGrid(stdout, s.slotGrid, s.w, s.h);

  run(s);

  return 0;
}

