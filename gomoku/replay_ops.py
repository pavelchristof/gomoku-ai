"""Ops used to construct training samples from serialized replays."""
import os.path

import tensorflow as tf

_replay_ops_module = tf.load_op_library(
    os.path.join(tf.resource_loader.get_data_files_path(),
                 'replay_ops_kernel.so'))
decode_replays = _replay_ops_module.decode_replays
