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
    s->bonusSymbols += countSymbolsCol(s->slotGrid, s->h, i, B);
  }

  if (s->bonusSymbols == BONUS_MIN) s->bonus = true;
}

void animateColumn(Slot* s, int column) {
  int am = (s->bonusSymbols == BONUS_MIN - 1) ? 100 : 20;
  spinColumn(s, column);
  for (int i = 0; i < am; i++) {
    columnDown(s, column);
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

char getRandomSymbol(Slot* s) {
  int i = randAlias(s->alias);
  assert(i >= 0);
  return s->symbols[i].s;
  //return s->symbols[i];
}
/**
symbol getRandomSymbol(Slot* s) {
  int i = randAlias(s->alias);
  assert(i >= 0);
  return s->symbols[i];
}
**/

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
  //s->slotGrid = (symb**)malloc(h * sizeof(symb*)); 
  s->w = w;
  s->h = h;
  s->bonusSymbols = 0;
  s->bonus = false;
  for (int i = 0; i < h; i++) {
    s->slotGrid[i] = (char*)malloc(w * sizeof(char)); 
    //s->slotGrid[i] = (symb*)malloc(w * sizeof(symb)); 
    for (int j = 0; j < w; j++) {
      //s->slotGrid[i][j] = (symb){'#', 33, 0, 0};
      s->slotGrid[i][j] = '#';
    }
  }

  symbol smbs[5] = {{'B', 31}, {'W', 32}, {'#', 33}, {'&', 34}, {'*', 35}};
  memcpy(s->symbols, smbs, sizeof(smbs));

  //char sym[5] = {'B', 'W', '#', '&', '*'};
  double p[5] = {0.01, 0.2, 0.3, 0.25, 0.4};

  //memcpy(s->symbols, sym, sizeof(sym));
  memcpy(s->prob, p, sizeof(p));

  s->alias = initialize(s->prob, sizeof(s->prob)/sizeof(s->prob[0]));
}

void freeSlot(Slot* s) {
  for (int i = 0; i < s->h; i++) {
    free(s->slotGrid[i]);
  }
  free(s->slotGrid);
  freeAlias(s->alias);

  // Free LineChecker and Renderer

  free(s->render);

  free(s);
}

char* getSymbColor(Slot* s, char symb) {
  int nSymb = 5;
  char* c = malloc(sizeof(char));
  for (int i = 0; i < nSymb; i++) {
    if (s->symbols[i].s == symb) c = (char*)(&s->symbols[i].color);
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
  size_t sizeOfSymb = sizeof(char) + sizeof("\033[00;42m") + sizeof("\033[0m ");
  char* buffer = (char*)malloc(sizeOfSymb);
  //buffer = "\0";

  for (int i = 0; i < grps->n; i++) {
    char symb = grps->groups[i]->symb;
    for (int j = 0; j < grps->groups[i]->n; j++) {
      //buffer[i][j] = '\033[' + getSymbColor(s, symb) + ';42m';
      // Buffer, replace at specific index calculated by the lenght of the chars
      
      /**
      char* sc = getSymbColor(s, symb);
      printf("%d", *sc);
      strcat(buffer, "\033[");
      strcat(buffer, sc);
      strcat(buffer, ";42m");
      strcat(buffer, &symb);
      strcat(buffer, "\033[0m ");
      **/

      // Make either slotgrid or grps use the symbol struct instead of the basic chars
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
  printf("Action:\n");
  cbreak();
  initscr();
  char c;
  while ((c = getch()) != ERR) {
    refresh();
    endwin();
    if (c == 'q') exit(0);
    else if (c == 32) {
      animateSpin(s);
      groups* grps = findGroups(s->slotGrid, s->w, s->h);

      // Highlight groups
      //highlightGroups(s, grps);

      freeGroups(grps);
    }
    else break;
  }
}

void exitSlot(Slot* s) {
  freeSlot(s);
  exit(0);
}

void update(Slot* s) {
  keyPress(s);

}

