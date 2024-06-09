#ifndef SLOT_H
#define SLOT_H

#include "../include/AliasTable.h"
#include "../include/LineChecker.h"
#include "../include/Player.h"
#include "../include/Renderer.h"
#include <stdbool.h>
#include <stdlib.h>

#define SLOT_WIDTH 10
#define SLOT_HEIGHT 10
#define BONUS_MIN 3
#define BONUS_BUY_MULTI 100
#define FREESPINS_BEGIN 10
#define FREESPINS_GAIN 5
#define MIN_BET 10
#define MAX_BET 1000
#define BET_INCREMENT 10
#define SYMBOL_AMOUNT 8

typedef struct {
  Player *player;
  LineChecker *check;
  Renderer *render;
  AliasTable *alias;
  AliasTable *aliasBonus;
  symbol **slotGrid;
  symbolTemplate allSymbols[SYMBOL_AMOUNT];
  int w, h;
  bool exitFlag;
  bool turboMode;
  int bonusSymbols;
  int freeSpins;
  bool bonus;

  double currentWinAm;
  double betAmountCurrent;

} Slot;

Slot *initSlot(int w, int h);
void freeSlot(Slot *s);
void checkProb(Slot *s);
void spin(Slot *s);
char spinSlot(Slot *s);
void popAndSpin(Slot *s, groups *grps);
void bonusMode(Slot *s);

void spinColumn(Slot *s, int column);
void animateSpin(Slot *s);
void animateColumn(Slot *s, int column);
void animateBlankSymbSpin(Slot *s);
void animateBlankSymbColumn(Slot *s, int column, int blanks);
void symbolsFallDown(Slot *s);
symbolTemplate getRandomSymbolTemplate(Slot *s);
symbol getRandomSymbol(Slot *s);
symbol getBlankSymbol();
void columnDown(Slot *s, int column);
char *getSymbColor(Slot *s, char symb);
void printHighlightedGrid(Slot *s, groups *grps);

void insertToStr(char **str, const char *insert, int index);
void clearLines(int line, int amount);
void cls();
void cursorTo(int x, int y);
char keyPress(Slot *s);
void handleKeyPress(Slot *s, const char c);
void update(Slot *s);
void run(Slot *s);
void exitSlot(Slot *s);
void printUI(Slot *s);
void printSymbolInfo(Slot *s);
void printLeaderboard(Slot *s);

void setPlayer(Slot *s, Player *p);
void increaseBetAmount(Slot *s);
void lowerBetAmount(Slot *s);
char setBet(Slot *s);
void toggleTurboMode(Slot *s);
char buyBonus(Slot *s);
char confirmBonusBuy(Slot *s);

#endif
