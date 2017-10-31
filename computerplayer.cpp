#include "computerplayer.h"

ComputerPlayer::ComputerPlayer(Board &bb) : b(&bb) {
  initializeAvailableMoves();
}

void ComputerPlayer::initChips(Chip user, Chip comp) {
  userChip = user;
  computerChip = comp;
  initializeAvailableMoves();
}

bool ComputerPlayer::computerCanPutChip(int ind) {
  return availableMoves.contains(ind);
}

void ComputerPlayer::setDifficulty(int val) { difficulty = val; }

void ComputerPlayer::updateAvailableMoves() {
  int ind = 0;
  while ((!availableMoves.empty()) && (ind != availableMoves.size())) {
    if (!b->areThereChips((availableMoves.begin() + ind).key(),
                          (availableMoves.begin() + ind).value(), computerChip,
                          userChip)) {
      availableMoves.erase(availableMoves.begin() + ind);
    } else
      ++ind;
  }
}

void ComputerPlayer::initializeAvailableMoves() {
  availableMoves.clear();
  b->initializeAvailableMoves(computerChip, availableMoves);
}

int ComputerPlayer::canPutChipRight(int ind) {
  if (ind % 7) {
    int newInd = ind;
    int boundary = 8 * (ind / 8 + 1) - 1;
    do {
      ++newInd;
    } while ((newInd <= boundary) && (b->getChip(newInd) == userChip));
    return newInd <= boundary ? newInd : -1;
  } else
    return -1;
}

int ComputerPlayer::canPutChipLeft(int ind) {
  if (ind % 8) {
    int newInd = ind;
    int boundary = 8 * (ind / 8);
    do {
      --newInd;
    } while ((newInd >= boundary) && (b->getChip(newInd) == userChip));
    return newInd >= boundary ? newInd : -1;
  } else
    return -1;
}

int ComputerPlayer::canPutChipUp(int ind) {
  if (ind > 7) {
    int newInd = ind;
    int boundary = ind % 8;
    do {
      newInd -= 8;
    } while ((newInd >= boundary) && (b->getChip(newInd) == userChip));
    return newInd >= boundary ? newInd : -1;
  } else
    return -1;
}

int ComputerPlayer::canPutChipDown(int ind) {
  if (ind < 56) {
    int newInd = ind;
    int boundary = 56 + ind % 8;
    do {
      newInd += 8;
    } while ((newInd <= boundary) && (b->getChip(newInd) == userChip));
    return newInd <= boundary ? newInd : -1;
  } else
    return -1;
}

void ComputerPlayer::moveMade(int ind) {
  availableMoves.erase(availableMoves.find(ind));
}
