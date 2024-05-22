#ifndef RENDERER_H
#define RENDERER_H

#include <stdio.h>
#include "../include/LineChecker.h"

typedef struct {

} Renderer;


void renderGrid(FILE* s, symbol** grid, int w, int h);
void render(FILE* s, const char* str);

#endif
