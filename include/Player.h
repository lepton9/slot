#ifndef PLAYER_H
#define PLAYER_H


typedef struct {
  double balance;
  int totalSpins;
  int bonusAmount;
  int lastBonus;
} Player;

Player* initPlayer(double bal);
void freePlayer(Player* p);

void addSpin(Player* p, char bonus);
char makeBet(Player* p, double amount);
void addWinnings(Player* p, double amount);
const char* playerInfo(Player* p);

#endif
