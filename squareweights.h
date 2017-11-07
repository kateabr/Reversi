#ifndef SQUAREWEIGHTS_H
#define SQUAREWEIGHTS_H

#include <array>

class SquareWeights {
public:
  SquareWeights();
  int getWeight(int x, int y) const;

private:
  std::array<std::array<int, 8>, 8> weights;
};

#endif // SQUAREWEIGHTS_H
