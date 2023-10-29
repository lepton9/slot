#include "../include/Renderer.h"
#include <string>


void Renderer::renderGrid(FILE* s, std::vector<std::vector<char>> grid) {
  std::string op;
  for (int i = 0; i < grid.size(); i++) {
    for (int j = 0; j < grid.size(); j++) {
      op = op + grid[i][j] + ' ';
    }
    op = op + '\n';
  }
  render(s, op);
}

void Renderer::render(FILE* s, const std::string& str) {
  fprintf(s, "%s", str.c_str());
}

