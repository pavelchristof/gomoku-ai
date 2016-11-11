#include "gflags/gflags.h"
#include "gomoku/supervisor.h"
#include "gomoku/actor.h"

DEFINE_string(replay_dir, "data/replays/", "Replay storage directory.");
DEFINE_string(model_dir, "data/model/", "Model storage directory.");

namespace gomoku {

void SelfPlay() {
  auto random_actor_factory = [] () {
      return std::unique_ptr<RandomActor>(new RandomActor()); };
  PlayerMap<ActorFactory> actors = {
      random_actor_factory, random_actor_factory};
  Supervisor supervisor(actors, FLAGS_replay_dir);
  while (true) {
    for (int i = 0; i < 100; ++i)
      supervisor.PlayBatch(1000);
    supervisor.RotateLog();
  }
}

}  // namespace gomoku

int main(int argc, char **argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gomoku::SelfPlay();
  return 0;
}
