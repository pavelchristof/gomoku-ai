gomoku-ai
=========

Gomoku AI with neural networks and self-play (instensive WIP!).

This is (will be) a small, powerful and scalable implementation of Monte Carlo
Tree Search bootstrapped with policy and value neural networks trained with
self-play, inspired by AlphaGo. It is written in a mix of C++ and Python, using
TensorFlow. Simplicity is favored, but performance is not sacrificed.

Gomoku is a more advanced variant of tic-tac-toe. The game is played on a 19x19
board on which players put down stones in turns. The first player to get 5
stones in a row (vertically, horizontally or diagonally) wins.

The plan
========

The current status is:

  * Self-play plays random games and records them.
  * The training pipeline reads these file, constructs features and trains
    a convolutional value network.

The TODO list is:

  * Prepare GPU training, CPU is too slow.
  * Implement basic Monte Carlo search to get better training data.
  * Compare how the network learns on random games vs Monte Carlo search.
  * Design some better evaluation strategy, perhaps an ELO ranking.
  * Plug the value network into the Monte Carlo at some depth.
  * Implement MCTS, try out random games vs the value network.
  * Train the policy network, integrate with MCTS.
  * Get more compute power.
  * ???
  * State of the art Gomoku AI.

Directory structure
===================

The code is contained entirely in gomoku/, other directories belong to
TensorFlow.

gomoku/
  core/ - game rules, actor interface, replay format, common utilities
  networks/ - neural networks and training
  self_play/ - self-play implementation

Requirements
============

You need to install bazel v0.4+. You need CUDA, cuDNN and gcc 5 for GPU
training. All the other libraries are here.

How to build
============

First you have to configure TensorFlow:

```bash
$> ./configure
```

Next, build gomoku with all its dependencies. Add the '--config=cuda' option
if you want to use GPU:

```bash
$> bazel build -c opt --config=cuda gomoku/self_play:self_play gomoku/networks:train
```
It takes a while to build TensorFlow. If bazel is using too much cpu or
memory add '--local_resources=MEMORY,CPU,1.0' to the command.

How to use
==========

To start self-play do:
```bash
$> ./bazel-bin/gomoku/self_play/self_play
```

This will write replays to "data/replays/", rotating if there are too many.


To start training do:
```bash
$> ./bazel-bin/gomoku/networks/train
```

This will read replays from "data/replays/", train a value network and write
model checkpoints and TensorBoard summaries to "data/model/".
