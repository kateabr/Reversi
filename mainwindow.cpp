#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  srand(time(0));
  connect(ui->blackChips, &QRadioButton::clicked,
          [&]() { ui->frame->setChips(Chip::Black, true); });
  connect(ui->whiteChips, &QRadioButton::clicked,
          [&]() { ui->frame->setChips(Chip::White, true); });
  connect(ui->difficulty, &QSlider::valueChanged, [&](int val) {
    ui->groupBox_2->setTitle("Difficulty: " + QString::number(val));
    ui->frame->setDifficulty(val);
  });
  connect(ui->startGame, &QPushButton::clicked, [&]() {
    ui->frame->clearLayout();
    ui->frame->repaint();
    ui->playerScore->setText("2");
    ui->computerScore->setText("2");
    setControlsEnabled(false);
  });
  connect(ui->endGame, &QPushButton::clicked,
          [&]() { setControlsEnabled(true); });
  connect(ui->frame, &Canvas::gameFinished, this, &MainWindow::gameFinished);
  connect(ui->frame, &Canvas::updateScores, this, &MainWindow::updateScores);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setControlsEnabled(bool val) {
  ui->frame->setStartGame(!val);
  ui->difficulty->setEnabled(val);
  ui->blackChips->setEnabled(val);
  ui->whiteChips->setEnabled(val);
  ui->startGame->setEnabled(val);
  ui->endGame->setEnabled(!val);
}

void MainWindow::gameFinished() {
  QString score = QString::number(ui->frame->playerScore()) + ":" +
                  QString::number(ui->frame->computerScore());
  QString winner;
  if (ui->frame->playerScore() > ui->frame->computerScore())
    winner = "\nYou win";
  else if (ui->frame->playerScore() < ui->frame->computerScore())
    winner = "\nComputer wins";
  else
    winner = "Tie";
  QMessageBox::information(this, "No moves left",
                           "Game over\nScore: " + score + winner,
                           QMessageBox::Ok);
  setControlsEnabled(true);
}

void MainWindow::updateScores(int player, int computer) {
  ui->playerScore->setText(QString::number(player));
  ui->computerScore->setText(QString::number(computer));
}
