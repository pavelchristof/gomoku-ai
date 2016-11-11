#include "gomoku/board.h"

namespace gomoku {

constexpr PlayerMap<int> kPlayerOffset{0, 1};

bool Board::Inside(Eigen::Vector2i position) {
  return position.x() >= 0 && position.x() < kWidth &&
         position.y() >= 0 && position.y() < kHeight;
}

Player Board::StoneAt(Eigen::Vector2i position) const {
  CHECK(Inside(position));
  int index = (position.x() * kHeight + position.y()) * 2;
  if (fields_[index]) return Player::FIRST;
  if (fields_[index+1]) return Player::SECOND;
  return Player::NONE;
}

void Board::ApplyMove(Eigen::Vector2i move) {
  CHECK(Inside(move));
  int index = (move.x() * kHeight + move.y()) * 2;
  CHECK(!fields_[index]);
  CHECK(!fields_[index + 1]);
  fields_[index + kPlayerOffset[current_player_]] = true;
  current_player_ = kOpponent[current_player_];
  moves_made_++;
}

void Board::RevertMove(Eigen::Vector2i move) {
  CHECK(Inside(move));
  int index = (move.x() * kHeight + move.y()) * 2;
  current_player_ = kOpponent[current_player_];
  CHECK(fields_[index + kPlayerOffset[current_player_]]);
  fields_[index + kPlayerOffset[current_player_]] = false;
  moves_made_--;
}

void Board::Reset() {
  fields_.reset();
  current_player_ = Player::FIRST;
  moves_made_ = 0;
}

bool Board::NoMovePossible() const {
  return moves_made_ == kWidth * kHeight;
}

bool Board::DidWin(Eigen::Vector2i last_move) const {
  CHECK(Inside(last_move));

  // There are one horizontal, one vertical and two diagonal lines where
  // the winning line of 5 stones could be located. We have to check all.
  const Eigen::Vector2i directions[] = {
    {0, 1}, {1, 0},  // vertical and horizontal
    {1, 1}, {1, -1},  // diagonal
  };

  // Counts the number of consecutive stones placed by the player and
  // going in the given direction from the last_move.
  auto CastRay = [&] (const Eigen::Vector2i& direction, Player player) {
    int count = 0;
    while (StoneAtPadded(last_move + (count + 1) * direction) == player)
      count++;
    return count;
  };

  for (const auto& direction : directions) {
    int negative_count = CastRay(-direction, kOpponent[current_player_]);
    int positive_count = CastRay(direction, kOpponent[current_player_]);
    if (positive_count + negative_count >= 4)
      return true;
  }

  return false;
}

}  // namespace gomoku
