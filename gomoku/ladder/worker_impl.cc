#include "gomoku/ladder/worker_impl.h"

#include "gomoku/actors/actor_registry.h"
#include "tensorflow/core/platform/env.h"

namespace gomoku {

StopSignal NewStopSignal(uint64 deadline_micros) {
  auto time_start = tensorflow::Env::Default()->NowMicros();
  return [=] () {
    auto time_now = tensorflow::Env::Default()->NowMicros();
    return time_now - time_start >= deadline_micros;
  };
}

grpc::Status WorkerImpl::PlayGame(grpc::ServerContext* context,
                                  const PlayGameRequest* request,
                                  PlayGameResponse* response) {
  PlayerMap<std::unique_ptr<Actor>> actors = {
    ActorRegistry::Global()->Create(
        request->first_player().type(), request->first_player().config()),
    ActorRegistry::Global()->Create(
        request->second_player().type(), request->second_player().config())
  };
  std::mt19937_64 rng(request->seed());

  for (auto player : kPlayers)
    actors[player]->GameStarted(player, rng());

  Board board;
  Eigen::Vector2i move;
  do {
    auto stop_signal = NewStopSignal(request->move_time_limit_micros());
    move = actors[board.CurrentPlayer()]->ChooseMove(stop_signal);
    board.ApplyMove(move);
    for (auto player : kPlayers)
      actors[player]->ObserveMove(move);

    // Record the move.
    auto move_record = response->mutable_recording()->add_move();
    move_record->set_x(move.x());
    move_record->set_y(move.y());
  } while (!board.DidWin(move) && !board.NoMovePossible());

  if (board.DidWin(move)) {
    auto winner = kOpponent[board.CurrentPlayer()];
    PlayerMap<float> scores{1.0f, 0.0f};
    response->mutable_recording()->set_score(scores[winner]);
  } else {
    response->mutable_recording()->set_score(0.0f);
  }

  return grpc::Status::OK;
}

}  // namespace gomoku
