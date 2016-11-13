#ifndef GOMOKU_CORE_MONTE_CARLO_
#define GOMOKU_CORE_MONTE_CARLO_

#include <random>

#include "gomoku/core/board.h"
#include "gomoku/core/feature.h"

namespace gomoku {

// Monte Carlo cell that keeps an estimate of the value of each move,
// updates them and decides what moves to make. Must be thread-safe.
// TODO: this will need metrics.
class McCell {
 public:
  virtual ~McCell() {}

  // Generates the best possible move.
  virtual Eigen::Vector2i GreedyMove(std::mt19937_64* rng) const = 0;

  // Generates a move that can be used for exploration.
  virtual Eigen::Vector2i ExploringMove(std::mt19937_64* rng) const = 0;

  // Updates the model with a game result.
  virtual void Update(Eigen::Vector2i move, float score) = 0;
};

// A cell that always plays a random move and doesn't keep any statistics.
class McRandomCell : public McCell {
 public:
  explicit McRandomCell(const Board& board);

  Eigen::Vector2i GreedyMove(std::mt19937_64* rng) const override;
  Eigen::Vector2i ExploringMove(std::mt19937_64* rng) const override;

  void Update(Eigen::Vector2i move, float score) override {};

 private:
  const FeatureMatrix legal_moves_;
};

// A cell that estimates the victory probability.
class McSamplingCell : public McCell {
 public:
  McSamplingCell(
      FeatureMatrix scores,
      const Board& board,
      float learning_rate);

  FeatureMatrix Scores() const { return scores_; }
  FeatureMatrix LegalMoves() const { return legal_moves_; }

  Eigen::Vector2i GreedyMove(std::mt19937_64* rng) const override;

  void Update(Eigen::Vector2i move, float score) override;

 private:
  FeatureMatrix scores_;
  const FeatureMatrix legal_moves_;
  const float learning_rate_;
};

// A logistic cell that samples according to a softmax of logits.
class McSoftmaxSamplingCell : public McSamplingCell {
 public:
  McSoftmaxSamplingCell(
      FeatureMatrix scores,
      const Board& board,
      float learning_rate,
      float temperature);

  Eigen::Vector2i ExploringMove(std::mt19937_64* rng) const override;

 private:
  float temperature_;
};

}  // namespace gomoku

#endif  // GOMOKU_CORE_MONTE_CARLO_
