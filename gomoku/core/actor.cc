#include "gomoku/core/actor.h"
#include "gomoku/core/feature.h"

namespace gomoku {

void RandomActor::GameStarted(Player me, int seed) {
  board_.Reset();
  rng_.seed(seed);
}

void RandomActor::ObserveMove(Eigen::Vector2i move) {
  board_.ApplyMove(move);
}

Eigen::Vector2i RandomActor::ChooseMove(StopSignal) {
  auto playable_moves_ = StoneMatrix(board_, Player::NONE);
  return SampleWeightMatrix(playable_moves_, &rng_);
}

}  // namespace gomoku
