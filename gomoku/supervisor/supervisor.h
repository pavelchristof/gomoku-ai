#ifndef GOMOKU_SUPERVISOR_SUPERVISOR_H
#define GOMOKU_SUPERVISOR_SUPERVISOR_H

#include <random>
#include <string>
#include <unordered_map>

#include "gomoku/supervisor/ladder_spec.pb.h"
#include "gomoku/supervisor/worker.grpc.pb.h"

namespace gomoku {

struct ActorStats {
  int won = 0;
  int games = 0;
};

class Supervisor {
 public:
  Supervisor(const LadderSpec& ladder_spec, int concurrent_games)
      : ladder_spec_(ladder_spec), concurrent_games_(concurrent_games) {}

  // Adds a worker to the pool.
  // TODO: grpc has some load-balancing built in. There is no need for more
  // then one worker.
  void AddWorker(std::unique_ptr<Worker::Stub> worker);

  // Playes 'num_games' games on the ladder.
  void Play(int num_games);

  // Prints the actor statistics.
  void PrintStats() const;

 private:
  void PlayGame(Worker::Stub* worker, grpc::CompletionQueue* queue);
  void PrepareGameRequest(PlayGameRequest* request);
  void CompleteGame(const PlayGameRequest& request,
                    const PlayGameResponse& response);

  std::vector<std::unique_ptr<Worker::Stub>> workers_;
  LadderSpec ladder_spec_;
  std::unordered_map<std::string, ActorStats> stats_;
  int concurrent_games_;
  std::random_device rng_;
};

}  // namespace gomoku

#endif  // GOMOKU_SUPERVISOR_SUPERVISOR_H
