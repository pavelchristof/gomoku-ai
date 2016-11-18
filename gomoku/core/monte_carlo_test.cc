#include "gomoku/core/monte_carlo.h"
#include "tensorflow/core/platform/test.h"

namespace gomoku {

TEST(McLinearCell, Converges) {
  McLinearCell cell(FeatureMatrix::Constant(0.5f), 1e-5f);
  std::mt19937_64 rng(42);
  const float p = 0.2;
  const Eigen::Vector2i move{1, 2};
  std::bernoulli_distribution dist(p);
  for (int i = 0; i < 500000; ++i) {
    cell.Update(move, dist(rng));
  }
  EXPECT_NEAR(p, cell.WinProb()(move.x(), move.y()), 0.001);
}

TEST(McLogisticCell, Converges) {
  McLogisticCell cell(FeatureMatrix::Constant(0.5f), 1e-5f);
  std::mt19937_64 rng(42);
  const float p = 0.2;
  const Eigen::Vector2i move{1, 2};
  std::bernoulli_distribution dist(p);
  for (int i = 0; i < 1000000; ++i)
    cell.Update(move, dist(rng));
  EXPECT_NEAR(p, cell.WinProb()(move.x(), move.y()), 0.001);
}

TEST(SampleGreedy, PicksTheMaximum) {
  FeatureMatrix win_prob = FeatureMatrix::Zero();
  FeatureMatrix legal_moves = FeatureMatrix::Constant(1.0f);

  Eigen::Vector2i move_1 = {1, 2};
  win_prob(move_1.x(), move_1.y()) = 0.5f;
  EXPECT_EQ(move_1, SampleGreedy(win_prob, legal_moves));

  Eigen::Vector2i move_2 = {2, 1};
  win_prob(move_2.x(), move_2.y()) = 1.0f;
  EXPECT_EQ(move_2, SampleGreedy(win_prob, legal_moves));
}

TEST(SampleSoftmax, AvoidsIllegalMoves) {
  FeatureMatrix win_prob = FeatureMatrix::Constant(0.5f);
  FeatureMatrix legal_moves = FeatureMatrix::Zero();
  Board::Iter([&] (int x, int y) {
    if ((x + y) % 2 != 0) legal_moves(x, y) = 1.0f;
  });

  std::mt19937_64 rng(42);
  for (int i = 0; i < 1000; ++i) {
    auto move = SampleSoftmax(win_prob, legal_moves, 1.0f, &rng);
    ASSERT_EQ(1, (move.x() + move.y()) % 2)
        << "win_prob: " << win_prob << "\n"
        << "legal_moves: " << legal_moves;
  }
}

}  // namespace gokomu

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
