#include "../include/LineChecker.h"
#include "testLib.h"


void initGridVisited(char*** grid, bool*** visited, const int w, const int h) {
    char gr[5][5] = {
        {'#','#','#','#','%'},
        {'#','&','#','%','#'},
        {'#','&','&','%','#'},
        {'#','#','&','%','#'},
        {'#','#','#','%','%'}
    };

    *grid = (char**)malloc(h * sizeof(char*));
    *visited = (bool**)malloc(h * sizeof(bool*));

    for (int i = 0; i < h; i++) {
        (*grid)[i] = (char*)malloc(w * sizeof(char));
        memcpy((*grid)[i], gr[i], w * sizeof(char));

        (*visited)[i] = (bool*)malloc(w * sizeof(bool));
        memset((*visited)[i], false, w * sizeof(bool));
    }
}

void freeGridVisited(char*** grid, bool*** visited, const int h) {
  for (int i = 0; i < h; i++) {
    free((*grid)[i]);
    free((*visited)[i]);
  }
  free(*grid);
  free(*visited);
}


void test_initGroup() {
  group* g = initGroup();
  assertf(g->n == 0, "Group size should be 0 at init");
  free(g);
}

void test_initGroups() {
  groups* grps = initGroups();
  assertf(grps->n == 0, "Groups size should be 0 at init");
  free(grps);
}

void test_addToGroup() {
  group* g = initGroup();
  addToGroup(g, (cell){0,0});
  assertf(g->n == 1, "Group size should increase by 1");
  addToGroup(g, (cell){3,1});
  assertf(g->n == 2, "Group size should increase by 1");
  free(g);
}

void test_addToGroups() {
  groups* grps = initGroups();
  
  addToGroups(grps, &(group){});
  assertf(grps->n == 1, "Groups size should increase by 1");
  
  addToGroups(grps, &(group){});
  assertf(grps->n == 2, "Groups size should increase by 1");
  free(grps);
}

void test_findGroup() {
  int w = 5; int h = 5;
  char** grid;
  bool** visited;

  initGridVisited(&grid, &visited, w, h);

  group* g = initGroup();
  findGroup(grid, visited, g, w, h, 1, 1);
  assertf(g->n == 4, "Group size not as expected");
  assertf(g->symb == '&', "Group symbol not as expected");
  assertf(g->cells[0].x == 1 && g->cells[0].y == 1, "First element in group not as expected");

  group* g1 = initGroup();
  findGroup(grid, visited, g1, w, h, 3, 3);
  assertf(g1->n == 5, "Group size not as expected");
  assertf(g1->symb == '%', "Group symbol not as expected");

  free(g);
  free(g1);
  freeGridVisited(&grid, &visited, h);
}

void test_findGroups() {
  int w = 5; int h = 5;
  char** grid;
  bool** visited;

  initGridVisited(&grid, &visited, w, h);

  groups* grps = findGroups(grid, w, h);
  assertf(grps->n == 5, "Amount of groups not as expected");

  int cells = 0;
  for (int i = 0; i < grps->n; i++) cells += grps->groups[i]->n;
  assertf(cells == w*h, "Wrong amount of total cells combined");

  freeGroups(grps);
  freeGridVisited(&grid, &visited, h);
}


void run_tests() {
  test_initGroup();
  test_initGroups();
  test_addToGroup();
  test_addToGroups();
  test_findGroup();
  test_findGroups();
}

int main(int argc, char** argv) {
  makeTests(argv[0]);
  run_tests();
  printLogs();
  return 0;
}

