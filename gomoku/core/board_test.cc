#include "gomoku/core/board.h"
#include "tensorflow/core/platform/test.h"

namespace gomoku {

TEST(Board, DetectsWin) {
  Board board;
  for (int i = 0; i < 4; ++i) {
    board.ApplyMove({0, i});
    EXPECT_EQ(board.CurrentPlayer(), Player::SECOND);
    EXPECT_FALSE(board.DidWin({0, i}));
    board.ApplyMove({1, i});
    EXPECT_EQ(board.CurrentPlayer(), Player::FIRST);
    EXPECT_FALSE(board.DidWin({0, i}));
  }
  board.ApplyMove({0, 4});
  EXPECT_TRUE(board.DidWin({0, 4}));
}

TEST(Board, RevertsMoves) {
  Eigen::Vector2i moves[] = {{2, 3}, {5, 2}, {3, 8}, {2, 1}, {6, 8}};
  Board board;
  board.ApplyMove({1, 1});
  const Board board_copy = board;
  for (auto move : moves)
    board.ApplyMove(move);
  std::reverse(std::begin(moves), std::end(moves));
  for (auto move : moves)
    board.RevertMove(move);
  EXPECT_EQ(board, board_copy);
}

TEST(Board, SampleAvoidsIllegalMoves) {
  Board board;
  Board::Iter([&] (int x, int y) {
    if ((x + y) % 2 == 0) board.ApplyMove({x, y});
  });

  std::mt19937_64 rng(42);
  for (int i = 0; i < 1000; ++i) {
    auto move = board.UniformlySampleLegalMove(&rng);
    ASSERT_EQ(1, (move.x() + move.y()) % 2);
  }
}

}  // namespace gokomu

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
