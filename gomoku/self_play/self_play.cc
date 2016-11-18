#include "gflags/gflags.h"
#include "gomoku/core/actor.h"
#include "gomoku/self_play/supervisor.h"

DEFINE_string(replay_dir, "data/replays/", "Replay storage directory.");
DEFINE_string(model_dir, "data/model/", "Model storage directory.");

namespace gomoku {

void SelfPlay() {
  PlayerMap<ActorFactory> actors = {
      MonteCarloActorFactory<McLinearCell>(),
      MonteCarloActorFactory<McLogisticCell>()};
  Supervisor supervisor(actors, FLAGS_replay_dir);
  while (true) {
    for (int i = 0; i < 100; i++) {
      supervisor.PlayBatch(8);
      supervisor.PrintStats();
    }
    supervisor.RotateLog();
  }
}

}  // namespace gomoku

int main(int argc, char **argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gomoku::SelfPlay();
  return 0;
}
