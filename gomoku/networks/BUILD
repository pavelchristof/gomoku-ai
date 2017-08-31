load("//tensorflow:tensorflow.bzl", "tf_custom_op_library")

package(default_visibility = ["//visibility:private"])

py_library(
    name = "networks",
    srcs = ["networks.py"],
    deps = [
        "//tensorflow:tensorflow_py",
    ],
    srcs_version = "PY2AND3",
)

cc_library(
    name = "default_features",
    hdrs = ["default_features.h"],
    srcs = ["default_features.cc"],
    deps = [
        "//gomoku/core:feature",
        "//third_party/eigen3",
    ],
)

tf_custom_op_library(
    name = "replay_ops_kernel.so",
    srcs = ["replay_ops_kernel.cc"],
    deps = [
        ":default_features",
        "//gomoku/core:board",
        "//gomoku/core:recording_proto_cc",
    ],
)

py_library(
    name = "replay_ops",
    srcs = ["replay_ops.py"],
    data = ["replay_ops_kernel.so"],
    deps = [
        "//tensorflow:tensorflow_py",
    ],
    srcs_version = "PY2AND3",
)

py_test(
    name = "replay_ops_test",
    srcs = ["replay_ops_test.py"],
    deps = [
        ":replay_ops",
        "//gomoku/core:recording_proto_py",
        "//tensorflow:tensorflow_py",
    ],
    srcs_version = "PY2AND3",
)

py_binary(
    name = "train",
    srcs = ["train.py"],
    deps = [
        ":networks",
        ":replay_ops",
        "//tensorflow:tensorflow_py",
    ],
    srcs_version = "PY2AND3",
)
