import tensorflow as tf

from gomoku import networks

tf.app.flags.DEFINE_string('replay_dir', 'data/replays/',
                     'Directory containing the replay files for training.')
tf.app.flags.DEFINE_string('model_dir', 'data/model/',
                     'Directory where the model snapshots will be saved.')
FLAGS = tf.app.flags.FLAGS

def main(argv):
  replay_files = tf.matching_files(FLAGS.replay_dir + "*.tfrecords")
  filename_queue = tf.train.string_input_producer(replay_files)
  options = tf.python_io.TFRecordOptions(
      tf.python_io.TFRecordCompressionType.ZLIB)
  reader = tf.TFRecordReader(options=options)
  key, record_string = reader.read(filename_queue)

  with tf.Session() as sess:
    coord = tf.train.Coordinator()
    threads = tf.train.start_queue_runners(coord=coord)

    print(sess.run(replay_files))
    print(sess.run([key, record_string]))

    coord.request_stop()
    coord.join(threads)

if __name__ == '__main__':
  tf.app.run()
