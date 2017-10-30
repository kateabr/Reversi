#include "computerplayer.h"

ComputerPlayer::ComputerPlayer(Board &bb) {
  b = bb;
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

void ComputerPlayer::updateAvailableMoves(int newChip) {
  int i = newChip % 8;
  int j = newChip / 8;

  int pos;
  if ((pos = canPutChipDown(newChip)) != -1)
    availableMoves.insert(pos);
  if ((pos = canPutChipLeft(newChip)) != -1)
    availableMoves.insert(pos);
  if ((pos = canPutChipRight(newChip)) != -1)
    availableMoves.insert(pos);
  if ((pos = canPutChipUp(newChip)) != -1)
    availableMoves.insert(pos);
}

void ComputerPlayer::initializeAvailableMoves() {
  availableMoves.clear();
  if (computerChip == Chip::Black) {
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

int ComputerPlayer::canPutChipRight(int ind) {
  if (ind % 7) {
    int newInd = ind;
    int boundary = 8 * (ind / 8 + 1) - 1;
    do {
      ++newInd;
    } while ((newInd <= boundary) && (b.getChip(newInd) == userChip));
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
    } while ((newInd >= boundary) && (b.getChip(newInd) == userChip));
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
    } while ((newInd >= boundary) && (b.getChip(newInd) == userChip));
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
    } while ((newInd <= boundary) && (b.getChip(newInd) == userChip));
    return newInd <= boundary ? newInd : -1;
  } else
    return -1;
}

void ComputerPlayer::moveMade(int ind) {
  availableMoves.erase(availableMoves.find(ind));
}
