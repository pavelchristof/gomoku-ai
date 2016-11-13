#include "gomoku/core/feature.h"

namespace gomoku {

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
