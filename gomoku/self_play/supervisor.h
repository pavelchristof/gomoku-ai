#ifndef GOMOKU_SELF_PLAY_SUPERVISOR_H_
#define GOMOKU_SELF_PLAY_SUPERVISOR_H_

#include <future>
#include <memory>
#include <thread>

#include "gomoku/actors/actor_registry.h"
#include "gomoku/core/board.h"
#include "gomoku/core/actor.h"
#include "gomoku/core/recording.pb.h"
#include "tensorflow/core/lib/io/record_writer.h"

namespace gomoku {

using StopSignalFactory = std::function<StopSignal()>;

// Plays a game between two actors and returns the recording.
std::unique_ptr<Recording> PlayGame(
    PlayerMap<Actor*> actors,
    PlayerMap<StopSignalFactory> make_stop_signal,
    PlayerMap<int> seeds);

class Supervisor {
 public:
  Supervisor(PlayerMap<ActorSpec> actor_specs,
             const std::string& replay_dir)
    : actor_specs_(actor_specs), replay_dir_(replay_dir), next_log_(0),
      player_1_won_(0.0f), player_2_won_(0.0f) {
    RotateLog();
  }

  // Finishes the replay log and rotates to a new one.
  void RotateLog();

  void PrintStats();

  // Plays a batch of games in parallel saving their recordings.
  void PlayBatch(int batch_size);

 private:
  std::unique_ptr<tensorflow::WritableFile> file_;
  std::unique_ptr<tensorflow::io::RecordWriter> writer_;
  PlayerMap<ActorSpec> actor_specs_;
  std::string replay_dir_;
  std::mt19937_64 rng_;
  std::uniform_int_distribution<int> gen_seed_;
  int next_log_;
  float player_1_won_;
  float player_2_won_;
};

}  // namespace gomoku

#endif  // GOMOKU_SELF_PLAY_SUPERVISOR_H_
