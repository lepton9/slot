#ifndef LINECHECKER_H
#define LINECHECKER_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/symbols.h"

#define MIN_GROUP_SIZE 6

typedef struct {

} LineChecker;

typedef struct {
  uint8_t x,y;
} cell;

typedef struct {
  cell cells[64];
  symbol symb;
  size_t n;
} group;

typedef struct {
  group* groups[128];
  size_t n;
} groups;

group* initGroup();
groups* initGroups();
void freeGroups(groups* grps);
void addToGroup(group* g, cell c);
void addToGroups(groups* grps, group* g);
void findGroup(symbol** grid, bool** visited, group* g, const int w, const int h, const int i, const int j);
groups* findGroups(symbol** grid, const int w, const int h);
void popGroup(symbol** grid, group* g);

int countSymbols(symbol** grid, int w, int h, char symbol);
int countSymbolsCol(symbol** grid, int h, int column, char symbol);
double calcGroupMulti(group* g);
double calcReturn(groups* grps, const double bet);

#endif

