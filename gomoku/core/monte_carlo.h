#ifndef GOMOKU_CORE_MONTE_CARLO_
#define GOMOKU_CORE_MONTE_CARLO_

#include <random>

#include "gomoku/core/board.h"
#include "gomoku/core/feature.h"

namespace gomoku {

// Monte Carlo cell that keeps an estimate of the value of each move,
// updates them and decides what moves to make.
class McCell {
 public:
  virtual ~McCell() {}

  virtual Eigen::Vector2i GreedyMove(std::mt19937_64* rng) const = 0;
  virtual Eigen::Vector2i ExploringMove(std::mt19937_64* rng) const = 0;
  virtual void Update(Eigen::Vector2i move, float score) = 0;
};

// A cell that always plays a random move and doesn't keep any statistics.
class McRandomCell : public McCell {
 public:
  McRandomCell(const Board& board);

  Eigen::Vector2i GreedyMove(std::mt19937_64* rng) const override;
  Eigen::Vector2i ExploringMove(std::mt19937_64* rng) const override;
  void Update(Eigen::Vector2i move, float score) override {};

 private:
  FeatureMatrix weights_;
};

}  // namespace gomoku

#endif  // GOMOKU_CORE_MONTE_CARLO_
