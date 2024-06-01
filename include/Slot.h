#ifndef SLOT_H
#define SLOT_H

#include <stdlib.h>
#include <stdbool.h>
#include "../include/Player.h"
#include "../include/Renderer.h"
#include "../include/LineChecker.h"
#include "../include/AliasTable.h"


#define BONUS_MIN 3
#define MIN_BET 10
#define MAX_BET 1000
#define BET_INCREMENT 10

typedef struct {
  char c;
  int prob;
} t;

typedef struct {
  Player* player;
  LineChecker* check;
  Renderer* render;
  AliasTable* alias;
  symbol** slotGrid;
  symbolTemplate allSymbols[5];
  double prob[5];
  int w,h;
  int bonusSymbols;
  bool bonus;
  bool exitFlag;
  bool turboMode;

  double betAmountCurrent;

} Slot;

Slot* initSlot(int w, int h);
void freeSlot(Slot* s);
void checkProb(Slot* s);
void spin(Slot* s);
char spinSlot(Slot* s);

void spinColumn(Slot* s, int column);
void animateSpin(Slot* s);
void animateColumn(Slot* s, int column);
symbolTemplate getRandomSymbolTemplate(Slot* s);
symbol getRandomSymbol(Slot* s);
void columnDown(Slot* s, int column);
char* getSymbColor(Slot* s, char symb);
void highlightGroups(Slot* s, groups* grps);

void insertToStr(char** str, const char* insert, int index);
void cls();
void cursorTo(int x, int y);
void update(Slot* s);
void run(Slot* s);
void exitSlot(Slot* s);
void printUI(Slot* s);

void setPlayer(Slot* s, Player* p);
void increaseBetAmount(Slot* s);
void lowerBetAmount(Slot* s);
char setBet(Slot* s);
void toggleTurboMode(Slot* s);


#endif
