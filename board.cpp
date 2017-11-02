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

QPair<int, QList<Direction>> Board::putChip(int i, int j, Chip ch, bool push) {
  layout[j * 8 + i] = ch;
  QPair<int, QList<Direction>> res;
  res.first = j * 8 + i;
  res.second = availableMoves[j * 8 + i];
  moveMade(j * 8 + i, false);

  if (push)
    playerLayout.insert(j * 8 + i);

  return res;
}

void Board::putChip(int ind, Chip ch, bool push) {
  putChip(ind % 8, ind / 8, ch, push);
}

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
  initAvM();
}

void Board::initializeAvailableMoves(Chip user,
                                     QMap<int, QList<Direction>> &avm,
                                     QSet<int> &pl) {
  avm.clear();
  pl.clear();
  if (user == Chip::Black) {
    avm[19].push_back(Direction::Down);
    avm[26].push_back(Direction::Right);
    avm[37].push_back(Direction::Left);
    avm[44].push_back(Direction::Up);
    pl.insert(28);
    pl.insert(35);
  } else {
    avm[20].push_back(Direction::Down);
    avm[29].push_back(Direction::Left);
    avm[34].push_back(Direction::Right);
    avm[43].push_back(Direction::Up);
    pl.insert(27);
    pl.insert(36);
  }
}

void Board::initAvM() {
  initializeAvailableMoves(userChip, availableMoves, playerLayout);
}

void Board::updAvM() {
  updateAvailableMoves(userChip, computerChip, availableMoves, playerLayout);
}

void Board::updatePlayerLayout(const QList<int> &takenChips, bool add) {
  for (int pos : takenChips) {
    auto it = std::find(playerLayout.begin(), playerLayout.end(), pos);
    if (!add) {
      if (it != playerLayout.end())
        playerLayout.erase(it);
    } else {
      if (it == playerLayout.end())
        playerLayout.insert(pos);
    }
  }
}

bool Board::canPutChip(int ind) { return availableMoves.contains(ind); }

void Board::moveMade(int ind, bool push) {
  availableMoves.erase(availableMoves.find(ind));
  if (push)
    playerLayout.insert(ind);
}

void Board::updateAvailableMoves(Chip user, Chip comp,
                                 QMap<int, QList<Direction>> &avm,
                                 QSet<int> pl) {
  int ind = 0;
  int indDir = 0;
  while ((!avm.empty()) && (ind != avm.size())) {
    while ((!((avm.begin() + ind)->empty())) &&
           (indDir != (avm.begin() + ind)->size())) {
      if (!areThereChips((avm.begin() + ind).key(),
                         (avm.begin() + ind).value()[indDir], user, comp)) {
        (avm.begin() + ind)
            .value()
            .erase((avm.begin() + ind).value().begin() + indDir);
      } else
        ++indDir;
    }
    if (!(avm.begin() + ind)->size())
      avm.erase(avm.begin() + ind);
    else {
      ++ind;
      indDir = 0;
    }
  }

  for (auto pos : pl) {
    addPossibleMoves(pos, user, comp, avm);
  }
}

Chip Board::getUserChip() { return userChip; }

void Board::addPossibleMoves(int pos, Chip user, Chip comp,
                             QMap<int, QList<Direction>> &avm) {
  int pp = availablePos(pos, Direction::Down, user, comp);
  if (pp != -1)
    avm[pp].push_back(Direction::Up);
  pp = availablePos(pos, Direction::Left, user, comp);
  if (pp != -1)
    avm[pp].push_back(Direction::Right);
  pp = availablePos(pos, Direction::Right, user, comp);
  if (pp != -1)
    avm[pp].push_back(Direction::Left);
  pp = availablePos(pos, Direction::Up, user, comp);
  if (pp != -1)
    avm[pp].push_back(Direction::Down);
  pp = availablePos(pos, Direction::LeftDown, user, comp);
  if (pp != -1)
    avm[pp].push_back(Direction::RightUp);
  pp = availablePos(pos, Direction::LeftUp, user, comp);
  if (pp != -1)
    avm[pp].push_back(Direction::RightDown);
  pp = availablePos(pos, Direction::RightDown, user, comp);
  if (pp != -1)
    avm[pp].push_back(Direction::LeftUp);
  pp = availablePos(pos, Direction::RightUp, user, comp);
  if (pp != -1)
    avm[pp].push_back(Direction::LeftDown);
}

