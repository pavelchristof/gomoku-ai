import os.path

import tensorflow as tf
from tensorflow.python.client import timeline

from gomoku import networks
from gomoku import replay_ops

tf.app.flags.DEFINE_string('replay_dir', 'data/replays/',
                     'Directory containing the replay files for training.')
tf.app.flags.DEFINE_string('model_dir', 'data/model/',
                     'Directory where the model snapshots will be saved.')
tf.app.flags.DEFINE_string(
    'timeline_dir', 'data/timelines/',
    'If tracing is enabled the timelines will be written there.')
tf.app.flags.DEFINE_boolean('trace', False,
                            'Whether performance tracing should be enabled.')
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
  replay_files = tf.train.match_filenames_once(FLAGS.replay_dir + "*.tfrecords")
  filename_queue = tf.train.string_input_producer(replay_files)
  options = tf.python_io.TFRecordOptions(
      tf.python_io.TFRecordCompressionType.ZLIB)
  reader = tf.TFRecordReader(options=options)
  key, record_string = reader.read(filename_queue)
  features, scores = replay_ops.decode_replays(record_string)
  batch_size = tf.size(scores)
  tf.summary.scalar('batch_size', batch_size)
  return features, scores

def model(features, scores):
  with tf.variable_scope('value') as value_scope:
    predicted_scores = networks.value(features)
    tf.contrib.losses.sigmoid_cross_entropy(predicted_scores, scores)
    total_loss = tf.contrib.losses.get_total_loss()
    train_op = tf.contrib.layers.optimize_loss(
        loss=total_loss,
        global_step=tf.contrib.framework.get_or_create_global_step(),
        learning_rate=0.1,
        optimizer='SGD')
    return predicted_scores, total_loss, train_op

def main(_):
  config = tf.contrib.learn.RunConfig(
      save_summary_steps=3,
      save_checkpoints_secs=300)
  estimator = tf.contrib.learn.Estimator(
      model_fn=model, model_dir=FLAGS.model_dir, config=config)
  monitors = []
  if FLAGS.trace:
    monitors.append(TimelineHook(FLAGS.timeline_dir))
  estimator.fit(input_fn=input, monitors=monitors)

if __name__ == '__main__':
  tf.app.run()
