#ifndef RENDERER_H
#define RENDERER_H

#include <istream>
#include <vector>
#include <string>
#include <iostream>

class Renderer {
  public:
    void renderGrid(FILE* s, std::vector<std::vector<char>>);
  private:
    void render(FILE* s, const std::string& str);
};


#endif
