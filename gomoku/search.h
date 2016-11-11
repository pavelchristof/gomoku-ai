// Monte Carlo Tree Search implementation.
#ifndef GOMOKU_SEARCH_H_
#define GOMOKU_SEARCH_H_

#include <random>

#include "gflags/gflags.h"
#include "gomoku/board.h"
#include "gomoku/gammas.h"

DEFINE_int32(max_search_depth, 3, "Maximum search depth.");

namespace gomoku {
namespace search {

class Node {
 public:


 private:
  Gammas gammas_;
  std::optional<float> value_;
  std::unordered_map<Vector, std::unique_ptr<SearchNode>> children_;
  // TODO: mutex
};

class Fiber {
 public:
  void Execute() {
    float score = PlayoutAndEstimateScore();
    for (Node* node : nodes_) {
      node->UpdateGamma(score);
    }
  }

 private:
  float PlayoutAndEstimateScore() {
    Node* node = nodes_[0];
    while (nodes_.size() < max_search_depth) {
      Vector move;
      std::tie (move, node) = node->Sample(rng_);
      moves_.push_back(move);
      nodes_.push_back(node);
      board_.ApplyMove(move);
      if (board_.DidWin(move))
        return 1.0f * board_.CurrentPlayer();
    }
    return node->Value(board_);
  }

  int depth_;
  Board board_;
  std::vector<Node*> nodes_;
  std::vector<Vector> moves_;
  std::mt19937_64 rng_;
};

// Parallel Monte Carlo Tree Search implementation.
template <typename Policy, typename Value>
class MonteCarloTreeSearch {
 public:

  void ApplyMove(Vector move) {
    board_.ApplyMove(move);
    auto new_root = std::move(root_->children_[move]);
    if (!new_root) new_root = new SearchNode(policy_(board_));
    root_ = new_root;
  }

  Gammas EstimateGammas(const std::atomic_bool& stop_signal) {
    return root_.gammas();
  }

 private:
  Board board_;
  std::unique_ptr<SearchNode> root_;
};

// A stateful Monte Carlo Tree Search, not thread-safe.
template <typename Policy, typename Value>
class SearchContext {
 public:
  // Builds a search algorithm with the given policy and value functions.
  Search(Policy policy, Value value, int seed)
    : policy_(std::move(policy)), value_(std::move(value)), rng_(seed) {}

  Vector Search(Board board) {
    return InnerSearch(board, 0);
  }

 private:
  float EstimateScore(board *board, int depth) {
    if (level > FLAGS_max_depth_search) {
      return value_(board);
    }

    Gammas gammas = policy_(*board);
    Vector move = gammas.sample(rng_);
    MoveScope move_scope(board, move);
    if (board->DidWin(move))
      return 1.0f;
    return EstimateScore(board, depth+1);
  }

  Policy policy_;
  Value value_;
  std::mt19937_64 rng_;
};

}  // namespace search
}  // namespace gomoku

#endif  // GOMOKU_SEARCH_H_
