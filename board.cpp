#include "board.h"

Board Board::MakeBoard() {
  Board res;
  res.layout = new Chip[64]();
  res.generateHash();
  return res;
}

const Chip *Board::getLayout() const { return layout; }

const Chip &Board::getChip(int i, int j) const { return layout[j * 8 + i]; }

void Board::putChip(int i, int j, Chip ch) { layout[j * 8 + i] = ch; }

size_t Board::getHash() const { return boardHash; }

void Board::print() const {
  QDebug deb = qDebug();
  for (int i = 0; i < 64; ++i)
    deb << static_cast<int>(layout[i]) << " ";
  deb << endl;
}

void Board::clearLayout() {
  for (int i = 0; i < 64; ++i)
    layout[i] = Chip::Empty;
}

bool Board::layoutIsEmpty() {
  for (int i = 0; i < 64; ++i)
    if (layout[i] != Chip::Empty)
      return false;
  return true;
}

bool Board::final() const { return true; }

int Board::heuristic() const { return 0; }

Board::~Board() { delete[] layout; }

Board::Board(const Board &other) {
  layout = new Chip[64];
  boardHash = other.boardHash;
  std::copy(other.layout, other.layout + 64, layout);
}

Board::Board(Board &&other) : layout(other.layout), boardHash(other.boardHash) {
  other.layout = nullptr;
}

Board &Board::operator=(const Board &other) {
  boardHash = other.boardHash;
  std::copy(other.layout, other.layout + 64, layout);
  return *this;
}

Board &Board::operator=(Board &&other) {
  delete[] layout;
  layout = other.layout;
  other.layout = nullptr;
  boardHash = other.boardHash;
  return *this;
}

void Board::generateHash() {
  boardHash = 0;
  std::hash<std::string> strHash;
  std::string tempHash = "";
  for (int i = 0; i < 64; ++i) {
    tempHash += std::to_string(static_cast<int>(layout[i]));
  }
  boardHash = strHash(tempHash);
}

Board::Board() : layout(nullptr) {}
