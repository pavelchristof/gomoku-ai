#include "gomoku/ladder/ladder.h"

#include "tensorflow/core/lib/gtl/map_util.h"
#include "tensorflow/core/platform/logging.h"

namespace gomoku {

void BasicLadder::LogStats() const {
  LOG(INFO) << "Statistics for ladder " << ladder_spec_.name() << ":";
  for (const auto& actor_spec : ladder_spec_.actor_spec()) {
    const ActorStats* stats = tensorflow::gtl::FindOrNull(stats_,
                                                          actor_spec.name());
    CHECK(stats != nullptr);
    LOG(INFO) << "Actor " << actor_spec.name() << ": "
        << stats->won << "/" << stats->games << " = "
        << (float)stats->won / (float)stats->games;
  }
}

void BasicLadder::PrepareGameRequest(PlayGameRequest* request) {
  std::uniform_int_distribution<int> first_dist(
      0, ladder_spec_.actor_spec().size() - 1);
  std::uniform_int_distribution<int> second_dist(
      0, ladder_spec_.actor_spec().size() - 2);
  int first = first_dist(rng_);
  int second = second_dist(rng_);
  if (second >= first) second += 1;
  request->mutable_first_player()->CopyFrom(ladder_spec_.actor_spec(first));
  request->mutable_second_player()->CopyFrom(ladder_spec_.actor_spec(second));
  request->set_move_time_limit_micros(ladder_spec_.move_time_limit_micros());
  request->set_seed(rng_());
}

void BasicLadder::GameCompleted(const PlayGameRequest& request,
                                const PlayGameResponse& response) {
  std::string winner;
  std::string loser;

  if (response.recording().score() >= 0.5f) {
    winner = request.first_player().name();
    loser = request.second_player().name();
  } else {
    loser = request.first_player().name();
    winner = request.second_player().name();
  }

  stats_[winner].won += 1;
  stats_[winner].games += 1;
  stats_[loser].games += 1;
}

}  // namespace gomoku