int Board::availablePos(int ind, Direction dir, Chip user, Chip comp) {
  int x = ind % 8;
  int y = ind / 8;
  bool changed = true;

  switch (static_cast<int>(dir)) {
  case 0: // left
    --x;
    while (x >= 0) {
      if (getChip(x, y) == comp)
        changed = false;
      else {
        if (getChip(x, y) == Chip::Empty) {
          if (!changed)
            return y * 8 + x;
        } else
          return -1;
      }
      --x;
    }
    break;
  case 1: // right
    ++x;
    while (x < 8) {
      if (getChip(x, y) == comp)
        changed = false;
      else {
        if (getChip(x, y) == Chip::Empty) {
          if (!changed)
            return y * 8 + x;
        } else
          return -1;
      }
      ++x;
    }
    break;
  case 2: // up
    --y;
    while (y >= 0) {
      if (getChip(x, y) == comp)
        changed = false;
      else {
        if (getChip(x, y) == Chip::Empty) {
          if (!changed)
            return y * 8 + x;
        } else
          return -1;
      }
      --y;
    }
    break;
  case 3: // down
    ++y;
    while (y < 8) {
      if (getChip(x, y) == comp)
        changed = false;
      else {
        if (getChip(x, y) == Chip::Empty) {
          if (!changed)
            return y * 8 + x;
        } else
          return -1;
      }
      ++y;
    }
    break;

  case 4: // left up
    --x;
    --y;
    while ((y >= 0) && (x >= 0)) {
      if (getChip(x, y) == comp)
        changed = false;
      else {
        if (getChip(x, y) == Chip::Empty) {
          if (!changed)
            return y * 8 + x;
        } else
          return -1;
      }
      --x;
      --y;
    }
    break;

  case 5: // left down
    --x;
    ++y;
    while ((y < 8) && (x >= 0)) {
      if (getChip(x, y) == comp)
        changed = false;
      else {
        if (getChip(x, y) == Chip::Empty) {
          if (!changed)
            return y * 8 + x;
        } else
          return -1;
      }
      --x;
      ++y;
    }
    break;

  case 6: // right up
    ++x;
    --y;
    while ((y >= 0) && (x < 8)) {
      if (getChip(x, y) == comp)
        changed = false;
      else {
        if (getChip(x, y) == Chip::Empty) {
          if (!changed)
            return y * 8 + x;
        } else
          return -1;
      }
      ++x;
      --y;
    }
    break;

  case 7: // right down
    ++x;
    ++y;
    while ((y < 8) && (x < 8)) {
      if (getChip(x, y) == comp)
        changed = false;
      else {
        if (getChip(x, y) == Chip::Empty) {
          if (!changed)
            return y * 8 + x;
        } else
          return -1;
      }
      ++x;
      ++y;
    }
    break;
  }

  return -1;
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

QList<int> Board::updateLayout(QPair<int, QList<Direction>> move, Chip user) {
  int x = move.first % 8;
  int y = move.first / 8;
  QList<int> res;

  for (auto dir : move.second) {
    int xx = x;
    int yy = y;
    switch (static_cast<int>(dir)) {
    case 0: // left
      while (xx >= 0) {
        if (getChip(xx - 1, y) == user)
          break;
        --xx;
      }
      if (xx >= 0) {
        for (int i = xx; i < x; ++i) {
          putChip(i, y, user, true);
          res.push_back(y * 8 + i);
        }
      }
      break;

    case 1: // right
      while (xx < 8) {
        if (getChip(xx + 1, y) == user)
          break;
        ++xx;
      }
      if (xx < 8) {
        for (int i = x + 1; i <= xx; ++i) {
          putChip(i, y, user, true);
          res.push_back(y * 8 + i);
        }
      }
      break;
    case 2: // up
      while (yy >= 0) {
        if (getChip(x, yy - 1) == user)
          break;
        --yy;
      }
      if (yy >= 0) {
        for (int j = yy; j < y; ++j) {
          putChip(x, j, user, true);
          res.push_back(j * 8 + x);
        }
      }
      break;
    case 3: // down
      while (yy < 8) {
        if (getChip(x, yy + 1) == user)
          break;
        ++yy;
      }
      if (yy < 8) {
        for (int j = y + 1; j <= yy; ++j) {
          putChip(x, j, user, true);
          res.push_back(j * 8 + x);
        }
      }
      break;
    case 4: // left up
      while ((xx >= 0) && (yy >= 0)) {
        if (getChip(xx - 1, yy - 1) == user)
          break;
        --xx;
        --yy;
      }
      if ((xx >= 0) && (yy >= 0)) {
        while ((xx != x) && (yy != y)) {
          putChip(xx, yy, user, true);
          res.push_back(yy * 8 + xx);
          ++xx;
          ++yy;
        }
      }
      break;
    case 5: // left down
      while ((xx >= 0) && (yy < 8)) {
        if (getChip(xx - 1, yy + 1) == user)
          break;
        --xx;
        ++yy;
      }
      if ((xx >= 0) && (yy < 8)) {
        while ((xx != x) && (yy != y)) {
          putChip(xx, yy, user, true);
          res.push_back(yy * 8 + xx);
          ++xx;
          --yy;
        }
      }
      break;
    case 6: // right up
      while ((xx < 8) && (yy >= 0)) {
        if (getChip(xx + 1, yy - 1) == user)
          break;
        ++xx;
        --yy;
      }
      if ((xx < 8) && (yy >= 0)) {
        while ((xx != x) && (yy != y)) {
          putChip(xx, yy, user, true);
          res.push_back(yy * 8 + xx);
          --xx;
          ++yy;
        }
      }
      break;
    case 7: // right down
      while ((xx < 8) && (yy < 8)) {
        if (getChip(xx + 1, yy + 1) == user)
          break;
        ++xx;
        ++yy;
      }
      if ((xx < 8) && (yy < 8)) {
        while ((xx != x) && (yy != y)) {
          putChip(xx, yy, user, true);
          res.push_back(yy * 8 + xx);
          --xx;
          --yy;
        }
      }
    }
  }

  return res;
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
  initializeAvailableMoves(getUserChip(), availableMoves, playerLayout);
}
