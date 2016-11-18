#ifndef GOMOKU_CORE_MONTE_CARLO_
#define GOMOKU_CORE_MONTE_CARLO_

#include <random>

#include "gomoku/core/board.h"
#include "gomoku/core/feature.h"

namespace gomoku {

// A model that estimates the value of each move. Must be thread-safe.
// TODO: this will need metrics.
class McCell {
 public:
  virtual ~McCell() {}

  // Returns the matrix of estimated victory probabilities.
  virtual FeatureMatrix WinProb() const = 0;

  // Updates the model with a game result.
  virtual void Update(Eigen::Vector2i move, float score) = 0;
};

// A cell that estimates the victory probability.
class McLinearCell : public McCell {
 public:
  McLinearCell(FeatureMatrix win_prob, float learning_rate);

  FeatureMatrix WinProb() const override { return win_prob_; }
  void Update(Eigen::Vector2i move, float score) override;

 private:
  FeatureMatrix win_prob_;
  const float learning_rate_;
};

// A cell that estimates the victory probability.
class McLogisticCell : public McCell {
 public:
  McLogisticCell(FeatureMatrix win_prob, float learning_rate);

  FeatureMatrix WinProb() const override { return win_prob_; }
  void Update(Eigen::Vector2i move, float score) override;

 private:
  FeatureMatrix win_prob_;
  const float learning_rate_;
};

// Picks the best possible legal move.
Eigen::Vector2i SampleGreedy(const FeatureMatrix& win_prob,
                             const FeatureMatrix& legal_moves);

// Picks a move using the softmax distribution.
Eigen::Vector2i SampleSoftmax(const FeatureMatrix& win_prob,
                              const FeatureMatrix& legal_moves,
                              float temperature,
                              std::mt19937_64* rng);

}  // namespace gomoku

#endif  // GOMOKU_CORE_MONTE_CARLO_
