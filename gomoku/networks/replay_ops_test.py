"""Tests the replay ops."""
from gomoku import replay_ops
from gomoku import recording_pb2
import tensorflow as tf

class ReplayOpsTest(tf.test.TestCase):

  def testDecodeReplays(self):
    """Tests if decode_replays works, without checking the exact features."""
    example_replay = recording_pb2.Recording()
    for i in range(1, 6):
      example_replay.move.add(x=0, y=i)
      example_replay.move.add(x=i, y=0)
    example_replay.score = 1.0
    record = example_replay.SerializeToString()
    features, scores = replay_ops.decode_replays(tf.constant(record))
    with self.test_session() as sess:
      features, scores = sess.run([features, scores])
      self.assertEqual(features.ndim, 4)
      self.assertEqual(features.shape[0], 11)
      self.assertAllEqual(scores, [1.0, 0.0] * 5 + [1.0])


if __name__ == '__main__':
  tf.test.main()
