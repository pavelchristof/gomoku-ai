import tensorflow as tf

from gomoku import networks
from gomoku import replay_ops

tf.app.flags.DEFINE_string('replay_dir', 'data/replays/',
                     'Directory containing the replay files for training.')
tf.app.flags.DEFINE_string('model_dir', 'data/model/',
                     'Directory where the model snapshots will be saved.')
FLAGS = tf.app.flags.FLAGS

def input():
  replay_files = tf.matching_files(FLAGS.replay_dir + "*.tfrecords")
  filename_queue = tf.train.string_input_producer(replay_files)
  options = tf.python_io.TFRecordOptions(
      tf.python_io.TFRecordCompressionType.ZLIB)
  reader = tf.TFRecordReader(options=options)
  key, record_string = reader.read(filename_queue)
  return replay_ops.decode_replays(record_string)

def model(features, scores):
  with tf.variable_scope('value') as value_scope:
    predicted_scores = networks.value(features)
    tf.contrib.losses.sigmoid_cross_entropy(predicted_scores, scores)
    total_loss = tf.contrib.losses.get_total_loss()
    train_op = tf.contrib.layers.optimize_loss(
        loss=total_loss,
        global_step=tf.contrib.framework.get_global_step(),
        learning_rate=0.01,
        optimizer='SGD')
    return predicted_scores, total_loss, train_op

def main(_):
  config = tf.contrib.learn.RunConfig(
      save_summary_steps=1,
      save_checkpoints_secs=300)
  estimator = tf.contrib.learn.Estimator(
      model_fn=model, model_dir=FLAGS.model_dir)
  estimator.fit(input_fn=input)

if __name__ == '__main__':
  tf.app.run()
