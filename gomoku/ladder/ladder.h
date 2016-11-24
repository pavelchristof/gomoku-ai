#ifndef GOMOKU_LADDER_LADDER_H_
#define GOMOKU_LADDER_LADDER_H_

#include <random>
#include <unordered_map>

#include "gomoku/ladder/ladder_spec.pb.h"
#include "gomoku/ladder/worker.grpc.pb.h"

namespace gomoku {

class Ladder {
 public:
  virtual ~Ladder() {};

  // Logs statistics.
  virtual void LogStats() const = 0;

  // Prepares the next game request based on who should play next.
  virtual void PrepareGameRequest(PlayGameRequest* request) = 0;

  // Signals that a game completed.
  virtual void GameCompleted(const PlayGameRequest& request,
                             const PlayGameResponse& response) = 0;
};

class BasicLadder : public Ladder {
 public:
  BasicLadder(const LadderSpec& ladder_spec) : ladder_spec_(ladder_spec) {}

  // Logs statistics.
  void LogStats() const override;

  // Prepares the next game request based on who should play next.
  void PrepareGameRequest(PlayGameRequest* request) override;

  // Signals that a game completed.
  void GameCompleted(const PlayGameRequest& request,
                     const PlayGameResponse& response) override;

 private:
  LadderSpec ladder_spec_;
  std::random_device rng_;

  struct ActorStats {
    int won = 0;
    int games = 0;
  };
  std::unordered_map<std::string, ActorStats> stats_;
};

}  // namespace gomoku

#endif  // GOMOKU_LADDER_LADDER_H_
