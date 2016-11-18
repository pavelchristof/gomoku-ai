#include "gomoku/core/actor.h"
#include "gomoku/core/feature.h"

#include <random>

namespace gomoku {

class RandomActor : public Actor {
 public:
  void GameStarted(Player me, int seed) override {
    board_.Reset();
    rng_.seed(seed);
  }

  void ObserveMove(Eigen::Vector2i move) override {
    board_.ApplyMove(move);
  }

  Eigen::Vector2i ChooseMove(StopSignal) override {
    auto playable_moves_ = StoneMatrix(board_, Player::NONE);
    return SampleWeightMatrix(playable_moves_, &rng_);
  }

 private:
  Board board_;
  std::mt19937_64 rng_;
};

ActorFactory RandomActorFactory() {
  return [] () {
    return std::unique_ptr<Actor>(new RandomActor());
  };
}

}  // namespace gomoku
