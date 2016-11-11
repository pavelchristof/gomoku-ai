#ifndef GOMOKU_SUPERVISOR_H_
#define GOMOKU_SUPERVISOR_H_

#include <future>
#include <memory>
#include <thread>

#include "gomoku/board.h"
#include "gomoku/actor.h"
#include "gomoku/recording.pb.h"
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
  Supervisor(PlayerMap<ActorFactory> actor_factory,
             const std::string& replay_dir)
    : actor_factory_(actor_factory), replay_dir_(replay_dir), next_log_(0) {
    RotateLog();
  }

  // Finishes the replay log and rotates to a new one.
  void RotateLog();

  // Plays a batch of games in parallel saving their recordings.
  void PlayBatch(int batch_size);

 private:
  std::unique_ptr<tensorflow::WritableFile> file_;
  std::unique_ptr<tensorflow::io::RecordWriter> writer_;
  PlayerMap<ActorFactory> actor_factory_;
  std::string replay_dir_;
  std::mt19937_64 rng_;
  std::uniform_int_distribution<int> gen_seed_;
  int next_log_;
};

}  // namespace gomoku

#endif  // GOMOKU_SUPERVISOR_H_
