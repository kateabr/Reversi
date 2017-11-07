#ifndef BOARD_H
#define BOARD_H

#include "availablemove.h"
#include "chips.h"
#include "current.h"
#include "direction.h"
#include "squareweights.h"
#include <QMap>

using byte = int;

class Board {
public:
  // constructors and assignment operators
  Board();
  ~Board();
  Board(const Board &other);
  Board(Board &&other);
  Board &operator=(const Board &other);
  Board &operator=(Board &&other);

  // chip manipulation
  Chip getChip(int i, int j) const;
  Chip getChip(int ind) const;
  Chip getUserChip();
  bool canPutChip(int x, int y, Current cur);
  bool canPutChip(int ind, Current cur);
  AvailableMove putChip(int i, int j, Current cur);
  AvailableMove putChip(int ind, Current cur);
  bool areThereChips(int ind, Direction dir, Chip chip, Chip enemyChip);

  // massive update functions
  void makeMove(int x, int y);
  void moveMade(AvailableMove &madeMove, Current cur);
  void updateAvailableMoves(Current cur);
  void updateLayouts(AvailableMove &move, Current cur);
  AvailableMove computerMakeMove();

  // utility functions
  bool final(Current cur) const;
  double heuristic() const;
  int playerScore();
  int computerScore();
  QList<AvailableMove> getComputerAvailableMoves() const;
  QList<AvailableMove> getPlayerAvailableMoves() const;

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
  SquareWeights sw;

  int availablePos(int ind, Direction dir, Chip enemyChip);
  void eraseFromLayout(int x, int y, Current cur);
  void addToLayout(int x, int y, Current cur);
  void addPossibleMovesForPosition(int pos, Chip enemyChip, Current cur,
                                   QList<AvailableMove> &avm);
  int alreadyAvailable(int pos, Current cur);
};

#endif // BOARD_H
