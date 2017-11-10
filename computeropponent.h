#ifndef COMPUTEROPPONENT_H
#define COMPUTEROPPONENT_H

#include "board.h"

class ComputerOpponent : public QObject {
  Q_OBJECT

public:
  ComputerOpponent(QObject *parent = nullptr);

  AvailableMove makeMove(Board &b);
  AvailableMove makeRandomMove(Board &b);

  void setDifficulty(int val);

signals:
  void wait();
  void finished();

private:
  int difficulty = 0;

  QPair<double, int> findOptimalMove(const Board &b, int depth,
                                     bool maximizingPlayer);
};

#endif // COMPUTEROPPONENT_H
