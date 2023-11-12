#ifndef SLOT_H
#define SLOT_H

#include <stdlib.h>
#include "../include/Player.h"
#include "../include/Renderer.h"
#include "../include/LineChecker.h"
#include "../include/AliasTable.h"

typedef struct {
  char c;
  int prob;
} t;

typedef struct {
  Player* p;
  LineChecker* check;
  Renderer* render;
  AliasTable* alias;
  char** slotGrid;
  char symbols[5];
  double prob[5];
  int w,h;

} Slot;

void initSlot(Slot* s, int w, int h);
void checkProb(Slot* s);
void spin(Slot* s);

void spinColumn(Slot* s, int column);
void animateSpin(Slot* s);
char getRandomSymbol(Slot* s);

#endif
