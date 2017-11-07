#include "availablemove.h"

AvailableMove::AvailableMove(int ind, QList<Direction> dirs)
    : moveIndex(ind), directions(dirs) {}

AvailableMove::AvailableMove() {}

AvailableMove::AvailableMove(int ind, Direction dir) : moveIndex(ind) {
  directions.push_back(dir);
}

AvailableMove::AvailableMove(const AvailableMove &other)
    : moveIndex(other.moveIndex), directions(other.directions) {}

AvailableMove::AvailableMove(AvailableMove &&other)
    : moveIndex(other.moveIndex) {
  directions = std::move(other.directions);
}

AvailableMove &AvailableMove::operator=(const AvailableMove &other) {
  moveIndex = other.moveIndex;
  directions = other.directions;
  return *this;
}

AvailableMove &AvailableMove::operator=(AvailableMove &&other) {
  moveIndex = other.moveIndex;
  directions = std::move(other.directions);
  return *this;
}

int AvailableMove::getInd() { return moveIndex; }

const QList<Direction> &AvailableMove::getDirs() { return directions; }

void AvailableMove::addDir(Direction dir) { directions.push_back(dir); }

bool AvailableMove::isEmpty() { return directions.size() != 0; }

int AvailableMove::size() { return directions.size(); }

void AvailableMove::eraseDirection(int ind) {
  directions.erase(directions.begin() + ind);
}

void AvailableMove::setInd(int ind) { moveIndex = ind; }

int AvailableMove::getIndX() const { return moveIndex % 8; }

int AvailableMove::getIndY() const { return moveIndex / 8; }
