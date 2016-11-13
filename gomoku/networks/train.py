import os.path

import tensorflow as tf
from tensorflow.python.framework import ops
from tensorflow.python.client import timeline

from gomoku.networks import networks
from gomoku.networks import replay_ops

tf.app.flags.DEFINE_string('replay_dir', 'data/replays/',
                     'Directory containing the replay files for training.')
tf.app.flags.DEFINE_string('model_dir', 'data/model/',
                     'Directory where the model snapshots will be saved.')
tf.app.flags.DEFINE_string(
    'timeline_dir', 'data/timelines/',
    'If tracing is enabled the timelines will be written there.')
tf.app.flags.DEFINE_boolean('trace', False,
                            'Whether performance tracing should be enabled.')
tf.app.flags.DEFINE_integer(
    'batch_size', 256,
    'How many game position should be in a single batch.')
FLAGS = tf.app.flags.FLAGS

tf.logging.set_verbosity(tf.logging.INFO)


class TimelineHook(tf.train.SessionRunHook):
  """Traces the ops and saves the timelines to a directory."""

  def __init__(self, timeline_dir):
    self._timeline_dir = timeline_dir
    self._counter = 0

  def before_run(self, run_context):
    return tf.train.SessionRunArgs([], should_trace=True)

  def after_run(self, run_context, run_values):
    tl = timeline.Timeline(run_values.run_metadata.step_stats)
    data = tl.generate_chrome_trace_format()
    path = os.path.join(self._timeline_dir,
                        'timeline_{}.json'.format(self._counter))
    with open(path, 'w') as f:
      f.write(data)
    self._counter += 1


def input():
  with tf.name_scope('input'):
    # Scan the available replay files and put them in a queue.
    # TODO: support rotating logs
    replay_files = tf.train.match_filenames_once(
        os.path.join(FLAGS.replay_dir, "*.tfrecords"))
    filename_queue = tf.train.string_input_producer(
        replay_files,
        capacity=20,
        shuffle=True,
        name="filename_queue")

    # Read single records from the files.
    options = tf.python_io.TFRecordOptions(
        tf.python_io.TFRecordCompressionType.ZLIB)
    reader = tf.TFRecordReader(options=options)
    key, record_string = reader.read(filename_queue, name="read_record")

    # Decode each replay into a batch of training features, one for each
    # game position in the replay.
    features, scores = replay_ops.decode_replays(
        record_string, name="decode_replays")
    game_length = tf.size(scores)
    tf.summary.scalar('game_length', game_length)

    # Rebatch into constant size batches.
    return tf.train.shuffle_batch(
        [features, scores],
        batch_size=FLAGS.batch_size,
        enqueue_many=True,
        capacity=10000,
        min_after_dequeue=5000,
        num_threads=2,
        name="rebatched_examples")


def score_metrics(predicted_scores, scores):
  """Computes metrics for scores (victory probability)."""
  tf.summary.histogram('predicted_scores', predicted_scores)


def binary_metrics(predicted_labels, labels):
  """Computes metrics for binary labels."""
  accuracy = tf.contrib.metrics.accuracy(predicted_labels, labels)
  exp_moving_average = tf.train.ExponentialMovingAverage(decay=0.99)
  update_op = exp_moving_average.apply([accuracy])
  tf.summary.scalar('accuracy', accuracy)
  tf.summary.scalar('accuracy_moving_average',
                    exp_moving_average.average(accuracy))
  return update_op


def model(features, scores):
  with tf.variable_scope('value') as value_scope:
    # Run the value network.
    predicted_logits = networks.value(features)
    predicted_scores = tf.nn.sigmoid(predicted_logits)

    # Compute the losses, used for training.
    sigmoid_loss = tf.contrib.losses.sigmoid_cross_entropy(
        predicted_logits, scores)
    tf.summary.scalar('sigmoid_loss', sigmoid_loss)
    mean_squared_loss = tf.contrib.losses.mean_squared_error(
        predicted_scores, scores)
    tf.summary.scalar('mean_squared_loss', mean_squared_loss)
    loss = mean_squared_loss

    # Compute the in-training metrics.
    mean_squared_loss = score_metrics(predicted_scores, scores)
    update_metrics_op = binary_metrics(predicted_logits >= 0.0, scores >= 0.5)

    # Use exponentially decaying learning rate.
    global_step = tf.contrib.framework.get_or_create_global_step()
    learning_rate = tf.train.exponential_decay(
        0.04,
        global_step=global_step,
        decay_steps=2000,
        decay_rate=0.94,
        staircase=True)
    tf.summary.scalar('learning_rate', learning_rate)

    # Standard SGD for now.
    train_op = tf.contrib.layers.optimize_loss(
        loss=loss,
        global_step=global_step,
        learning_rate=learning_rate,
        optimizer='SGD')

    all_ops = tf.group(train_op, update_metrics_op)
    return predicted_scores, loss, all_ops


def main(_):
  config = tf.contrib.learn.RunConfig(
      save_summary_steps=100,
      save_checkpoints_secs=300)
  estimator = tf.contrib.learn.Estimator(
      model_fn=model, model_dir=FLAGS.model_dir, config=config)
  monitors = []
  if FLAGS.trace:
    monitors.append(TimelineHook(FLAGS.timeline_dir))
  estimator.fit(input_fn=input, monitors=monitors)

if __name__ == '__main__':
  tf.app.run()
