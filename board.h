#ifndef BOARD_H
#define BOARD_H

#include "chips.h"
#include <QDebug>
#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

using byte = int;

class Board {
public:
  static Board MakeBoard();
  bool final() const;
  int heuristic() const;
  const Chip *getLayout() const;
  const Chip &getChip(int i, int j) const;
  void putChip(int i, int j, Chip ch);
  size_t getHash() const;
  void print() const;
  void clearLayout();

  Board();
  ~Board();
  Board(const Board &other);
  Board(Board &&other);
  Board &operator=(const Board &other);
  Board &operator=(Board &&other);

private:
  Chip *layout;
  size_t boardHash;
  void generateHash();
  size_t whiteChips = 0;
  size_t blackChips = 0;
};

#endif // BOARD_H
