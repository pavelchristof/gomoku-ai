// Data structures used to store the board and make moves.
#ifndef GOMOKU_CORE_BOARD_H_
#define GOMOKU_CORE_BOARD_H_

#include <bitset>
#include <random>
#include <utility>

#include <Eigen/Dense>

#include "gomoku/core/integral_types.h"
#include "tensorflow/core/platform/logging.h"

namespace gomoku {

enum class Player {
  FIRST,
  SECOND,
  // TODO: use optional instead
  NONE,
};

constexpr Player kPlayers[] = {Player::FIRST, Player::SECOND};

// A handy map from players to some values.
template <typename T>
struct PlayerMap {
  T first;
  T second;

  T& operator[] (Player player) {
    if (player == Player::FIRST) return first;
    if (player == Player::SECOND) return second;
    LOG(FATAL) << "PlayerMap[Player::NONE] called";
    __builtin_unreachable();
  }

  const T& operator[] (Player player) const {
    if (player == Player::FIRST) return first;
    if (player == Player::SECOND) return second;
    LOG(FATAL) << "PlayerMap[Player::NONE] called";
    __builtin_unreachable();
  }

  template <typename F>
  auto Map(F&& fn) -> PlayerMap<decltype(fn(first))> {
    return {fn(first), fn(second)};
  }

  template <typename F>
  auto IndexMap(F&& fn) -> PlayerMap<decltype(fn(Player::FIRST, first))> {
    return {fn(Player::FIRST, first), fn(Player::SECOND, second)};
  }
};

// A map from players to the opponent.
constexpr PlayerMap<Player> kOpponent = {Player::SECOND, Player::FIRST};

// Represents a board of Gomoku. The implementation is based on a bitset,
// with 2 bits per field marking if a player had put down a stone there.
class Board {
 public:
  // Size of the board.
  static constexpr int32 kWidth = 19;
  static constexpr int32 kHeight = 19;

  // Executes a function for every field in the board.
  template <typename T>
  static void Iter(T&& function) {
    for (int x = 0; x < kWidth; ++x) {
      for (int y = 0; y < kHeight; ++y) {
        function(x, y);
      }
    }
  }

  // Checks if the position is inside the board.
  static bool Inside(Eigen::Vector2i position);

  // Creates a starting board.
  Board() { Reset(); }

  // Comparison for tests.
  bool operator== (const Board& board) const {
    return fields_ == board.fields_ &&
           current_player_ == board.current_player_ &&
           moves_made_ == board.moves_made_;
  };
  bool operator!= (const Board& board) const {
    return !(*this == board);
  }

  // The player that should make the next move.
  Player CurrentPlayer() const { return current_player_; }

  // The owner of the stone at the given position. The position must be inside
  // the board.
  Player StoneAt(Eigen::Vector2i position) const;

  // The owner of the stone at the given position. If the position is outside
  // the board then Player::NONE is returned.
  Player StoneAtPadded(Eigen::Vector2i position) const {
    if (!Inside(position)) return Player::NONE;
    return StoneAt(position);
  }

  // Puts a stone on the given position.
  void ApplyMove(Eigen::Vector2i move);

  // Reverts the last move by removing the stone on the given position.
  void RevertMove(Eigen::Vector2i move);

  // Resets the board to the starting position.
  void Reset();

  // Whether there is no place to put down a stone. This does not check
  // if a player won.
  bool NoMovePossible() const;

  // Checks if the last move won the game.
  bool DidWin(Eigen::Vector2i last_move) const;

  // Samples a random legal move with uniform distribution.
  Eigen::Vector2i UniformlySampleLegalMove(std::mt19937_64* rng) const;

 private:
  std::bitset<kWidth * kHeight * 2> fields_;
  Player current_player_;
  int moves_made_;
};

}  // namespace gomoku

#endif  // GOMOKU_CORE_BOARD_H_
