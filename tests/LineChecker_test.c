#include "../include/LineChecker.h"
#include "../include/Slot.h"
#include "../../testLibC/utestC.h"


void initGridVisited(Slot* s, bool*** visited, const int w, const int h) {
    char gr[5][5] = {
        {'#','#','#','#','%'},
        {'#','&','#','%','#'},
        {'#','&','&','%','#'},
        {'#','#','&','%','#'},
        {'#','#','%','%','%'}
    };

    //*grid = (symbol**)malloc(h * sizeof(symbol*));
    *visited = (bool**)malloc(h * sizeof(bool*));

    for (int i = 0; i < h; i++) {
      (*visited)[i] = (bool*)malloc(w * sizeof(bool));
      memset((*visited)[i], false, w * sizeof(bool));
      for (int j = 0; j < w; j++) {
        s->slotGrid[i][j].st.c = gr[i][j];
        s->slotGrid[i][j].st.colorGroup = (int)(*getSymbColor(s, gr[i][j]));
      }
    }
/**
    for (int i = 0; i < h; i++) {
        (*grid)[i] = (char*)malloc(w * sizeof(char));
        memcpy((*grid)[i], gr[i], w * sizeof(char));

        (*visited)[i] = (bool*)malloc(w * sizeof(bool));
        memset((*visited)[i], false, w * sizeof(bool));
    }
  **/
}

void freeGridVisited(bool*** visited, const int h) {
  for (int i = 0; i < h; i++) {
    //free((*grid)[i]);
    free((*visited)[i]);
  }
  //free(*grid);
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
  Slot* slot = initSlot(w, h);
  bool** visited;

  initGridVisited(slot, &visited, w, h);

  group* g = initGroup();
  findGroup(slot->slotGrid, visited, g, w, h, 1, 1);
  assertf(g->n == 4, "Group size not as expected");
  assertf(g->symb.st.c == '&', "Group symbol not as expected");
  assertf(g->cells[0].x == 1 && g->cells[0].y == 1, "First element in group not as expected");

  group* g1 = initGroup();
  findGroup(slot->slotGrid, visited, g1, w, h, 3, 3);
  assertf(g1->n == 6, "Group size not as expected");
  assertf(g1->symb.st.c == '%', "Group symbol not as expected");

  free(g);
  free(g1);
  freeSlot(slot);
  freeGridVisited(&visited, h);
}

void test_findGroups() {
  int w = 5; int h = 5;
  Slot* slot = initSlot(w, h);
  bool** visited;

  initGridVisited(slot, &visited, w, h);


  groups* grps = findGroups(slot->slotGrid, w, h);
  assertf(grps->n == 2, "Amount of groups not as expected");

  /**
  int cells = 0;
  for (int i = 0; i < grps->n; i++) cells += grps->groups[i]->n;
  assertf(cells == w*h, "Wrong amount of total cells combined");
  **/

  freeGroups(grps);
  freeSlot(slot);
  freeGridVisited(&visited, h);
}

void test_popGroup() {
  int w = 5; int h = 5;
  Slot* slot = initSlot(w, h);
  bool** visited;
  initGridVisited(slot, &visited, w, h);
  group* g = initGroup();
  findGroup(slot->slotGrid, visited, g, w, h, 1, 1);

  char gr[5][5] = {
      {'#','#','#','#','%'},
      {'#',' ','#','%','#'},
      {'#',' ',' ','%','#'},
      {'#','#',' ','%','#'},
      {'#','#','%','%','%'}
  };

  popGroup(slot->slotGrid, g);
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < h; j++) {
      assertEq_char(slot->slotGrid[i][j].st.c, gr[i][j], "Should be same symbols");
    }
  }

  free(g);
  freeSlot(slot);
  freeGridVisited(&visited, h);
}

int main(int argc, char** argv) {
  makeTests(argv[0]);
  // makeTests(__FILE__);
  addTestFunc(test_initGroup);
  addTestFunc(test_initGroups);
  addTestFunc(test_addToGroup);
  addTestFunc(test_addToGroups);
  addTestFunc(test_findGroup);
  addTestFunc(test_findGroups);
  addTestFunc(test_popGroup);
  runTests();
  return 0;
}

