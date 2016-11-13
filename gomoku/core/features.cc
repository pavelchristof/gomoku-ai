#include "gomoku/core/features.h"

namespace gomoku {

Features DefaultFeatures() {
  Features features;
  features["stone_me"] = [] (const Board& board) {
    return StoneMatrix(board, board.CurrentPlayer());
  };
  features["stone_opp"] = [] (const Board& board) {
    return StoneMatrix(board, kOpponent[board.CurrentPlayer()]);
  };
  features["stone_none"] = [] (const Board& board) {
    return StoneMatrix(board, Player::NONE);
  };
  return features;
}

FeatureMatrix StoneMatrix(const Board& board, Player player) {
  FeatureMatrix matrix;
  Board::Iter([&] (int x, int y) {
    if (board.StoneAt({x, y}) == player)
      matrix(x, y) = 1.0f;
    else
      matrix(x, y) = 0.0f;
  });
  return matrix;
}

}  // namespace gomoku
