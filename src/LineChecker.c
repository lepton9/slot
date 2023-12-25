#include "../include/LineChecker.h"
#include <stdio.h>
#include <string.h>

#define MIN_GROUP_SIZE 1


group* initGroup() {
  group* g = (group*)malloc(sizeof(group));
  g->n = 0;
  g->symb = -1;
  memset(g->cells, 0, 64 * sizeof(cell));
  return g;
}

groups* initGroups() {
  groups* grps = (groups*)malloc(sizeof(groups));
  grps->n = 0;
  memset(grps->groups, 0, 32 * sizeof(group*));
  return grps;
}

void freeGroups(groups* grps) {
  for (int i = 0; i < grps->n; i++) free(grps->groups[i]);
  free(grps);
}

void addToGroup(group* g, cell c) {
  g->cells[g->n++] = c;
}

void addToGroups(groups* grps, group* g) {
  grps->groups[grps->n++] = g;
}

void findGroup(char** grid, bool** visited, group* g, const int w, const int h, const int i, const int j) {
  if (g->symb < 0) memcpy(&(g->symb), &(grid[i][j]), sizeof(char));
  if (i >= w || j >= h || i < 0 || j < 0 || visited[i][j] || grid[i][j] != g->symb) return;
  visited[i][j] = true;
  cell c = {i,j};
  addToGroup(g, c);
  findGroup(grid, visited, g, w, h, i+1 ,j);
  findGroup(grid, visited, g, w, h, i ,j+1);
  findGroup(grid, visited, g, w, h, i-1 ,j);
  findGroup(grid, visited, g, w, h, i ,j-1);
}

groups* findGroups(char** grid, const int w, const int h) {
  bool** visited = (bool**)malloc(h*sizeof(bool*));
  for (int i = 0; i < h; i++) {
    visited[i] = (bool*)malloc(w*sizeof(bool));
    for (int j = 0; j < w; j++) {
      visited[i][j] = false;
    }
  }

  groups* grps = initGroups();
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (visited[i][j]) continue;
      group* g = initGroup();
      //g->symb = grid[i][j];
      findGroup(grid, visited, g, w, h, i, j);
      if (g->n < MIN_GROUP_SIZE) free(g);
      else addToGroups(grps, g);
    }
  }
  return grps;
}


int countSymbols(char** grid, int w, int h, char symbol) {
  int sum = 0;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      sum += (grid[i][j] == symbol) ? 1 : 0;
    }
  }
  return sum;
}

int countSymbolsCol(char** grid, int h, int column, char symbol) {
  int sum = 0;
  for (int i = 0; i < h; i++) {
      sum += (grid[i][column] == symbol) ? 1 : 0;
  }
  return sum;
}

