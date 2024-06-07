#include "../include/Slot.h"
#include <assert.h>
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>
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
  s->bonusSymbols = 0;
  for (int i = 0; i < s->w; i++) {
    animateColumn(s, i);
    s->bonusSymbols += countSymbolsCol(s->slotGrid, s->h, i, BONUS);
  }
}

void animateColumn(Slot* s, int column) {
  int am = (s->turboMode && !s->bonus) ? 1 : (s->bonusSymbols == BONUS_MIN - 1) ? 30 : 10;
  spinColumn(s, column);
  for (int i = 0; i < am; i++) {
    for (int col = column; col < s->w; col++) columnDown(s, col);
    printHighlightedGrid(s, NULL);
    usleep(20000);
  }
}

void animateBlankSymbColumn(Slot* s, int column, int blanks) {
  if (blanks <= 0) return;
  int am = (s->turboMode) ? 1 : (s->bonusSymbols == BONUS_MIN - 1) ? 30 : 10;
  for (int i = 0; i < blanks; i++) {
    s->slotGrid[i][column] = getRandomSymbol(s);
  }
  for (int i = 0; i < am; i++) {
    for (int i = blanks-1; i > 0; i--) {
      s->slotGrid[i][column] = s->slotGrid[i-1][column];
    }
    s->slotGrid[0][column] = getRandomSymbol(s);
    printHighlightedGrid(s, NULL);
    usleep(20000);
  }
}

void animateBlankSymbSpin(Slot* s) {
  s->bonusSymbols = 0;
  for (int i = 0; i < s->w; i++) {
    animateBlankSymbColumn(s, i, countSymbolsCol(s->slotGrid, s->h, i, BLANK));
    s->bonusSymbols += countSymbolsCol(s->slotGrid, s->h, i, BONUS);
  }
}

void columnGravity(Slot* s, int col, int bY) {
  for (int y = bY+1; y < s->h && s->slotGrid[y][col].st.c == BLANK; y++) {
    s->slotGrid[y][col] = s->slotGrid[y-1][col];
    s->slotGrid[y-1][col] = getBlankSymbol();
  }
}

void symbolsFallDown(Slot* s) {
  for (int col = 0; col < s->w; col++) {
    for (int y = s->h-1; y >= 0; y--) {
      if (s->slotGrid[y][col].st.c != BLANK) columnGravity(s, col, y);
    }
  }
}

