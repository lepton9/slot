#include "../include/LineChecker.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MIN_GROUP_SIZE 3


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
  memset(grps->groups, 0, 128 * sizeof(group*));
  return grps;
}

void freeGroups(groups* grps) {
  for (int i = 0; i < grps->n; i++) {
    if (grps->groups[i] != NULL) free(grps->groups[i]);
  }
  free(grps);
}

void addToGroup(group* g, cell c) {
  g->cells[g->n++] = c;
}

void addToGroups(groups* grps, group* g) {
  grps->groups[grps->n++] = g;
}

void findGroup(symbol** grid, bool** visited, group* curGrp, const int w, const int h, const int i, const int j) {
  if (i >= w || j >= h || i < 0 || j < 0 || visited[i][j] || grid[i][j].inGroup) return;
  if (curGrp->n == 0) memcpy(&(curGrp->symb), &(grid[i][j].c), sizeof(char));

  if (grid[i][j].c == curGrp->symb || grid[i][j].c == WILD) {
    cell c = {i,j};
    addToGroup(curGrp, c);
    grid[i][j].inGroup = true;
    grid[i][j].colorGroup = grid[curGrp->cells[0].x][curGrp->cells[0].y].colorGroup;
    visited[i][j] = true;

    findGroup(grid, visited, curGrp, w, h, i+1, j  );
    findGroup(grid, visited, curGrp, w, h, i  , j+1);
    findGroup(grid, visited, curGrp, w, h, i-1, j  );
    findGroup(grid, visited, curGrp, w, h, i  , j-1);
  }
}

groups* findGroups(symbol** grid, const int w, const int h) {
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
      if (grid[i][j].inGroup || visited[i][j] || grid[i][j].c == WILD) continue;
      group* g = initGroup();
      //memcpy(&(g->symb), &(grid[i][j].c), sizeof(char));
      findGroup(grid, visited, g, w, h, i, j);
      if (g->n < MIN_GROUP_SIZE){
        for (int i = 0; i < g->n; i++) grid[g->cells[i].x][g->cells[i].y].inGroup = false;
        free(g);
      }
      else addToGroups(grps, g);
      //visited[i][j] = true;
    }
  }
  for (int i = 0; i < h; i++) free(visited[i]);
  free(visited);

  return grps;
}




int countSymbols(symbol** grid, int w, int h, char symbol) {
  int sum = 0;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      sum += (grid[i][j].c == symbol) ? 1 : 0;
    }
  }
  return sum;
}

int countSymbolsCol(symbol** grid, int h, int column, char symbol) {
  int sum = 0;
  for (int i = 0; i < h; i++) {
      sum += (grid[i][column].c == symbol) ? 1 : 0;
  }
  return sum;
}

