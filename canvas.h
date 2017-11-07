#ifndef CANVAS_H
#define CANVAS_H

#include "board.h"
#include "computeropponent.h"
#include <QtWidgets>

class Canvas : public QWidget {
  Q_OBJECT
public:
  explicit Canvas(QWidget *parent = nullptr);
  const Board &getBoard();
  void computerPlayerMakeMove();
  void userMakeMove(int x, int y);
  int playerScore();
  int computerScore();
  void setDifficulty(int val);

public slots:
  void setChips(Chip user, bool initLayout);
  void setStartGame(bool s);
  void clearLayout();

signals:
  void gameFinished();

protected:
  void paintEvent(QPaintEvent *);
  void mousePressEvent(QMouseEvent *e);

private:
  Board b;
  ComputerOpponent cp;
  Chip userChip = Chip::Black;
  bool gameStarted = false;
};

#endif // CANVAS_H
