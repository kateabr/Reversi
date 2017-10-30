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
  Chip getChip(int i, int j) const;
  Chip getChip(int ind) const;
  void putChip(int i, int j, Chip ch);
  size_t getHash() const;
  void print() const;
  void clearLayout();
  bool layoutIsEmpty();
  QList<QPair<int, int>> getPossibleMoves();
  void initChips(Chip user, Chip comp);
  bool canPutChip(int ind);
  void moveMade(int ind);

  Board();
  ~Board();
  Board(const Board &other);
  Board(Board &&other);
  Board &operator=(const Board &other);
  Board &operator=(Board &&other);
  void updateLayout(int x, int y);

public slots:
  void initializeAvailableMoves();

private:
  Chip *layout;
  Chip userChip = Chip::Black;
  Chip computerChip = Chip::White;
  size_t boardHash;
  void generateHash();
  QSet<int> availableMoves;
};

#endif // BOARD_H
