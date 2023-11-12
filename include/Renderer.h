#ifndef RENDERER_H
#define RENDERER_H

#include <stdio.h>

typedef struct {

} Renderer;

void renderGrid(FILE* s, char** grid, int w, int h);
void render(FILE* s, const char* str);

#endif
