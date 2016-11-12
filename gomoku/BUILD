load("//tensorflow:tensorflow.bzl", "tf_custom_op_library")
load("//tensorflow/core:platform/default/build_config.bzl", "tf_proto_library")

py_library(
    name = "networks",
    srcs = ["networks.py"],
    deps = [
        "//tensorflow:tensorflow_py",
    ]
)

cc_library(
    name = "board",
    hdrs = ["board.h"],
    srcs = ["board.cc"],
    deps = [
        "//tensorflow/core:framework_headers_lib",
        "//third_party/eigen3",
    ],
)

cc_test(
    name = "board_test",
    srcs = ["board_test.cc"],
    deps = [
        ":board",
        "//tensorflow/core:test",
    ],
)

cc_library(
    name = "features",
    hdrs = ["features.h"],
    srcs = ["features.cc"],
    deps = [
        ":board",
        "//tensorflow/core:framework_headers_lib",
        "//third_party/eigen3",
    ],
)

cc_test(
    name = "features_test",
    srcs = ["features_test.cc"],
    deps = [
        ":features",
        "//tensorflow/core:test",
    ],
)

cc_library(
    name = "actor",
    hdrs = ["actor.h"],
    srcs = ["actor.cc"],
    deps = [
        ":board",
        ":features",
    ],
)

tf_proto_library(
    name = "recording_proto",
    srcs = ["recording.proto"],
    cc_api_version = 2,
)

cc_library(
    name = "supervisor",
    hdrs = ["supervisor.h"],
    srcs = ["supervisor.cc"],
    deps = [
        ":actor",
        ":recording_proto_cc",
        "//tensorflow/core:lib",
        "@gflags//:gflags",
    ],
)

cc_binary(
    name = "self_play",
    srcs = ["self_play.cc"],
    deps = [
        ":actor",
        ":supervisor",
    ]
)

tf_custom_op_library(
    name = "replay_ops_kernel.so",
    srcs = ["replay_ops_kernel.cc"],
    deps = [
        ":board",
        ":features",
        ":recording_proto_cc",
    ]
)

py_library(
    name = "replay_ops",
    srcs = ["replay_ops.py"],
    data = ["replay_ops_kernel.so"],
    deps = [
        "//tensorflow:tensorflow_py",
    ]
)

py_test(
    name = "replay_ops_test",
    srcs = ["replay_ops_test.py"],
    deps = [
        ":replay_ops",
        ":recording_proto_py",
        "//tensorflow:tensorflow_py",
    ]
)

py_binary(
    name = "train",
    srcs = ["train.py"],
    deps = [
        ":networks",
        ":replay_ops",
        "//tensorflow:tensorflow_py",
    ]
)
