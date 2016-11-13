#include "gomoku/core/monte_carlo.h"

namespace gomoku {

McRandomCell::McRandomCell(const Board& board)
    : weights_(StoneMatrix(board, Player::NONE)) {}

Eigen::Vector2i McRandomCell::GreedyMove(std::mt19937_64* rng) const {
  return SampleWeightMatrix(weights_, rng);
}

Eigen::Vector2i McRandomCell::ExploringMove(std::mt19937_64* rng) const {
  return SampleWeightMatrix(weights_, rng);
}

}  // namespace gomoku
