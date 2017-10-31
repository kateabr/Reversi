#ifndef COMPUTERPLAYER_H
#define COMPUTERPLAYER_H

#include "board.h"

class ComputerPlayer {
public:
  ComputerPlayer(Board &bb);
  void initChips(Chip user, Chip comp);
  bool computerCanPutChip(int ind);
  void updateAvailableMoves();

public slots:
  void setDifficulty(int val);
  void initializeAvailableMoves();

private:
  Board *b; // copy of original board
  Chip computerChip = Chip::White;
  Chip userChip = Chip::Black;
  int difficulty = 1;
  QMap<int, Direction> availableMoves;

  int canPutChipRight(int ind);
  int canPutChipLeft(int ind);
  int canPutChipUp(int ind);
  int canPutChipDown(int ind);
  void moveMade(int ind);
};

#endif // COMPUTERPLAYER_H
