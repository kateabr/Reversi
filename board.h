#ifndef BOARD_H
#define BOARD_H

#include "availablemove.h"
#include "chips.h"
#include "current.h"
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
  bool final();
  int heuristic() const;
  const Chip *getLayout() const;
  Chip getChip(int i, int j) const;
  Chip getChip(int ind) const;
  AvailableMove putChip(int i, int j, Current cur);
  AvailableMove putChip(int ind, Current cur);
  size_t getHash() const;
  void print() const;
  bool layoutIsEmpty();
  bool canPutChip(int ind, Current cur);
  bool canPutChip(int x, int y, Current cur);
  void updateAvailableMoves(Current cur);
  Chip getUserChip();
  int availablePos(int ind, Direction dir, Chip enemyChip);
  AvailableMove computerMakeMove();

  Board();
  ~Board();
  Board(const Board &other);
  Board(Board &&other);
  Board &operator=(const Board &other);
  Board &operator=(Board &&other);
  void updateLayouts(AvailableMove &move, Current cur);
  bool areThereChips(int ind, Direction dir, Chip chip, Chip enemyChip);
  QList<int> getChipsToUpdateLayout(int x, int y, Chip user);

  void initializeAvailableMoves(Chip user, QMap<int, QList<Direction>> &avm,
                                QSet<int> &pl);
  void updatePlayerLayout(const QList<int> &takenChips, bool add = true);
  void initAvM();
  void updAvM();
  void moveMade(AvailableMove &madeMove, Current cur);
  int playerScore();
  int computerScore();

public slots:
  void initializeLayouts();
  void changeChips();

private:
  Chip playerChip = Chip::Black;
  Chip computerChip = Chip::White;
  QSet<int> playerLayout;
  QSet<int> computerLayout;
  QList<AvailableMove> playerAvailableMoves;
  QList<AvailableMove> computerAvailableMoves;

  size_t boardHash;
  void generateHash();

  void eraseFromLayout(int x, int y, Current cur);
  void addToLayout(int x, int y, Current cur);
  void addPossibleMovesForPosition(int pos, Chip enemyChip, Current cur,
                                   QList<AvailableMove> &avm);
  int alreadyAvailable(int pos, Current cur);
};

#endif // BOARD_H
