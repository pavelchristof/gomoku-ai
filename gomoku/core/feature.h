#ifndef GOMOKU_CORE_FEATURE_H
#define GOMOKU_CORE_FEATURE_H

#include <map>
#include <random>

#include "gomoku/core/board.h"

namespace gomoku {

// A matrix of features, one for each board field.
using FeatureMatrix = Eigen::Matrix<
    float, Board::kWidth, Board::kHeight, Eigen::ColMajor>;

// A function that builds a feature from a board.
using Feature = std::function<FeatureMatrix(const Board&)>;

// A collection of named features.
using Features = std::map<std::string, Feature>;

// Builds a matrix that contain 1.0 at positions where the given player has
// a stone and 0 in all other position.
FeatureMatrix StoneMatrix(const Board& board, Player player);

// Samples a position weighted by the given matrix.
template <typename Rng>
Eigen::Vector2i SampleWeightMatrix(
    const FeatureMatrix& weights, Rng* rng) {
  std::discrete_distribution<int> dist(
      weights.data(), weights.data() + weights.size());
  int index = dist(*rng);
  return {index % Board::kWidth, index / Board::kWidth};
}

}  // namespace gomoku

#endif  // GOMOKU_CORE_FEATURE_H
