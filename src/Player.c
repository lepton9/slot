#include "../include/Player.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Player *initPlayer(double bal) {
  Player *p = (Player *)malloc(sizeof(Player));
  p->balance = bal;
  p->totalSpins = 0;
  p->bonusAmount = 0;
  p->lastBonus = -1;
  p->lbN = 0;
  return p;
}

void freePlayer(Player *p) { free(p); }

void addSpin(Player *p, char bonus) {
  p->totalSpins++;
  if (bonus) {
    p->bonusAmount++;
    p->lastBonus = 0;
  } else if (p->bonusAmount > 0)
    p->lastBonus++;
}

char makeBet(Player *p, double amount) {
  if (amount > 0 && p->balance >= amount) {
    p->balance -= amount;
    return 1;
  }
  return 0;
}

void addWinnings(Player *p, double amount) {
  if (amount > 0)
    p->balance += amount;
}

const char *playerInfo(Player *p) {
  char *info = malloc(100 * sizeof(char));
  sprintf(info,
          "Balance: %.2f | Spins: %d | Bonuses: %d | Last bonus: %d\033[0K\n",
          p->balance, p->totalSpins, p->bonusAmount, p->lastBonus);
  return info;
}

int addToLb(Player *p, double multi) {
  for (int i = 0; i < p->lbN; i++) {
    if (multi > p->bestBonusMulti[i]) {
      for (int j = p->lbN; j > i + 1; j--) {
        if (j == MAX_LEADERBOARD - 1)
          continue;
        p->bestBonusMulti[j] = p->bestBonusMulti[j - 1];
      }
      p->bestBonusMulti[i] = multi;
      p->lbN++;
      return i;
    }
  }
  if (p->lbN < MAX_LEADERBOARD) {
    p->bestBonusMulti[p->lbN++] = multi;
    return p->lbN;
  }
  p->bestBonusMulti[p->lbN - 1] = multi;
  return p->lbN;
}

Player *loadPlayer() {
  FILE *file;
  file = fopen(PLAYER_FILENAME, "r");

  Player *p = initPlayer(1000);

  int rank;
  double multi;
  char line[100];
  if (file != NULL) {
    while (fgets(line, 100, file)) {
      if (sscanf(line, "balance: %lf", &p->balance) == 1)
        continue;
      if (sscanf(line, "spins: %d", &p->totalSpins) == 1)
        continue;
      if (sscanf(line, "bonuses: %d", &p->bonusAmount) == 1)
        continue;
      if (sscanf(line, "lastbonus: %d", &p->lastBonus) == 1)
        continue;
      if (sscanf(line, "%*d: %lf x", &multi) == 1) {
        p->bestBonusMulti[p->lbN++] = multi;
      }
    }
  }
  fclose(file);
  return p;
}

char savePlayer(Player *p) {
  FILE *file = fopen(PLAYER_FILENAME, "w");
  if (file == NULL) {
    printf("Error opening file.\n");
    return 0;
  }
  fprintf(file, "balance: %f\n", p->balance);
  fprintf(file, "spins: %d\n", p->totalSpins);
  fprintf(file, "bonuses: %d\n", p->bonusAmount);
  fprintf(file, "lastbonus: %d\n", p->lastBonus);
  fprintf(file, "\n-----Best multiplyers-----\n");
  for (int i = 0; i < p->lbN; i++) {
    fprintf(file, "%d: %lf x\n", i + 1, p->bestBonusMulti[i]);
  }
  fclose(file);

  return 1;
}
