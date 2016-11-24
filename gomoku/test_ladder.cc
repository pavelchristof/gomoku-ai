#include "gflags/gflags.h"
#include "gomoku/actors/actor_config.h"
#include "gomoku/ladder/supervisor.h"
#include "gomoku/ladder/worker.grpc.pb.h"
#include "grpc++/grpc++.h"

DEFINE_string(worker_address, "unix:/tmp/gomoku_worker",
              "Address of the gomoku worker.");

namespace gomoku {

LadderSpec TestLadderSpec() {
  LadderSpec spec;
  spec.set_name("TestLadder");
  spec.set_move_time_limit_micros(1e2);

  ActorSpec* random_actor = spec.add_actor_spec();
  random_actor->set_name("random_actor");
  random_actor->set_type("RandomActor");

  ActorSpec* mc_linear_actor = spec.add_actor_spec();
  mc_linear_actor->set_name("mc_linear_actor");
  mc_linear_actor->set_type("MonteCarloActor");
  AddConfigItem(mc_linear_actor->mutable_config(), "cell", "linear");

  ActorSpec* mc_logistic_actor = spec.add_actor_spec();
  mc_logistic_actor->set_name("mc_logistic_actor");
  mc_logistic_actor->set_type("MonteCarloActor");
  AddConfigItem(mc_logistic_actor->mutable_config(), "cell", "logistic");

  return spec;
}

void TestLadder() {
  Supervisor supervisor(TestLadderSpec(),  /*concurrent_games=*/8);
  auto channel = grpc::CreateChannel(
      FLAGS_worker_address, grpc::InsecureChannelCredentials());
  supervisor.AddWorker(Worker::NewStub(channel));
  for (int i = 0; i < 100; ++i) {
    supervisor.Play(64);
    supervisor.PrintStats();
  }
}

}  // namespace gomoku

int main(int argc, char **argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gomoku::TestLadder();
  return 0;
}
