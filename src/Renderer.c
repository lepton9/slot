#include "../include/Renderer.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>


void renderGrid(FILE* s, char** grid, int w, int h) {
  assert(w*h*2 < 512);
  char op[512] = "";
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      char c[2] = {grid[i][j], '\0'};
      strcat(op, c);
      strcat(op, " ");
    }
    strcat(op, "\n");
  }
  render(s, op);
}


void render(FILE* s, const char* str) {
  fprintf(s, "%s", str);
}

