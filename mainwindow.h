#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

public slots:
  void setControlsEnabled(bool val);
  void gameFinished();
  void updateScores(int player, int computer);

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
