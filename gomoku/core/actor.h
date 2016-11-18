#ifndef GOMOKU_CORE_ACTOR_H_
#define GOMOKU_CORE_ACTOR_H_

#include <atomic>
#include <memory>
#include <random>

#include "gomoku/core/board.h"

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

class RandomActor : public Actor {
 public:
  void GameStarted(Player me, int seed) override;
  void ObserveMove(Eigen::Vector2i move) override;
  Eigen::Vector2i ChooseMove(StopSignal) override;

 private:
  Board board_;
  std::mt19937_64 rng_;
};

// Factory used to create multiple copies of the same Actor.
using ActorFactory = std::function<std::unique_ptr<Actor>()>;

}  // namespace gomoku

#endif  // GOMOKU_CORE_ACTOR_H_
