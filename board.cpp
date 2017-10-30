#include "board.h"

Board Board::MakeBoard() {
  Board res;
  res.layout = new Chip[64]();
  res.generateHash();
  return res;
}

const Chip *Board::getLayout() const { return layout; }

Chip Board::getChip(int i, int j) const { return layout[j * 8 + i]; }

Chip Board::getChip(int ind) const { return layout[ind]; }

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
  putChip(3, 4, Chip::Black);
  putChip(4, 3, Chip::Black);
  putChip(3, 3, Chip::White);
  putChip(4, 4, Chip::White);
}

bool Board::layoutIsEmpty() {
  for (int i = 0; i < 64; ++i)
    if (layout[i] != Chip::Empty)
      return false;
  return true;
}

QList<QPair<int, int>> Board::getPossibleMoves() {
  for (int i = 0; i < 64; ++i) {
  }
}

void Board::initChips(Chip user, Chip comp) {
  userChip = user;
  computerChip = comp;
  initializeAvailableMoves();
}

void Board::initializeAvailableMoves() {
  availableMoves.clear();
  if (userChip == Chip::Black) {
    availableMoves.insert(19);
    availableMoves.insert(26);
    availableMoves.insert(37);
    availableMoves.insert(44);
  } else {
    availableMoves.insert(20);
    availableMoves.insert(29);
    availableMoves.insert(34);
    availableMoves.insert(43);
  }
}

bool Board::canPutChip(int ind) { return availableMoves.contains(ind); }

void Board::moveMade(int ind) {
  availableMoves.erase(availableMoves.find(ind));
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

void Board::updateLayout(int x, int y) {
  int xx = x;
  while (xx >= 0) {
    if (getChip(xx - 1, y) == userChip)
      break;
    --xx;
  }
  if (xx >= 0)
    for (int i = xx; i < x; ++i)
      putChip(i, y, userChip);
  xx = x;
  while (xx < 8) {
    if (getChip(xx + 1, y) == userChip)
      break;
    ++xx;
  }
  if (xx < 8)
    for (int i = x + 1; i <= xx; ++i)
      putChip(i, y, userChip);
  int yy = y;
  while (yy >= 0) {
    if (getChip(x, yy - 1) == userChip)
      break;
    --yy;
  }
  if (yy >= 0)
    for (int j = yy; j < y; ++j)
      putChip(x, j, userChip);
  yy = y;
  while (yy < 8) {
    if (getChip(x, yy + 1) == userChip)
      break;
    ++yy;
  }
  if (yy < 8)
    for (int j = y + 1; j <= yy; ++j)
      putChip(x, j, userChip);
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

Board::Board() {
  layout = new Chip[64]();
  putChip(3, 4, Chip::Black);
  putChip(4, 3, Chip::Black);
  putChip(3, 3, Chip::White);
  putChip(4, 4, Chip::White);
  generateHash();
  initializeAvailableMoves();
}
