#include "gomoku/ladder/worker_impl.h"

#include "gomoku/actors/actor_registry.h"
#include "gomoku/core/metrics.h"
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
  PlayerMap<ActorSpec> actor_specs = {
    request->first_player(), request->second_player()
  };
  PlayerMap<std::unique_ptr<Actor>> actors;
  std::mt19937_64 rng(request->seed());

  // Prepare the actors.
  for (Player player : kPlayers) {
    const ActorSpec& spec = actor_specs[player];
    actors[player] = ActorRegistry::Global()->Create(spec.type(),
                                                     spec.config());
    Metrics* actor_metrics = AddMetricGroup(
        response->mutable_metrics(), spec.name());
    actors[player]->GameStarted(player, rng(), actor_metrics);
  }

  // Playout the game.
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

  // Check if the game was a draw.
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
