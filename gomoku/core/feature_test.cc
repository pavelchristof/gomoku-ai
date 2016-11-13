#include "gomoku/core/feature.h"
#include "tensorflow/core/platform/test.h"

namespace gomoku {

TEST(FeatureMatrix, StoneMatrix) {
  Board board;
  board.ApplyMove({1, 2});
  board.ApplyMove({3, 1});

  auto first = StoneMatrix(board, Player::FIRST);
  Board::Iter([&] (int x, int y) {
    if (x == 1 && y == 2) EXPECT_EQ(first(x, y), 1.0f);
    else EXPECT_EQ(first(x, y), 0.0f);
  });

  auto second = StoneMatrix(board, Player::SECOND);
  Board::Iter([&] (int x, int y) {
    if (x == 3 && y == 1) EXPECT_EQ(second(x, y), 1.0f);
    else EXPECT_EQ(second(x, y), 0.0f);
  });

  auto none = StoneMatrix(board, Player::NONE);
  Board::Iter([&] (int x, int y) {
    if (x == 1 && y == 2) EXPECT_EQ(none(x, y), 0.0f);
    else if (x == 3 && y == 1) EXPECT_EQ(none(x, y), 0.0f);
    else EXPECT_EQ(none(x, y), 1.0f);
  });
}

TEST(FeatureMatrix, Sample2x3) {
  std::mt19937_64 rng(42);
  FeatureMatrix weights = FeatureMatrix::Zero();
  weights(2, 3) = 1.0f;
  EXPECT_EQ(SampleWeightMatrix(weights, &rng), Eigen::Vector2i(2, 3))
      << "Sample failed for board: \n" << weights;
}

TEST(FeatureMatrix, Sample3x2) {
  std::mt19937_64 rng(42);
  FeatureMatrix weights = FeatureMatrix::Zero();
  weights(3, 2) = 1.0f;
  EXPECT_EQ(SampleWeightMatrix(weights, &rng), Eigen::Vector2i(3, 2))
      << "Sample failed for board: \n" << weights;
}

}  // namespace gokomu

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
