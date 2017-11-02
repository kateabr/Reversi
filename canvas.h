#ifndef CANVAS_H
#define CANVAS_H

#include "board.h"
#include "computerplayer.h"
#include <QtWidgets>

class Canvas : public QWidget {
  Q_OBJECT
public:
  explicit Canvas(QWidget *parent = nullptr);
  const Board &getBoard();
  void computerPlayerMakeMove();
  void userMakeMove(int x, int y);

signals:
  void initAvailMoves();

public slots:
  void setChips(Chip user, Chip comp);
  void setStartGame(bool s);
  void setDifficulty(int val);
  void clearLayout();

protected:
  void paintEvent(QPaintEvent *);
  void mousePressEvent(QMouseEvent *e);

private:
  Board b;
  ComputerPlayer cp;
  Chip userChip = Chip::Black;
  bool gameStarted = false;
};

#endif // CANVAS_H
