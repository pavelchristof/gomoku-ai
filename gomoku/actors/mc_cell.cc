#include "gomoku/actors/mc_cell.h"

namespace gomoku {

McLinearCell::McLinearCell(FeatureMatrix win_prob, float learning_rate)
    : win_prob_(win_prob), learning_rate_(learning_rate) {}

void McLinearCell::Update(Eigen::Vector2i move, float score) {
  auto& value = win_prob_(move.x(), move.y());
  value = value + learning_rate_ * (score - value);
}

McLogisticCell::McLogisticCell(FeatureMatrix win_prob, float learning_rate)
    : win_prob_(win_prob), learning_rate_(learning_rate) {}

void McLogisticCell::Update(Eigen::Vector2i move, float score) {
  auto& value = win_prob_(move.x(), move.y());
  float gradient = score * (1.0f / value) -
      (1.0f - score) * (1.0f / (1.0 - value));
  value = value + learning_rate_ * gradient;
}

// Picks the best possible legal move.
Eigen::Vector2i SampleGreedy(const FeatureMatrix& win_prob,
                             const FeatureMatrix& legal_moves) {
  int x, y;
  win_prob.cwiseProduct(legal_moves).maxCoeff(&x, &y);
  return {x, y};
}

// Picks a move using the softmax distribution.
Eigen::Vector2i SampleSoftmax(const FeatureMatrix& win_prob,
                              const FeatureMatrix& legal_moves,
                              float temperature,
                              std::mt19937_64* rng) {
  FeatureMatrix matrix = (win_prob / temperature)
      .array().exp().matrix()
      .cwiseProduct(legal_moves);
  auto weights = matrix / matrix.sum();
  return SampleWeightMatrix(weights, rng);
}

}  // namespace gomoku
