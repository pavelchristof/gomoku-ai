"""Ops used to construct training samples from serialized replays."""
import tensorflow as tf

_replay_ops_module = tf.load_op_library('replay_ops.so')
#decode_replays = _replay_ops_module.decode_replays
