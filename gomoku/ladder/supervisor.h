#ifndef GOMOKU_LADDER_SUPERVISOR_H_
#define GOMOKU_LADDER_SUPERVISOR_H_

#include <random>
#include <string>
#include <unordered_map>

#include "gomoku/core/metrics.h"
#include "gomoku/ladder/ladder.h"
#include "gomoku/ladder/worker.grpc.pb.h"

namespace gomoku {

class Supervisor {
 public:
  Supervisor(Ladder* ladder, MetricCollector* metric_collector,
             int concurrent_games)
      : ladder_(ladder), metric_collector_(metric_collector),
        concurrent_games_(concurrent_games) {}

  // Adds a worker to the pool.
  // TODO: grpc has some load-balancing built in. There is no need for more
  // then one worker.
  void AddWorker(std::unique_ptr<Worker::Stub> worker);

  // Playes 'num_games' games on the ladder.
  void Play(int num_games);

 private:
  void PlayGame(Worker::Stub* worker, grpc::CompletionQueue* queue);

  Ladder* ladder_;
  MetricCollector* metric_collector_;
  std::vector<std::unique_ptr<Worker::Stub>> workers_;
  int concurrent_games_;
};

}  // namespace gomoku

#endif  // GOMOKU_LADDER_SUPERVISOR_H_
