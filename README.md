gomoku-ai
=========

This is an AI for gomoku (early WIP!). The code is contained in gomoku/

It's supposed to be Monte Carlo Tree Search bootstrapped with policy/value
neural networks pre-trained with self-play.

What are all these files and commits
====================================

This is based on the TensorFlow repo. It is simple the easiest (the only?) way
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

Only self-play is in a runnable state (and very crude).

To start self-play do:
```bash
$> ./bazel-bin/gomoku/self_play --alsologtostderr
```

This will write replays to "data/replays/", rotating if there are too many.
