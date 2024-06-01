#include "../include/Player.h"
#include <stdlib.h>
#include <stdio.h>

Player* initPlayer(double bal) {
  Player* p = (Player*)malloc(sizeof(Player));
  p->balance = bal;
  p->totalSpins = 0;
  p->bonusAmount = 0;
  p->lastBonus = -1;
  return p;
}

void freePlayer(Player* p) {
  free(p);
}

void addSpin(Player* p, char bonus) {
  p->totalSpins++;
  if (bonus) {
    p->bonusAmount++;
    p->lastBonus = 0;
  }
  else if (p->bonusAmount > 0) p->lastBonus++;
}

char makeBet(Player* p, double amount) {
  if (amount > 0 && p->balance >= amount) {
    p->balance -= amount;
    return 1;
  }
  return 0;
}

void addWinnings(Player* p, double amount) {
  if (amount > 0) p->balance += amount;
}

const char* playerInfo(Player* p) {
  char* info = malloc(100 * sizeof(char));
  sprintf(info, "Balance: %f | Spins: %d | Bonuses: %d | Last bonus: %d\033[0K\n", p->balance, p->totalSpins, p->bonusAmount, p->lastBonus);
  return info;
}

