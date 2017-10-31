#ifndef BOARD_H
#define BOARD_H

#include "chips.h"
#include "direction.h"
#include <QDebug>
#include <QMap>
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
  void putChip(int i, int j, Chip ch, bool push = true);
  void putChip(int ind, Chip ch, bool push = true);
  size_t getHash() const;
  void print() const;
  void clearLayout();
  bool layoutIsEmpty();
  void initChips(Chip user, Chip comp);
  bool canPutChip(int ind);
  void moveMade(int ind);
  void updateAvailableMoves(Chip user, Chip comp, QMap<int, Direction> &avm,
                            QSet<int> pl);
  Chip getUserChip();
  void addPossibleMoves(int pos, Chip user, Chip comp,
                        QMap<int, Direction> &avm);
  int availablePos(int ind, Direction dir, Chip user, Chip comp);

  Board();
  ~Board();
  Board(const Board &other);
  Board(Board &&other);
  Board &operator=(const Board &other);
  Board &operator=(Board &&other);
  QList<int> updateLayout(int x, int y, Chip user);
  bool areThereChips(int ind, Direction dir, Chip user, Chip comp);

  void initializeAvailableMoves(Chip user, QMap<int, Direction> &avm,
                                QSet<int> &pl);
  void updatePlayerLayout(const QList<int> &takenChips, bool add = true);
  void initAvM();
  void updAvM();

private:
  QSet<int> playerLayout;
  Chip *layout;
  Chip userChip = Chip::Black;
  Chip computerChip = Chip::White;
  size_t boardHash;
  void generateHash();
  QMap<int, Direction> availableMoves;
};

#endif // BOARD_H
