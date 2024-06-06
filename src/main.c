#include "../include/Slot.h"

int main(int argc, char **argv) {
  Slot *s = initSlot(SLOT_WIDTH, SLOT_HEIGHT);
  run(s);
  free(s);

  return 0;
}
