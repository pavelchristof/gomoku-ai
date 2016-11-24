#include "gflags/gflags.h"
#include "gomoku/ladder/worker_impl.h"
#include "grpc++/grpc++.h"
#include "tensorflow/core/platform/logging.h"

DEFINE_string(worker_address, "unix:/tmp/gomoku_worker",
              "Address of the gomoku worker.");

namespace gomoku {

void RunWorker() {
  WorkerImpl worker;
  grpc::ServerBuilder builder;
  builder.AddListeningPort(FLAGS_worker_address,
                           grpc::InsecureServerCredentials());
  builder.RegisterService(&worker);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  LOG(INFO) << "Server listening on " << FLAGS_worker_address;
  server->Wait();
}

}  // namespace gomoku

int main(int argc, char **argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gomoku::RunWorker();
  return 0;
}
