#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>

#define PLAYER_FILENAME "slot_player.txt"
#define MAX_LEADERBOARD 100

typedef struct {
  double balance;
  int totalSpins;
  int bonusAmount;
  int lastBonus;
  double bestBonusMulti[MAX_LEADERBOARD];
  int lbN;
} Player;

Player* initPlayer(double bal);
void freePlayer(Player* p);

void addSpin(Player* p, char bonus);
char makeBet(Player* p, double amount);
void addWinnings(Player* p, double amount);
const char* playerInfo(Player* p);
int addToLb(Player *p, double multi);
Player* loadPlayer();
char savePlayer(Player* p);
void printMultiLB(FILE* file, Player* p);

#endif
