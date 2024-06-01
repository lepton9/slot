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



void setPlayer(Slot* s, Player* p) {
  if (p) s->player = p;
}

void increaseBetAmount(Slot* s) {
  s->betAmountCurrent += (s->betAmountCurrent + BET_INCREMENT > MAX_BET) ? 0 : BET_INCREMENT;
}

void lowerBetAmount(Slot* s) {
  s->betAmountCurrent -= (s->betAmountCurrent - BET_INCREMENT < MIN_BET) ? 0 : BET_INCREMENT;
}

char setBet(Slot* s) {
  return makeBet(s->player, s->betAmountCurrent);
}

void toggleTurboMode(Slot* s) {
  s->turboMode ^= 1;
}

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
  s->bonus = false;
  s->bonusSymbols = 0;
  for (int i = 0; i < s->w; i++) {
    animateColumn(s, i);
    s->bonusSymbols += countSymbolsCol(s->slotGrid, s->h, i, BONUS);
  }

  if (s->bonusSymbols == BONUS_MIN) s->bonus = true;
}

void animateColumn(Slot* s, int column) {
  int am = (s->turboMode) ? 1 : (s->bonusSymbols == BONUS_MIN - 1) ? 30 : 10;
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

void cursorTo(const int line, const int column) {
  printf("\033[%d;%dH", line, column);
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
  s->betAmountCurrent = MIN_BET;
  s->bonusSymbols = 0;
  s->bonus = false;
  s->exitFlag = false;
  s->turboMode = false;
  for (int i = 0; i < h; i++) {
    s->slotGrid[i] = (symbol*)malloc(w * sizeof(symbol)); 
    for (int j = 0; j < w; j++) {
      s->slotGrid[i][j] = (symbol){'#', 33, 0};
    }
  }

  symbolTemplate smbs[5] = {{'B', 31}, {'W', 32}, {'#', 33}, {'&', 34}, {'*', 35}};
  memcpy(s->allSymbols, smbs, sizeof(smbs));

  //              'B',  'W', '#',  '&', '*' 
  double p[5] = {0.01, 0.03, 0.3, 0.25, 0.4};

  memcpy(s->prob, p, sizeof(p));

  s->alias = initialize(s->prob, sizeof(s->prob)/sizeof(s->prob[0]));

  s->player = initPlayer(1000);

  return s;
}

void freeSlot(Slot* s) {
  for (int i = 0; i < s->h; i++) free(s->slotGrid[i]);
  free(s->slotGrid);
  freeAlias(s->alias);
  freePlayer(s->player);

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

void insertToStr(char** str, const char* insert, int index) {
  int strLen = strlen(*str);
  int insertLen = strlen(insert);
  char* newStr = malloc(strLen + insertLen + 1);
  strncpy(newStr, *str, index);
  strcpy(newStr + index, insert);
  strcpy(newStr + index + insertLen, *str + index);
  *str = realloc(*str, strLen + insertLen + 1);
  strcpy(*str, newStr);
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
      else sprintf(color, "\033[%d;40m", White);
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

char spinSlot(Slot* s) {
  char betMade = setBet(s);
  if (!betMade) return 0;

  animateSpin(s);
  groups* grps = findGroups(s->slotGrid, s->w, s->h);
  highlightGroups(s, grps);
  freeGroups(grps);
  addSpin(s->player, s->bonus);
  if (s->bonus) {
  // Bonus function
    printf("Bonus!\n");
  }
  return 1;
}

void printUI(Slot* s) {
  cursorTo(s->h + 2, 1);
  const char* info = playerInfo(s->player);
  fprintf(stdout, info);
  fprintf(stdout, "Bet: %f | Turbo mode: %s\033[0K\n", s->betAmountCurrent, (s->turboMode) ? "ON" : "OFF");
  fprintf(stdout, "\n| Spin: <space> | +Bet: <w> | -Bet: <s> | Turbo: <t> | Quit: <q> |\n");
  free((void*)info);
}

void keyPress(Slot* s) {
  if (s->exitFlag) return;
  printUI(s);
  cbreak();
  char c = getch();
  refresh();
  endwin();
  switch (c) {
    case 'q':
      exitSlot(s);
      break;
    case 'w':
      increaseBetAmount(s);
      break;
    case 's':
      lowerBetAmount(s);
      break;
    case 't':
      toggleTurboMode(s);
      break;
    case 32:
      char spin = spinSlot(s);
      if (!spin) fprintf(stdout, "Bet amount too large\n");
      break;
    default:
      break;
  }
}

void exitSlot(Slot* s) {
  s->exitFlag = true;
}

void update(Slot* s) {
  keyPress(s);
}

void run(Slot* s) {
  cls();
  initscr();
  refresh();
  endwin();
  renderGrid(stdout, s->slotGrid, s->w, s->h);
  while(!s->exitFlag) {
    update(s);
  }
  freeSlot(s);
  free(s);
}

