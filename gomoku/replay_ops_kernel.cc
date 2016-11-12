#include "gomoku/board.h"
#include "gomoku/features.h"
#include "gomoku/recording.pb.h"
#include "tensorflow/core/framework/op.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/framework/shape_inference.h"
#include "tensorflow/core/platform/protobuf.h"

using namespace tensorflow;
using shape_inference::DimensionHandle;
using shape_inference::InferenceContext;
using shape_inference::ShapeHandle;

namespace gomoku {

REGISTER_OP("DecodeReplays")
    .Input("records: string")
    .Output("features: float32")
    .Output("scores: float32")
    .SetShapeFn([](InferenceContext* c) {
      // Validate records.
      ShapeHandle records;
      TF_RETURN_IF_ERROR(c->WithRankAtMost(c->input(0), 1, &records));

      DimensionHandle rebatch_dim = c->UnknownDim();
      ShapeHandle board_shape = c->MakeShape({
          rebatch_dim, Board::kWidth, Board::kHeight,
          static_cast<int>(DefaultFeatures().size())});
      c->set_output(0, board_shape);
      ShapeHandle score_shape = c->MakeShape({rebatch_dim});
      CHECK(c->RankKnown(board_shape));
      c->set_output(1, score_shape);
      return Status::OK();
    })
    .Doc(R"doc(
Decode gomoku replays and prepare features for learning.

records: A 1-D tensor of strings, containing serialized Recording protos.
features: A tensor of shape [states, BOARD_WIDTH, BOARD_HEIGHT, FEATURES]
  containing features for every state in every decoded game.
scores: A tensor of shape [states] containing 1.0 if the current player won
  that game or 0.0 otherwise.
)doc");

class DecodeReplaysOp : public OpKernel {
 public:
  explicit DecodeReplaysOp(OpKernelConstruction* context)
    : OpKernel(context), features_(DefaultFeatures()) {}

  void Compute(OpKernelContext* context) override {
    const Tensor* records = nullptr;
    OP_REQUIRES_OK(context, context->input("records", &records));

    // Parse the protos first to determine how many samples we have.
    std::vector<Recording*> recordings;
    protobuf::Arena arena;
    int samples = 0;
    for (int i = 0; i < records->NumElements(); ++i) {
      const string& record = records->flat<string>()(i);
      auto recording = protobuf::Arena::CreateMessage<Recording>(&arena);
      OP_REQUIRES(context, recording->ParseFromString(record),
                  errors::InvalidArgument(
                      "DecodeReplay couldn't deserialize a Recording."));
      recordings.push_back(recording);
      samples += recording->move_size() + 1;
    }

    // Allocate the outputs.
    Tensor* features = nullptr;
    Tensor* scores = nullptr;
    TensorShape features_shape = {samples, Board::kWidth, Board::kHeight,
                                  static_cast<int>(features_.size())};
    TensorShape score_shape = {samples};
    OP_REQUIRES_OK(context, context->allocate_output("features", features_shape,
                                                     &features));
    OP_REQUIRES_OK(context, context->allocate_output("scores", score_shape,
                                                     &scores));

    // Generate the features and scores.
    int i = 0;
    for (auto recording : recordings) {
      Board board;
      WriteOutput(board, recording->score(), features, scores, i++);
      for (const auto& move : recording->move()) {
        board.ApplyMove({move.x(), move.y()});
        WriteOutput(board, recording->score(), features, scores, i++);
      }
    }
  }

 private:
  // Writes features and the score for the given board.
  void WriteOutput(const Board& board, float score, Tensor* features,
                   Tensor* scores, int i) {
    int j = 0;
    for (const auto& named_feature : features_) {
      FeatureMatrix matrix = named_feature.second(board);
      Board::Iter([&] (int x, int y) {
        features->tensor<float, 4>()(i, x, y, j) = matrix(x, y);
      });
      j++;
    }

    // Save the score for the current player.
    PlayerMap<float> player_scores{score, 1.0f - score};
    scores->vec<float>()(i) = player_scores[board.CurrentPlayer()];
  }

  const Features features_;
};

REGISTER_KERNEL_BUILDER(
    Name("DecodeReplays").Device(DEVICE_CPU), DecodeReplaysOp);

}  // namespace gomoku
