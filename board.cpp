#include "board.h"

Chip Board::getChip(int ind) const {
  if (playerLayout.contains(ind))
    return playerChip;
  else if (computerLayout.contains(ind))
    return computerChip;
  else
    return Chip::Empty;
}

Chip Board::getChip(int i, int j) const { return getChip(j * 8 + i); }

AvailableMove Board::putChip(int ind, Current cur) {
  AvailableMove madeMove;
  switch (cur) {
  case Current::User: {
    int pos = -1;
    for (int i = 0; i < playerAvailableMoves.size(); ++i)
      if (playerAvailableMoves[i].getInd() == ind) {
        pos = i;
        break;
      }
    if (pos != -1) {
      madeMove = playerAvailableMoves[pos];
      playerLayout.insert(ind);
    } else
      // qDebug() << "putChip() error: such move can't be made";
      break;
  }
  case Current::Computer: {
    int pos = -1;
    for (int i = 0; i < computerAvailableMoves.size(); ++i)
      if (computerAvailableMoves[i].getInd() == ind) {
        pos = i;
        break;
      }
    if (pos != -1) {
      madeMove = computerAvailableMoves[pos];
      computerLayout.insert(ind);
    } else
      // qDebug() << "putChip() error: such move can't be made";
      break;
  }
  }
  return madeMove;
}

AvailableMove Board::putChip(int i, int j, Current cur) {
  return putChip(j * 8 + i, cur);
}

void Board::initializeLayouts() {
  playerAvailableMoves.clear();
  computerAvailableMoves.clear();
  playerLayout.clear();
  computerLayout.clear();
  QList<AvailableMove> blacks{
      AvailableMove(19, Direction::Down), AvailableMove(26, Direction::Right),
      AvailableMove(37, Direction::Left), AvailableMove(44, Direction::Up)};
  QList<AvailableMove> whites{
      AvailableMove(20, Direction::Down), AvailableMove(34, Direction::Right),
      AvailableMove(29, Direction::Left), AvailableMove(43, Direction::Up)};
  if (playerChip == Chip::Black) {
    playerAvailableMoves = blacks;
    computerAvailableMoves = whites;
    playerLayout.insert(28);
    playerLayout.insert(35);
    computerLayout.insert(27);
    computerLayout.insert(36);
  } else {
    playerAvailableMoves = whites;
    computerAvailableMoves = blacks;
    computerLayout.insert(28);
    computerLayout.insert(35);
    playerLayout.insert(27);
    playerLayout.insert(36);
  }
}

void Board::changeChips() {
  std::swap(playerChip, computerChip);
  std::swap(playerLayout, computerLayout);
  std::swap(playerAvailableMoves, computerAvailableMoves);
}

bool Board::canPutChip(int ind, Current cur) {
  switch (cur) {
  case Current::User:
    for (auto move : playerAvailableMoves)
      if (move.getInd() == ind)
        return true;
    return false;
  case Current::Computer:
    for (auto move : computerAvailableMoves)
      if (move.getInd() == ind)
        return true;
    return false;
  }
}

bool Board::canPutChip(int x, int y, Current cur) {
  return canPutChip(y * 8 + x, cur);
}

void Board::moveMade(AvailableMove &madeMove, Current cur) {
  updateLayouts(madeMove, cur);
  updateAvailableMoves(cur);
  updateAvailableMoves(enemy(cur));
}

int Board::playerScore() { return playerLayout.size(); }

int Board::computerScore() { return computerLayout.size(); }

QList<AvailableMove> Board::getComputerAvailableMoves() const {
  return computerAvailableMoves;
}

QList<AvailableMove> Board::getPlayerAvailableMoves() const {
  return playerAvailableMoves;
}

void Board::makeMove(int x, int y) {
  AvailableMove move = putChip(x, y, Current::User);
  moveMade(move, Current::User);
}

