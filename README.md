gomoku-ai
=========

This is an AI for gomoku (early WIP!). The code is contained in gomoku/

It's supposed to be Monte Carlo Tree Search bootstrapped with policy/value
neural networks pre-trained with self-play.

What are all these files and commits
====================================

This is based on the TensorFlow repo. It is the easiest (the only?) way
to build and link to the TensorFlow C++ API with bazel.

Requirements
============

You need to install bazel v0.4+. All the other libraries are here.

How to build
============

First you have to configure TensorFlow:

```bash
$> ./configure
```

Next, try to build gomoku with all its dependencies (and go for a walk when
TensorFlow builds):

```bash
$> bazel -c opt build gomoku:all
```

How to use
==========

Crude self-play and value network training is implemented.

To start self-play do:
```bash
$> ./bazel-bin/gomoku/self_play
```

This will write replays to "data/replays/", rotating if there are too many.
Currently self-play launches random actors. Basic monte carlo is next on the
TODO list.

To start training do:
```bash
$> ./bazel-bin/gomoku/train
```

This will read replays from "data/replays/" and write model checkpoints and
TensorBoard summaries to "data/model/". Training runs but needs a lot more work!
