#ifndef CANVAS_H
#define CANVAS_H

#include "board.h"
#include "computerplayer.h"
#include <QtWidgets>

class Canvas : public QWidget {
  Q_OBJECT
public:
  explicit Canvas(QWidget *parent = nullptr);

public slots:
  void setChips(Chip user, Chip comp);
  void setStartGame(bool s);
  void setDifficulty(int val);

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
