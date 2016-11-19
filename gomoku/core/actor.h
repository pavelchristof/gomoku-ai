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

}  // namespace gomoku

#endif  // GOMOKU_CORE_ACTOR_H_
