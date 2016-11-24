#ifndef GOMOKU_LADDER_WORKER_IMPL_H_
#define GOMOKU_LADDER_WORKER_IMPL_H_

#include "gomoku/ladder/worker.grpc.pb.h"

namespace gomoku {

class WorkerImpl final : public Worker::Service {
 public:
  grpc::Status PlayGame(grpc::ServerContext* context,
                        const PlayGameRequest* request,
                        PlayGameResponse* response) override;
};

}  // namespace gomoku

#endif  // GOMOKU_LADDER_WORKER_IMPL_H_
