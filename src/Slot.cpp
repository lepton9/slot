#include "../include/Slot.h"
#include <cstdio>
#include <assert.h>
#include <locale>


void Slot::spin() {
  for (int i = 0; i < slotGrid.size(); i++) {
    spinColumn(i);
  }
}

void Slot::spinColumn(int column) {
  for (int i = 0; i < slotGrid.size(); i++) {
    slotGrid[i][column] = getRandomSymbol();
  }
}

void Slot::animateSpin() {

}

char Slot::getRandomSymbol() {
  int i = randAlias(alias);
  assert(i >= 0);
  return symbols[i];
}

void Slot::checkProb() {
  assert(sizeof(prob)/sizeof(prob[0]) == sizeof(symbols)/sizeof(symbols[0]));

  double sum;
  for (int i = 0; i < sizeof(prob)/sizeof(prob[0]); i++) {
    sum += prob[i];
  }
  //assert(sum == 1);  
}

void Slot::initSlot(int w, int h) {
  slotGrid = std::vector<std::vector<char>>(h, std::vector<char>(w, '#'));

  alias = initialize(prob, sizeof(prob)/sizeof(prob[0]));
}


int main() {
  Slot s;
  s.initSlot(10,10);
  s.render->renderGrid(stdout, s.slotGrid);
  s.checkProb();
  s.spin();
  printf("\n");
  s.render->renderGrid(stdout, s.slotGrid);

  return 0;
}

