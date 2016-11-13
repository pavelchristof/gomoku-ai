#include "gomoku/networks/default_features.h"

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

}  // namespace gomoku
