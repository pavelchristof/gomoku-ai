#include "gomoku/actors/actor_registry.h"

namespace gomoku {

class RandomActor : public Actor {
 public:
  void GameStarted(Player me, int seed, Metrics*) override {
    board_.Reset();
    rng_.seed(seed);
  }

  void ObserveMove(Eigen::Vector2i move) override {
    board_.ApplyMove(move);
  }

  Eigen::Vector2i ChooseMove(StopSignal) override {
    return board_.UniformlySampleLegalMove(&rng_);
  }

 private:
  Board board_;
  std::mt19937_64 rng_;
};

REGISTER_ACTOR("RandomActor", [] (const ActorConfig&) {
  return std::unique_ptr<Actor>(new RandomActor);
});

}  // namespace gomoku
