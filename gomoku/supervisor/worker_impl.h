#ifndef GOMOKU_SUPERVISOR_WORKER_IMPL_H_
#define GOMOKU_SUPERVISOR_WORKER_IMPL_H_

#include "gomoku/supervisor/worker.grpc.pb.h"

namespace gomoku {

class WorkerImpl final : public Worker::Service {
 public:
  grpc::Status PlayGame(grpc::ServerContext* context,
                        const PlayGameRequest* request,
                        PlayGameResponse* response) override;
};

}  // namespace gomoku

#endif  // GOMOKU_SUPERVISOR_WORKER_IMPL_H_
