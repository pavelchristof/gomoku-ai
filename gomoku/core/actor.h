#ifndef GOMOKU_CORE_ACTOR_H_
#define GOMOKU_CORE_ACTOR_H_

#include <atomic>
#include <memory>

#include "gomoku/core/board.h"
#include "gomoku/core/monte_carlo.h"

namespace gomoku {

// A function that returns whether the time for a move is out.
using StopSignal = std::function<bool()>;

class Actor {
 public:
  virtual ~Actor() {}

  // Signals that the game started and the player you are playing as.
  virtual void GameStarted(Player me, int seed) = 0;

  // Called when someone made a move.
  virtual void ObserveMove(Eigen::Vector2i move) = 0;

  // Computes the next move.
  virtual Eigen::Vector2i ChooseMove(StopSignal stop_signal) = 0;
};

// Factory used to create multiple copies of the same Actor.
using ActorFactory = std::function<std::unique_ptr<Actor>()>;

// Creates actors that always make random moves.
ActorFactory RandomActorFactory();

template <typename Cell>
class MonteCarloActor : public Actor {
  void GameStarted(Player me, int seed) override {
    board_.Reset();
    rng_.seed(seed);
  }
  void ObserveMove(Eigen::Vector2i move) override {
    board_.ApplyMove(move);
  }

  Eigen::Vector2i ChooseMove(StopSignal stop_signal) override {
    Cell cell{FeatureMatrix::Constant(0.5f), 1e-5f};
    while (!stop_signal()) {
      Board board = board_;
      const Eigen::Vector2i exploring_move = SampleSoftmax(
          cell.WinProb(), StoneMatrix(board, Player::NONE), 1.0f, &rng_);
      board.ApplyMove(exploring_move);
      Eigen::Vector2i move = exploring_move;
      while (!board.DidWin(move)) {
        // TODO: this is a terribly inefficient way to sample a random move
        move = SampleWeightMatrix(StoneMatrix(board, Player::NONE), &rng_);
        board.ApplyMove(move);
      }
      Player winner = kOpponent[board.CurrentPlayer()];
      float score = winner == board_.CurrentPlayer() ? 1.0f : 0.0f;
      cell.Update(exploring_move, score);
    }
    return SampleGreedy(cell.WinProb(), StoneMatrix(board_, Player::NONE));
  }

 private:
  Board board_;
  std::mt19937_64 rng_;
};

template <typename Cell>
ActorFactory MonteCarloActorFactory() {
  return [] () -> std::unique_ptr<Actor> {
    return std::unique_ptr<Actor>(new MonteCarloActor<Cell>());
  };
}

}  // namespace gomoku

#endif  // GOMOKU_CORE_ACTOR_H_
