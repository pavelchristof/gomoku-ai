#include "gomoku/self_play/supervisor.h"

#include "gflags/gflags.h"
#include "tensorflow/core/lib/io/path.h"
#include "tensorflow/core/lib/strings/stringprintf.h"

DEFINE_uint64(micros_per_move, 1e5, "Microseconds allowed per move.");
DEFINE_int32(max_replay_files, 10, "Maximum number of log files.");

namespace gomoku {

std::unique_ptr<Recording> PlayGame(
    PlayerMap<Actor*> actors,
    PlayerMap<StopSignalFactory> make_stop_signal,
    PlayerMap<int> seeds) {
  for (auto player : kPlayers)
    actors[player]->GameStarted(player, seeds[player]);

  Board board;
  Eigen::Vector2i move;
  std::unique_ptr<Recording> recording(new Recording);
  do {
    auto stop_signal = make_stop_signal[board.CurrentPlayer()]();
    move = actors[board.CurrentPlayer()]->ChooseMove(stop_signal);
    board.ApplyMove(move);
    for (auto player : kPlayers)
      actors[player]->ObserveMove(move);

    // Record the move.
    auto move_record = recording->add_move();
    move_record->set_x(move.x());
    move_record->set_y(move.y());
  } while (!board.DidWin(move) && !board.NoMovePossible());

  if (board.DidWin(move)) {
    auto winner = kOpponent[board.CurrentPlayer()];
    PlayerMap<float> scores{1.0f, 0.0f};
    recording->set_score(scores[winner]);
  } else {
    recording->set_score(0.0f);
  }
  return recording;
}

void Supervisor::RotateLog() {
  writer_.reset();
  file_.reset();

  if (next_log_ - FLAGS_max_replay_files >= 0) {
    auto old_filename = tensorflow::strings::Printf(
        "replay%d.tfrecords", next_log_ - FLAGS_max_replay_files);
    auto old_filepath = tensorflow::io::JoinPath(replay_dir_, old_filename);
    LOG(INFO) << "deleting old replay log " << old_filepath;
    LOG(INFO) << tensorflow::Env::Default()->DeleteFile(old_filepath);
  }

  auto filename = tensorflow::strings::Printf(
      "replay%d.tfrecords", next_log_++);
  auto filepath = tensorflow::io::JoinPath(replay_dir_, filename);
  LOG(INFO) << "Opening new replay log " << filepath;
  TF_CHECK_OK(tensorflow::Env::Default()->NewWritableFile(filepath, &file_));
  auto options =
      tensorflow::io::RecordWriterOptions::CreateRecordWriterOptions("ZLIB");
  writer_.reset(new tensorflow::io::RecordWriter(file_.get(), options));
}

namespace {

StopSignal TimerFactory() {
  auto time_start = tensorflow::Env::Default()->NowMicros();
  return [=] () {
    auto time_now = tensorflow::Env::Default()->NowMicros();
    return time_now - time_start >= FLAGS_micros_per_move;
  };
}

}  // namespace

void Supervisor::PlayBatch(int batch_size) {
  CHECK(writer_);

  std::vector<std::future<std::unique_ptr<Recording>>> recordings;
  for (int i = 0; i < batch_size; ++i) {
    recordings.push_back(std::async(std::launch::async, [&] () {
      auto actors = actor_factory_.map([] (const ActorFactory& factory) {
          return factory();
      });
      return PlayGame(
          actors.map([] (const std::unique_ptr<Actor>& actor) {
            return actor.get();
          }),
          {TimerFactory, TimerFactory},
          {gen_seed_(rng_), gen_seed_(rng_)});
    }));
  }

  std::string record;
  for (auto& recording : recordings) {
    CHECK(recording.get()->SerializeToString(&record));
    TF_CHECK_OK(writer_->WriteRecord(record));
  }
  TF_CHECK_OK(writer_->Flush());
}

}  // namespace gomoku
