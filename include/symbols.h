#ifndef SYMBOLS_H
#define SYMBOLS_H

typedef struct {
  char c;
  int colorGroup;
  double baseGroupMulti;
  double perSymbMulti;
  double probability;
} symbolTemplate;

typedef struct {
  symbolTemplate st;
  bool inGroup;
} symbol;

enum groupColors {
Red = 31, Green, Yellow, Blue, Magenta, Cyan, White
};

enum symbols {
  BONUS = 'B', 
  WILD = 'W',
  C = '#', D = '&', E = '*', BLANK = ' '
};

#endif