void clearLines(int line, int amount) {
  for (int i = 0; i < amount; i++) {
    cursorTo(line + i, 1);
    printf("\033[2K");
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
  int i = (s->bonus) ? randAlias(s->aliasBonus) : randAlias(s->alias);
  assert(i >= 0);
  return s->allSymbols[i];
}

symbol getRandomSymbol(Slot* s) {
  symbolTemplate st = getRandomSymbolTemplate(s);
  symbol symb = (symbol){st, 0};
  return symb;
}

symbol getBlankSymbol() {
  return (symbol){(symbolTemplate){' '}, 0};
}

Slot* initSlot(int w, int h) {
  Slot* s = (Slot*)malloc(sizeof(Slot));
  s->slotGrid = (symbol**)malloc(h * sizeof(symbol*)); 
  s->w = w;
  s->h = h;
  s->currentWinAm = 0.0;
  s->betAmountCurrent = MIN_BET;
  s->bonusSymbols = 0;
  s->bonus = false;
  s->freeSpins = 0;
  s->exitFlag = false;
  s->turboMode = false;
  for (int i = 0; i < h; i++) {
    s->slotGrid[i] = (symbol*)malloc(w * sizeof(symbol)); 
    for (int j = 0; j < w; j++) {
      s->slotGrid[i][j] = (symbol){'#', 33, 0};
    }
  }

  // {char, color, baseMul, mulPerSym, probability}
  symbolTemplate smbs[SYMBOL_AMOUNT] = {
    {'B', 31,   0,    0, 0.005}, 
    {'W',  0,   0,    0, 0.05 }, 
    {'#', 32, 0.2,  1.1, 0.25 }, 
    {'&', 33, 0.2, 1.25, 0.21 }, 
    {'*', 34, 0.3,  1.5, 0.17 },
    {'%', 35, 0.4,  1.5, 0.14 },
    {'@', 36, 0.5,  1.5, 0.10 },
    {'?', 91, 0.7, 1.75, 0.075}
  };
  memcpy(s->allSymbols, smbs, sizeof(smbs));

  double ps[SYMBOL_AMOUNT] = {smbs[0].probability, smbs[1].probability, smbs[2].probability, smbs[3].probability, smbs[4].probability, smbs[5].probability, smbs[6].probability, smbs[7].probability};

  double pBonus[SYMBOL_AMOUNT] = {0.005, 0.1, 0.1, 0.11, 0.16, 0.175, 0.20, 0.15};

  {
    double sum = 0.0;
    double sumBonus = 0.0;
    for (int i = 0; i < SYMBOL_AMOUNT; i++) sum += ps[i];
    for (int i = 0; i < SYMBOL_AMOUNT; i++) sumBonus += pBonus[i];
    printf("sum: %f\n", sum);
    assert(fabs(sum - 1.0) < 0.000001);
    printf("sumBonus: %f\n", sumBonus);
    assert(fabs(sumBonus - 1.0) < 0.000001);
  }

  s->alias = initialize(ps, SYMBOL_AMOUNT);
  s->aliasBonus = initialize(pBonus, SYMBOL_AMOUNT);

  s->player = loadPlayer();

  return s;
}

void freeSlot(Slot* s) {
  for (int i = 0; i < s->h; i++) free(s->slotGrid[i]);
  free(s->slotGrid);
  freeAlias(s->alias);
  freeAlias(s->aliasBonus);
  freePlayer(s->player);

  // Free LineChecker and Renderer
  //free(s->render);
}

char* getSymbColor(Slot* s, char symb) {
  int nSymb = 5;
  char* c = malloc(sizeof(char));
  for (int i = 0; i < nSymb; i++) {
    if (s->allSymbols[i].c == symb) c = (char*)(&s->allSymbols[i].colorGroup);
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

void printHighlightedGrid(Slot* s, groups* grps) {
  size_t sizeOfSymb = strlen("\033[00m") + sizeof(char) + strlen("\033[0m ");
  char* buffer = (char*)malloc(sizeOfSymb * s->w * s->h + s->h * sizeof('\n'));
  buffer[0] = '\0';

  for (int i = 0; i < s->h; i++) {
    for (int j = 0; j < s->w; j++) {
      char color[12];
      symbol symb = s->slotGrid[i][j];
      if (symb.inGroup || symb.st.c == BONUS) sprintf(color, "\033[%dm", s->slotGrid[i][j].st.colorGroup);
      else sprintf(color, "\033[%dm", White);
      strcat(buffer, color);
      strncat(buffer, &(symb.st.c), 1);
      strcat(buffer, "\033[0m ");
    }
    strcat(buffer, "\n");
  }

  cursorTo(1, 1);
  render(stdout, buffer);
  free(buffer);
}

void input() {
  size_t s = 64;
  char buffer[s];
}

void popAndSpin(Slot* s, groups* grps) {
  while(grps->n > 0) {
    usleep((s->turboMode && !s->bonus) ? 100000 : 500000);
    for (int i = 0; i < grps->n; i++) {
      popGroup(s->slotGrid, grps->groups[i]);
      printHighlightedGrid(s, grps);
      double retAm = calcReturnGroup(grps->groups[i], s->betAmountCurrent);
      s->currentWinAm += retAm;
      printf("\033[2KWon: %.2f\n", retAm);
      usleep((s->turboMode && !s->bonus) ? 100000 : 500000);
    }
    symbolsFallDown(s);
    animateBlankSymbSpin(s);

    freeGroups(grps);
    grps = findGroups(s->slotGrid, s->w, s->h);
    printHighlightedGrid(s, grps);
  }
}

void bonusMode(Slot* s) {
  s->bonus = true;
  s->freeSpins = FREESPINS_BEGIN;
  if (s->bonusSymbols > BONUS_MIN) s->freeSpins += FREESPINS_GAIN;
  cursorTo(s->h+1, 1);
  printf("\033[2KBONUS! %d free spins won!\n", s->freeSpins);
  usleep(5000000);
  printf("\033[2KPress 'b' to start bonus!\n");
  char c;
  while ((c = keyPress(s)) != 'b') {
    continue;
  }

  while (s->freeSpins > 0) {
    s->freeSpins--;
    cursorTo(s->h+2, 1);
    printf("\033[2KFree spins: %d | Win amount: %.2f\n", s->freeSpins, s->currentWinAm);
    spinSlot(s);
    if (s->bonusSymbols >= BONUS_MIN) {
      s->freeSpins += FREESPINS_GAIN;
      cursorTo(s->h+1, 1);
      printf("\033[2KWon %d more free spins!\n", FREESPINS_GAIN);
    }
    usleep(500000);
  }

  addToLb(s->player, calcMultiplyer(s->currentWinAm, s->betAmountCurrent));
  clearLines(s->h+2, 1);
  s->bonus = false;
}

char spinSlot(Slot* s) {
  if (!s->bonus) {
    char betMade = setBet(s);
    if (!betMade) return 0;
    s->currentWinAm = 0.0;
  }

  clearLines(s->h+1, 1);
  printUI(s);
  animateSpin(s);
  groups* grps = findGroups(s->slotGrid, s->w, s->h);
  printHighlightedGrid(s, grps);

  if (grps->n > 0) popAndSpin(s, grps);

  freeGroups(grps);
  if (!s->bonus) addSpin(s->player, s->bonusSymbols >= BONUS_MIN);
  if (s->bonusSymbols >= BONUS_MIN && !s->bonus) {
    printUI(s);
    bonusMode(s);
  }
  cursorTo(s->h+1, 1);
  printf("\033[2KTotal won: %.2f\n", s->currentWinAm);
  if (!s->bonus) addWinnings(s->player, s->currentWinAm);
  return 1;
}

void printSymbolInfo(Slot* s) {
  fprintf(stdout, "\nSymbol : Probability\n");
  for (int i = 0; i < SYMBOL_AMOUNT; i++) {
    symbolTemplate sym = s->allSymbols[i];
    fprintf(stdout, "\033[%dm%c\033[0m : %0.3f\n", sym.colorGroup, sym.c, sym.probability);
  }
}

void printUI(Slot* s) {
  cursorTo(s->h + 3, 1);
  const char* info = playerInfo(s->player);
  fprintf(stdout, "%s", info);
  fprintf(stdout, "Bet: %.2f | Turbo mode: %s\033[0K\n", s->betAmountCurrent, (s->turboMode) ? "\033[32mON\033[0m" : "\033[31mOFF\033[0m");
  fprintf(stdout, "\n| Spin: <space> | +Bet: <w> | -Bet: <s> | Turbo: <t> | Help: <h> | Quit: <q> |\n");
  free((void*)info);
}

char keyPress(Slot* s) {
  if (s->exitFlag) return '\0';
  cbreak();
  char c = getch();
  refresh();
  endwin();
  return c;
}

void handleKeyPress(Slot* s, const char c) {
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
    case 'h':
      printSymbolInfo(s);
      break;
    case 32:
      if (!spinSlot(s)) fprintf(stdout, "Bet amount too large\n");
      break;
    default:
      break;
  }
}

void exitSlot(Slot* s) {
  s->exitFlag = true;
}

void update(Slot* s) {
  printUI(s);
  handleKeyPress(s, keyPress(s));
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
  savePlayer(s->player);
  freeSlot(s);
  // free(s);
}

