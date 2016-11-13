"""Ops used to construct training samples from serialized replays."""
import os.path

import tensorflow as tf
from tensorflow.python.framework import common_shapes
from tensorflow.python.framework import ops

ops.NotDifferentiable('DecodeReplays')
ops.RegisterShape('DecodeReplays')(common_shapes.call_cpp_shape_fn)

_replay_ops_module = tf.load_op_library(
    os.path.join(tf.resource_loader.get_data_files_path(),
                 'replay_ops_kernel.so'))
decode_replays = _replay_ops_module.decode_replays
