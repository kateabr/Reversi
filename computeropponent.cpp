#include "computeropponent.h"
#include <QMap>

ComputerOpponent::ComputerOpponent() {}

AvailableMove ComputerOpponent::makeRandomMove(Board &b) {
  int rnd = rand() % b.getComputerAvailableMoves().size();
  return b.getComputerAvailableMoves()[rnd];
}

AvailableMove ComputerOpponent::makeMove(Board &b) {
  if (!difficulty) {
    return makeRandomMove(b);
  }

  int moveToMake = findOptimalMove(b, difficulty, true).second;

  return b.getComputerAvailableMoves()[moveToMake];
}

void ComputerOpponent::setDifficulty(int val) { difficulty = val; }

QPair<double, int> ComputerOpponent::findOptimalMove(const Board &b, int depth,
                                                     bool maximizingPlayer) {
  if ((depth == 0) || b.final(Current::Computer) || b.final(Current::User))
    return QPair<double, int>(b.heuristic(), -1);

  QPair<double, int> bestValue;
  QPair<double, int> childValue;
  if (maximizingPlayer) {
    bestValue.first = -1000000;
    for (int i = 0; i < b.getComputerAvailableMoves().size(); ++i) {
      AvailableMove move = b.getComputerAvailableMoves()[i];
      Board bChild(b);
      AvailableMove move2 =
          bChild.putChip(move.getIndX(), move.getIndY(), Current::Computer);
      bChild.moveMade(move2, Current::Computer);
      childValue = findOptimalMove(bChild, depth - 1, false);
      if (bestValue.first < childValue.first) {
        bestValue.first = childValue.first;
        bestValue.second = i;
      }
    }
    return bestValue;
  } else {
    bestValue.first = 1000000;
    for (auto move : b.getPlayerAvailableMoves()) {
      Board bChild(b);
      bChild.makeMove(move.getIndX(), move.getIndY());
      childValue = findOptimalMove(bChild, depth - 1, true);
      bestValue = bestValue.first > childValue.first ? bestValue : childValue;
    }
    return bestValue;
  }
}
