#include "../include/Renderer.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void renderGrid(FILE *s, symbol **grid, int w, int h) {
  // assert(w*h*2 < 512);
  size_t size = 2 * w * h;
  char op[size];

  memset(op, 0, size * sizeof(char));

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      char c[2] = {grid[i][j].st.c, '\0'};
      strcat(op, c);
      strcat(op, " ");
    }
    strcat(op, "\n");
  }
  render(s, op);
}

void render(FILE *s, const char *str) {
  fprintf(s, "%s", str);
  fflush(s);
}
