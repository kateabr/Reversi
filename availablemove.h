#ifndef AVAILABLEMOVE_H
#define AVAILABLEMOVE_H

#include "direction.h"
#include <QList>

class AvailableMove {
public:
  AvailableMove(int ind, QList<Direction> dirs);
  AvailableMove();
  AvailableMove(int ind, Direction dir);
  AvailableMove(const AvailableMove &other);
  AvailableMove(AvailableMove &&other);
  AvailableMove &operator=(const AvailableMove &other);
  AvailableMove &operator=(AvailableMove &&other);

  int getInd();
  const QList<Direction> &getDirs();
  void addDir(Direction dir);
  bool isEmpty();
  int size();
  void eraseDirection(int ind);
  void setInd(int ind);
  int getIndX() const;
  int getIndY() const;

private:
  int moveIndex;
  QList<Direction> directions;
};

#endif // AVAILABLEMOVE_H