void Board::updateAvailableMoves(Current cur) {
  int ind = 0;
  int dirInd = 0;
  Chip enemyChip, chip;
  QList<AvailableMove> *avm;
  QSet<int> *layout;
  switch (static_cast<int>(cur)) {
  case 0:
    chip = playerChip;
    enemyChip = computerChip;
    avm = &playerAvailableMoves;
    layout = &playerLayout;
    break;
  case 1:
    chip = playerChip;
    enemyChip = playerChip;
    avm = &computerAvailableMoves;
    layout = &computerLayout;
    break;
  }
  while ((!avm->empty()) && (ind != avm->size())) {
    while ((!(*avm)[ind].isEmpty()) && (dirInd != (*avm)[ind].size())) {
      if (!areThereChips((*avm)[ind].getInd(), (*avm)[ind].getDirs()[dirInd],
                         chip, enemyChip)) {
        (*avm)[ind].eraseDirection(dirInd);
      } else
        ++dirInd;
    }
    if (!avm->isEmpty())
      avm->erase(avm->begin() + ind);
    else {
      ++ind;
      dirInd = 0;
    }
  }

  for (int pos : (*layout)) {
    addPossibleMovesForPosition(pos, enemyChip, cur, (*avm));
  }
}

Chip Board::getUserChip() { return playerChip; }

void Board::addPossibleMovesForPosition(int pos, Chip enemyChip, Current cur,
                                        QList<AvailableMove> &avm) {
  int pp = availablePos(pos, Direction::Down, enemyChip);
  int ind;
  if (pp != -1) {
    ind = alreadyAvailable(pp, cur);
    if (ind != -1)
      avm[ind].addDir(Direction::Up);
    else
      avm.push_back(AvailableMove(pp, Direction::Up));
  }
  pp = availablePos(pos, Direction::Left, enemyChip);
  if (pp != -1) {
    ind = alreadyAvailable(pp, cur);
    if (ind != -1)
      avm[ind].addDir(Direction::Right);
    else
      avm.push_back(AvailableMove(pp, Direction::Right));
  }
  pp = availablePos(pos, Direction::Right, enemyChip);
  ind = alreadyAvailable(pp, cur);
  if (pp != -1) {
    if (ind != -1)
      avm[ind].addDir(Direction::Left);
    else
      avm.push_back(AvailableMove(pp, Direction::Left));
  }
  pp = availablePos(pos, Direction::Up, enemyChip);
  if (pp != -1) {
    ind = alreadyAvailable(pp, cur);
    if (ind != -1)
      avm[ind].addDir(Direction::Down);
    else
      avm.push_back(AvailableMove(pp, Direction::Down));
  }
  pp = availablePos(pos, Direction::LeftDown, enemyChip);
  if (pp != -1) {
    ind = alreadyAvailable(pp, cur);
    if (ind != -1)
      avm[ind].addDir(Direction::RightUp);
    else
      avm.push_back(AvailableMove(pp, Direction::RightUp));
  }
  pp = availablePos(pos, Direction::LeftUp, enemyChip);
  if (pp != -1) {
    ind = alreadyAvailable(pp, cur);
    if (ind != -1)
      avm[ind].addDir(Direction::RightDown);
    else
      avm.push_back(AvailableMove(pp, Direction::RightDown));
  }
  pp = availablePos(pos, Direction::RightDown, enemyChip);
  if (pp != -1) {
    ind = alreadyAvailable(pp, cur);
    if (ind != -1)
      avm[ind].addDir(Direction::LeftUp);
    else
      avm.push_back(AvailableMove(pp, Direction::LeftUp));
  }
  pp = availablePos(pos, Direction::RightUp, enemyChip);
  if (pp != -1) {
    ind = alreadyAvailable(pp, cur);
    if (ind != -1)
      avm[ind].addDir(Direction::LeftDown);
    else
      avm.push_back(AvailableMove(pp, Direction::LeftDown));
  }
}

int Board::alreadyAvailable(int pos, Current cur) {
  switch (cur) {
  case Current::User:
    for (int i = 0; i < playerAvailableMoves.size(); ++i)
      if (playerAvailableMoves[i].getInd() == pos)
        return i;
    return -1;
  case Current::Computer:
    for (int i = 0; i < computerAvailableMoves.size(); ++i)
      if (computerAvailableMoves[i].getInd() == pos)
        return i;
    return -1;
  }
}

