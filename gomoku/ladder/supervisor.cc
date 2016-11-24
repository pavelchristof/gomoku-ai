#include "gomoku/ladder/supervisor.h"

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
  ladder_->PrepareGameRequest(&call->request);
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

    ladder_->GameCompleted(call->request, call->response);
    games_completed += 1;

    if (games_started < num_games) {
      PlayGame(call->worker, &queue);
      games_started++;
    }
  }
}

}  // namespace gomoku
