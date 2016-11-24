#include "gomoku/supervisor/supervisor.h"

#include "gflags/gflags.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/lib/gtl/map_util.h"

namespace gomoku {

void Supervisor::AddWorker(std::unique_ptr<Worker::Stub> worker) {
  workers_.emplace_back(std::move(worker));
}

namespace {

struct PlayGameCall {
  PlayGameRequest request;
  PlayGameResponse response;
  grpc::ClientContext context;
  grpc::Status status;
  Worker::Stub* worker;
  std::unique_ptr<grpc::ClientAsyncResponseReader<PlayGameResponse>> reader;
};

}  // namespace

void Supervisor::PlayGame(Worker::Stub* worker, grpc::CompletionQueue* queue) {
  PlayGameCall* call = new PlayGameCall;
  PrepareGameRequest(&call->request);
  call->worker = worker;
  call->reader = worker->AsyncPlayGame(&call->context, call->request, queue);
  call->reader->Finish(&call->response, &call->status, (void*)call);
}

// Playes 'num_games' games on the ladder.
void Supervisor::Play(int num_games) {
  grpc::CompletionQueue queue;
  int games_started = 0;
  int games_completed = 0;

  LOG(INFO) << "Sending the initial wave of game requests";
  for (auto& worker : workers_) {
    for (int i = 0; i < concurrent_games_ && games_started < num_games; ++i) {
      PlayGame(worker.get(), &queue);
      games_started++;
    }
  }

  LOG(INFO) << "Entering RPC loop";
  void* tag;
  bool ok;
  while (games_completed < num_games && queue.Next(&tag, &ok)) {
    if (!ok) {
      LOG(ERROR) << "Error in queue.Next()";
      continue;
    }

    std::unique_ptr<PlayGameCall> call((PlayGameCall*)tag);

    if (!call->status.ok()) {
      LOG(ERROR) << "RPC failed: " << call->status.error_message();
      continue;
    }

    CompleteGame(call->request, call->response);
    games_completed += 1;

    if (games_started < num_games) {
      PlayGame(call->worker, &queue);
      games_started++;
    }
  }
}

void Supervisor::PrintStats() const {
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

void Supervisor::PrepareGameRequest(PlayGameRequest* request) {
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

void Supervisor::CompleteGame(const PlayGameRequest& request,
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
