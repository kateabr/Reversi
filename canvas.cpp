#include "canvas.h"
#include <QtConcurrent/QtConcurrent>

Canvas::Canvas(QWidget *parent) : QWidget(parent), cp(b) {}

void Canvas::setChips(Chip user, Chip comp) {
  b.initChips(user, comp);
  cp.initChips(user, comp);
  userChip = user;
  repaint();
}

void Canvas::setStartGame(bool s) {
  gameStarted = s;
  if (!s) {
    b.clearLayout();
    b.initializeAvailableMoves();
    cp.initializeAvailableMoves();
  }
}

void Canvas::setDifficulty(int val) { cp.setDifficulty(val); }

void Canvas::paintEvent(QPaintEvent *) {
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);
  p.setBrush(Qt::black);
  p.setPen(QPen(Qt::black, 2));
  for (int i = 0; i < 8; ++i) {
    p.drawLine(1 + i * (width() / 8), 1, 1 + i * (width() / 8), height() - 1);
  }
  p.drawLine(width() - 1, 1, width() - 1, height() - 1);

  for (int i = 0; i < 8; ++i) {
    p.drawLine(1, 1 + i * (height() / 8), width() - 1, 1 + i * (height() / 8));
  }
  p.drawLine(1, height() - 1, width() - 1, height() - 1);

  float radX = (width() * 1.0) / 16;
  float radY = (height() * 1.0) / 16;

  p.setPen(QPen(Qt::gray, 2));

  for (int j = 0; j < 8; ++j)
    for (int i = 0; i < 8; ++i) {
      if (b.getChip(i, j) == Chip::Empty) {
        if (b.canPutChip(j * 8 + i)) {
          p.setBrush(Qt::green);
          p.drawEllipse(QPointF((2 * i + 1) * radX, (2 * j + 1) * radY), 10,
                        10);
        } else if (cp.computerCanPutChip(j * 8 + i)) {
          p.setBrush(Qt::red);
          p.drawEllipse(QPointF((2 * i + 1) * radX, (2 * j + 1) * radY), 10,
                        10);
        }
        continue;
      }
      if (b.getChip(i, j) == Chip::White)
        p.setBrush(Qt::white);
      else
        p.setBrush(Qt::black);
      p.drawEllipse(QPointF((2 * i + 1) * radX, (2 * j + 1) * radY), radX - 10,
                    radY - 10);
    }
}

void Canvas::mousePressEvent(QMouseEvent *e) {
  if (!gameStarted)
    return;
  float squareWidth = (width() * 1.0) / 8;
  QPoint curPos = e->pos();
  size_t x = curPos.x() / squareWidth;
  size_t y = curPos.y() / squareWidth;
  if ((b.getChip(x, y) != Chip::Empty) || (!b.canPutChip(y * 8 + x)))
    return;
  b.putChip(x, y, userChip);
  b.moveMade(y * 8 + x);
  cp.updateAvailableMoves(y * 8 + x);
  b.updateLayout(x, y);
  repaint();
}