int Board::availablePos(int ind, Direction dir, Chip enemyChip) {
  int x = ind % 8;
  int y = ind / 8;
  bool changed = true;

  switch (static_cast<int>(dir)) {
  case 0: // left
    --x;
    while (x >= 0) {
      if (getChip(x, y) == enemyChip)
        changed = false;
      else {
        if (getChip(x, y) == Chip::Empty) {
          if (!changed) {
            // qDebug() << "left square for " << ind << ": " << (y * 8 + x);
            return y * 8 + x;
          }
        }
        return -1;
      }
      --x;
    }
    break;
  case 1: // right
    ++x;
    while (x < 8) {
      if (getChip(x, y) == enemyChip)
        changed = false;
      else {
        if (getChip(x, y) == Chip::Empty) {
          if (!changed) {
            // qDebug() << "right square for " << ind << ": " << (y * 8 + x);
            return y * 8 + x;
          }
        }
        return -1;
      }
      ++x;
    }
    break;
  case 2: // up
    --y;
    while (y >= 0) {
      if (getChip(x, y) == enemyChip)
        changed = false;
      else {
        if (getChip(x, y) == Chip::Empty) {
          if (!changed) {
            // qDebug() << "up square for " << ind << ": " << (y * 8 + x);
            return y * 8 + x;
          }
        }
        return -1;
      }
      --y;
    }
    break;
  case 3: // down
    ++y;
    while (y < 8) {
      if (getChip(x, y) == enemyChip)
        changed = false;
      else {
        if (getChip(x, y) == Chip::Empty) {
          if (!changed) {
            // qDebug() << "down square for " << ind << ": " << (y * 8 + x);
            return y * 8 + x;
          }
        }
        return -1;
      }
      ++y;
    }
    break;
  case 4: // left up
    --x;
    --y;
    while ((y >= 0) && (x >= 0)) {
      if (getChip(x, y) == enemyChip)
        changed = false;
      else {
        if (getChip(x, y) == Chip::Empty) {
          if (!changed) {
            // qDebug() << "left up square for " << ind << ": " << (y * 8 + x);
            return y * 8 + x;
          }
        }
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
      if (getChip(x, y) == enemyChip)
        changed = false;
      else {
        if (getChip(x, y) == Chip::Empty) {
          if (!changed) {
            // qDebug() << "left down square for " << ind << ": " << (y * 8 +
            // x);
            return y * 8 + x;
          }
        }
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
      if (getChip(x, y) == enemyChip)
        changed = false;
      else {
        if (getChip(x, y) == Chip::Empty) {
          if (!changed) {
            // qDebug() << "right up square for " << ind << ": " << (y * 8 + x);
            return y * 8 + x;
          }
        }
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
      if (getChip(x, y) == enemyChip)
        changed = false;
      else {
        if (getChip(x, y) == Chip::Empty) {
          if (!changed) {
            // qDebug() << "right down square for " << ind << ": " << (y * 8 +
            // x);
            return y * 8 + x;
          }
        }
        return -1;
      }
      ++x;
      ++y;
    }
    break;
  }

  return -1;
}

AvailableMove Board::computerMakeMove() {
  int rnd = rand() % computerAvailableMoves.size();
  return putChip(computerAvailableMoves[rnd].getInd(), Current::Computer);
}

bool Board::final(Current cur) const {
  switch (cur) {
  case Current::User:
    return playerAvailableMoves.size() == 0;
  case Current::Computer:
    return computerAvailableMoves.size() == 0;
  }
}

double Board::heuristic() const {
  int my_tiles = 0;
  int opp_tiles = 0;
  int X1[] = {-1, -1, 0, 1, 1, 1, 0, -1};
  int Y1[] = {0, 1, 1, 1, 0, -1, -1, -1};
  int x = 0;
  int y = 0;
  int my_front_tiles = 0;
  int opp_front_tiles = 0;
  double p = 0;
  double f = 0;
  double c = 0;
  double m = 0;
  double l = 0;
  double d = 0;

  // Piece difference, frontier disks and disk squares
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++) {
      if (getChip(i, j) == playerChip) {
        d += sw.getWeight(i, j);
        my_tiles++;
      } else if (getChip(i, j) == computerChip) {
        d -= sw.getWeight(i, j);
        opp_tiles++;
      }
      if (getChip(i, j) != Chip::Empty) {
        for (int k = 0; k < 8; k++) {
          x = i + X1[k];
          y = j + Y1[k];
          if (x >= 0 && x < 8 && y >= 0 && y < 8 &&
              getChip(x, y) == Chip::Empty) {
            if (playerLayout.contains(j * 8 + i))
              my_front_tiles++;
            else
              opp_front_tiles++;
            break;
          }
        }
      }
    }
  if (my_tiles > opp_tiles)
    p = (100.0 * my_tiles) / (my_tiles + opp_tiles);
  else if (my_tiles < opp_tiles)
    p = -(100.0 * opp_tiles) / (my_tiles + opp_tiles);
  else
    p = 0;

  if (my_front_tiles > opp_front_tiles)
    f = -(100.0 * my_front_tiles) / (my_front_tiles + opp_front_tiles);
  else if (my_front_tiles < opp_front_tiles)
    f = (100.0 * opp_front_tiles) / (my_front_tiles + opp_front_tiles);
  else
    f = 0;

  // Corner occupancy
  my_tiles = opp_tiles = 0;
  if (getChip(0, 0) == playerChip)
    my_tiles++;
  else if (getChip(0, 0) == computerChip)
    opp_tiles++;
  if (getChip(0, 7) == playerChip)
    my_tiles++;
  else if (getChip(0, 7) == computerChip)
    opp_tiles++;
  if (getChip(7, 0) == playerChip)
    my_tiles++;
  else if (getChip(7, 0) == computerChip)
    opp_tiles++;
  if (getChip(7, 7) == playerChip)
    my_tiles++;
  else if (getChip(7, 7) == computerChip)
    opp_tiles++;
  c = 25 * (my_tiles - opp_tiles);

  // Corner closeness
  my_tiles = opp_tiles = 0;
  if (getChip(0, 0) == Chip::Empty) {
    if (getChip(0, 1) == playerChip)
      my_tiles++;
    else if (getChip(0, 1) == computerChip)
      opp_tiles++;
    if (getChip(1, 1) == playerChip)
      my_tiles++;
    else if (getChip(1, 1) == computerChip)
      opp_tiles++;
    if (getChip(1, 0) == playerChip)
      my_tiles++;
    else if (getChip(1, 0) == computerChip)
      opp_tiles++;
  }
  if (getChip(0, 7) == Chip::Empty) {
    if (getChip(0, 6) == playerChip)
      my_tiles++;
    else if (getChip(0, 6) == computerChip)
      opp_tiles++;
    if (getChip(1, 6) == playerChip)
      my_tiles++;
    else if (getChip(1, 6) == computerChip)
      opp_tiles++;
    if (getChip(1, 7) == playerChip)
      my_tiles++;
    else if (getChip(1, 7) == computerChip)
      opp_tiles++;
  }
  if (getChip(7, 0) == Chip::Empty) {
    if (getChip(7, 1) == playerChip)
      my_tiles++;
    else if (getChip(7, 1) == computerChip)
      opp_tiles++;
    if (getChip(6, 1) == playerChip)
      my_tiles++;
    else if (getChip(6, 1) == computerChip)
      opp_tiles++;
    if (getChip(6, 0) == playerChip)
      my_tiles++;
    else if (getChip(6, 0) == computerChip)
      opp_tiles++;
  }
  if (getChip(7, 7) == Chip::Empty) {
    if (getChip(6, 7) == playerChip)
      my_tiles++;
    else if (getChip(6, 7) == computerChip)
      opp_tiles++;
    if (getChip(6, 6) == playerChip)
      my_tiles++;
    else if (getChip(6, 6) == computerChip)
      opp_tiles++;
    if (getChip(7, 6) == playerChip)
      my_tiles++;
    else if (getChip(7, 6) == computerChip)
      opp_tiles++;
  }
  l = -12.5 * (my_tiles - opp_tiles);

  // Mobility
  my_tiles = playerAvailableMoves.size();
  opp_tiles = computerAvailableMoves.size();
  if (my_tiles > opp_tiles)
    m = (100.0 * my_tiles) / (my_tiles + opp_tiles);
  else if (my_tiles < opp_tiles)
    m = -(100.0 * opp_tiles) / (my_tiles + opp_tiles);
  else
    m = 0;

  // final weighted score
  double score = (10 * p) + (801.724 * c) + (382.026 * l) + (78.922 * m) +
                 (74.396 * f) + (10 * d);
  return score;
}

Board::~Board() {}

Board::Board(const Board &other)
    : playerChip(other.playerChip), computerChip(other.computerChip) {
  playerLayout = other.playerLayout;
  computerLayout = other.computerLayout;
  playerAvailableMoves = other.playerAvailableMoves;
  computerAvailableMoves = other.computerAvailableMoves;
}

Board::Board(Board &&other)
    : playerChip(other.playerChip), computerChip(other.computerChip) {
  playerLayout = std::move(other.playerLayout);
  computerLayout = std::move(other.computerLayout);
  playerAvailableMoves = std::move(other.playerAvailableMoves);
  computerAvailableMoves = std::move(other.computerAvailableMoves);
}

Board &Board::operator=(const Board &other) {
  playerChip = other.playerChip;
  computerChip = other.computerChip;
  playerLayout = other.playerLayout;
  computerLayout = other.computerLayout;
  playerAvailableMoves = other.playerAvailableMoves;
  computerAvailableMoves = other.computerAvailableMoves;
  return *this;
}

Board &Board::operator=(Board &&other) {
  playerChip = other.playerChip;
  computerChip = other.computerChip;
  playerLayout = std::move(other.playerLayout);
  computerLayout = std::move(other.computerLayout);
  playerAvailableMoves = std::move(other.playerAvailableMoves);
  computerAvailableMoves = std::move(other.computerAvailableMoves);
  return *this;
}

void Board::updateLayouts(AvailableMove &move, Current cur) {
  int x = move.getInd() % 8;
  int y = move.getInd() / 8;
  QSet<int> *enemyL;
  Chip chip;
  switch (static_cast<int>(cur)) {
  case 0:
    chip = playerChip;
    enemyL = &computerLayout;
    break;
  case 1:
    chip = computerChip;
    enemyL = &playerLayout;
    break;
  }

  for (auto dir : move.getDirs()) {
    int xx = x;
    int yy = y;
    switch (static_cast<int>(dir)) {
    case 0: // left
      while (xx >= 0) {
        if (getChip(xx - 1, y) == chip)
          break;
        --xx;
      }
      if (xx >= 0) {
        for (int i = xx; i < x; ++i) {
          addToLayout(i, y, cur);
          eraseFromLayout(i, y, enemy(cur));
        }
      }
      break;

    case 1: // right
      while (xx < 8) {
        if (getChip(xx + 1, y) == chip)
          break;
        ++xx;
      }
      if (xx < 8) {
        for (int i = x + 1; i <= xx; ++i) {
          addToLayout(i, y, cur);
          eraseFromLayout(i, y, enemy(cur));
        }
      }
      break;
    case 2: // up
      while (yy >= 0) {
        if (getChip(x, yy - 1) == chip)
          break;
        --yy;
      }
      if (yy >= 0) {
        for (int j = yy; j < y; ++j) {
          addToLayout(x, j, cur);
          eraseFromLayout(x, j, enemy(cur));
        }
      }
      break;
    case 3: // down
      while (yy < 8) {
        if (getChip(x, yy + 1) == chip)
          break;
        ++yy;
      }
      if (yy < 8) {
        for (int j = y + 1; j <= yy; ++j) {
          addToLayout(x, j, cur);
          eraseFromLayout(x, j, enemy(cur));
        }
      }
      break;
    case 4: // left up
      while ((xx >= 0) && (yy >= 0)) {
        if (getChip(xx - 1, yy - 1) == chip)
          break;
        --xx;
        --yy;
      }
      if ((xx >= 0) && (yy >= 0)) {
        while ((xx != x) && (yy != y)) {
          addToLayout(xx, yy, cur);
          eraseFromLayout(xx, yy, enemy(cur));
          ++xx;
          ++yy;
        }
      }
      break;
    case 5: // left down
      while ((xx >= 0) && (yy < 8)) {
        if (getChip(xx - 1, yy + 1) == chip)
          break;
        --xx;
        ++yy;
      }
      if ((xx >= 0) && (yy < 8)) {
        while ((xx != x) && (yy != y)) {
          addToLayout(xx, yy, cur);
          eraseFromLayout(xx, yy, enemy(cur));
          ++xx;
          --yy;
        }
      }
      break;
    case 6: // right up
      while ((xx < 8) && (yy >= 0)) {
        if (getChip(xx + 1, yy - 1) == chip)
          break;
        ++xx;
        --yy;
      }
      if ((xx < 8) && (yy >= 0)) {
        while ((xx != x) && (yy != y)) {
          addToLayout(xx, yy, cur);
          eraseFromLayout(xx, yy, enemy(cur));
          --xx;
          ++yy;
        }
      }
      break;
    case 7: // right down
      while ((xx < 8) && (yy < 8)) {
        if (getChip(xx + 1, yy + 1) == chip)
          break;
        ++xx;
        ++yy;
      }
      if ((xx < 8) && (yy < 8)) {
        while ((xx != x) && (yy != y)) {
          addToLayout(xx, yy, cur);
          eraseFromLayout(xx, yy, enemy(cur));
          --xx;
          --yy;
        }
      }
    }
  }
}

void Board::eraseFromLayout(int x, int y, Current cur) {
  switch (cur) {
  case Current::User: {
    auto it = std::find(playerLayout.begin(), playerLayout.end(), y * 8 + x);
    playerLayout.erase(it);
    break;
  }
  case Current::Computer: {
    auto it =
        std::find(computerLayout.begin(), computerLayout.end(), y * 8 + x);
    computerLayout.erase(it);
    break;
  }
  }
}

void Board::addToLayout(int x, int y, Current cur) {
  switch (cur) {
  case Current::User: {
    playerLayout.insert(y * 8 + x);
    break;
  }
  case Current::Computer: {
    computerLayout.insert(y * 8 + x);
    break;
  }
  }
}

bool Board::areThereChips(int ind, Direction dir, Chip chip, Chip enemyChip) {
  int x = ind % 8;
  int y = ind / 8;
  bool changed = true;

  switch (static_cast<int>(dir)) {
  case 0: // left
    --x;
    while (x >= 0) {
      if (getChip(x, y) == enemyChip)
        changed = false;
      else if (getChip(x, y) == chip)
        return !changed;
      else if (getChip(x, y) == Chip::Empty)
        break;
      --x;
    }
    break;
  case 1: // right
    ++x;
    while (x < 8) {
      if (getChip(x, y) == enemyChip)
        changed = false;
      else if (getChip(x, y) == chip)
        return !changed;
      else if (getChip(x, y) == Chip::Empty)
        break;
      ++x;
    }
    break;
  case 2: // up
    --y;
    while (y >= 0) {
      if (getChip(x, y) == enemyChip)
        changed = false;
      else if (getChip(x, y) == chip)
        return !changed;
      else if (getChip(x, y) == Chip::Empty)
        break;
      --y;
    }
    break;
  case 3: // down
    ++y;
    while (y < 8) {
      if (getChip(x, y) == enemyChip)
        changed = false;
      else if (getChip(x, y) == chip)
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
      if (getChip(x, y) == enemyChip)
        changed = false;
      else if (getChip(x, y) == chip)
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
      if (getChip(x, y) == enemyChip)
        changed = false;
      else if (getChip(x, y) == chip)
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
      if (getChip(x, y) == enemyChip)
        changed = false;
      else if (getChip(x, y) == chip)
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
      if (getChip(x, y) == enemyChip)
        changed = false;
      else if (getChip(x, y) == chip)
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

Board::Board() {}
