#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  connect(ui->blackChips, &QRadioButton::clicked,
          [&]() { ui->frame->setChips(Chip::Black, Chip::White); });
  connect(ui->whiteChips, &QRadioButton::clicked,
          [&]() { ui->frame->setChips(Chip::White, Chip::Black); });
  connect(ui->difficulty, &QSlider::valueChanged, [&](int val) {
    ui->groupBox_2->setTitle("Difficulty: " + QString::number(val));
  });
  connect(ui->startGame, &QPushButton::clicked, [&]() {
    setControlsEnabled(false);
    ui->frame->repaint();
    ui->frame->setDifficulty(ui->difficulty->value());
  });
  connect(ui->endGame, &QPushButton::clicked,
          [&]() { setControlsEnabled(true); });
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
