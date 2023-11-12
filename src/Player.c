#include "../include/Player.h"
#include <stdlib.h>

Player* initPlayer(double bal) {
  Player* p = (Player*)malloc(sizeof(Player));
  p->balance = bal;
  return p;
}

void freePlayer(Player* p) {
  free(p);
}

