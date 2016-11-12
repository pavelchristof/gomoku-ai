"""Neural Networks used to represent the value functions."""
import tensorflow as tf
from tensorflow.contrib import layers

def Policy(board):
  """Builds a policy network.

  The policy network predicts the expected value (win chance) of actions for a
  particular state.

  Args:
    board: A 4D tensor of the shape [batch, width, height, features] that
      contains the features for each board position.
  Returns:
    logits: A 3D tensor of the shape [batch, width, height] that for each board
      position contains the logits of the probability that making a move there
      leads to victory.
  """
  net = layers.conv2d(board, 128, [9, 9], scope='conv9_1')
  net = layers.conv2d(net, 64, [9, 9], scope='conv9_2')
  net = layers.conv2d(net, 1, [9, 9], scope='conv9_3')
  return net

def Value(board):
  """Builds a value network.

  The policy network predicts the expected value (win chance) of a state,
  assuming we make the best move.

  Args:
    board: A 4D tensor of the shape [batch, width, height, features] that
      contains the features for each board position.
  Returns:
    logits: A 1D tensor of the shape [batch] that contains the logits of the
      probability that the state leads to victory.
  """
  net = layers.conv2d(board, 128, [9, 9], scope='conv9_1')
  net = layers.conv2d(net, 64, [9, 9], scope='conv9_2')
  net = layers.conv2d(net, 16, [9, 9], scope='conv9_3')
  net = layers.flatten(net)
  net = layers.fully_connected(net, 512)
  net = layers.dropout(net)
  net = layers.fully_connected(net, 1)
  net = tf.squeeze(net, squeeze_dims=[1])
  return net
