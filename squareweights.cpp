#include "squareweights.h"

SquareWeights::SquareWeights() {
  weights[0] = {20, -3, 11, 8, 8, 11, -3, 20};
  weights[1] = {-3, -7, -4, 1, 1, -4, -7, -3};
  weights[2] = {11, -4, 2, 2, 2, 2, -4, 11};
  weights[3] = {8, 1, 2, -3, -3, 2, 1, 8};
  weights[4] = {8, 1, 2, -3, -3, 2, 1, 8};
  weights[5] = {11, -4, 2, 2, 2, 2, -4, 11};
  weights[6] = {-3, -7, -4, 1, 1, -4, -7, -3};
  weights[7] = {20, -3, 11, 8, 8, 11, -3, 20};
}

int SquareWeights::getWeight(int x, int y) const { return weights[x][y]; }
