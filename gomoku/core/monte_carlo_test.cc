#include "gomoku/core/monte_carlo.h"
#include "tensorflow/core/platform/test.h"

namespace gomoku {

TEST(McSoftmaxSamplingCell, GreedyMove) {
  FeatureMatrix scores = FeatureMatrix::Zero();
  FeatureMatrix legal_moves = FeatureMatrix::Constant(1.0f);
  McSoftmaxSamplingCell cell(scores, legal_moves, 1.0f, 1.0f);
  std::mt19937_64 rng(42);

  Eigen::Vector2i move_1 = {1, 2};
  cell.Update(move_1, 0.5);
  for (int i = 0; i < 1000; ++i)
    EXPECT_EQ(move_1, cell.GreedyMove(&rng));

  Eigen::Vector2i move_2 = {2, 1};
  cell.Update(move_2, 1.0);
  for (int i = 0; i < 1000; ++i)
    EXPECT_EQ(move_2, cell.GreedyMove(&rng));
}

TEST(McSoftmaxSamplingCell, ExploringMove) {
  FeatureMatrix legal_moves = FeatureMatrix::Zero();
  Board::Iter([&] (int x, int y) {
    if ((x + y) % 2 != 0) legal_moves(x, y) = 1.0f;
  });

  FeatureMatrix scores = FeatureMatrix::Zero();
  McSoftmaxSamplingCell cell(scores, legal_moves, 1.0f, 1.0f);
  std::mt19937_64 rng(42);
  for (int i = 0; i < 1000; ++i) {
    auto move = cell.ExploringMove(&rng);
    ASSERT_EQ(1, (move.x() + move.y()) % 2)
        << "Scores(): " << cell.Scores() << "\n"
        << "LegalMoves(): " << cell.LegalMoves();
  }
}

}  // namespace gokomu

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
