#ifndef PLAYER_H
#define PLAYER_H


typedef struct {
  double balance;
} Player;

Player* initPlayer(double bal);
void freePlayer(Player* p);

#endif
