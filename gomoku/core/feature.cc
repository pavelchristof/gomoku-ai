#include "gomoku/core/feature.h"

namespace gomoku {

FeatureMatrix StoneMatrix(const Board& board, Player player) {
  FeatureMatrix matrix;
  Board::Iter([&] (int x, int y) {
    if (board.StoneAt({x, y}) == player)
      matrix(x, y) = 1.0f;
    else
      matrix(x, y) = 0.0f;
  });
  return matrix;
}

Eigen::Vector2i SampleWeightMatrix(
    const FeatureMatrix& weights, std::mt19937_64* rng) {
  std::discrete_distribution<int> dist(
      weights.data(), weights.data() + weights.size());
  int index = dist(*rng);
  return {index % Board::kWidth, index / Board::kWidth};
}

}  // namespace gomoku
