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

void Board::initChips(Chip user, Chip comp) {
  userChip = user;
  computerChip = comp;
  initializeAvailableMoves(userChip, availableMoves);
}

void Board::initializeAvailableMoves(Chip user, QMap<int, Direction> &avm) {
  avm.clear();
  if (user == Chip::Black) {
    avm.insert(19, Direction::Down);
    avm.insert(26, Direction::Right);
    avm.insert(37, Direction::Left);
    avm.insert(44, Direction::Up);
  } else {
    avm.insert(20, Direction::Down);
    avm.insert(29, Direction::Left);
    avm.insert(34, Direction::Right);
    avm.insert(43, Direction::Up);
  }
}

void Board::initAvM() { initializeAvailableMoves(userChip, availableMoves); }

bool Board::canPutChip(int ind) { return availableMoves.contains(ind); }

void Board::moveMade(int ind) {
  availableMoves.erase(availableMoves.find(ind));
}

void Board::updateAvailableMoves() {
  int ind = 0;
  while ((!availableMoves.empty()) && (ind != availableMoves.size())) {
    if (!areThereChips((availableMoves.begin() + ind).key(),
                       (availableMoves.begin() + ind).value(), userChip,
                       computerChip)) {
      availableMoves.erase(availableMoves.begin() + ind);
    } else
      ++ind;
  }
}

Chip Board::getUserChip() { return userChip; }

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
  // QSet<Direction> res;

  // left
  int xx = x;
  while (xx >= 0) {
    if (getChip(xx - 1, y) == userChip)
      break;
    --xx;
  }
  if (xx >= 0) {
    // res.insert(Direction::Left);
    for (int i = xx; i < x; ++i)
      putChip(i, y, userChip);
  }

  // right
  xx = x;
  while (xx < 8) {
    if (getChip(xx + 1, y) == userChip)
      break;
    ++xx;
  }
  if (xx < 8) {
    // res.insert(Direction::Right);
    for (int i = x + 1; i <= xx; ++i)
      putChip(i, y, userChip);
  }

  // up
  int yy = y;
  while (yy >= 0) {
    if (getChip(x, yy - 1) == userChip)
      break;
    --yy;
  }
  if (yy >= 0) {
    // res.insert(Direction::Up);
    for (int j = yy; j < y; ++j)
      putChip(x, j, userChip);
  }

  // down
  yy = y;
  while (yy < 8) {
    if (getChip(x, yy + 1) == userChip)
      break;
    ++yy;
  }
  if (yy < 8) {
    // res.insert(Direction::Down);
    for (int j = y + 1; j <= yy; ++j)
      putChip(x, j, userChip);
  }

  // left up
  xx = x;
  yy = y;
  while ((xx >= 0) && (yy >= 0)) {
    if (getChip(xx - 1, yy - 1) == userChip)
      break;
    --xx;
    --yy;
  }
  if ((xx >= 0) && (yy >= 0)) {
    // res.insert(Direction::LeftUp);
    while ((xx != x) && (yy != y)) {
      putChip(xx, yy, userChip);
      ++xx;
      ++yy;
    }
  }

  // left down
  xx = x;
  yy = y;
  while ((xx >= 0) && (yy < 8)) {
    if (getChip(xx - 1, yy + 1) == userChip)
      break;
    --xx;
    ++yy;
  }
  if ((xx >= 0) && (yy < 8)) {
    // res.insert(Direction::LeftDown);
    while ((xx != x) && (yy != y)) {
      putChip(xx, yy, userChip);
      ++xx;
      --yy;
    }
  }

  // right up
  xx = x;
  yy = y;
  while ((xx < 8) && (yy >= 0)) {
    if (getChip(xx + 1, yy - 1) == userChip)
      break;
    ++xx;
    --yy;
  }
  if ((xx < 8) && (yy >= 0)) {
    // res.insert(Direction::RightUp);
    while ((xx != x) && (yy != y)) {
      putChip(xx, yy, userChip);
      --xx;
      ++yy;
    }
  }

  // right down
  xx = x;
  yy = y;
  while ((xx < 8) && (yy < 8)) {
    if (getChip(xx + 1, yy + 1) == userChip)
      break;
    ++xx;
    ++yy;
  }
  if ((xx < 8) && (yy < 8)) {
    // res.insert(Direction::RightDown);
    while ((xx != x) && (yy != y)) {
      putChip(xx, yy, userChip);
      --xx;
      --yy;
    }
  }

  // return res;
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

bool Board::areThereChips(int ind, Direction dir, Chip user, Chip comp) {
  int x = ind % 8;
  int y = ind / 8;
  bool changed = true;

  switch (static_cast<int>(dir)) {
  case 0: // left
    --x;
    while (x >= 0) {
      if (getChip(x, y) == comp)
        changed = !changed;
      else if (getChip(x, y) == user)
        return !changed;
      else if (getChip(x, y) == Chip::Empty)
        break;
      --x;
    }
    break;
  case 1: // right
    ++x;
    while (x < 8) {
      if (getChip(x, y) == comp)
        changed = !changed;
      else if (getChip(x, y) == user)
        return !changed;
      else if (getChip(x, y) == Chip::Empty)
        break;
      ++x;
    }
    break;
  case 2: // up
    --y;
    while (y >= 0) {
      if (getChip(x, y) == comp)
        changed = !changed;
      else if (getChip(x, y) == user)
        return !changed;
      else if (getChip(x, y) == Chip::Empty)
        break;
      --y;
    }
    break;
  case 3: // down
    ++y;
    while (y < 8) {
      if (getChip(x, y) == comp)
        changed = !changed;
      else if (getChip(x, y) == user)
        return !changed;
      else if (getChip(x, y) == Chip::Empty)
        break;
      ++y;
    }
    break;

  case 4: // left up
    --x;
    --y;
    while ((y >= 0) && (x >= 0)) {
      if (getChip(x, y) == comp)
        changed = !changed;
      else if (getChip(x, y) == user)
        return !changed;
      else if (getChip(x, y) == Chip::Empty)
        break;
      --x;
      --y;
    }
    break;

  case 5: // left down
    --x;
    ++y;
    while ((y < 8) && (x >= 0)) {
      if (getChip(x, y) == comp)
        changed = !changed;
      else if (getChip(x, y) == user)
        return !changed;
      else if (getChip(x, y) == Chip::Empty)
        break;
      --x;
      ++y;
    }
    break;

  case 6: // right up
    ++x;
    --y;
    while ((y >= 0) && (x < 8)) {
      if (getChip(x, y) == comp)
        changed = !changed;
      else if (getChip(x, y) == user)
        return !changed;
      else if (getChip(x, y) == Chip::Empty)
        break;
      ++x;
      --y;
    }
    break;

  case 7: // right down
    ++x;
    ++y;
    while ((y < 8) && (x < 8)) {
      if (getChip(x, y) == comp)
        changed = !changed;
      else if (getChip(x, y) == user)
        return !changed;
      else if (getChip(x, y) == Chip::Empty)
        break;
      ++x;
      ++y;
    }
    break;
  }

  return false;
}

Board::Board() {
  layout = new Chip[64]();
  putChip(3, 4, Chip::Black);
  putChip(4, 3, Chip::Black);
  putChip(3, 3, Chip::White);
  putChip(4, 4, Chip::White);
  generateHash();
  initializeAvailableMoves(userChip, availableMoves);
}
