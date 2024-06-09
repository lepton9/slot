#include "../include/Player.h"
#include <../../testLibC/utestC.h>

void test_addSpin() {
  Player *p = initPlayer(1000);
  assertEq_int(p->totalSpins, 0, "Spin amount");
  addSpin(p, 0);
  assertEq_int(p->totalSpins, 1, "Spin amount");
  assertEq_int(p->bonusAmount, 0, "Bonus amount");
  addSpin(p, 1);
  assertEq_int(p->totalSpins, 2, "Spin amount");
  assertEq_int(p->bonusAmount, 1, "Bonus amount");
  assertEq_int(p->lastBonus, 0, "Bonus amount");
  addSpin(p, 0);
  assertEq_int(p->totalSpins, 3, "Spin amount");
  assertEq_int(p->bonusAmount, 1, "Bonus amount");
  assertEq_int(p->lastBonus, 1, "Bonus amount");
  freePlayer(p);
}

void test_makeBet() {
  Player *p = initPlayer(1000);
  char ret;
  assertEq_double(p->balance, 1000, "Player balance");
  ret = makeBet(p, 100);
  assertEq_double(p->balance, 900, "Player balance");
  assertEq_char(ret, 1, "Bet success");
  ret = makeBet(p, 500.50);
  assertEq_double(p->balance, 399.50, "Player balance");
  ret = makeBet(p, 400);
  assertEq_double(p->balance, 399.50, "Player balance");
  assertEq_char(ret, 0, "Bet success");
  freePlayer(p);
}

void test_addWinnings() {
  Player *p = initPlayer(1000);
  assertEq_double(p->balance, 1000, "Player balance");
  addWinnings(p, 100);
  assertEq_double(p->balance, 1100, "Player balance");
  addWinnings(p, 0);
  assertEq_double(p->balance, 1100, "Player balance");
  addWinnings(p, -100);
  assertEq_double(p->balance, 1100, "Player balance");
  addWinnings(p, 1);
  assertEq_double(p->balance, 1101, "Player balance");
  freePlayer(p);
}

void test_addToLb() {
  Player *p = initPlayer(1000);

  assertEq_int(p->lbN, 0, "Init size");
  addToLb(p, 10.0);
  assertEq_double(p->bestBonusMulti[0], 10.0, "Element value");
  addToLb(p, 12.0);
  assertEq_double(p->bestBonusMulti[0], 12.0, "Element value");
  assertEq_double(p->bestBonusMulti[1], 10.0, "Element value");
  addToLb(p, 11.0);
  assertEq_double(p->bestBonusMulti[0], 12.0, "Element value");
  assertEq_double(p->bestBonusMulti[1], 11.0, "Element value");
  assertEq_double(p->bestBonusMulti[2], 10.0, "Element value");
  assertEq_int(p->lbN, 3, "Array size");
  freePlayer(p);
}

int main(int argc, char **argv) {
  makeTests(argv[0]);
  addTestFunc(test_addSpin);
  addTestFunc(test_makeBet);
  addTestFunc(test_addWinnings);
  addTestFunc(test_addToLb);
  runTests();
  return 0;
}
