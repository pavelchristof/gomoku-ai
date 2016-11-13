#include "gomoku/core/monte_carlo.h"
#include "tensorflow/core/platform/test.h"

namespace gomoku {

TEST(McSoftmaxSamplingCell, GreedyMove) {
  Board board;
  FeatureMatrix scores = FeatureMatrix::Zero();
  McSoftmaxSamplingCell cell(scores, board, 1.0f, 1.0f);
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
  Board board;
  Board::Iter([&] (int x, int y) {
    if (x % 2 == 0) board.ApplyMove({x, y});
  });

  FeatureMatrix scores = FeatureMatrix::Zero();
  McSoftmaxSamplingCell cell(scores, board, 1.0f, 1.0f);
  std::mt19937_64 rng(42);
  for (int i = 0; i < 1000; ++i) {
    ASSERT_EQ(1, cell.ExploringMove(&rng).x() % 2)
        << "Scores(): " << cell.Scores() << "\n"
        << "LegalMoves(): " << cell.LegalMoves();
  }
}

}  // namespace gokomu

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
