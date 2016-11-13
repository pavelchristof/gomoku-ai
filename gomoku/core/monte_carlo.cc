#include "gomoku/core/monte_carlo.h"

namespace gomoku {

McRandomCell::McRandomCell(FeatureMatrix legal_moves)
  : legal_moves_(std::move(legal_moves)) {}

Eigen::Vector2i McRandomCell::GreedyMove(std::mt19937_64* rng) const {
  return SampleWeightMatrix(legal_moves_, rng);
}

Eigen::Vector2i McRandomCell::ExploringMove(std::mt19937_64* rng) const {
  return SampleWeightMatrix(legal_moves_, rng);
}

McSamplingCell::McSamplingCell(
    FeatureMatrix scores,
    FeatureMatrix legal_moves,
    float learning_rate)
  : scores_(std::move(scores)),
    legal_moves_(std::move(legal_moves)),
    learning_rate_(learning_rate) {}

Eigen::Vector2i McSamplingCell::GreedyMove(std::mt19937_64*) const {
  int x, y;
  Scores().cwiseProduct(LegalMoves()).maxCoeff(&x, &y);
  return {x, y};
}

void McSamplingCell::Update(Eigen::Vector2i move, float score) {
  auto& value = scores_(move.x(), move.y());
  value = value + learning_rate_ * (score - value);
}

McSoftmaxSamplingCell::McSoftmaxSamplingCell(
    FeatureMatrix scores,
    FeatureMatrix legal_moves,
    float learning_rate,
    float temperature)
  : McSamplingCell(std::move(scores), std::move(legal_moves), learning_rate),
    temperature_(temperature) {}

Eigen::Vector2i McSoftmaxSamplingCell::ExploringMove(std::mt19937_64* rng)
    const {
  FeatureMatrix matrix = (Scores() / temperature_)
      .array().exp().matrix()
      .cwiseProduct(LegalMoves());
  auto weights = matrix / matrix.sum();
  return SampleWeightMatrix(weights, rng);
}

}  // namespace gomoku
