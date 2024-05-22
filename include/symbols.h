#ifndef SYMBOLS_H
#define SYMBOLS_H

typedef struct {
  char s;
  int color;
} symbolTemplate;

typedef struct {
  char c;
  int colorGroup;
  bool inGroup;
} symbol;

enum groupColors {
  Red = 31, Green, Yellow, Blue, Magenta, Cyan, White
};

enum symbols {
  BONUS = 'B', 
  WILD = 'W',
  C = '#', D = '&', E = '*'
};

#endif
