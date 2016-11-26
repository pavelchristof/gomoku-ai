#include "gomoku/actors/actor_registry.h"
#include "gomoku/actors/actor_config.h"
#include "gomoku/actors/mc_cell.h"
#include "gomoku/core/metrics.h"

namespace gomoku {

template <typename Cell>
class MonteCarloActor : public Actor {
  void GameStarted(Player me, int seed, Metrics* metrics) override {
    board_.Reset();
    rng_.seed(seed);
    metrics_ = metrics;
  }
  void ObserveMove(Eigen::Vector2i move) override {
    board_.ApplyMove(move);
  }

  Eigen::Vector2i ChooseMove(StopSignal stop_signal) override {
    Cell cell{FeatureMatrix::Constant(0.5f), 1e-3f};

    // Simulate games until the time runs out.
    int iterations = 0;
    while (!stop_signal()) {
      Board board = board_;
      const Eigen::Vector2i exploring_move = SampleSoftmax(
          cell.WinProb(), StoneMatrix(board, Player::NONE), 1.0f, &rng_);
      board.ApplyMove(exploring_move);
      Eigen::Vector2i move = exploring_move;
      while (!board.DidWin(move)) {
        move = board.UniformlySampleLegalMove(&rng_);
        board.ApplyMove(move);
      }
      Player winner = kOpponent[board.CurrentPlayer()];
      float score = winner == board_.CurrentPlayer() ? 1.0f : 0.0f;
      cell.Update(exploring_move, score);
      iterations++;
    }
    AddMetricMean(metrics_, "iterations", board_.MovesMade(), iterations);

    // Pick the best move.
    FeatureMatrix win_prob = cell.WinProb();
    Eigen::Vector2i move = SampleGreedy(
        cell.WinProb(), StoneMatrix(board_, Player::NONE));
    AddMetricMean(metrics_, "expected_score", board_.MovesMade(),
                  win_prob(move.x(), move.y()));

    return move;
  }

 private:
  Board board_;
  std::mt19937_64 rng_;
  Metrics* metrics_;
};

REGISTER_ACTOR("MonteCarloActor", [] (const ActorConfig& config) {
  std::string cell;
  CHECK(GetConfigString(config, "cell", &cell));
  if (cell == "linear")
    return std::unique_ptr<Actor>(new MonteCarloActor<McLinearCell>());
  if (cell == "logistic")
    return std::unique_ptr<Actor>(new MonteCarloActor<McLogisticCell>());
  LOG(FATAL) << "Unknown cell " << cell;
});

}  // namespace gomoku
