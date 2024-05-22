#include "../include/Slot.h"
#include <assert.h>
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <ncurses.h>
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
    s->bonusSymbols += countSymbolsCol(s->slotGrid, s->h, i, BONUS);
  }

  if (s->bonusSymbols == BONUS_MIN) s->bonus = true;
}

void animateColumn(Slot* s, int column) {
  int am = (s->bonusSymbols == BONUS_MIN - 1) ? 80 : 20;
  spinColumn(s, column);
  for (int i = 0; i < am; i++) {
    for (int col = column; col < s->w; col++) columnDown(s, col);
    cls();
    renderGrid(stdout, s->slotGrid, s->w, s->h);
    printf("Bonus: %d\n", s->bonusSymbols);
    usleep(20000);
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

symbolTemplate getRandomSymbolTemplate(Slot* s) {
  int i = randAlias(s->alias);
  assert(i >= 0);
  return s->allSymbols[i];
}

symbol getRandomSymbol(Slot* s) {
  symbolTemplate st = getRandomSymbolTemplate(s);
  symbol symb = (symbol){st.s, st.color, 0};
  return symb;
}

void checkProb(Slot* s) {
  assert(sizeof(s->prob)/sizeof(s->prob[0]) == sizeof(s->allSymbols)/sizeof(s->allSymbols[0]));

  double sum = 0;
  for (int i = 0; i < sizeof(s->prob)/sizeof(s->prob[0]); i++) {
    sum += s->prob[i];
  }
  assert(sum > 0);  
}

Slot* initSlot(int w, int h) {
  Slot* s = (Slot*)malloc(sizeof(Slot));
  s->slotGrid = (symbol**)malloc(h * sizeof(symbol*)); 
  s->w = w;
  s->h = h;
  s->bonusSymbols = 0;
  s->bonus = false;
  s->exitFlag = false;
  for (int i = 0; i < h; i++) {
    s->slotGrid[i] = (symbol*)malloc(w * sizeof(symbol)); 
    for (int j = 0; j < w; j++) {
      s->slotGrid[i][j] = (symbol){'#', 33, 0};
    }
  }

  symbolTemplate smbs[5] = {{'B', 31}, {'W', 32}, {'#', 33}, {'&', 34}, {'*', 35}};
  memcpy(s->allSymbols, smbs, sizeof(smbs));

  //char sym[5] = {'B', 'W', '#', '&', '*'};
  double p[5] = {0.01, 0.03, 0.3, 0.25, 0.4};

  //memcpy(s->symbols, sym, sizeof(sym));
  memcpy(s->prob, p, sizeof(p));

  s->alias = initialize(s->prob, sizeof(s->prob)/sizeof(s->prob[0]));
  return s;
}

void freeSlot(Slot* s) {
  for (int i = 0; i < s->h; i++) free(s->slotGrid[i]);
  free(s->slotGrid);
  freeAlias(s->alias);

  // Free LineChecker and Renderer
  //free(s->render);
}

char* getSymbColor(Slot* s, char symb) {
  int nSymb = 5;
  char* c = malloc(sizeof(char));
  for (int i = 0; i < nSymb; i++) {
    if (s->allSymbols[i].s == symb) c = (char*)(&s->allSymbols[i].color);
  }
  return c;
}

void insertToStr(char* str, char* insert, int index) {
  char* newStr = malloc(sizeof(str) + sizeof(insert));
  strncpy(newStr, str, index);
  strcat(newStr, insert);
  strcat(newStr, str + index);

  str = realloc(str, sizeof(str) + sizeof(insert));
  *str = *newStr;
  free(newStr);
}

void highlightGroups(Slot* s, groups* grps) {
  size_t sizeOfSymb = strlen("\033[00;40m") + sizeof(char) + strlen("\033[0m ");
  char* buffer = (char*)malloc(sizeOfSymb * s->w * s->h + s->h * sizeof('\n'));
  buffer[0] = '\0';
  cls();

  for (int i = 0; i < s->h; i++) {
    for (int j = 0; j < s->w; j++) {
      char color[12];
      symbol symb = s->slotGrid[i][j];
      if (symb.inGroup || symb.c == BONUS) sprintf(color, "\033[%d;40m", s->slotGrid[i][j].colorGroup);
      else sprintf(color, "\033[%d;40m", 37);
      strcat(buffer, color);
      strncat(buffer, &(symb.c), 1);
      strcat(buffer, "\033[0m ");
    }
    strcat(buffer, "\n");
  }

  render(stdout, buffer);
  free(buffer);
}

void input() {
  size_t s = 64;
  char buffer[s];
}

void keyPress(Slot* s) {
  if (s->exitFlag) return;
  printf("Action:\n");
  cbreak();
  initscr();
  char c;
  while ((c = getch()) != ERR) {
    refresh();
    endwin();
    if (c == 'q') exitSlot(s);
    else if (c == 32) {
      animateSpin(s);
      groups* grps = findGroups(s->slotGrid, s->w, s->h);

      // Highlight groups
      highlightGroups(s, grps);

      freeGroups(grps);
    }
    else break;
  }
}

void exitSlot(Slot* s) {
  s->exitFlag = true;
}

void update(Slot* s) {
  keyPress(s);
}

