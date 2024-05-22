#ifndef SLOT_H
#define SLOT_H

#include <stdlib.h>
#include <stdbool.h>
#include "../include/Player.h"
#include "../include/Renderer.h"
#include "../include/LineChecker.h"
#include "../include/AliasTable.h"

typedef struct {
  char s;
  int color;
} symbolTemplate;

enum groupColors {
  Red = 31, Green, Yellow, Blue, Magenta, Cyan, White
};

enum symbolsC {
  B = 'B', 
  W = 'W',
  C = '#', D = '&', E = '*'
};

typedef struct {
  char c;
  int prob;
} t;

typedef struct {
  Player* p;
  LineChecker* check;
  Renderer* render;
  AliasTable* alias;
  symbol** slotGrid;
  symbolTemplate allSymbols[5];
  double prob[5];
  int w,h;
  int bonusSymbols;
  bool bonus;

} Slot;

void initSlot(Slot* s, int w, int h);
void freeSlot(Slot* s);
void checkProb(Slot* s);
void spin(Slot* s);

void spinColumn(Slot* s, int column);
void animateSpin(Slot* s);
void animateColumn(Slot* s, int column);
symbolTemplate getRandomSymbolTemplate(Slot* s);
symbol getRandomSymbol(Slot* s);
void columnDown(Slot* s, int column);
char* getSymbColor(Slot* s, char symb);
void highlightGroups(Slot* s, groups* grps);

void insertToStr(char* str, char* insert, int index);
void cls();
void update(Slot* s);


#endif
