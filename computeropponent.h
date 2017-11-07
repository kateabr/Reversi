#ifndef COMPUTEROPPONENT_H
#define COMPUTEROPPONENT_H

#include "board.h"

class ComputerOpponent {
public:
  ComputerOpponent();

  AvailableMove makeMove(Board &b);
  AvailableMove makeRandomMove(Board &b);

  void setDifficulty(int val);

private:
  int difficulty = 0;

  QPair<double, int> findOptimalMove(const Board &b, int depth,
                                     bool maximizingPlayer);
};

#endif // COMPUTEROPPONENT_H
