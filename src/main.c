#include "../include/Slot.h"


int main(int argc, char** argv) {
  Slot s;
  initSlot(&s, 10,10);
  renderGrid(stdout, s.slotGrid, s.w, s.h);
  checkProb(&s);
  //sleep(2);
  //animateSpin(&s);
  //printf("Grid:\n");
  //renderGrid(stdout, s.slotGrid, s.w, s.h);

  while(true) {
    update(&s);
  }

  return 0;
}

