#ifndef CANVAS_H
#define CANVAS_H

#include "board.h"
#include <QtWidgets>

class Canvas : public QWidget {
  Q_OBJECT
public:
  explicit Canvas(QWidget *parent = nullptr);

public slots:
  void setUserChips(Chip ch);
  void setStartGame(bool s);

protected:
  void paintEvent(QPaintEvent *);
  void mousePressEvent(QMouseEvent *e);

private:
  Board b;
  Chip userChip = Chip::Black;
  bool gameStarted = false;
};

#endif // CANVAS_H
