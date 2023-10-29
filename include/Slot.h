#ifndef SLOT_H
#define SLOT_H

#include <stdio.h>
#include <vector>
#include "../include/Player.h"
#include "../include/Renderer.h"
#include "../include/LineChecker.h"
#include "../include/AliasTable.h"

typedef struct {
  char c;
  int prob;
} t;

class Slot {
  public:
    Player* p;
    LineChecker* check;
    Renderer* render;
    void initSlot(int w, int h);
    std::vector<std::vector<char>> slotGrid;
    void checkProb();
    void spin();

  private:
    AliasTable* alias;

    char symbols[5] = {'B', 'W', '#', '&', '*'};
    double prob[5] = {0.1, 0.2, 0.3, 0.25, 0.4};
    void spinColumn(int column);
    void animateSpin();
    char getRandomSymbol();

};


#endif
